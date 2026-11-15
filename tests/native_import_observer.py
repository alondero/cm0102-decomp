"""Test-only live adapter for the trusted local external-breakpoint fixture.

Not a production debugger. Only a newly created child can be attached. The
fixture's verified NOP/RET contract supplies stack cleanup and output sizes.
Sibling build receipts detect accidental artifact drift, not malicious
replacement: source, executable, receipt and producer are trusted local inputs.
"""
from __future__ import annotations

import ctypes
import json
from pathlib import Path
import struct
import subprocess
import time
import uuid

from cmre import capture_record as api, env
from cmre.import_observe import ImportObserver, MemoryRead, Snapshot
from native_debug_fixture import SOURCE, DEFINITION, SCENARIOS, _line_with_timeout


SINGLE_STEP = (0x80000004, 0x4000001e)
BREAKPOINT = (api.STATUS_BREAKPOINT, api.STATUS_WX86_BREAKPOINT)
TF = 0x100


def observe_calls(exe: Path, scenario: str, *, detach_at: str | None = None,
                  fail_at: str | None = None, report: dict | None = None, seconds=12) -> dict:
    """Use real caller return-site breakpoints, including recursive/shared sites.

    Early detach disables ONLY the authored end-of-scenario INT3, explicitly
    recorded as fixture control, so the child can finish without a debugger.
    It is not counted as a debugger-inserted breakpoint or a restored byte.
    """
    if scenario not in SCENARIOS or detach_at not in (None, "entry-hit", "step-prepared", "step-stop"):
        raise ValueError("unsupported fixture scenario/detach point")
    if fail_at not in (None, "step-prepared", "step-stop"):
        raise ValueError("unsupported failure injection")
    build = json.loads((exe.parent / "build.json").read_text(encoding="utf-8"))
    if (build.get("source_sha256") != env.digest(SOURCE)
            or build.get("definition_sha256") != env.digest(DEFINITION)
            or build.get("exe_sha256") != env.digest(exe)
            or build.get("external_breakpoints") is not True):
        raise ValueError("requires matching authored external-breakpoint fixture receipt")
    k = api._kernel32()
    k.DebugActiveProcess.argtypes = k.DebugActiveProcessStop.argtypes = (ctypes.wintypes.DWORD,)
    k.DebugSetProcessKillOnExit.argtypes = (ctypes.wintypes.BOOL,)
    k.SuspendThread.argtypes = k.ResumeThread.argtypes = (ctypes.wintypes.HANDLE,)
    k.SuspendThread.restype = k.ResumeThread.restype = ctypes.wintypes.DWORD
    k.GetExitCodeThread.argtypes = (ctypes.wintypes.HANDLE, ctypes.POINTER(ctypes.wintypes.DWORD))
    k.WaitForSingleObject.argtypes = (ctypes.wintypes.HANDLE, ctypes.wintypes.DWORD)
    k.WaitForSingleObject.restype = ctypes.wintypes.DWORD
    k.GetCurrentProcess.restype = ctypes.wintypes.HANDLE
    k.DuplicateHandle.argtypes = (
        ctypes.wintypes.HANDLE, ctypes.wintypes.HANDLE, ctypes.wintypes.HANDLE,
        ctypes.POINTER(ctypes.wintypes.HANDLE), ctypes.wintypes.DWORD,
        ctypes.wintypes.BOOL, ctypes.wintypes.DWORD)
    child = subprocess.Popen([str(exe)], stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                             stderr=subprocess.PIPE)
    result = {} if report is None else report
    result.update(dict(scenario=scenario, steps=[], debug_events=[], fixture_controls=[],
                       cleanup={}, run_id=uuid.uuid4().hex))
    process, event_process, pending = None, None, None
    attached, exited, ready, early = False, False, False, False
    main_tid, attach_tid, command_released = None, None, False
    threads, originals, armed, facts = {}, {}, set(), {}
    generations = {}
    suspended = {}
    stepping, stopped_hit = None, None
    manifest = {}
    obs = ImportObserver(enabled=True, source=dict(
        kind="authored-native-debugger", session=result["run_id"], pid=child.pid,
        executable_sha256=env.digest(exe), source_sha256=env.digest(SOURCE),
        fixture_definition_sha256=env.digest(DEFINITION),
        thread_identity="Generation stamped on receipt from one serialized Windows debug-event stream",
        warning="Authored surrogate call observations; not game captures or API semantics"))

    def checked(ok, operation):
        if not ok:
            raise OSError(ctypes.get_last_error(), operation)

    def duplicate(handle):
        # Debug-event handles can be invalidated by detach/exit. Our own
        # duplicates must survive detach so we can release explicit suspends.
        owned = ctypes.wintypes.HANDLE()
        current = k.GetCurrentProcess()
        checked(k.DuplicateHandle(current, handle, current, ctypes.byref(owned), 0, False, 2),
                "duplicate debug-event handle")
        return owned.value

    def memory(address, size, purpose):
        buffer, count = ctypes.create_string_buffer(size), ctypes.c_size_t()
        ok = k.ReadProcessMemory(process, address, buffer, size, ctypes.byref(count))
        error = None if ok and count.value == size else f"ReadProcessMemory:{ctypes.get_last_error()}"
        return MemoryRead(address, size, buffer.raw[:count.value], purpose, error)

    def exact(address, size):
        read = memory(address, size, "adapter-control")
        if not read.complete:
            raise OSError(read.error)
        return read.data

    def write(address, blob):
        count = ctypes.c_size_t()
        checked(k.WriteProcessMemory(process, address, blob, len(blob), ctypes.byref(count)), "write")
        if count.value != len(blob):
            raise OSError("short breakpoint write")
        checked(k.FlushInstructionCache(process, address, len(blob)), "instruction-cache flush")
        if exact(address, len(blob)) != blob:
            raise OSError("breakpoint readback mismatch")

    def install(address):
        if address not in originals:
            original = exact(address, 1)
            if original == b"\xcc":
                raise RuntimeError("refusing collision with fixture-owned INT3")
            originals[address] = original
        write(address, b"\xcc")
        armed.add(address)

    def restore(address):
        write(address, originals[address])
        armed.discard(address)

    def context(tid):
        ctx = api._Wow64Context()
        ctx.ContextFlags = api.WOW64_CONTEXT_ALL
        checked(k.Wow64GetThreadContext(threads[tid], ctypes.byref(ctx)), "get WOW64 context")
        return ctx

    def set_control(tid, ctx):
        ctx.ContextFlags = 0x00010001
        checked(k.Wow64SetThreadContext(threads[tid], ctypes.byref(ctx)), "set WOW64 control")

    def snapshot(ctx, *, ip=None, reads=()):
        regs = {name: getattr(ctx, name.capitalize()) for name in (
            "eax", "ebx", "ecx", "edx", "esi", "edi", "ebp", "esp", "eip")}
        regs["eflags"] = ctx.EFlags
        regs["debugger_raw_eip"] = ctx.Eip
        if ip is not None:
            regs["eip"] = ip
        return Snapshot(regs, tuple(reads))

    def continue_pending(status=api.DBG_CONTINUE):
        nonlocal pending
        if pending is not None:
            checked(k.ContinueDebugEvent(*pending, status), "continue event")
            pending = None

    def drain_requests():
        for request in obs.breakpoint_requests():
            address = request["address"]
            try:
                if request["operation"] == "arm":
                    install(address)
                else:
                    restore(address)
                obs.acknowledge_breakpoint(address, request["revision"], success=True,
                                           memory=(memory(address, 1, "breakpoint-readback"),))
            except OSError as exc:
                obs.acknowledge_breakpoint(address, request["revision"], success=False, error=str(exc))
                raise

    def resume_others():
        for tid, lease in list(suspended.items()):
            if (threads.get(tid) != lease["handle"]
                    or generations.get(tid) != lease["generation"]):
                raise RuntimeError("refusing resume through a reused thread identity")
            previous = lease["previous"]
            count = k.ResumeThread(lease["handle"])
            if count == 0xffffffff or count != previous + 1:
                raise OSError(ctypes.get_last_error(),
                              f"suspend-count mismatch on thread {tid}: {count} expected {previous + 1}")
            result["cleanup"].setdefault("suspension_releases", []).append(
                dict(tid=tid, generation=lease["generation"],
                     restored_count=previous, observed_count=count))
            del suspended[tid]

    def suspend_one(tid, handle):
        if tid in suspended:
            if (suspended[tid]["handle"] != handle
                    or suspended[tid]["generation"] != generations.get(tid)):
                raise RuntimeError("suspension lease belongs to another thread generation")
            return
        count = k.SuspendThread(handle)
        if count == 0xffffffff:
            error = ctypes.get_last_error()
            exit_code = ctypes.wintypes.DWORD()
            queried = k.GetExitCodeThread(handle, ctypes.byref(exit_code))
            wait = k.WaitForSingleObject(handle, 10)
            if wait == 0 or (queried and exit_code.value != 259):
                result.setdefault("terminated_peers_not_suspended", []).append(
                    dict(tid=tid, generation=generations.get(tid), exit_code=exit_code.value,
                         suspend_error=error, wait_result=wait))
                return  # proven terminated; its exit event may still be queued
            raise OSError(error, f"suspend peer {tid}; exit={exit_code.value}, wait={wait}")
        suspended[tid] = dict(previous=count, handle=handle, generation=generations.get(tid))

    def prepare_step(tid, generation, address, ctx):
        nonlocal stepping, stopped_hit
        if ctx.EFlags & TF:
            raise RuntimeError("fixture unexpectedly supplied TF; refusing to steal trap ownership")
        stepping = dict(tid=tid, generation=generation, address=address,
                        original_flags=ctx.EFlags, executed=False)
        for other, handle in threads.items():
            if other != tid:
                suspend_one(other, handle)
        restore(address)
        ctx.Eip, ctx.EFlags = address, ctx.EFlags | TF
        set_control(tid, ctx)
        stopped_hit = None

    def finish_step():
        nonlocal stepping
        tid, address = stepping["tid"], stepping["address"]
        ctx = context(tid)
        ctx.EFlags = (ctx.EFlags & ~TF) | (stepping["original_flags"] & TF)
        set_control(tid, ctx)
        wanted = address == manifest["entry"] or any(
            site["address"] == address and site["owners"] for site in obs.report()["breakpoints"])
        if wanted:
            install(address)
        result["steps"].append(dict(tid=tid, generation=stepping["generation"], address=address, next_ip=ctx.Eip,
                                     rearmed=wanted, trap_flag_after=context(tid).EFlags & TF,
                                     peers_suspended=len(suspended)))
        resume_others()
        stepping = None

    def drain_cleanup_events():
        """No thread may run while queued traps are normalized and continued."""
        nonlocal pending
        deadline = time.monotonic() + 3
        event = api._DebugEvent()
        while time.monotonic() < deadline:
            if not k.WaitForDebugEvent(ctypes.byref(event), 50):
                if ctypes.get_last_error() != 121:
                    raise OSError(ctypes.get_last_error(), "cleanup event drain")
                return
            tid, code = event.dwThreadId, event.dwDebugEventCode
            if event.dwProcessId != child.pid:
                raise RuntimeError("foreign process during cleanup")
            pending = (child.pid, tid)
            fact = dict(tid=tid, generation=generations.get(tid), code=code)
            result["cleanup"].setdefault("drained_events", []).append(fact)
            status = api.DBG_CONTINUE
            if code == api.EXCEPTION_DEBUG_EVENT:
                record = event.u.Exception.ExceptionRecord
                fact.update(exception_code=record.ExceptionCode, address=record.ExceptionAddress,
                            first_chance=event.u.Exception.dwFirstChance)
                ctx = context(tid)
                fact["raw_context_hex"] = bytes(ctx).hex()
                if record.ExceptionCode in BREAKPOINT and record.ExceptionAddress in originals:
                    # This exception was generated before another thread's
                    # stop/restore. The queued trap still requires an IP rewind.
                    fact["stack"] = memory(ctx.Esp, 20, "queued-trap-stack").report()
                    ctx.Eip = record.ExceptionAddress
                    set_control(tid, ctx)
                    fact["rewound_ip"] = context(tid).Eip
                elif (record.ExceptionCode in SINGLE_STEP and stepping is not None
                      and tid == stepping["tid"]):
                    ctx.EFlags = (ctx.EFlags & ~TF) | (stepping["original_flags"] & TF)
                    set_control(tid, ctx)
                else:
                    status = api.DBG_EXCEPTION_NOT_HANDLED
                    fact["unowned_exception"] = True
            elif code == api.CREATE_THREAD_DEBUG_EVENT:
                threads[tid] = duplicate(event.u.CreateThread.hThread)
                generations.pop(tid, None)
                fact.update(generation=None, identity_status="cleanup-only-unregistered-thread")
                suspend_one(tid, threads[tid])
            elif code == api.EXIT_THREAD_DEBUG_EVENT:
                generations.pop(tid, None)
                handle = threads.pop(tid, None)
                if handle:
                    k.CloseHandle(handle)
                suspended.pop(tid, None)  # no live thread left to resume
            elif code == api.LOAD_DLL_DEBUG_EVENT:
                if event.u.LoadDll.hFile:
                    k.CloseHandle(event.u.LoadDll.hFile)
            elif code == api.EXIT_PROCESS_DEBUG_EVENT:
                raise RuntimeError("process exited during suspended cleanup")
            continue_pending(status)
        raise TimeoutError("cleanup event drain did not quiesce")

    try:
        manifest = json.loads(_line_with_timeout(child.stdout, 5))
        if manifest.get("version") != 1:
            raise ValueError("unknown fixture manifest")
        result["manifest"] = manifest
        process = k.OpenProcess(0x1f0fff, False, child.pid)
        checked(process, "open authored child")
        checked(k.DebugActiveProcess(child.pid), "attach authored child")
        attached = True
        checked(k.DebugSetProcessKillOnExit(False), "preserve child on debugger exit")
        event = api._DebugEvent()
        deadline = time.monotonic() + seconds
        while time.monotonic() < deadline:
            if not k.WaitForDebugEvent(ctypes.byref(event), 50):
                if ctypes.get_last_error() != 121:
                    raise OSError(ctypes.get_last_error(), "wait debug event")
                continue
            tid, code = event.dwThreadId, event.dwDebugEventCode
            if event.dwProcessId != child.pid:
                raise RuntimeError("foreign process debug event")
            pending = (child.pid, tid)
            # Stamp identity at transport ingress, before any deferred dispatch.
            # Never look up today's generation when delivering an old event.
            event_generation = generations.get(tid)
            result["debug_events"].append(dict(tid=tid, generation=event_generation, code=code))
            status = api.DBG_CONTINUE
            if code == api.CREATE_PROCESS_DEBUG_EVENT:
                main_tid = tid
                event_process = duplicate(event.u.CreateProcessInfo.hProcess)
                threads[tid] = duplicate(event.u.CreateProcessInfo.hThread)
                event_generation = generations[tid] = obs.thread_start(tid)
                result["debug_events"][-1]["generation"] = event_generation
                if event.u.CreateProcessInfo.hFile:
                    k.CloseHandle(event.u.CreateProcessInfo.hFile)
            elif code == api.CREATE_THREAD_DEBUG_EVENT:
                threads[tid] = duplicate(event.u.CreateThread.hThread)
                event_generation = generations[tid] = obs.thread_start(tid)
                result["debug_events"][-1]["generation"] = event_generation
            elif code == api.LOAD_DLL_DEBUG_EVENT:
                if event.u.LoadDll.hFile:
                    k.CloseHandle(event.u.LoadDll.hFile)
            elif code == api.EXIT_THREAD_DEBUG_EVENT:
                obs.thread_exit(tid, generation=event_generation,
                                exit_code=struct.unpack_from("<I", bytes(event.u.raw))[0])
                lease = suspended.pop(tid, None)
                if lease is not None:
                    result["cleanup"].setdefault("suspended_thread_exits", []).append(dict(
                        tid=tid, generation=event_generation, previous_count=lease["previous"]))
                generations.pop(tid)
                handle = threads.pop(tid, None)
                if handle:
                    k.CloseHandle(handle)
                drain_requests()
                if ready and tid == attach_tid and not command_released:
                    child.stdin.write(SCENARIOS[scenario].encode("ascii"))
                    child.stdin.flush()
                    command_released = True
            elif code == api.EXIT_PROCESS_DEBUG_EVENT:
                obs.process_exit(exit_code=struct.unpack_from("<I", bytes(event.u.raw))[0])
                exited = True
            elif code == api.EXCEPTION_DEBUG_EVENT:
                record = event.u.Exception.ExceptionRecord
                address, exception = record.ExceptionAddress, record.ExceptionCode
                result["debug_events"][-1].update(
                    exception_code=exception, exception_address=address,
                    exception_flags=record.ExceptionFlags,
                    first_chance=event.u.Exception.dwFirstChance,
                    information=list(record.ExceptionInformation)[:record.NumberParameters])
                raw_ctx = context(tid)
                result["debug_events"][-1].update(
                    wow64_context_hex=bytes(raw_ctx).hex(), context_flags=raw_ctx.ContextFlags)
                if (stepping is not None and tid == stepping["tid"]
                        and event_generation != stepping["generation"]):
                    raise RuntimeError("single-step event has a different thread generation")
                if stepping is not None and exception in SINGLE_STEP and tid == stepping["tid"]:
                    stepping["executed"] = True
                    if detach_at == "step-stop" or fail_at == "step-stop":
                        early = True
                        if fail_at:
                            raise RuntimeError("injected failure at step-stop")
                        break
                    finish_step()
                elif (stepping is not None and exception in BREAKPOINT
                      and address in originals and tid != stepping["tid"]):
                    if tid not in suspended:
                        raise RuntimeError("queued peer trap escaped single-step suspension")
                    # This trap may have been queued before we unpatched the
                    # owner's instruction. Continue it while its thread remains
                    # explicitly suspended; rewind now so it traps afresh only
                    # after the owner finishes and the site is safely rearmed.
                    ctx = context(tid)
                    ctx.Eip = address
                    set_control(tid, ctx)
                    result.setdefault("deferred_peer_traps", []).append(dict(
                        tid=tid, generation=event_generation, address=address, raw_context_hex=bytes(raw_ctx).hex(),
                        rewound_ip=context(tid).Eip))
                elif exception in BREAKPOINT and address in originals:
                    ctx = context(tid)
                    stopped_hit = (tid, address)
                    if address == manifest["entry"]:
                        raw = exact(ctx.Esp, 20)
                        caller, mode, depth, out, token = struct.unpack("<5I", raw)
                        return_code = exact(manifest["return"], 2)
                        if return_code != b"\x90\xc3":
                            raise RuntimeError("fixture NOP/RET contract mismatch")
                        invocation = obs.entry(
                            tid, "authored-fixture-scope", "fixture!fixture_call",
                            snapshot(ctx, ip=address, reads=(
                                memory(ctx.Esp, 4, "return-word"),
                                memory(ctx.Esp + 4, 16, "fixture-stack-arguments"),
                                memory(manifest["return"], 2, "fixture-NOP-RET-basis"),
                                memory(out, 4, "fixture-output-before"))),
                            generation=event_generation,
                            return_site=caller, return_sp=ctx.Esp + 4,
                            return_sp_basis=f"Authored fixture verified NOP C3 at process VA {manifest['return']:#x}")
                        if invocation is None:
                            raise RuntimeError("entry event rejected its stamped generation")
                        facts[invocation] = dict(mode=mode, depth=depth, out=out, token=token)
                        drain_requests()
                        if detach_at == "entry-hit":
                            early = True
                            break
                    else:
                        candidates = [r for r in obs.report()["invocations"]
                                      if r["tid"] == tid and r["state"] == "pending"
                                      and r["thread_generation"] == event_generation
                                      and r["return_site"] == address and r["return_sp"] == ctx.Esp]
                        reads = ()
                        if len(candidates) == 1:
                            fact = facts[candidates[0]["invocation"]]
                            reads = (memory(fact["out"], 4, "fixture-output-after"),)
                        # A shared continuation can also be an ordinary branch
                        # target (fixture depth==0 bypasses the recursive CALL).
                        # Keep the rejected endpoint raw; never fabricate a pair.
                        obs.return_hit(tid, address, snapshot(ctx, ip=address, reads=reads),
                                       generation=event_generation)
                        drain_requests()
                    prepare_step(tid, event_generation, address, ctx)
                    if detach_at == "step-prepared" or fail_at == "step-prepared":
                        early = True
                        if fail_at:
                            raise RuntimeError("injected failure at step-prepared")
                        break
                elif exception in BREAKPOINT and address == manifest["done"]:
                    break  # fixture-owned trap: leave post-INT3 IP unchanged
                elif exception in BREAKPOINT and not ready:
                    if (exact(manifest["entry"], 1) != b"\x90"
                            or exact(manifest["return"], 2) != b"\x90\xc3"
                            or exact(manifest["probe"], 1) != b"\x90"):
                        raise RuntimeError("external fixture bytes do not match source contract")
                    install(manifest["entry"])
                    install(manifest["probe"])
                    ready = True
                    attach_tid = tid
                    # Let the injected attach thread finish before releasing the
                    # fixture. A terminating debugger thread can reject suspend
                    # while still reporting STILL_ACTIVE; never assume it safe.
                    if tid == main_tid:
                        child.stdin.write(SCENARIOS[scenario].encode("ascii"))
                        child.stdin.flush()
                        command_released = True
                else:
                    obs.exception(tid, generation=event_generation, code=exception, address=address,
                                  first_chance=bool(event.u.Exception.dwFirstChance),
                                  snapshot=snapshot(context(tid)))
                    status = api.DBG_EXCEPTION_NOT_HANDLED
            continue_pending(status)
            if exited:
                for handle in threads.values():
                    k.CloseHandle(handle)
                threads.clear()
                if event_process:
                    k.CloseHandle(event_process)
                event_process = None
                break
        else:
            raise TimeoutError("authored observer exceeded deadline")
    finally:
        try:
            if attached and not exited:
                # All normal/injected stop paths hold an outstanding debug event.
                # Async break-in introduces a new thread with its own pending
                # loader trap. This test adapter does not pretend it can safely
                # detach from that partially established stop.
                if pending is None:
                    result["cleanup"]["unavailable"] = "no outstanding stopped event; terminate owned child"
                    obs.detach("safe stopped-event cleanup unavailable")
                    raise RuntimeError(result["cleanup"]["unavailable"])
                if early:
                    # Fixture-only test control: suppress its future owned INT3.
                    # Keep it separate from debugger-byte restoration claims.
                    before = exact(manifest["done"], 1)
                    if before != b"\xcc":
                        raise RuntimeError("unexpected fixture done marker")
                    write(manifest["done"], b"\x90")
                    result["fixture_controls"].append(dict(
                        address=manifest["done"], before_hex="cc", after_hex="90",
                        purpose="allow authored child to complete after in-flight detach"))
                if stopped_hit is not None:
                    tid, address = stopped_hit
                    ctx = context(tid)
                    ctx.Eip = address
                    set_control(tid, ctx)
                if stepping is not None:
                    tid = stepping["tid"]
                    ctx = context(tid)
                    ctx.EFlags = (ctx.EFlags & ~TF) | (stepping["original_flags"] & TF)
                    set_control(tid, ctx)
                    result["cleanup"]["interrupted_step"] = dict(
                        tid=tid, generation=stepping["generation"],
                        executed=stepping["executed"], ip=context(tid).Eip,
                        trap_flag_after=context(tid).EFlags & TF)
                obs.detach("authored driver in-flight detach" if early else "authored driver completed")
                drain_requests()
                for address in list(originals):
                    restore(address)
                result["cleanup"]["restored_bytes"] = [
                    dict(address=address, original_hex=original.hex(),
                         observed_hex=exact(address, 1).hex())
                    for address, original in originals.items()]
                result["cleanup"]["all_debugger_bytes_restored"] = all(
                    exact(address, 1) == original for address, original in originals.items())
                # A different thread can already have an owned INT3 queued.
                # Keep EVERY thread suspended across continuation and detach,
                # drain/rewind those owned traps, then release only our counts.
                for tid, handle in list(threads.items()):
                    suspend_one(tid, handle)
                continue_pending()
                drain_cleanup_events()
                checked(k.DebugActiveProcessStop(child.pid), "detach authored child")
                attached = False
                result["cleanup"]["detached"] = True
                resume_others()
                if event_process:
                    k.CloseHandle(event_process)
                    event_process = None
                for handle in threads.values():
                    k.CloseHandle(handle)
                threads.clear()
                child.stdin.write(b"!")
                child.stdin.flush()
            stdout, stderr = child.communicate(timeout=7)
            result.update(child_returncode=child.returncode, stdout=stdout.decode(), stderr=stderr.decode())
        finally:
            if child.poll() is None:
                child.kill()  # only our own hash-bound authored child
                if attached:
                    continue_pending()
                    deadline = time.monotonic() + 3
                    event = api._DebugEvent()
                    while time.monotonic() < deadline:
                        if k.WaitForDebugEvent(ctypes.byref(event), 50):
                            k.ContinueDebugEvent(event.dwProcessId, event.dwThreadId, api.DBG_CONTINUE)
                            if event.dwDebugEventCode == api.EXIT_PROCESS_DEBUG_EVENT:
                                if not obs.report()["process_exited"]:
                                    obs.process_exit(exit_code=struct.unpack_from("<I", bytes(event.u.raw))[0])
                                break
                child.communicate(timeout=5)
            for handle in threads.values():
                k.CloseHandle(handle)
            if event_process:
                k.CloseHandle(event_process)
            if process:
                k.CloseHandle(process)
            child.stdin.close()
            child.stdout.close()
            child.stderr.close()
            result["protocol"] = obs.report()
            result["fixture_facts"] = facts
            path = exe.parent / f"protocol-{scenario}-{result['run_id']}.json"
            result["evidence_path"] = str(path)
            path.write_text(json.dumps(result, indent=2), encoding="utf-8")
    return result

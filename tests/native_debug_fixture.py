"""Bounded live driver for ONLY the authored native_debuggee fixture.

This deliberately is not the production import-call observer/state machine.
It reuses the recorder's Win64 debug-event/WOW64 declarations, not Recorder
(whose identity policy must continue rejecting this executable).
"""
from __future__ import annotations

import ctypes
import hashlib
import json
import os
from pathlib import Path
import queue
import struct
import subprocess
import threading
import time

from cmre import env
from cmre import capture_record as api

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "tests/fixtures/native_debuggee.c"
DEFINITION = SOURCE.with_suffix(".def")
SCENARIOS = {"ordinary": "o", "nested": "n", "shared": "s", "threads": "t",
             "fault": "f", "unreadable": "u", "thread-exit": "x", "exit-race": "r"}


class Unavailable(RuntimeError):
    """Required platform/tool binary is absent; never a passing execution."""


def build_fixture(directory: Path, *, external_breakpoints: bool = False) -> Path:
    if os.name != "nt" or struct.calcsize("P") != 8:
        raise Unavailable("requires 64-bit Windows Python driving an i386 child")
    for name in ("gcc", "as", "ld", "dlltool"):
        if not Path(env.tool(name)).is_file():
            raise Unavailable(f"missing compiler/link tool: {env.tool(name)}")
    directory.mkdir(parents=True, exist_ok=True)
    obj, lib, exe = (directory / name for name in ("fixture.o", "kernel32.a", "fixture.exe"))
    commands = [
        [env.tool("gcc"), "-m32", "-c", str(SOURCE), "-o", str(obj), "-O0",
         "-std=c11", "-Wall", "-Wextra", "-Werror", "-ffreestanding",
         "-fno-stack-protector", "-fno-asynchronous-unwind-tables"],
        [env.tool("dlltool"), "-m", "i386", "--as", env.tool("as"), "--as-flags", "--32",
         "--kill-at", "-d", str(DEFINITION),
         "-l", str(lib)],
        [env.tool("ld"), "-mi386pe", "--entry", "_start", "--subsystem", "console",
         "--disable-dynamicbase", "-o", str(exe), str(obj), str(lib)],
    ]
    if external_breakpoints:
        commands[0].append("-DFIXTURE_EXTERNAL_BREAKPOINTS")
    receipt = {"source_sha256": env.digest(SOURCE), "definition_sha256": env.digest(DEFINITION),
               "external_breakpoints": external_breakpoints, "commands": [],
               "tools": {name: env.digest(Path(env.tool(name)))
                         for name in ("gcc", "as", "ld", "dlltool")}}
    try:
        for command in commands:
            result = env.run_tool(command, timeout=30)
            receipt["commands"].append({"argv": command, "returncode": result.returncode,
                                        "stdout": result.stdout, "stderr": result.stderr})
            if result.returncode:
                raise RuntimeError(f"fixture build failed: {result.stderr}")
        raw = exe.read_bytes()
        pe = struct.unpack_from("<I", raw, 0x3c)[0]
        if raw[pe:pe + 4] != b"PE\0\0" or struct.unpack_from("<H", raw, pe + 4)[0] != 0x14c:
            raise RuntimeError("compiler did not produce an i386 PE")
        receipt["exe_sha256"] = hashlib.sha256(raw).hexdigest()
        return exe
    finally:
        (directory / "build.json").write_text(json.dumps(receipt, indent=2), encoding="utf-8")


def _line_with_timeout(stream, seconds: float) -> bytes:
    result = queue.Queue()
    # A bounded child lifetime guarantees this reader cannot outlive cleanup.
    thread = threading.Thread(target=lambda: result.put(stream.readline()), daemon=True)
    thread.start()
    try:
        return result.get(timeout=seconds)
    except queue.Empty:
        raise TimeoutError("fixture startup handshake timed out") from None


def observe(exe: Path, scenario: str, *, seconds: float = 10,
            fail_at_done: bool = False, report: dict | None = None) -> dict:
    """Observe markers, retaining raw facts (no inferred API semantics).

    Identity is scoped to our newly-created Popen child; no caller-supplied PID.
    Frames correlate by (tid, entry ESP), never by return-address alone.
    """
    if scenario not in SCENARIOS:
        raise ValueError(scenario)
    build = json.loads((exe.parent / "build.json").read_text(encoding="utf-8"))
    if (build.get("source_sha256") != env.digest(SOURCE)
            or build.get("definition_sha256") != env.digest(DEFINITION)
            or build.get("exe_sha256") != env.digest(exe)
            or build.get("external_breakpoints") is not False):
        raise ValueError("marker driver requires a matching authored fixture build receipt")
    k = api._kernel32()
    k.DebugActiveProcess.argtypes = (ctypes.wintypes.DWORD,)
    k.DebugActiveProcessStop.argtypes = (ctypes.wintypes.DWORD,)
    k.DebugSetProcessKillOnExit.argtypes = (ctypes.wintypes.BOOL,)
    child = subprocess.Popen([str(exe)], stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                             stderr=subprocess.PIPE)
    process = None
    threads, frames, events, exceptions, exits = {}, {}, [], [], []
    attached, exited, pending, armed, ready = False, False, None, False, False
    original = None
    event_process = None
    manifest = {}
    result = {} if report is None else report
    result.update({"scenario": scenario, "events": events, "exceptions": exceptions, "debug_events": [],
                   "thread_exits": exits, "cleanup": {}})

    def checked(ok, operation):
        if not ok:
            raise OSError(ctypes.get_last_error(), operation)

    def read(address, size):
        buf, count = ctypes.create_string_buffer(size), ctypes.c_size_t()
        if not k.ReadProcessMemory(process, address, buf, size, ctypes.byref(count)) or count.value != size:
            return None
        return buf.raw

    def write(address, blob):
        count = ctypes.c_size_t()
        checked(k.WriteProcessMemory(process, address, blob, len(blob), ctypes.byref(count)),
                "WriteProcessMemory")
        if count.value != len(blob):
            raise OSError("short WriteProcessMemory")
        checked(k.FlushInstructionCache(process, address, len(blob)), "FlushInstructionCache")

    def continue_pending(status=api.DBG_CONTINUE):
        nonlocal pending
        if pending is not None:
            checked(k.ContinueDebugEvent(pending[0], pending[1], status), "ContinueDebugEvent")
            pending = None

    try:
        manifest = json.loads(_line_with_timeout(child.stdout, 5))
        if manifest["version"] != 1:
            raise ValueError("unknown fixture contract")
        result["manifest"] = manifest
        result["exe_sha256"] = env.digest(exe)
        process = k.OpenProcess(0x1F0FFF, False, child.pid)
        checked(process, "OpenProcess")
        checked(k.DebugActiveProcess(child.pid), "DebugActiveProcess")
        attached = True
        checked(k.DebugSetProcessKillOnExit(False), "DebugSetProcessKillOnExit")
        deadline = time.monotonic() + seconds
        event = api._DebugEvent()
        while time.monotonic() < deadline:
            if not k.WaitForDebugEvent(ctypes.byref(event), 50):
                error = ctypes.get_last_error()
                if error != 121:  # ERROR_SEM_TIMEOUT
                    raise OSError(error, "WaitForDebugEvent")
                continue
            code, tid = event.dwDebugEventCode, event.dwThreadId
            result["debug_events"].append({"pid": event.dwProcessId, "tid": tid, "code": code,
                                          "exception": event.u.Exception.ExceptionRecord.ExceptionCode
                                          if code == api.EXCEPTION_DEBUG_EVENT else None,
                                          "address": event.u.Exception.ExceptionRecord.ExceptionAddress
                                          if code == api.EXCEPTION_DEBUG_EVENT else None})
            if event.dwProcessId != child.pid:
                raise RuntimeError("received event for another debuggee")
            pending = (event.dwProcessId, tid)
            status = api.DBG_CONTINUE
            if code == api.CREATE_PROCESS_DEBUG_EVENT:
                info = event.u.CreateProcessInfo
                event_process = info.hProcess
                threads[tid] = info.hThread
                if info.hFile:
                    k.CloseHandle(info.hFile)
            elif code == api.CREATE_THREAD_DEBUG_EVENT:
                threads[tid] = event.u.CreateThread.hThread
            elif code == api.LOAD_DLL_DEBUG_EVENT:
                if event.u.LoadDll.hFile:
                    k.CloseHandle(event.u.LoadDll.hFile)
            elif code == api.EXIT_THREAD_DEBUG_EVENT:
                exits.append({"tid": tid, "open_frames": [
                    frame for (owner, _), frame in frames.items() if owner == tid]})
                frames = {key: frame for key, frame in frames.items() if key[0] != tid}
                threads.pop(tid, None)  # debug-event handles close on exit continuation
            elif code == api.EXIT_PROCESS_DEBUG_EVENT:
                exited = True
                result["process_exit_code"] = struct.unpack_from("<I", bytes(event.u.raw))[0]
            elif code == api.EXCEPTION_DEBUG_EVENT:
                record = event.u.Exception.ExceptionRecord
                address, exception = record.ExceptionAddress, record.ExceptionCode
                is_break = exception in (api.STATUS_BREAKPOINT, api.STATUS_WX86_BREAKPOINT)
                if is_break and address in (manifest["entry"], manifest["return"]):
                    context = api._Wow64Context()
                    context.ContextFlags = api.WOW64_CONTEXT_ALL
                    checked(k.Wow64GetThreadContext(threads[tid], ctypes.byref(context)),
                            "Wow64GetThreadContext")
                    raw = read(context.Esp, 20)
                    if raw is None:
                        raise RuntimeError("fixture frame unreadable")
                    caller, mode, depth, out, token = struct.unpack("<5I", raw)
                    kind = "entry" if address == manifest["entry"] else "return"
                    fact = {"kind": kind, "tid": tid, "esp": context.Esp,
                            "caller": caller, "mode": mode, "depth": depth,
                            "out": out, "token": token, "eax": context.Eax,
                            "edx": context.Edx}
                    key = tid, context.Esp
                    if kind == "entry":
                        if key in frames:
                            raise RuntimeError("duplicate fixture frame identity")
                        frames[key] = fact
                    else:
                        entry = frames.pop(key, None)
                        if entry is None or entry["token"] != token:
                            raise RuntimeError("unmatched fixture return")
                        output = read(out, 4)
                        fact["output"] = None if output is None else output.hex()
                        fact["output_status"] = "unreadable" if output is None else "read"
                    events.append(fact)
                elif is_break and address == manifest["done"]:
                    result["open_frames_at_done"] = list(frames.values())
                    if fail_at_done:
                        raise RuntimeError("injected failure at done marker")
                    break
                elif is_break and not ready:
                    # The one attach breakpoint, before releasing the stdin gate.
                    original = read(manifest["probe"], 1)
                    if original != b"\x90":
                        raise RuntimeError("cleanup probe is not its authored NOP")
                    write(manifest["probe"], b"\xcc")
                    armed = True
                    if read(manifest["probe"], 1) != b"\xcc":
                        raise RuntimeError("cleanup probe breakpoint not installed")
                    ready = True
                    child.stdin.write(SCENARIOS[scenario].encode("ascii"))
                    child.stdin.flush()
                else:
                    exceptions.append({"tid": tid, "code": exception, "address": address,
                                       "first_chance": event.u.Exception.dwFirstChance})
                    status = api.DBG_EXCEPTION_NOT_HANDLED
            continue_pending(status)
            if exited:
                threads.clear()  # system closes debug-event handles on exit continuation
                event_process = None
                break
        else:
            raise TimeoutError(f"fixture {scenario} exceeded {seconds}s")
    finally:
        try:
            if attached and not exited:
                if armed:
                    write(manifest["probe"], original)
                    result["cleanup"]["restored"] = read(manifest["probe"], 1) == original
                    armed = False
                # Do not detach with an uncontinued debug event.
                continue_pending()
                checked(k.DebugActiveProcessStop(child.pid), "DebugActiveProcessStop")
                attached = False
                if event_process:
                    k.CloseHandle(event_process)
                    event_process = None
                for handle in threads.values():
                    k.CloseHandle(handle)
                threads.clear()
                result["cleanup"]["detached"] = True
                child.stdin.write(b"!")
                child.stdin.flush()
            stdout, stderr = child.communicate(timeout=5)
            result["child_returncode"] = child.returncode
            result["stdout"], result["stderr"] = stdout.decode(), stderr.decode()
        finally:
            if child.poll() is None:
                child.kill()  # only the authored child this invocation created
                # Termination while attached still requires draining exit events.
                if attached:
                    continue_pending()
                    drain_deadline = time.monotonic() + 3
                    while time.monotonic() < drain_deadline:
                        event = api._DebugEvent()
                        if k.WaitForDebugEvent(ctypes.byref(event), 50):
                            k.ContinueDebugEvent(event.dwProcessId, event.dwThreadId, api.DBG_CONTINUE)
                            if event.dwDebugEventCode == api.EXIT_PROCESS_DEBUG_EVENT:
                                threads.clear()
                                event_process = None
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
            (exe.parent / f"{scenario}-last.json").write_text(
                json.dumps(result, indent=2), encoding="utf-8")
    return result

"""Record function-entry captures from the running pinned game (ADR 0009).

Tooling, not harness: it produces inputs for `cmre/capture.py`'s store and never
decides a test result. The recorder stops the process at a function entry,
emulates that one call with absent pages supplied from the frozen process, and
keeps exactly the pages the call touched.
"""
from __future__ import annotations

import ctypes
import json
import struct
import time
import zlib
from ctypes import wintypes
from pathlib import Path

from . import env
from .capture import PAGE, STACK_WORDS, save

# Trap flag and resume flag belong to the debugger, not to the function's input.
EFLAGS_INPUT_MASK = 0x00000ED5 | 0x400


EXCEPTION_DEBUG_EVENT, CREATE_THREAD_DEBUG_EVENT, CREATE_PROCESS_DEBUG_EVENT = 1, 2, 3
EXIT_THREAD_DEBUG_EVENT, EXIT_PROCESS_DEBUG_EVENT, LOAD_DLL_DEBUG_EVENT = 4, 5, 6
STATUS_WX86_BREAKPOINT, STATUS_BREAKPOINT = 0x4000001F, 0x80000003
DBG_CONTINUE, DBG_EXCEPTION_NOT_HANDLED = 0x00010002, 0x80010001
WOW64_CONTEXT_ALL = 0x00010000 | 0x1 | 0x2 | 0x4 | 0x8  # control, integer, segments, floating point
MEM_COMMIT, PAGE_GUARD, PAGE_NOACCESS = 0x1000, 0x100, 0x01


class _ExceptionRecord(ctypes.Structure):
    _fields_ = [("ExceptionCode", wintypes.DWORD), ("ExceptionFlags", wintypes.DWORD),
                ("ExceptionRecord", ctypes.c_void_p), ("ExceptionAddress", ctypes.c_void_p),
                ("NumberParameters", wintypes.DWORD), ("ExceptionInformation", ctypes.c_uint64 * 15)]


class _ExceptionInfo(ctypes.Structure):
    _fields_ = [("ExceptionRecord", _ExceptionRecord), ("dwFirstChance", wintypes.DWORD)]


class _CreateThread(ctypes.Structure):
    _fields_ = [("hThread", wintypes.HANDLE), ("lpThreadLocalBase", ctypes.c_void_p),
                ("lpStartAddress", ctypes.c_void_p)]


class _CreateProcess(ctypes.Structure):
    _fields_ = [("hFile", wintypes.HANDLE), ("hProcess", wintypes.HANDLE), ("hThread", wintypes.HANDLE),
                ("lpBaseOfImage", ctypes.c_void_p), ("dwDebugInfoFileOffset", wintypes.DWORD),
                ("nDebugInfoSize", wintypes.DWORD), ("lpThreadLocalBase", ctypes.c_void_p),
                ("lpStartAddress", ctypes.c_void_p), ("lpImageName", ctypes.c_void_p),
                ("fUnicode", wintypes.WORD)]


class _LoadDll(ctypes.Structure):
    _fields_ = [("hFile", wintypes.HANDLE), ("lpBaseOfDll", ctypes.c_void_p),
                ("dwDebugInfoFileOffset", wintypes.DWORD), ("nDebugInfoSize", wintypes.DWORD),
                ("lpImageName", ctypes.c_void_p), ("fUnicode", wintypes.WORD)]


class _EventUnion(ctypes.Union):
    _fields_ = [("Exception", _ExceptionInfo), ("CreateThread", _CreateThread),
                ("CreateProcessInfo", _CreateProcess), ("LoadDll", _LoadDll),
                ("raw", ctypes.c_byte * 160)]


class _DebugEvent(ctypes.Structure):
    _fields_ = [("dwDebugEventCode", wintypes.DWORD), ("dwProcessId", wintypes.DWORD),
                ("dwThreadId", wintypes.DWORD), ("u", _EventUnion)]


class _FloatSave(ctypes.Structure):
    _fields_ = [("ControlWord", wintypes.DWORD), ("StatusWord", wintypes.DWORD), ("TagWord", wintypes.DWORD),
                ("ErrorOffset", wintypes.DWORD), ("ErrorSelector", wintypes.DWORD),
                ("DataOffset", wintypes.DWORD), ("DataSelector", wintypes.DWORD),
                ("RegisterArea", ctypes.c_byte * 80), ("Cr0NpxState", wintypes.DWORD)]


class _Wow64Context(ctypes.Structure):
    _fields_ = [("ContextFlags", wintypes.DWORD)] + [(f"Dr{i}", wintypes.DWORD) for i in (0, 1, 2, 3, 6, 7)] + [
        ("FloatSave", _FloatSave)] + [(n, wintypes.DWORD) for n in (
            "SegGs", "SegFs", "SegEs", "SegDs", "Edi", "Esi", "Ebx", "Edx", "Ecx", "Eax",
            "Ebp", "Eip", "SegCs", "EFlags", "Esp", "SegSs")] + [("ExtendedRegisters", ctypes.c_byte * 512)]


class _MemoryInfo(ctypes.Structure):
    _fields_ = [("BaseAddress", ctypes.c_void_p), ("AllocationBase", ctypes.c_void_p),
                ("AllocationProtect", wintypes.DWORD), ("PartitionId", wintypes.WORD),
                ("RegionSize", ctypes.c_size_t), ("State", wintypes.DWORD),
                ("Protect", wintypes.DWORD), ("Type", wintypes.DWORD)]


def _kernel32():
    k = ctypes.WinDLL("kernel32", use_last_error=True)
    k.OpenProcess.restype = k.OpenThread.restype = wintypes.HANDLE
    k.OpenProcess.argtypes = k.OpenThread.argtypes = (wintypes.DWORD, wintypes.BOOL, wintypes.DWORD)
    k.ReadProcessMemory.argtypes = (wintypes.HANDLE, ctypes.c_void_p, ctypes.c_void_p, ctypes.c_size_t,
                                    ctypes.POINTER(ctypes.c_size_t))
    k.WriteProcessMemory.argtypes = k.ReadProcessMemory.argtypes
    k.FlushInstructionCache.argtypes = (wintypes.HANDLE, ctypes.c_void_p, ctypes.c_size_t)
    k.Wow64GetThreadContext.argtypes = k.Wow64SetThreadContext.argtypes = (
        wintypes.HANDLE, ctypes.POINTER(_Wow64Context))
    k.WaitForDebugEvent.argtypes = (ctypes.POINTER(_DebugEvent), wintypes.DWORD)
    k.ContinueDebugEvent.argtypes = (wintypes.DWORD, wintypes.DWORD, wintypes.DWORD)
    k.VirtualQueryEx.argtypes = (wintypes.HANDLE, ctypes.c_void_p, ctypes.POINTER(_MemoryInfo), ctypes.c_size_t)
    k.VirtualQueryEx.restype = ctypes.c_size_t
    k.CloseHandle.argtypes = (wintypes.HANDLE,)
    k.QueryFullProcessImageNameW.argtypes = (wintypes.HANDLE, wintypes.DWORD, wintypes.LPWSTR,
                                             ctypes.POINTER(wintypes.DWORD))
    return k


class Plateau:
    """Ends a round once no new target function has been reached for `seconds`.
    Holiday saturates (two seasons reached little new code), so game time past the
    plateau belongs to a different activity. None or 0 disables the stop."""

    def __init__(self, seconds: float | None, *, start: float):
        self.seconds, self.since, self.best, self.fired = seconds or 0, start, 0, False

    def update(self, now: float, *, reached: int) -> bool:
        if reached > self.best:
            self.best, self.since = reached, now
        self.fired = self.fired or bool(self.seconds) and now - self.since >= self.seconds
        return self.fired

    def reason(self) -> str:
        return f"plateau: no new target function for {round(self.seconds / 60)} min"


class Recorder:
    """Attach to one running pinned game process and capture entry inputs for `targets`.

    The only bytes ever written to the process are a one-byte breakpoint at a
    target entry and the original byte that restores it. Detaching restores
    every entry and leaves the game running.
    """

    def __init__(self, pid: int, targets: list[int], *, per_function: int = 24, target_coverage: float = 0.9,
                 cooldown: float = 0.25, frozen_budget: float = 6.0, log=print):
        from . import difftest, emu, image, inventory
        self.emu, self.difftest = emu, difftest
        self.k = _kernel32()
        self.pid, self.log = pid, log
        self.img = image.primary()
        self.inv = inventory.load()
        self.per_function, self.target_coverage, self.cooldown = per_function, target_coverage, cooldown
        self.targets = [va for va in targets if va in self.inv.functions]
        self.kept: dict[int, list[str]] = {va: [] for va in self.targets}
        self.covered: dict[int, set[int]] = {va: set() for va in self.targets}
        self.hits: dict[int, int] = {va: 0 for va in self.targets}
        # Seconds the game may stay frozen for one function across the session.
        self.frozen_budget = frozen_budget
        self.frozen: dict[int, float] = {va: 0.0 for va in self.targets}
        self.armed: set[int] = set()
        self.rearm_at: dict[int, float] = {}
        self.done: set[int] = set()
        self.threads: dict[int, int] = {}
        self.stats = {"hits": 0, "kept": 0, "no-new-coverage": 0, "unstable": 0, "seconds_frozen": 0.0}
        self.process = self.k.OpenProcess(0x1F0FFF, False, pid)
        if not self.process:
            raise OSError(ctypes.get_last_error(), "cannot open the game process")
        self._check_identity()
        self.machine = emu.Machine(self.img)
        self.machine.pager = self._pager
        self._blocks: set[tuple[int, int]] = set()
        text = self.img.text
        import unicorn
        self.machine.uc.hook_add(unicorn.UC_HOOK_BLOCK, lambda uc, a, s, _: self._blocks.add((a, s)),
                                 begin=text.va, end=text.end - 1)
        self.machine.reset()  # take the pristine snapshot of the file image
        self._writable = [(lo, hi) for lo, hi in self.machine._writable_image]

    # process access ---------------------------------------------------------------------------
    def read(self, address: int, size: int) -> bytes | None:
        buffer = ctypes.create_string_buffer(size)
        done = ctypes.c_size_t()
        if not self.k.ReadProcessMemory(self.process, address, buffer, size, ctypes.byref(done)) or done.value != size:
            return None
        return buffer.raw

    def _write(self, address: int, data: bytes) -> None:
        done = ctypes.c_size_t()
        if not self.k.WriteProcessMemory(self.process, address, data, len(data), ctypes.byref(done)):
            raise OSError(ctypes.get_last_error(), f"cannot write the process at {address:#010x}")
        self.k.FlushInstructionCache(self.process, address, len(data))

    def _check_identity(self) -> None:
        size = wintypes.DWORD(1024)
        name = ctypes.create_unicode_buffer(1024)
        if not self.k.QueryFullProcessImageNameW(self.process, 0, name, ctypes.byref(size)):
            raise OSError(ctypes.get_last_error(), "cannot name the process image")
        self.image_path = Path(name.value)
        if env.digest(self.image_path) != env.TARGET_SHA256:
            raise ValueError(f"{self.image_path} is not the pinned 3.9.68 executable")
        text = self.img.text
        live = self.read(text.va, len(text.raw))
        if live != text.raw:
            raise ValueError("live .text differs from the pinned file at the preferred base; refusing to record")

    def _pager(self, page: int) -> bytes | None:
        # Real DLL images and the top of the address space are outside the function's inputs.
        if not 0x10000 <= page < 0x60000000:
            return None
        return self.read(page, PAGE)

    # breakpoints ------------------------------------------------------------------------------
    def _arm(self, va: int) -> None:
        self._write(va, b"\xCC")
        self.armed.add(va)

    def _disarm(self, va: int) -> None:
        self._write(va, self.img.read(va, 1))
        self.armed.discard(va)

    def _thread(self, tid: int) -> int:
        handle = self.threads.get(tid)
        if not handle:
            handle = self.threads[tid] = self.k.OpenThread(0x1FFFFF, False, tid)
        return handle

    # capture ----------------------------------------------------------------------------------
    def _capture(self, va: int, context: _Wow64Context) -> None:
        emu, machine = self.emu, self.machine
        regs = {"eax": context.Eax, "ecx": context.Ecx, "edx": context.Edx, "ebx": context.Ebx,
                "esp": context.Esp, "ebp": context.Ebp, "esi": context.Esi, "edi": context.Edi,
                "eflags": context.EFlags & EFLAGS_INPUT_MASK | 0x2}
        fpcw = context.FloatSave.ControlWord & 0xFFFF
        raw = self.read(context.Esp + 4, 4 * STACK_WORDS)
        if raw is None:  # the frame ends near the top of the stack mapping
            raw = b""
            for index in range(STACK_WORDS):
                word = self.read(context.Esp + 4 + 4 * index, 4)
                if word is None:
                    break
                raw += word
        stack = list(struct.unpack(f"<{len(raw) // 4}I", raw))
        opts = self.difftest.Options.for_function(va)
        words = (stack + [0] * opts.arg_count)[:opts.arg_count]
        # Live content of writable image pages that differs from the file image.
        live = {}
        for lo, hi in self._writable:
            blob = self.read(lo, hi - lo)
            if blob is None:
                continue
            for offset in range(0, hi - lo, PAGE):
                page = blob[offset:offset + PAGE]
                if page != machine.pristine[lo + offset]:
                    live[lo + offset] = page
        seed = zlib.crc32(struct.pack("<II", va, self.hits[va]))
        arena = bytes(machine.arena_size)
        self._blocks = set()
        case = emu.Case(regs, words, arena, {}, seed, fpcw, None, dict(live))
        result = machine.run(va, case, opts.budget)
        touched = {address & ~(PAGE - 1) for kind, address, size, _ in
                   (e for e in result.events if e[0] != "c")}
        touched |= {(e[1] + e[2] - 1) & ~(PAGE - 1) for e in result.events if e[0] != "c"}
        pages = dict(machine.paged)
        pages.update({page: blob for page, blob in live.items() if page in touched})
        end = va + self.inv.functions[va].body_bytes
        own = {b for address, size in self._blocks for b in range(address, address + size) if va <= b < end}
        first = (result.outcome, result.detail, result.regs)
        machine.pager = None
        try:
            again = machine.run(va, emu.Case(regs, words, arena, {}, seed, fpcw, None, dict(pages)), opts.budget)
        finally:
            machine.pager = self._pager
        if (again.outcome, again.detail, again.regs) != first:
            self.stats["unstable"] += 1
            return
        if self.kept[va] and not own - self.covered[va]:
            self.stats["no-new-coverage"] += 1
            return
        meta = {"recorded_utc": time.strftime("%Y-%m-%dT%H:%M:%SZ", time.gmtime()), "pid": self.pid,
                "hit": self.hits[va], "outcome": result.outcome, "budget": opts.budget,
                "arg_count": opts.arg_count, "esp": context.Esp, "eip": va}
        name = save(va, regs, fpcw, stack, pages, seed, meta)
        self.kept[va].append(name)
        self.covered[va] |= own
        self.stats["kept"] += 1

    def _finished(self, va: int) -> bool:
        size = self.inv.functions[va].body_bytes or 1
        return (len(self.kept[va]) >= self.per_function
                or len(self.covered[va]) / size >= self.target_coverage)

    def _hit(self, va: int, tid: int) -> None:
        started = time.perf_counter()
        self._disarm(va)
        handle = self._thread(tid)
        context = _Wow64Context()
        context.ContextFlags = WOW64_CONTEXT_ALL
        if not self.k.Wow64GetThreadContext(handle, ctypes.byref(context)):
            raise OSError(ctypes.get_last_error(), "cannot read the thread context")
        context.Eip = va
        context.ContextFlags = 0x00010000 | 0x1  # write back the instruction pointer only
        if not self.k.Wow64SetThreadContext(handle, ctypes.byref(context)):
            raise OSError(ctypes.get_last_error(), "cannot restore the instruction pointer")
        self.hits[va] += 1
        self.stats["hits"] += 1
        try:
            self._capture(va, context)
        except (ValueError, OSError) as exc:
            self.stats["errors"] = self.stats.get("errors", 0) + 1
            self.log(f"fn_{va:08x} capture error: {exc}")
        self.frozen[va] += time.perf_counter() - started
        if self._finished(va) or self.frozen[va] >= self.frozen_budget:
            self.done.add(va)
        else:
            # Spread later captures over the session; consecutive calls are near-duplicates.
            self.rearm_at[va] = time.monotonic() + self.cooldown * 2 ** min(self.hits[va], 6)
        self.stats["seconds_frozen"] += time.perf_counter() - started

    # session ----------------------------------------------------------------------------------
    def run(self, seconds: float, status_path: Path | None = None, plateau_seconds: float | None = None) -> dict:
        k = self.k
        if not k.DebugActiveProcess(self.pid):
            raise OSError(ctypes.get_last_error(), "cannot attach to the game process")
        k.DebugSetProcessKillOnExit(False)
        event = _DebugEvent()
        deadline, last_status, exited = time.monotonic() + seconds, 0.0, False
        plateau = Plateau(plateau_seconds, start=time.monotonic())
        try:
            pending = list(self.targets)
            while time.monotonic() < deadline and not exited and not plateau.fired:
                if k.WaitForDebugEvent(ctypes.byref(event), 50):
                    code, status = event.dwDebugEventCode, DBG_CONTINUE
                    if code == CREATE_PROCESS_DEBUG_EVENT:
                        info = event.u.CreateProcessInfo
                        if info.lpBaseOfImage != self.img.base:
                            raise ValueError("the game is not loaded at its preferred base")
                        self.threads[event.dwThreadId] = info.hThread
                        if info.hFile:
                            k.CloseHandle(info.hFile)
                    elif code == CREATE_THREAD_DEBUG_EVENT:
                        self.threads[event.dwThreadId] = event.u.CreateThread.hThread
                    elif code == EXIT_THREAD_DEBUG_EVENT:
                        self.threads.pop(event.dwThreadId, None)
                    elif code == LOAD_DLL_DEBUG_EVENT:
                        if event.u.LoadDll.hFile:
                            k.CloseHandle(event.u.LoadDll.hFile)
                    elif code == EXIT_PROCESS_DEBUG_EVENT:
                        exited = True
                    elif code == EXCEPTION_DEBUG_EVENT:
                        record = event.u.Exception.ExceptionRecord
                        address = record.ExceptionAddress or 0
                        if record.ExceptionCode == STATUS_WX86_BREAKPOINT and address in self.armed:
                            self._hit(address, event.dwThreadId)
                        elif record.ExceptionCode in (STATUS_WX86_BREAKPOINT, STATUS_BREAKPOINT) and not (
                                self.img.text.va <= address < self.img.text.end):
                            pass  # the attach breakpoint inside the system loader
                        else:
                            status = DBG_EXCEPTION_NOT_HANDLED
                    k.ContinueDebugEvent(event.dwProcessId, event.dwThreadId, status)
                now = time.monotonic()
                for _ in range(min(len(pending), 256)):  # arm gradually; each write is one byte
                    self._arm(pending.pop())
                for va in [va for va, when in self.rearm_at.items() if when <= now]:
                    del self.rearm_at[va]
                    self._arm(va)
                if now - last_status > 5:
                    last_status = now
                    plateau.update(now, reached=sum(1 for va in self.targets if self.hits[va]))
                    if status_path is not None:
                        self._status(status_path)
        finally:
            if plateau.fired:
                self.stats["stopped"] = plateau.reason()
            if not exited:
                for va in list(self.armed):
                    self._disarm(va)
                k.DebugActiveProcessStop(self.pid)
            if status_path is not None:
                self._status(status_path)
        return self.summary()

    def summary(self) -> dict:
        sizes = {va: self.inv.functions[va].body_bytes or 1 for va in self.targets}
        reached = [va for va in self.targets if self.hits[va]]
        good = [va for va in self.targets if len(self.covered[va]) / sizes[va] >= 0.6]
        return {**self.stats, "targets": len(self.targets), "reached": len(reached),
                "reached_bytes": sum(sizes[va] for va in reached),
                "byte_coverage_at_least_0.6": len(good), "bytes_at_least_0.6": sum(sizes[va] for va in good),
                "armed": len(self.armed), "done": len(self.done)}

    def _status(self, path: Path) -> None:
        sizes = {va: self.inv.functions[va].body_bytes or 1 for va in self.targets}
        detail = {f"fn_{va:08x}": {"hits": self.hits[va], "captures": self.kept[va],
                                   "byte_coverage": round(len(self.covered[va]) / sizes[va], 4)}
                  for va in self.targets if self.hits[va]}
        temporary = path.with_suffix(".tmp")
        temporary.write_text(json.dumps({"summary": self.summary(), "functions": detail}, indent=1), encoding="utf-8")
        temporary.replace(path)

"""Test prototype: bounded endpoint observations of TWO real OS imports.

Trusted local fixture only. No PID option, game recorder, API model, transcript
conversion or replay support. Raw records belong exclusively in ignored local/.
"""
from __future__ import annotations

import ctypes
import json
import os
from pathlib import Path
import struct
import uuid

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
import pefile

from cmre import env, capture_record as api
from cmre.import_observe import MemoryRead
from native_debug_fixture import Unavailable
from native_import_observer import observe_calls

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "tests/fixtures/native_import_results.c"
DEFINITION = SOURCE.with_suffix(".def")
DOCS = {
    "HeapAlloc": "https://learn.microsoft.com/en-us/windows/win32/api/heapapi/nf-heapapi-heapalloc",
    "ReadFile": "https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-readfile",
    "CreateFileA": "https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilea",
    "stdcall": "https://learn.microsoft.com/en-us/cpp/cpp/stdcall?view=msvc-170",
    "x86": "https://learn.microsoft.com/en-us/cpp/cpp/argument-passing-and-naming-conventions?view=msvc-170",
}


def bounded_range(reader, address, size, purpose, *, limit=64):
    """Keep requested range, cap the actual read, retain partial bytes/errors."""
    if (type(address) is not int or type(size) is not int or
            not 0 <= address <= 0xffffffff or size < 0 or address + size > 0x100000000):
        raise ValueError("invalid output range")
    if type(limit) is not int or not 0 < limit <= 4096:
        raise ValueError("invalid observation limit")
    acquired = reader(address, min(size, limit), purpose)
    errors = ([f"observation-limit:{limit}"] if size > limit else [])
    if acquired.error:
        errors.append(acquired.error)
    return MemoryRead(address, size, acquired.data, purpose, ";".join(errors) or None)


def output_observations(reader, name, args, eax):
    """Explicit two-prototype allowlist; uncertainty is retained, not repaired."""
    gaps = []
    if name == "HeapAlloc":
        if len(args) != 3:
            raise ValueError("HeapAlloc requires evidence for exactly three stack arguments")
        if eax == 0:
            return (), ("allocation-failed",)
        return (bounded_range(reader, eax, args[2], "HeapAlloc.buffer"),), ()
    if name != "ReadFile" or len(args) != 5:
        raise ValueError("unsupported native output prototype")
    _, buffer, requested, count_pointer, overlapped = args
    reads = []
    if overlapped:
        gaps.append("overlapped-unsupported")
    if eax == 0:
        gaps.append("api-error-or-pending")
    if count_pointer:
        count = bounded_range(reader, count_pointer, 4, "ReadFile.byte-count")
        reads.append(count)
        if count.complete:
            actual = struct.unpack("<I", count.data)[0]
            if actual < requested:
                gaps.append("short-read")
            elif actual > requested:
                gaps.append("byte-count-exceeds-request")
        else:
            gaps.append("byte-count-unreadable")
    else:
        gaps.append("byte-count-pointer-null")
    # Observe the DECLARED range, not a guessed count when count observation fails.
    # Unwritten tails are raw observations, never labelled returned file bytes.
    reads.append(bounded_range(reader, buffer, requested, "ReadFile.buffer"))
    return tuple(reads), tuple(gaps)


def verify_caller(name, fields, exact, imports):
    """Native caller bytes + PE IAT import binding, NOT name-only ABI inference."""
    argc = {"HeapAlloc": 3, "ReadFile": 5}[name]
    if imports.get(fields["iat"]) != f"KERNEL32.dll!{name}":
        raise ValueError("manifest IAT is not the PE's declared import")
    start, call, continuation = (fields[key] for key in ("pushes", "call", "continuation"))
    if not 0 < continuation - start <= 64 or continuation != call + 6:
        raise ValueError("unbounded or unsupported native caller span")
    raw = exact(start, continuation - start)
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(raw, start))
    expected_call = b"\xff\x15" + struct.pack("<I", fields["iat"])
    if (len(instructions) != argc + 1 or instructions[-1].address != call or
            instructions[-1].bytes != expected_call or
            any(insn.mnemonic != "push" or len(insn.operands) != 1 or insn.operands[0].size != 4
                for insn in instructions[:-1])):
        raise ValueError("caller must push each u32 argument then CALL [verified IAT]")
    return dict(argument_count=argc, stack_argument_bytes=4 * argc,
                expected_endpoint_delta=4 + 4 * argc, caller_bytes_hex=raw.hex(),
                instructions=[dict(address=i.address, bytes_hex=i.bytes.hex(),
                                   mnemonic=i.mnemonic, operands=i.op_str) for i in instructions],
                docs={key: DOCS[key] for key in (name, "stdcall", "x86")},
                limitation="Documented stdcall expectation, verified by native caller and observed endpoint SP; "
                           "callee RET path not instruction-traced")


def build_native_results(directory):
    if os.name != "nt" or struct.calcsize("P") != 8:
        raise Unavailable("native import results require 64-bit Windows Python")
    names = ("gcc", "as", "dlltool", "ld")
    for name in names:
        if not Path(env.tool(name)).is_file():
            raise Unavailable(f"missing tool: {env.tool(name)}")
    directory.mkdir(parents=True, exist_ok=True)
    obj, lib, exe = (directory / name for name in ("fixture.o", "kernel32.a", "fixture.exe"))
    commands = [
        [env.tool("gcc"), "-m32", "-c", str(SOURCE), "-o", str(obj), "-O0", "-std=c11",
         "-Wall", "-Wextra", "-Werror", "-ffreestanding", "-fno-stack-protector",
         "-fno-asynchronous-unwind-tables"],
        [env.tool("dlltool"), "-m", "i386", "--as", env.tool("as"), "--as-flags", "--32",
         "--kill-at", "-d", str(DEFINITION), "-l", str(lib)],
        [env.tool("ld"), "-mi386pe", "--entry", "_start", "--subsystem", "console",
         "--disable-dynamicbase", "-o", str(exe), str(obj), str(lib)],
    ]
    receipt = dict(source_sha256=env.digest(SOURCE), definition_sha256=env.digest(DEFINITION),
                   external_breakpoints=True, purpose="authored-real-OS-import-results",
                   tools={name: env.digest(Path(env.tool(name))) for name in names}, commands=[])
    try:
        for command in commands:
            result = env.run_tool(command, timeout=30)
            receipt["commands"].append(dict(argv=command, returncode=result.returncode,
                                            stdout=result.stdout, stderr=result.stderr))
            if result.returncode:
                raise RuntimeError(f"native import fixture build failed: {result.stderr}")
        pe = pefile.PE(str(exe))
        if pe.FILE_HEADER.Machine != 0x14c:
            raise ValueError("requires i386 PE")
        receipt["exe_sha256"] = env.digest(exe)
        return exe
    finally:
        (directory / "build.json").write_text(json.dumps(receipt, indent=2), encoding="utf-8")


class NativeResultsProfile:
    """Only this owned authored caller, its two IAT slots and documented ranges."""
    source, definition = SOURCE, DEFINITION
    scenarios = {"exact": "o", "short": "s", "limit": "l", "error": "e"}
    manifest_version, event_budget = 2, 1024
    warning = ("Real OS import endpoint observations from authored owned child. "
               "No game capture, all-effects completeness, transcript or replay claim. "
               "Unknown async/overlapped/error/short or limited output is incomplete.")

    def __init__(self, exe, workdir):
        self.exe, self.workdir = exe, workdir
        self.by_address = {}
        pe = pefile.PE(str(exe))
        self.imports = {imp.address: f"{entry.dll.decode()}!{imp.name.decode()}"
                        for entry in pe.DIRECTORY_ENTRY_IMPORT for imp in entry.imports}

    def bind(self, manifest, process, k, memory, exact, result):
        if exact(manifest["probe"], 1) != b"\x90" or exact(manifest["done"], 1) != b"\xcc":
            raise ValueError("unexpected authored control bytes")
        result["resolved_imports"] = {}
        k.K32GetMappedFileNameW.argtypes = (
            ctypes.wintypes.HANDLE, ctypes.c_void_p, ctypes.wintypes.LPWSTR, ctypes.wintypes.DWORD)
        k.K32GetMappedFileNameW.restype = ctypes.wintypes.DWORD
        for name in ("HeapAlloc", "ReadFile"):
            fields = manifest[name]
            abi = verify_caller(name, fields, exact, self.imports)
            if name == "ReadFile":
                abi["handle_basis"] = (
                    "Hash-bound authored start() opens this run's authored-input.bin with "
                    "CreateFileA OPEN_EXISTING, GENERIC_READ, flags=0 before attach; "
                    "error scenario closes it and passes INVALID_HANDLE_VALUE. "
                    "This is fixture-specific provenance, not handle classification by import name.")
                abi["docs"]["CreateFileA"] = DOCS["CreateFileA"]
            target = struct.unpack("<I", exact(fields["iat"], 4))[0]
            info = api._MemoryInfo()
            if not k.VirtualQueryEx(process, target, ctypes.byref(info), ctypes.sizeof(info)):
                raise OSError(ctypes.get_last_error(), "query resolved import")
            path = ctypes.create_unicode_buffer(2048)
            length = k.K32GetMappedFileNameW(process, target, path, len(path))
            if (not 0 < length < len(path) or info.Type != 0x1000000 or info.State != 0x1000 or
                    info.Protect & 0xff not in (0x10, 0x20, 0x40, 0x80)):
                raise ValueError("resolved target not identified as executable mapped DLL image")
            if not path.value.lower().endswith(("\\ntdll.dll", "\\kernelbase.dll", "\\kernel32.dll")):
                raise ValueError("resolved target outside explicitly allowed OS modules")
            if target in self.by_address:
                raise ValueError("aliased targets require a separately evidenced dispatch policy")
            fact = dict(name=name, target=target, iat=fields["iat"],
                        continuation=fields["continuation"], mapped_module=path.value,
                        allocation_base=info.AllocationBase, protection=info.Protect, abi=abi,
                        pristine_target_bytes=memory(target, 32, "resolved-target-code").report())
            self.by_address[target] = fact
            result["resolved_imports"][name] = fact
        self.manifest, self.result = manifest, result
        return self.by_address

    def entry(self, obs, tid, generation, address, ctx, snapshot, memory, exact, facts):
        binding = self.by_address[address]
        caller = struct.unpack("<I", exact(ctx.Esp, 4))[0]
        if caller != binding["continuation"]:
            self.result.setdefault("unselected_target_hits", []).append(
                dict(tid=tid, generation=generation, address=address, caller=caller))
            return None  # real target can be reached from incidental OS callers
        name = binding["name"]
        argc = binding["abi"]["argument_count"]
        raw_args = memory(ctx.Esp + 4, argc * 4, f"{name}.stack-arguments")
        if not raw_args.complete:
            self.result.setdefault("argument_read_failures", []).append(dict(
                tid=tid, generation=generation, address=address, read=raw_args.report()))
            raise RuntimeError("native arguments unreadable; cannot establish bounded output contract")
        args = list(struct.unpack(f"<{argc}I", raw_args.data))
        reads = [memory(ctx.Esp, 4, "return-word"), raw_args,
                 memory(binding["iat"], 4, "resolved-IAT-slot")]
        if name == "ReadFile":
            reads.extend((bounded_range(memory, args[1], args[2], "ReadFile.buffer-before"),
                          bounded_range(memory, args[3], 4, "ReadFile.byte-count-before")))
        basis = json.dumps(binding["abi"], sort_keys=True)
        invocation = obs.entry(tid, "owned-authored-native-caller", f"KERNEL32.dll!{name}",
                               snapshot(ctx, ip=address, reads=reads),
                               generation=generation, return_site=caller,
                               return_sp=ctx.Esp + binding["abi"]["expected_endpoint_delta"],
                               return_sp_basis=basis)
        if invocation is None:
            raise RuntimeError("native import entry rejected generation")
        facts[invocation] = dict(name=name, args=args, binding=binding)
        return invocation

    def outputs(self, fact, ctx, memory):
        return output_observations(memory, fact["name"], fact["args"], ctx.Eax)


def observe_native_results(exe, scenario, **options):
    if scenario not in NativeResultsProfile.scenarios:
        raise ValueError("unsupported authored native scenario")
    # Unique retained data per child, never overwrite another run's input.
    directory = exe.parent / ("run-" + uuid.uuid4().hex)
    directory.mkdir()
    payload = (b"CMRE-real-ReadFile\x00\x7f\x80\xff!\n" if scenario != "short" else b"short")
    assert len(payload) == (5 if scenario == "short" else 24)
    if scenario == "limit":
        payload = bytes(range(128))
    (directory / "authored-input.bin").write_bytes(payload)
    report = options.pop("report", {})
    report.update(authored_payload_hex=payload.hex(), authored_data_path=str(directory / "authored-input.bin"),
                  authored_data_sha256=env.digest(directory / "authored-input.bin"),
                  effects_scope="Explicit output ranges only; other memory/OS effects unobserved")
    return observe_calls(exe, scenario, _profile=NativeResultsProfile(exe, directory),
                         report=report, **options)

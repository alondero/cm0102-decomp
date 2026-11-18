"""Owned authored Windows file/time child: raw observations, never CRT replay.

C source/definition are emitted only into ignored local build storage. No game,
external PID, desktop, API replacement, or file-content reads are supported.
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

from cmre import capture_record as api, env
from cmre.file_time_observe import SIZES, classify_output, read_output
from native_debug_fixture import Unavailable
from native_import_observer import observe_calls
from native_import_results import ROOT, NativeResultsProfile, bounded_range

DOC_ROOT = "https://learn.microsoft.com/en-us/windows/win32/api/"
DOCS = {name: DOC_ROOT + group + "/nf-" + group + "-" + name.lower()
        for name, group in (("FindFirstFileA", "fileapi"), ("FindNextFileA", "fileapi"),
                            ("FileTimeToLocalFileTime", "fileapi"),
                            ("FileTimeToSystemTime", "timezoneapi"))}


def verify_caller(name, fields, exact, imports):
    if name not in SIZES or imports.get(fields["iat"]) != f"KERNEL32.dll!{name}":
        raise ValueError("not the PE-declared supported import")
    start, call, continuation = (fields[k] for k in ("pushes", "call", "continuation"))
    if not 0 < continuation - start <= 32 or continuation != call + 6:
        raise ValueError("unbounded native caller")
    raw = exact(start, continuation - start)
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(raw, start))
    if (len(instructions) != 3 or instructions[-1].address != call
            or instructions[-1].bytes != b"\xff\x15" + struct.pack("<I", fields["iat"])
            or any(i.mnemonic != "push" or len(i.operands) != 1 or i.operands[0].size != 4
                   for i in instructions[:-1])):
        raise ValueError("requires exactly two native u32 pushes and CALL [IAT]")
    return dict(argument_count=2, stack_argument_bytes=8, expected_endpoint_delta=12,
                caller_bytes_hex=raw.hex(), docs=DOCS[name],
                abi_docs="https://learn.microsoft.com/en-us/cpp/cpp/stdcall?view=msvc-170",
                limitation="Verified caller plus observed endpoint SP, not callee RET tracing")


# Minimal freestanding child. All selected continuations first store raw EAX.
# The names 'crt' in filenames identify a supply lane, NOT original CRT frames.
SOURCE_HEAD = r"""
typedef unsigned int U32;
typedef void *HANDLE;
#define API __attribute__((stdcall, dllimport))
API HANDLE GetStdHandle(U32);
API int ReadFile(HANDLE, void *, U32, U32 *, void *);
API int WriteFile(HANDLE, const void *, U32, U32 *, void *);
API HANDLE FindFirstFileA(const char *, void *);
API int FindNextFileA(HANDLE, void *);
API int FileTimeToLocalFileTime(const void *, void *);
API int FileTimeToSystemTime(const void *, void *);
API int FindClose(HANDLE);
API U32 SetErrorMode(U32);
API void ExitProcess(U32) __attribute__((noreturn));
_Static_assert(sizeof(void *) == 4 && sizeof(U32) == 4, "i386 only");
unsigned char first_data[320], next_data[320], local_data[8], local_saved[8], system_data[16];
U32 first_result, next_result, local_result, system_result;
U32 time_input[2] = {0x69050000, 0x01d5c036};
const char *pattern = "authored-*.bin";
extern char fixture_done, fixture_probe;
extern void calls(void), done_marker(void), probe(void);
static void text(const char *s) {
    U32 n = 0, wrote;
    while (s[n]) ++n;
    if (!WriteFile(GetStdHandle((U32)-11), s, n, &wrote, 0) || wrote != n) ExitProcess(70);
}
static void number(U32 n) {
    char b[11]; U32 i = 10; b[i] = 0;
    do { b[--i] = '0' + n % 10; n /= 10; } while (n);
    text(b + i);
}
static U32 command(void) {
    char c; U32 got;
    if (!ReadFile(GetStdHandle((U32)-10), &c, 1, &got, 0) || got != 1) ExitProcess(71);
    return (U32)c;
}
static void hex(const unsigned char *p, U32 size) {
    const char *digits = "0123456789abcdef"; char pair[3]; U32 i; pair[2] = 0;
    for (i = 0; i < size; ++i) {
        pair[0] = digits[p[i] >> 4]; pair[1] = digits[p[i] & 15]; text(pair);
    }
}
"""
CALLS = (
    ("FindFirstFileA", "first", "$_first_data", "_pattern", "first_result"),
    ("FindNextFileA", "next", "$_next_data", "_first_result", "next_result"),
    ("FileTimeToLocalFileTime", "local", "$_local_data", "$_time_input", "local_result"),
    ("FileTimeToSystemTime", "system", "$_system_data", "$_local_data", "system_result"),
)


def child_source():
    declarations, assembly, manifest, output = [], [".text", ".globl _calls", "_calls:"], [], []
    for name, tag, out, inp, scalar in CALLS:
        declarations.append(f'extern U32 {tag}_iat __asm__("__imp__{name}@8");\n'
                            f'extern char {tag}_pushes, {tag}_call, {tag}_cont;')
        assembly.extend([
            f".globl _{tag}_pushes, _{tag}_call, _{tag}_cont",
            f"_{tag}_pushes:", f"pushl {out}", f"pushl {inp}",
            f"_{tag}_call:", f"call *__imp__{name}@8",
            f"_{tag}_cont:", f"movl %eax, _{scalar}",
        ])
        # Use the real first file's last-write FILETIME as subsequent input.
        if tag == "first":
            assembly.extend(["cmpl $-1, %eax", "je 1f",
                             "movl _first_data+20, %eax", "movl %eax, _time_input",
                             "movl _first_data+24, %eax", "movl %eax, _time_input+4", "1:"])
        # Error scenario deliberately supplies an out-of-range SYSTEMTIME input,
        # independently of whether the preceding local conversion succeeded.
        if tag == "local":
            assembly.extend(["movl _local_data, %eax", "movl %eax, _local_saved",
                             "movl _local_data+4, %eax", "movl %eax, _local_saved+4",
                             "cmpl $-1, _first_result", "jne 2f",
                             "movl $-1, _local_data", "movl $-1, _local_data+4", "2:"])
        manifest.append(
            f'text(",\\"{name}\\":{{\\"iat\\":"); number((U32)&{tag}_iat);'
            f'text(",\\"pushes\\":"); number((U32)&{tag}_pushes);'
            f'text(",\\"call\\":"); number((U32)&{tag}_call);'
            f'text(",\\"continuation\\":"); number((U32)&{tag}_cont); text("}}");')
        output.append(
            f'text("{"," if output else ""}\\"{name}\\":{{\\"eax\\":"); number({scalar});'
            f'text(",\\"hex\\":\\""); hex({tag + "_saved" if tag == "local" else tag + "_data"}, '
            f'{SIZES[name]}); text("\\"}}");')
    assembly.extend(["ret", ".globl _done_marker, _fixture_done",
                     "_done_marker:", "_fixture_done:", "int3", "ret",
                     ".globl _probe, _fixture_probe", "_probe:", "_fixture_probe:", "nop", "ret"])
    return (SOURCE_HEAD + "\n".join(declarations) + "\n__asm__(" +
            json.dumps("\n".join(assembly) + "\n") + ");\n" + r"""
void start(void) {
    U32 i, mode;
    SetErrorMode(3);
    for (i = 0; i < 320; ++i) first_data[i] = next_data[i] = 0xa5;
    for (i = 0; i < 8; ++i) local_data[i] = 0xa5;
    for (i = 0; i < 16; ++i) system_data[i] = 0xa5;
    text("{\"version\":3,\"done\":"); number((U32)&fixture_done);
    text(",\"probe\":"); number((U32)&fixture_probe);
""" + "\n".join(manifest) + r"""
    text("}\n");
    mode = command();
    if (mode == 'e') { pattern = "missingx-*.bin"; time_input[0] = time_input[1] = 0xffffffff; }
    calls();
    done_marker();
    command();
    if (*(volatile unsigned char *)&fixture_probe != 0x90) ExitProcess(74);
    probe();
    text("{");
""" + "\n".join(output) + r"""
    text("}\n");
    if (first_result != 0xffffffff && !FindClose((HANDLE)first_result)) ExitProcess(73);
    ExitProcess(0);
}
""")


def build_native_crt_results(directory):
    if os.name != "nt" or struct.calcsize("P") != 8:
        raise Unavailable("requires 64-bit Windows Python and WOW64 debugging")
    names = ("gcc", "as", "dlltool", "ld")
    for name in names:
        if not Path(env.tool(name)).is_file():
            raise Unavailable(f"missing build tool: {env.tool(name)}")
    directory.mkdir(parents=True, exist_ok=True)
    source, definition = directory / "fixture.c", directory / "fixture.def"
    source.write_text(child_source(), encoding="ascii")
    definition.write_text("LIBRARY KERNEL32.dll\nEXPORTS\n" + "\n".join(
        [name + "@8" for name in SIZES] +
        ["GetStdHandle@4", "ReadFile@20", "WriteFile@20", "FindClose@4",
         "SetErrorMode@4", "ExitProcess@4"]) + "\n", encoding="ascii")
    obj, lib, exe = (directory / n for n in ("fixture.o", "kernel32.a", "fixture.exe"))
    commands = [
        [env.tool("gcc"), "-m32", "-c", str(source), "-o", str(obj), "-O0", "-std=c11",
         "-Wall", "-Wextra", "-Werror", "-ffreestanding", "-fno-stack-protector",
         "-fno-asynchronous-unwind-tables"],
        [env.tool("dlltool"), "-m", "i386", "--as", env.tool("as"), "--as-flags", "--32",
         "--kill-at", "-d", str(definition), "-l", str(lib)],
        [env.tool("ld"), "-mi386pe", "--entry", "_start", "--subsystem", "console",
         "--disable-dynamicbase", "-o", str(exe), str(obj), str(lib)],
    ]
    receipt = dict(source_sha256=env.digest(source), definition_sha256=env.digest(definition),
                   external_breakpoints=True, purpose="authored-file-time-OS-observations",
                   tools={n: env.digest(Path(env.tool(n))) for n in names}, commands=[])
    try:
        for command in commands:
            result = env.run_tool(command, timeout=30)
            receipt["commands"].append(dict(argv=command, returncode=result.returncode,
                                            stdout=result.stdout, stderr=result.stderr))
            if result.returncode:
                raise RuntimeError(f"authored fixture build failed: {result.stderr}")
        if pefile.PE(str(exe)).FILE_HEADER.Machine != 0x14c:
            raise ValueError("requires i386 PE")
        receipt["exe_sha256"] = env.digest(exe)
        return exe
    finally:
        (directory / "build.json").write_text(json.dumps(receipt, indent=2), encoding="utf-8")


class NativeCrtResultsProfile(NativeResultsProfile):
    """Private authored-child seam; never usable as a game import model."""
    scenarios = {"success": "s", "exhausted": "x", "error": "e"}
    manifest_version, event_budget = 3, 1024
    warning = ("Authored real file/time OS observations only. No original CRT frame, "
               "game parity, all-effects, timezone oracle or replay claim.")

    def __init__(self, exe, workdir):
        super().__init__(exe, workdir)
        self.source, self.definition = exe.parent / "fixture.c", exe.parent / "fixture.def"

    def bind(self, manifest, process, k, memory, exact, result):
        if exact(manifest["probe"], 1) != b"\x90" or exact(manifest["done"], 1) != b"\xcc":
            raise ValueError("unexpected authored control bytes")
        result["resolved_imports"], result["output_classification"] = {}, {}
        k.K32GetMappedFileNameW.argtypes = (
            ctypes.wintypes.HANDLE, ctypes.c_void_p, ctypes.wintypes.LPWSTR, ctypes.wintypes.DWORD)
        k.K32GetMappedFileNameW.restype = ctypes.wintypes.DWORD
        for name in SIZES:
            fields = manifest[name]
            abi = verify_caller(name, fields, exact, self.imports)
            target = struct.unpack("<I", exact(fields["iat"], 4))[0]
            info = api._MemoryInfo()
            if not k.VirtualQueryEx(process, target, ctypes.byref(info), ctypes.sizeof(info)):
                raise OSError(ctypes.get_last_error(), "query real OS target")
            path = ctypes.create_unicode_buffer(2048)
            length = k.K32GetMappedFileNameW(process, target, path, len(path))
            if (not 0 < length < len(path) or info.Type != 0x1000000 or info.State != 0x1000
                    or info.Protect & 0xff not in (0x10, 0x20, 0x40, 0x80)
                    or not path.value.lower().endswith(
                        ("\\ntdll.dll", "\\kernelbase.dll", "\\kernel32.dll"))):
                raise ValueError("resolved target not an allowed executable mapped OS DLL")
            if target in self.by_address:
                raise ValueError("aliased target unsupported")
            fact = dict(name=name, target=target, iat=fields["iat"],
                        continuation=fields["continuation"], mapped_module=path.value,
                        allocation_base=info.AllocationBase, protection=info.Protect, abi=abi,
                        pristine_target_bytes=memory(target, 32, "resolved-target-code").report())
            self.by_address[target] = result["resolved_imports"][name] = fact
        self.result = result
        return self.by_address

    def entry(self, obs, tid, generation, address, ctx, snapshot, memory, exact, facts):
        binding = self.by_address[address]
        caller = struct.unpack("<I", exact(ctx.Esp, 4))[0]
        if caller != binding["continuation"]:
            self.result.setdefault("unselected_target_hits", []).append(
                dict(tid=tid, generation=generation, address=address, caller=caller))
            return None
        name = binding["name"]
        raw = memory(ctx.Esp + 4, 8, name + ".stack-arguments")
        if not raw.complete:
            raise RuntimeError("unreadable selected arguments")
        args = list(struct.unpack("<2I", raw.data))
        before = read_output(memory, name, args, "before")
        reads = [memory(ctx.Esp, 4, "return-word"), raw, before,
                 memory(binding["iat"], 4, "resolved-IAT-slot")]
        if name == "FindFirstFileA":
            # Both allowed patterns have the same length; never follow a string
            # past its explicitly authored bound or enumerate another directory.
            pattern = bounded_range(memory, args[0], 15, name + ".input", limit=15)
            if not pattern.complete or pattern.data not in (b"authored-*.bin\0", b"missingx-*.bin\0"):
                raise ValueError("non-authored enumeration pattern")
            reads.append(pattern)
        elif name.startswith("FileTime"):
            reads.append(bounded_range(memory, args[0], 8, name + ".input", limit=8))
        invocation = obs.entry(
            tid, "owned-authored-file-time-child", "KERNEL32.dll!" + name,
            snapshot(ctx, ip=address, reads=reads), generation=generation, return_site=caller,
            return_sp=ctx.Esp + 12, return_sp_basis=json.dumps(binding["abi"], sort_keys=True))
        if invocation is None:
            raise RuntimeError("entry identity rejected")
        facts[invocation] = dict(name=name, args=args, before=before.report())
        return invocation

    def outputs(self, fact, ctx, memory):
        name = fact["name"]
        after = read_output(memory, name, fact["args"], "after")
        gaps = ["bounded-storage-only-not-complete-effects"]
        if fact["before"]["complete"] and after.complete:
            classification = classify_output(
                name, ctx.Eax, bytes.fromhex(fact["before"]["data_hex"]), after.data)
            self.result["output_classification"][name] = classification
            gaps = classification["gaps"]
            if classification["output_status"] == "undefined-on-failure":
                gaps = gaps + ["undefined-failure-output-storage"]
        else:
            gaps.append("output-classification-unavailable-partial-read")
        return (after,), tuple(gaps)


def observe_native_crt_results(exe, scenario, **options):
    if scenario not in NativeCrtResultsProfile.scenarios:
        raise ValueError("unsupported authored scenario")
    # Dedicated per-run directory retained in ignored local storage, not a host
    # directory supplied by a caller. These empty authored files contain no data.
    directory = exe.parent / ("run-" + uuid.uuid4().hex)
    directory.mkdir()
    filenames = ["authored-one.bin"] + (["authored-two.bin"] if scenario == "success" else [])
    for filename in filenames:
        (directory / filename).write_bytes(b"")
    report = options.pop("report", {})
    report.update(authored_directory=str(directory), authored_files=filenames,
                  effects_scope="Only declared bounded raw storage and actual EAX; no replay")
    return observe_calls(exe, scenario, _profile=NativeCrtResultsProfile(exe, directory),
                         report=report, **options)

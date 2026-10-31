# ADR 0003: FS-segment access in plain C

Status: accepted (2026-10-19, chosen by the project owner).

## Context

MSVC functions with structured exception handling (SEH) link a registration
record into the thread's exception list at `fs:[0]` on entry and unlink it
before returning:

```
push -1 ; push handler ; mov eax, fs:[0] ; push eax ; mov fs:[0], esp
...
mov ecx, [frame] ; mov fs:[0], ecx
```

About 24 of the next 160 C-queue IDs (at least 129 bytes) have this shape.
Inline assembly is reserved for legacy `abi` records ([C.md](../agent/C.md)),
so plain C had no sanctioned way to reach `fs:[0]`. One worker hard-coded the
emulator's thread block address `0x7FFDF000` (`recovery/c/fn_005a02f0.c`).
That passes the harness only because of where the emulator puts the thread block.
The source was rejected.

The emulator already gives FS a descriptor whose base is its thread block
(`cmre/emu.py`). [ADR 0002](0002-semantic-c-profile.md) already canonicalises
a test-stack address stored as a value, such as the registration pointer.

## Decision

`recovery/include/cmre.h` defines

```c
#define CM_FS(type, offset) (*(type volatile __seg_fs *)(offset))
```

This uses GCC's x86 named address space. The pinned toolchain (MinGW GCC 15.2,
`-m32`) emits `mov %fs:0x0, reg` and `mov reg, %fs:0x0` at `-O0` and `-O2`. No
inline assembly is involved. The registration record is one `volatile` struct
in listing order, so the compiler keeps its words adjacent and stored:

```c
struct seh_registration { void *prev; u32 handler; u32 state; };
volatile struct seh_registration frame;
frame.state = 0xffffffffu;
frame.handler = 0x0095918bu;           /* handler VA from the listing */
frame.prev = CM_FS(void *, 0);
CM_FS(void *, 0) = (void *)&frame;
...                                    /* state updates where the listing has them */
CM_FS(void *, 0) = frame.prev;
```

## Consequences

- The semantic gate compares the thread block like other non-stack memory.
  Leaving a frame linked fails, because the thread block keeps a stack address
  where the original restored the previous head. A wrong `fs:` offset or width
  fails too. `tests/test_cmre_semantic.py::test_seh_registration_through_the_fs_segment`
  holds the passing form and the dangling-frame near miss.
- Exception dispatch and unwinding remain unproven. The emulator ends a case at
  a fault and never runs the handler. Whether the record's handler and state
  words are right is therefore checked by review against the listing, not by
  the gate. The same holds for the frame layout an `__except_handler3`-style
  handler would read through EBP.
- The macro is GCC-specific. The native port (M6/M7) must replace it with the
  platform's thread-information or exception mechanism. Each use marks a site
  that needs that replacement.
- Changing `cmre.h` changes every C record's recorded dependency hash. Adding
  the macro required the C refresh and full revalidation described in
  [HARNESS.md](../agent/HARNESS.md).

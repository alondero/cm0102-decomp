/* Tier-2 C recovery support for the 3.9.68 primary executable.

   Rules (see docs/agent/PLAYBOOK.md):
   - Name the function cm_fn_<8 hex digits of the original VA> so the harness
     links and tests it; keep the original calling convention.
   - Read/write original globals with CM_GLOBAL at their preferred-image VA.
   - Call other original functions through CM_ORIGINAL declarations: the build
     binds cm_fn_<va> to the original address, an explicit hybrid boundary.
   - Never invent behaviour. Integer widths, signedness and evaluation order must
     follow the original instructions; tests compare against them. */
#ifndef CMRE_H
#define CMRE_H

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;

#define CM_THISCALL __attribute__((thiscall))
#define CM_STDCALL __attribute__((stdcall))
#define CM_FASTCALL __attribute__((fastcall))
#define CM_CDECL __attribute__((cdecl))

/* Source-declared near CALL execution contract. Eight lowercase hex digits,
   without 0x, identify the original call, return and callee preferred-image VAs.
   The builder verifies pinned CALL and emitted PUSH/JMP bytes. VM continuation
   dispatch is required; this sequence alone is not standalone native binding.
   Byte directives work inside either assembler syntax; the return word is
   written once by native PUSH, and original image bytes remain intact. */
#define CM_ORIGINAL_CALL(callva, returnva, calleeva) \
    "v_" #callva ":\n" \
    "cmre_call_" #callva "_" #returnva "_" #calleeva ":\n" \
    ".byte 0x68\n.long 0x" #returnva "\n" \
    ".byte 0xe9\n.long _cm_fn_" #calleeva " - . - 4\n" \
    "cmre_return_" #callva "_" #returnva ":\n" \
    "v_" #returnva ":\n"

/* Verified CALL [EAX], CALL [EDX], CALL EAX, CALL ESI, CALL EBP
   and CALL [absolute32] only.
   A native JMP enters that
   original instruction under owned VM execution. The CPU reads the target
   before writing the original return word, including return-slot aliases and
   native faults. The VM redirects selected callee entries and owned near RETs
   by EIP only. Actual callee cleanup is observed, never presumed. Original
   interior targets, other indirect forms and SEH/non-RET unwind are unproven. */
#define CM_ORIGINAL_INDIRECT_CALL(callva, returnva) \
    "v_" #callva ":\n" \
    "cmre_indirect_" #callva "_" #returnva ":\n" \
    ".byte 0xe9\n.long 0x" #callva " - . - 4\n" \
    "cmre_return_" #callva "_" #returnva ":\n" \
    "v_" #returnva ":\n"

/* An original global at preferred-image VA `va` with C type `type`. */
#define CM_GLOBAL(type, va) (*(type *)(va))

/* The current thread's information block at byte `offset`, through the FS
   segment like the original `fs:[offset]` (GCC named address space; no inline
   assembly). fs:[0] heads the SEH registration chain: link a volatile
   { prev, handler, state } record in listing order and restore prev before
   returning. Exception dispatch and unwinding remain unproven (ADR 0003). */
#define CM_FS(type, offset) (*(type volatile __seg_fs *)(offset))

#endif

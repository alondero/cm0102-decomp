/* cm_fn_005c6f00  (module comp/friendly.cpp)
   Pinned binary: CM0102 3.9.68 cm0102.exe, SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86
   (preferred-image base 0x00400000; all VAs below are preferred-image VAs).

   ABI evidence from the original instructions:
   - 0x005c71e9 `ret 0x10` pops 16 stack bytes -> four stack arguments.
   - ECX is read before it is written in this body: 0x005c6f2a stores the entry
     ECX home and 0x005c6f32 calls 0x005c88f0 while that value is still live, so
     the entry is MSVC thiscall (ECX = this, then four stack arguments, callee
     pops them).
   - EAX is the only return register read (0x005c6fd2/0x005c7131 `xor eax,eax`,
     0x005c71cf `mov eax,edi`).

   Stack frame recovered from the listing (E = entry ESP, i.e. address of the
   return slot; L = E-0x41c, the base established by `sub esp,0x410`):
     E-0x0c  SEH prev     (0x005c6f0d push fs:[0], restored at 0x005c71dc)
     E-0x08  SEH handler  (0x005c6f02 push 0x95a15c)
     E-0x04  SEH state    (0x005c6f00 push -1, rewritten at 0x005c6f53/6f69/70a2/70bc)
     L+0x00  found flag   ([esp+0x18] at 0x005c6f2e / [esp+0x10] at 0x005c7138)
     L+0x04  entry ECX    ([esp+0x1c] at 0x005c6f2a / [esp+0x14] at 0x005c7082)
     L+0x08  object slot  ([esp+0x18] at 0x005c6f4d/70a0, address `lea`ed at 0x005c7050)
     L+0x10  0x100-byte buffer  ([esp+0x020] at 0x005c6f76/70d6, [esp+0x0d2]... )
     L+0x110  0x100-byte buffer  ([esp+0x120] at 0x005c6f7a/70cf)
     L+0x210  0x100-byte buffer  ([esp+0x220] at 0x005c7156)
     L+0x310  0x100-byte buffer  ([esp+0x320] at 0x005c715d)
   The four buffers occupy [E-0x40c, E-0x0c) and are kept in one scratch
   allocation with explicit offsets so their adjacency survives.

   OPEN ITEM (still held: existing-record ECX transport):
   0x005c7001 `mov cx,[edi+2]` and 0x005c7005 `mov dx,[edi]` write only the low
   16 bits of ECX/EDX, and 0x005c700a/0x005c7011 then push the FULL 32-bit
   ECX/EDX into 0x005ae870. The upper halves are live register residue (on the
   existing-record path the ECX residue of 0x005c88f0 through 0x0068d1b0; on
   the new-record path whatever 0x0054a110 and the error-path calls leave).
   0x005ae924 loads the full third argument and 0x005ae955 pushes the full EDI
   into 0x005abf10, 0x005abf56/0x005abf81 forward the whole year into
   0x005aa2d0/0x005a9cf0, so the upper 16 bits of that argument are not proven
   unobserved; only the second argument is masked to 16 bits at 0x005abfc7.
    EDX is now transported through EDX:EAX return declarations, not replaced
    with zero. For this call's zero first stack argument, 0x005c8926 skips
    0x004a7f20; the original 0x005c895c loads EDX from the second argument,
    and 0x0068d1b0 writes EDX at 0x0068d1b0/0x0068d1b9. Both return paths
    of 0x005c88f0 leave that EDX intact.
    On the new-record path, 0x00944cfc/0x00944cfd pop ECX, then
    0x005c6f5c sets ECX to obj. 0x0054a110..0x0054a124 never writes ECX
    or EDX, so obj supplies ECX and the allocator supplies EDX.
    Existing-record ECX remains untransported: 0x005c890f/0x005c897c
    load it from the manager field; 0x0068d1b0..0x0068d1c2 preserve it.
    Re-reading that field here would add a memory access absent from the
    caller and is not a general ABI transport solution.

    The EDX input at 0x005c714b is now resolved: 0x00417c35 loads EDX
    from [ECX], and 0x00417c37..0x00417c40 leave it intact. Capture that
    actual return register through EDX:EAX. If 0x005c7094 calls the allocator,
    its EDX replaces it; 0x0068d020..0x0068d030 never writes EDX.
    These are bound original-target reviews, not callee composition or
    runtime proof. All instruction VAs refer to the pinned binary above.

    The error-path pushes preceding 0x0090d130 belong to 0x00944c9f:
    at 0x005c6f9d/6fa2/6fa3 they push line, second buffer, first buffer;
    0x005c6fa9/6faa/6faf then push string, format, destination. The single
    deferred cleanup at 0x005c6fc9 confirms their continued stack lifetime.
    The corresponding sequences are 0x005c70f6..0x005c712e and
    0x005c7183..0x005c71bb. All six formatting arguments are explicit below.
    This fixes those call boundaries, not the held ECX-upper issue above. */

#include "cmre.h"

/* SEH registration record at fs:[0]: { prev, handler, state } (ADR 0003). */
typedef struct {
  u32 prev;
  u32 handler;
  u32 state;
} cmre_seh_record;

/* Callee boundaries.  Legacy/compiled callees keep their recorded
   conventions; the receiver of each thiscall is the value the listing sets
   in ECX immediately before the call. */
extern unsigned long long CM_THISCALL cm_fn_00417c10(u32 *self, u32 *arg1, u16 *arg2);
extern void CM_FASTCALL cm_fn_0054a110(u16 *self);
extern u32 CM_THISCALL cm_fn_005ae870(u8 *self, u32 arg1, u32 arg2, u32 arg3, u32 arg4);
extern unsigned long long CM_THISCALL cm_fn_005c88f0(u32 self, u32 arg1, u32 arg2);
extern void cm_fn_005e8290(const char *arg1, const char *arg2, int arg3);
extern u32 CM_THISCALL cm_fn_0068d020(u32 self);
extern unsigned long long CM_FASTCALL cm_fn_0068d0d0(volatile u8 *self, u32 edx_in,
                                                     const u8 *src, s16 count);
extern char *cm_fn_0090d130(void);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg1);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

u32 CM_THISCALL cm_fn_005c6f00(u32 self, u32 arg1, u16 *arg2, u32 arg3, u32 arg4)
{
  u8 scratch[0x410];
  volatile cmre_seh_record frame;
  u32 obj;      /* ESI: the record, existing or freshly constructed */
  u32 result;   /* EAX at 0x005c71cf */
  u32 mgr;      /* EBX: the reloaded entry ECX (0x005c7082) */
  u32 field;    /* EAX scratch: object field / manager / formatted string */
  u32 day;      /* full EDX pushed at 0x005c7011 -- low 16 observed */
  u32 year;     /* full ECX pushed at 0x005c700a -- low 16 observed */
  unsigned long long pair;
  u32 edx_value;

  /* 0x005c6f00 push -1 / 0x005c6f02 push 0x95a15c / 0x005c6f07-0x005c6f0e
     read fs:[0], push it, then link the frame. */
  frame.state = 0xffffffffu;
  frame.handler = 0x0095a15cu;
  frame.prev = CM_FS(u32, 0);
  CM_FS(u32, 0) = (u32)&frame;

  /* 0x005c6f2a store the entry ECX home, 0x005c6f2e clear the found flag
     (after 0x005c6f28/0x005c6f29 pushed arg1 and 0). */
  *(u32 *)(scratch + 4) = self;
  *(u32 *)(scratch + 0) = 0;

  pair = cm_fn_005c88f0(self, 0, arg1);
  obj = (u32)pair;
  edx_value = (u32)(pair >> 32);
  if (obj == 0) {
    pair = cm_fn_00944cf1(0x19);
    obj = (u32)pair;
    edx_value = (u32)(pair >> 32);
    *(u32 *)(scratch + 8) = obj;
    frame.state = 0;
    if (obj != 0) {
      cm_fn_0054a110((u16 *)obj);
    }
    else {
      obj = 0;
    }
    frame.state = 0xffffffffu;
    if (obj == 0) {
      /* 0x005c6f76..0x005c6fd4 */
      cm_fn_00944cff(0x009cd0a0u, 0, 0, (void *)(scratch + 0x110),
                     (void *)(scratch + 0x10));
      field = (u32)cm_fn_0090d130();
      cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, field,
                     scratch + 0x110, scratch + 0x10, 0xe6b);
      cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
      CM_GLOBAL(void *, 0x00b67a34) = (void *)0;
      result = 0;
      goto epilogue;
    }
  }
  else {
    /* 0x005c6fd9 */
    *(u32 *)(scratch + 0) = 1;
  }

  /* 0x005c6fe1: EAX = arg4, EBX = arg3, EDI = arg2 at 0x005c6ff1. */
  if (arg4 != 0) {
    field = *(u32 *)(arg1 + 0x69);
    if (field != 0) {
      /* 0x005c7001 `mov cx,word[edi+2]`, 0x005c7005 `mov dx,word[edi]`:
         only the low 16 bits of ECX/EDX are written; see the OPEN ITEM. */
      /* Existing-record case retains the starting defect, not a proven zero. */
      year = (u32)arg2[1];
      if (*(u32 *)(scratch + 0) == 0)
        year |= obj & 0xffff0000u;
      day = (edx_value & 0xffff0000u) | (u32)arg2[0];
      if (cm_fn_005ae870((u8 *)CM_GLOBAL(u32, 0x00ae2a58), field, day, year,
                         0xffffffffu) != 0) {
        result = 0;
        goto epilogue;
      }
    }
  }
  else if (arg3 != 0) {
    field = *(u32 *)(arg3 + 0x69);
    if (field != 0) {
      /* 0x005c702e/0x005c7032, same partial-register pushes. */
      /* Existing-record case retains the starting defect, not a proven zero. */
      year = (u32)arg2[1];
      if (*(u32 *)(scratch + 0) == 0)
        year |= obj & 0xffff0000u;
      day = (edx_value & 0xffff0000u) | (u32)arg2[0];
      if (cm_fn_005ae870((u8 *)CM_GLOBAL(u32, 0x00ae2a58), field, day, year,
                         0xffffffffu) != 0) {
        result = 0;
        goto epilogue;
      }
    }
  }

  /* 0x005c7050: `lea ecx,[esp+0x18]` is the address of the object slot;
     0x005c7054 push arg2; 0x005c7055 push that address; 0x005c7056 this = obj;
     0x005c7058 store arg1; 0x005c705b call. */
  *(u32 *)(obj + 0xc) = arg1;
  pair = cm_fn_00417c10((u32 *)obj, (u32 *)(scratch + 8), arg2);
  edx_value = (u32)(pair >> 32);

  /* 0x005c7060..0x005c707f */
  *(u8 *)(obj + 0x18) = 1;
  *(u32 *)(obj + 8) = 0;
  if (arg4 != 0) {
    *(u32 *)(obj + 0x10) = arg1;
    *(u32 *)(obj + 0x14) = arg3;
  }
  else {
    *(u32 *)(obj + 0x10) = arg3;
    *(u32 *)(obj + 0x14) = arg1;
  }

  /* 0x005c7082 reload of the entry ECX home. */
  mgr = *(u32 *)(scratch + 4);
  if (*(u32 *)(mgr + 0x34c8) == 0) {
    /* 0x005c7092..0x005c70c7 */
    pair = cm_fn_00944cf1(0xe);
    field = (u32)pair;
    edx_value = (u32)(pair >> 32);
    *(u32 *)(scratch + 8) = field;
    frame.state = 1;
    if (field != 0) {
      field = cm_fn_0068d020(field);
    }
    else {
      field = 0;
    }
    frame.state = 0xffffffffu;
    *(u32 *)(mgr + 0x34c8) = field;
    if (field == 0) {
      /* 0x005c70cf..0x005c7133 */
      cm_fn_00944cff(0x009cd0a0u, 0, 0, (void *)(scratch + 0x10),
                     (void *)(scratch + 0x110));
      field = (u32)cm_fn_0090d130();
      cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, field,
                     scratch + 0x10, scratch + 0x110, 0xe9f);
      cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
      CM_GLOBAL(void *, 0x00b67a34) = (void *)0;
      cm_fn_00944c94((void *)obj);
      result = 0;
      goto epilogue;
    }
  }

  /* 0x005c7138 */
  if (*(u32 *)(scratch + 0) != 0) {
    /* 0x005c71cd: an existing record is returned untouched. */
    result = obj;
  }
  else {
    /* 0x005c7142 ECX = the manager field, 0x005c7148/0x005c714a push count and
       source, 0x005c714b call. EDX is the captured original return register
       from 0x00417c10 or the allocator; the 64-bit result is kept and
       only EAX is used (0x005c7150 `mov edi,eax`). */
    result = (u32)cm_fn_0068d0d0((volatile u8 *)*(u32 *)(mgr + 0x34c8), edx_value,
                                 (const u8 *)obj, 0x19);
    if (result != 0) {
      /* 0x005c71c2 */
      cm_fn_00944c94((void *)obj);
    }
    else {
      /* 0x005c7156..0x005c71c0 */
      cm_fn_00944cff(0x009cd0a0u, 0, 0, (void *)(scratch + 0x310),
                     (void *)(scratch + 0x210));
      field = (u32)cm_fn_0090d130();
      cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, field,
                     scratch + 0x310, scratch + 0x210, 0xeac);
      cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
      CM_GLOBAL(void *, 0x00b67a34) = (void *)0;
      cm_fn_00944c94((void *)obj);
      result = 0;
    }
  }

epilogue:
  /* 0x005c71d1 `mov ecx,[esp+0x420]` then 0x005c71dc `mov fs:[0],ecx`. */
  CM_FS(u32, 0) = frame.prev;
  return result;
}

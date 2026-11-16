/* Translation of preferred-image VA 0x007435a0 (CM 01/02 3.9.68, image base
   0x00400000).  Convention from the listing: `ret 0xc` (three caller-owned
   dword arguments) and no ECX read before its first write, so this is a
   CM_STDCALL function with (arg1 = owner object forwarded as ECX to
   cm_fn_00785830, arg2 = mode flag tested at entry, arg3 = target object
   forwarded as ECX to cm_fn_004ae660 / cm_fn_004ae8a0).  The full EAX result
   (cm_fn_004ae8a0's EAX, or 0 when cm_fn_0053e990 reports nothing) is
   returned.

   Stack frame, original instructions:
     007435a0  mov eax, 0x1534
     007435a5  call 0x945610            ; __chkstk-style probe + ESP -= 0x1534
     007435aa  mov eax, [esp+0x153c]    ; == S+8  -> arg2
     007435b4  mov esi, [esp+0x1544]    ; == S+4  -> arg1 (after 3 pushes)
     00743a3f / 00743a45                ; add esp,0x1534 ; ret 0xc

   With S = ESP at entry (S+0 = return address) the post-probe frame base is
   B = S - 0x1544, so the original's argument homes are arg1=[B+0x1548],
   arg2=[B+0x154c], arg3=[B+0x1550], and the local slots read/written by the
   listing are B+0x10, B+0x14, B+0x18, B+0x1c, B+0x20, B+0x24, B+0x28, a
   400-byte (0x190) formatted-record area at B+0x2c and message text starting
   at B+0x1bc, i.e. immediately above it.  All of that is one scratch object of
   0x1534 bytes (= the original frame size) with explicit offsets, so the two
   adjacent buffers keep their original adjacency.

   UNRESOLVED entry boundary; this source does not resolve the probe hold.
   Evidence: CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86,
   preferred-image VAs 0x007435a0 (EAX=0x1534), 0x007435a5 (CALL
   0x00945610), 0x007435aa (first argument read), and 0x00743a3f /
   0x00743a45 (release 0x1534 bytes; RET 0xc). The supplied target listing
   establishes that boundary and its ordering, not the helper's individual
   accesses. The scratch object preserves relative buffer adjacency only:
   equal size does not establish placement relative to entry ESP or equivalent
   probe/fault ordering. Do not infer either from a passing semantic gate.
   Calling the helper with a normal C declaration does not express its
   EAX input or stack-replacing return contract. An authorized entry-boundary
   implementation and original-helper instruction evidence are required;
   inline assembly and naked functions are outside this plain-C worker lane.

   Listing order is preserved: ten/thirteen cm_fn_00785830 calls, then the
   DAT_00ae23b4 record load (before cm_fn_0053e140, which may write it), the
   cm_fn_0053e140 call, the format selection, cm_fn_004ae660(text), then
   cm_fn_0053e990 and, when it returns nonzero, cm_fn_004ae8a0 with a fresh
   reload of DAT_00ae23bc (both global reads are volatile so the original
   reload survives a callee writing the global).  The format helper has 3, 4, 5
   or 7 stack arguments at different sites, so it stays variadic rather than
   inventing a fixed arity. */

#include "cmre.h"

/* Both globals are reloaded at distinct instruction sites in the original, so
   the reads are volatile and never cached across the intervening calls. */
#define CM_VOL_U32(va) CM_GLOBAL(volatile u32, (va))

/* cm_fn_00785830 / cm_fn_004ae660 / cm_fn_004ae8a0 receive their receiver in
   ECX (mov ecx,esi / mov ecx,ebp / mov ecx,[esp+0x1550] in the listing) and
   pop the pushed stack arguments themselves. */
extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);
extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 arg1);
extern u32 CM_THISCALL cm_fn_004ae8a0(void *self, u32 a1, u32 a2, u32 a3);

/* Five- and seven-argument cdecl callees; the caller emits the matching
   `add esp,0x14` / `add esp,0x1c` / `add esp,0x10` / `add esp,0xc`. */
extern u32 CM_CDECL cm_fn_0053e140(u32 first, u32 second, u32 third, u32 output,
                                   u32 size, u32 kind, u32 person);
extern u32 CM_CDECL cm_fn_0053e990(u32 person, u32 first, u32 second, u32 third,
                                   u32 date);
/* Text formatter: called with (dest, fmt) plus 1, 2, 3 or 5 further arguments
   at the nine call sites below, so the tail is variadic. */
extern char * CM_CDECL cm_fn_0066f4e0(void *dest, const char *fmt, ...);

u32 CM_STDCALL cm_fn_007435a0(void *owner, u32 mode, void *target)
{
  /* Relative buffer offsets only, not a reconstruction of entry ESP or the
     omitted probe operation; see the unresolved entry boundary above. */
  u8 scratch[0x1534];
  u8 *const out = scratch + 0x2c;   /* 0x190-byte record text, B+0x2c */
  u8 *const text = scratch + 0x1bc; /* message text, B+0x1bc */
  u32 r0, r1, r2, r3, r4, r5, r6, r7, r8, r9;
  u32 base, rec;

  if (mode == 0) {
    /* 007438a9: the alternate layout, indices 0..7. */
    r0 = (u32)cm_fn_00785830(owner, 0);
    r1 = (u32)cm_fn_00785830(owner, 1);
    r2 = (u32)cm_fn_00785830(owner, 2);
    r3 = (u32)cm_fn_00785830(owner, 3);
    (void)cm_fn_00785830(owner, 4); /* 007438d7: result discarded */
    r5 = (u32)cm_fn_00785830(owner, 5);
    r6 = (u32)cm_fn_00785830(owner, 6);
    r7 = (u32)cm_fn_00785830(owner, 7);
    /* 00743915: (r1, r2, r3, &out, 0x190, 5, r0); edx at 0x007438ff is the
       [esp+0x14] slot (frame B+0x14 = index 3's result), not the [esp+0x10]
       slot that the r6 tests below reload. */
    cm_fn_0053e140(r1, r2, r3, (u32)out, 400, 5, r0);
    if (r5 == 2) {
      /* 00743994 */
      if (r6 == 2) {
        cm_fn_0066f4e0(text, (const char *)0x00a38264, (u32)out);
      } else if (r7 == 4) {
        cm_fn_0066f4e0(text, (const char *)0x00a3822c, (u32)out);
      } else {
        cm_fn_0066f4e0(text, (const char *)0x00a381f8, (u32)out);
      }
    } else if (r6 == 2) {
      cm_fn_0066f4e0(text, (const char *)0x00a38318, 100, (u32)out);
    } else if (r7 == 4) {
      cm_fn_0066f4e0(text, (const char *)0x00a382d8, 100, (u32)out);
    } else {
      cm_fn_0066f4e0(text, (const char *)0x00a38298, 100, (u32)out);
    }
    goto done;
  }

  /* 007435c8: the primary layout, indices 0..9. */
  r0 = (u32)cm_fn_00785830(owner, 0);
  r1 = (u32)cm_fn_00785830(owner, 1);
  r2 = (u32)cm_fn_00785830(owner, 2);
  r3 = (u32)cm_fn_00785830(owner, 3);
  r4 = (u32)cm_fn_00785830(owner, 4);
  r5 = (u32)cm_fn_00785830(owner, 5);
  r6 = (u32)cm_fn_00785830(owner, 6);
  r7 = (u32)cm_fn_00785830(owner, 7);
  r8 = (u32)cm_fn_00785830(owner, 8);
  r9 = (u32)cm_fn_00785830(owner, 9);
  /* 0074363c: base load precedes the cm_fn_0053e140 call, which may store to
     it; the record stride is 0x245 = 581 bytes. */
  base = (u32)CM_VOL_U32(0x00ae23b4);
  rec = base + r4 * 581u;
  /* 0074366f: (r1, r2, r3, &out, 0x190, 1, r0) */
  cm_fn_0053e140(r1, r2, r3, (u32)out, 400, 1, r0);
  if (r5 == 2) {
    /* 00743797 */
    if (r8 == 3) {
      cm_fn_0066f4e0(text, (const char *)0x00a3879c, (u32)out);
    } else if (r6 == 2) {
      cm_fn_0066f4e0(text, (const char *)0x00a386f0, 100, (u32)out);
    } else if (r6 == 3) {
      if (r7 == 2) {
        cm_fn_0066f4e0(text, (const char *)0x00a385d0, (u32)out);
      } else if (r7 == 4) {
        cm_fn_0066f4e0(text, (const char *)0x00a384fc, (u32)out);
      } else {
        cm_fn_0066f4e0(text, (const char *)0x00a38408, (u32)out);
      }
    } else {
      cm_fn_0066f4e0(text, (const char *)0x00a3834c, (u32)out);
    }
  } else if (r8 == 3) {
    /* 0074368b: signed byte attribute of the record at +0x52, read before the
       record pointer is advanced; both pushes use the same value. */
    s32 chr = (s32)*(s8 *)(unsigned long)(rec + 0x52);
    if (r9 == 0) {
      cm_fn_0066f4e0(text, (const char *)0x00a38c2c, 100, chr, chr, (u32)out,
                     (u32)(rec + 0x38));
    } else {
      cm_fn_0066f4e0(text, (const char *)0x00a38d18, 100, chr, chr, (u32)out,
                     (u32)(rec + 0x38));
    }
  } else if (r6 == 2) {
    cm_fn_0066f4e0(text, (const char *)0x00a38ba4, 100, (u32)out);
  } else if (r6 == 3) {
    cm_fn_0066f4e0(text, (const char *)0x00a38ad0, 100, (u32)out);
  } else if (r7 == 4) {
    cm_fn_0066f4e0(text, (const char *)0x00a38a04, 100, (u32)out);
  } else if (r7 != 0) {
    /* 00743771: the same pointer is pushed twice. */
    cm_fn_0066f4e0(text, (const char *)0x00a3882c, 100, (u32)out, (u32)out);
  } else {
    cm_fn_0066f4e0(text, (const char *)0x00a38928, 100, (u32)out);
  }

done:
  /* 0074383b: target object from its argument home, then the text buffer. */
  cm_fn_004ae660(target, (u32)text);
  /* 00743872 / 00743a10: person = r0*110 + DAT_00ae23bc. */
  if (cm_fn_0053e990(r0 * 110u + (u32)CM_VOL_U32(0x00ae23bc), r1, r2, r3, 0) == 0) {
    return 0;
  }
  /* 00743882 / 00743a1c: DAT_00ae23bc is reloaded after cm_fn_0053e990. */
  return cm_fn_004ae8a0(target, (u32)out, 0x7d7,
                        r0 * 110u + (u32)CM_VOL_U32(0x00ae23bc));
}

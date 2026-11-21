/* Reconstructed from the original instructions of preferred-image VA
   0x004db2e0 (image base 0x00400000) in CM0102 3.9.68, SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.

   ABI (from original instructions):
   - `ret 0xc` at 0x004db4b4 / 0x004db5bc pops 12 stack bytes: three dword
     arguments with callee cleanup (stdcall).
   - no register is read before it is written on entry, so this is not
     thiscall/fastcall.
   Arguments: a1 = receiver for the cm_fn_00785830 batch (0x004db2f8 loads it
   from [esp+0x150] = entry [esp+4]), a2 = selector tested at 0x004db2ea from
   entry [esp+8], a3 = receiver for the later cm_fn_004ae660/cm_fn_004ae8a0
   calls (0x004db414 / 0x004db558 load [esp+0x154] = entry [esp+0xc]).
   Locals: one 300-byte output buffer, [esp+0x24] at 0x004db35b and
   [esp+0x1c]/[esp+0x20] elsewhere (all frame offset -0x12c); it is passed to
   cm_fn_0053e140 and used as the name pointer in the final cm_fn_004ae8a0. */

#include "cmre.h"

#define G_00ae23b4 CM_GLOBAL(u32, 0x00ae23b4)
#define G_00ae23bc CM_GLOBAL(volatile u32, 0x00ae23bc)

#define DEST_00de1f64 ((u32)0x00de1f64)
#define FMT_009af260  ((char *)0x009af260)
#define FMT_009af1b4  ((char *)0x009af1b4)
#define FMT_009af178  ((char *)0x009af178)
#define FMT_009af13c  ((char *)0x009af13c)

extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 arg1);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32 self, const char *name, u32 key_a, u32 key_b);
extern u32 CM_CDECL cm_fn_0053e140(u32 first, u32 second, u32 third, u32 output, u32 size, u32 kind, u32 person);
extern u32 CM_CDECL cm_fn_0053e990(u32 person, u32 first, u32 second, u32 third, u32 date);
extern char * CM_CDECL cm_fn_0066f4e0(u32 dest, const char *fmt, ...);
extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);

void CM_STDCALL cm_fn_004db2e0(u32 a1, u32 a2, u32 a3)
{
  u8 buf[300];

  if (a2 != 0) {
    void *self = (void *)a1;
    u32 v0, v1, v2, v3, v4, v5, v6;
    u32 rs, rd, base;

    /* 0x004db301 .. 0x004db349: seven cm_fn_00785830(self, index) calls */
    v0 = (u32)cm_fn_00785830(self, 0);
    v1 = (u32)cm_fn_00785830(self, 1);
    v2 = (u32)cm_fn_00785830(self, 2);
    v3 = (u32)cm_fn_00785830(self, 3);
    v4 = (u32)cm_fn_00785830(self, 4);
    v5 = (u32)cm_fn_00785830(self, 5);
    v6 = (u32)cm_fn_00785830(self, 6);

    /* 0x004db36c */
    cm_fn_0053e140(v1, v2, v3, (u32)buf, 0x12cu, 1u, v0);

    rs = 581u * v5;
    rd = 581u * v4;

    /* 0x004db374: cmp ebx(v6), esi(v5) */
    if (v6 == v5) {
      /* 0x004db378 */
      base = G_00ae23b4;
      {
        s32 ba = (s32)*(s8 *)(rs + base + 0x37);
        s32 bb = (s32)*(s8 *)(rd + base + 0x37);
        u32 sa = rs + base + 0x38;
        u32 sb = rd + base + 0x38;

        /* 0x004db3be */
        cm_fn_0066f4e0(DEST_00de1f64, FMT_009af260,
                       (u32)ba, (u32)ba, (u32)bb, (u32)bb, (u32)ba, (u32)ba,
                       sa, (u32)buf, sb, sa);
      }
    }
    else {
      /* 0x004db3c8 */
      base = G_00ae23b4;
      {
        u32 sa = rs + base + 0x38;
        s32 ba = (s32)*(s8 *)(rs + base + 0x37);
        s32 bb = (s32)*(s8 *)(rd + base + 0x37);
        u32 sb = rd + base + 0x38;

        /* 0x004db40c */
        cm_fn_0066f4e0(DEST_00de1f64, FMT_009af1b4,
                       (u32)bb, (u32)bb, 0u, (u32)ba, (u32)ba,
                       sb, (u32)buf, sa);
      }
    }

    /* 0x004db422: receiver [esp+0x154] = a3 */
    cm_fn_004ae660((void *)a3, DEST_00de1f64);

    /* 0x004db427: edi = 581*v4 + [0xae23b4]; name = edi+0x38 */
    rd = rd + G_00ae23b4;
    cm_fn_004ae8a0(a3, (const char *)(rd + 0x38), 0x7d5u, rd);

    /* 0x004db440: esi = 581*v5 + [0xae23b4]; name = esi+0x38 */
    rs = rs + G_00ae23b4;
    cm_fn_004ae8a0(a3, (const char *)(rs + 0x38), 0x7d5u, rs);

    /* 0x004db481 */
    base = 0x6eu * v0 + G_00ae23bc;
    if (cm_fn_0053e990(base, v1, v2, v3, 0) != 0) {
      /* 0x004db491 reloads the base after 0x004db481. */
      cm_fn_004ae8a0(a3, (const char *)buf, 0x7d7u,
                     0x6eu * v0 + G_00ae23bc);
    }
  }
  else {
    void *self = (void *)a1;
    u32 v0, v1, v2, v3, v5, v6, people;

    /* 0x004db4c0 .. 0x004db4fb:
       six cm_fn_00785830(self, index) calls (no index 4) */
    v0 = (u32)cm_fn_00785830(self, 0);
    v1 = (u32)cm_fn_00785830(self, 1);
    v2 = (u32)cm_fn_00785830(self, 2);
    v3 = (u32)cm_fn_00785830(self, 3);
    v5 = (u32)cm_fn_00785830(self, 5);
    v6 = (u32)cm_fn_00785830(self, 6);

    /* 0x004db516 */
    cm_fn_0053e140(v1, v2, v3, (u32)buf, 0x12cu, 1u, v0);

    /* 0x004db522: cmp esi(v6), eax(v5) */
    if (v6 == v5) {
      /* 0x004db535 */
      cm_fn_0066f4e0(DEST_00de1f64, FMT_009af178, (u32)buf);
    }
    else {
      /* 0x004db550 */
      cm_fn_0066f4e0(DEST_00de1f64, FMT_009af13c, 0x64u, (u32)buf);
    }

    /* 0x004db564: receiver [esp+0x154] = a3 */
    cm_fn_004ae660((void *)a3, DEST_00de1f64);

    /* 0x004db587 */
    people = 0x6eu * v0 + G_00ae23bc;
    if (cm_fn_0053e990(people, v1, v2, v3, 0) != 0) {
      /* 0x004db593 reloads the base after 0x004db587. */
      cm_fn_004ae8a0(a3, (const char *)buf, 0x7d7u,
                     0x6eu * v0 + G_00ae23bc);
    }
  }
}

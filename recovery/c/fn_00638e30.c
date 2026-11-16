/* Preferred-image VA 0x00638e30, image base 0x00400000.
   Partial translation of the pinned CM0102 3.9.68 listing
   (SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1c627a22c445d14d489bd9bff86).

   UNRESOLVED FIDELITY HOLD: original incoming stack bytes are not inputs
   to this C entry. With L the ESP after 0x00638e30 sub esp,0x2b0,
   0x00638f19 conditionally writes pool DWORDs at L+0x10. After the call
   at 0x00639089, 0x00639093 addresses L+0x10 and 0x0063909d through
   0x006390b5 copy sixteen DWORDs regardless of the selected count.
   If the helper leaves unwritten entries untouched, those reads observe
   original pre-existing frame bytes. This fresh scratch allocation does
   NOT preserve those bytes; its indeterminate DWORD reads are not a
   faithful representation. Passing the current gate cannot close this hold.
   An explicit original-frame input/boundary is needed outside this worker's
   authorized plain-C interface. Do not replace missing bytes with defaults,
   static storage, compiler-layout assumptions, or a shortened copy.
   Scratch keeps only the evidenced adjacency of pool and message buffers
   (local_200 at L+0xb0, local_100 at L+0x1b0). */
#include "cmre.h"

#define DAT_009cf80c CM_GLOBAL(s32, 0x009cf80c)
#define DAT_009cf810 CM_GLOBAL(s32, 0x009cf810)
#define DAT_009cf850 CM_GLOBAL(s32, 0x009cf850)
#define DAT_00ae2350 CM_GLOBAL(s32, 0x00ae2350)
#define DAT_00ae235c CM_GLOBAL(s32, 0x00ae235c)
#define DAT_00ae23b4 CM_GLOBAL(s32, 0x00ae23b4)
#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)

extern u32 CM_CDECL cm_fn_004afbd0(u32 base, u32 count, u32 wanted);
extern int * CM_CDECL cm_fn_00545880(int *param_1);
extern void CM_CDECL cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern char * CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);

u32 CM_FASTCALL cm_fn_00638e30(int param_1)
{
  u8 *obj;
  u8 scratch[0x2a0];
  u32 *pool;
  u8 *local_200;
  u8 *local_100;
  s32 esi;
  s32 local_2a8;
  s32 local_2a4;
  u32 pv;
  u32 cur;
  u32 vp;
  u32 vt;
  u8 *pb;
  u8 b1;
  u8 b2;
  s32 cmp;
  u32 uVar3;

  obj = (u8 *)(u32)param_1;
  pool = (u32 *)scratch;
  local_200 = scratch + 0xa0;
  local_100 = scratch + 0x1a0;

  esi = 0;
  *(u16 *)(obj + 0x3e) = 0x14;
  pv = (u32)cm_fn_00944e46(0x78);
  *(volatile u32 *)(obj + 0xa7) = pv;
  if (pv == 0) {
    cm_fn_00944cff((u32)(char *)0x009e9f84, 0, 0, local_100, local_200);
    uVar3 = (u32)cm_fn_0090d130();
    cm_fn_00944c9f((u32)(&DAT_00ae24d0), (u32)(char *)0x009870e8, uVar3,
                   (u32)local_100, (u32)local_200, 0x11a);
    cm_fn_005e8290((char *)0x009870e0, (const char *)(&DAT_00ae24d0), 0);
    DAT_00b67a34 = 0;
    return 0xffffffff;
  }

  local_2a8 = 0;
  if ((s32)((u32)DAT_00ae235c - ((u32)DAT_00ae2350 + (u32)DAT_00ae2350)) > 0) {
    cur = (u32)DAT_00ae23b4;
    local_2a4 = 0;
    do {
      vp = *(volatile u32 *)(cur + 0x57);
      if (vp != 0) {
        if ((s16)esi < 0x28) {
          if (*(volatile u32 *)vp == (u32)DAT_009cf850) {
            pool[(s16)esi] = cur;
            esi = esi + 1;
            goto loop_next;
          }
        }
        /* 0x00638f26: vp non-null was already established at 0x00638efb. */
        vt = *(volatile u32 *)vp;
        if (vt == (u32)DAT_009cf80c || vt == (u32)DAT_009cf810) {
          pb = (u8 *)(u32)cm_fn_00545880((int *)cur);

          /* Cork City */
          {
            u8 *p = pb;
            char *q = (char *)0x009e9ff4;
            for (;;) {
              b1 = *p;
              b2 = b1 < (u8)*q;
              if (b1 != (u8)*q) {
                cmp = (s32)((1 - (u32)b2) - (u32)(b2 != 0));
                break;
              }
              if (b1 == 0) { cmp = 0; break; }
              b1 = p[1];
              b2 = b1 < (u8)q[1];
              if (b1 != (u8)q[1]) {
                cmp = (s32)((1 - (u32)b2) - (u32)(b2 != 0));
                break;
              }
              p = p + 2;
              q = q + 2;
              if (b1 == 0) { cmp = 0; break; }
            }
          }
          if (cmp != 0) {
            /* Cobh Ramblers */
            {
              u8 *p = pb;
              char *q = (char *)0x009e9fe4;
              for (;;) {
                b1 = *p;
                b2 = b1 < (u8)*q;
                if (b1 != (u8)*q) {
                  cmp = (s32)((1 - (u32)b2) - (u32)(b2 != 0));
                  break;
                }
                if (b1 == 0) { cmp = 0; break; }
                b1 = p[1];
                b2 = b1 < (u8)q[1];
                if (b1 != (u8)q[1]) {
                  cmp = (s32)((1 - (u32)b2) - (u32)(b2 != 0));
                  break;
                }
                p = p + 2;
                q = q + 2;
                if (b1 == 0) { cmp = 0; break; }
              }
            }
            if (cmp != 0) {
              /* Waterford United */
              {
                u8 *p = pb;
                char *q = (char *)0x009e9fd0;
                for (;;) {
                  b1 = *p;
                  b2 = b1 < (u8)*q;
                  if (b1 != (u8)*q) {
                    cmp = (s32)((1 - (u32)b2) - (u32)(b2 != 0));
                    break;
                  }
                  if (b1 == 0) { cmp = 0; break; }
                  b1 = p[1];
                  b2 = b1 < (u8)q[1];
                  if (b1 != (u8)q[1]) {
                    cmp = (s32)((1 - (u32)b2) - (u32)(b2 != 0));
                    break;
                  }
                  p = p + 2;
                  q = q + 2;
                  if (b1 == 0) { cmp = 0; break; }
                }
              }
              if (cmp != 0) {
                /* Limerick F.C. */
                {
                  u8 *p = pb;
                  char *q = (char *)0x009e9fc0;
                  for (;;) {
                    b1 = *p;
                    b2 = b1 < (u8)*q;
                    if (b1 != (u8)*q) {
                      cmp = (s32)((1 - (u32)b2) - (u32)(b2 != 0));
                      break;
                    }
                    if (b1 == 0) { cmp = 0; break; }
                    b1 = p[1];
                    b2 = b1 < (u8)q[1];
                    if (b1 != (u8)q[1]) {
                      cmp = (s32)((1 - (u32)b2) - (u32)(b2 != 0));
                      break;
                    }
                    p = p + 2;
                    q = q + 2;
                    if (b1 == 0) { cmp = 0; break; }
                  }
                }
                if (cmp != 0) goto loop_next;
              }
            }
          }
          /* 0x00639025 / 0x0063902e */
          if (local_2a4 < 0x18) {
            u32 hp = *(volatile u32 *)(obj + 0xa7);
            *(u32 *)((u32)local_2a4 + hp + 0x60) = cur;
            hp = *(volatile u32 *)(obj + 0xa7);
            *(u8 *)((u32)local_2a4 + hp + 0x65) = 0;
            local_2a4 = local_2a4 + 6;
          }
        }
      }
    loop_next:
      local_2a8 = local_2a8 + 1;
      cur = cur + 0x245;
    } while (local_2a8 <
             (s32)((u32)DAT_00ae235c - ((u32)DAT_00ae2350 + (u32)DAT_00ae2350)));
  }

  cm_fn_004afbd0((u32)pool, (u32)esi, 0x10);
  {
    u32 *src = pool;
    u32 off = 0;
    do {
      u32 val = *src;
      u32 hp;
      src = src + 1;
      hp = *(volatile u32 *)(obj + 0xa7);
      *(u32 *)(off + hp) = val;
      hp = *(volatile u32 *)(obj + 0xa7);
      *(u8 *)(off + hp + 5) = 0;
      off = off + 6;
    } while (off < 0x60);
  }
  return 1;
}

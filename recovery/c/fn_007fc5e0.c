/* fn_007fc5e0: thiscall (ECX = object, read before written), no stack arguments, ret 0.
   Preferred-image VA 0x007fc5e0, image base 0x00400000.
   The indirect call through [[this+0x7e2+bp*0x300]+4] has no mov ecx before it in the
   listing, so no receiver is passed on its behalf; ECX there is inherited. */
#include "cmre.h"

extern void CM_FASTCALL cm_fn_007fef10(volatile u8 *self);
extern void CM_CDECL cm_fn_0090ba90(void *arg1, void *arg2, u32 count);

void CM_THISCALL cm_fn_007fc5e0(u8 *param_1)
{
  u8 *p;        /* [esp+0x10]: pointer into the count table, advanced by 2 */
  u8 *blk;      /* ebx: block base, advanced by 0x300 */
  u32 cnt;      /* [esp+0x14]: 0x10 * outer index */
  s16 bp;       /* ebp: outer index, compared as 16 bits */
  s16 si;       /* esi: inner index, compared as 16 bits */
  u32 eax;
  u32 vt;
  u32 target;

  p = param_1 + 0x380a;
  blk = param_1 + 0x7e2;
  cnt = 0;
  bp = 0;
  do {
    si = 0;
    if ((s16)*(s16 *)p > 0) {
      do {
        *(s16 *)(param_1 + 0x382a) = bp;
        if (si != 0) {
          eax = (u32)(s32)si + cnt;
          eax = eax * 3u;
          eax = eax << 4;
          cm_fn_0090ba90((void *)(param_1 + 0x7d4 + eax), (void *)(blk - 0xe), 0x30u);
        }
        vt = *(u32 *)blk;
        if (vt != 0u) {
          target = *(u32 *)(vt + 4u);
          if (target != 0u) {
            ((void (*)(void))target)();
          }
        }
        cm_fn_007fef10((volatile u8 *)param_1);
        si = (s16)(si + 1);
      } while (si < (s16)*(s16 *)p);
    }
    p = p + 2;
    cnt = cnt + 0x10u;
    blk = blk + 0x300;
    bp = (s16)(bp + 1);
  } while (bp < 0x10);
}

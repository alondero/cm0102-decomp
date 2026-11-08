/* Preferred-image VA 0x004cd030, base 0x00400000; CM0102 3.9.68,
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Physical EAX/unused EDX/ECX prefix preserves the entry-EAX null exit.
   Native two stack arguments and RET8; original callees remain boundaries. */
#include "cmre.h"

extern u32 CM_CDECL cm_fn_00540a50(u32 club, u32 direction, u32 require_flag);
extern u32 CM_THISCALL cm_fn_008e3db0(u32 receiver, u32 *param_1, u32 *param_2,
                                       s8 param_3, s32 param_4);

__attribute__((regparm(3)))
u32 CM_STDCALL cm_fn_004cd030(u32 incoming_eax, u32 unused_edx,
                             u8 *self, u8 *param_1, u32 param_2)
{
  volatile u32 out0;
  u32 club;
  u32 eax;
  u32 limit;
  u32 edx;
  u32 ebp;
  u32 *elem;
  u8 *obj;
  u8 pass;
  u32 rec;
  u32 receiver;

  (void)unused_edx;
  obj = param_1;
  eax = incoming_eax;
  if (obj == 0) {
    return eax;
  }

  eax = cm_fn_00540a50((u32)obj, (u32)&out0, 1u);
  club = eax;
  if (club != 0u) {
    eax = out0;
    if (eax == 0u) {
      return eax;
    }
  }

  limit = CM_GLOBAL(volatile u32, 0x00ae2364);
  eax = eax & 0xffffff00u;
  pass = 0u;
  do {
    if (pass == 1u) {
      obj = (u8 *)club;
    }
    if (obj != 0) {
      elem = (u32 *)(obj + 0xd7u);
      for (ebp = 0x32u; ebp != 0u; ebp--, elem++) {
        eax = *(volatile u32 *)elem;
        if (eax == 0u) {
          continue;
        }
        edx = *(volatile u32 *)eax;
        if ((s32)edx >= (s32)limit) {
          continue;
        }
        eax = edx;
        if ((s32)eax >= (s32)limit) {
          continue;
        }
        edx = eax * 79u;
        eax = CM_GLOBAL(volatile u32, 0x00ae2d0c);
        eax = *(volatile u32 *)(edx + eax);
        if ((s32)eax < 0) {
          continue;
        }
        edx = (u32)self;
        eax = eax * 80u;
        eax = eax + *(volatile u32 *)edx;
        if (eax == 0u) {
          continue;
        }
        edx = *(volatile u32 *)(eax + 4u);
        if (edx != *(volatile u32 *)obj) {
          continue;
        }
        if (*(volatile u8 *)(eax + 0x1fu) != 1u) {
          continue;
        }
        *(volatile u8 *)(eax + 0x1fu) = 2u;
        rec = eax;
        receiver = CM_GLOBAL(volatile u32, 0x00de1f54);
        eax = *(volatile u32 *)elem;
        eax = cm_fn_008e3db0(receiver, (u32 *)eax, (u32 *)param_2, 3,
                       (s32)rec);
        limit = CM_GLOBAL(volatile u32, 0x00ae2364);
      }
    }
    eax = (eax & 0xffffff00u) | (u32)pass;
    pass = (u8)(pass + 1u);
    eax = (eax & 0xffffff00u) | (u32)pass;
  } while ((s32)pass < 2);

  return eax;
}

/* WIP: caller-visible EAX mismatch, case 72 (param_1 == 0 exit returns the entry EAX)
   fn_004cd030: thiscall (ECX = object, read before written), two stack arguments
   (ret 8). Preferred-image VA 0x004cd030, image base 0x00400000.
   Stack arguments: param_1 (entry [esp+4]) is an object pointer; param_2 (entry
   [esp+8]) is a raw u32 passed on to 0x008e3db0. The original stores its
   saved-ECX copy at [esp-8] and uses it as the object for the record scan
   (named self here by its role). Its local frame is 0x0c bytes: the out-value
   for 0x00540a50 at frame offset 0, the saved club pointer at offset 8, and the
   pass counter byte in the original param_1 slot.
   EAX on the param_1 == 0 exit is the entry EAX (the listing does not write it
   before `je 0x4cd111`); plain C has no source for it. */
#include "cmre.h"

extern u32 CM_CDECL cm_fn_00540a50(u32 club, u32 direction, u32 require_flag);
extern void CM_THISCALL cm_fn_008e3db0(u32 receiver, u32 *param_1, u32 *param_2,
                                       s8 param_3, s32 param_4);

u32 CM_THISCALL cm_fn_004cd030(u8 *self, u8 *param_1, u32 param_2)
{
  u32 out0;
  u32 club;
  u32 eax;
  u32 limit;
  u32 edx;
  u32 ebp;
  u32 *elem;
  u8 *obj;
  u8 pass;
  u32 rec;

  obj = param_1;
  eax = 0u; /* WIP: entry EAX on the param_1 == 0 exit has no plain-C source */
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

  limit = CM_GLOBAL(u32, 0x00ae2364);
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
        eax = CM_GLOBAL(u32, 0x00ae2d0c);
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
        *(u8 *)(eax + 0x1fu) = 2u;
        rec = eax;
        eax = *(volatile u32 *)elem;
        cm_fn_008e3db0(CM_GLOBAL(u32, 0x00de1f54), (u32 *)eax, (u32 *)param_2, 3,
                       (s32)rec);
        limit = CM_GLOBAL(u32, 0x00ae2364);
      }
    }
    eax = (eax & 0xffffff00u) | (u32)pass;
    pass = (u8)(pass + 1u);
    eax = (eax & 0xffffff00u) | (u32)pass;
  } while ((s32)pass < 2);

  return eax;
}

/* Draft for preferred-image VA 0x00694440, image base 0x00400000, converted from Ghidra 12.1.4
   pseudocode by `cmre draft`. Unvalidated: check names, widths and signedness against
   `cmre show --asm`; acceptance comes only from `cmre test --c` and `cmre accept --c`. */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0
#define DAT_009cfa10 CM_GLOBAL(volatile s32, 0x009cfa10)
#define DAT_00adadfc CM_GLOBAL(volatile u32, 0x00adadfc)
#define e_g_Juvent_00a03038 ((char *)0x00a03038)
#define e_g_Juvent_00a030a4 ((char *)0x00a030a4)
#define e_g_Juvent_00a03118 ((char *)0x00a03118)
#define e_g_Juvent_00a031cc ((char *)0x00a031cc)
#define e_g_Juvent_00a03258 ((char *)0x00a03258)
#define e_g_Juvent_00a0331c ((char *)0x00a0331c)
#define e_g_Juvent_00a033ac ((char *)0x00a033ac)
#define e_g_Juvent_00a03480 ((char *)0x00a03480)
#define e_g_Juvent_00a034ec ((char *)0x00a034ec)
#define e_g_Juvent_00a03598 ((char *)0x00a03598)
#define e_g_Juvent_00a0361c ((char *)0x00a0361c)
#define e_g_Juvent_00a036d8 ((char *)0x00a036d8)
#define e_g_Juvent_00a03760 ((char *)0x00a03760)
#define e_g_Juvent_00a03828 ((char *)0x00a03828)
#define e_g_Juvent_00a038a0 ((char *)0x00a038a0)
#define e_g_Juvent_00a03958 ((char *)0x00a03958)
#define e_g_Juvent_00a039e8 ((char *)0x00a039e8)
#define e_g_Juvent_00a03ab0 ((char *)0x00a03ab0)
#define e_g_Juvent_00a03b44 ((char *)0x00a03b44)
#define e_g_Juvent_00a03c18 ((char *)0x00a03c18)
#define e_g_Juvent_00a03c7c ((char *)0x00a03c7c)
#define e_g_Juvent_00a03d18 ((char *)0x00a03d18)
#define e_g_Juvent_00a03d9c ((char *)0x00a03d9c)
#define e_g_Juvent_00a03e58 ((char *)0x00a03e58)
#define e_g_Juvent_00a03edc ((char *)0x00a03edc)
#define e_g_Juvent_00a03f98 ((char *)0x00a03f98)
#define e_g_Juvent_00a03ffc ((char *)0x00a03ffc)
#define e_g_Juvent_00a040a0 ((char *)0x00a040a0)
#define e_g_Juvent_00a04124 ((char *)0x00a04124)
#define e_g_Juvent_00a041e8 ((char *)0x00a041e8)
#define e_g_Juvent_00a04268 ((char *)0x00a04268)
#define e_g_Juvent_00a0432c ((char *)0x00a0432c)
#define e_g_Juvent_00a04390 ((char *)0x00a04390)
#define e_g_Juvent_00a04438 ((char *)0x00a04438)
#define e_g_Juvent_00a044b0 ((char *)0x00a044b0)
#define e_g_Juvent_00a04528 ((char *)0x00a04528)
#define e_g_Juvent_00a045e4 ((char *)0x00a045e4)
#define e_g_Juvent_00a046a0 ((char *)0x00a046a0)
#define e_g_Juvent_00a046f0 ((char *)0x00a046f0)
#define e_g_Juvent_00a04744 ((char *)0x00a04744)
#define e_g_Juvent_00a047c0 ((char *)0x00a047c0)
#define e_g_Juvent_00a04840 ((char *)0x00a04840)
#define e_g_Juvent_00a048a4 ((char *)0x00a048a4)
#define e_g_Juvent_00a0490c ((char *)0x00a0490c)
#define e_g_Juvent_00a049b4 ((char *)0x00a049b4)
#define e_g_Juvent_00a04a60 ((char *)0x00a04a60)
#define e_g_Juvent_00a04ae8 ((char *)0x00a04ae8)
#define e_g_Juvent_00a04b80 ((char *)0x00a04b80)
#define e_g_Juvent_00a04c4c ((char *)0x00a04c4c)
extern char * CM_CDECL cm_fn_0066f4e0(char *out, char *format, ...);
typedef s32 (CM_THISCALL *cm_fn_00694440_slot)(u32 self);
static s32 cm_fn_00694440_slot_result(u32 nested)
{
  u32 index = *(volatile u32 *)nested;
  u32 table_base = CM_GLOBAL(volatile u32, 0x00adadfc);
  u32 receiver = *(volatile u32 *)(table_base + index * 4u);
  u32 vtable = *(volatile u32 *)receiver;
  cm_fn_00694440_slot slot = *(cm_fn_00694440_slot volatile *)(vtable + 0x98u);
  return slot(receiver);
}
static s32 cm_fn_00694440_has_club(u32 object)
{
  u32 club = *(volatile u32 *)(object + 0x53u);
  if (club == 0) return 0;
  u32 chain = *(volatile u32 *)(club + 0x71u);
  if (chain == 0) return 0;
  u32 identity = *(volatile u32 *)chain;
  s32 sentinel = CM_GLOBAL(volatile s32, 0x009cfa10);
  return (s32)identity != sentinel;
}
static char *cm_fn_00694440_emit(char *out, char *format, u32 object)
{
  s32 code = *(volatile s8 *)(object + 0x37u);
  u32 suffix = object + 4u;
  return cm_fn_0066f4e0(out, format, code, code, suffix);
}

char * CM_STDCALL cm_fn_00694440(u32 param_1,u32 param_2,char *param_3,u32 param_4)

{
  u32 nested;
  (void)param_4;
  int iVar2;
  
  *(volatile u8 *)param_3 = 0;
  if ((param_2 & 1) == 0) {
    if ((param_2 & 2) != 0) {
      if (((param_2 & 0x400) == 0) && ((param_2 & 0x100) == 0)) {
        if ((param_2 & 0x800) == 0) {
          nested = *(volatile u32 *)(param_1 + 0x57);
          if (nested != 0) {
            iVar2 = cm_fn_00694440_slot_result(nested);
            if (iVar2 != 0) {
              return cm_fn_00694440_emit(param_3, e_g_Juvent_00a044b0, param_1);

            }
          }
          return cm_fn_00694440_emit(param_3, e_g_Juvent_00a04438, param_1);

        }
        nested = *(volatile u32 *)(param_1 + 0x57);
        if (nested != 0) {
          iVar2 = cm_fn_00694440_slot_result(nested);
          if (iVar2 != 0) {
            return cm_fn_00694440_emit(param_3, e_g_Juvent_00a045e4, param_1);

          }
        }
        return cm_fn_00694440_emit(param_3, e_g_Juvent_00a04528, param_1);

      }
      if ((param_2 & 0x800) != 0) {
        return cm_fn_00694440_emit(param_3, e_g_Juvent_00a04390, param_1);

      }
      return cm_fn_00694440_emit(param_3, e_g_Juvent_00a0432c, param_1);

    }
    if ((param_2 & 4) != 0) {
      if ((param_2 & 0x400) != 0) {
        if ((param_2 & 0x800) != 0) {
          return cm_fn_00694440_emit(param_3, e_g_Juvent_00a04268, param_1);

        }
        return cm_fn_00694440_emit(param_3, e_g_Juvent_00a041e8, param_1);

      }
      if ((param_2 & 0x100) == 0) {
        if ((param_2 & 0x800) != 0) {
          return cm_fn_00694440_emit(param_3, e_g_Juvent_00a03ffc, param_1);

        }
        return cm_fn_00694440_emit(param_3, e_g_Juvent_00a03f98, param_1);

      }
      if ((param_2 & 0x800) != 0) {
        return cm_fn_00694440_emit(param_3, e_g_Juvent_00a04124, param_1);

      }
      return cm_fn_00694440_emit(param_3, e_g_Juvent_00a040a0, param_1);

    }
    if ((param_2 & 8) != 0) {
      if ((param_2 & 0x400) != 0) {
        if ((param_2 & 0x800) != 0) {
          return cm_fn_00694440_emit(param_3, e_g_Juvent_00a03edc, param_1);

        }
        return cm_fn_00694440_emit(param_3, e_g_Juvent_00a03e58, param_1);

      }
      if ((param_2 & 0x100) == 0) {
        if ((param_2 & 0x800) != 0) {
          return cm_fn_00694440_emit(param_3, e_g_Juvent_00a03c7c, param_1);

        }
        return cm_fn_00694440_emit(param_3, e_g_Juvent_00a03c18, param_1);

      }
      if ((param_2 & 0x800) != 0) {
        return cm_fn_00694440_emit(param_3, e_g_Juvent_00a03d9c, param_1);

      }
      return cm_fn_00694440_emit(param_3, e_g_Juvent_00a03d18, param_1);

    }
    if ((param_2 & 0x40) != 0) {
      if ((param_2 & 0x400) != 0) {
        if ((param_2 & 0x800) != 0) {
          return cm_fn_00694440_emit(param_3, e_g_Juvent_00a03b44, param_1);

        }
        return cm_fn_00694440_emit(param_3, e_g_Juvent_00a03ab0, param_1);

      }
      if ((param_2 & 0x100) == 0) {
        if ((param_2 & 0x800) != 0) {
          return cm_fn_00694440_emit(param_3, e_g_Juvent_00a038a0, param_1);

        }
        return cm_fn_00694440_emit(param_3, e_g_Juvent_00a03828, param_1);

      }
      if ((param_2 & 0x800) != 0) {
        return cm_fn_00694440_emit(param_3, e_g_Juvent_00a039e8, param_1);

      }
      return cm_fn_00694440_emit(param_3, e_g_Juvent_00a03958, param_1);

    }
    if ((param_2 & 0x10) != 0) {
      if ((param_2 & 0x400) != 0) {
        if ((param_2 & 0x800) != 0) {
          return cm_fn_00694440_emit(param_3, e_g_Juvent_00a03760, param_1);

        }
        return cm_fn_00694440_emit(param_3, e_g_Juvent_00a036d8, param_1);

      }
      if ((param_2 & 0x100) == 0) {
        if ((param_2 & 0x800) != 0) {
          return cm_fn_00694440_emit(param_3, e_g_Juvent_00a034ec, param_1);

        }
        return cm_fn_00694440_emit(param_3, e_g_Juvent_00a03480, param_1);

      }
      if ((param_2 & 0x800) != 0) {
        return cm_fn_00694440_emit(param_3, e_g_Juvent_00a0361c, param_1);

      }
      return cm_fn_00694440_emit(param_3, e_g_Juvent_00a03598, param_1);

    }
    if ((param_2 & 0x20) == 0) {
      return cm_fn_00694440_emit(param_3, e_g_Juvent_00a03038, param_1);

    }
    if ((param_2 & 0x400) != 0) {
      if ((param_2 & 0x800) != 0) {
        return cm_fn_00694440_emit(param_3, e_g_Juvent_00a033ac, param_1);

      }
      return cm_fn_00694440_emit(param_3, e_g_Juvent_00a0331c, param_1);

    }
    if ((param_2 & 0x100) == 0) {
      if ((param_2 & 0x800) != 0) {
        return cm_fn_00694440_emit(param_3, e_g_Juvent_00a03118, param_1);

      }
      return cm_fn_00694440_emit(param_3, e_g_Juvent_00a030a4, param_1);

    }
    if ((param_2 & 0x800) != 0) {
      return cm_fn_00694440_emit(param_3, e_g_Juvent_00a03258, param_1);

    }
    return cm_fn_00694440_emit(param_3, e_g_Juvent_00a031cc, param_1);

  }
  if ((param_2 & 0x400) != 0) {
    
    if ((param_2 & 0x800) != 0) {
      if (cm_fn_00694440_has_club(param_1) != 0) {
        return cm_fn_00694440_emit(param_3, e_g_Juvent_00a04c4c, param_1);

      }
      return cm_fn_00694440_emit(param_3, e_g_Juvent_00a04b80, param_1);

    }
    if (cm_fn_00694440_has_club(param_1) != 0) {
      return cm_fn_00694440_emit(param_3, e_g_Juvent_00a04ae8, param_1);

    }
    return cm_fn_00694440_emit(param_3, e_g_Juvent_00a04a60, param_1);

  }
  if ((param_2 & 0x100) == 0) {
    
    if ((param_2 & 0x800) == 0) {
      if (cm_fn_00694440_has_club(param_1) != 0) {
        return cm_fn_00694440_emit(param_3, e_g_Juvent_00a046f0, param_1);

      }
      return cm_fn_00694440_emit(param_3, e_g_Juvent_00a046a0, param_1);

    }
    if (cm_fn_00694440_has_club(param_1) != 0) {
      return cm_fn_00694440_emit(param_3, e_g_Juvent_00a047c0, param_1);

    }
    return cm_fn_00694440_emit(param_3, e_g_Juvent_00a04744, param_1);

  }
  
  if ((param_2 & 0x800) != 0) {
    if (cm_fn_00694440_has_club(param_1) != 0) {
      return cm_fn_00694440_emit(param_3, e_g_Juvent_00a049b4, param_1);

    }
    return cm_fn_00694440_emit(param_3, e_g_Juvent_00a0490c, param_1);

  }
  if (cm_fn_00694440_has_club(param_1) != 0) {
    return cm_fn_00694440_emit(param_3, e_g_Juvent_00a048a4, param_1);

  }
  return cm_fn_00694440_emit(param_3, e_g_Juvent_00a04840, param_1);

}

/* Draft for preferred-image VA 0x006177f0, image base 0x00400000, converted from Ghidra 12.1.4
   pseudocode by `cmre draft`. Unvalidated: check names, widths and signedness against
   `cmre show --asm`; acceptance comes only from `cmre test --c` and `cmre accept --c`. */
#include "cmre.h"
#define DAT_009cfa08 CM_GLOBAL(volatile int, 0x009cfa08)
#define DAT_009cfa0c CM_GLOBAL(volatile int, 0x009cfa0c)
#define DAT_009cfa10 CM_GLOBAL(volatile int, 0x009cfa10)
#define DAT_009cfa14 CM_GLOBAL(volatile int, 0x009cfa14)
#define DAT_009cfa18 CM_GLOBAL(volatile int, 0x009cfa18)
#define DAT_009d086c CM_GLOBAL(volatile u8, 0x009d086c)
#define DAT_009d0878 CM_GLOBAL(volatile u32, 0x009d0878)
#define DAT_009d0884 CM_GLOBAL(volatile u8, 0x009d0884)
#define DAT_009d0890 CM_GLOBAL(volatile u32, 0x009d0890)
#define DAT_009d0b2c CM_GLOBAL(volatile u32, 0x009d0b2c)
#define DAT_009d0b38 CM_GLOBAL(volatile u8, 0x009d0b38)
#define DAT_009d0b44 CM_GLOBAL(volatile u32, 0x009d0b44)
#define DAT_009d0b50 CM_GLOBAL(volatile u8, 0x009d0b50)
#define DAT_009d0d08 CM_GLOBAL(volatile u8, 0x009d0d08)
#define DAT_009d0d14 CM_GLOBAL(volatile u8, 0x009d0d14)
#define DAT_009d0d20 CM_GLOBAL(volatile u32, 0x009d0d20)
#define DAT_009d0d2c CM_GLOBAL(volatile u8, 0x009d0d2c)
#define s_Africana_009d104c ((char *)0x009d104c)
#define s_Africanas_009d0d38 ((char *)0x009d0d38)
#define s_Africane_009d1040 ((char *)0x009d1040)
#define s_Africani_009d1058 ((char *)0x009d1058)
#define s_Africano_009d1064 ((char *)0x009d1064)
#define s_Africanos_009d0d44 ((char *)0x009d0d44)
#define s_Asiatica_009d101c ((char *)0x009d101c)
#define s_Asiatiche_009d1010 ((char *)0x009d1010)
#define s_Asiatici_009d1028 ((char *)0x009d1028)
#define s_Asiatico_009d1034 ((char *)0x009d1034)
#define s_Europea_009e20ec ((char *)0x009e20ec)
#define s_Europeas_009e20d4 ((char *)0x009e20d4)
#define s_Europee_009e20c4 ((char *)0x009e20c4)
#define s_Europei_009e20cc ((char *)0x009e20cc)
#define s_Europeia_009e2114 ((char *)0x009e2114)
#define s_Europeias_009e20fc ((char *)0x009e20fc)
#define s_Europeo_009e20f4 ((char *)0x009e20f4)
#define s_Europeos_009e20e0 ((char *)0x009e20e0)
#define s_Europeu_009e2120 ((char *)0x009e2120)
#define s_Europeus_009e2108 ((char *)0x009e2108)
#define s_Nord_Americana_009d0e90 ((char *)0x009d0e90)
#define s_Nord_Americane_009d0e80 ((char *)0x009d0e80)
#define s_Nord_Americani_009d0ea0 ((char *)0x009d0ea0)
#define s_Nord_Americano_009d0eb0 ((char *)0x009d0eb0)
#define s_Norte_Americana_009d0b84 ((char *)0x009d0b84)
#define s_Norte_Americanas_009d0b5c ((char *)0x009d0b5c)
#define s_Norte_Americano_009d0b94 ((char *)0x009d0b94)
#define s_Norte_Americanos_009d0b70 ((char *)0x009d0b70)
#define s_Norteamericana_009d08bc ((char *)0x009d08bc)
#define s_Norteamericanas_009d089c ((char *)0x009d089c)
#define s_Norteamericano_009d08cc ((char *)0x009d08cc)
#define s_Norteamericanos_009d08ac ((char *)0x009d08ac)
#define s_Oceanica_009d0e5c ((char *)0x009d0e5c)
#define s_Oceaniche_009d0e50 ((char *)0x009d0e50)
#define s_Oceanici_009d0e68 ((char *)0x009d0e68)
#define s_Oceanico_009d0e74 ((char *)0x009d0e74)
#define s_Sud_Americana_009d0df0 ((char *)0x009d0df0)
#define s_Sud_Americane_009d0de0 ((char *)0x009d0de0)
#define s_Sud_Americani_009d0e00 ((char *)0x009d0e00)
#define s_Sud_Americano_009d0e10 ((char *)0x009d0e10)
#define s_Sudamericana_009d084c ((char *)0x009d084c)
#define s_Sudamericanas_009d082c ((char *)0x009d082c)
#define s_Sudamericano_009d085c ((char *)0x009d085c)
#define s_Sudamericanos_009d083c ((char *)0x009d083c)
#define s_Sul_Americana_009d0ad4 ((char *)0x009d0ad4)
#define s_Sul_Americanas_009d0ab4 ((char *)0x009d0ab4)
#define s_Sul_Americano_009d0ae4 ((char *)0x009d0ae4)
#define s_Sul_Americanos_009d0ac4 ((char *)0x009d0ac4)
extern unsigned long long CM_CDECL cm_fn_00545a60(u32 arg1,u32 arg2);
extern u32 CM_CDECL cm_fn_0066f4d0(void);

static u32 cm_fn_006177f0_copy(u32 destination, u32 source)
{
  u32 length = 0;
  for (;;) {
    u8 byte = *(volatile u8 *)(source + length);
    ++length;
    if (byte == 0) break;
  }
  u32 words = length >> 2;
  for (u32 i = 0; i < words; ++i) {
    u32 value = *(volatile u32 *)(source + i * 4u);
    *(volatile u32 *)(destination + i * 4u) = value;
  }
  for (u32 i = words * 4u; i < length; ++i) {
    u8 value = *(volatile u8 *)(source + i);
    *(volatile u8 *)(destination + i) = value;
  }
  return 1;
}

u32 CM_CDECL cm_fn_006177f0(u32 param_1,u32 param_2,s16 param_3,u32 param_4)

{
  char cVar1;
  int iVar2;
  volatile char *pcVar5;
  u8 bVar7;
  
  *(volatile u8 *)param_2 = 0;
  if (((param_1 == 0) || (param_2 == 0)) || (param_3 < 0x32)) {
    return 0;
  }
  if ((char)param_4 == '\0') {
    pcVar5 = (volatile char *)(param_1 + 0x23u);
    goto LAB_00617be2;
  }
  iVar2 = cm_fn_0066f4d0();
  if (iVar2 == 9) {
    cVar1 = cm_fn_00545a60(param_4,9);
    iVar2 = *(volatile s32 *)param_1;
    if (iVar2 == DAT_009cfa08) {
      if (cVar1 == '\0') {
        pcVar5 = s_Africano_009d1064;
        goto LAB_0061819a;
      }
      if (cVar1 == '\x01') {
        pcVar5 = s_Africana_009d104c;
      }
      else {
        if (cVar1 == '\x02') {
          pcVar5 = s_Africanos_009d0d44;
          goto LAB_0061819a;
        }
        pcVar5 = s_Africanas_009d0d38;
      }
      goto LAB_00617be2;
    }
    if (iVar2 != DAT_009cfa0c) {
      if (iVar2 == DAT_009cfa10) {
        if (cVar1 == '\0') {
          pcVar5 = s_Europeu_009e2120;
        }
        else {
          if (cVar1 == '\x01') {
            pcVar5 = s_Europeia_009e2114;
            return cm_fn_006177f0_copy(param_2, (u32)(unsigned long)pcVar5);
          }
          if (cVar1 != '\x02') {
            pcVar5 = s_Europeias_009e20fc;
            return cm_fn_006177f0_copy(param_2, (u32)(unsigned long)pcVar5);
          }
          pcVar5 = s_Europeus_009e2108;
        }
      }
      else if (iVar2 == DAT_009cfa14) {
        if (cVar1 == '\0') {
          pcVar5 = s_Norte_Americano_009d0b94;
        }
        else {
          if (cVar1 == '\x01') {
            pcVar5 = s_Norte_Americana_009d0b84;
            return cm_fn_006177f0_copy(param_2, (u32)(unsigned long)pcVar5);
          }
          if (cVar1 != '\x02') {
            pcVar5 = s_Norte_Americanas_009d0b5c;
            return cm_fn_006177f0_copy(param_2, (u32)(unsigned long)pcVar5);
          }
          pcVar5 = s_Norte_Americanos_009d0b70;
        }
      }
      else if (iVar2 == DAT_009cfa18) {
        if (cVar1 == '\0') {
          pcVar5 = (volatile char *)&DAT_009d0b50;
        }
        else {
          if (cVar1 == '\x01') {
            pcVar5 = (volatile char *)&DAT_009d0b44;
            return cm_fn_006177f0_copy(param_2, (u32)(unsigned long)pcVar5);
          }
          if (cVar1 != '\x02') {
            pcVar5 = (volatile char *)&DAT_009d0b2c;
            return cm_fn_006177f0_copy(param_2, (u32)(unsigned long)pcVar5);
          }
          pcVar5 = (volatile char *)&DAT_009d0b38;
        }
      }
      else if (cVar1 == '\0') {
        pcVar5 = s_Sul_Americano_009d0ae4;
      }
      else {
        if (cVar1 == '\x01') {
          pcVar5 = s_Sul_Americana_009d0ad4;
            return cm_fn_006177f0_copy(param_2, (u32)(unsigned long)pcVar5);
        }
        if (cVar1 != '\x02') {
          pcVar5 = s_Sul_Americanas_009d0ab4;
            return cm_fn_006177f0_copy(param_2, (u32)(unsigned long)pcVar5);
        }
        pcVar5 = s_Sul_Americanos_009d0ac4;
      }
      goto LAB_0061819a;
    }
    if (cVar1 == '\0') {
      pcVar5 = (volatile char *)&DAT_009d0d2c;
      goto LAB_0061819a;
    }
    if (cVar1 == '\x01') {
      pcVar5 = (volatile char *)&DAT_009d0d20;
      goto LAB_00617be2;
    }
    bVar7 = cVar1 == '\x02';
  }
  else {
    iVar2 = cm_fn_0066f4d0();
    if (iVar2 != 5) {
      iVar2 = cm_fn_0066f4d0();
      if (iVar2 == 4) {
        cVar1 = cm_fn_00545a60(param_4,4);
        iVar2 = *(volatile s32 *)param_1;
        if (iVar2 == DAT_009cfa08) {
          if (cVar1 == '\0') {
            pcVar5 = s_Africano_009d1064;
          }
          else {
            if (cVar1 == '\x01') {
              pcVar5 = s_Africani_009d1058;
            return cm_fn_006177f0_copy(param_2, (u32)(unsigned long)pcVar5);
            }
            if (cVar1 != '\x02') {
              pcVar5 = s_Africane_009d1040;
            return cm_fn_006177f0_copy(param_2, (u32)(unsigned long)pcVar5);
            }
            pcVar5 = s_Africana_009d104c;
          }
        }
        else if (iVar2 == DAT_009cfa0c) {
          if (cVar1 == '\0') {
            pcVar5 = s_Asiatico_009d1034;
          }
          else {
            if (cVar1 == '\x01') {
              pcVar5 = s_Asiatici_009d1028;
            return cm_fn_006177f0_copy(param_2, (u32)(unsigned long)pcVar5);
            }
            if (cVar1 != '\x02') {
              pcVar5 = s_Asiatiche_009d1010;
            return cm_fn_006177f0_copy(param_2, (u32)(unsigned long)pcVar5);
            }
            pcVar5 = s_Asiatica_009d101c;
          }
        }
        else if (iVar2 == DAT_009cfa10) {
          if (cVar1 == '\0') {
            pcVar5 = s_Europeo_009e20f4;
          }
          else {
            if (cVar1 == '\x01') {
              pcVar5 = s_Europei_009e20cc;
            return cm_fn_006177f0_copy(param_2, (u32)(unsigned long)pcVar5);
            }
            if (cVar1 != '\x02') {
              pcVar5 = s_Europee_009e20c4;
            return cm_fn_006177f0_copy(param_2, (u32)(unsigned long)pcVar5);
            }
            pcVar5 = s_Europea_009e20ec;
          }
        }
        else if (iVar2 == DAT_009cfa14) {
          if (cVar1 == '\0') {
            pcVar5 = s_Nord_Americano_009d0eb0;
          }
          else {
            if (cVar1 == '\x01') {
              pcVar5 = s_Nord_Americani_009d0ea0;
            return cm_fn_006177f0_copy(param_2, (u32)(unsigned long)pcVar5);
            }
            if (cVar1 != '\x02') {
              pcVar5 = s_Nord_Americane_009d0e80;
            return cm_fn_006177f0_copy(param_2, (u32)(unsigned long)pcVar5);
            }
            pcVar5 = s_Nord_Americana_009d0e90;
          }
        }
        else if (iVar2 == DAT_009cfa18) {
          if (cVar1 == '\0') {
            pcVar5 = s_Oceanico_009d0e74;
          }
          else {
            if (cVar1 == '\x01') {
              pcVar5 = s_Oceanici_009d0e68;
            return cm_fn_006177f0_copy(param_2, (u32)(unsigned long)pcVar5);
            }
            if (cVar1 != '\x02') {
              pcVar5 = s_Oceaniche_009d0e50;
            return cm_fn_006177f0_copy(param_2, (u32)(unsigned long)pcVar5);
            }
            pcVar5 = s_Oceanica_009d0e5c;
          }
        }
        else if (cVar1 == '\0') {
          pcVar5 = s_Sud_Americano_009d0e10;
        }
        else {
          if (cVar1 == '\x01') {
            pcVar5 = s_Sud_Americani_009d0e00;
            return cm_fn_006177f0_copy(param_2, (u32)(unsigned long)pcVar5);
          }
          if (cVar1 != '\x02') {
            pcVar5 = s_Sud_Americane_009d0de0;
            return cm_fn_006177f0_copy(param_2, (u32)(unsigned long)pcVar5);
          }
          pcVar5 = s_Sud_Americana_009d0df0;
        }
      }
      else {
        pcVar5 = (volatile char *)(param_1 + 0x23u);
      }
      goto LAB_0061819a;
    }
    cVar1 = cm_fn_00545a60(param_4,5);
    iVar2 = *(volatile s32 *)param_1;
    if (iVar2 == DAT_009cfa08) {
      if (cVar1 == '\0') {
        pcVar5 = s_Africano_009d1064;
      }
      else {
        if (cVar1 == '\x01') {
          pcVar5 = s_Africano_009d1064;
            return cm_fn_006177f0_copy(param_2, (u32)(unsigned long)pcVar5);
        }
        if (cVar1 == '\x02') {
          pcVar5 = s_Africana_009d104c;
        }
        else {
          if (cVar1 == '\x03') {
            pcVar5 = s_Africanos_009d0d44;
            return cm_fn_006177f0_copy(param_2, (u32)(unsigned long)pcVar5);
          }
          pcVar5 = s_Africanas_009d0d38;
        }
      }
      goto LAB_0061819a;
    }
    if (iVar2 != DAT_009cfa0c) {
      if (iVar2 == DAT_009cfa10) {
        if (cVar1 == '\0') {
          pcVar5 = s_Europeo_009e20f4;
        }
        else {
          if (cVar1 == '\x01') {
            pcVar5 = s_Europeo_009e20f4;
            return cm_fn_006177f0_copy(param_2, (u32)(unsigned long)pcVar5);
          }
          if (cVar1 == '\x02') {
            pcVar5 = s_Europea_009e20ec;
          }
          else {
            if (cVar1 == '\x03') {
              pcVar5 = s_Europeos_009e20e0;
            return cm_fn_006177f0_copy(param_2, (u32)(unsigned long)pcVar5);
            }
            pcVar5 = s_Europeas_009e20d4;
          }
        }
      }
      else if (iVar2 == DAT_009cfa14) {
        if (cVar1 == '\0') {
          pcVar5 = s_Norteamericano_009d08cc;
            return cm_fn_006177f0_copy(param_2, (u32)(unsigned long)pcVar5);
        }
        if (cVar1 == '\x01') {
          pcVar5 = s_Norteamericano_009d08cc;
        }
        else {
          if (cVar1 == '\x02') {
            pcVar5 = s_Norteamericana_009d08bc;
            return cm_fn_006177f0_copy(param_2, (u32)(unsigned long)pcVar5);
          }
          if (cVar1 != '\x03') {
            pcVar5 = s_Norteamericanas_009d089c;
            return cm_fn_006177f0_copy(param_2, (u32)(unsigned long)pcVar5);
          }
          pcVar5 = s_Norteamericanos_009d08ac;
        }
      }
      else if (iVar2 == DAT_009cfa18) {
        if (cVar1 == '\0') {
          pcVar5 = (volatile char *)&DAT_009d0890;
        }
        else {
          if (cVar1 == '\x01') {
            pcVar5 = (volatile char *)&DAT_009d0890;
            return cm_fn_006177f0_copy(param_2, (u32)(unsigned long)pcVar5);
          }
          if (cVar1 == '\x02') {
            pcVar5 = (volatile char *)&DAT_009d0884;
          }
          else {
            if (cVar1 == '\x03') {
              pcVar5 = (volatile char *)&DAT_009d0878;
            return cm_fn_006177f0_copy(param_2, (u32)(unsigned long)pcVar5);
            }
            pcVar5 = (volatile char *)&DAT_009d086c;
          }
        }
      }
      else {
        if (cVar1 == '\0') {
          pcVar5 = s_Sudamericano_009d085c;
            return cm_fn_006177f0_copy(param_2, (u32)(unsigned long)pcVar5);
        }
        if (cVar1 == '\x01') {
          pcVar5 = s_Sudamericano_009d085c;
        }
        else {
          if (cVar1 == '\x02') {
            pcVar5 = s_Sudamericana_009d084c;
            return cm_fn_006177f0_copy(param_2, (u32)(unsigned long)pcVar5);
          }
          if (cVar1 != '\x03') {
            pcVar5 = s_Sudamericanas_009d082c;
            return cm_fn_006177f0_copy(param_2, (u32)(unsigned long)pcVar5);
          }
          pcVar5 = s_Sudamericanos_009d083c;
        }
      }
      goto LAB_0061819a;
    }
    if (cVar1 == '\0') {
      pcVar5 = (volatile char *)&DAT_009d0d2c;
            return cm_fn_006177f0_copy(param_2, (u32)(unsigned long)pcVar5);
    }
    if (cVar1 == '\x01') {
      pcVar5 = (volatile char *)&DAT_009d0d2c;
      goto LAB_0061819a;
    }
    if (cVar1 == '\x02') {
      pcVar5 = (volatile char *)&DAT_009d0d20;
            return cm_fn_006177f0_copy(param_2, (u32)(unsigned long)pcVar5);
    }
    bVar7 = cVar1 == '\x03';
  }
  if (bVar7) {
    pcVar5 = (volatile char *)&DAT_009d0d14;
LAB_0061819a:
  return cm_fn_006177f0_copy(param_2, (u32)(unsigned long)pcVar5);
  }
  pcVar5 = (volatile char *)&DAT_009d0d08;
LAB_00617be2:
  return cm_fn_006177f0_copy(param_2, (u32)(unsigned long)pcVar5);
}

#include "cmre.h"

/* Stdcall with four stack arguments (ret 0x10). ECX is not read before it is
   written, so there is no receiver. The function returns EAX: at each exit it is
   the arg2 pointer, except on the two exits that leave the 0x00669470 result in
   EAX (cmp al,1 / test al,al / cmp 3..7 fall-through to 8d3061 and 8d3077).

   Argument names follow the listing at entry: arg1 (esi) is the record pointer;
   arg2 receives one byte (1, 0xff, 2, 4); arg3 and arg4 receive dwords.
   Field offsets follow the listing: record+0x14 (pointer), +0x1c (pointer),
   +0x53 and +0x71 (dwords via the pointer read from +0x1c), and +0x59 via the
   record+0x14 pointer. Global 0x009cf684 is compared with the dword at *(record+0x14). */

extern unsigned long long CM_THISCALL cm_fn_005c6710(void *self, u32 arg);
extern u32 cm_fn_00669470(int *param_1);

u32 CM_STDCALL cm_fn_008d2fa0(int param_1, u8 *param_2, int *param_3, u32 *param_4)
{
    u8 *rec = (u8 *)param_1;
    u32 edx;
    u32 ecx;
    u32 q;
    u32 v;
    u32 t;
    u32 r;
    s8 cvar;

    edx = CM_GLOBAL(volatile u32, 0x009cf684);
    q = *(volatile u32 *)(rec + 0x14);
    ecx = *(volatile u32 *)q;
    if (ecx == edx) {
        edx = *(volatile u32 *)(rec + 0x1c);
        v = *(volatile u32 *)(edx + 0x53);
        *(volatile u32 *)param_3 = v;
        if (v != 0) {
            t = *(volatile u32 *)(v + 0x71);
            *(volatile u32 *)param_4 = t;
            *(volatile u8 *)param_2 = 1;
            return (u32)param_2;
        }
        *(volatile u32 *)param_4 = 0;
        *(volatile u8 *)param_2 = 1;
        return (u32)param_2;
    }

    /* Thiscall receiver: ECX = dword at 0x00ae2c74; arg = the record+0x14
       value already in EAX from 0x008d2fab (not reloaded). */
    ecx = CM_GLOBAL(volatile u32, 0x00ae2c74);
    r =(u32)cm_fn_005c6710((void *)ecx, q);
    if (r != 0) {
        *(volatile u32 *)param_3 = 0;
        *(volatile u32 *)param_4 = 0;
        *(volatile u8 *)param_2 = 0xff;
        return (u32)param_2;
    }

    q = *(volatile u32 *)(rec + 0x14);
    r = cm_fn_00669470((int *)q);
    cvar = (s8)r;
    ecx = CM_GLOBAL(volatile u32, 0x00b63d60 + (s32)cvar * 72);
    *(volatile u32 *)param_3 = ecx;
    q = *(volatile u32 *)(rec + 0x14);
    edx = *(volatile u32 *)(q + 0x59);
    *(volatile u32 *)param_4 = edx;
    if (cvar == 1) {
        *(volatile u8 *)param_2 = 4;
        return r;
    }
    if (cvar == 0) {
        *(volatile u8 *)param_2 = 4;
        return r;
    }
    if (cvar == 3 || cvar == 4 || cvar == 5 || cvar == 6 || cvar == 7) {
        *(volatile u8 *)param_2 = 2;
        return (u32)param_2;
    }
    *(volatile u8 *)param_2 = 1;
    return r;
}

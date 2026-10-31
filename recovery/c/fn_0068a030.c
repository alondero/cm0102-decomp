#include "cmre.h"

/* fn_0068a030: cdecl with seven stack arguments (ret 0), no ECX read before write.
   Preferred-image VA 0x0068a030, image base 0x00400000.
   Arguments: param_1 int, param_2 char* (a one-byte state, updated in place), param_3 compared
   with the global at 0x009cf6e8, param_4 and param_5 signed 16-bit (both must pass minimums),
   param_6 signed 16-bit range, param_7 flag byte (bit 0x40 and bit 0x80 select paths).
   The 0x0090cfc0 probe is called with 4 and its result is compared with 2 as a signed value. */

extern int cm_fn_0090cfc0(int param_1);

int cm_fn_0068a030(int param_1, char *param_2, int param_3, s16 param_4, s16 param_5, s16 param_6,
                   u8 param_7)
{
    s32 g;
    s32 esi;
    s32 c;
    u32 eax;
    s32 r;

    g = CM_GLOBAL(s32, 0x009cf6e8);
    if (param_3 == g) {
        if (param_1 == 2) {
            return 4;
        }
        if (param_1 == 5) {
            return 3;
        }
        return (int)((u32)param_1 + 1u);
    }

    if ((s16)param_5 < 2 || (s16)param_4 < 8) {
        return (int)((u32)param_1 + 1u);
    }
    if ((param_7 & 0x40u) != 0u) {
        return (int)((u32)param_1 + 1u);
    }
    if ((param_7 & 0x80u) != 0u) {
        esi = (s32)(s16)param_6;
        eax = (u32)param_1 + 1u;
        if ((s32)eax == esi / 2) {
            return (int)((u32)esi - 1u);
        }
        if ((s32)eax == esi) {
            return esi / 2;
        }
        return (int)eax;
    }

    c = (s32)*(s8 *)param_2;
    if (c == 1) {
        *param_2 = 2;
        if ((s32)param_1 - 1 >= 0) {
            return (int)((u32)param_1 - 1u);
        }
        return (int)((u32)(s16)param_6 - 1u);
    }
    if (c == 2) {
        *param_2 = 3;
        if ((s32)param_1 - 1 >= 0) {
            return (int)((u32)param_1 - 1u);
        }
        return (int)((u32)(s16)param_6 - 1u);
    }
    if (c == 3) {
        *param_2 = 0;
        eax = (u32)param_1 + 3u;
        if ((s32)eax < (s32)(s16)param_6) {
            return (int)eax;
        }
        return (int)(eax - (u32)(s16)param_6);
    }

    esi = (s32)(s16)param_6;
    if ((s32)param_1 < esi - 5) {
        r = cm_fn_0090cfc0(4);
        if (r >= 2) {
            eax = (u32)param_1 + 3u;
            *param_2 = 1;
            if ((s32)eax < esi) {
                return (int)eax;
            }
            return (int)(eax - (u32)esi);
        }
    }
    eax = (u32)param_1 + 1u;
    if ((s32)eax < esi) {
        return (int)eax;
    }
    return (int)(eax - (u32)esi);
}

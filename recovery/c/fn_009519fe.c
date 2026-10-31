#include "cmre.h"

/* Original 3.9.68 VA 0x009519fe. Convention: cdecl (ebp frame, two stack
   arguments: param_1 at [ebp+8], param_2 at [ebp+0xc]; `ret` with no pops).
   The two `push ecx` after the frame setup allocate two dword locals:
   [ebp-8] holds the '=' position (eq) and [ebp-4] holds the table pointer
   read from global 0x00de2fb4 (saved_table).

   Globals (dword pointers): 0x00de2fb4 table A, 0x00de2fb8 comparison value B,
   0x00de2fbc pointer C (allocated once, zeroed). The import through the slot
   at 0x00967048 is SetEnvironmentVariableA (stdcall, two arguments).

   Argument text: the byte after '=' is read at eq+1; the value pointer is
   eq + (name - param_1) + 1, or NULL when that byte was zero. */

typedef u32 (CM_STDCALL *fn_set_environment_variable_a)(const char *name, const char *value);

extern u32 CM_CDECL cm_fn_00951300(u32 arg_1, u32 arg_2);
extern int cm_fn_00951b85(u8 *param_1, u32 param_2);
extern u32 * cm_fn_00951bdd(int *param_1);
extern u32 cm_fn_0094df1d(void);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);
extern unsigned long long cm_fn_00945667(u32 arg8, u32 argC);
extern u32 CM_CDECL cm_fn_009488e0(u32 source);
extern u32 * cm_fn_0094c3a0(u32 *param_1, u32 *param_2);

u32 CM_CDECL cm_fn_009519fe(char *param_1, u32 param_2)
{
    u32 eq;          /* [ebp-8]: pointer to the '=' in param_1 */
    u32 saved_table; /* [ebp-4]: table pointer read from global A */
    u32 empty;       /* ebx: 1 when the byte after '=' is zero */
    u32 t;           /* eax: working pointer */
    u32 len;
    s32 index;       /* esi after the 0x00951b85 call */
    u32 table;       /* edi */
    u32 name;        /* esi: allocated name buffer */
    u32 value;
    u32 sVar4;
    u32 *cur;

    if ((u32)param_1 == 0u) {
        return 0xffffffffu;
    }
    eq = cm_fn_00951300((u32)param_1, 0x3du);
    if (eq == 0u) {
        return 0xffffffffu;
    }
    if ((u32)param_1 == eq) {
        return 0xffffffffu;
    }

    t = CM_GLOBAL(u32, 0x00de2fb4u);
    empty = (*(u8 *)(eq + 1u) == 0u) ? 1u : 0u;
    if (t == CM_GLOBAL(u32, 0x00de2fb8u)) {
        t = (u32)cm_fn_00951bdd((int *)t);
        CM_GLOBAL(u32, 0x00de2fb4u) = t;
    }

    if (t == 0u) {
        if (param_2 == 0u || CM_GLOBAL(u32, 0x00de2fbcu) == 0u) {
            if (empty != 0u) {
                return 0u;
            }
            t = (u32)cm_fn_00944e46(4u);
            CM_GLOBAL(u32, 0x00de2fb4u) = t;
            if (t == 0u) {
                return 0xffffffffu;
            }
            *(u32 *)t = 0u;
            if (CM_GLOBAL(u32, 0x00de2fbcu) == 0u) {
                t = (u32)cm_fn_00944e46(4u);
                CM_GLOBAL(u32, 0x00de2fbcu) = t;
                if (t == 0u) {
                    return 0xffffffffu;
                }
                *(u32 *)t = 0u;
            }
        } else {
            if (cm_fn_0094df1d() != 0u) {
                return 0xffffffffu;
            }
        }
    }

    len = eq - (u32)param_1;
    table = CM_GLOBAL(u32, 0x00de2fb4u);
    saved_table = table;
    index = cm_fn_00951b85((u8 *)param_1, len);

    if (index < 0 || *(u32 *)table == 0u) {
        if (empty != 0u) {
            return 0u;
        }
        if (index < 0) {
            index = (s32)(0u - (u32)index);
        }
        t = (u32)cm_fn_00945667(table, (u32)index * 4u + 8u);
        if (t == 0u) {
            return 0xffffffffu;
        }
        *(u32 *)(t + (u32)index * 4u) = (u32)param_1;
        *(u32 *)(t + (u32)index * 4u + 4u) = 0u;
        CM_GLOBAL(u32, 0x00de2fb4u) = t;
    } else {
        if (empty == 0u) {
            *(u32 *)(table + (u32)index * 4u) = (u32)param_1;
        } else {
            cm_fn_009452ca(*(u32 *)(table + (u32)index * 4u));
            cur = (u32 *)(table + (u32)index * 4u);
            while (*cur != 0u) {
                index = index + 1;
                *cur = cur[1];
                cur = cur + 1;
            }
            t = (u32)cm_fn_00945667(saved_table, (u32)index * 4u);
            if (t != 0u) {
                CM_GLOBAL(u32, 0x00de2fb4u) = t;
            }
        }
    }

    if (param_2 != 0u) {
        sVar4 = cm_fn_009488e0((u32)param_1);
        name = (u32)cm_fn_00944e46(sVar4 + 2u);
        if (name != 0u) {
            cm_fn_0094c3a0((u32 *)name, (u32 *)param_1);
            *(u8 *)(eq + (name - (u32)param_1)) = 0u;
            value = (empty != 0u) ? 0u : (eq + (name - (u32)param_1) + 1u);
            {
                fn_set_environment_variable_a set_environment =
                    CM_GLOBAL(volatile fn_set_environment_variable_a, 0x00967048u);
                set_environment((const char *)name, (const char *)value);
            }
            cm_fn_009452ca(name);
        }
    }
    return 0u;
}

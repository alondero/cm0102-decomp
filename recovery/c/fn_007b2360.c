/* Semantic reconstruction of preferred-image VA 0x007b2360
   (player_search.cpp), image base 0x00400000.  Native C reconstruction;
   callees and globals remain explicit boundaries to original code. */
#include "cmre.h"

#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)
#define FMT_00a701b0 ((u32)0x00a701b0)
#define ERR_009870e0 ((u32)0x009870e0)
#define FMT_009870e8 ((u32)0x009870e8)

extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
/* listing 0x007b28aa "mov ecx, esi" then 0x007b28ac "call 0x007b1520":
   explicit receiver = incoming ECX (this) passed as first thiscall parameter. */
extern u32 CM_THISCALL cm_fn_007b1520(u32 self, u32 arg1);
extern char *cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00932e10(u32 buffer, u32 size, u32 count, u32 stream);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

/* All error sites except the param_2==0 case pass (arg4=low, arg5=high) and
   then report with (arg1=low, arg2=high). */
#define ERR_GOTO(c)                                                      \
    do {                                                                 \
        cm_fn_00944cff(FMT_00a701b0, 0, 0, low, high);                   \
        earg1 = low;                                                     \
        earg2 = high;                                                    \
        code = (u32)(c);                                                 \
        goto REPORT;                                                     \
    } while (0)

u32 CM_THISCALL cm_fn_007b2360(u32 param_1, u32 param_2)
{
    /* One 0x200-byte scratch allocation: 'low' is L+0, 'high' is L+0x100. */
    u8 scratch[0x200];
    u8 *low = scratch;
    u8 *high = scratch + 0x100;
    u8 *earg1;
    u8 *earg2;
    u32 code;
    u32 r;
    int count;
    volatile int *piVar1;

    if (param_2 == 0) {
        cm_fn_00944cff(FMT_00a701b0, 0, 0, high, low);
        earg1 = high;
        earg2 = low;
        code = 0x905;
        goto REPORT;
    }

    r = cm_fn_00932e10(param_1, 4, 1, param_2);
    if (r != 1) ERR_GOTO(0x90c);

    r = cm_fn_00932e10(param_1 + 4, 1, 1, param_2);
    if (r != 1) ERR_GOTO(0x913);

    /* 0x007b243e "lea ecx, [esi + 5]"; 0x16b is the size pushed next. */
    r = cm_fn_00932e10(param_1 + 5, 0x16b, 1, param_2);
    if (r != 1) ERR_GOTO(0x91a);

    r = cm_fn_00932e10(param_1 + 0x174, 0x18, 1, param_2);
    if (r != 1) ERR_GOTO(0x921);

    r = cm_fn_00932e10(param_1 + 0x190, 4, 1, param_2);
    if (r != 1) ERR_GOTO(0x928);

    r = cm_fn_00932e10(param_1 + 0x18c, 4, 1, param_2);
    if (r != 1) ERR_GOTO(0x92f);

    r = cm_fn_00932e10(param_1 + 0x194, 1, 1, param_2);
    if (r != 1) ERR_GOTO(0x936);

    r = cm_fn_00932e10(param_1 + 0x195, 4, 1, param_2);
    if (r != 1) ERR_GOTO(0x93d);

    r = cm_fn_00932e10(param_1 + 0x199, 4, 1, param_2);
    if (r != 1) ERR_GOTO(0x944);

    r = cm_fn_00932e10(param_1 + 0x19d, 4, 1, param_2);
    if (r != 1) ERR_GOTO(0x94b);

    r = cm_fn_00932e10(param_1 + 0x1a1, 4, 1, param_2);
    if (r != 1) ERR_GOTO(0x953);

    r = cm_fn_00932e10(param_1 + 0x1ad, 8, 1, param_2);
    if (r != 1) ERR_GOTO(0x95a);

    r = cm_fn_00932e10(param_1 + 0x1a5, 8, 1, param_2);
    if (r != 1) ERR_GOTO(0x961);

    r = cm_fn_00932e10(param_1 + 0x1b5, 1, 1, param_2);
    if (r != 1) ERR_GOTO(0x968);

    r = cm_fn_00932e10(param_1 + 0x1b6, 8, 1, param_2);
    if (r != 1) ERR_GOTO(0x96f);

    r = cm_fn_00932e10(param_1 + 0x1be, 8, 1, param_2);
    if (r != 1) ERR_GOTO(0x976);

    r = cm_fn_00932e10(param_1 + 0x1c6, 4, 10, param_2);
    if (r != 10) ERR_GOTO(0x97d);

    cm_fn_007b1520(param_1, 0);

    r = cm_fn_00932e10(param_1 + 0x1f2, 4, 1, param_2);
    if (r != 1) ERR_GOTO(0x987);

    piVar1 = (volatile int *)(param_1 + 0x1f6);
    r = cm_fn_00932e10((u32)piVar1, 4, 1, param_2);
    if (r != 1) ERR_GOTO(0x98e);

    count = *piVar1;
    if (0 < count) {
        r = cm_fn_00932e10(*(u32 *)(param_1 + 0x1ee), 0x2d, (u32)count, param_2);
        if (r != (u32)*piVar1) ERR_GOTO(0x998);
    }

    r = cm_fn_00932e10(param_1 + 0x1fa, 0x99, 1, param_2);
    if (r != 1) ERR_GOTO(0x9a0);

    r = cm_fn_00932e10(param_1 + 0x293, 0x11, 1, param_2);
    if (r != 1) ERR_GOTO(0x9a7);

    r = cm_fn_00932e10(param_1 + 0x2a4, 4, 1, param_2);
    if (r != 1) ERR_GOTO(0x9ae);

    r = cm_fn_00932e10(param_1 + 0x2a8, 4, 1, param_2);
    if (r != 1) ERR_GOTO(0x9b5);

    r = cm_fn_00932e10(param_1 + 0x2ac, 4, 1, param_2);
    if (r == 1) {
        /* 0x7b2b4d: extra guard, then success (EAX=1). */
        if (*(int *)(param_1 + 0x190) > -1) {
            u32 p = *(u32 *)(param_1 + 0x170);
            if (p != 0) {
                r = cm_fn_00932e10(p, 0x51a8, 1, param_2);
                if (r != 1) {
                    cm_fn_00944cff(FMT_00a701b0, 0, 0, low, high);
                    cm_fn_00944c9f((u32)&DAT_00ae24d0, FMT_009870e8,
                                   (u32)cm_fn_0090d130(),
                                   (u32)low, (u32)high, 0x1ffb);
                    cm_fn_005e8290((const char *)ERR_009870e0,
                                   (const char *)&DAT_00ae24d0, 0);
                    DAT_00b67a34 = 0;
                }
            }
        }
        return 1;
    }

    ERR_GOTO(0x9bc);

REPORT:
    cm_fn_00944c9f((u32)&DAT_00ae24d0, FMT_009870e8,
                   (u32)cm_fn_0090d130(),
                   (u32)earg1, (u32)earg2, code);
    cm_fn_005e8290((const char *)ERR_009870e0, (const char *)&DAT_00ae24d0, 0);
    DAT_00b67a34 = 0;
    return 0;
}

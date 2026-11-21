/* Reconstruction of the original routine at preferred-image VA 0x00725f70
   (module match_screens.cpp, CM 01/02 3.9.68,
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86).

   Stack shape reproduced from the listing:
     sub esp,0x200 / push esi  -> local area L-0x200 .. L-0x001,
     buffer A at L-0x200 and buffer B at L-0x100 (adjacent, 0x100 apart),
     saved ESI at L-0x204.

   Callee ABIs taken from the original instructions:
     0x0090d130  mov eax,0xad4394 / ret      -> no arguments, constant result
     0x00944cff  reads [ebp+8],[ebp+0xc],[ebp+0x10],[ebp+0x14],[ebp+0x18]
                                              -> five cdecl arguments
     0x00944c9f  variadic, six arguments reach it here
     0x005e8290  reads [esp+0x80],[esp+0x84],[esp+0x88] after its own frame
                                              -> three cdecl arguments */

#include "cmre.h"

#define DAT_007260f0 CM_GLOBAL(u8, 0x007260f0)
#define DAT_00727c30 CM_GLOBAL(u8, 0x00727c30)
#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)
#define s_00a161d4 ((char *)0x00a161d4)
#define s_009870e0 ((char *)0x009870e0)
#define s_009870e8 ((char *)0x009870e8)

extern u32 CM_CDECL cm_fn_00765480(u32 input);
/* 0x00725f8c: ECX = 0xb74450, five stack arguments pushed right to left. */
extern u32 CM_THISCALL cm_fn_007fe600(u32 self, u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5);
/* 0x007260b9/0x007260c8/0x007260d7: ECX = 0xb74450, three stack arguments. */
extern void CM_THISCALL cm_fn_007ff270(int param_1, int param_2, char *param_3, int param_4);
/* 0x00726050: mov eax,0xad4394 / ret - takes no arguments. */
extern char * cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);

void cm_fn_00725f70(void)
{
    u8 scratch[0x200];
    u8 *const buf_a = scratch;          /* original L-0x200 */
    u8 *const buf_b = scratch + 0x100;  /* original L-0x100 */
    u32 ptr;
    u32 val_e6;
    u32 esi_val;
    void *text_a;
    void *text_b;
    u32 tag;

    /* 0x00725f76: receiver 0xb74450; args 0x7260f0, 0x727c30, 1, 0, 0. */
    if (cm_fn_007fe600(0xb74450u, (u32)(void *)&DAT_007260f0, (u32)(void *)&DAT_00727c30, 1u, 0u, 0u) == 0u) {
        return; /* 0x00725f93 -> 0x007260dd */
    }

    ptr = cm_fn_00765480(0u); /* 0x00725f9b */

    if (ptr == 0u) {
        /* 0x00725fa7: split the path, then the 0x1e1c text. */
        cm_fn_00944cff((u32)(void *)s_00a161d4, 0u, 0u, (void *)buf_b, (void *)buf_a);
        text_a = (void *)buf_b;
        text_b = (void *)buf_a;
        tag = 0x1e1cu;
    } else {
        val_e6 = *(volatile u32 *)(ptr + 0xe6); /* 0x00725fd9 */
        if (val_e6 == 0u) {
            /* 0x00725fe3: split the path, then the 0x1e24 text. */
            cm_fn_00944cff((u32)(void *)s_00a161d4, 0u, 0u, (void *)buf_a, (void *)buf_b);
            text_a = (void *)buf_a;
            text_b = (void *)buf_b;
            tag = 0x1e24u;
        } else {
            esi_val = *(u32 *)(ptr + 0xe2) + 4u; /* 0x00726015/0x0072601b */
            if (esi_val != 0u) {
                int flag;
                /* 0x0072608b: the two candidate slots, in listing order. */
                if (val_e6 == *(volatile u32 *)(esi_val + 0x1c) &&
                    *(volatile u8 *)(esi_val + 0x40) == 0u) {
                    flag = 1;
                } else if (val_e6 == *(volatile u32 *)(esi_val + 0x20) &&
                           *(volatile u8 *)(esi_val + 0x41) == 0u) {
                    flag = 1;
                } else {
                    flag = 0;
                }
                cm_fn_007ff270(0xb74450, 0, (char *)(u32)val_e6, 0);
                cm_fn_007ff270(0xb74450, 1, (char *)(u32)esi_val, 0);
                cm_fn_007ff270(0xb74450, 2, (char *)(u32)(u32)flag, 0);
                return; /* 0x007260dc -> 0x007260dd */
            }
            /* 0x00726020: split the path, then the 0x1e2c text. */
            cm_fn_00944cff((u32)(void *)s_00a161d4, 0u, 0u, (void *)buf_a, (void *)buf_b);
            text_a = (void *)buf_a;
            text_b = (void *)buf_b;
            tag = 0x1e2cu;
        }
    }

    /* 0x00726050: the pending tag and buffer pointers are the fourth,
       fifth and sixth arguments of the call at 0x00726060. */
    {
        char *res = cm_fn_0090d130();
        /* 0x00726055 */
        cm_fn_00944c9f((u32)(void *)&DAT_00ae24d0, (u32)(void *)s_009870e8, (u32)(void *)res,
                       text_a, text_b, tag);
    }
    /* 0x00726065 */
    cm_fn_005e8290((const char *)s_009870e0, (const char *)(void *)&DAT_00ae24d0, 0);
    /* 0x00726079 */
    DAT_00b67a34 = 0u;
}

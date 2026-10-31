#include "cmre.h"

/* Original frame: sub esp, 0x200, then push esi (saved) below it. The buffers
   use the post-sub base L: buffer A is L + 0 (scratch + 0, [esp+4] after the
   push) and buffer B is L + 0x100 (scratch + 0x100, [esp+0x104] after the push).

   Non-null path: the single stack argument (entry [esp+4]) is the flag edi
   tests. The loop walks 16 records of 110 bytes (0x6e) from the table at
   [0x00ae23bc], counting from the highest index down; the record count is
   re-read from [0x00ae2364] on every iteration. */

extern void cm_fn_005e8290(const char *message, const char *detail, int flags);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4,
                                  void *arg5);
extern u32 CM_THISCALL cm_fn_007ffe40(u32 self, u32 key);

u32 CM_CDECL cm_fn_00838d10(u32 param_1)
{
    u8 scratch[0x200];

    if (CM_GLOBAL(volatile u32, 0x00ae23bcu) == 0) {
        (void)cm_fn_00944cff(0x00a801dcu, 0, 0, scratch + 0x100, scratch);
        /* Pushes 0x39d8, edx and eax are not consumed by cm_fn_0090d130 (void).
           They are the varargs of the following cm_fn_00944c9f call. */
        u32 ret_0090d130 = cm_fn_0090d130();
        (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, ret_0090d130,
                             scratch + 0x100, scratch, 0x39d8u);
        cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0;
        return 0;
    }

    {
        u32 index = 0;   /* esi */
        u32 count = 0;   /* ebp */

        do {
            /* mov eax, [0xae2364]; sub eax, esi; dec eax */
            u32 n = CM_GLOBAL(volatile u32, 0x00ae2364u);
            u32 k = n - index - 1u;
            /* mov eax, [0xae23bc] */
            u32 base = CM_GLOBAL(volatile u32, 0x00ae23bcu);
            /* Record address base + k * 110; flag byte at record + 0x5f. */
            u32 record = base + k * 110u;
            u8 flag = *(volatile u8 *)(record + 0x5fu);

            if (flag == 1) {
                if (param_1 == 0) {
                    count++;
                } else {
                    u32 hit = cm_fn_007ffe40(0x00b74450u, record);

                    if (hit != 0) {
                        count++;
                    }
                }
            }
            index++;
        } while (index < 0x10u);

        return count;
    }
}

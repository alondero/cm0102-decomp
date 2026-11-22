/* Preferred-image VA 0x0085bea0, image base 0x00400000, SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Translated from the original instructions in the bundle listing. */
#include "cmre.h"

#define DAT_009a7f28 CM_GLOBAL(u8, 0x009a7f28)
#define DAT_00a835a4 CM_GLOBAL(u32, 0x00a835a4)
#define DAT_00ae23b0 CM_GLOBAL(int, 0x00ae23b0)
#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)

/* 0x009870e0 "Error", 0x009870e8 format, 0x00987da0 "CM3.TEMP",
   0x00a835b0 source path, 0x00a835e0 "Stadium.tmp" are string constants. */

extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern u32 cm_fn_0090bde0(u8 *arg1, u8 *arg2);
extern char *cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern u32 CM_CDECL cm_fn_00945a09(u32 stream);
extern volatile u32 *CM_CDECL cm_fn_00945b00(u32 path, volatile s8 *mode_text);
extern u32 CM_CDECL cm_fn_00945ce6(u32 destination, u32 size, u32 count, u32 stream);

void CM_FASTCALL cm_fn_0085bea0(u32 *param_1)
{
    /* One scratch allocation: L+0x00 saved stream, L+0x04 lo[256],
       L+0x104 hi[256].  lea [esp+0x14] is L+4, lea [esp+0x114] is L+0x104. */
    u8 scratch[0x204];
    u8 *lo = scratch + 0x04;
    u8 *hi = scratch + 0x104;
    u32 stream;
    u32 line;
    u8 *pu10;
    u8 *pu11;
    u32 read;
    int count;
    int i;
    u32 off;
    u32 v;
    u32 *entry;
    u32 t;

    /* push 0x9a7f28; push 0xa835e0; push 0x987da0; call 0x90bde0; add esp,8;
       push eax; call 0x945b00; add esp,8.  The first push (0x9a7f28) is the
       second argument of the following call, so GCC's right-to-left argument
       evaluation reproduces the original push order:
       cm_fn_00945b00(cm_fn_0090bde0(0x987da0, 0xa835e0), 0x9a7f28). */
    stream = (u32)cm_fn_00945b00(
        cm_fn_0090bde0((u8 *)0x00987da0, (u8 *)0x00a835e0),
        (volatile s8 *)&DAT_009a7f28);
    *(u32 *)scratch = stream;             /* mov [esp+0x10], ebp */
    i = 0;                                /* xor ebx, ebx */

    if (stream == 0) {
        /* 0x85bed8: arg4=0x114 buffer (hi), arg5=0x14 buffer (lo) */
        cm_fn_00944cff((u32)0x00a835b0, 0, 0, (void *)hi, (void *)lo);
        pu10 = hi;
        pu11 = lo;
        line = 0x179;
        goto LAB_0085c206;
    }

    if (cm_fn_00945ce6((u32)(param_1 + 2), 4, 1, stream) != 1) {
        /* 0x85bf21: arg4=lo, arg5=hi */
        cm_fn_00944cff((u32)0x00a835b0, 0, 0, (void *)lo, (void *)hi);
        pu10 = lo;
        pu11 = hi;
        line = 0x181;
        goto LAB_0085c206;
    }

    if (cm_fn_00945ce6((u32)(param_1 + 1), 4, 1, stream) != 1) {
        /* 0x85bf6a: arg4=lo, arg5=hi */
        cm_fn_00944cff((u32)0x00a835b0, 0, 0, (void *)lo, (void *)hi);
        pu10 = lo;
        pu11 = hi;
        line = 0x187;
        goto LAB_0085c206;
    }

    count = *(int *)(param_1 + 1);        /* mov eax, [edi] (edi = esi+4) */
    if (count != 0) {
        read = cm_fn_00945ce6(param_1[0], 0xc, (u32)count, stream);
        /* mov ecx, [edi] reload after the call */
        if (read != (u32)*(int *)(param_1 + 1)) {
            /* 0x85bfb8: arg4=lo, arg5=hi */
            cm_fn_00944cff((u32)0x00a835b0, 0, 0, (void *)lo, (void *)hi);
            pu10 = lo;
            pu11 = hi;
            line = 0x192;
            goto LAB_0085c206;
        }
    }

    if (cm_fn_00945ce6((u32)(param_1 + 5), 4, 1, stream) != 1) {
        /* 0x85c001: arg4=lo, arg5=hi */
        cm_fn_00944cff((u32)0x00a835b0, 0, 0, (void *)lo, (void *)hi);
        pu10 = lo;
        pu11 = hi;
        line = 0x19b;
        goto LAB_0085c206;
    }

    /* edi = esi+0x10 from 0x85c034 */
    if (cm_fn_00945ce6((u32)(param_1 + 4), 4, 1, stream) != 1) {
        /* 0x85c049: arg4=lo, arg5=hi */
        cm_fn_00944cff((u32)0x00a835b0, 0, 0, (void *)lo, (void *)hi);
        pu10 = lo;
        pu11 = hi;
        line = 0x1a1;
        goto LAB_0085c206;
    }

    if (cm_fn_00945ce6((u32)(param_1 + 6), 1, 1, stream) != 1) {
        /* 0x85c092: arg4=lo, arg5=hi */
        cm_fn_00944cff((u32)0x00a835b0, 0, 0, (void *)lo, (void *)hi);
        pu10 = lo;
        pu11 = hi;
        line = 0x1a7;
        goto LAB_0085c206;
    }

    /* 0x85c0c4: eax = [edi] = param_1[4]; compare with ebx (0) */
    count = *(int *)(param_1 + 4);
    if (count == 0) {
        i = 0;
        goto LAB_0085c1c1;
    }
    if (count > 0) {
        i = 0;
        off = 0;
        do {
            entry = (u32 *)(*(volatile u32 *)(param_1 + 3) + off + 0xc);
            v = *entry;
            if (v == 0) {
                *entry = 0xffffffffu;
            } else {
                *entry = *(u32 *)v;
            }
            i++;
            off += 0x10;
        } while (i < *(volatile int *)(param_1 + 4));
    }
    count = *(volatile int *)(param_1 + 4);
    read = cm_fn_00945ce6(*(volatile u32 *)(param_1 + 3), 0x10, (u32)count, stream);
    if (read != (u32)*(int *)(param_1 + 4)) {   /* 0x85c108 reload [edi] */
        /* inline error 0x1be, returns without the shared epilogue */
        cm_fn_00944cff((u32)0x00a835b0, 0, 0, (void *)lo, (void *)hi);
        /* push 0x1be; push hi; push lo; call 0x90d130 (no args);
           push eax; push fmt; push dest; call 0x944c9f.  The three earlier
           pushes are trailing varargs of cm_fn_00944c9f. */
        cm_fn_00944c9f((u32)&DAT_00ae24d0, (u32)0x009870e8,
                       (u32)cm_fn_0090d130(), (u32)lo, (u32)hi, 0x1beu);
        cm_fn_005e8290((const char *)0x009870e0, (const char *)&DAT_00ae24d0, 0);
        DAT_00b67a34 = 0;
        return;
    }

    /* 0x85c17f: xor ebx,ebx; ecx = count; test/jle */
    i = 0;
    count = *(int *)(param_1 + 4);
    if (count > 0) {
        off = 0;
        do {
            entry = (u32 *)(*(volatile u32 *)(param_1 + 3) + off + 0xc);
            v = *entry;
            if (v == 0xffffffffu) {
                *entry = 0;
            } else {
                t = v * 5u;       /* lea ebp,[eax+eax*4] */
                t <<= 3;          /* shl ebp,3 */
                t -= v;           /* sub ebp,eax  => v*39 */
                t = (u32)DAT_00ae23b0 + t * 2u; /* lea eax,[eax+ebp*2] */
                stream = *(u32 *)scratch;       /* mov ebp,[esp+0x10] */
                *entry = t;
            }
            i++;
            off += 0x10;
        } while (i < *(volatile int *)(param_1 + 4));
    }
    i = 0;                        /* 0x85c1bf xor ebx,ebx */

LAB_0085c1c1:
    if (cm_fn_00945ce6((u32)&DAT_00a835a4, 4, 1, stream) != 1) {
        cm_fn_00944cff((u32)0x00a835b0, 0, 0, (void *)lo, (void *)hi);
        pu10 = lo;
        pu11 = hi;
        line = 0x1d1;
        goto LAB_0085c206;
    }
    cm_fn_00945a09(stream);
    return;

LAB_0085c206:
    /* The three pushes made in each branch (line, pu11, pu10) precede the
       call to 0x90d130 and are consumed as trailing varargs by 0x944c9f. */
    cm_fn_00944c9f((u32)&DAT_00ae24d0, (u32)0x009870e8,
                   (u32)cm_fn_0090d130(), (u32)pu10, (u32)pu11, line);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)&DAT_00ae24d0, 0);
    DAT_00b67a34 = 0;             /* mov [0xb67a34], ebx (ebx == 0 here) */
    return;
}

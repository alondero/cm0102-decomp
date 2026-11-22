/* Preferred-image VA 0x00917830, image base 0x00400000.
   Pinned binary CM0102 3.9.68 SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Plain-C reconstruction checked against the original listing. */
#include "cmre.h"

#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)
#define ExceptionList CM_FS(void *, 0)

extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
/* 0x0091790d/0x00917930/0x00917953: ECX = [edx+ebp*4{,+4,+8}], stack arg = loop counter. */
extern u32 CM_THISCALL cm_fn_006826d0(void *self, u32 arg);
/* 0x009179d7 mov ecx,eax (cm_fn_00944cf1 result) is the receiver (param_1).
   Every stack push in the listing is a full 32-bit slot; declare all stack
   parameters as u32 so their exact 32-bit values are preserved. */
extern u32 *CM_THISCALL cm_fn_0068b100(u32 *param_1, u32 param_2, u32 param_3, u32 param_4,
    u32 param_5, u32 param_6, u32 param_7, u32 param_8, u32 param_9, u32 param_10, u32 param_11,
    u32 param_12, u32 param_13, u32 param_14, u32 param_15, u32 param_16, u32 param_17,
    u32 param_18, u32 param_19, u32 param_20, u32 param_21, u32 param_22, u32 param_23);
/* 0x009178ce / 0x00917a4a : the three pushes before this call belong to the
   following printf-style call (MSVC right-to-left evaluation, single add esp). */
extern char *cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg);

/* Local allocation from 0x00917845 sub esp,0x41c.  Base = FB-0x428. */
typedef struct {
    u8 b428, b427, b426, b425;      /* FB-0x428..FB-0x425 : 1,2,3,0 */
    u32 alloc;                       /* FB-0x424 : cm_fn_00944e46(0x18) result */
    u8 args_buf[4];                  /* FB-0x420 : byte args, set in listing order */
    u32 idx;                         /* FB-0x41c : low byte = [this+0x30] */
    u32 out2;                        /* FB-0x418 : vtable out-param 2 */
    u32 out3;                        /* FB-0x414 : vtable out-param 3 */
    u32 pv;                          /* FB-0x410 : cm_fn_00944cf1 result */
    u8 buf40c[256];                  /* FB-0x40c */
    u8 buf30c[256];                  /* FB-0x30c */
    u8 buf20c[256];                  /* FB-0x20c */
    u8 buf10c[256];                  /* FB-0x10c */
} locals_t;

/* SEH frame linked at 0x00917832..0x0091783e, unlinked at 0x00917a78..0x00917a83. */
typedef struct {
    volatile void *prev;
    volatile u32 handler;
    volatile u32 state;
} seh_t;

typedef u32 (CM_THISCALL *vtbl_fn)(void *self, u32, u32, u32, u32);

void CM_FASTCALL cm_fn_00917830(int *param_1)
{
    locals_t L;
    volatile seh_t frame;
    u8 *base = (u8 *)param_1;
    u8 byte_ba, byte_30;
    u32 i, vres, result, residue;
    s32 loop_idx, store_idx;
    u32 vtbl;

    /* SEH link: state=-1, handler=0x96591e, prev=old fs:[0], fs:[0]=&frame. */
    frame.state = 0xffffffffu;
    frame.handler = 0x0096591eu;
    frame.prev = ExceptionList;
    ExceptionList = (void *)&frame;

    /* 0x00917850..0x0091788a : byte setup.  Reads of the object happen here,
       before the first call (0x0091788e). */
    byte_ba = base[0xba];            /* mov cl,[esi+0xba] */
    byte_30 = base[0x30];            /* mov bl,[esi+0x30] */
    *(u8 *)&L.idx = byte_30;         /* 0x0091785e [esp+0x1c] = bl -> FB-0x41c */
    L.b428 = 1;                      /* 0x00917864 */
    L.b427 = 2;                      /* 0x00917869 */
    L.b426 = 3;                      /* 0x0091786e */
    L.b425 = 0;                      /* 0x00917873 */
    L.args_buf[1] = 0;               /* 0x00917878 */
    L.args_buf[2] = 0;               /* 0x0091787d */
    L.args_buf[3] = 0;               /* 0x00917882 */
    L.args_buf[0] = (u8)(3u - byte_ba); /* 0x0091788a al -> FB-0x420 */
    loop_idx = (s8)(u8)(byte_30 - 3u); /* 0x00917887 sub bl,3; 0x00917901 movsx ebp,bl */

    /* 0x0091788e cm_fn_00944e46(0x18) : 64-bit result, EAX compared/stored. */
    L.alloc = (u32)cm_fn_00944e46(0x18);

    if (L.alloc == 0) {
        /* 0x009178a0..0x009178fc */
        cm_fn_00944cff((u32)0x00ad4850, 0, 0, L.buf20c, L.buf40c);
        /* Single printf call: pushes 0x723,&buf40c,&buf20c, calls 0090d130,
           then pushes its result, format, destination (0x9178c7..0x9178de). */
        cm_fn_00944c9f((u32)0x00ae24d0, (u32)0x009870e8, (u32)cm_fn_0090d130(),
            (u32)L.buf20c, (u32)L.buf40c, 0x723);
        cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        DAT_00b67a34 = 0;
    } else {
        u32 *p = (u32 *)(u32)L.alloc;
        u32 *arr;

        /* Three rows of two thiscall calls; each reloads [esi+0xc]. */
        for (i = 0; i < 2; i++) {
            arr = (u32 *)*(volatile u32 *)(base + 0xc);
            p[i] = *(u32 *)(u32)cm_fn_006826d0((void *)arr[loop_idx], i);
        }
        for (i = 0; i < 2; i++) {
            arr = (u32 *)*(volatile u32 *)(base + 0xc);
            p[2 + i] = *(u32 *)(u32)cm_fn_006826d0((void *)arr[loop_idx + 1], i);
        }
        for (i = 0; i < 2; i++) {
            arr = (u32 *)*(volatile u32 *)(base + 0xc);
            p[4 + i] = *(u32 *)(u32)cm_fn_006826d0((void *)arr[loop_idx + 2], i);
        }

        /* 0x00917979 call [vtable+0x3c], ECX=this=param_1, 4 stack args. */
        vtbl = *(u32 *)base;                 /* mov edx,[esi] */
        vres = ((vtbl_fn)(*(u32 *)(vtbl + 0x3c)))(
            param_1, L.idx, (u32)&L.out2, (u32)&L.out3, 0);

        /* 0x0091797e..0x0091799a : cm_fn_00944cf1(0xee); state=0. */
        L.pv = (u32)cm_fn_00944cf1(0xee);
        frame.state = 0;

        if (L.pv == 0) {
            result = 0;                      /* 0x009179e0 xor eax,eax */
        } else {
            /* 0x0091799c..0x009179d9.  param_14 upper 16 bits come from ECX
               residue left by the earlier vtable call (0x009179a0 mov cx,[esi+0x40]
               only rewrites the low 16).  No listing VA shows the consumer reading
               those upper bits: explicit unknown boundary, use 0 here. */
            residue = 0;
            result = (u32)cm_fn_0068b100(
                (u32 *)L.pv,                                     /* this (ECX), 0x9179d7 */
                (u32)param_1,                                    /* param_2  (esi) */
                6,                                               /* param_3 */
                (u32)L.alloc,                                    /* param_4 */
                2,                                               /* param_5 */
                (u32)(*(u32 *)(base + 4)),                       /* param_6  [esi+4] */
                vres,                                            /* param_7  (ebx) */
                L.out2,                                          /* param_8 */
                3,                                               /* param_9 */
                1,                                               /* param_10 */
                10,                                              /* param_11 */
                (u32)&L.b428,                                    /* param_12 */
                (u32)L.args_buf,                                 /* param_13 */
                (u32)((residue << 16) | *(u16 *)(base + 0x40)),  /* param_14 */
                L.idx,                                           /* param_15 */
                L.out3,                                          /* param_16 */
                0x14,                                            /* param_17 */
                4,                                               /* param_18 */
                0,                                               /* param_19 */
                0x24,                                            /* param_20 */
                0xffffffffu,                                     /* param_21 */
                0,                                               /* param_22 */
                2                                                /* param_23 */
            );
        }

        /* 0x009179e2..0x009179fd : store result at [arr + (s8)[this+0x30]*4]. */
        frame.state = 0xffffffffu;
        store_idx = (s8)byte_30;             /* movsx edi, byte [esp+0x1c] */
        arr = *(u32 * volatile *)(base + 0xc);
        *(u32 *)((u8 *)arr + store_idx * 4) = result;

        cm_fn_009452ca((u32)L.alloc);        /* 0x00917a00 */
        cm_fn_009452ca(vres);                /* 0x00917a05 */

        /* 0x00917a0b..0x00917a14 : reload arr and test stored value. */
        arr = *(u32 * volatile *)(base + 0xc);
        if (*(u32 *)((u8 *)arr + store_idx * 4) == 0) {
            cm_fn_00944cff((u32)0x00ad4850, 0, 0, L.buf10c, L.buf30c);
            /* 0x917a28..0x917a5a : same deferred-cleanup pattern. */
            cm_fn_00944c9f((u32)0x00ae24d0, (u32)0x009870e8, (u32)cm_fn_0090d130(),
                (u32)L.buf10c, (u32)L.buf30c, 0x742);
            cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
            DAT_00b67a34 = 0;
        }
    }

    /* 0x00917a78..0x00917a83 : re-read saved prev, unlink. */
    ExceptionList = (void *)frame.prev;
    return;
}

#include "cmre.h"

/* Preferred-image VA 0x005c5310, image base 0x00400000.
   CM0102 3.9.68, SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */

#define SEH_HANDLER ((u32)0x0095a029)
#define GS_AE24D0 CM_GLOBAL(u8, 0x00ae24d0)
#define GS_B67A34 CM_GLOBAL(u32, 0x00b67a34)
#define GS_B7431C CM_GLOBAL(u32, 0x00b7431c)
#define S_9CD0A0 ((char *)0x009cd0a0)
#define S_9870E0 ((char *)0x009870e0)
#define S_9870E8 ((char *)0x009870e8)

extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_THISCALL cm_fn_0068d020(u32 self);
extern u32 CM_CDECL cm_fn_00944cff(u32 a1, u32 a2, u32 a3, void *a4, void *a5);
extern char * cm_fn_0090d130(const u8 *a1, const u8 *a2, u32 a3);
extern u32 CM_CDECL cm_fn_00944c9f(u32 dest, u32 fmt, ...);
extern void cm_fn_005e8290(const char *p1, const char *p2, int p3);
extern u32 CM_THISCALL cm_fn_0068d1f0(u8 *self, const u8 *target);
extern unsigned long long CM_FASTCALL cm_fn_0068d0d0(volatile u8 *self, u32 edx_in, const u8 *src, s16 count);
extern u32 CM_STDCALL cm_fn_007875e0(u32 club);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 a1, u32 a2);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self, u32 idx, u32 val);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *self, u32 a1, u32 a2);

struct cm_seh_frame {
    volatile u32 prev;
    volatile u32 handler;
    volatile u32 state;
};

/* Scratch offsets, relative to L = ESP immediately after `sub esp,0x2e8`:
   index 3  : E-0x2f1  loop counter byte ([esp+0xb])
   index 4  : E-0x2f0  allocated pointer store ([esp+0xc])
   index 8  : E-0x2ec  receiver object ([esp+0x10] / [esp+0x18] after 2 pushes)
   index 0xe8 : E-0x20c buffer A ([esp+0xf0])
   index 0x1e8: E-0x10c buffer B ([esp+0x1f0]) */

void CM_THISCALL cm_fn_005c5310(u32 param_1, volatile u32 param_2)
{
    struct cm_seh_frame frame;
    u8 scratch[0x2e8];
    u32 old_head;
    u32 p;
    u32 obj;
    u8 clu;
    s32 i;
    u32 fv;
    unsigned long long r;

    /* SEH prologue, in listing order (0x005c5310..0x005c531e). */
    old_head = (u32)CM_FS(void *, 0);      /* mov eax, fs:[0] */
    frame.state = (u32)-1;                 /* push -1 */
    frame.handler = SEH_HANDLER;           /* push 0x95a029 */
    frame.prev = old_head;                 /* push eax */
    CM_FS(void *, 0) = &frame;             /* mov fs:[0], esp */

    if (*(u32 *)((u8 *)param_1 + 0x34d0) == 0) {   /* 0x005c532f/5335 */
        r = cm_fn_00944cf1(0xe);                   /* 0x005c533f */
        *(volatile u32 *)(scratch + 4) = (u32)r;   /* 0x005c5347 [esp+0xc] */
        frame.state = 0;                           /* 0x005c534d */
        p = (u32)r;
        if (p == 0) {
            p = 0;                                 /* 0x005c5363 xor eax,eax */
        } else {
            p = cm_fn_0068d020(p);                 /* 0x005c535c ecx=eax */
        }
        frame.state = (u32)-1;                     /* 0x005c5367 */
        *(u32 *)((u8 *)param_1 + 0x34d0) = p;      /* 0x005c5372 */
        if (p != 0) goto path2;                    /* 0x005c5378 jne */

        /* error report path, 0x005c537a..0x005c53d9 */
        cm_fn_00944cff((u32)S_9CD0A0, 0, 0,
                       (void *)(scratch + 0x1e8), (void *)(scratch + 0xe8));
        {
            char *rr = cm_fn_0090d130((const u8 *)(scratch + 0x1e8),
                                      (const u8 *)(scratch + 0xe8), 0x944);
            /* 0x005c53b0/53c0: the three pushes made for 0x90d130 stay live
               (single add esp,0x24 at 0x53d6), so the variadic 0x944c9f
               reads them as further format arguments. */
            cm_fn_00944c9f((u32)&GS_AE24D0, (u32)S_9870E8, (u32)rr,
                           (u32)(scratch + 0x1e8), (u32)(scratch + 0xe8), 0x944);
        }
        cm_fn_005e8290(S_9870E0, (const char *)&GS_AE24D0, 0);
        GS_B67A34 = 0;                             /* 0x005c53d9 */
        goto done;
    }

path2:
    /* 0x005c53e8..0x005c5410 */
    obj = *(volatile u32 *)((u8 *)param_1 + 0x34d0);
    cm_fn_0068d1f0((u8 *)obj, (const u8 *)&param_2);                       /* 0x005c53f6 */
    obj = *(volatile u32 *)((u8 *)param_1 + 0x34d0);                      /* 0x005c53fb */
    cm_fn_0068d0d0((volatile u8 *)obj, (u32)&param_2, (const u8 *)&param_2, 4); /* 0x005c540b */

    p = param_2;                                   /* 0x005c5410 */
    clu = 1;                                       /* 0x005c5417/541c ebx=1, cl=1 */
    scratch[3] = clu;                              /* 0x005c5422 */
    if ((s16)*(u16 *)((u8 *)p + 0x13) <= 1) goto done; /* 0x005c541e/5426 jle */

    do {
        i = (s32)(s8)clu;                          /* 0x005c542c movsx esi,cl */
        fv = *(u32 *)((u8 *)p + i * 4 + 0x15);     /* 0x005c542f */
        if (fv != 0 && *(u8 *)((u8 *)p + i + 0x25) == 1) { /* 0x005c5433/543b */
            if (cm_fn_007875e0(fv) != 0) {         /* 0x005c544c/5451 */
                p = param_2;                       /* 0x005c5453 */
                *(u8 *)((u8 *)p + i + 0x25) = 2;   /* 0x005c5468 */
                cm_fn_0077bbe0((void *)(scratch + 8), 9, 0); /* 0x005c546d */
                p = param_2;                       /* 0x005c5472 */
                frame.state = 1;                   /* 0x005c5479 mov [esp+0x2f8],ebx */
                {
                    u32 pp = *(u32 *)((u8 *)p);    /* 0x005c5480 */
                    cm_fn_00785790((void *)(scratch + 8), 0, *(u32 *)((u8 *)pp)); /* 0x005c548b */
                }
                p = param_2;                       /* 0x005c5490 */
                {
                    u32 pp = *(u32 *)((u8 *)p + i * 4 + 0x15); /* 0x005c5497 */
                    cm_fn_00785790((void *)(scratch + 8), 1, *(u32 *)((u8 *)pp)); /* 0x005c54a3 */
                }
                p = param_2;                       /* 0x005c54a8 */
                {
                    s32 v = (s32)(s8)*(s8 *)((u8 *)p + 0x64); /* 0x005c54af */
                    cm_fn_00785790((void *)(scratch + 8), 2, (u32)v); /* 0x005c54ba */
                }
                p = param_2;                       /* 0x005c54bf */
                {
                    s32 v = (s32)(s8)*(s8 *)((u8 *)p + i + 0x25); /* 0x005c54c6 */
                    cm_fn_00785790((void *)(scratch + 8), 3, (u32)v); /* 0x005c54d2 */
                }
                p = param_2;                       /* 0x005c54d7 */
                {
                    s32 v = (s32)(s16)*(s16 *)((u8 *)p + 4); /* 0x005c54e2 */
                    cm_fn_00785790((void *)(scratch + 8), 5, (u32)v); /* 0x005c54e9 */
                }
                p = param_2;                       /* 0x005c54ee */
                {
                    u32 v = (u32)*(u16 *)((u8 *)p + 6); /* 0x005c54f5/54fd */
                    cm_fn_00785790((void *)(scratch + 8), 6, v); /* 0x005c5505 */
                }
                p = param_2;                       /* 0x005c550a */
                {
                    u32 v = *(u32 *)((u8 *)p + i * 4 + 0x15); /* 0x005c5515 */
                    u32 self = GS_B7431C;          /* 0x005c551b */
                    cm_fn_007861e0((void *)self, (u32)(scratch + 8), v); /* 0x005c5521 */
                }
                p = param_2;                       /* 0x005c5526 */
                frame.state = (u32)-1;             /* 0x005c552d */
            }
        }
        clu = (u8)(clu + 1);                       /* 0x005c553c inc cl */
        scratch[3] = clu;                          /* 0x005c5546 */
        p = param_2;                               /* eax at 0x005c5542 */
    } while ((s16)(s8)clu < (s16)*(u16 *)((u8 *)p + 0x13)); /* 0x005c5542 jl */

done:
    CM_FS(void *, 0) = (void *)frame.prev;         /* 0x005c5550/5559 */
    return;
}

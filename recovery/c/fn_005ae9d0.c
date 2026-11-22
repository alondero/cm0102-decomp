/* Translation of preferred-image VA 0x005ae9d0 (CM0102 3.9.68).
   thiscall: ECX (self) -> ebx, one 4-byte stack argument (p) -> ebp.
   Scratch sc[k] models L+k, where L is ESP after `sub esp,0x118`. */
#include "cmre.h"

#define ExceptionList CM_FS(void *, 0)
#define LAB_00959846 CM_GLOBAL(u8, 0x00959846)
#define GLOBAL_AE2C74 CM_GLOBAL(volatile u32, 0x00ae2c74)

extern void CM_THISCALL cm_fn_00417c10(u32 *param_1,u32 *param_2,u16 *param_3);
extern unsigned long long CM_THISCALL cm_fn_00549ef0(void *self, u32 first, u32 second);
extern void CM_FASTCALL cm_fn_0054a110(u16 *param_1);
extern u32 CM_THISCALL cm_fn_0054c5b0(short *param_1,u32 *param_2,int param_3);
extern u32 CM_THISCALL cm_fn_0054c8e0(short *param_1,u32 *param_2,int param_3);
extern u8 CM_THISCALL cm_fn_005aa680(int param_1,int param_2,int param_3);
extern unsigned long long CM_THISCALL cm_fn_005aac70(u32 self,volatile u32 first_arg,volatile u32 second_arg,volatile u32 date_arg);
extern u32 CM_THISCALL cm_fn_005abc30(u32 self, u32 record);
extern u32 CM_THISCALL cm_fn_005acbe0(u32 self, volatile u32 first_day, volatile u32 first_year, volatile u16 last_day, volatile u16 last_year, volatile u32 club_arg, volatile u32 output_arg, volatile u32 key_arg, volatile u8 filter_arg, volatile u8 side_arg);
extern u32 CM_THISCALL cm_fn_005ad780(u32 self, u32 fixture, volatile u32 output, volatile u32 extra);
extern unsigned long long CM_THISCALL cm_fn_005c6710(void *self, u32 arg);
extern u32 CM_THISCALL cm_fn_0068d020(u32 self);
extern u32 CM_THISCALL cm_fn_0068d040(u32 self);
extern u32 CM_THISCALL cm_fn_0068d080(u32 self);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32 self);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg4);

typedef struct { volatile u32 prev; volatile u32 handler; volatile u32 state; } seh_frame;

typedef u32 u32_mayalias __attribute__((may_alias));
typedef u16 u16_mayalias __attribute__((may_alias));
static u32 ld32(const u8 *p){ return *(const u32_mayalias *)(const void *)p; }
static void st32(u8 *p,u32 v){ *(u32_mayalias *)(void *)p = v; }
static u16 ld16(const u8 *p){ return *(const u16_mayalias *)(const void *)p; }
static void st16(u8 *p,u16 v){ *(u16_mayalias *)(void *)p = v; }
static void copy_bytes(u8 *d,const u8 *s,u32 n){ u32 i; for(i=0;i<n;i++) d[i]=s[i]; }
static int cmp_bytes(const u8 *a,const u8 *b,u32 n){ u32 i; for(i=0;i<n;i++){ if(a[i]!=b[i]) return (int)a[i]-(int)b[i]; } return 0; }

void CM_THISCALL cm_fn_005ae9d0(u32 self, u32 p)
{
    seh_frame frame;
    u8 sc[0x118];
#define SC(o) (sc + (o))
#define LD32(o) ld32(SC(o))
#define ST32(o,v) st32(SC(o),(u32)(v))
#define LD16(o) ld16(SC(o))
#define ST16(o,v) st16(SC(o),(u16)(v))
#define LD8(o) sc[(o)]
#define ST8(o,v) (sc[(o)] = (u8)(v))
#define LD32_FROM_P(base,o) ld32((const u8 *)((u32)(base)+(u32)(o)))
#define LD16_FROM_P(base,o) ld16((const u8 *)((u32)(base)+(u32)(o)))
#define LD8_FROM_P(base,o) (*(const u8 *)((u32)(base)+(u32)(o)))
#define ST32_FROM_P(base,o,v) st32((u8 *)((u32)(base)+(u32)(o)),(u32)(v))

    /* SEH prologue: push -1 / push handler / push prev ; link */
    frame.state = 0xFFFFFFFFu;
    frame.handler = (u32)&LAB_00959846;
    frame.prev = (u32)ExceptionList;
    ExceptionList = (void *)&frame;

    /* ecx = L+4 -> cm_fn_0054a110 */
    cm_fn_0054a110((u16 *)SC(0x4));
    frame.state = 0;
    cm_fn_0068d020((u32)SC(0x14));
    /* ebp == p (stack arg at L+0x128) */
    {
        void *recv = (void *)GLOBAL_AE2C74;
        frame.state = 1;
        u32 r = (u32)cm_fn_005c6710(recv, LD32_FROM_P(p, 0x14));
        if (r != 0) {
            frame.state = 0;
            cm_fn_0068d040((u32)SC(0x14));
            goto epilogue;
        }
    }

    ST8(0x3, 0); /* al = 0 */
    do {
        u32 rec;
        u16 extraout = 0;
        if (LD8(0x3) == 0)
            ST32(0x10, LD32_FROM_P(p, 0x1c));
        else
            ST32(0x10, LD32_FROM_P(p, 0x20));
        rec = LD32(0x10);
        {
            u32 f53 = LD32_FROM_P(rec, 0x53);
            if (f53 == 0 || LD8_FROM_P(f53, 0x11c) == 0)
                goto after_second;
            {
                u32 first_549, second_549, r54c8, r549lo;
                u32 cb;
                /* 005aea7d: ax=[p+0x28], cx=[p+0x2a]; push eax; push ecx */
                second_549 = ((u32)(u16)(f53 >> 16) << 16) | (u32)LD16_FROM_P(p, 0x28);
                first_549  = (u32)LD16_FROM_P(p, 0x2a);
                r549lo = (u32)cm_fn_00549ef0(SC(0x110), first_549, second_549);
                frame.state = 2;
                r54c8 = (u32)cm_fn_0054c8e0((short *)r549lo, (u32 *)SC(0xF0), 2);
                cm_fn_00417c10((u32 *)SC(0x4), (u32 *)SC(0x100), (u16 *)(u32)r54c8);
                frame.state = 1;
                cm_fn_0068d080((u32)SC(0x14));

                cb = cm_fn_005acbe0(self, LD32(0x4), LD32(0x6), LD32(0x4), LD32(0x6),
                                    LD32_FROM_P(rec, 0), (u32)SC(0x14),
                                    0xFFFFFFFFu, (u8)0xFEu, 1);
                extraout = (u16)(cb >> 16);
                if ((u16)cb != 0) {
                    u32 d1, v8c, r5c;
                    ST32(0x1E, LD32(0x16));
                    d1 = cm_fn_0068d1b0((u32)SC(0x14));
                    copy_bytes(SC(0x78), (void *)d1, 0x4F);
                    copy_bytes(SC(0x28), SC(0x78), 0x4F);
                    v8c = LD32(0x8C);
                    r5c = (u32)cm_fn_005c6710((void *)GLOBAL_AE2C74, v8c);
                    if (r5c == 0 && LD8(0x6B) == 0xFF) {
                        u32 r54c8b, d1b, flag, ebp5;
                        r54c8b = (u32)cm_fn_0054c8e0((short *)SC(0x4), (u32 *)SC(0xC8), 1);
                        cm_fn_00417c10((u32 *)SC(0x4), (u32 *)SC(0xD8), (u16 *)(u32)r54c8b);
                        ST16(0x52, LD16(0x4));
                        ST16(0x50, LD16(0x6));
                        ST32(0xC, 0);
                        ebp5 = cm_fn_005ad780(self, (u32)SC(0x28), (u32)SC(0x24), 0);
                        if (ebp5 != 0) {
                            ST32_FROM_P(ebp5, 0xa, LD32_FROM_P(ebp5, 2));
                            d1b = cm_fn_0068d1b0(ebp5);
                            if (d1b != 0) {
                                for (;;) {
                                    if (cmp_bytes(SC(0x78), (const u8 *)d1b, 0x4F) != 0) {
                                        ST32(0xC, 1);
                                        break;
                                    }
                                    d1b = cm_fn_0068d1b0(ebp5);
                                    if (d1b == 0) break;
                                }
                            }
                            cm_fn_0068d040(ebp5);
                            (void)cm_fn_00944c94((void *)ebp5);
                            flag = LD32(0xC);
                            if (flag != 0) {
                                /* bVar1 -> jne 0x5aec37 */
                                goto lab_549_2;
                            }
                        }
                        cm_fn_005abc30(self, (u32)SC(0x78));
                        cm_fn_005aa680(self, (int)SC(0x28), 0);
                        cm_fn_005aac70(self, (u32)SC(0x28), p, 0);
                    }
                }
            }
        }
        {
            /* LAB_005aec3e: second 549ef0 + second block */
            u32 first2, second2, r54c8c, r549lo2, cb2;
        lab_549_2:
            {
                u32 ebpv = p;
                second2 = ((u32)extraout << 16) | (u32)LD16_FROM_P(ebpv, 0x28);
                first2  = (u32)LD16_FROM_P(ebpv, 0x2a);
            }
            r549lo2 = (u32)cm_fn_00549ef0(SC(0xF8), first2, second2);
            frame.state = 3;
            r54c8c = (u32)cm_fn_0054c5b0((short *)r549lo2, (u32 *)SC(0xE8), 2);
            cm_fn_00417c10((u32 *)SC(0x4), (u32 *)SC(0x108), (u16 *)(u32)r54c8c);
            frame.state = 1;
            cm_fn_0068d080((u32)SC(0x14));

            cb2 = cm_fn_005acbe0(self, LD32(0x4), LD32(0x6), LD32(0x4), LD32(0x6),
                                 LD32_FROM_P(rec, 0), (u32)SC(0x14),
                                 0xFFFFFFFFu, (u8)0xFEu, 1);
            if ((u16)cb2 != 0) {
                u32 d1c, v8cc, r5c2;
                ST32(0x1E, LD32(0x16));
                d1c = cm_fn_0068d1b0((u32)SC(0x14));
                copy_bytes(SC(0x78), (void *)d1c, 0x4F);
                copy_bytes(SC(0x28), SC(0x78), 0x4F);
                v8cc = LD32(0x8C);
                r5c2 = (u32)cm_fn_005c6710((void *)GLOBAL_AE2C74, v8cc);
                if (r5c2 == 0) {
                    u32 r54c8d, d1d, flag2, ebp5b;
                    r54c8d = (u32)cm_fn_0054c5b0((short *)SC(0x4), (u32 *)SC(0xD0), 1);
                    cm_fn_00417c10((u32 *)SC(0x4), (u32 *)SC(0xE0), (u16 *)(u32)r54c8d);
                    ST16(0x52, LD16(0x4));
                    ST16(0x50, LD16(0x6));
                    ST32(0xC, 0);
                    ebp5b = cm_fn_005ad780(self, (u32)SC(0x28), (u32)SC(0x24), 0);
                    if (ebp5b != 0) {
                        ST32_FROM_P(ebp5b, 0xa, LD32_FROM_P(ebp5b, 2));
                        d1d = cm_fn_0068d1b0(ebp5b);
                        if (d1d != 0) {
                            for (;;) {
                                if (cmp_bytes(SC(0x78), (const u8 *)d1d, 0x4F) != 0) {
                                    ST32(0xC, 1);
                                    break;
                                }
                                d1d = cm_fn_0068d1b0(ebp5b);
                                if (d1d == 0) break;
                            }
                        }
                        cm_fn_0068d040(ebp5b);
                        (void)cm_fn_00944c94((void *)ebp5b);
                        flag2 = LD32(0xC);
                        if (flag2 != 0) goto after_second;
                    }
                    cm_fn_005abc30(self, (u32)SC(0x78));
                    cm_fn_005aa680(self, (int)SC(0x28), 0);
                    cm_fn_005aac70(self, (u32)SC(0x28), p, 0);
                }
            }
        }
    after_second:
        ST8(0x3, (u8)(LD8(0x3) + 1));
    } while (LD8(0x3) < 2);

    frame.state = (frame.state & 0xFFFFFF00u);
    cm_fn_0068d040((u32)SC(0x14));

epilogue:
    ExceptionList = (void *)frame.prev;
}

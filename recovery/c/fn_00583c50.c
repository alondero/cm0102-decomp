#include "cmre.h"

extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);
extern void CM_CDECL cm_fn_00521e60(u32, u32, u32, u32, u32, u32, u32);
extern unsigned long long CM_CDECL cm_fn_00521eb0(u32, u32, u32, u32,
    u32, u32, u32, u32, u32);
extern void CM_CDECL cm_fn_00521f10(u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0068a160(u32, u32, u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0068a1c0(u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00549f70(void *, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0054c8e0(void *, void *, u32);
extern u32 CM_THISCALL cm_fn_00417c10(void *, void *, u32);

#define B(p) (*(volatile u8 *)(u32)(p))
#define W(p) (*(volatile u16 *)(u32)(p))
#define D(p) (*(volatile u32 *)(u32)(p))
#define SX_YEAR ((u32)(s32)(s16)W(self + 0x40u))
#define START(i,d,m,delta,w) \
    cm_fn_00521e60(base,i,d,m,delta,w,SX_YEAR)
#define END(i,d,m,delta,w,last) \
    cm_fn_00521eb0(base,i,d,m,delta,w,2,SX_YEAR,last)
#define DATE(i,d,m,delta,w) \
    cm_fn_0068a160(base,i,d,m,delta,w,2,(u32)W(self + 0x40u),0)

/* Preferred-image VA 00583C50, pinned 3.9.68. THIS4/RET10, full EAX.
   No indexed direct callers: retain the pointer-or-zero result explicitly.
   L is the original base after SUB418, before four register saves. Date
   objects occupy L0/L8/L10; error buffers L18/L118/L218/L318. Keep them
   together and unwritten until the original helpers initialize them.
   68A160's year upper16 are unused: 549F70 stores CX and ANDs ECX,FFFF;
   the wrapper's year subtraction ultimately stores only DX. */
u32 CM_THISCALL cm_fn_00583c50(u32 self, u32 phase_arg,
    u32 count_out, u32 code_out, u32 optional_out)
{
    volatile struct { u32 prev, handler, state; } frame;
    volatile u8 scratch[0x418];
    s8 phase = (s8)phase_arg;
    u32 base, result, file, extension, line, temporary, year;
    u32 i;

    frame.prev = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x00958ac6u;
    CM_FS(u32, 0) = (u32)&frame;

    if (phase == -1) {
        if (optional_out != 0) D(optional_out) = 0;
        W(count_out) = 3;
        W(code_out) = 0;
        base = (u32)cm_fn_00944e46(0x138);
        if (base == 0) {
            file = (u32)(scratch + 0x118);
            extension = (u32)(scratch + 0x18);
            line = 0x12c;
            goto error;
        }
        START(0,0x1c,5,0,2);
        END(0,0xc,6,0,2,0);
        W(base+7)=0xfa;
        W(base+9)=0;
        W(base+0xb)=4;
        W(base+0xd)=0;
        B(base+0x17)=8;
        W(base+0x18)=0x12;
        W(base+0x1a)=9;
        W(base+0x1c)=0x12;
        W(base+0xf)=3;
        W(base+0x1e)=0;
        B(base+0x20)=0;
        B(base+0x21)=2;
        B(base+0x22)=7;
        D(base+0x5c)=0;
        D(base+0x60)=0;
        D(base+0x64)=0;
        START(1,0x1c,5,0,2);
        END(1,0x1a,6,0,2,0);
        W(base+0x6f)=0x104;
        W(base+0x71)=1;
        W(base+0x73)=4;
        W(base+0x75)=0x400;
        B(base+0x7f)=8;
        W(base+0x80)=0x1c;
        W(base+0x82)=0xe;
        W(base+0x84)=0x13;
        W(base+0x77)=3;
        W(base+0x86)=0x12;
        B(base+0x88)=0;
        B(base+0x89)=2;
        B(base+0x8a)=7;
        D(base+0xc4)=0;
        D(base+0xc8)=0;
        D(base+0xcc)=0;
        START(2,0x15,6,0,4);
        END(2,9,7,0,2,0);
        W(base+0xd7)=0x10e;
        W(base+0xd9)=2;
        W(base+0xdb)=4;
        W(base+0xdd)=0x400;
        B(base+0xe7)=8;
        W(base+0xe8)=0x20;
        W(base+0xea)=0x10;
        W(base+0xec)=0x12;
        W(base+0xdf)=3;
        W(base+0xee)=0x25;
        B(base+0xf0)=0;
        B(base+0xf1)=2;
        B(base+0xf2)=0xe;
        D(base+0x12c)=0;
        D(base+0x130)=0;
        D(base+0x134)=0;
        result = base;
        goto done;
    }

    if (phase < 8) {
        if (optional_out != 0) D(optional_out) = 1;
        W(count_out) = 6;
        W(code_out) = (u16)((s32)phase + 0x43d);
        base = (u32)cm_fn_00944e46(0x186);
        if (base == 0) {
            file = (u32)(scratch + 0x18);
            extension = (u32)(scratch + 0x118);
            line = 0x17a;
            goto error;
        }
        if (phase < 4) {
            DATE(0,0xc,8,0,1);
            DATE(1,0x14,8,0,2);
            DATE(2,0x1b,8,0,2);
            DATE(3,0x11,9,0,1);
            DATE(4,0x19,9,0,2);
            DATE(5,7,0xa,0,1);
        } else {
            DATE(0,0xd,8,0,2);
            DATE(1,0x13,8,0,1);
            DATE(2,0x1a,8,0,1);
            DATE(3,0x12,9,0,2);
            DATE(4,0x18,9,0,1);
            DATE(5,8,0xa,0,2);
        }
        for (i=0; (s16)i<6; ++i)
            cm_fn_0068a1c0(base,i,0,0xffffffffu,0xffffffffu,0xffffffffu,0);
        result = base;
        goto done;
    }

    if (phase < 12) {
        if (optional_out != 0) D(optional_out) = 1;
        W(count_out) = 6;
        W(code_out) = (u16)((s32)phase + 0x43d);
        base = (u32)cm_fn_00944e46(0x186);
        if (base == 0) {
            file = (u32)(scratch + 0x18);
            extension = (u32)(scratch + 0x118);
            line = 0x1aa;
            goto error;
        }
        year = (u32)(u16)(W(self+0x40u)+1u);
        cm_fn_00549f70((void *)scratch,1,2,year,2);
        frame.state = 0;
        temporary = cm_fn_0054c8e0((void *)scratch,(void *)(scratch+8),1);
        B((u32)&frame.state) = 1;
        cm_fn_00417c10((void *)scratch,(void *)(scratch+0x10),temporary);
        year = (u32)W(self+0x40u);
        B((u32)&frame.state) = 0;
        if (phase < 10) {
            cm_fn_0068a160(base,0,0x15,0xa,0,1,2,year,0);
            DATE(1,6,0xb,0,2);
            DATE(2,0xe,1,1,2);
            DATE(3,0x14,1,1,1);
            DATE(4,7,2,1,2);
            DATE(5,0xd,2,1,1);
        } else {
            cm_fn_0068a160(base,0,0x16,0xa,0,2,2,year,0);
            DATE(1,5,0xb,0,1);
            DATE(2,0xd,1,1,1);
            DATE(3,0x15,1,1,2);
            DATE(4,6,2,1,1);
            DATE(5,0xe,2,1,2);
        }
        for (i=0; (s16)i<6; ++i)
            cm_fn_0068a1c0(base,i,0,0xffffffffu,0xffffffffu,0xffffffffu,0);
        result = base;
        goto done;
    }

    if (phase != 12) {
        result = 0;
        goto done;
    }
    if (optional_out != 0) D(optional_out) = 0;
    W(count_out) = 3;
    W(code_out) = 0;
    base = (u32)cm_fn_00944e46(0x138);
    if (base == 0) {
        file = (u32)(scratch + 0x318);
        extension = (u32)(scratch + 0x218);
        line = 0x1db;
        goto error;
    }
    START(0,0xf,2,1,3);
    END(0,4,3,1,2,0);
    cm_fn_00521f10(base,0,0,0xffffffffu,0xffffffffu,0xffffffffu,0);
    W(base+7)=0x78;
    W(base+9)=0;
    W(base+0xb)=0;
    W(base+0xd)=0x200;
    B(base+0x17)=8;
    W(base+0x18)=8;
    W(base+0x1a)=4;
    W(base+0x1c)=8;
    W(base+0xf)=3;
    W(base+0x1e)=0;
    B(base+0x20)=0;
    B(base+0x21)=2;
    B(base+0x22)=0xe;
    D(base+0x5c)=0x186a00;
    D(base+0x60)=0;
    D(base+0x64)=0;
    START(1,0x14,3,1,4);
    END(1,2,4,1,2,0);
    cm_fn_00521f10(base,1,0,0xffffffffu,0xffffffffu,0xffffffffu,0);
    W(base+0x6f)=0x82;
    W(base+0x71)=1;
    W(base+0x73)=0;
    W(base+0x75)=0;
    B(base+0x7f)=8;
    W(base+0x80)=4;
    W(base+0x82)=2;
    W(base+0x84)=0;
    W(base+0x77)=3;
    W(base+0x86)=0;
    B(base+0x88)=0;
    B(base+0x89)=2;
    B(base+0x8a)=7;
    D(base+0xc4)=0x1e8480;
    D(base+0xc8)=0;
    D(base+0xcc)=0;
    START(2,3,4,1,3);
    year = (u32)(s32)(s16)W(self+0x40u);
    if ((u16)year == 0x7d0)
        cm_fn_00521eb0(base,2,0x17,4,1,2,2,0x7d0,0x80);
    else
        cm_fn_00521eb0(base,2,0x17,4,1,2,2,year,4);
    cm_fn_00521f10(base,2,0,0xffffffffu,0xffffffffu,0xffffffffu,0);
    W(base+0xd7)=0x96;
    W(base+0xd9)=2;
    W(base+0xdb)=0;
    W(base+0xdd)=0x83;
    B(base+0xe7)=8;
    W(base+0xe8)=2;
    W(base+0xea)=1;
    W(base+0xec)=0;
    W(base+0xdf)=0;
    W(base+0xee)=0;
    B(base+0xf0)=0;
    B(base+0xf1)=1;
    B(base+0xf2)=0;
    D(base+0x12c)=0;
    D(base+0x130)=0x3d0900;
    D(base+0x134)=0x30d400;
    result = base;
    goto done;

error:
    cm_fn_00944cff(0x009c4900u,0,0,(void *)file,(void *)extension);
    temporary = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u,0x009870e8u,temporary,file,extension,line);
    cm_fn_005e8290(0x009870e0u,0x00ae24d0u,0);
    D(0x00b67a34u) = 0;
    result = 0;
done:
    CM_FS(u32, 0) = frame.prev;
    return result;
}

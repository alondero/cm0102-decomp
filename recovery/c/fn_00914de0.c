#include "cmre.h"

extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);
extern void CM_CDECL cm_fn_00521e60(u32, u32, u32, u32, u32, u32, u32);
extern unsigned long long CM_CDECL cm_fn_00521eb0(u32, u32, u32, u32,
    u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0068a160(u32, u32, u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0068a1c0(u32, u32, u32, u32, u32, u32, u32);

#define B(p) (*(volatile u8 *)(u32)(p))
#define W(p) (*(volatile u16 *)(u32)(p))
#define D(p) (*(volatile u32 *)(u32)(p))
#define YEAR ((u32)(s32)(s16)W(self+0x40u))
#define START(i,d,m,delta,w) cm_fn_00521e60(base,i,d,m,delta,w,YEAR)
#define END(i,d,m,delta,w) cm_fn_00521eb0(base,i,d,m,delta,w,1,YEAR,0)
#define PAIR(i,d,m,delta,w,k) do { \
    cm_fn_0068a160(base,i,d,m,delta,w,k,(u32)W(self+0x40u),0); \
    cm_fn_0068a1c0(base,i,0,0xffffffffu,0xffffffffu,0xffffffffu,0); \
} while (0)

/* Preferred-image VA 00914DE0, pinned 3.9.68. THIS4/RET10, full EAX;
   no indexed direct callers. Original helpers remain explicit boundaries.
   Partial AX/CX/DX years are consumed only at their low word by 549F70
   and the date wrapper's final WORD store. The MOVSX-to-word arithmetic
   at 914EBE/ED3/EE6 and 914F7D/F91 similarly feeds only WORD stores.
   Keep each original byte/year reload and every metadata store in order. */
u32 CM_THISCALL cm_fn_00914de0(u32 self, u32 phase_arg,
    u32 count_out, u32 code_out, u32 optional_out)
{
    volatile u8 scratch[0x200];
    s8 phase = (s8)phase_arg;
    u8 mode;
    u32 base, count, line, file, extension, version, value;

    if (phase == -1) {
        if (optional_out != 0) D(optional_out)=0;
        W(count_out)=3;
        W(code_out)=0x40f;
        base=(u32)cm_fn_00944e46(0x138);
        if (base==0) {
            file=(u32)(scratch+0x100);
            extension=(u32)scratch;
            line=0xd4;
            goto error;
        }
        START(0,0xa,1,0,4);
        END(0,4,2,0,5);
        W(base+7)=0xa;
        W(base+9)=0;
        W(base+0xb)=0;
        if (W(self+0x40u)==0x7d0) W(base+0xd)=0x200;
        else W(base+0xd)=0;
        B(base+0x17)=0xa;
        value=12u-(u32)(s32)(s8)B(self+0xbbu);
        W(base+0x18)=(u16)(value*2u);
        value=12u-(u32)(s32)(s8)B(self+0xbbu);
        W(base+0x1a)=(u16)value;
        value=12u-(u32)(s32)(s8)B(self+0xbbu);
        W(base+0x1c)=(u16)(value*2u);
        W(base+0xf)=3;
        W(base+0x1e)=0;
        B(base+0x20)=0;
        B(base+0x21)=2;
        B(base+0x22)=0xe;
        D(base+0x5c)=0;
        D(base+0x60)=0;
        D(base+0x64)=0;
        START(1,0x13,2,0,6);
        END(1,2,3,0,5);
        W(base+0x6f)=0x14;
        W(base+0x71)=1;
        W(base+0x73)=0;
        if (W(self+0x40u)==0x7d0) W(base+0x75)=0x200;
        else W(base+0x75)=0;
        W(base+0x77)=3;
        B(base+0x7f)=0xa;
        W(base+0x80)=0xc;
        W(base+0x82)=6;
        W(base+0x84)=(u16)(s16)(s8)B(self+0xbbu);
        value=12u-(u32)(s32)(s8)B(self+0xbbu);
        B(base+0x88)=0;
        W(base+0x86)=(u16)(value*2u);
        B(base+0x89)=2;
        B(base+0x8a)=0xe;
        D(base+0xc4)=0;
        D(base+0xc8)=0;
        D(base+0xcc)=0;
        START(2,0x11,3,0,6);
        END(2,7,4,0,6);
        W(base+0xd7)=0x1e;
        W(base+0xd9)=2;
        W(base+0xdb)=0;
        if (W(self+0x40u)==0x7d0) W(base+0xdd)=0x200;
        else W(base+0xdd)=0;
        W(base+0xdf)=3;
        B(base+0xe7)=0xa;
        W(base+0xe8)=6;
        W(base+0xea)=3;
        W(base+0xec)=0;
        W(base+0xee)=0;
        B(base+0xf0)=0;
        B(base+0xf1)=2;
        B(base+0xf2)=0xe;
        D(base+0x12c)=0;
        D(base+0x130)=0;
        D(base+0x134)=0;
        return base;
    }

    if (phase<2) {
        mode=B(self+0xbcu);
        count=((mode==1 && phase==1) || mode==2) ? 2u : 6u;
        if (optional_out!=0) D(optional_out)=1;
        W(count_out)=(u16)count;
        W(code_out)=(u16)(0x473u+(phase!=0));
        base=(u32)cm_fn_00944e46(count*65u);
        if (base==0) {
            line=0x136;
            goto error_low;
        }
        PAIR(0,5,2,0,6,1);
        PAIR(1,0x13,2,0,6,1);
        if ((s16)count<=2) return base;
        PAIR(2,2,3,0,6,1);
        PAIR(3,0x10,3,0,6,1);
        PAIR(4,7,4,0,6,1);
        PAIR(5,0x13,4,0,6,1);
        return base;
    }

    if (phase==2) {
        if (optional_out!=0) D(optional_out)=0;
        W(count_out)=1;
        W(code_out)=0;
        base=(u32)cm_fn_00944e46(0x68);
        if (base==0) {
            line=0x162;
            goto error_low;
        }
        START(0,0x14,4,0,0);
        END(0,3,5,0,5);
        W(base+0x18)=6;
        W(base+0x1c)=6;
        W(base+7)=0x172;
        W(base+9)=0;
        W(base+0xb)=0;
        W(base+0xd)=0;
        B(base+0x17)=0xa;
        W(base+0x1a)=3;
        W(base+0xf)=3;
        W(base+0x1e)=0;
        B(base+0x20)=0;
        B(base+0x21)=2;
        B(base+0x22)=0xe;
        D(base+0x5c)=0;
        D(base+0x60)=0;
        D(base+0x64)=0;
        return base;
    }

    if (phase<6) {
        if (optional_out!=0) D(optional_out)=1;
        W(count_out)=6;
        W(code_out)=(u16)((s32)phase+0x409);
        base=(u32)cm_fn_00944e46(0x186);
        if (base==0) {
            line=0x18c;
            goto error_low;
        }
        PAIR(0,0xc,6,0,2,2);
        PAIR(1,0x1a,6,0,2,2);
        PAIR(2,0x10,7,0,2,2);
        PAIR(3,2,8,0,5,1);
        PAIR(4,0xb,9,0,2,2);
        PAIR(5,0xf,0xa,0,2,2);
        return base;
    }

    if (phase!=6) return 0;
    if (optional_out!=0) D(optional_out)=1;
    W(count_out)=0xa;
    W(code_out)=0x96;
    base=(u32)cm_fn_00944e46(0x28a);
    if (base==0) {
        line=0x1b5;
        goto error_low;
    }
    PAIR(0,2,2,1,6,1);
    PAIR(1,0x10,2,1,6,1);
    PAIR(2,0x17,2,1,6,1);
    PAIR(3,0x14,3,1,6,1);
    PAIR(4,0x1d,5,1,6,1);
    PAIR(5,9,8,1,2,1);
    PAIR(6,1,9,1,2,2);
    PAIR(7,2,0xa,1,6,1);
    PAIR(8,9,0xa,1,6,1);
    PAIR(9,0x10,0xa,1,6,1);
    return base;

error_low:
    file=(u32)scratch;
    extension=(u32)(scratch+0x100);
error:
    cm_fn_00944cff(0x00ad4850u,0,0,(void *)file,(void *)extension);
    version=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u,0x009870e8u,version,file,extension,line);
    cm_fn_005e8290(0x009870e0u,0x00ae24d0u,0);
    D(0x00b67a34u)=0;
    return 0;
}

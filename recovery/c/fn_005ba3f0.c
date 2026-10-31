#include "cmre.h"

/* Preferred-image VA005BA3F0/base00400000; CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS4/RET16/full EAX. Only BYTE phase FF constructs nine 104-byte rows.
   Every date tuple/year reload and ordered store follows the pinned listing.
   L=afterSUB200; the error buffers are adjacent L0/L100, uninitialized. */
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00521e60(u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00521eb0(u32,u32,u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,void *,void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

static u32 year(u32 self)
{
    return (u32)(s32)(s16)W(self + 0x40u);
}

static void tail(u32 row, u32 last)
{
    D(row + 0x5cu) = 0;
    D(row + 0x60u) = 0;
    D(row + 0x64u) = last;
}

u32 CM_THISCALL cm_fn_005ba3f0(u32 self, volatile u32 first,
                              volatile u32 second, volatile u32 third,
                              volatile u32 fourth)
{
    volatile u8 scratch[0x200];
    u32 l = (u32)scratch, pointer, count, code, output;
    if ((u8)first != 0xffu) return 0;
    pointer = fourth;
    if (pointer != 0) D(pointer) = 0;
    count = second;
    code = third;
    W(count) = 9;
    W(code) = 0;
    output = cm_fn_00944e46(0x3a8);
    if (output == 0) {
        cm_fn_00944cff(0x009ccb28u,0,0,(void *)(l + 0x100u),(void *)l);
        pointer = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u,0x009870e8u,pointer,l + 0x100u,l,0xa6);
        cm_fn_005e8290(0x009870e0u,0x00ae24d0u,0);
        D(0x00b67a34u) = 0;
        return 0;
    }
    cm_fn_00521e60(output,0,0xa,9,0,0xffffffffu,year(self));
    cm_fn_00521eb0(output,0,6,0xa,0,5,2,year(self),0);
    W(output + 7u) = 0x3c;
    W(output + 9u) = 0;
    W(output + 0xbu) = 0;
    W(output + 0xdu) = 3;
    B(output + 0x17u) = 4;
    W(output + 0x18u) = 0x28;
    W(output + 0x1au) = 0x14;
    W(output + 0x1cu) = 0x28;
    W(output + 0xfu) = 0;
    W(output + 0x1eu) = 0;
    B(output + 0x20u) = 0;
    B(output + 0x21u) = 1;
    B(output + 0x22u) = 0;
    tail(output,0);

    cm_fn_00521e60(output,1,0xc,0xa,0,4,year(self));
    cm_fn_00521eb0(output,1,0x1b,0xa,0,5,2,year(self),0);
    W(output + 0x6fu) = 0x46;
    W(output + 0x71u) = 1;
    W(output + 0x73u) = 1;
    W(output + 0x75u) = 3;
    B(output + 0x7fu) = 4;
    W(output + 0x80u) = 0xb8;
    W(output + 0x82u) = 0x5c;
    W(output + 0x84u) = 0xa4;
    W(output + 0x77u) = 0;
    W(output + 0x86u) = 0x28;
    B(output + 0x88u) = 0;
    B(output + 0x89u) = 1;
    B(output + 0x8au) = 0;
    tail(output + 0x68u,0);

    cm_fn_00521e60(output,2,3,0xb,0,4,year(self));
    cm_fn_00521eb0(output,2,0x12,0xb,0,5,2,year(self),0);
    W(output + 0xd7u) = 0x50;
    W(output + 0xd9u) = 2;
    W(output + 0xdbu) = 1;
    W(output + 0xddu) = 3;
    W(output + 0xdfu) = 0;
    B(output + 0xe7u) = 4;
    W(output + 0xe8u) = 0x5c;
    W(output + 0xeau) = 0x2e;
    W(output + 0xecu) = 0;
    W(output + 0xeeu) = 0xcc;
    B(output + 0xf0u) = 0;
    B(output + 0xf1u) = 1;
    B(output + 0xf2u) = 0;
    tail(output + 0xd0u,0);

    cm_fn_00521e60(output,3,6,0,1,3,year(self));
    cm_fn_00521eb0(output,3,0x16,0,1,5,2,year(self),0);
    W(output + 0x13fu) = 0x5a;
    W(output + 0x141u) = 3;
    W(output + 0x143u) = 1;
    W(output + 0x145u) = 3;
    W(output + 0x147u) = 0;
    B(output + 0x14fu) = 4;
    W(output + 0x150u) = 0x40;
    W(output + 0x152u) = 0x20;
    W(output + 0x154u) = 0x12;
    W(output + 0x156u) = 0xcc;
    B(output + 0x158u) = 0;
    B(output + 0x159u) = 1;
    B(output + 0x15au) = 0;
    tail(output + 0x138u,0);

    cm_fn_00521e60(output,4,0x1b,0,1,3,year(self));
    cm_fn_00521eb0(output,4,0xc,1,1,5,2,year(self),0);
    W(output + 0x1a7u) = 0x64;
    W(output + 0x1a9u) = 4;
    W(output + 0x1abu) = 1;
    W(output + 0x1adu) = 3;
    B(output + 0x1b7u) = 4;
    W(output + 0x1b8u) = 0x20;
    W(output + 0x1bau) = 0x10;
    W(output + 0x1bcu) = 0;
    W(output + 0x1afu) = 0;
    W(output + 0x1beu) = 0;
    B(output + 0x1c0u) = 0;
    B(output + 0x1c1u) = 1;
    B(output + 0x1c2u) = 0;
    tail(output + 0x1a0u,0);

    cm_fn_00521e60(output,5,0x14,1,1,6,year(self));
    cm_fn_00521eb0(output,5,4,2,1,5,2,year(self),0);
    W(output + 0x20fu) = 0x6e;
    W(output + 0x211u) = 5;
    W(output + 0x213u) = 1;
    W(output + 0x215u) = 3;
    B(output + 0x21fu) = 4;
    W(output + 0x220u) = 0x10;
    W(output + 0x222u) = 8;
    W(output + 0x224u) = 0;
    W(output + 0x217u) = 0;
    W(output + 0x226u) = 0;
    B(output + 0x228u) = 0;
    B(output + 0x229u) = 1;
    B(output + 0x22au) = 0;
    tail(output + 0x208u,0x186a0);

    cm_fn_00521e60(output,6,5,2,1,6,year(self));
    cm_fn_00521eb0(output,6,0x13,2,1,6,2,year(self),0);
    W(output + 0x277u) = 0x78;
    W(output + 0x279u) = 6;
    W(output + 0x27bu) = 1;
    W(output + 0x27du) = 3;
    B(output + 0x287u) = 6;
    W(output + 0x288u) = 8;
    W(output + 0x28au) = 4;
    W(output + 0x28cu) = 0;
    W(output + 0x27fu) = 0;
    W(output + 0x28eu) = 0;
    B(output + 0x290u) = 0;
    B(output + 0x291u) = 1;
    B(output + 0x292u) = 0;
    tail(output + 0x270u,0x30d40);

    cm_fn_00521e60(output,7,0x14,2,1,0,year(self));
    cm_fn_00521eb0(output,7,0xc,3,1,2,2,year(self),0);
    W(output + 0x2dfu) = 0x82;
    W(output + 0x2e1u) = 7;
    W(output + 0x2e3u) = 1;
    W(output + 0x2e5u) = 3;
    B(output + 0x2efu) = 6;
    W(output + 0x2f0u) = 4;
    W(output + 0x2f2u) = 2;
    W(output + 0x2f4u) = 0;
    W(output + 0x2e7u) = 0;
    W(output + 0x2f6u) = 0;
    B(output + 0x2f8u) = 0;
    B(output + 0x2f9u) = 1;
    B(output + 0x2fau) = 0;
    tail(output + 0x2d8u,0x61a80);

    cm_fn_00521e60(output,8,0xd,3,1,3,year(self));
    cm_fn_00521eb0(output,8,6,4,1,5,2,year(self),4);
    W(output + 0x347u) = 0x96;
    W(output + 0x349u) = 8;
    W(output + 0x34bu) = 0;
    W(output + 0x34du) = 3;
    B(output + 0x357u) = 6;
    W(output + 0x358u) = 2;
    W(output + 0x35au) = 1;
    W(output + 0x35cu) = 0;
    W(output + 0x34fu) = 0;
    W(output + 0x35eu) = 0;
    B(output + 0x360u) = 0;
    B(output + 0x361u) = 1;
    B(output + 0x362u) = 0;
    D(output + 0x39cu) = 0;
    D(output + 0x3a0u) = 0xf4240;
    D(output + 0x3a4u) = 0xc3500;
    return output;
}

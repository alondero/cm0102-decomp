#include "cmre.h"

/* Preferred-image VA0088CF40/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS4/RET16/full EAX, including negative signed-BYTE phases. Each native
   helper tuple, year reload and store order is retained. Callees are original.
   L=afterSUB204; diagnostic buffers L4/L104 remain adjacent/uninitialized. */
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

static void tail(u32 row)
{
    D(row + 0x5cu) = 0;
    D(row + 0x60u) = 0xffffffffu;
    D(row + 0x64u) = 0;
}

u32 CM_THISCALL cm_fn_0088cf40(u32 self, volatile u32 first,
                              volatile u32 second, volatile u32 third,
                              volatile u32 fourth)
{
    volatile u8 scratch[0x204];
    u32 l = (u32)scratch, output, count, code, pointer, a, b, line;
    s8 phase = (s8)(u8)first;
    s32 parity;
    if (phase < 7) {
        pointer = fourth;
        if (pointer != 0) D(pointer) = 0;
        count = second;
        code = third;
        W(count) = 2;
        W(code) = (u16)((s32)phase + 0x464);
        output = cm_fn_00944e46(0xd0);
        if (output == 0) {
            a = l + 0x104u;
            b = l + 4u;
            line = 0xcb;
            goto error;
        }
        cm_fn_00521e60(output,0,0xf,6,0,5,year(self));
        cm_fn_00521eb0(output,0,0x1b,6,0,6,1,year(self),0);
        parity = (s32)(s8)(u8)first % 2;
        W(output + 7u) = 0xa;
        W(output + 9u) = 0;
        W(output + 0xbu) = 1;
        W(output + 0xdu) = 0x83;
        B(output + 0x17u) = 4;
        D(l) = (u32)parity;
        if (parity == 0) {
            a = 0x1e;
            W(output + 0x1au) = 0xf;
            W(output + 0x18u) = (u16)a;
        } else {
            a = 0x1c;
            W(output + 0x1au) = 0xe;
            W(output + 0x18u) = (u16)a;
        }
        W(output + 0x1cu) = (u16)a;
        W(output + 0xfu) = 0;
        W(output + 0x1eu) = 0;
        B(output + 0x20u) = 0;
        B(output + 0x21u) = 1;
        B(output + 0x22u) = 0;
        tail(output);
        cm_fn_00521e60(output,1,0x1c,6,0,0,year(self));
        cm_fn_00521eb0(output,1,0x1b,7,0,2,2,year(self),0);
        a = D(l);
        W(output + 0x6fu) = 0x14;
        W(output + 0x71u) = 1;
        W(output + 0x73u) = 1;
        W(output + 0x75u) = 0x83;
        W(output + 0x77u) = 0;
        B(output + 0x7fu) = 4;
        W(output + 0x80u) = 0x10;
        W(output + 0x82u) = 8;
        if (a == 0) {
            W(output + 0x84u) = 1;
            W(output + 0x86u) = 0x1e;
        } else {
            W(output + 0x84u) = 2;
            W(output + 0x86u) = 0x1c;
        }
        B(output + 0x88u) = 0;
        B(output + 0x89u) = 1;
        B(output + 0x8au) = 0;
        tail(output + 0x68u);
        return output;
    }
    if (phase != 7) return 0;
    pointer = fourth;
    if (pointer != 0) D(pointer) = 0;
    count = second;
    code = third;
    W(count) = 6;
    W(code) = 0;
    output = cm_fn_00944e46(0x270);
    if (output == 0) {
        a = l + 4u;
        b = l + 0x104u;
        line = 0x119;
        goto error;
    }

    cm_fn_00521e60(output,0,0x1d,7,0,3,year(self));
    cm_fn_00521eb0(output,0,0x19,8,0,2,2,year(self),0);
    W(output + 7u) = 0x1e;
    W(output + 9u) = 0;
    W(output + 0xbu) = 1;
    W(output + 0xdu) = 0x83;
    W(output + 0xfu) = 0;
    B(output + 0x17u) = 4;
    W(output + 0x18u) = 0x40;
    W(output + 0x1au) = 0x20;
    W(output + 0x1cu) = 0x40;
    W(output + 0x1eu) = 0;
    B(output + 0x20u) = 0;
    B(output + 0x21u) = 1;
    B(output + 0x22u) = 0;
    tail(output);

    cm_fn_00521e60(output,1,0x1a,8,0,3,year(self));
    cm_fn_00521eb0(output,1,0x1d,2,1,6,1,year(self),0);
    W(output + 0x6fu) = 0x28;
    W(output + 0x71u) = 1;
    W(output + 0x73u) = 1;
    W(output + 0x75u) = 0x83;
    W(output + 0x77u) = 0;
    B(output + 0x7fu) = 4;
    W(output + 0x80u) = 0x20;
    W(output + 0x82u) = 0x10;
    W(output + 0x84u) = 0;
    W(output + 0x86u) = 0;
    B(output + 0x88u) = 0;
    B(output + 0x89u) = 0;
    B(output + 0x8au) = 0;
    tail(output + 0x68u);

    cm_fn_00521e60(output,2,0x1e,2,1,0,year(self));
    cm_fn_00521eb0(output,2,9,3,1,2,2,year(self),0);
    W(output + 0xd7u) = 0x32;
    W(output + 0xd9u) = 2;
    W(output + 0xdbu) = 1;
    W(output + 0xddu) = 0x83;
    B(output + 0xe7u) = 4;
    W(output + 0xe8u) = 0x10;
    W(output + 0xeau) = 8;
    W(output + 0xecu) = 0;
    W(output + 0xdfu) = 3;
    W(output + 0xeeu) = 0;
    B(output + 0xf0u) = 0;
    B(output + 0xf1u) = 1;
    B(output + 0xf2u) = 0;
    tail(output + 0xd0u);

    cm_fn_00521e60(output,3,0xa,3,1,3,year(self));
    cm_fn_00521eb0(output,3,0x1e,3,1,2,2,year(self),0);
    W(output + 0x13fu) = 0x78;
    W(output + 0x141u) = 3;
    W(output + 0x143u) = 0;
    W(output + 0x145u) = 0x83;
    B(output + 0x14fu) = 6;
    W(output + 0x150u) = 8;
    W(output + 0x152u) = 4;
    W(output + 0x154u) = 0;
    W(output + 0x147u) = 0;
    W(output + 0x156u) = 0;
    B(output + 0x158u) = 0;
    B(output + 0x159u) = 1;
    B(output + 0x15au) = 0;
    tail(output + 0x138u);

    cm_fn_00521e60(output,4,1,4,1,3,year(self));
    cm_fn_00521eb0(output,4,7,4,1,2,1,year(self),0);
    W(output + 0x1a7u) = 0x82;
    W(output + 0x1a9u) = 4;
    W(output + 0x1abu) = 0;
    W(output + 0x1adu) = 0x83;
    B(output + 0x1b7u) = 6;
    W(output + 0x1b8u) = 4;
    W(output + 0x1bau) = 2;
    W(output + 0x1bcu) = 0;
    W(output + 0x1afu) = 0;
    W(output + 0x1beu) = 0;
    B(output + 0x1c0u) = 0;
    B(output + 0x1c1u) = 0;
    B(output + 0x1c2u) = 0;
    tail(output + 0x1a0u);

    cm_fn_00521e60(output,5,8,4,1,3,year(self));
    cm_fn_00521eb0(output,5,0x15,4,1,2,1,year(self),4);
    W(output + 0x20fu) = 0x96;
    W(output + 0x211u) = 5;
    W(output + 0x213u) = 0;
    W(output + 0x215u) = 0;
    B(output + 0x21fu) = 6;
    W(output + 0x220u) = 2;
    W(output + 0x222u) = 1;
    W(output + 0x224u) = 0;
    W(output + 0x217u) = 0x83;
    W(output + 0x226u) = 0;
    B(output + 0x228u) = 0;
    B(output + 0x229u) = 2;
    B(output + 0x22au) = 7;
    tail(output + 0x208u);
    return output;

error:
    cm_fn_00944cff(0x00a8b4a0u,0,0,(void *)a,(void *)b);
    pointer = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u,0x009870e8u,pointer,a,b,line);
    cm_fn_005e8290(0x009870e0u,0x00ae24d0u,0);
    D(0x00b67a34u) = 0;
    return 0;
}

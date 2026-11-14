/* Preferred-image VA 0x0056c570, image base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   thiscall: ECX = competition object; stack: BYTE flag, WORD out, WORD out,
   DWORD out (ret 0x10 pops four bytes of arguments).
   0056c570 reads only the low byte of the flag argument and compares it with
   0xff; any other value returns NULL (EAX = 0) without touching the outputs.
   The three pushes before 0056c5ed are not arguments of 0090d130 (which takes
   none): they are the trailing variadic words of the later 00944c9f call.
   The object fields read are WORD self+0x40 (sign extended) and BYTE
   self+0xb2 (sign extended); both are re-read at every use point. */
#include "cmre.h"

extern u32 CM_CDECL cm_fn_00521e60(u32 arg1, u32 arg2, u32 arg3, u32 arg4,
                                   u32 arg5, u32 arg6, u32 arg7);
extern unsigned long long CM_CDECL cm_fn_00521eb0(u32 arg1, u32 arg2, u32 arg3,
                                                  u32 arg4, u32 arg5, u32 arg6,
                                                  u32 arg7, u32 arg8, u32 arg9);
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern char *cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4,
                                   void *arg5);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);

#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))
#define INDEX40() ((u32)(s32)(s16)W(self + 0x40u))
#define BYTEB2() ((s32)(s8)B(self + 0xb2u))

void *CM_THISCALL cm_fn_0056c570(u32 self, u8 flag, u32 out1, u32 out2, u32 out3)
{
    volatile u8 scratch[0x200];
    u32 local = (u32)scratch;
    u32 allocated;
    u32 rec;
    u32 text;
    s32 b2;

    if (flag != 0xffu)
        return (void *)0;

    if (out3 != 0u)
        D(out3) = 0u;
    W(out1) = 7u;
    W(out2) = 0u;

    allocated = (u32)cm_fn_00944e46(0x2d8u);
    if (allocated == 0u) {
        cm_fn_00944cff(0x009c4224u, 0u, 0u, (void *)(local + 0x100u),
                       (void *)local);
        text = (u32)(unsigned long)cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text, local + 0x100u,
                       local, 0xa2u);
        cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
        D(0x00b67a34u) = 0u;
        return (void *)0;
    }

    rec = allocated;

    cm_fn_00521e60(rec, 0u, 0x17u, 6u, 0u, 5u, INDEX40());
    cm_fn_00521eb0(rec, 0u, 0x16u, 7u, 0u, 1u, 2u, INDEX40(), 0u);

    W(rec + 7u) = 10u;
    W(rec + 9u) = 0u;
    W(rec + 0xbu) = 0u;
    W(rec + 0xdu) = 0x403u;
    B(rec + 0x17u) = 4u;
    b2 = BYTEB2();
    W(rec + 0x18u) = (u16)(2 * b2 + 0x38);
    b2 = BYTEB2();
    W(rec + 0x1au) = (u16)(b2 + 0x1cu);
    b2 = BYTEB2();
    W(rec + 0x1cu) = (u16)(2 * b2 + 0x38);
    W(rec + 0xfu) = 0u;
    W(rec + 0x1eu) = 0u;
    B(rec + 0x20u) = 0u;
    B(rec + 0x21u) = 1u;
    B(rec + 0x22u) = 0u;
    D(rec + 0x5cu) = 0u;
    D(rec + 0x60u) = 0xffffffffu;
    D(rec + 0x64u) = 0u;

    cm_fn_00521e60(rec, 1u, 0x17u, 7u, 0u, 2u, INDEX40());
    cm_fn_00521eb0(rec, 1u, 0x14u, 8u, 0u, 2u, 2u, INDEX40(), 0u);

    W(rec + 0x6fu) = 0x14u;
    W(rec + 0x71u) = 1u;
    W(rec + 0x73u) = 0u;
    W(rec + 0x75u) = 0x403u;
    W(rec + 0x77u) = 0u;
    B(rec + 0x7fu) = 4u;
    b2 = BYTEB2();
    W(rec + 0x80u) = (u16)((0x20 - b2) * 2);
    b2 = BYTEB2();
    W(rec + 0x82u) = (u16)(0x20 - b2);
    b2 = BYTEB2();
    W(rec + 0x84u) = (u16)(3 * (0x0c - b2));
    b2 = BYTEB2();
    W(rec + 0x86u) = (u16)(2 * b2 + 0x38);
    B(rec + 0x88u) = 0u;
    B(rec + 0x89u) = 1u;
    B(rec + 0x8au) = 0u;
    D(rec + 0xc4u) = 0u;
    D(rec + 0xc8u) = 0xffffffffu;
    D(rec + 0xccu) = 0u;

    cm_fn_00521e60(rec, 2u, 0x1cu, 8u, 0u, 3u, INDEX40());
    cm_fn_00521eb0(rec, 2u, 0x1cu, 9u, 0u, 2u, 2u, INDEX40(), 0u);

    W(rec + 0xd7u) = 0x1eu;
    W(rec + 0xd9u) = 2u;
    W(rec + 0xdbu) = 0u;
    W(rec + 0xddu) = 3u;
    W(rec + 0xdfu) = 0u;
    B(rec + 0xe7u) = 4u;
    W(rec + 0xe8u) = 0x20u;
    W(rec + 0xeau) = 0x10u;
    b2 = BYTEB2();
    W(rec + 0xecu) = (u16)b2;
    b2 = BYTEB2();
    W(rec + 0xeeu) = (u16)(0x5cu - b2);
    B(rec + 0xf0u) = 0u;
    B(rec + 0xf1u) = 1u;
    B(rec + 0xf2u) = 0u;
    D(rec + 0x12cu) = 0u;
    D(rec + 0x130u) = 0xffffffffu;
    D(rec + 0x134u) = 0u;

    cm_fn_00521e60(rec, 3u, 0x1du, 9u, 0u, 3u, INDEX40());
    cm_fn_00521eb0(rec, 3u, 0x0bu, 0x0au, 0u, 2u, 2u, INDEX40(), 0u);

    W(rec + 0x13fu) = 0x28u;
    W(rec + 0x141u) = 3u;
    W(rec + 0x143u) = 0u;
    W(rec + 0x145u) = 0x203u;
    B(rec + 0x14fu) = 4u;
    W(rec + 0x150u) = 0x10u;
    W(rec + 0x152u) = 8u;
    W(rec + 0x154u) = 0u;
    W(rec + 0x147u) = 0u;
    W(rec + 0x156u) = 0u;
    B(rec + 0x158u) = 0u;
    B(rec + 0x159u) = 1u;
    B(rec + 0x15au) = 0u;
    D(rec + 0x194u) = 0u;
    D(rec + 0x198u) = 0xffffffffu;
    D(rec + 0x19cu) = 0u;

    cm_fn_00521e60(rec, 4u, 0x0cu, 0x0au, 0u, 3u, INDEX40());
    cm_fn_00521eb0(rec, 4u, 2u, 0x0bu, 0u, 2u, 2u, INDEX40(), 0u);

    W(rec + 0x1a7u) = 0x78u;
    W(rec + 0x1a9u) = 4u;
    W(rec + 0x1abu) = 0u;
    W(rec + 0x1adu) = 0x203u;
    B(rec + 0x1b7u) = 6u;
    W(rec + 0x1b8u) = 8u;
    W(rec + 0x1bau) = 4u;
    W(rec + 0x1bcu) = 0u;
    W(rec + 0x1afu) = 0u;
    W(rec + 0x1beu) = 0u;
    B(rec + 0x1c0u) = 0u;
    B(rec + 0x1c1u) = 1u;
    B(rec + 0x1c2u) = 0u;
    D(rec + 0x1fcu) = 0u;
    D(rec + 0x200u) = 0xffffffffu;
    D(rec + 0x204u) = 0u;

    cm_fn_00521e60(rec, 5u, 3u, 0x0bu, 0u, 3u, INDEX40());
    cm_fn_00521eb0(rec, 5u, 0x1bu, 0u, 1u, 2u, 2u, INDEX40(), 0u);

    W(rec + 0x20fu) = 0x82u;
    W(rec + 0x211u) = 5u;
    W(rec + 0x213u) = 0u;
    W(rec + 0x215u) = 0x4200u;
    B(rec + 0x21fu) = 6u;
    W(rec + 0x220u) = 4u;
    W(rec + 0x222u) = 2u;
    W(rec + 0x224u) = 0u;
    W(rec + 0x217u) = 0x4003u;
    W(rec + 0x226u) = 0u;
    B(rec + 0x228u) = 0u;
    B(rec + 0x229u) = 2u;
    B(rec + 0x22au) = 0x15u;
    D(rec + 0x264u) = 0u;
    D(rec + 0x268u) = 0xffffffffu;
    D(rec + 0x26cu) = 0u;

    cm_fn_00521e60(rec, 6u, 0x12u, 1u, 1u, 3u, INDEX40());
    cm_fn_00521eb0(rec, 6u, 1u, 3u, 1u, 6u, 1u, INDEX40(), 4u);

    W(rec + 0x277u) = 0x96u;
    W(rec + 0x279u) = 6u;
    W(rec + 0x27bu) = 0u;
    W(rec + 0x27du) = 3u;
    B(rec + 0x287u) = 6u;
    W(rec + 0x288u) = 2u;
    W(rec + 0x28au) = 1u;
    W(rec + 0x28cu) = 0u;
    W(rec + 0x27fu) = 0u;
    W(rec + 0x28eu) = 0u;
    B(rec + 0x290u) = 0u;
    B(rec + 0x291u) = 1u;
    B(rec + 0x292u) = 0u;
    D(rec + 0x2ccu) = 0u;
    D(rec + 0x2d0u) = 0xffffffffu;
    D(rec + 0x2d4u) = 0u;

    return (void *)rec;
}

/* Preferred-image VA 0x0065c8e0, base 0x00400000.
   CM0102/Game/cm0102.exe SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Every schedule call below preserves the original immediate pushes, receiver
   displacement and call order; each following receiver is a fresh DWORD read.
   Direct caller VA 0x0065b5d6 discards EAX before its next use. */
#include "cmre.h"

extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern void CM_STDCALL cm_fn_009463af(u32 start, u32 stride, s32 count,
                                    u32 constructor, u32 destructor);
extern void CM_STDCALL cm_fn_009462bb(u32 start, u32 stride, s32 count,
                                    u32 destructor);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *allocation);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory,
                                  void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(const char *title, const char *message,
                                   s32 flags);
extern void CM_THISCALL cm_fn_0068aff0(u32 receiver, u32 competition,
                                      u32 day, u32 month, u32 year,
                                      u32 name1, u32 name2);

void CM_THISCALL cm_fn_0065c8e0(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x204];
    s32 half, factor, count;
    u32 allocation, receiver, table, cursor, index;

    frame.state = 0xffffffffu;
    frame.handler = 0x95bd8eu;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    half = (s32)CM_GLOBAL(volatile s16, self + 0x3eu) / 2;
    factor = (s32)CM_GLOBAL(volatile s16, self + 0xa9u);
    count = (s32)((u32)half * (u32)factor);
    allocation = (u32)cm_fn_00944cf1(((u32)count << 4) + 4u);
    /* L+0 is the saved allocation; the two buffers are L+4 and L+0x104. */
    *(volatile u32 *)(scratch + 0) = allocation;
    frame.state = 0;
    if (allocation != 0) {
        receiver = allocation + 4u;
        CM_GLOBAL(volatile u32, allocation) = (u32)count;
        cm_fn_009463af(receiver, 0x10u, count, 0x00645800u, 0x005a53c0u);
    } else {
        receiver = 0;
    }
    CM_GLOBAL(volatile u32, self + 0xeau) = receiver;
    frame.state = 0xffffffffu;
    if (receiver == 0) {
        u32 text;
        cm_fn_00944cff(0x9ea83cu, 0, 0, scratch + 0x104, scratch + 4);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text,
                       scratch + 0x104, scratch + 4, 0x3cdu);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0;
    } else {
    cm_fn_0068aff0(receiver + 0x0u, self, 2u, 8u, 0x7d1u, 0x9ea918u, 0x9ea924u); /* VA 0065c9e5 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x10u, self, 2u, 8u, 0x7d1u, 0x9ea900u, 0x9ea90cu); /* VA 0065ca07 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x20u, self, 2u, 8u, 0x7d1u, 0x9ea8e8u, 0x9ea8f4u); /* VA 0065ca29 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x30u, self, 2u, 8u, 0x7d1u, 0x9ea8d8u, 0x9ea8e0u); /* VA 0065ca4b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x40u, self, 2u, 8u, 0x7d1u, 0x9ea8c4u, 0x9ea8ccu); /* VA 0065ca6d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x50u, self, 2u, 8u, 0x7d1u, 0x9ea8b0u, 0x9ea8bcu); /* VA 0065ca8f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x60u, self, 2u, 8u, 0x7d1u, 0x9ea8a0u, 0x9ea8a8u); /* VA 0065cab1 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x70u, self, 2u, 8u, 0x7d1u, 0x9ea890u, 0x9ea898u); /* VA 0065cad3 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x80u, self, 2u, 8u, 0x7d1u, 0x9ea878u, 0x9ea880u); /* VA 0065caf8 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x90u, self, 9u, 8u, 0x7d1u, 0x9ea8a8u, 0x9ea890u); /* VA 0065cb1d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa0u, self, 9u, 8u, 0x7d1u, 0x9ea924u, 0x9ea8b0u); /* VA 0065cb42 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb0u, self, 9u, 8u, 0x7d1u, 0x9ea8f4u, 0x9ea918u); /* VA 0065cb67 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc0u, self, 9u, 8u, 0x7d1u, 0x9ea90cu, 0x9ea8c4u); /* VA 0065cb8c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd0u, self, 9u, 8u, 0x7d1u, 0x9ea8e0u, 0x9ea8a0u); /* VA 0065cbb1 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe0u, self, 9u, 8u, 0x7d1u, 0x9ea898u, 0x9ea8e8u); /* VA 0065cbd6 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf0u, self, 9u, 8u, 0x7d1u, 0x9ea8bcu, 0x9ea900u); /* VA 0065cbfb */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x100u, self, 9u, 8u, 0x7d1u, 0x9ea8ccu, 0x9ea878u); /* VA 0065cc20 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x110u, self, 9u, 8u, 0x7d1u, 0x9ea880u, 0x9ea8d8u); /* VA 0065cc45 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x120u, self, 0x10u, 8u, 0x7d1u, 0x9ea918u, 0x9ea8bcu); /* VA 0065cc6a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x130u, self, 0x10u, 8u, 0x7d1u, 0x9ea900u, 0x9ea8ccu); /* VA 0065cc8f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x140u, self, 0x10u, 8u, 0x7d1u, 0x9ea8e8u, 0x9ea8a8u); /* VA 0065ccb4 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x150u, self, 0x10u, 8u, 0x7d1u, 0x9ea924u, 0x9ea8e0u); /* VA 0065ccd9 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x160u, self, 0x10u, 8u, 0x7d1u, 0x9ea8c4u, 0x9ea8f4u); /* VA 0065ccfe */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x170u, self, 0x10u, 8u, 0x7d1u, 0x9ea8b0u, 0x9ea90cu); /* VA 0065cd23 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x180u, self, 0x10u, 8u, 0x7d1u, 0x9ea8a0u, 0x9ea880u); /* VA 0065cd48 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x190u, self, 0x10u, 8u, 0x7d1u, 0x9ea890u, 0x9ea8d8u); /* VA 0065cd6d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1a0u, self, 0x10u, 8u, 0x7d1u, 0x9ea878u, 0x9ea898u); /* VA 0065cd92 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1b0u, self, 0x17u, 8u, 0x7d1u, 0x9ea8d8u, 0x9ea8a0u); /* VA 0065cdb7 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1c0u, self, 0x17u, 8u, 0x7d1u, 0x9ea8a8u, 0x9ea878u); /* VA 0065cddc */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1d0u, self, 0x17u, 8u, 0x7d1u, 0x9ea8f4u, 0x9ea8b0u); /* VA 0065ce01 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1e0u, self, 0x17u, 8u, 0x7d1u, 0x9ea90cu, 0x9ea924u); /* VA 0065ce26 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1f0u, self, 0x17u, 8u, 0x7d1u, 0x9ea8e0u, 0x9ea8e8u); /* VA 0065ce4b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x200u, self, 0x17u, 8u, 0x7d1u, 0x9ea898u, 0x9ea900u); /* VA 0065ce70 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x210u, self, 0x17u, 8u, 0x7d1u, 0x9ea8bcu, 0x9ea8c4u); /* VA 0065ce95 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x220u, self, 0x17u, 8u, 0x7d1u, 0x9ea8ccu, 0x9ea918u); /* VA 0065ceba */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x230u, self, 0x17u, 8u, 0x7d1u, 0x9ea880u, 0x9ea890u); /* VA 0065cedf */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x240u, self, 0x1eu, 8u, 0x7d1u, 0x9ea918u, 0x9ea880u); /* VA 0065cf04 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x250u, self, 0x1eu, 8u, 0x7d1u, 0x9ea900u, 0x9ea8a8u); /* VA 0065cf29 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x260u, self, 0x1eu, 8u, 0x7d1u, 0x9ea8e8u, 0x9ea8a0u); /* VA 0065cf4e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x270u, self, 0x1eu, 8u, 0x7d1u, 0x9ea924u, 0x9ea8f4u); /* VA 0065cf73 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x280u, self, 0x1eu, 8u, 0x7d1u, 0x9ea8c4u, 0x9ea898u); /* VA 0065cf98 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x290u, self, 0x1eu, 8u, 0x7d1u, 0x9ea8b0u, 0x9ea878u); /* VA 0065cfbd */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x2a0u, self, 0x1eu, 8u, 0x7d1u, 0x9ea890u, 0x9ea8e0u); /* VA 0065cfe2 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x2b0u, self, 0x1eu, 8u, 0x7d1u, 0x9ea8bcu, 0x9ea90cu); /* VA 0065d007 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x2c0u, self, 0x1eu, 8u, 0x7d1u, 0x9ea8ccu, 0x9ea8d8u); /* VA 0065d02c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x2d0u, self, 7u, 9u, 0x7d1u, 0x9ea8d8u, 0x9ea900u); /* VA 0065d051 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x2e0u, self, 7u, 9u, 0x7d1u, 0x9ea8a8u, 0x9ea8b0u); /* VA 0065d076 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x2f0u, self, 7u, 9u, 0x7d1u, 0x9ea8a0u, 0x9ea8c4u); /* VA 0065d09b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x300u, self, 7u, 9u, 0x7d1u, 0x9ea90cu, 0x9ea8f4u); /* VA 0065d0c0 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x310u, self, 7u, 9u, 0x7d1u, 0x9ea8e0u, 0x9ea8bcu); /* VA 0065d0e5 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x320u, self, 7u, 9u, 0x7d1u, 0x9ea890u, 0x9ea8e8u); /* VA 0065d10a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x330u, self, 7u, 9u, 0x7d1u, 0x9ea898u, 0x9ea924u); /* VA 0065d12f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x340u, self, 7u, 9u, 0x7d1u, 0x9ea880u, 0x9ea8ccu); /* VA 0065d154 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x350u, self, 7u, 9u, 0x7d1u, 0x9ea878u, 0x9ea918u); /* VA 0065d179 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x360u, self, 0xeu, 9u, 0x7d1u, 0x9ea918u, 0x9ea8a0u); /* VA 0065d19e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x370u, self, 0xeu, 9u, 0x7d1u, 0x9ea900u, 0x9ea890u); /* VA 0065d1c3 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x380u, self, 0xeu, 9u, 0x7d1u, 0x9ea924u, 0x9ea8c4u); /* VA 0065d1e8 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x390u, self, 0xeu, 9u, 0x7d1u, 0x9ea8b0u, 0x9ea8e8u); /* VA 0065d20d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x3a0u, self, 0xeu, 9u, 0x7d1u, 0x9ea8f4u, 0x9ea8ccu); /* VA 0065d232 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x3b0u, self, 0xeu, 9u, 0x7d1u, 0x9ea90cu, 0x9ea880u); /* VA 0065d257 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x3c0u, self, 0xeu, 9u, 0x7d1u, 0x9ea898u, 0x9ea8d8u); /* VA 0065d27c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x3d0u, self, 0xeu, 9u, 0x7d1u, 0x9ea8bcu, 0x9ea8a8u); /* VA 0065d2a1 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x3e0u, self, 0xeu, 9u, 0x7d1u, 0x9ea878u, 0x9ea8e0u); /* VA 0065d2c6 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x3f0u, self, 0x15u, 9u, 0x7d1u, 0x9ea8e8u, 0x9ea918u); /* VA 0065d2eb */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x400u, self, 0x15u, 9u, 0x7d1u, 0x9ea8d8u, 0x9ea878u); /* VA 0065d310 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x410u, self, 0x15u, 9u, 0x7d1u, 0x9ea8a8u, 0x9ea8f4u); /* VA 0065d335 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x420u, self, 0x15u, 9u, 0x7d1u, 0x9ea8c4u, 0x9ea900u); /* VA 0065d35a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x430u, self, 0x15u, 9u, 0x7d1u, 0x9ea8a0u, 0x9ea8bcu); /* VA 0065d37f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x440u, self, 0x15u, 9u, 0x7d1u, 0x9ea8e0u, 0x9ea90cu); /* VA 0065d3a4 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x450u, self, 0x15u, 9u, 0x7d1u, 0x9ea890u, 0x9ea924u); /* VA 0065d3c9 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x460u, self, 0x15u, 9u, 0x7d1u, 0x9ea8ccu, 0x9ea8b0u); /* VA 0065d3ee */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x470u, self, 0x15u, 9u, 0x7d1u, 0x9ea880u, 0x9ea898u); /* VA 0065d413 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x480u, self, 0x1cu, 9u, 0x7d1u, 0x9ea918u, 0x9ea8c4u); /* VA 0065d438 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x490u, self, 0x1cu, 9u, 0x7d1u, 0x9ea924u, 0x9ea900u); /* VA 0065d45d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x4a0u, self, 0x1cu, 9u, 0x7d1u, 0x9ea8b0u, 0x9ea890u); /* VA 0065d482 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x4b0u, self, 0x1cu, 9u, 0x7d1u, 0x9ea8f4u, 0x9ea8a0u); /* VA 0065d4a7 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x4c0u, self, 0x1cu, 9u, 0x7d1u, 0x9ea90cu, 0x9ea8d8u); /* VA 0065d4cc */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x4d0u, self, 0x1cu, 9u, 0x7d1u, 0x9ea898u, 0x9ea8a8u); /* VA 0065d4f1 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x4e0u, self, 0x1cu, 9u, 0x7d1u, 0x9ea8bcu, 0x9ea8ccu); /* VA 0065d516 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x4f0u, self, 0x1cu, 9u, 0x7d1u, 0x9ea880u, 0x9ea8e0u); /* VA 0065d53b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x500u, self, 0x1cu, 9u, 0x7d1u, 0x9ea878u, 0x9ea8e8u); /* VA 0065d560 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x510u, self, 4u, 0xau, 0x7d1u, 0x9ea900u, 0x9ea918u); /* VA 0065d585 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x520u, self, 4u, 0xau, 0x7d1u, 0x9ea8e8u, 0x9ea8bcu); /* VA 0065d5aa */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x530u, self, 4u, 0xau, 0x7d1u, 0x9ea8d8u, 0x9ea8f4u); /* VA 0065d5cf */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x540u, self, 4u, 0xau, 0x7d1u, 0x9ea8a8u, 0x9ea880u); /* VA 0065d5f4 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x550u, self, 4u, 0xau, 0x7d1u, 0x9ea8c4u, 0x9ea8b0u); /* VA 0065d619 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x560u, self, 4u, 0xau, 0x7d1u, 0x9ea8a0u, 0x9ea878u); /* VA 0065d63e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x570u, self, 4u, 0xau, 0x7d1u, 0x9ea8e0u, 0x9ea898u); /* VA 0065d663 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x580u, self, 4u, 0xau, 0x7d1u, 0x9ea890u, 0x9ea90cu); /* VA 0065d688 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x590u, self, 4u, 0xau, 0x7d1u, 0x9ea8ccu, 0x9ea924u); /* VA 0065d6ad */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x5a0u, self, 0xbu, 0xau, 0x7d1u, 0x9ea8d8u, 0x9ea8a8u); /* VA 0065d6d2 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x5b0u, self, 0xbu, 0xau, 0x7d1u, 0x9ea8b0u, 0x9ea918u); /* VA 0065d6f7 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x5c0u, self, 0xbu, 0xau, 0x7d1u, 0x9ea8f4u, 0x9ea900u); /* VA 0065d71c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x5d0u, self, 0xbu, 0xau, 0x7d1u, 0x9ea90cu, 0x9ea8ccu); /* VA 0065d741 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x5e0u, self, 0xbu, 0xau, 0x7d1u, 0x9ea8e0u, 0x9ea8c4u); /* VA 0065d766 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x5f0u, self, 0xbu, 0xau, 0x7d1u, 0x9ea898u, 0x9ea8a0u); /* VA 0065d78b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x600u, self, 0xbu, 0xau, 0x7d1u, 0x9ea8bcu, 0x9ea924u); /* VA 0065d7b0 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x610u, self, 0xbu, 0xau, 0x7d1u, 0x9ea880u, 0x9ea8e8u); /* VA 0065d7d5 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x620u, self, 0xbu, 0xau, 0x7d1u, 0x9ea878u, 0x9ea890u); /* VA 0065d7fa */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x630u, self, 0x12u, 0xau, 0x7d1u, 0x9ea918u, 0x9ea90cu); /* VA 0065d81f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x640u, self, 0x12u, 0xau, 0x7d1u, 0x9ea900u, 0x9ea8b0u); /* VA 0065d844 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x650u, self, 0x12u, 0xau, 0x7d1u, 0x9ea8e8u, 0x9ea8d8u); /* VA 0065d869 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x660u, self, 0x12u, 0xau, 0x7d1u, 0x9ea8a8u, 0x9ea8e0u); /* VA 0065d88e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x670u, self, 0x12u, 0xau, 0x7d1u, 0x9ea924u, 0x9ea880u); /* VA 0065d8b3 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x680u, self, 0x12u, 0xau, 0x7d1u, 0x9ea8c4u, 0x9ea878u); /* VA 0065d8d8 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x690u, self, 0x12u, 0xau, 0x7d1u, 0x9ea8a0u, 0x9ea890u); /* VA 0065d8fd */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x6a0u, self, 0x12u, 0xau, 0x7d1u, 0x9ea8f4u, 0x9ea8bcu); /* VA 0065d922 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x6b0u, self, 0x12u, 0xau, 0x7d1u, 0x9ea8ccu, 0x9ea898u); /* VA 0065d947 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x6c0u, self, 0x19u, 0xau, 0x7d1u, 0x9ea8e8u, 0x9ea900u); /* VA 0065d96c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x6d0u, self, 0x19u, 0xau, 0x7d1u, 0x9ea8d8u, 0x9ea8bcu); /* VA 0065d991 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x6e0u, self, 0x19u, 0xau, 0x7d1u, 0x9ea8a8u, 0x9ea918u); /* VA 0065d9b6 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x6f0u, self, 0x19u, 0xau, 0x7d1u, 0x9ea8a0u, 0x9ea8b0u); /* VA 0065d9db */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x700u, self, 0x19u, 0xau, 0x7d1u, 0x9ea8e0u, 0x9ea8ccu); /* VA 0065da00 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x710u, self, 0x19u, 0xau, 0x7d1u, 0x9ea890u, 0x9ea8c4u); /* VA 0065da25 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x720u, self, 0x19u, 0xau, 0x7d1u, 0x9ea898u, 0x9ea90cu); /* VA 0065da4a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x730u, self, 0x19u, 0xau, 0x7d1u, 0x9ea880u, 0x9ea8f4u); /* VA 0065da6f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x740u, self, 0x19u, 0xau, 0x7d1u, 0x9ea878u, 0x9ea924u); /* VA 0065da94 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x750u, self, 2u, 0xbu, 0x7d1u, 0x9ea918u, 0x9ea8e0u); /* VA 0065dab9 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x760u, self, 2u, 0xbu, 0x7d1u, 0x9ea900u, 0x9ea8a0u); /* VA 0065dade */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x770u, self, 2u, 0xbu, 0x7d1u, 0x9ea924u, 0x9ea8a8u); /* VA 0065db03 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x780u, self, 2u, 0xbu, 0x7d1u, 0x9ea8c4u, 0x9ea8e8u); /* VA 0065db28 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x790u, self, 2u, 0xbu, 0x7d1u, 0x9ea8b0u, 0x9ea8d8u); /* VA 0065db4d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x7a0u, self, 2u, 0xbu, 0x7d1u, 0x9ea8f4u, 0x9ea898u); /* VA 0065db72 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x7b0u, self, 2u, 0xbu, 0x7d1u, 0x9ea90cu, 0x9ea878u); /* VA 0065db97 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x7c0u, self, 2u, 0xbu, 0x7d1u, 0x9ea8bcu, 0x9ea880u); /* VA 0065dbbc */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x7d0u, self, 2u, 0xbu, 0x7d1u, 0x9ea8ccu, 0x9ea890u); /* VA 0065dbe1 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x7e0u, self, 9u, 0xbu, 0x7d1u, 0x9ea8e8u, 0x9ea8ccu); /* VA 0065dc06 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x7f0u, self, 9u, 0xbu, 0x7d1u, 0x9ea8d8u, 0x9ea924u); /* VA 0065dc2b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x800u, self, 9u, 0xbu, 0x7d1u, 0x9ea8a8u, 0x9ea8c4u); /* VA 0065dc50 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x810u, self, 9u, 0xbu, 0x7d1u, 0x9ea8a0u, 0x9ea90cu); /* VA 0065dc75 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x820u, self, 9u, 0xbu, 0x7d1u, 0x9ea8e0u, 0x9ea8b0u); /* VA 0065dc9a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x830u, self, 9u, 0xbu, 0x7d1u, 0x9ea890u, 0x9ea918u); /* VA 0065dcbf */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x840u, self, 9u, 0xbu, 0x7d1u, 0x9ea898u, 0x9ea8bcu); /* VA 0065dce4 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x850u, self, 9u, 0xbu, 0x7d1u, 0x9ea880u, 0x9ea900u); /* VA 0065dd09 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x860u, self, 9u, 0xbu, 0x7d1u, 0x9ea878u, 0x9ea8f4u); /* VA 0065dd2e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x870u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea918u, 0x9ea898u); /* VA 0065dd53 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x880u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea900u, 0x9ea878u); /* VA 0065dd78 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x890u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea924u, 0x9ea8e8u); /* VA 0065dd9d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x8a0u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea8c4u, 0x9ea8d8u); /* VA 0065ddc2 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x8b0u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea8b0u, 0x9ea880u); /* VA 0065dde7 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x8c0u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea8f4u, 0x9ea8e0u); /* VA 0065de0c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x8d0u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea90cu, 0x9ea8a8u); /* VA 0065de31 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x8e0u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea8bcu, 0x9ea890u); /* VA 0065de56 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x8f0u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea8ccu, 0x9ea8a0u); /* VA 0065de7b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x900u, self, 0x17u, 0xbu, 0x7d1u, 0x9ea8e8u, 0x9ea90cu); /* VA 0065dea0 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x910u, self, 0x17u, 0xbu, 0x7d1u, 0x9ea8d8u, 0x9ea918u); /* VA 0065dec5 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x920u, self, 0x17u, 0xbu, 0x7d1u, 0x9ea8a8u, 0x9ea8ccu); /* VA 0065deea */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x930u, self, 0x17u, 0xbu, 0x7d1u, 0x9ea8a0u, 0x9ea924u); /* VA 0065df0f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x940u, self, 0x17u, 0xbu, 0x7d1u, 0x9ea8e0u, 0x9ea900u); /* VA 0065df34 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x950u, self, 0x17u, 0xbu, 0x7d1u, 0x9ea890u, 0x9ea8f4u); /* VA 0065df59 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x960u, self, 0x17u, 0xbu, 0x7d1u, 0x9ea898u, 0x9ea8b0u); /* VA 0065df7e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x970u, self, 0x17u, 0xbu, 0x7d1u, 0x9ea880u, 0x9ea8c4u); /* VA 0065dfa3 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x980u, self, 0x17u, 0xbu, 0x7d1u, 0x9ea878u, 0x9ea8bcu); /* VA 0065dfc8 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x990u, self, 6u, 0u, 0x7d2u, 0x9ea924u, 0x9ea918u); /* VA 0065dfed */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x9a0u, self, 6u, 0u, 0x7d2u, 0x9ea90cu, 0x9ea900u); /* VA 0065e012 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x9b0u, self, 6u, 0u, 0x7d2u, 0x9ea8f4u, 0x9ea8e8u); /* VA 0065e037 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x9c0u, self, 6u, 0u, 0x7d2u, 0x9ea8e0u, 0x9ea8d8u); /* VA 0065e05c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x9d0u, self, 6u, 0u, 0x7d2u, 0x9ea8ccu, 0x9ea8c4u); /* VA 0065e081 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x9e0u, self, 6u, 0u, 0x7d2u, 0x9ea8bcu, 0x9ea8b0u); /* VA 0065e0a6 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x9f0u, self, 6u, 0u, 0x7d2u, 0x9ea8a8u, 0x9ea8a0u); /* VA 0065e0cb */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa00u, self, 6u, 0u, 0x7d2u, 0x9ea898u, 0x9ea890u); /* VA 0065e0f0 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa10u, self, 6u, 0u, 0x7d2u, 0x9ea880u, 0x9ea878u); /* VA 0065e115 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa20u, self, 0xdu, 0u, 0x7d2u, 0x9ea890u, 0x9ea8a8u); /* VA 0065e13a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa30u, self, 0xdu, 0u, 0x7d2u, 0x9ea8b0u, 0x9ea924u); /* VA 0065e15f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa40u, self, 0xdu, 0u, 0x7d2u, 0x9ea918u, 0x9ea8f4u); /* VA 0065e184 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa50u, self, 0xdu, 0u, 0x7d2u, 0x9ea8c4u, 0x9ea90cu); /* VA 0065e1a9 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa60u, self, 0xdu, 0u, 0x7d2u, 0x9ea8a0u, 0x9ea8e0u); /* VA 0065e1ce */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa70u, self, 0xdu, 0u, 0x7d2u, 0x9ea8e8u, 0x9ea898u); /* VA 0065e1f3 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa80u, self, 0xdu, 0u, 0x7d2u, 0x9ea900u, 0x9ea8bcu); /* VA 0065e218 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa90u, self, 0xdu, 0u, 0x7d2u, 0x9ea878u, 0x9ea8ccu); /* VA 0065e23d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xaa0u, self, 0xdu, 0u, 0x7d2u, 0x9ea8d8u, 0x9ea880u); /* VA 0065e262 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xab0u, self, 0x14u, 0u, 0x7d2u, 0x9ea8bcu, 0x9ea918u); /* VA 0065e287 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xac0u, self, 0x14u, 0u, 0x7d2u, 0x9ea8ccu, 0x9ea900u); /* VA 0065e2ac */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xad0u, self, 0x14u, 0u, 0x7d2u, 0x9ea8a8u, 0x9ea8e8u); /* VA 0065e2d1 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xae0u, self, 0x14u, 0u, 0x7d2u, 0x9ea8e0u, 0x9ea924u); /* VA 0065e2f6 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xaf0u, self, 0x14u, 0u, 0x7d2u, 0x9ea8f4u, 0x9ea8c4u); /* VA 0065e31b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb00u, self, 0x14u, 0u, 0x7d2u, 0x9ea90cu, 0x9ea8b0u); /* VA 0065e340 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb10u, self, 0x14u, 0u, 0x7d2u, 0x9ea880u, 0x9ea8a0u); /* VA 0065e365 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb20u, self, 0x14u, 0u, 0x7d2u, 0x9ea8d8u, 0x9ea890u); /* VA 0065e38a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb30u, self, 0x14u, 0u, 0x7d2u, 0x9ea898u, 0x9ea878u); /* VA 0065e3af */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb40u, self, 0x1bu, 0u, 0x7d2u, 0x9ea8a0u, 0x9ea8d8u); /* VA 0065e3d4 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb50u, self, 0x1bu, 0u, 0x7d2u, 0x9ea878u, 0x9ea8a8u); /* VA 0065e3f9 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb60u, self, 0x1bu, 0u, 0x7d2u, 0x9ea8b0u, 0x9ea8f4u); /* VA 0065e41e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb70u, self, 0x1bu, 0u, 0x7d2u, 0x9ea924u, 0x9ea90cu); /* VA 0065e443 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb80u, self, 0x1bu, 0u, 0x7d2u, 0x9ea8e8u, 0x9ea8e0u); /* VA 0065e468 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb90u, self, 0x1bu, 0u, 0x7d2u, 0x9ea900u, 0x9ea898u); /* VA 0065e48d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xba0u, self, 0x1bu, 0u, 0x7d2u, 0x9ea8c4u, 0x9ea8bcu); /* VA 0065e4b2 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xbb0u, self, 0x1bu, 0u, 0x7d2u, 0x9ea918u, 0x9ea8ccu); /* VA 0065e4d7 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xbc0u, self, 0x1bu, 0u, 0x7d2u, 0x9ea890u, 0x9ea880u); /* VA 0065e4fc */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xbd0u, self, 3u, 1u, 0x7d2u, 0x9ea880u, 0x9ea918u); /* VA 0065e521 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xbe0u, self, 3u, 1u, 0x7d2u, 0x9ea8a8u, 0x9ea900u); /* VA 0065e546 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xbf0u, self, 3u, 1u, 0x7d2u, 0x9ea8a0u, 0x9ea8e8u); /* VA 0065e56b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc00u, self, 3u, 1u, 0x7d2u, 0x9ea8f4u, 0x9ea924u); /* VA 0065e590 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc10u, self, 3u, 1u, 0x7d2u, 0x9ea898u, 0x9ea8c4u); /* VA 0065e5b5 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc20u, self, 3u, 1u, 0x7d2u, 0x9ea878u, 0x9ea8b0u); /* VA 0065e5da */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc30u, self, 3u, 1u, 0x7d2u, 0x9ea8e0u, 0x9ea890u); /* VA 0065e5ff */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc40u, self, 3u, 1u, 0x7d2u, 0x9ea90cu, 0x9ea8bcu); /* VA 0065e624 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc50u, self, 3u, 1u, 0x7d2u, 0x9ea8d8u, 0x9ea8ccu); /* VA 0065e649 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc60u, self, 0xau, 1u, 0x7d2u, 0x9ea900u, 0x9ea8d8u); /* VA 0065e66e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc70u, self, 0xau, 1u, 0x7d2u, 0x9ea8b0u, 0x9ea8a8u); /* VA 0065e693 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc80u, self, 0xau, 1u, 0x7d2u, 0x9ea8c4u, 0x9ea8a0u); /* VA 0065e6b8 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc90u, self, 0xau, 1u, 0x7d2u, 0x9ea8f4u, 0x9ea90cu); /* VA 0065e6dd */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xca0u, self, 0xau, 1u, 0x7d2u, 0x9ea8bcu, 0x9ea8e0u); /* VA 0065e702 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xcb0u, self, 0xau, 1u, 0x7d2u, 0x9ea8e8u, 0x9ea890u); /* VA 0065e727 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xcc0u, self, 0xau, 1u, 0x7d2u, 0x9ea924u, 0x9ea898u); /* VA 0065e74c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xcd0u, self, 0xau, 1u, 0x7d2u, 0x9ea8ccu, 0x9ea880u); /* VA 0065e771 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xce0u, self, 0xau, 1u, 0x7d2u, 0x9ea918u, 0x9ea878u); /* VA 0065e796 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xcf0u, self, 0x11u, 1u, 0x7d2u, 0x9ea8a0u, 0x9ea918u); /* VA 0065e7bb */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd00u, self, 0x11u, 1u, 0x7d2u, 0x9ea890u, 0x9ea900u); /* VA 0065e7e0 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd10u, self, 0x11u, 1u, 0x7d2u, 0x9ea8c4u, 0x9ea924u); /* VA 0065e805 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd20u, self, 0x11u, 1u, 0x7d2u, 0x9ea8e8u, 0x9ea8b0u); /* VA 0065e82a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd30u, self, 0x11u, 1u, 0x7d2u, 0x9ea8ccu, 0x9ea8f4u); /* VA 0065e84f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd40u, self, 0x11u, 1u, 0x7d2u, 0x9ea880u, 0x9ea90cu); /* VA 0065e874 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd50u, self, 0x11u, 1u, 0x7d2u, 0x9ea8d8u, 0x9ea898u); /* VA 0065e899 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd60u, self, 0x11u, 1u, 0x7d2u, 0x9ea8a8u, 0x9ea8bcu); /* VA 0065e8be */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd70u, self, 0x11u, 1u, 0x7d2u, 0x9ea8e0u, 0x9ea878u); /* VA 0065e8e3 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd80u, self, 0x18u, 1u, 0x7d2u, 0x9ea918u, 0x9ea8e8u); /* VA 0065e908 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd90u, self, 0x18u, 1u, 0x7d2u, 0x9ea878u, 0x9ea8d8u); /* VA 0065e92d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xda0u, self, 0x18u, 1u, 0x7d2u, 0x9ea8f4u, 0x9ea8a8u); /* VA 0065e952 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xdb0u, self, 0x18u, 1u, 0x7d2u, 0x9ea900u, 0x9ea8c4u); /* VA 0065e977 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xdc0u, self, 0x18u, 1u, 0x7d2u, 0x9ea8bcu, 0x9ea8a0u); /* VA 0065e99c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xdd0u, self, 0x18u, 1u, 0x7d2u, 0x9ea90cu, 0x9ea8e0u); /* VA 0065e9c1 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xde0u, self, 0x18u, 1u, 0x7d2u, 0x9ea924u, 0x9ea890u); /* VA 0065e9e6 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xdf0u, self, 0x18u, 1u, 0x7d2u, 0x9ea8b0u, 0x9ea8ccu); /* VA 0065ea0b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe00u, self, 0x18u, 1u, 0x7d2u, 0x9ea898u, 0x9ea880u); /* VA 0065ea30 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe10u, self, 3u, 2u, 0x7d2u, 0x9ea8c4u, 0x9ea918u); /* VA 0065ea55 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe20u, self, 3u, 2u, 0x7d2u, 0x9ea900u, 0x9ea924u); /* VA 0065ea7a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe30u, self, 3u, 2u, 0x7d2u, 0x9ea890u, 0x9ea8b0u); /* VA 0065ea9f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe40u, self, 3u, 2u, 0x7d2u, 0x9ea8a0u, 0x9ea8f4u); /* VA 0065eac4 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe50u, self, 3u, 2u, 0x7d2u, 0x9ea8d8u, 0x9ea90cu); /* VA 0065eae9 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe60u, self, 3u, 2u, 0x7d2u, 0x9ea8a8u, 0x9ea898u); /* VA 0065eb0e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe70u, self, 3u, 2u, 0x7d2u, 0x9ea8ccu, 0x9ea8bcu); /* VA 0065eb33 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe80u, self, 3u, 2u, 0x7d2u, 0x9ea8e0u, 0x9ea880u); /* VA 0065eb58 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe90u, self, 3u, 2u, 0x7d2u, 0x9ea8e8u, 0x9ea878u); /* VA 0065eb7d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xea0u, self, 0xau, 2u, 0x7d2u, 0x9ea918u, 0x9ea900u); /* VA 0065eba2 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xeb0u, self, 0xau, 2u, 0x7d2u, 0x9ea8bcu, 0x9ea8e8u); /* VA 0065ebc7 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xec0u, self, 0xau, 2u, 0x7d2u, 0x9ea8f4u, 0x9ea8d8u); /* VA 0065ebec */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xed0u, self, 0xau, 2u, 0x7d2u, 0x9ea880u, 0x9ea8a8u); /* VA 0065ec11 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xee0u, self, 0xau, 2u, 0x7d2u, 0x9ea8b0u, 0x9ea8c4u); /* VA 0065ec36 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xef0u, self, 0xau, 2u, 0x7d2u, 0x9ea878u, 0x9ea8a0u); /* VA 0065ec5b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf00u, self, 0xau, 2u, 0x7d2u, 0x9ea898u, 0x9ea8e0u); /* VA 0065ec80 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf10u, self, 0xau, 2u, 0x7d2u, 0x9ea90cu, 0x9ea890u); /* VA 0065eca5 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf20u, self, 0xau, 2u, 0x7d2u, 0x9ea924u, 0x9ea8ccu); /* VA 0065ecca */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf30u, self, 0x18u, 2u, 0x7d2u, 0x9ea8a8u, 0x9ea8d8u); /* VA 0065ecef */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf40u, self, 0x18u, 2u, 0x7d2u, 0x9ea918u, 0x9ea8b0u); /* VA 0065ed14 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf50u, self, 0x18u, 2u, 0x7d2u, 0x9ea900u, 0x9ea8f4u); /* VA 0065ed39 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf60u, self, 0x18u, 2u, 0x7d2u, 0x9ea8ccu, 0x9ea90cu); /* VA 0065ed5e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf70u, self, 0x18u, 2u, 0x7d2u, 0x9ea8c4u, 0x9ea8e0u); /* VA 0065ed83 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf80u, self, 0x18u, 2u, 0x7d2u, 0x9ea8a0u, 0x9ea898u); /* VA 0065eda8 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf90u, self, 0x18u, 2u, 0x7d2u, 0x9ea924u, 0x9ea8bcu); /* VA 0065edcd */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xfa0u, self, 0x18u, 2u, 0x7d2u, 0x9ea8e8u, 0x9ea880u); /* VA 0065edf2 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xfb0u, self, 0x18u, 2u, 0x7d2u, 0x9ea890u, 0x9ea878u); /* VA 0065ee17 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xfc0u, self, 0x1eu, 2u, 0x7d2u, 0x9ea90cu, 0x9ea918u); /* VA 0065ee3c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xfd0u, self, 0x1eu, 2u, 0x7d2u, 0x9ea8b0u, 0x9ea900u); /* VA 0065ee61 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xfe0u, self, 0x1eu, 2u, 0x7d2u, 0x9ea8d8u, 0x9ea8e8u); /* VA 0065ee86 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xff0u, self, 0x1eu, 2u, 0x7d2u, 0x9ea8e0u, 0x9ea8a8u); /* VA 0065eeab */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1000u, self, 0x1eu, 2u, 0x7d2u, 0x9ea880u, 0x9ea924u); /* VA 0065eed0 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1010u, self, 0x1eu, 2u, 0x7d2u, 0x9ea878u, 0x9ea8c4u); /* VA 0065eef5 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1020u, self, 0x1eu, 2u, 0x7d2u, 0x9ea890u, 0x9ea8a0u); /* VA 0065ef1a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1030u, self, 0x1eu, 2u, 0x7d2u, 0x9ea8bcu, 0x9ea8f4u); /* VA 0065ef3f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1040u, self, 0x1eu, 2u, 0x7d2u, 0x9ea898u, 0x9ea8ccu); /* VA 0065ef64 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1050u, self, 7u, 3u, 0x7d2u, 0x9ea900u, 0x9ea8e8u); /* VA 0065ef89 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1060u, self, 7u, 3u, 0x7d2u, 0x9ea8bcu, 0x9ea8d8u); /* VA 0065efae */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1070u, self, 7u, 3u, 0x7d2u, 0x9ea918u, 0x9ea8a8u); /* VA 0065efd3 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1080u, self, 7u, 3u, 0x7d2u, 0x9ea8b0u, 0x9ea8a0u); /* VA 0065eff8 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1090u, self, 7u, 3u, 0x7d2u, 0x9ea8ccu, 0x9ea8e0u); /* VA 0065f01d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x10a0u, self, 7u, 3u, 0x7d2u, 0x9ea8c4u, 0x9ea890u); /* VA 0065f042 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x10b0u, self, 7u, 3u, 0x7d2u, 0x9ea90cu, 0x9ea898u); /* VA 0065f067 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x10c0u, self, 7u, 3u, 0x7d2u, 0x9ea8f4u, 0x9ea880u); /* VA 0065f08c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x10d0u, self, 7u, 3u, 0x7d2u, 0x9ea924u, 0x9ea878u); /* VA 0065f0b1 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x10e0u, self, 0xeu, 3u, 0x7d2u, 0x9ea8e0u, 0x9ea918u); /* VA 0065f0d6 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x10f0u, self, 0xeu, 3u, 0x7d2u, 0x9ea8a0u, 0x9ea900u); /* VA 0065f0fb */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1100u, self, 0xeu, 3u, 0x7d2u, 0x9ea8a8u, 0x9ea924u); /* VA 0065f120 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1110u, self, 0xeu, 3u, 0x7d2u, 0x9ea8e8u, 0x9ea8c4u); /* VA 0065f145 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1120u, self, 0xeu, 3u, 0x7d2u, 0x9ea8d8u, 0x9ea8b0u); /* VA 0065f16a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1130u, self, 0xeu, 3u, 0x7d2u, 0x9ea898u, 0x9ea8f4u); /* VA 0065f18f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1140u, self, 0xeu, 3u, 0x7d2u, 0x9ea878u, 0x9ea90cu); /* VA 0065f1b4 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1150u, self, 0xeu, 3u, 0x7d2u, 0x9ea880u, 0x9ea8bcu); /* VA 0065f1d9 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1160u, self, 0xeu, 3u, 0x7d2u, 0x9ea890u, 0x9ea8ccu); /* VA 0065f1fe */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1170u, self, 0x15u, 3u, 0x7d2u, 0x9ea8ccu, 0x9ea8e8u); /* VA 0065f223 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1180u, self, 0x15u, 3u, 0x7d2u, 0x9ea924u, 0x9ea8d8u); /* VA 0065f248 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1190u, self, 0x15u, 3u, 0x7d2u, 0x9ea8c4u, 0x9ea8a8u); /* VA 0065f26d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x11a0u, self, 0x15u, 3u, 0x7d2u, 0x9ea90cu, 0x9ea8a0u); /* VA 0065f292 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x11b0u, self, 0x15u, 3u, 0x7d2u, 0x9ea8b0u, 0x9ea8e0u); /* VA 0065f2b7 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x11c0u, self, 0x15u, 3u, 0x7d2u, 0x9ea918u, 0x9ea890u); /* VA 0065f2dc */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x11d0u, self, 0x15u, 3u, 0x7d2u, 0x9ea8bcu, 0x9ea898u); /* VA 0065f301 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x11e0u, self, 0x15u, 3u, 0x7d2u, 0x9ea900u, 0x9ea880u); /* VA 0065f326 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x11f0u, self, 0x15u, 3u, 0x7d2u, 0x9ea8f4u, 0x9ea878u); /* VA 0065f34b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1200u, self, 0x1cu, 3u, 0x7d2u, 0x9ea898u, 0x9ea918u); /* VA 0065f370 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1210u, self, 0x1cu, 3u, 0x7d2u, 0x9ea878u, 0x9ea900u); /* VA 0065f395 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1220u, self, 0x1cu, 3u, 0x7d2u, 0x9ea8e8u, 0x9ea924u); /* VA 0065f3ba */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1230u, self, 0x1cu, 3u, 0x7d2u, 0x9ea8d8u, 0x9ea8c4u); /* VA 0065f3df */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1240u, self, 0x1cu, 3u, 0x7d2u, 0x9ea880u, 0x9ea8b0u); /* VA 0065f404 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1250u, self, 0x1cu, 3u, 0x7d2u, 0x9ea8e0u, 0x9ea8f4u); /* VA 0065f429 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1260u, self, 0x1cu, 3u, 0x7d2u, 0x9ea8a8u, 0x9ea90cu); /* VA 0065f44e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1270u, self, 0x1cu, 3u, 0x7d2u, 0x9ea890u, 0x9ea8bcu); /* VA 0065f473 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1280u, self, 0x1cu, 3u, 0x7d2u, 0x9ea8a0u, 0x9ea8ccu); /* VA 0065f498 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1290u, self, 5u, 4u, 0x7d2u, 0x9ea90cu, 0x9ea8e8u); /* VA 0065f4bd */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x12a0u, self, 5u, 4u, 0x7d2u, 0x9ea918u, 0x9ea8d8u); /* VA 0065f4e2 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x12b0u, self, 5u, 4u, 0x7d2u, 0x9ea8ccu, 0x9ea8a8u); /* VA 0065f507 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x12c0u, self, 5u, 4u, 0x7d2u, 0x9ea924u, 0x9ea8a0u); /* VA 0065f52c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x12d0u, self, 5u, 4u, 0x7d2u, 0x9ea900u, 0x9ea8e0u); /* VA 0065f551 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x12e0u, self, 5u, 4u, 0x7d2u, 0x9ea8f4u, 0x9ea890u); /* VA 0065f576 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x12f0u, self, 5u, 4u, 0x7d2u, 0x9ea8b0u, 0x9ea898u); /* VA 0065f59b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1300u, self, 5u, 4u, 0x7d2u, 0x9ea8c4u, 0x9ea880u); /* VA 0065f5c0 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1310u, self, 5u, 4u, 0x7d2u, 0x9ea8bcu, 0x9ea878u); /* VA 0065f5e5 */
    half = (s32)CM_GLOBAL(volatile s16, self + 0x3eu) / 2;
    factor = (s32)CM_GLOBAL(volatile s16, self + 0xa9u);
    count = (s32)((u32)half * (u32)factor);
    if (count > 0) {
        table = CM_GLOBAL(volatile u32, self + 0xeau);
        cursor = table + 0xcu;
        index = 0;
        do {
            if (CM_GLOBAL(volatile u32, cursor - 4u) == 0xffffffffu ||
                CM_GLOBAL(volatile u32, cursor) == 0xffffffffu) {
                if (table != 0) {
                    u32 length = CM_GLOBAL(volatile u32, table - 4u);
                    u32 previous = table - 4u;
                    cm_fn_009462bb(table, 0x10u, (s32)length, 0x005a53c0u);
                    cm_fn_00944c94((void *)previous);
                }
                CM_GLOBAL(volatile u32, self + 0xeau) = 0;
                break;
            }
            ++index;
            cursor += 0x10u;
        } while ((s32)index < count);
    }
    }
    CM_FS(u32, 0) = frame.prev;
}

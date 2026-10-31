/* Preferred-image VA 0x00650100, base 0x00400000.
   CM0102/Game/cm0102.exe SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Every schedule call below preserves the original immediate pushes, receiver
   displacement and call order; each following receiver is a fresh DWORD read.
   Direct caller VA 0x0064ed16 discards EAX before its next use. */
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

void CM_THISCALL cm_fn_00650100(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x204];
    s32 half, factor, count;
    u32 allocation, receiver, table, cursor, index;

    frame.state = 0xffffffffu;
    frame.handler = 0x95bbaeu;
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
        cm_fn_00944cff(0x9ea558u, 0, 0, scratch + 0x104, scratch + 4);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text,
                       scratch + 0x104, scratch + 4, 0x404u);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0;
    } else {
    cm_fn_0068aff0(receiver + 0x0u, self, 2u, 8u, 0x7d1u, 0x9ea638u, 0x9ea640u); /* VA 00650205 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x10u, self, 2u, 8u, 0x7d1u, 0x9ea620u, 0x9ea62cu); /* VA 00650227 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x20u, self, 2u, 8u, 0x7d1u, 0x9ea60cu, 0x9ea614u); /* VA 00650249 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x30u, self, 2u, 8u, 0x7d1u, 0x9ea5f8u, 0x9ea604u); /* VA 0065026b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x40u, self, 2u, 8u, 0x7d1u, 0x9ea5e0u, 0x9ea5ecu); /* VA 0065028d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x50u, self, 2u, 8u, 0x7d1u, 0x9ea5d0u, 0x9ea5d8u); /* VA 006502af */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x60u, self, 2u, 8u, 0x7d1u, 0x9ea5bcu, 0x9ea5c4u); /* VA 006502d1 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x70u, self, 2u, 8u, 0x7d1u, 0x9ea5a8u, 0x9ea5b0u); /* VA 006502f3 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x80u, self, 2u, 8u, 0x7d1u, 0x9ea594u, 0x9ea5a0u); /* VA 00650318 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x90u, self, 9u, 8u, 0x7d1u, 0x9ea614u, 0x9ea594u); /* VA 0065033d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa0u, self, 9u, 8u, 0x7d1u, 0x9ea640u, 0x9ea5f8u); /* VA 00650362 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb0u, self, 9u, 8u, 0x7d1u, 0x9ea604u, 0x9ea5d0u); /* VA 00650387 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc0u, self, 9u, 8u, 0x7d1u, 0x9ea5d8u, 0x9ea5e0u); /* VA 006503ac */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd0u, self, 9u, 8u, 0x7d1u, 0x9ea62cu, 0x9ea5bcu); /* VA 006503d1 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe0u, self, 9u, 8u, 0x7d1u, 0x9ea5ecu, 0x9ea5a8u); /* VA 006503f6 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf0u, self, 9u, 8u, 0x7d1u, 0x9ea5c4u, 0x9ea620u); /* VA 0065041b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x100u, self, 9u, 8u, 0x7d1u, 0x9ea5a0u, 0x9ea60cu); /* VA 00650440 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x110u, self, 9u, 8u, 0x7d1u, 0x9ea5b0u, 0x9ea638u); /* VA 00650465 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x120u, self, 0x10u, 8u, 0x7d1u, 0x9ea638u, 0x9ea604u); /* VA 0065048a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x130u, self, 0x10u, 8u, 0x7d1u, 0x9ea620u, 0x9ea5a0u); /* VA 006504af */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x140u, self, 0x10u, 8u, 0x7d1u, 0x9ea60cu, 0x9ea5ecu); /* VA 006504d4 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x150u, self, 0x10u, 8u, 0x7d1u, 0x9ea5f8u, 0x9ea594u); /* VA 006504f9 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x160u, self, 0x10u, 8u, 0x7d1u, 0x9ea5e0u, 0x9ea614u); /* VA 0065051e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x170u, self, 0x10u, 8u, 0x7d1u, 0x9ea5d0u, 0x9ea62cu); /* VA 00650543 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x180u, self, 0x10u, 8u, 0x7d1u, 0x9ea5bcu, 0x9ea5d8u); /* VA 00650568 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x190u, self, 0x10u, 8u, 0x7d1u, 0x9ea5a8u, 0x9ea640u); /* VA 0065058d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1a0u, self, 0x10u, 8u, 0x7d1u, 0x9ea5b0u, 0x9ea5c4u); /* VA 006505b2 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1b0u, self, 0x17u, 8u, 0x7d1u, 0x9ea614u, 0x9ea5d0u); /* VA 006505d7 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1c0u, self, 0x17u, 8u, 0x7d1u, 0x9ea640u, 0x9ea620u); /* VA 006505fc */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1d0u, self, 0x17u, 8u, 0x7d1u, 0x9ea604u, 0x9ea5a8u); /* VA 00650621 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1e0u, self, 0x17u, 8u, 0x7d1u, 0x9ea5d8u, 0x9ea60cu); /* VA 00650646 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1f0u, self, 0x17u, 8u, 0x7d1u, 0x9ea62cu, 0x9ea5f8u); /* VA 0065066b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x200u, self, 0x17u, 8u, 0x7d1u, 0x9ea5ecu, 0x9ea638u); /* VA 00650690 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x210u, self, 0x17u, 8u, 0x7d1u, 0x9ea5c4u, 0x9ea5e0u); /* VA 006506b5 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x220u, self, 0x17u, 8u, 0x7d1u, 0x9ea5a0u, 0x9ea5b0u); /* VA 006506da */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x230u, self, 0x17u, 8u, 0x7d1u, 0x9ea594u, 0x9ea5bcu); /* VA 006506ff */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x240u, self, 0x1eu, 8u, 0x7d1u, 0x9ea638u, 0x9ea5a0u); /* VA 00650724 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x250u, self, 0x1eu, 8u, 0x7d1u, 0x9ea620u, 0x9ea5ecu); /* VA 00650749 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x260u, self, 0x1eu, 8u, 0x7d1u, 0x9ea640u, 0x9ea604u); /* VA 0065076e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x270u, self, 0x1eu, 8u, 0x7d1u, 0x9ea5f8u, 0x9ea5c4u); /* VA 00650793 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x280u, self, 0x1eu, 8u, 0x7d1u, 0x9ea5e0u, 0x9ea594u); /* VA 006507b8 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x290u, self, 0x1eu, 8u, 0x7d1u, 0x9ea5d0u, 0x9ea60cu); /* VA 006507dd */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x2a0u, self, 0x1eu, 8u, 0x7d1u, 0x9ea5bcu, 0x9ea614u); /* VA 00650802 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x2b0u, self, 0x1eu, 8u, 0x7d1u, 0x9ea5a8u, 0x9ea5d8u); /* VA 00650827 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x2c0u, self, 0x1eu, 8u, 0x7d1u, 0x9ea5b0u, 0x9ea62cu); /* VA 0065084c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x2d0u, self, 7u, 9u, 0x7d1u, 0x9ea620u, 0x9ea5bcu); /* VA 00650871 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x2e0u, self, 7u, 9u, 0x7d1u, 0x9ea614u, 0x9ea5f8u); /* VA 00650896 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x2f0u, self, 7u, 9u, 0x7d1u, 0x9ea60cu, 0x9ea5e0u); /* VA 006508bb */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x300u, self, 7u, 9u, 0x7d1u, 0x9ea604u, 0x9ea5c4u); /* VA 006508e0 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x310u, self, 7u, 9u, 0x7d1u, 0x9ea5d8u, 0x9ea5b0u); /* VA 00650905 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x320u, self, 7u, 9u, 0x7d1u, 0x9ea62cu, 0x9ea638u); /* VA 0065092a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x330u, self, 7u, 9u, 0x7d1u, 0x9ea5ecu, 0x9ea640u); /* VA 0065094f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x340u, self, 7u, 9u, 0x7d1u, 0x9ea5a0u, 0x9ea5a8u); /* VA 00650974 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x350u, self, 7u, 9u, 0x7d1u, 0x9ea594u, 0x9ea5d0u); /* VA 00650999 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x360u, self, 0xeu, 9u, 0x7d1u, 0x9ea638u, 0x9ea614u); /* VA 006509be */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x370u, self, 0xeu, 9u, 0x7d1u, 0x9ea640u, 0x9ea5a0u); /* VA 006509e3 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x380u, self, 0xeu, 9u, 0x7d1u, 0x9ea5f8u, 0x9ea5d8u); /* VA 00650a08 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x390u, self, 0xeu, 9u, 0x7d1u, 0x9ea5ecu, 0x9ea604u); /* VA 00650a2d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x3a0u, self, 0xeu, 9u, 0x7d1u, 0x9ea5e0u, 0x9ea5d0u); /* VA 00650a52 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x3b0u, self, 0xeu, 9u, 0x7d1u, 0x9ea5c4u, 0x9ea594u); /* VA 00650a77 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x3c0u, self, 0xeu, 9u, 0x7d1u, 0x9ea5bcu, 0x9ea60cu); /* VA 00650a9c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x3d0u, self, 0xeu, 9u, 0x7d1u, 0x9ea5a8u, 0x9ea62cu); /* VA 00650ac1 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x3e0u, self, 0xeu, 9u, 0x7d1u, 0x9ea5b0u, 0x9ea620u); /* VA 00650ae6 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x3f0u, self, 0x15u, 9u, 0x7d1u, 0x9ea620u, 0x9ea604u); /* VA 00650b0b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x400u, self, 0x15u, 9u, 0x7d1u, 0x9ea614u, 0x9ea5c4u); /* VA 00650b30 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x410u, self, 0x15u, 9u, 0x7d1u, 0x9ea60cu, 0x9ea5b0u); /* VA 00650b55 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x420u, self, 0x15u, 9u, 0x7d1u, 0x9ea5d8u, 0x9ea640u); /* VA 00650b7a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x430u, self, 0x15u, 9u, 0x7d1u, 0x9ea62cu, 0x9ea5e0u); /* VA 00650b9f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x440u, self, 0x15u, 9u, 0x7d1u, 0x9ea5d0u, 0x9ea5f8u); /* VA 00650bc4 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x450u, self, 0x15u, 9u, 0x7d1u, 0x9ea5a0u, 0x9ea5ecu); /* VA 00650be9 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x460u, self, 0x15u, 9u, 0x7d1u, 0x9ea5bcu, 0x9ea638u); /* VA 00650c0e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x470u, self, 0x15u, 9u, 0x7d1u, 0x9ea594u, 0x9ea5a8u); /* VA 00650c33 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x480u, self, 0x1cu, 9u, 0x7d1u, 0x9ea638u, 0x9ea620u); /* VA 00650c58 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x490u, self, 0x1cu, 9u, 0x7d1u, 0x9ea640u, 0x9ea5e0u); /* VA 00650c7d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x4a0u, self, 0x1cu, 9u, 0x7d1u, 0x9ea604u, 0x9ea5b0u); /* VA 00650ca2 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x4b0u, self, 0x1cu, 9u, 0x7d1u, 0x9ea5f8u, 0x9ea5bcu); /* VA 00650cc7 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x4c0u, self, 0x1cu, 9u, 0x7d1u, 0x9ea5ecu, 0x9ea62cu); /* VA 00650cec */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x4d0u, self, 0x1cu, 9u, 0x7d1u, 0x9ea5c4u, 0x9ea5d0u); /* VA 00650d11 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x4e0u, self, 0x1cu, 9u, 0x7d1u, 0x9ea5a0u, 0x9ea614u); /* VA 00650d36 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x4f0u, self, 0x1cu, 9u, 0x7d1u, 0x9ea5a8u, 0x9ea60cu); /* VA 00650d5b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x500u, self, 0x1cu, 9u, 0x7d1u, 0x9ea594u, 0x9ea5d8u); /* VA 00650d80 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x510u, self, 4u, 0xau, 0x7d1u, 0x9ea620u, 0x9ea5a8u); /* VA 00650da5 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x520u, self, 4u, 0xau, 0x7d1u, 0x9ea614u, 0x9ea640u); /* VA 00650dca */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x530u, self, 4u, 0xau, 0x7d1u, 0x9ea60cu, 0x9ea594u); /* VA 00650def */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x540u, self, 4u, 0xau, 0x7d1u, 0x9ea5d8u, 0x9ea5c4u); /* VA 00650e14 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x550u, self, 4u, 0xau, 0x7d1u, 0x9ea62cu, 0x9ea604u); /* VA 00650e39 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x560u, self, 4u, 0xau, 0x7d1u, 0x9ea5e0u, 0x9ea5f8u); /* VA 00650e5e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x570u, self, 4u, 0xau, 0x7d1u, 0x9ea5d0u, 0x9ea638u); /* VA 00650e83 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x580u, self, 4u, 0xau, 0x7d1u, 0x9ea5bcu, 0x9ea5a0u); /* VA 00650ea8 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x590u, self, 4u, 0xau, 0x7d1u, 0x9ea5b0u, 0x9ea5ecu); /* VA 00650ecd */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x5a0u, self, 0xbu, 0xau, 0x7d1u, 0x9ea638u, 0x9ea5a8u); /* VA 00650ef2 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x5b0u, self, 0xbu, 0xau, 0x7d1u, 0x9ea614u, 0x9ea5d8u); /* VA 00650f17 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x5c0u, self, 0xbu, 0xau, 0x7d1u, 0x9ea640u, 0x9ea5b0u); /* VA 00650f3c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x5d0u, self, 0xbu, 0xau, 0x7d1u, 0x9ea604u, 0x9ea5e0u); /* VA 00650f61 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x5e0u, self, 0xbu, 0xau, 0x7d1u, 0x9ea5f8u, 0x9ea60cu); /* VA 00650f86 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x5f0u, self, 0xbu, 0xau, 0x7d1u, 0x9ea5c4u, 0x9ea5ecu); /* VA 00650fab */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x600u, self, 0xbu, 0xau, 0x7d1u, 0x9ea5d0u, 0x9ea5bcu); /* VA 00650fd0 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x610u, self, 0xbu, 0xau, 0x7d1u, 0x9ea5a0u, 0x9ea62cu); /* VA 00650ff5 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x620u, self, 0xbu, 0xau, 0x7d1u, 0x9ea594u, 0x9ea620u); /* VA 0065101a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x630u, self, 0x12u, 0xau, 0x7d1u, 0x9ea620u, 0x9ea614u); /* VA 0065103f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x640u, self, 0x12u, 0xau, 0x7d1u, 0x9ea60cu, 0x9ea5c4u); /* VA 00651064 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x650u, self, 0x12u, 0xau, 0x7d1u, 0x9ea604u, 0x9ea5a0u); /* VA 00651089 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x660u, self, 0x12u, 0xau, 0x7d1u, 0x9ea5d8u, 0x9ea638u); /* VA 006510ae */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x670u, self, 0x12u, 0xau, 0x7d1u, 0x9ea62cu, 0x9ea640u); /* VA 006510d3 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x680u, self, 0x12u, 0xau, 0x7d1u, 0x9ea5ecu, 0x9ea5d0u); /* VA 006510f8 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x690u, self, 0x12u, 0xau, 0x7d1u, 0x9ea5e0u, 0x9ea5bcu); /* VA 0065111d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x6a0u, self, 0x12u, 0xau, 0x7d1u, 0x9ea5a8u, 0x9ea5f8u); /* VA 00651142 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x6b0u, self, 0x12u, 0xau, 0x7d1u, 0x9ea5b0u, 0x9ea594u); /* VA 00651167 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x6c0u, self, 0x19u, 0xau, 0x7d1u, 0x9ea614u, 0x9ea5a8u); /* VA 0065118c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x6d0u, self, 0x19u, 0xau, 0x7d1u, 0x9ea60cu, 0x9ea638u); /* VA 006511b1 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x6e0u, self, 0x19u, 0xau, 0x7d1u, 0x9ea5d8u, 0x9ea604u); /* VA 006511d6 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x6f0u, self, 0x19u, 0xau, 0x7d1u, 0x9ea5f8u, 0x9ea5a0u); /* VA 006511fb */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x700u, self, 0x19u, 0xau, 0x7d1u, 0x9ea5e0u, 0x9ea5b0u); /* VA 00651220 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x710u, self, 0x19u, 0xau, 0x7d1u, 0x9ea5c4u, 0x9ea62cu); /* VA 00651245 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x720u, self, 0x19u, 0xau, 0x7d1u, 0x9ea5d0u, 0x9ea620u); /* VA 0065126a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x730u, self, 0x19u, 0xau, 0x7d1u, 0x9ea5bcu, 0x9ea5ecu); /* VA 0065128f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x740u, self, 0x19u, 0xau, 0x7d1u, 0x9ea594u, 0x9ea640u); /* VA 006512b4 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x750u, self, 2u, 0xbu, 0x7d1u, 0x9ea638u, 0x9ea5c4u); /* VA 006512d9 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x760u, self, 2u, 0xbu, 0x7d1u, 0x9ea620u, 0x9ea5d8u); /* VA 006512fe */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x770u, self, 2u, 0xbu, 0x7d1u, 0x9ea640u, 0x9ea5bcu); /* VA 00651323 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x780u, self, 2u, 0xbu, 0x7d1u, 0x9ea604u, 0x9ea60cu); /* VA 00651348 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x790u, self, 2u, 0xbu, 0x7d1u, 0x9ea62cu, 0x9ea594u); /* VA 0065136d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x7a0u, self, 2u, 0xbu, 0x7d1u, 0x9ea5ecu, 0x9ea614u); /* VA 00651392 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x7b0u, self, 2u, 0xbu, 0x7d1u, 0x9ea5a0u, 0x9ea5e0u); /* VA 006513b7 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x7c0u, self, 2u, 0xbu, 0x7d1u, 0x9ea5a8u, 0x9ea5d0u); /* VA 006513dc */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x7d0u, self, 2u, 0xbu, 0x7d1u, 0x9ea5b0u, 0x9ea5f8u); /* VA 00651401 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x7e0u, self, 9u, 0xbu, 0x7d1u, 0x9ea614u, 0x9ea5b0u); /* VA 00651426 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x7f0u, self, 9u, 0xbu, 0x7d1u, 0x9ea60cu, 0x9ea62cu); /* VA 0065144b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x800u, self, 9u, 0xbu, 0x7d1u, 0x9ea5d8u, 0x9ea5a0u); /* VA 00651470 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x810u, self, 9u, 0xbu, 0x7d1u, 0x9ea5f8u, 0x9ea620u); /* VA 00651495 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x820u, self, 9u, 0xbu, 0x7d1u, 0x9ea5e0u, 0x9ea638u); /* VA 006514ba */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x830u, self, 9u, 0xbu, 0x7d1u, 0x9ea5c4u, 0x9ea5a8u); /* VA 006514df */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x840u, self, 9u, 0xbu, 0x7d1u, 0x9ea5d0u, 0x9ea640u); /* VA 00651504 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x850u, self, 9u, 0xbu, 0x7d1u, 0x9ea5bcu, 0x9ea604u); /* VA 00651529 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x860u, self, 9u, 0xbu, 0x7d1u, 0x9ea594u, 0x9ea5ecu); /* VA 0065154e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x870u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea638u, 0x9ea5f8u); /* VA 00651573 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x880u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea620u, 0x9ea5e0u); /* VA 00651598 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x890u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea640u, 0x9ea60cu); /* VA 006515bd */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x8a0u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea604u, 0x9ea594u); /* VA 006515e2 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x8b0u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea62cu, 0x9ea614u); /* VA 00651607 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x8c0u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea5ecu, 0x9ea5d8u); /* VA 0065162c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x8d0u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea5a0u, 0x9ea5c4u); /* VA 00651651 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x8e0u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea5a8u, 0x9ea5bcu); /* VA 00651676 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x8f0u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea5b0u, 0x9ea5d0u); /* VA 0065169b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x900u, self, 0x17u, 0xbu, 0x7d1u, 0x9ea614u, 0x9ea604u); /* VA 006516c0 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x910u, self, 0x17u, 0xbu, 0x7d1u, 0x9ea60cu, 0x9ea620u); /* VA 006516e5 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x920u, self, 0x17u, 0xbu, 0x7d1u, 0x9ea5d8u, 0x9ea62cu); /* VA 0065170a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x930u, self, 0x17u, 0xbu, 0x7d1u, 0x9ea5f8u, 0x9ea5ecu); /* VA 0065172f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x940u, self, 0x17u, 0xbu, 0x7d1u, 0x9ea5e0u, 0x9ea5a8u); /* VA 00651754 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x950u, self, 0x17u, 0xbu, 0x7d1u, 0x9ea5c4u, 0x9ea640u); /* VA 00651779 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x960u, self, 0x17u, 0xbu, 0x7d1u, 0x9ea5d0u, 0x9ea5a0u); /* VA 0065179e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x970u, self, 0x17u, 0xbu, 0x7d1u, 0x9ea5bcu, 0x9ea5b0u); /* VA 006517c3 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x980u, self, 0x17u, 0xbu, 0x7d1u, 0x9ea594u, 0x9ea638u); /* VA 006517e8 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x990u, self, 6u, 0u, 0x7d2u, 0x9ea640u, 0x9ea638u); /* VA 0065180d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x9a0u, self, 6u, 0u, 0x7d2u, 0x9ea62cu, 0x9ea620u); /* VA 00651832 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x9b0u, self, 6u, 0u, 0x7d2u, 0x9ea614u, 0x9ea60cu); /* VA 00651857 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x9c0u, self, 6u, 0u, 0x7d2u, 0x9ea604u, 0x9ea5f8u); /* VA 0065187c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x9d0u, self, 6u, 0u, 0x7d2u, 0x9ea5ecu, 0x9ea5e0u); /* VA 006518a1 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x9e0u, self, 6u, 0u, 0x7d2u, 0x9ea5d8u, 0x9ea5d0u); /* VA 006518c6 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x9f0u, self, 6u, 0u, 0x7d2u, 0x9ea5c4u, 0x9ea5bcu); /* VA 006518eb */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa00u, self, 6u, 0u, 0x7d2u, 0x9ea5b0u, 0x9ea5a8u); /* VA 00651910 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa10u, self, 6u, 0u, 0x7d2u, 0x9ea5a0u, 0x9ea594u); /* VA 00651935 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa20u, self, 0xdu, 0u, 0x7d2u, 0x9ea594u, 0x9ea614u); /* VA 0065195a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa30u, self, 0xdu, 0u, 0x7d2u, 0x9ea5f8u, 0x9ea640u); /* VA 0065197f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa40u, self, 0xdu, 0u, 0x7d2u, 0x9ea5d0u, 0x9ea604u); /* VA 006519a4 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa50u, self, 0xdu, 0u, 0x7d2u, 0x9ea5e0u, 0x9ea5d8u); /* VA 006519c9 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa60u, self, 0xdu, 0u, 0x7d2u, 0x9ea5bcu, 0x9ea62cu); /* VA 006519ee */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa70u, self, 0xdu, 0u, 0x7d2u, 0x9ea5a8u, 0x9ea5ecu); /* VA 00651a13 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa80u, self, 0xdu, 0u, 0x7d2u, 0x9ea620u, 0x9ea5c4u); /* VA 00651a38 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa90u, self, 0xdu, 0u, 0x7d2u, 0x9ea60cu, 0x9ea5a0u); /* VA 00651a5d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xaa0u, self, 0xdu, 0u, 0x7d2u, 0x9ea638u, 0x9ea5b0u); /* VA 00651a82 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xab0u, self, 0x14u, 0u, 0x7d2u, 0x9ea604u, 0x9ea638u); /* VA 00651aa7 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xac0u, self, 0x14u, 0u, 0x7d2u, 0x9ea5a0u, 0x9ea620u); /* VA 00651acc */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xad0u, self, 0x14u, 0u, 0x7d2u, 0x9ea5ecu, 0x9ea60cu); /* VA 00651af1 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xae0u, self, 0x14u, 0u, 0x7d2u, 0x9ea594u, 0x9ea5f8u); /* VA 00651b16 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xaf0u, self, 0x14u, 0u, 0x7d2u, 0x9ea614u, 0x9ea5e0u); /* VA 00651b3b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb00u, self, 0x14u, 0u, 0x7d2u, 0x9ea62cu, 0x9ea5d0u); /* VA 00651b60 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb10u, self, 0x14u, 0u, 0x7d2u, 0x9ea5d8u, 0x9ea5bcu); /* VA 00651b85 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb20u, self, 0x14u, 0u, 0x7d2u, 0x9ea640u, 0x9ea5a8u); /* VA 00651baa */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb30u, self, 0x14u, 0u, 0x7d2u, 0x9ea5c4u, 0x9ea5b0u); /* VA 00651bcf */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb40u, self, 0x1bu, 0u, 0x7d2u, 0x9ea5d0u, 0x9ea614u); /* VA 00651bf4 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb50u, self, 0x1bu, 0u, 0x7d2u, 0x9ea620u, 0x9ea640u); /* VA 00651c19 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb60u, self, 0x1bu, 0u, 0x7d2u, 0x9ea5a8u, 0x9ea604u); /* VA 00651c3e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb70u, self, 0x1bu, 0u, 0x7d2u, 0x9ea60cu, 0x9ea5d8u); /* VA 00651c63 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb80u, self, 0x1bu, 0u, 0x7d2u, 0x9ea5f8u, 0x9ea62cu); /* VA 00651c88 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb90u, self, 0x1bu, 0u, 0x7d2u, 0x9ea638u, 0x9ea5ecu); /* VA 00651cad */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xba0u, self, 0x1bu, 0u, 0x7d2u, 0x9ea5e0u, 0x9ea5c4u); /* VA 00651cd2 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xbb0u, self, 0x1bu, 0u, 0x7d2u, 0x9ea5b0u, 0x9ea5a0u); /* VA 00651cf7 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xbc0u, self, 0x1bu, 0u, 0x7d2u, 0x9ea5bcu, 0x9ea594u); /* VA 00651d1c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xbd0u, self, 3u, 1u, 0x7d2u, 0x9ea5a0u, 0x9ea638u); /* VA 00651d41 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xbe0u, self, 3u, 1u, 0x7d2u, 0x9ea5ecu, 0x9ea620u); /* VA 00651d66 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xbf0u, self, 3u, 1u, 0x7d2u, 0x9ea604u, 0x9ea640u); /* VA 00651d8b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc00u, self, 3u, 1u, 0x7d2u, 0x9ea5c4u, 0x9ea5f8u); /* VA 00651db0 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc10u, self, 3u, 1u, 0x7d2u, 0x9ea594u, 0x9ea5e0u); /* VA 00651dd5 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc20u, self, 3u, 1u, 0x7d2u, 0x9ea60cu, 0x9ea5d0u); /* VA 00651dfa */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc30u, self, 3u, 1u, 0x7d2u, 0x9ea614u, 0x9ea5bcu); /* VA 00651e1f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc40u, self, 3u, 1u, 0x7d2u, 0x9ea5d8u, 0x9ea5a8u); /* VA 00651e44 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc50u, self, 3u, 1u, 0x7d2u, 0x9ea62cu, 0x9ea5b0u); /* VA 00651e69 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc60u, self, 0xau, 1u, 0x7d2u, 0x9ea5bcu, 0x9ea620u); /* VA 00651e8e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc70u, self, 0xau, 1u, 0x7d2u, 0x9ea5f8u, 0x9ea614u); /* VA 00651eb3 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc80u, self, 0xau, 1u, 0x7d2u, 0x9ea5e0u, 0x9ea60cu); /* VA 00651ed8 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc90u, self, 0xau, 1u, 0x7d2u, 0x9ea5c4u, 0x9ea604u); /* VA 00651efd */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xca0u, self, 0xau, 1u, 0x7d2u, 0x9ea5b0u, 0x9ea5d8u); /* VA 00651f22 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xcb0u, self, 0xau, 1u, 0x7d2u, 0x9ea638u, 0x9ea62cu); /* VA 00651f47 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xcc0u, self, 0xau, 1u, 0x7d2u, 0x9ea640u, 0x9ea5ecu); /* VA 00651f6c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xcd0u, self, 0xau, 1u, 0x7d2u, 0x9ea5a8u, 0x9ea5a0u); /* VA 00651f91 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xce0u, self, 0xau, 1u, 0x7d2u, 0x9ea5d0u, 0x9ea594u); /* VA 00651fb6 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xcf0u, self, 0x11u, 1u, 0x7d2u, 0x9ea614u, 0x9ea638u); /* VA 00651fdb */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd00u, self, 0x11u, 1u, 0x7d2u, 0x9ea5a0u, 0x9ea640u); /* VA 00652000 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd10u, self, 0x11u, 1u, 0x7d2u, 0x9ea5d8u, 0x9ea5f8u); /* VA 00652025 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd20u, self, 0x11u, 1u, 0x7d2u, 0x9ea604u, 0x9ea5ecu); /* VA 0065204a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd30u, self, 0x11u, 1u, 0x7d2u, 0x9ea5d0u, 0x9ea5e0u); /* VA 0065206f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd40u, self, 0x11u, 1u, 0x7d2u, 0x9ea594u, 0x9ea5c4u); /* VA 00652094 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd50u, self, 0x11u, 1u, 0x7d2u, 0x9ea60cu, 0x9ea5bcu); /* VA 006520b9 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd60u, self, 0x11u, 1u, 0x7d2u, 0x9ea62cu, 0x9ea5a8u); /* VA 006520de */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd70u, self, 0x11u, 1u, 0x7d2u, 0x9ea620u, 0x9ea5b0u); /* VA 00652103 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd80u, self, 3u, 2u, 0x7d2u, 0x9ea604u, 0x9ea620u); /* VA 00652128 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd90u, self, 3u, 2u, 0x7d2u, 0x9ea5c4u, 0x9ea614u); /* VA 0065214d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xda0u, self, 3u, 2u, 0x7d2u, 0x9ea5b0u, 0x9ea60cu); /* VA 00652172 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xdb0u, self, 3u, 2u, 0x7d2u, 0x9ea640u, 0x9ea5d8u); /* VA 00652197 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xdc0u, self, 3u, 2u, 0x7d2u, 0x9ea5e0u, 0x9ea62cu); /* VA 006521bc */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xdd0u, self, 3u, 2u, 0x7d2u, 0x9ea5f8u, 0x9ea5d0u); /* VA 006521e1 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xde0u, self, 3u, 2u, 0x7d2u, 0x9ea5ecu, 0x9ea5a0u); /* VA 00652206 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xdf0u, self, 3u, 2u, 0x7d2u, 0x9ea638u, 0x9ea5bcu); /* VA 0065222b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe00u, self, 3u, 2u, 0x7d2u, 0x9ea5a8u, 0x9ea594u); /* VA 00652250 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe10u, self, 0xau, 2u, 0x7d2u, 0x9ea620u, 0x9ea638u); /* VA 00652275 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe20u, self, 0xau, 2u, 0x7d2u, 0x9ea5e0u, 0x9ea640u); /* VA 0065229a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe30u, self, 0xau, 2u, 0x7d2u, 0x9ea5b0u, 0x9ea604u); /* VA 006522bf */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe40u, self, 0xau, 2u, 0x7d2u, 0x9ea5bcu, 0x9ea5f8u); /* VA 006522e4 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe50u, self, 0xau, 2u, 0x7d2u, 0x9ea62cu, 0x9ea5ecu); /* VA 00652309 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe60u, self, 0xau, 2u, 0x7d2u, 0x9ea5d0u, 0x9ea5c4u); /* VA 0065232e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe70u, self, 0xau, 2u, 0x7d2u, 0x9ea614u, 0x9ea5a0u); /* VA 00652353 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe80u, self, 0xau, 2u, 0x7d2u, 0x9ea60cu, 0x9ea5a8u); /* VA 00652378 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe90u, self, 0xau, 2u, 0x7d2u, 0x9ea5d8u, 0x9ea594u); /* VA 0065239d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xea0u, self, 0x11u, 2u, 0x7d2u, 0x9ea5a8u, 0x9ea620u); /* VA 006523c2 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xeb0u, self, 0x11u, 2u, 0x7d2u, 0x9ea640u, 0x9ea614u); /* VA 006523e7 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xec0u, self, 0x11u, 2u, 0x7d2u, 0x9ea594u, 0x9ea60cu); /* VA 0065240c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xed0u, self, 0x11u, 2u, 0x7d2u, 0x9ea5c4u, 0x9ea5d8u); /* VA 00652431 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xee0u, self, 0x11u, 2u, 0x7d2u, 0x9ea604u, 0x9ea62cu); /* VA 00652456 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xef0u, self, 0x11u, 2u, 0x7d2u, 0x9ea5f8u, 0x9ea5e0u); /* VA 0065247b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf00u, self, 0x11u, 2u, 0x7d2u, 0x9ea638u, 0x9ea5d0u); /* VA 006524a0 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf10u, self, 0x11u, 2u, 0x7d2u, 0x9ea5a0u, 0x9ea5bcu); /* VA 006524c5 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf20u, self, 0x11u, 2u, 0x7d2u, 0x9ea5ecu, 0x9ea5b0u); /* VA 006524ea */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf30u, self, 0x18u, 2u, 0x7d2u, 0x9ea5a8u, 0x9ea638u); /* VA 0065250f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf40u, self, 0x18u, 2u, 0x7d2u, 0x9ea5d8u, 0x9ea614u); /* VA 00652534 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf50u, self, 0x18u, 2u, 0x7d2u, 0x9ea5b0u, 0x9ea640u); /* VA 00652559 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf60u, self, 0x18u, 2u, 0x7d2u, 0x9ea5e0u, 0x9ea604u); /* VA 0065257e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf70u, self, 0x18u, 2u, 0x7d2u, 0x9ea60cu, 0x9ea5f8u); /* VA 006525a3 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf80u, self, 0x18u, 2u, 0x7d2u, 0x9ea5ecu, 0x9ea5c4u); /* VA 006525c8 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf90u, self, 0x18u, 2u, 0x7d2u, 0x9ea5bcu, 0x9ea5d0u); /* VA 006525ed */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xfa0u, self, 0x18u, 2u, 0x7d2u, 0x9ea62cu, 0x9ea5a0u); /* VA 00652612 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xfb0u, self, 0x18u, 2u, 0x7d2u, 0x9ea620u, 0x9ea594u); /* VA 00652637 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xfc0u, self, 0x1eu, 2u, 0x7d2u, 0x9ea614u, 0x9ea620u); /* VA 0065265c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xfd0u, self, 0x1eu, 2u, 0x7d2u, 0x9ea5c4u, 0x9ea60cu); /* VA 00652681 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xfe0u, self, 0x1eu, 2u, 0x7d2u, 0x9ea5a0u, 0x9ea604u); /* VA 006526a6 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xff0u, self, 0x1eu, 2u, 0x7d2u, 0x9ea638u, 0x9ea5d8u); /* VA 006526cb */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1000u, self, 0x1eu, 2u, 0x7d2u, 0x9ea640u, 0x9ea62cu); /* VA 006526f0 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1010u, self, 0x1eu, 2u, 0x7d2u, 0x9ea5d0u, 0x9ea5ecu); /* VA 00652715 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1020u, self, 0x1eu, 2u, 0x7d2u, 0x9ea5bcu, 0x9ea5e0u); /* VA 0065273a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1030u, self, 0x1eu, 2u, 0x7d2u, 0x9ea5f8u, 0x9ea5a8u); /* VA 0065275f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1040u, self, 0x1eu, 2u, 0x7d2u, 0x9ea594u, 0x9ea5b0u); /* VA 00652784 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1050u, self, 7u, 3u, 0x7d2u, 0x9ea5a8u, 0x9ea614u); /* VA 006527a9 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1060u, self, 7u, 3u, 0x7d2u, 0x9ea638u, 0x9ea60cu); /* VA 006527ce */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1070u, self, 7u, 3u, 0x7d2u, 0x9ea604u, 0x9ea5d8u); /* VA 006527f3 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1080u, self, 7u, 3u, 0x7d2u, 0x9ea5a0u, 0x9ea5f8u); /* VA 00652818 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1090u, self, 7u, 3u, 0x7d2u, 0x9ea5b0u, 0x9ea5e0u); /* VA 0065283d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x10a0u, self, 7u, 3u, 0x7d2u, 0x9ea62cu, 0x9ea5c4u); /* VA 00652862 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x10b0u, self, 7u, 3u, 0x7d2u, 0x9ea620u, 0x9ea5d0u); /* VA 00652887 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x10c0u, self, 7u, 3u, 0x7d2u, 0x9ea5ecu, 0x9ea5bcu); /* VA 006528ac */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x10d0u, self, 7u, 3u, 0x7d2u, 0x9ea640u, 0x9ea594u); /* VA 006528d1 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x10e0u, self, 0xeu, 3u, 0x7d2u, 0x9ea5c4u, 0x9ea638u); /* VA 006528f6 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x10f0u, self, 0xeu, 3u, 0x7d2u, 0x9ea5d8u, 0x9ea620u); /* VA 0065291b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1100u, self, 0xeu, 3u, 0x7d2u, 0x9ea5bcu, 0x9ea640u); /* VA 00652940 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1110u, self, 0xeu, 3u, 0x7d2u, 0x9ea60cu, 0x9ea604u); /* VA 00652965 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1120u, self, 0xeu, 3u, 0x7d2u, 0x9ea594u, 0x9ea62cu); /* VA 0065298a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1130u, self, 0xeu, 3u, 0x7d2u, 0x9ea614u, 0x9ea5ecu); /* VA 006529af */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1140u, self, 0xeu, 3u, 0x7d2u, 0x9ea5e0u, 0x9ea5a0u); /* VA 006529d4 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1150u, self, 0xeu, 3u, 0x7d2u, 0x9ea5d0u, 0x9ea5a8u); /* VA 006529f9 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1160u, self, 0xeu, 3u, 0x7d2u, 0x9ea5f8u, 0x9ea5b0u); /* VA 00652a1e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1170u, self, 0x15u, 3u, 0x7d2u, 0x9ea5b0u, 0x9ea614u); /* VA 00652a43 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1180u, self, 0x15u, 3u, 0x7d2u, 0x9ea62cu, 0x9ea60cu); /* VA 00652a68 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1190u, self, 0x15u, 3u, 0x7d2u, 0x9ea5a0u, 0x9ea5d8u); /* VA 00652a8d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x11a0u, self, 0x15u, 3u, 0x7d2u, 0x9ea620u, 0x9ea5f8u); /* VA 00652ab2 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x11b0u, self, 0x15u, 3u, 0x7d2u, 0x9ea638u, 0x9ea5e0u); /* VA 00652ad7 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x11c0u, self, 0x15u, 3u, 0x7d2u, 0x9ea5a8u, 0x9ea5c4u); /* VA 00652afc */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x11d0u, self, 0x15u, 3u, 0x7d2u, 0x9ea640u, 0x9ea5d0u); /* VA 00652b21 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x11e0u, self, 0x15u, 3u, 0x7d2u, 0x9ea604u, 0x9ea5bcu); /* VA 00652b46 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x11f0u, self, 0x15u, 3u, 0x7d2u, 0x9ea5ecu, 0x9ea594u); /* VA 00652b6b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1200u, self, 0x1cu, 3u, 0x7d2u, 0x9ea5f8u, 0x9ea638u); /* VA 00652b90 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1210u, self, 0x1cu, 3u, 0x7d2u, 0x9ea5e0u, 0x9ea620u); /* VA 00652bb5 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1220u, self, 0x1cu, 3u, 0x7d2u, 0x9ea60cu, 0x9ea640u); /* VA 00652bda */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1230u, self, 0x1cu, 3u, 0x7d2u, 0x9ea594u, 0x9ea604u); /* VA 00652bff */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1240u, self, 0x1cu, 3u, 0x7d2u, 0x9ea614u, 0x9ea62cu); /* VA 00652c24 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1250u, self, 0x1cu, 3u, 0x7d2u, 0x9ea5d8u, 0x9ea5ecu); /* VA 00652c49 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1260u, self, 0x1cu, 3u, 0x7d2u, 0x9ea5c4u, 0x9ea5a0u); /* VA 00652c6e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1270u, self, 0x1cu, 3u, 0x7d2u, 0x9ea5bcu, 0x9ea5a8u); /* VA 00652c93 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1280u, self, 0x1cu, 3u, 0x7d2u, 0x9ea5d0u, 0x9ea5b0u); /* VA 00652cb8 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1290u, self, 5u, 4u, 0x7d2u, 0x9ea604u, 0x9ea614u); /* VA 00652cdd */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x12a0u, self, 5u, 4u, 0x7d2u, 0x9ea620u, 0x9ea60cu); /* VA 00652d02 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x12b0u, self, 5u, 4u, 0x7d2u, 0x9ea62cu, 0x9ea5d8u); /* VA 00652d27 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x12c0u, self, 5u, 4u, 0x7d2u, 0x9ea5ecu, 0x9ea5f8u); /* VA 00652d4c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x12d0u, self, 5u, 4u, 0x7d2u, 0x9ea5a8u, 0x9ea5e0u); /* VA 00652d71 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x12e0u, self, 5u, 4u, 0x7d2u, 0x9ea640u, 0x9ea5c4u); /* VA 00652d96 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x12f0u, self, 5u, 4u, 0x7d2u, 0x9ea5a0u, 0x9ea5d0u); /* VA 00652dbb */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1300u, self, 5u, 4u, 0x7d2u, 0x9ea5b0u, 0x9ea5bcu); /* VA 00652de0 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1310u, self, 5u, 4u, 0x7d2u, 0x9ea638u, 0x9ea594u); /* VA 00652e05 */
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

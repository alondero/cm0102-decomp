/* Preferred-image VA 0x00658750, base 0x00400000.
   CM0102/Game/cm0102.exe SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Every schedule call below preserves the original immediate pushes, receiver
   displacement and call order; each following receiver is a fresh DWORD read.
   Direct caller VA 0x00657446 discards EAX before its next use. */
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

void CM_THISCALL cm_fn_00658750(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x204];
    s32 half, factor, count;
    u32 allocation, receiver, table, cursor, index;

    frame.state = 0xffffffffu;
    frame.handler = 0x95bceeu;
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
        cm_fn_00944cff(0x9ea75cu, 0, 0, scratch + 0x104, scratch + 4);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text,
                       scratch + 0x104, scratch + 4, 0x3d3u);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0;
    } else {
    cm_fn_0068aff0(receiver + 0x0u, self, 2u, 8u, 0x7d1u, 0x9ea82cu, 0x9ea834u); /* VA 00658855 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x10u, self, 2u, 8u, 0x7d1u, 0x9ea818u, 0x9ea820u); /* VA 00658877 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x20u, self, 2u, 8u, 0x7d1u, 0x9ea804u, 0x9ea80cu); /* VA 00658899 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x30u, self, 2u, 8u, 0x7d1u, 0x9ea7fcu, 0x9d9860u); /* VA 006588bb */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x40u, self, 2u, 8u, 0x7d1u, 0x9ea7ecu, 0x9ea7f4u); /* VA 006588dd */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x50u, self, 2u, 8u, 0x7d1u, 0x9ea7d4u, 0x9ea7e4u); /* VA 006588ff */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x60u, self, 2u, 8u, 0x7d1u, 0x9ea7bcu, 0x9ea7c8u); /* VA 00658921 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x70u, self, 2u, 8u, 0x7d1u, 0x9ea7a8u, 0x9ea7b4u); /* VA 00658943 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x80u, self, 2u, 8u, 0x7d1u, 0x9ea798u, 0x9ea7a0u); /* VA 00658968 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x90u, self, 9u, 8u, 0x7d1u, 0x9ea80cu, 0x9ea7d4u); /* VA 0065898d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa0u, self, 9u, 8u, 0x7d1u, 0x9ea7a0u, 0x9ea804u); /* VA 006589b2 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb0u, self, 9u, 8u, 0x7d1u, 0x9ea820u, 0x9ea7ecu); /* VA 006589d7 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc0u, self, 9u, 8u, 0x7d1u, 0x9ea7f4u, 0x9ea82cu); /* VA 006589fc */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd0u, self, 9u, 8u, 0x7d1u, 0x9ea7c8u, 0x9ea7fcu); /* VA 00658a21 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe0u, self, 9u, 8u, 0x7d1u, 0x9ea7b4u, 0x9ea818u); /* VA 00658a46 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf0u, self, 9u, 8u, 0x7d1u, 0x9d9860u, 0x9ea7bcu); /* VA 00658a6b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x100u, self, 9u, 8u, 0x7d1u, 0x9ea7e4u, 0x9ea798u); /* VA 00658a90 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x110u, self, 9u, 8u, 0x7d1u, 0x9ea834u, 0x9ea7a8u); /* VA 00658ab5 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x120u, self, 0x10u, 8u, 0x7d1u, 0x9ea82cu, 0x9ea7a0u); /* VA 00658ada */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x130u, self, 0x10u, 8u, 0x7d1u, 0x9ea818u, 0x9ea7ecu); /* VA 00658aff */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x140u, self, 0x10u, 8u, 0x7d1u, 0x9ea804u, 0x9ea7e4u); /* VA 00658b24 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x150u, self, 0x10u, 8u, 0x7d1u, 0x9ea7fcu, 0x9ea80cu); /* VA 00658b49 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x160u, self, 0x10u, 8u, 0x7d1u, 0x9ea7d4u, 0x9ea7b4u); /* VA 00658b6e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x170u, self, 0x10u, 8u, 0x7d1u, 0x9d9860u, 0x9ea7f4u); /* VA 00658b93 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x180u, self, 0x10u, 8u, 0x7d1u, 0x9ea7bcu, 0x9ea820u); /* VA 00658bb8 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x190u, self, 0x10u, 8u, 0x7d1u, 0x9ea7a8u, 0x9ea7c8u); /* VA 00658bdd */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1a0u, self, 0x10u, 8u, 0x7d1u, 0x9ea798u, 0x9ea834u); /* VA 00658c02 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1b0u, self, 0x17u, 8u, 0x7d1u, 0x9ea80cu, 0x9ea798u); /* VA 00658c27 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1c0u, self, 0x17u, 8u, 0x7d1u, 0x9ea7a0u, 0x9d9860u); /* VA 00658c4c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1d0u, self, 0x17u, 8u, 0x7d1u, 0x9ea820u, 0x9ea7a8u); /* VA 00658c71 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1e0u, self, 0x17u, 8u, 0x7d1u, 0x9ea7f4u, 0x9ea7d4u); /* VA 00658c96 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1f0u, self, 0x17u, 8u, 0x7d1u, 0x9ea7c8u, 0x9ea804u); /* VA 00658cbb */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x200u, self, 0x17u, 8u, 0x7d1u, 0x9ea7b4u, 0x9ea82cu); /* VA 00658ce0 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x210u, self, 0x17u, 8u, 0x7d1u, 0x9ea7ecu, 0x9ea7bcu); /* VA 00658d05 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x220u, self, 0x17u, 8u, 0x7d1u, 0x9ea7e4u, 0x9ea7fcu); /* VA 00658d2a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x230u, self, 0x17u, 8u, 0x7d1u, 0x9ea834u, 0x9ea818u); /* VA 00658d4f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x240u, self, 0x1eu, 8u, 0x7d1u, 0x9ea7a0u, 0x9ea7f4u); /* VA 00658d74 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x250u, self, 0x1eu, 8u, 0x7d1u, 0x9ea820u, 0x9ea7e4u); /* VA 00658d99 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x260u, self, 0x1eu, 8u, 0x7d1u, 0x9ea82cu, 0x9ea7c8u); /* VA 00658dbe */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x270u, self, 0x1eu, 8u, 0x7d1u, 0x9ea7b4u, 0x9ea80cu); /* VA 00658de3 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x280u, self, 0x1eu, 8u, 0x7d1u, 0x9d9860u, 0x9ea7d4u); /* VA 00658e08 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x290u, self, 0x1eu, 8u, 0x7d1u, 0x9ea7bcu, 0x9ea818u); /* VA 00658e2d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x2a0u, self, 0x1eu, 8u, 0x7d1u, 0x9ea7a8u, 0x9ea7ecu); /* VA 00658e52 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x2b0u, self, 0x1eu, 8u, 0x7d1u, 0x9ea798u, 0x9ea804u); /* VA 00658e77 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x2c0u, self, 0x1eu, 8u, 0x7d1u, 0x9ea834u, 0x9ea7fcu); /* VA 00658e9c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x2d0u, self, 7u, 9u, 0x7d1u, 0x9ea80cu, 0x9ea7a0u); /* VA 00658ec1 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x2e0u, self, 7u, 9u, 0x7d1u, 0x9ea7f4u, 0x9ea834u); /* VA 00658ee6 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x2f0u, self, 7u, 9u, 0x7d1u, 0x9ea818u, 0x9ea7a8u); /* VA 00658f0b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x300u, self, 7u, 9u, 0x7d1u, 0x9ea804u, 0x9d9860u); /* VA 00658f30 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x310u, self, 7u, 9u, 0x7d1u, 0x9ea7fcu, 0x9ea82cu); /* VA 00658f55 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x320u, self, 7u, 9u, 0x7d1u, 0x9ea7c8u, 0x9ea820u); /* VA 00658f7a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x330u, self, 7u, 9u, 0x7d1u, 0x9ea7ecu, 0x9ea7b4u); /* VA 00658f9f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x340u, self, 7u, 9u, 0x7d1u, 0x9ea7d4u, 0x9ea798u); /* VA 00658fc4 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x350u, self, 7u, 9u, 0x7d1u, 0x9ea7e4u, 0x9ea7bcu); /* VA 00658fe9 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x360u, self, 0xeu, 9u, 0x7d1u, 0x9ea7a0u, 0x9ea7d4u); /* VA 0065900e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x370u, self, 0xeu, 9u, 0x7d1u, 0x9ea820u, 0x9ea804u); /* VA 00659033 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x380u, self, 0xeu, 9u, 0x7d1u, 0x9ea7f4u, 0x9ea7fcu); /* VA 00659058 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x390u, self, 0xeu, 9u, 0x7d1u, 0x9ea82cu, 0x9ea7ecu); /* VA 0065907d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x3a0u, self, 0xeu, 9u, 0x7d1u, 0x9ea7b4u, 0x9ea7bcu); /* VA 006590a2 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x3b0u, self, 0xeu, 9u, 0x7d1u, 0x9d9860u, 0x9ea80cu); /* VA 006590c7 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x3c0u, self, 0xeu, 9u, 0x7d1u, 0x9ea7a8u, 0x9ea7e4u); /* VA 006590ec */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x3d0u, self, 0xeu, 9u, 0x7d1u, 0x9ea798u, 0x9ea818u); /* VA 00659111 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x3e0u, self, 0xeu, 9u, 0x7d1u, 0x9ea834u, 0x9ea7c8u); /* VA 00659136 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x3f0u, self, 0x15u, 9u, 0x7d1u, 0x9ea80cu, 0x9ea7c8u); /* VA 0065915b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x400u, self, 0x15u, 9u, 0x7d1u, 0x9ea818u, 0x9ea82cu); /* VA 00659180 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x410u, self, 0x15u, 9u, 0x7d1u, 0x9ea804u, 0x9ea7f4u); /* VA 006591a5 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x420u, self, 0x15u, 9u, 0x7d1u, 0x9ea7fcu, 0x9ea7a0u); /* VA 006591ca */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x430u, self, 0x15u, 9u, 0x7d1u, 0x9ea7ecu, 0x9d9860u); /* VA 006591ef */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x440u, self, 0x15u, 9u, 0x7d1u, 0x9ea7d4u, 0x9ea7a8u); /* VA 00659214 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x450u, self, 0x15u, 9u, 0x7d1u, 0x9ea7bcu, 0x9ea834u); /* VA 00659239 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x460u, self, 0x15u, 9u, 0x7d1u, 0x9ea7e4u, 0x9ea7b4u); /* VA 0065925e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x470u, self, 0x15u, 9u, 0x7d1u, 0x9ea798u, 0x9ea820u); /* VA 00659283 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x480u, self, 0x1cu, 9u, 0x7d1u, 0x9ea7a0u, 0x9ea7ecu); /* VA 006592a8 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x490u, self, 0x1cu, 9u, 0x7d1u, 0x9ea7f4u, 0x9ea80cu); /* VA 006592cd */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x4a0u, self, 0x1cu, 9u, 0x7d1u, 0x9ea82cu, 0x9ea7bcu); /* VA 006592f2 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x4b0u, self, 0x1cu, 9u, 0x7d1u, 0x9ea7c8u, 0x9ea7e4u); /* VA 00659317 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x4c0u, self, 0x1cu, 9u, 0x7d1u, 0x9ea7b4u, 0x9ea804u); /* VA 0065933c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x4d0u, self, 0x1cu, 9u, 0x7d1u, 0x9ea7d4u, 0x9ea818u); /* VA 00659361 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x4e0u, self, 0x1cu, 9u, 0x7d1u, 0x9d9860u, 0x9ea798u); /* VA 00659386 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x4f0u, self, 0x1cu, 9u, 0x7d1u, 0x9ea7a8u, 0x9ea7fcu); /* VA 006593ab */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x500u, self, 0x1cu, 9u, 0x7d1u, 0x9ea834u, 0x9ea820u); /* VA 006593d0 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x510u, self, 4u, 0xau, 0x7d1u, 0x9ea80cu, 0x9ea7a8u); /* VA 006593f5 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x520u, self, 4u, 0xau, 0x7d1u, 0x9ea820u, 0x9ea7b4u); /* VA 0065941a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x530u, self, 4u, 0xau, 0x7d1u, 0x9ea818u, 0x9ea7c8u); /* VA 0065943f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x540u, self, 4u, 0xau, 0x7d1u, 0x9ea804u, 0x9ea82cu); /* VA 00659464 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x550u, self, 4u, 0xau, 0x7d1u, 0x9ea7fcu, 0x9ea7d4u); /* VA 00659489 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x560u, self, 4u, 0xau, 0x7d1u, 0x9ea7ecu, 0x9ea834u); /* VA 006594ae */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x570u, self, 4u, 0xau, 0x7d1u, 0x9ea7bcu, 0x9ea7a0u); /* VA 006594d3 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x580u, self, 4u, 0xau, 0x7d1u, 0x9ea7e4u, 0x9d9860u); /* VA 006594f8 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x590u, self, 4u, 0xau, 0x7d1u, 0x9ea798u, 0x9ea7f4u); /* VA 0065951d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x5a0u, self, 0xbu, 0xau, 0x7d1u, 0x9ea7a0u, 0x9ea7e4u); /* VA 00659542 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x5b0u, self, 0xbu, 0xau, 0x7d1u, 0x9ea7f4u, 0x9ea820u); /* VA 00659567 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x5c0u, self, 0xbu, 0xau, 0x7d1u, 0x9ea82cu, 0x9ea80cu); /* VA 0065958c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x5d0u, self, 0xbu, 0xau, 0x7d1u, 0x9ea7fcu, 0x9ea7ecu); /* VA 006595b1 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x5e0u, self, 0xbu, 0xau, 0x7d1u, 0x9ea7c8u, 0x9ea798u); /* VA 006595d6 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x5f0u, self, 0xbu, 0xau, 0x7d1u, 0x9ea7b4u, 0x9ea834u); /* VA 006595fb */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x600u, self, 0xbu, 0xau, 0x7d1u, 0x9ea7d4u, 0x9ea804u); /* VA 00659620 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x610u, self, 0xbu, 0xau, 0x7d1u, 0x9d9860u, 0x9ea818u); /* VA 00659645 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x620u, self, 0xbu, 0xau, 0x7d1u, 0x9ea7a8u, 0x9ea7bcu); /* VA 0065966a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x630u, self, 0x12u, 0xau, 0x7d1u, 0x9ea820u, 0x9ea7d4u); /* VA 0065968f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x640u, self, 0x12u, 0xau, 0x7d1u, 0x9ea818u, 0x9ea7a0u); /* VA 006596b4 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x650u, self, 0x12u, 0xau, 0x7d1u, 0x9ea804u, 0x9ea7fcu); /* VA 006596d9 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x660u, self, 0x12u, 0xau, 0x7d1u, 0x9ea7ecu, 0x9ea7c8u); /* VA 006596fe */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x670u, self, 0x12u, 0xau, 0x7d1u, 0x9ea7bcu, 0x9ea7f4u); /* VA 00659723 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x680u, self, 0x12u, 0xau, 0x7d1u, 0x9ea7a8u, 0x9ea82cu); /* VA 00659748 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x690u, self, 0x12u, 0xau, 0x7d1u, 0x9ea7e4u, 0x9ea80cu); /* VA 0065976d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x6a0u, self, 0x12u, 0xau, 0x7d1u, 0x9ea798u, 0x9ea7b4u); /* VA 00659792 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x6b0u, self, 0x12u, 0xau, 0x7d1u, 0x9ea834u, 0x9d9860u); /* VA 006597b7 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x6c0u, self, 0x19u, 0xau, 0x7d1u, 0x9ea80cu, 0x9ea7bcu); /* VA 006597dc */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x6d0u, self, 0x19u, 0xau, 0x7d1u, 0x9ea7a0u, 0x9ea834u); /* VA 00659801 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x6e0u, self, 0x19u, 0xau, 0x7d1u, 0x9ea7f4u, 0x9ea7e4u); /* VA 00659826 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x6f0u, self, 0x19u, 0xau, 0x7d1u, 0x9ea82cu, 0x9ea820u); /* VA 0065984b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x700u, self, 0x19u, 0xau, 0x7d1u, 0x9ea804u, 0x9ea818u); /* VA 00659870 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x710u, self, 0x19u, 0xau, 0x7d1u, 0x9ea7fcu, 0x9ea798u); /* VA 00659895 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x720u, self, 0x19u, 0xau, 0x7d1u, 0x9ea7c8u, 0x9ea7b4u); /* VA 006598ba */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x730u, self, 0x19u, 0xau, 0x7d1u, 0x9ea7d4u, 0x9ea7ecu); /* VA 006598df */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x740u, self, 0x19u, 0xau, 0x7d1u, 0x9d9860u, 0x9ea7a8u); /* VA 00659904 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x750u, self, 2u, 0xbu, 0x7d1u, 0x9ea820u, 0x9ea80cu); /* VA 00659929 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x760u, self, 2u, 0xbu, 0x7d1u, 0x9ea82cu, 0x9ea7e4u); /* VA 0065994e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x770u, self, 2u, 0xbu, 0x7d1u, 0x9ea818u, 0x9ea7fcu); /* VA 00659973 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x780u, self, 2u, 0xbu, 0x7d1u, 0x9ea7c8u, 0x9ea7a0u); /* VA 00659998 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x790u, self, 2u, 0xbu, 0x7d1u, 0x9ea7b4u, 0x9d9860u); /* VA 006599bd */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x7a0u, self, 2u, 0xbu, 0x7d1u, 0x9ea7ecu, 0x9ea798u); /* VA 006599e2 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x7b0u, self, 2u, 0xbu, 0x7d1u, 0x9ea7bcu, 0x9ea804u); /* VA 00659a07 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x7c0u, self, 2u, 0xbu, 0x7d1u, 0x9ea7a8u, 0x9ea7f4u); /* VA 00659a2c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x7d0u, self, 2u, 0xbu, 0x7d1u, 0x9ea834u, 0x9ea7d4u); /* VA 00659a51 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x7e0u, self, 9u, 0xbu, 0x7d1u, 0x9ea80cu, 0x9ea834u); /* VA 00659a76 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x7f0u, self, 9u, 0xbu, 0x7d1u, 0x9ea7a0u, 0x9ea7a8u); /* VA 00659a9b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x800u, self, 9u, 0xbu, 0x7d1u, 0x9ea7f4u, 0x9ea7c8u); /* VA 00659ac0 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x810u, self, 9u, 0xbu, 0x7d1u, 0x9ea804u, 0x9ea7ecu); /* VA 00659ae5 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x820u, self, 9u, 0xbu, 0x7d1u, 0x9ea7fcu, 0x9ea7b4u); /* VA 00659b0a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x830u, self, 9u, 0xbu, 0x7d1u, 0x9ea7d4u, 0x9ea82cu); /* VA 00659b2f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x840u, self, 9u, 0xbu, 0x7d1u, 0x9d9860u, 0x9ea820u); /* VA 00659b54 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x850u, self, 9u, 0xbu, 0x7d1u, 0x9ea7e4u, 0x9ea818u); /* VA 00659b79 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x860u, self, 9u, 0xbu, 0x7d1u, 0x9ea798u, 0x9ea7bcu); /* VA 00659b9e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x870u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea820u, 0x9ea7a0u); /* VA 00659bc3 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x880u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea82cu, 0x9d9860u); /* VA 00659be8 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x890u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea818u, 0x9ea80cu); /* VA 00659c0d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x8a0u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea7c8u, 0x9ea7d4u); /* VA 00659c32 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x8b0u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea7b4u, 0x9ea7f4u); /* VA 00659c57 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x8c0u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea7ecu, 0x9ea7e4u); /* VA 00659c7c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x8d0u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea7bcu, 0x9ea7fcu); /* VA 00659ca1 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x8e0u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea7a8u, 0x9ea798u); /* VA 00659cc6 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x8f0u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea834u, 0x9ea804u); /* VA 00659ceb */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x900u, self, 0x17u, 0xbu, 0x7d1u, 0x9ea80cu, 0x9ea7ecu); /* VA 00659d10 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x910u, self, 0x17u, 0xbu, 0x7d1u, 0x9ea7a0u, 0x9ea7b4u); /* VA 00659d35 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x920u, self, 0x17u, 0xbu, 0x7d1u, 0x9ea7f4u, 0x9ea818u); /* VA 00659d5a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x930u, self, 0x17u, 0xbu, 0x7d1u, 0x9ea804u, 0x9ea7a8u); /* VA 00659d7f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x940u, self, 0x17u, 0xbu, 0x7d1u, 0x9ea7fcu, 0x9ea820u); /* VA 00659da4 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x950u, self, 0x17u, 0xbu, 0x7d1u, 0x9ea7d4u, 0x9ea7bcu); /* VA 00659dc9 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x960u, self, 0x17u, 0xbu, 0x7d1u, 0x9d9860u, 0x9ea7c8u); /* VA 00659dee */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x970u, self, 0x17u, 0xbu, 0x7d1u, 0x9ea7e4u, 0x9ea834u); /* VA 00659e13 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x980u, self, 0x17u, 0xbu, 0x7d1u, 0x9ea798u, 0x9ea82cu); /* VA 00659e38 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x990u, self, 6u, 0u, 0x7d2u, 0x9ea834u, 0x9ea82cu); /* VA 00659e5d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x9a0u, self, 6u, 0u, 0x7d2u, 0x9ea820u, 0x9ea818u); /* VA 00659e82 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x9b0u, self, 6u, 0u, 0x7d2u, 0x9ea80cu, 0x9ea804u); /* VA 00659ea7 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x9c0u, self, 6u, 0u, 0x7d2u, 0x9d9860u, 0x9ea7fcu); /* VA 00659ecc */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x9d0u, self, 6u, 0u, 0x7d2u, 0x9ea7f4u, 0x9ea7ecu); /* VA 00659ef1 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x9e0u, self, 6u, 0u, 0x7d2u, 0x9ea7e4u, 0x9ea7d4u); /* VA 00659f16 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x9f0u, self, 6u, 0u, 0x7d2u, 0x9ea7c8u, 0x9ea7bcu); /* VA 00659f3b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa00u, self, 6u, 0u, 0x7d2u, 0x9ea7b4u, 0x9ea7a8u); /* VA 00659f60 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa10u, self, 6u, 0u, 0x7d2u, 0x9ea7a0u, 0x9ea798u); /* VA 00659f85 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa20u, self, 0xdu, 0u, 0x7d2u, 0x9ea7d4u, 0x9ea80cu); /* VA 00659faa */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa30u, self, 0xdu, 0u, 0x7d2u, 0x9ea804u, 0x9ea7a0u); /* VA 00659fcf */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa40u, self, 0xdu, 0u, 0x7d2u, 0x9ea7ecu, 0x9ea820u); /* VA 00659ff4 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa50u, self, 0xdu, 0u, 0x7d2u, 0x9ea82cu, 0x9ea7f4u); /* VA 0065a019 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa60u, self, 0xdu, 0u, 0x7d2u, 0x9ea7fcu, 0x9ea7c8u); /* VA 0065a03e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa70u, self, 0xdu, 0u, 0x7d2u, 0x9ea818u, 0x9ea7b4u); /* VA 0065a063 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa80u, self, 0xdu, 0u, 0x7d2u, 0x9ea7bcu, 0x9d9860u); /* VA 0065a088 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa90u, self, 0xdu, 0u, 0x7d2u, 0x9ea798u, 0x9ea7e4u); /* VA 0065a0ad */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xaa0u, self, 0xdu, 0u, 0x7d2u, 0x9ea7a8u, 0x9ea834u); /* VA 0065a0d2 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xab0u, self, 0x14u, 0u, 0x7d2u, 0x9ea7a0u, 0x9ea82cu); /* VA 0065a0f7 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xac0u, self, 0x14u, 0u, 0x7d2u, 0x9ea7ecu, 0x9ea818u); /* VA 0065a11c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xad0u, self, 0x14u, 0u, 0x7d2u, 0x9ea7e4u, 0x9ea804u); /* VA 0065a141 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xae0u, self, 0x14u, 0u, 0x7d2u, 0x9ea80cu, 0x9ea7fcu); /* VA 0065a166 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xaf0u, self, 0x14u, 0u, 0x7d2u, 0x9ea7b4u, 0x9ea7d4u); /* VA 0065a18b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb00u, self, 0x14u, 0u, 0x7d2u, 0x9ea7f4u, 0x9d9860u); /* VA 0065a1b0 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb10u, self, 0x14u, 0u, 0x7d2u, 0x9ea820u, 0x9ea7bcu); /* VA 0065a1d5 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb20u, self, 0x14u, 0u, 0x7d2u, 0x9ea7c8u, 0x9ea7a8u); /* VA 0065a1fa */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb30u, self, 0x14u, 0u, 0x7d2u, 0x9ea834u, 0x9ea798u); /* VA 0065a21f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb40u, self, 0x1bu, 0u, 0x7d2u, 0x9ea798u, 0x9ea80cu); /* VA 0065a244 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb50u, self, 0x1bu, 0u, 0x7d2u, 0x9d9860u, 0x9ea7a0u); /* VA 0065a269 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb60u, self, 0x1bu, 0u, 0x7d2u, 0x9ea7a8u, 0x9ea820u); /* VA 0065a28e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb70u, self, 0x1bu, 0u, 0x7d2u, 0x9ea7d4u, 0x9ea7f4u); /* VA 0065a2b3 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb80u, self, 0x1bu, 0u, 0x7d2u, 0x9ea804u, 0x9ea7c8u); /* VA 0065a2d8 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb90u, self, 0x1bu, 0u, 0x7d2u, 0x9ea82cu, 0x9ea7b4u); /* VA 0065a2fd */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xba0u, self, 0x1bu, 0u, 0x7d2u, 0x9ea7bcu, 0x9ea7ecu); /* VA 0065a322 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xbb0u, self, 0x1bu, 0u, 0x7d2u, 0x9ea7fcu, 0x9ea7e4u); /* VA 0065a347 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xbc0u, self, 0x1bu, 0u, 0x7d2u, 0x9ea818u, 0x9ea834u); /* VA 0065a36c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xbd0u, self, 3u, 1u, 0x7d2u, 0x9ea7f4u, 0x9ea7a0u); /* VA 0065a391 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xbe0u, self, 3u, 1u, 0x7d2u, 0x9ea7e4u, 0x9ea820u); /* VA 0065a3b6 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xbf0u, self, 3u, 1u, 0x7d2u, 0x9ea7c8u, 0x9ea82cu); /* VA 0065a3db */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc00u, self, 3u, 1u, 0x7d2u, 0x9ea80cu, 0x9ea7b4u); /* VA 0065a400 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc10u, self, 3u, 1u, 0x7d2u, 0x9ea7d4u, 0x9d9860u); /* VA 0065a425 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc20u, self, 3u, 1u, 0x7d2u, 0x9ea818u, 0x9ea7bcu); /* VA 0065a44a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc30u, self, 3u, 1u, 0x7d2u, 0x9ea7ecu, 0x9ea7a8u); /* VA 0065a46f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc40u, self, 3u, 1u, 0x7d2u, 0x9ea804u, 0x9ea798u); /* VA 0065a494 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc50u, self, 3u, 1u, 0x7d2u, 0x9ea7fcu, 0x9ea834u); /* VA 0065a4b9 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc60u, self, 0xau, 1u, 0x7d2u, 0x9ea7a0u, 0x9ea80cu); /* VA 0065a4de */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc70u, self, 0xau, 1u, 0x7d2u, 0x9ea834u, 0x9ea7f4u); /* VA 0065a503 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc80u, self, 0xau, 1u, 0x7d2u, 0x9ea7a8u, 0x9ea818u); /* VA 0065a528 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc90u, self, 0xau, 1u, 0x7d2u, 0x9d9860u, 0x9ea804u); /* VA 0065a54d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xca0u, self, 0xau, 1u, 0x7d2u, 0x9ea82cu, 0x9ea7fcu); /* VA 0065a572 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xcb0u, self, 0xau, 1u, 0x7d2u, 0x9ea820u, 0x9ea7c8u); /* VA 0065a597 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xcc0u, self, 0xau, 1u, 0x7d2u, 0x9ea7b4u, 0x9ea7ecu); /* VA 0065a5bc */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xcd0u, self, 0xau, 1u, 0x7d2u, 0x9ea798u, 0x9ea7d4u); /* VA 0065a5e1 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xce0u, self, 0xau, 1u, 0x7d2u, 0x9ea7bcu, 0x9ea7e4u); /* VA 0065a606 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xcf0u, self, 0x11u, 1u, 0x7d2u, 0x9ea7d4u, 0x9ea7a0u); /* VA 0065a62b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd00u, self, 0x11u, 1u, 0x7d2u, 0x9ea804u, 0x9ea820u); /* VA 0065a650 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd10u, self, 0x11u, 1u, 0x7d2u, 0x9ea7fcu, 0x9ea7f4u); /* VA 0065a675 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd20u, self, 0x11u, 1u, 0x7d2u, 0x9ea7ecu, 0x9ea82cu); /* VA 0065a69a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd30u, self, 0x11u, 1u, 0x7d2u, 0x9ea7bcu, 0x9ea7b4u); /* VA 0065a6bf */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd40u, self, 0x11u, 1u, 0x7d2u, 0x9ea80cu, 0x9d9860u); /* VA 0065a6e4 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd50u, self, 0x11u, 1u, 0x7d2u, 0x9ea7e4u, 0x9ea7a8u); /* VA 0065a709 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd60u, self, 0x11u, 1u, 0x7d2u, 0x9ea818u, 0x9ea798u); /* VA 0065a72e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd70u, self, 0x11u, 1u, 0x7d2u, 0x9ea7c8u, 0x9ea834u); /* VA 0065a753 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd80u, self, 0x18u, 1u, 0x7d2u, 0x9ea7c8u, 0x9ea80cu); /* VA 0065a778 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd90u, self, 0x18u, 1u, 0x7d2u, 0x9ea82cu, 0x9ea818u); /* VA 0065a79d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xda0u, self, 0x18u, 1u, 0x7d2u, 0x9ea7f4u, 0x9ea804u); /* VA 0065a7c2 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xdb0u, self, 0x18u, 1u, 0x7d2u, 0x9ea7a0u, 0x9ea7fcu); /* VA 0065a7e7 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xdc0u, self, 0x18u, 1u, 0x7d2u, 0x9d9860u, 0x9ea7ecu); /* VA 0065a80c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xdd0u, self, 0x18u, 1u, 0x7d2u, 0x9ea7a8u, 0x9ea7d4u); /* VA 0065a831 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xde0u, self, 0x18u, 1u, 0x7d2u, 0x9ea834u, 0x9ea7bcu); /* VA 0065a856 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xdf0u, self, 0x18u, 1u, 0x7d2u, 0x9ea7b4u, 0x9ea7e4u); /* VA 0065a87b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe00u, self, 0x18u, 1u, 0x7d2u, 0x9ea820u, 0x9ea798u); /* VA 0065a8a0 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe10u, self, 3u, 2u, 0x7d2u, 0x9ea7ecu, 0x9ea7a0u); /* VA 0065a8c5 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe20u, self, 3u, 2u, 0x7d2u, 0x9ea80cu, 0x9ea7f4u); /* VA 0065a8ea */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe30u, self, 3u, 2u, 0x7d2u, 0x9ea7bcu, 0x9ea82cu); /* VA 0065a90f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe40u, self, 3u, 2u, 0x7d2u, 0x9ea7e4u, 0x9ea7c8u); /* VA 0065a934 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe50u, self, 3u, 2u, 0x7d2u, 0x9ea804u, 0x9ea7b4u); /* VA 0065a959 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe60u, self, 3u, 2u, 0x7d2u, 0x9ea818u, 0x9ea7d4u); /* VA 0065a97e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe70u, self, 3u, 2u, 0x7d2u, 0x9ea798u, 0x9d9860u); /* VA 0065a9a3 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe80u, self, 3u, 2u, 0x7d2u, 0x9ea7fcu, 0x9ea7a8u); /* VA 0065a9c8 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe90u, self, 3u, 2u, 0x7d2u, 0x9ea820u, 0x9ea834u); /* VA 0065a9ed */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xea0u, self, 0xau, 2u, 0x7d2u, 0x9ea7a8u, 0x9ea80cu); /* VA 0065aa12 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xeb0u, self, 0xau, 2u, 0x7d2u, 0x9ea7b4u, 0x9ea820u); /* VA 0065aa37 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xec0u, self, 0xau, 2u, 0x7d2u, 0x9ea7c8u, 0x9ea818u); /* VA 0065aa5c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xed0u, self, 0xau, 2u, 0x7d2u, 0x9ea82cu, 0x9ea804u); /* VA 0065aa81 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xee0u, self, 0xau, 2u, 0x7d2u, 0x9ea7d4u, 0x9ea7fcu); /* VA 0065aaa6 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xef0u, self, 0xau, 2u, 0x7d2u, 0x9ea834u, 0x9ea7ecu); /* VA 0065aacb */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf00u, self, 0xau, 2u, 0x7d2u, 0x9ea7a0u, 0x9ea7bcu); /* VA 0065aaf0 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf10u, self, 0xau, 2u, 0x7d2u, 0x9d9860u, 0x9ea7e4u); /* VA 0065ab15 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf20u, self, 0xau, 2u, 0x7d2u, 0x9ea7f4u, 0x9ea798u); /* VA 0065ab3a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf30u, self, 0x18u, 2u, 0x7d2u, 0x9ea7e4u, 0x9ea7a0u); /* VA 0065ab5f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf40u, self, 0x18u, 2u, 0x7d2u, 0x9ea820u, 0x9ea7f4u); /* VA 0065ab84 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf50u, self, 0x18u, 2u, 0x7d2u, 0x9ea80cu, 0x9ea82cu); /* VA 0065aba9 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf60u, self, 0x18u, 2u, 0x7d2u, 0x9ea7ecu, 0x9ea7fcu); /* VA 0065abce */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf70u, self, 0x18u, 2u, 0x7d2u, 0x9ea798u, 0x9ea7c8u); /* VA 0065abf3 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf80u, self, 0x18u, 2u, 0x7d2u, 0x9ea834u, 0x9ea7b4u); /* VA 0065ac18 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf90u, self, 0x18u, 2u, 0x7d2u, 0x9ea804u, 0x9ea7d4u); /* VA 0065ac3d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xfa0u, self, 0x18u, 2u, 0x7d2u, 0x9ea818u, 0x9d9860u); /* VA 0065ac62 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xfb0u, self, 0x18u, 2u, 0x7d2u, 0x9ea7bcu, 0x9ea7a8u); /* VA 0065ac87 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xfc0u, self, 0x1eu, 2u, 0x7d2u, 0x9ea7d4u, 0x9ea820u); /* VA 0065acac */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xfd0u, self, 0x1eu, 2u, 0x7d2u, 0x9ea7a0u, 0x9ea818u); /* VA 0065acd1 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xfe0u, self, 0x1eu, 2u, 0x7d2u, 0x9ea7fcu, 0x9ea804u); /* VA 0065acf6 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xff0u, self, 0x1eu, 2u, 0x7d2u, 0x9ea7c8u, 0x9ea7ecu); /* VA 0065ad1b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1000u, self, 0x1eu, 2u, 0x7d2u, 0x9ea7f4u, 0x9ea7bcu); /* VA 0065ad40 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1010u, self, 0x1eu, 2u, 0x7d2u, 0x9ea82cu, 0x9ea7a8u); /* VA 0065ad65 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1020u, self, 0x1eu, 2u, 0x7d2u, 0x9ea80cu, 0x9ea7e4u); /* VA 0065ad8a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1030u, self, 0x1eu, 2u, 0x7d2u, 0x9ea7b4u, 0x9ea798u); /* VA 0065adaf */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1040u, self, 0x1eu, 2u, 0x7d2u, 0x9d9860u, 0x9ea834u); /* VA 0065add4 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1050u, self, 7u, 3u, 0x7d2u, 0x9ea7bcu, 0x9ea80cu); /* VA 0065adf9 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1060u, self, 7u, 3u, 0x7d2u, 0x9ea834u, 0x9ea7a0u); /* VA 0065ae1e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1070u, self, 7u, 3u, 0x7d2u, 0x9ea7e4u, 0x9ea7f4u); /* VA 0065ae43 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1080u, self, 7u, 3u, 0x7d2u, 0x9ea820u, 0x9ea82cu); /* VA 0065ae68 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1090u, self, 7u, 3u, 0x7d2u, 0x9ea818u, 0x9ea804u); /* VA 0065ae8d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x10a0u, self, 7u, 3u, 0x7d2u, 0x9ea798u, 0x9ea7fcu); /* VA 0065aeb2 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x10b0u, self, 7u, 3u, 0x7d2u, 0x9ea7b4u, 0x9ea7c8u); /* VA 0065aed7 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x10c0u, self, 7u, 3u, 0x7d2u, 0x9ea7ecu, 0x9ea7d4u); /* VA 0065aefc */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x10d0u, self, 7u, 3u, 0x7d2u, 0x9ea7a8u, 0x9d9860u); /* VA 0065af21 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x10e0u, self, 0xeu, 3u, 0x7d2u, 0x9ea80cu, 0x9ea820u); /* VA 0065af46 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x10f0u, self, 0xeu, 3u, 0x7d2u, 0x9ea7e4u, 0x9ea82cu); /* VA 0065af6b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1100u, self, 0xeu, 3u, 0x7d2u, 0x9ea7fcu, 0x9ea818u); /* VA 0065af90 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1110u, self, 0xeu, 3u, 0x7d2u, 0x9ea7a0u, 0x9ea7c8u); /* VA 0065afb5 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1120u, self, 0xeu, 3u, 0x7d2u, 0x9d9860u, 0x9ea7b4u); /* VA 0065afda */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1130u, self, 0xeu, 3u, 0x7d2u, 0x9ea798u, 0x9ea7ecu); /* VA 0065afff */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1140u, self, 0xeu, 3u, 0x7d2u, 0x9ea804u, 0x9ea7bcu); /* VA 0065b024 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1150u, self, 0xeu, 3u, 0x7d2u, 0x9ea7f4u, 0x9ea7a8u); /* VA 0065b049 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1160u, self, 0xeu, 3u, 0x7d2u, 0x9ea7d4u, 0x9ea834u); /* VA 0065b06e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1170u, self, 0x15u, 3u, 0x7d2u, 0x9ea834u, 0x9ea80cu); /* VA 0065b093 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1180u, self, 0x15u, 3u, 0x7d2u, 0x9ea7a8u, 0x9ea7a0u); /* VA 0065b0b8 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1190u, self, 0x15u, 3u, 0x7d2u, 0x9ea7c8u, 0x9ea7f4u); /* VA 0065b0dd */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x11a0u, self, 0x15u, 3u, 0x7d2u, 0x9ea7ecu, 0x9ea804u); /* VA 0065b102 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x11b0u, self, 0x15u, 3u, 0x7d2u, 0x9ea7b4u, 0x9ea7fcu); /* VA 0065b127 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x11c0u, self, 0x15u, 3u, 0x7d2u, 0x9ea82cu, 0x9ea7d4u); /* VA 0065b14c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x11d0u, self, 0x15u, 3u, 0x7d2u, 0x9ea820u, 0x9d9860u); /* VA 0065b171 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x11e0u, self, 0x15u, 3u, 0x7d2u, 0x9ea818u, 0x9ea7e4u); /* VA 0065b196 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x11f0u, self, 0x15u, 3u, 0x7d2u, 0x9ea7bcu, 0x9ea798u); /* VA 0065b1bb */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1200u, self, 0x1cu, 3u, 0x7d2u, 0x9ea7a0u, 0x9ea820u); /* VA 0065b1e0 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1210u, self, 0x1cu, 3u, 0x7d2u, 0x9d9860u, 0x9ea82cu); /* VA 0065b205 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1220u, self, 0x1cu, 3u, 0x7d2u, 0x9ea80cu, 0x9ea818u); /* VA 0065b22a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1230u, self, 0x1cu, 3u, 0x7d2u, 0x9ea7d4u, 0x9ea7c8u); /* VA 0065b24f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1240u, self, 0x1cu, 3u, 0x7d2u, 0x9ea7f4u, 0x9ea7b4u); /* VA 0065b274 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1250u, self, 0x1cu, 3u, 0x7d2u, 0x9ea7e4u, 0x9ea7ecu); /* VA 0065b299 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1260u, self, 0x1cu, 3u, 0x7d2u, 0x9ea7fcu, 0x9ea7bcu); /* VA 0065b2be */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1270u, self, 0x1cu, 3u, 0x7d2u, 0x9ea798u, 0x9ea7a8u); /* VA 0065b2e3 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1280u, self, 0x1cu, 3u, 0x7d2u, 0x9ea804u, 0x9ea834u); /* VA 0065b308 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1290u, self, 5u, 4u, 0x7d2u, 0x9ea7ecu, 0x9ea80cu); /* VA 0065b32d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x12a0u, self, 5u, 4u, 0x7d2u, 0x9ea7b4u, 0x9ea7a0u); /* VA 0065b352 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x12b0u, self, 5u, 4u, 0x7d2u, 0x9ea818u, 0x9ea7f4u); /* VA 0065b377 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x12c0u, self, 5u, 4u, 0x7d2u, 0x9ea7a8u, 0x9ea804u); /* VA 0065b39c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x12d0u, self, 5u, 4u, 0x7d2u, 0x9ea820u, 0x9ea7fcu); /* VA 0065b3c1 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x12e0u, self, 5u, 4u, 0x7d2u, 0x9ea7bcu, 0x9ea7d4u); /* VA 0065b3e6 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x12f0u, self, 5u, 4u, 0x7d2u, 0x9ea7c8u, 0x9d9860u); /* VA 0065b40b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1300u, self, 5u, 4u, 0x7d2u, 0x9ea834u, 0x9ea7e4u); /* VA 0065b430 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1310u, self, 5u, 4u, 0x7d2u, 0x9ea82cu, 0x9ea798u); /* VA 0065b455 */
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

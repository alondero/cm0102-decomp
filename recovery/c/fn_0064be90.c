/* Preferred-image VA 0x0064be90, base 0x00400000.
   CM0102/Game/cm0102.exe SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Every schedule call below preserves the original immediate pushes, receiver
   displacement and call order; each following receiver is a fresh DWORD read.
   Direct caller VA 0x0064ab66 discards EAX before its next use. */
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

void CM_THISCALL cm_fn_0064be90(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x204];
    s32 half, factor, count;
    u32 allocation, receiver, table, cursor, index;

    frame.state = 0xffffffffu;
    frame.handler = 0x95bb0eu;
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
        cm_fn_00944cff(0x9ea474u, 0, 0, scratch + 0x104, scratch + 4);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text,
                       scratch + 0x104, scratch + 4, 0x3fdu);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0;
    } else {
    cm_fn_0068aff0(receiver + 0x0u, self, 2u, 8u, 0x7d1u, 0x9ea540u, 0x9ea54cu); /* VA 0064bf95 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x10u, self, 2u, 8u, 0x7d1u, 0x9ea530u, 0x9ea538u); /* VA 0064bfb7 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x20u, self, 2u, 8u, 0x7d1u, 0x9ea520u, 0x9ea528u); /* VA 0064bfd9 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x30u, self, 2u, 8u, 0x7d1u, 0x9ea50cu, 0x9ea518u); /* VA 0064bffb */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x40u, self, 2u, 8u, 0x7d1u, 0x9ea4fcu, 0x9ea504u); /* VA 0064c01d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x50u, self, 2u, 8u, 0x7d1u, 0x9ea4e4u, 0x9ea4f0u); /* VA 0064c03f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x60u, self, 2u, 8u, 0x7d1u, 0x9ea4d4u, 0x9ea4dcu); /* VA 0064c061 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x70u, self, 2u, 8u, 0x7d1u, 0x9ea4c4u, 0x9ea4ccu); /* VA 0064c083 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x80u, self, 2u, 8u, 0x7d1u, 0x9ea4b0u, 0x9ea4b8u); /* VA 0064c0a8 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x90u, self, 9u, 8u, 0x7d1u, 0x9ea538u, 0x9ea4fcu); /* VA 0064c0cd */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa0u, self, 9u, 8u, 0x7d1u, 0x9ea54cu, 0x9ea4d4u); /* VA 0064c0f2 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb0u, self, 9u, 8u, 0x7d1u, 0x9ea518u, 0x9ea540u); /* VA 0064c117 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc0u, self, 9u, 8u, 0x7d1u, 0x9ea528u, 0x9ea4b0u); /* VA 0064c13c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd0u, self, 9u, 8u, 0x7d1u, 0x9ea4b8u, 0x9ea50cu); /* VA 0064c161 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe0u, self, 9u, 8u, 0x7d1u, 0x9ea4ccu, 0x9ea520u); /* VA 0064c186 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf0u, self, 9u, 8u, 0x7d1u, 0x9ea4dcu, 0x9ea4e4u); /* VA 0064c1ab */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x100u, self, 9u, 8u, 0x7d1u, 0x9ea504u, 0x9ea4c4u); /* VA 0064c1d0 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x110u, self, 9u, 8u, 0x7d1u, 0x9ea4f0u, 0x9ea530u); /* VA 0064c1f5 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x120u, self, 0x10u, 8u, 0x7d1u, 0x9ea540u, 0x9ea4f0u); /* VA 0064c21a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x130u, self, 0x10u, 8u, 0x7d1u, 0x9ea530u, 0x9ea4c4u); /* VA 0064c23f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x140u, self, 0x10u, 8u, 0x7d1u, 0x9ea538u, 0x9ea528u); /* VA 0064c264 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x150u, self, 0x10u, 8u, 0x7d1u, 0x9ea54cu, 0x9ea4dcu); /* VA 0064c289 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x160u, self, 0x10u, 8u, 0x7d1u, 0x9ea520u, 0x9ea4e4u); /* VA 0064c2ae */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x170u, self, 0x10u, 8u, 0x7d1u, 0x9ea50cu, 0x9ea4ccu); /* VA 0064c2d3 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x180u, self, 0x10u, 8u, 0x7d1u, 0x9ea4fcu, 0x9ea4b8u); /* VA 0064c2f8 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x190u, self, 0x10u, 8u, 0x7d1u, 0x9ea4d4u, 0x9ea518u); /* VA 0064c31d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1a0u, self, 0x10u, 8u, 0x7d1u, 0x9ea4b0u, 0x9ea504u); /* VA 0064c342 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1b0u, self, 0x17u, 8u, 0x7d1u, 0x9ea518u, 0x9ea520u); /* VA 0064c367 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1c0u, self, 0x17u, 8u, 0x7d1u, 0x9ea528u, 0x9ea4d4u); /* VA 0064c38c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1d0u, self, 0x17u, 8u, 0x7d1u, 0x9ea4b8u, 0x9ea54cu); /* VA 0064c3b1 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1e0u, self, 0x17u, 8u, 0x7d1u, 0x9ea4ccu, 0x9ea4fcu); /* VA 0064c3d6 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1f0u, self, 0x17u, 8u, 0x7d1u, 0x9ea4dcu, 0x9ea540u); /* VA 0064c3fb */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x200u, self, 0x17u, 8u, 0x7d1u, 0x9ea4e4u, 0x9ea530u); /* VA 0064c420 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x210u, self, 0x17u, 8u, 0x7d1u, 0x9ea4c4u, 0x9ea4b0u); /* VA 0064c445 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x220u, self, 0x17u, 8u, 0x7d1u, 0x9ea504u, 0x9ea50cu); /* VA 0064c46a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x230u, self, 0x17u, 8u, 0x7d1u, 0x9ea4f0u, 0x9ea538u); /* VA 0064c48f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x240u, self, 0x1eu, 8u, 0x7d1u, 0x9ea540u, 0x9ea4e4u); /* VA 0064c4b4 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x250u, self, 0x1eu, 8u, 0x7d1u, 0x9ea530u, 0x9ea4ccu); /* VA 0064c4d9 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x260u, self, 0x1eu, 8u, 0x7d1u, 0x9ea538u, 0x9ea4dcu); /* VA 0064c4fe */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x270u, self, 0x1eu, 8u, 0x7d1u, 0x9ea54cu, 0x9ea528u); /* VA 0064c523 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x280u, self, 0x1eu, 8u, 0x7d1u, 0x9ea520u, 0x9ea504u); /* VA 0064c548 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x290u, self, 0x1eu, 8u, 0x7d1u, 0x9ea518u, 0x9ea4f0u); /* VA 0064c56d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x2a0u, self, 0x1eu, 8u, 0x7d1u, 0x9ea4fcu, 0x9ea4c4u); /* VA 0064c592 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x2b0u, self, 0x1eu, 8u, 0x7d1u, 0x9ea4d4u, 0x9ea4b8u); /* VA 0064c5b7 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x2c0u, self, 0x1eu, 8u, 0x7d1u, 0x9ea4b0u, 0x9ea50cu); /* VA 0064c5dc */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x2d0u, self, 7u, 9u, 0x7d1u, 0x9ea530u, 0x9ea4b0u); /* VA 0064c601 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x2e0u, self, 7u, 9u, 0x7d1u, 0x9ea4b8u, 0x9ea518u); /* VA 0064c626 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x2f0u, self, 7u, 9u, 0x7d1u, 0x9ea50cu, 0x9ea520u); /* VA 0064c64b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x300u, self, 7u, 9u, 0x7d1u, 0x9ea4ccu, 0x9ea538u); /* VA 0064c670 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x310u, self, 7u, 9u, 0x7d1u, 0x9ea4dcu, 0x9ea4f0u); /* VA 0064c695 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x320u, self, 7u, 9u, 0x7d1u, 0x9ea4fcu, 0x9ea540u); /* VA 0064c6ba */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x330u, self, 7u, 9u, 0x7d1u, 0x9ea4e4u, 0x9ea528u); /* VA 0064c6df */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x340u, self, 7u, 9u, 0x7d1u, 0x9ea4c4u, 0x9ea54cu); /* VA 0064c704 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x350u, self, 7u, 9u, 0x7d1u, 0x9ea504u, 0x9ea4d4u); /* VA 0064c729 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x360u, self, 0xeu, 9u, 0x7d1u, 0x9ea540u, 0x9ea4b8u); /* VA 0064c74e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x370u, self, 0xeu, 9u, 0x7d1u, 0x9ea538u, 0x9ea4b0u); /* VA 0064c773 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x380u, self, 0xeu, 9u, 0x7d1u, 0x9ea54cu, 0x9ea4e4u); /* VA 0064c798 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x390u, self, 0xeu, 9u, 0x7d1u, 0x9ea520u, 0x9ea4c4u); /* VA 0064c7bd */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x3a0u, self, 0xeu, 9u, 0x7d1u, 0x9ea518u, 0x9ea530u); /* VA 0064c7e2 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x3b0u, self, 0xeu, 9u, 0x7d1u, 0x9ea528u, 0x9ea4fcu); /* VA 0064c807 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x3c0u, self, 0xeu, 9u, 0x7d1u, 0x9ea4dcu, 0x9ea504u); /* VA 0064c82c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x3d0u, self, 0xeu, 9u, 0x7d1u, 0x9ea4d4u, 0x9ea4ccu); /* VA 0064c851 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x3e0u, self, 0xeu, 9u, 0x7d1u, 0x9ea4f0u, 0x9ea50cu); /* VA 0064c876 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x3f0u, self, 0x15u, 9u, 0x7d1u, 0x9ea520u, 0x9ea538u); /* VA 0064c89b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x400u, self, 0x15u, 9u, 0x7d1u, 0x9ea50cu, 0x9ea540u); /* VA 0064c8c0 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x410u, self, 0x15u, 9u, 0x7d1u, 0x9ea4ccu, 0x9ea4dcu); /* VA 0064c8e5 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x420u, self, 0x15u, 9u, 0x7d1u, 0x9ea4fcu, 0x9ea530u); /* VA 0064c90a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x430u, self, 0x15u, 9u, 0x7d1u, 0x9ea4e4u, 0x9ea4b8u); /* VA 0064c92f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x440u, self, 0x15u, 9u, 0x7d1u, 0x9ea4c4u, 0x9ea518u); /* VA 0064c954 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x450u, self, 0x15u, 9u, 0x7d1u, 0x9ea504u, 0x9ea54cu); /* VA 0064c979 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x460u, self, 0x15u, 9u, 0x7d1u, 0x9ea4f0u, 0x9ea528u); /* VA 0064c99e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x470u, self, 0x15u, 9u, 0x7d1u, 0x9ea4b0u, 0x9ea4d4u); /* VA 0064c9c3 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x480u, self, 0x1cu, 9u, 0x7d1u, 0x9ea540u, 0x9ea504u); /* VA 0064c9e8 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x490u, self, 0x1cu, 9u, 0x7d1u, 0x9ea530u, 0x9ea520u); /* VA 0064ca0d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x4a0u, self, 0x1cu, 9u, 0x7d1u, 0x9ea538u, 0x9ea50cu); /* VA 0064ca32 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x4b0u, self, 0x1cu, 9u, 0x7d1u, 0x9ea54cu, 0x9ea4ccu); /* VA 0064ca57 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x4c0u, self, 0x1cu, 9u, 0x7d1u, 0x9ea518u, 0x9ea4e4u); /* VA 0064ca7c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x4d0u, self, 0x1cu, 9u, 0x7d1u, 0x9ea528u, 0x9ea4dcu); /* VA 0064caa1 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x4e0u, self, 0x1cu, 9u, 0x7d1u, 0x9ea4b8u, 0x9ea4f0u); /* VA 0064cac6 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x4f0u, self, 0x1cu, 9u, 0x7d1u, 0x9ea4d4u, 0x9ea4c4u); /* VA 0064caeb */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x500u, self, 0x1cu, 9u, 0x7d1u, 0x9ea4b0u, 0x9ea4fcu); /* VA 0064cb10 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x510u, self, 4u, 0xau, 0x7d1u, 0x9ea520u, 0x9ea4b0u); /* VA 0064cb35 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x520u, self, 4u, 0xau, 0x7d1u, 0x9ea528u, 0x9ea4b8u); /* VA 0064cb5a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x530u, self, 4u, 0xau, 0x7d1u, 0x9ea50cu, 0x9ea4fcu); /* VA 0064cb7f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x540u, self, 4u, 0xau, 0x7d1u, 0x9ea4ccu, 0x9ea540u); /* VA 0064cba4 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x550u, self, 4u, 0xau, 0x7d1u, 0x9ea4dcu, 0x9ea518u); /* VA 0064cbc9 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x560u, self, 4u, 0xau, 0x7d1u, 0x9ea4e4u, 0x9ea4d4u); /* VA 0064cbee */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x570u, self, 4u, 0xau, 0x7d1u, 0x9ea4c4u, 0x9ea538u); /* VA 0064cc13 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x580u, self, 4u, 0xau, 0x7d1u, 0x9ea504u, 0x9ea530u); /* VA 0064cc38 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x590u, self, 4u, 0xau, 0x7d1u, 0x9ea4f0u, 0x9ea54cu); /* VA 0064cc5d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x5a0u, self, 0xbu, 0xau, 0x7d1u, 0x9ea540u, 0x9ea528u); /* VA 0064cc82 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x5b0u, self, 0xbu, 0xau, 0x7d1u, 0x9ea530u, 0x9ea50cu); /* VA 0064cca7 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x5c0u, self, 0xbu, 0xau, 0x7d1u, 0x9ea538u, 0x9ea504u); /* VA 0064cccc */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x5d0u, self, 0xbu, 0xau, 0x7d1u, 0x9ea518u, 0x9ea54cu); /* VA 0064ccf1 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x5e0u, self, 0xbu, 0xau, 0x7d1u, 0x9ea4b8u, 0x9ea4dcu); /* VA 0064cd16 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x5f0u, self, 0xbu, 0xau, 0x7d1u, 0x9ea4fcu, 0x9ea520u); /* VA 0064cd3b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x600u, self, 0xbu, 0xau, 0x7d1u, 0x9ea4d4u, 0x9ea4f0u); /* VA 0064cd60 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x610u, self, 0xbu, 0xau, 0x7d1u, 0x9ea4c4u, 0x9ea4e4u); /* VA 0064cd85 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x620u, self, 0xbu, 0xau, 0x7d1u, 0x9ea4b0u, 0x9ea4ccu); /* VA 0064cdaa */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x630u, self, 0x12u, 0xau, 0x7d1u, 0x9ea540u, 0x9ea4d4u); /* VA 0064cdcf */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x640u, self, 0x12u, 0xau, 0x7d1u, 0x9ea54cu, 0x9ea4fcu); /* VA 0064cdf4 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x650u, self, 0x12u, 0xau, 0x7d1u, 0x9ea528u, 0x9ea518u); /* VA 0064ce19 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x660u, self, 0x12u, 0xau, 0x7d1u, 0x9ea4b8u, 0x9ea538u); /* VA 0064ce3e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x670u, self, 0x12u, 0xau, 0x7d1u, 0x9ea50cu, 0x9ea4c4u); /* VA 0064ce63 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x680u, self, 0x12u, 0xau, 0x7d1u, 0x9ea4dcu, 0x9ea530u); /* VA 0064ce88 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x690u, self, 0x12u, 0xau, 0x7d1u, 0x9ea4e4u, 0x9ea4b0u); /* VA 0064cead */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x6a0u, self, 0x12u, 0xau, 0x7d1u, 0x9ea504u, 0x9ea4ccu); /* VA 0064ced2 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x6b0u, self, 0x12u, 0xau, 0x7d1u, 0x9ea4f0u, 0x9ea520u); /* VA 0064cef7 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x6c0u, self, 0x19u, 0xau, 0x7d1u, 0x9ea530u, 0x9ea540u); /* VA 0064cf1c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x6d0u, self, 0x19u, 0xau, 0x7d1u, 0x9ea538u, 0x9ea4e4u); /* VA 0064cf41 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x6e0u, self, 0x19u, 0xau, 0x7d1u, 0x9ea520u, 0x9ea4dcu); /* VA 0064cf66 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x6f0u, self, 0x19u, 0xau, 0x7d1u, 0x9ea50cu, 0x9ea54cu); /* VA 0064cf8b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x700u, self, 0x19u, 0xau, 0x7d1u, 0x9ea4ccu, 0x9ea4b8u); /* VA 0064cfb0 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x710u, self, 0x19u, 0xau, 0x7d1u, 0x9ea4fcu, 0x9ea4d4u); /* VA 0064cfd5 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x720u, self, 0x19u, 0xau, 0x7d1u, 0x9ea4c4u, 0x9ea528u); /* VA 0064cffa */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x730u, self, 0x19u, 0xau, 0x7d1u, 0x9ea504u, 0x9ea4f0u); /* VA 0064d01f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x740u, self, 0x19u, 0xau, 0x7d1u, 0x9ea4b0u, 0x9ea518u); /* VA 0064d044 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x750u, self, 2u, 0xbu, 0x7d1u, 0x9ea540u, 0x9ea4b0u); /* VA 0064d069 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x760u, self, 2u, 0xbu, 0x7d1u, 0x9ea54cu, 0x9ea530u); /* VA 0064d08e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x770u, self, 2u, 0xbu, 0x7d1u, 0x9ea518u, 0x9ea538u); /* VA 0064d0b3 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x780u, self, 2u, 0xbu, 0x7d1u, 0x9ea528u, 0x9ea4ccu); /* VA 0064d0d8 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x790u, self, 2u, 0xbu, 0x7d1u, 0x9ea4b8u, 0x9ea504u); /* VA 0064d0fd */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x7a0u, self, 2u, 0xbu, 0x7d1u, 0x9ea4dcu, 0x9ea4fcu); /* VA 0064d122 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x7b0u, self, 2u, 0xbu, 0x7d1u, 0x9ea4e4u, 0x9ea50cu); /* VA 0064d147 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x7c0u, self, 2u, 0xbu, 0x7d1u, 0x9ea4d4u, 0x9ea520u); /* VA 0064d16c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x7d0u, self, 2u, 0xbu, 0x7d1u, 0x9ea4f0u, 0x9ea4c4u); /* VA 0064d191 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x7e0u, self, 9u, 0xbu, 0x7d1u, 0x9ea530u, 0x9ea4d4u); /* VA 0064d1b6 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x7f0u, self, 9u, 0xbu, 0x7d1u, 0x9ea538u, 0x9ea540u); /* VA 0064d1db */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x800u, self, 9u, 0xbu, 0x7d1u, 0x9ea520u, 0x9ea54cu); /* VA 0064d200 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x810u, self, 9u, 0xbu, 0x7d1u, 0x9ea50cu, 0x9ea528u); /* VA 0064d225 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x820u, self, 9u, 0xbu, 0x7d1u, 0x9ea4ccu, 0x9ea518u); /* VA 0064d24a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x830u, self, 9u, 0xbu, 0x7d1u, 0x9ea4fcu, 0x9ea4f0u); /* VA 0064d26f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x840u, self, 9u, 0xbu, 0x7d1u, 0x9ea4c4u, 0x9ea4b8u); /* VA 0064d294 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x850u, self, 9u, 0xbu, 0x7d1u, 0x9ea504u, 0x9ea4e4u); /* VA 0064d2b9 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x860u, self, 9u, 0xbu, 0x7d1u, 0x9ea4b0u, 0x9ea4dcu); /* VA 0064d2de */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x870u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea540u, 0x9ea520u); /* VA 0064d303 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x880u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea54cu, 0x9ea538u); /* VA 0064d328 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x890u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea518u, 0x9ea4fcu); /* VA 0064d34d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x8a0u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea528u, 0x9ea504u); /* VA 0064d372 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x8b0u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea4b8u, 0x9ea530u); /* VA 0064d397 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x8c0u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea4dcu, 0x9ea4c4u); /* VA 0064d3bc */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x8d0u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea4e4u, 0x9ea4ccu); /* VA 0064d3e1 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x8e0u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea4d4u, 0x9ea50cu); /* VA 0064d406 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x8f0u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea4f0u, 0x9ea4b0u); /* VA 0064d42b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x900u, self, 0x17u, 0xbu, 0x7d1u, 0x9ea530u, 0x9ea528u); /* VA 0064d450 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x910u, self, 0x17u, 0xbu, 0x7d1u, 0x9ea538u, 0x9ea4d4u); /* VA 0064d475 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x920u, self, 0x17u, 0xbu, 0x7d1u, 0x9ea520u, 0x9ea4b8u); /* VA 0064d49a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x930u, self, 0x17u, 0xbu, 0x7d1u, 0x9ea50cu, 0x9ea4dcu); /* VA 0064d4bf */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x940u, self, 0x17u, 0xbu, 0x7d1u, 0x9ea4ccu, 0x9ea4f0u); /* VA 0064d4e4 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x950u, self, 0x17u, 0xbu, 0x7d1u, 0x9ea4fcu, 0x9ea4e4u); /* VA 0064d509 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x960u, self, 0x17u, 0xbu, 0x7d1u, 0x9ea4c4u, 0x9ea540u); /* VA 0064d52e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x970u, self, 0x17u, 0xbu, 0x7d1u, 0x9ea504u, 0x9ea518u); /* VA 0064d553 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x980u, self, 0x17u, 0xbu, 0x7d1u, 0x9ea4b0u, 0x9ea54cu); /* VA 0064d578 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x990u, self, 6u, 0u, 0x7d2u, 0x9ea54cu, 0x9ea540u); /* VA 0064d59d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x9a0u, self, 6u, 0u, 0x7d2u, 0x9ea538u, 0x9ea530u); /* VA 0064d5c2 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x9b0u, self, 6u, 0u, 0x7d2u, 0x9ea528u, 0x9ea520u); /* VA 0064d5e7 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x9c0u, self, 6u, 0u, 0x7d2u, 0x9ea518u, 0x9ea50cu); /* VA 0064d60c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x9d0u, self, 6u, 0u, 0x7d2u, 0x9ea504u, 0x9ea4fcu); /* VA 0064d631 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x9e0u, self, 6u, 0u, 0x7d2u, 0x9ea4f0u, 0x9ea4e4u); /* VA 0064d656 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x9f0u, self, 6u, 0u, 0x7d2u, 0x9ea4dcu, 0x9ea4d4u); /* VA 0064d67b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa00u, self, 6u, 0u, 0x7d2u, 0x9ea4ccu, 0x9ea4c4u); /* VA 0064d6a0 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa10u, self, 6u, 0u, 0x7d2u, 0x9ea4b8u, 0x9ea4b0u); /* VA 0064d6c5 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa20u, self, 0xdu, 0u, 0x7d2u, 0x9ea4fcu, 0x9ea538u); /* VA 0064d6ea */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa30u, self, 0xdu, 0u, 0x7d2u, 0x9ea4d4u, 0x9ea54cu); /* VA 0064d70f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa40u, self, 0xdu, 0u, 0x7d2u, 0x9ea540u, 0x9ea518u); /* VA 0064d734 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa50u, self, 0xdu, 0u, 0x7d2u, 0x9ea4b0u, 0x9ea528u); /* VA 0064d759 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa60u, self, 0xdu, 0u, 0x7d2u, 0x9ea50cu, 0x9ea4b8u); /* VA 0064d77e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa70u, self, 0xdu, 0u, 0x7d2u, 0x9ea520u, 0x9ea4ccu); /* VA 0064d7a3 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa80u, self, 0xdu, 0u, 0x7d2u, 0x9ea4e4u, 0x9ea4dcu); /* VA 0064d7c8 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa90u, self, 0xdu, 0u, 0x7d2u, 0x9ea4c4u, 0x9ea504u); /* VA 0064d7ed */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xaa0u, self, 0xdu, 0u, 0x7d2u, 0x9ea530u, 0x9ea4f0u); /* VA 0064d812 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xab0u, self, 0x14u, 0u, 0x7d2u, 0x9ea4f0u, 0x9ea540u); /* VA 0064d837 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xac0u, self, 0x14u, 0u, 0x7d2u, 0x9ea4c4u, 0x9ea530u); /* VA 0064d85c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xad0u, self, 0x14u, 0u, 0x7d2u, 0x9ea528u, 0x9ea538u); /* VA 0064d881 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xae0u, self, 0x14u, 0u, 0x7d2u, 0x9ea4dcu, 0x9ea54cu); /* VA 0064d8a6 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xaf0u, self, 0x14u, 0u, 0x7d2u, 0x9ea4e4u, 0x9ea520u); /* VA 0064d8cb */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb00u, self, 0x14u, 0u, 0x7d2u, 0x9ea4ccu, 0x9ea50cu); /* VA 0064d8f0 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb10u, self, 0x14u, 0u, 0x7d2u, 0x9ea4b8u, 0x9ea4fcu); /* VA 0064d915 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb20u, self, 0x14u, 0u, 0x7d2u, 0x9ea518u, 0x9ea4d4u); /* VA 0064d93a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb30u, self, 0x14u, 0u, 0x7d2u, 0x9ea504u, 0x9ea4b0u); /* VA 0064d95f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb40u, self, 0x1bu, 0u, 0x7d2u, 0x9ea520u, 0x9ea518u); /* VA 0064d984 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb50u, self, 0x1bu, 0u, 0x7d2u, 0x9ea4d4u, 0x9ea528u); /* VA 0064d9a9 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb60u, self, 0x1bu, 0u, 0x7d2u, 0x9ea54cu, 0x9ea4b8u); /* VA 0064d9ce */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb70u, self, 0x1bu, 0u, 0x7d2u, 0x9ea4fcu, 0x9ea4ccu); /* VA 0064d9f3 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb80u, self, 0x1bu, 0u, 0x7d2u, 0x9ea540u, 0x9ea4dcu); /* VA 0064da18 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb90u, self, 0x1bu, 0u, 0x7d2u, 0x9ea530u, 0x9ea4e4u); /* VA 0064da3d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xba0u, self, 0x1bu, 0u, 0x7d2u, 0x9ea4b0u, 0x9ea4c4u); /* VA 0064da62 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xbb0u, self, 0x1bu, 0u, 0x7d2u, 0x9ea50cu, 0x9ea504u); /* VA 0064da87 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xbc0u, self, 0x1bu, 0u, 0x7d2u, 0x9ea538u, 0x9ea4f0u); /* VA 0064daac */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xbd0u, self, 3u, 1u, 0x7d2u, 0x9ea4e4u, 0x9ea540u); /* VA 0064dad1 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xbe0u, self, 3u, 1u, 0x7d2u, 0x9ea4ccu, 0x9ea530u); /* VA 0064daf6 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xbf0u, self, 3u, 1u, 0x7d2u, 0x9ea4dcu, 0x9ea538u); /* VA 0064db1b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc00u, self, 3u, 1u, 0x7d2u, 0x9ea528u, 0x9ea54cu); /* VA 0064db40 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc10u, self, 3u, 1u, 0x7d2u, 0x9ea504u, 0x9ea520u); /* VA 0064db65 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc20u, self, 3u, 1u, 0x7d2u, 0x9ea4f0u, 0x9ea518u); /* VA 0064db8a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc30u, self, 3u, 1u, 0x7d2u, 0x9ea4c4u, 0x9ea4fcu); /* VA 0064dbaf */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc40u, self, 3u, 1u, 0x7d2u, 0x9ea4b8u, 0x9ea4d4u); /* VA 0064dbd4 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc50u, self, 3u, 1u, 0x7d2u, 0x9ea50cu, 0x9ea4b0u); /* VA 0064dbf9 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc60u, self, 0xau, 1u, 0x7d2u, 0x9ea4b0u, 0x9ea530u); /* VA 0064dc1e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc70u, self, 0xau, 1u, 0x7d2u, 0x9ea518u, 0x9ea4b8u); /* VA 0064dc43 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc80u, self, 0xau, 1u, 0x7d2u, 0x9ea520u, 0x9ea50cu); /* VA 0064dc68 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc90u, self, 0xau, 1u, 0x7d2u, 0x9ea538u, 0x9ea4ccu); /* VA 0064dc8d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xca0u, self, 0xau, 1u, 0x7d2u, 0x9ea4f0u, 0x9ea4dcu); /* VA 0064dcb2 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xcb0u, self, 0xau, 1u, 0x7d2u, 0x9ea540u, 0x9ea4fcu); /* VA 0064dcd7 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xcc0u, self, 0xau, 1u, 0x7d2u, 0x9ea528u, 0x9ea4e4u); /* VA 0064dcfc */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xcd0u, self, 0xau, 1u, 0x7d2u, 0x9ea54cu, 0x9ea4c4u); /* VA 0064dd21 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xce0u, self, 0xau, 1u, 0x7d2u, 0x9ea4d4u, 0x9ea504u); /* VA 0064dd46 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xcf0u, self, 0x11u, 1u, 0x7d2u, 0x9ea4b8u, 0x9ea540u); /* VA 0064dd6b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd00u, self, 0x11u, 1u, 0x7d2u, 0x9ea4b0u, 0x9ea538u); /* VA 0064dd90 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd10u, self, 0x11u, 1u, 0x7d2u, 0x9ea4e4u, 0x9ea54cu); /* VA 0064ddb5 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd20u, self, 0x11u, 1u, 0x7d2u, 0x9ea4c4u, 0x9ea520u); /* VA 0064ddda */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd30u, self, 0x11u, 1u, 0x7d2u, 0x9ea530u, 0x9ea518u); /* VA 0064ddff */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd40u, self, 0x11u, 1u, 0x7d2u, 0x9ea4fcu, 0x9ea528u); /* VA 0064de24 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd50u, self, 0x11u, 1u, 0x7d2u, 0x9ea504u, 0x9ea4dcu); /* VA 0064de49 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd60u, self, 0x11u, 1u, 0x7d2u, 0x9ea4ccu, 0x9ea4d4u); /* VA 0064de6e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd70u, self, 0x11u, 1u, 0x7d2u, 0x9ea50cu, 0x9ea4f0u); /* VA 0064de93 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd80u, self, 3u, 2u, 0x7d2u, 0x9ea538u, 0x9ea520u); /* VA 0064deb8 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd90u, self, 3u, 2u, 0x7d2u, 0x9ea540u, 0x9ea50cu); /* VA 0064dedd */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xda0u, self, 3u, 2u, 0x7d2u, 0x9ea4dcu, 0x9ea4ccu); /* VA 0064df02 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xdb0u, self, 3u, 2u, 0x7d2u, 0x9ea530u, 0x9ea4fcu); /* VA 0064df27 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xdc0u, self, 3u, 2u, 0x7d2u, 0x9ea4b8u, 0x9ea4e4u); /* VA 0064df4c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xdd0u, self, 3u, 2u, 0x7d2u, 0x9ea518u, 0x9ea4c4u); /* VA 0064df71 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xde0u, self, 3u, 2u, 0x7d2u, 0x9ea54cu, 0x9ea504u); /* VA 0064df96 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xdf0u, self, 3u, 2u, 0x7d2u, 0x9ea528u, 0x9ea4f0u); /* VA 0064dfbb */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe00u, self, 3u, 2u, 0x7d2u, 0x9ea4d4u, 0x9ea4b0u); /* VA 0064dfe0 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe10u, self, 0xau, 2u, 0x7d2u, 0x9ea504u, 0x9ea540u); /* VA 0064e005 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe20u, self, 0xau, 2u, 0x7d2u, 0x9ea520u, 0x9ea530u); /* VA 0064e02a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe30u, self, 0xau, 2u, 0x7d2u, 0x9ea50cu, 0x9ea538u); /* VA 0064e04f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe40u, self, 0xau, 2u, 0x7d2u, 0x9ea4ccu, 0x9ea54cu); /* VA 0064e074 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe50u, self, 0xau, 2u, 0x7d2u, 0x9ea4e4u, 0x9ea518u); /* VA 0064e099 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe60u, self, 0xau, 2u, 0x7d2u, 0x9ea4dcu, 0x9ea528u); /* VA 0064e0be */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe70u, self, 0xau, 2u, 0x7d2u, 0x9ea4f0u, 0x9ea4b8u); /* VA 0064e0e3 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe80u, self, 0xau, 2u, 0x7d2u, 0x9ea4c4u, 0x9ea4d4u); /* VA 0064e108 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe90u, self, 0xau, 2u, 0x7d2u, 0x9ea4fcu, 0x9ea4b0u); /* VA 0064e12d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xea0u, self, 0x11u, 2u, 0x7d2u, 0x9ea4b0u, 0x9ea520u); /* VA 0064e152 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xeb0u, self, 0x11u, 2u, 0x7d2u, 0x9ea4b8u, 0x9ea528u); /* VA 0064e177 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xec0u, self, 0x11u, 2u, 0x7d2u, 0x9ea4fcu, 0x9ea50cu); /* VA 0064e19c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xed0u, self, 0x11u, 2u, 0x7d2u, 0x9ea540u, 0x9ea4ccu); /* VA 0064e1c1 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xee0u, self, 0x11u, 2u, 0x7d2u, 0x9ea518u, 0x9ea4dcu); /* VA 0064e1e6 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xef0u, self, 0x11u, 2u, 0x7d2u, 0x9ea4d4u, 0x9ea4e4u); /* VA 0064e20b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf00u, self, 0x11u, 2u, 0x7d2u, 0x9ea538u, 0x9ea4c4u); /* VA 0064e230 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf10u, self, 0x11u, 2u, 0x7d2u, 0x9ea530u, 0x9ea504u); /* VA 0064e255 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf20u, self, 0x11u, 2u, 0x7d2u, 0x9ea54cu, 0x9ea4f0u); /* VA 0064e27a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf30u, self, 0x18u, 2u, 0x7d2u, 0x9ea528u, 0x9ea540u); /* VA 0064e29f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf40u, self, 0x18u, 2u, 0x7d2u, 0x9ea50cu, 0x9ea530u); /* VA 0064e2c4 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf50u, self, 0x18u, 2u, 0x7d2u, 0x9ea504u, 0x9ea538u); /* VA 0064e2e9 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf60u, self, 0x18u, 2u, 0x7d2u, 0x9ea54cu, 0x9ea518u); /* VA 0064e30e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf70u, self, 0x18u, 2u, 0x7d2u, 0x9ea4dcu, 0x9ea4b8u); /* VA 0064e333 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf80u, self, 0x18u, 2u, 0x7d2u, 0x9ea520u, 0x9ea4fcu); /* VA 0064e358 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf90u, self, 0x18u, 2u, 0x7d2u, 0x9ea4f0u, 0x9ea4d4u); /* VA 0064e37d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xfa0u, self, 0x18u, 2u, 0x7d2u, 0x9ea4e4u, 0x9ea4c4u); /* VA 0064e3a2 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xfb0u, self, 0x18u, 2u, 0x7d2u, 0x9ea4ccu, 0x9ea4b0u); /* VA 0064e3c7 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xfc0u, self, 0x1eu, 2u, 0x7d2u, 0x9ea4d4u, 0x9ea540u); /* VA 0064e3ec */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xfd0u, self, 0x1eu, 2u, 0x7d2u, 0x9ea4fcu, 0x9ea54cu); /* VA 0064e411 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xfe0u, self, 0x1eu, 2u, 0x7d2u, 0x9ea518u, 0x9ea528u); /* VA 0064e436 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xff0u, self, 0x1eu, 2u, 0x7d2u, 0x9ea538u, 0x9ea4b8u); /* VA 0064e45b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1000u, self, 0x1eu, 2u, 0x7d2u, 0x9ea4c4u, 0x9ea50cu); /* VA 0064e480 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1010u, self, 0x1eu, 2u, 0x7d2u, 0x9ea530u, 0x9ea4dcu); /* VA 0064e4a5 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1020u, self, 0x1eu, 2u, 0x7d2u, 0x9ea4b0u, 0x9ea4e4u); /* VA 0064e4ca */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1030u, self, 0x1eu, 2u, 0x7d2u, 0x9ea4ccu, 0x9ea504u); /* VA 0064e4ef */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1040u, self, 0x1eu, 2u, 0x7d2u, 0x9ea520u, 0x9ea4f0u); /* VA 0064e514 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1050u, self, 7u, 3u, 0x7d2u, 0x9ea540u, 0x9ea530u); /* VA 0064e539 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1060u, self, 7u, 3u, 0x7d2u, 0x9ea4e4u, 0x9ea538u); /* VA 0064e55e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1070u, self, 7u, 3u, 0x7d2u, 0x9ea4dcu, 0x9ea520u); /* VA 0064e583 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1080u, self, 7u, 3u, 0x7d2u, 0x9ea54cu, 0x9ea50cu); /* VA 0064e5a8 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1090u, self, 7u, 3u, 0x7d2u, 0x9ea4b8u, 0x9ea4ccu); /* VA 0064e5cd */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x10a0u, self, 7u, 3u, 0x7d2u, 0x9ea4d4u, 0x9ea4fcu); /* VA 0064e5f2 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x10b0u, self, 7u, 3u, 0x7d2u, 0x9ea528u, 0x9ea4c4u); /* VA 0064e617 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x10c0u, self, 7u, 3u, 0x7d2u, 0x9ea4f0u, 0x9ea504u); /* VA 0064e63c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x10d0u, self, 7u, 3u, 0x7d2u, 0x9ea518u, 0x9ea4b0u); /* VA 0064e661 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x10e0u, self, 0xeu, 3u, 0x7d2u, 0x9ea4b0u, 0x9ea540u); /* VA 0064e686 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x10f0u, self, 0xeu, 3u, 0x7d2u, 0x9ea530u, 0x9ea54cu); /* VA 0064e6ab */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1100u, self, 0xeu, 3u, 0x7d2u, 0x9ea538u, 0x9ea518u); /* VA 0064e6d0 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1110u, self, 0xeu, 3u, 0x7d2u, 0x9ea4ccu, 0x9ea528u); /* VA 0064e6f5 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1120u, self, 0xeu, 3u, 0x7d2u, 0x9ea504u, 0x9ea4b8u); /* VA 0064e71a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1130u, self, 0xeu, 3u, 0x7d2u, 0x9ea4fcu, 0x9ea4dcu); /* VA 0064e73f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1140u, self, 0xeu, 3u, 0x7d2u, 0x9ea50cu, 0x9ea4e4u); /* VA 0064e764 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1150u, self, 0xeu, 3u, 0x7d2u, 0x9ea520u, 0x9ea4d4u); /* VA 0064e789 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1160u, self, 0xeu, 3u, 0x7d2u, 0x9ea4c4u, 0x9ea4f0u); /* VA 0064e7ae */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1170u, self, 0x15u, 3u, 0x7d2u, 0x9ea4d4u, 0x9ea530u); /* VA 0064e7d3 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1180u, self, 0x15u, 3u, 0x7d2u, 0x9ea540u, 0x9ea538u); /* VA 0064e7f8 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1190u, self, 0x15u, 3u, 0x7d2u, 0x9ea54cu, 0x9ea520u); /* VA 0064e81d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x11a0u, self, 0x15u, 3u, 0x7d2u, 0x9ea528u, 0x9ea50cu); /* VA 0064e842 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x11b0u, self, 0x15u, 3u, 0x7d2u, 0x9ea518u, 0x9ea4ccu); /* VA 0064e867 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x11c0u, self, 0x15u, 3u, 0x7d2u, 0x9ea4f0u, 0x9ea4fcu); /* VA 0064e88c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x11d0u, self, 0x15u, 3u, 0x7d2u, 0x9ea4b8u, 0x9ea4c4u); /* VA 0064e8b1 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x11e0u, self, 0x15u, 3u, 0x7d2u, 0x9ea4e4u, 0x9ea504u); /* VA 0064e8d6 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x11f0u, self, 0x15u, 3u, 0x7d2u, 0x9ea4dcu, 0x9ea4b0u); /* VA 0064e8fb */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1200u, self, 0x1cu, 3u, 0x7d2u, 0x9ea520u, 0x9ea540u); /* VA 0064e920 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1210u, self, 0x1cu, 3u, 0x7d2u, 0x9ea538u, 0x9ea54cu); /* VA 0064e945 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1220u, self, 0x1cu, 3u, 0x7d2u, 0x9ea4fcu, 0x9ea518u); /* VA 0064e96a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1230u, self, 0x1cu, 3u, 0x7d2u, 0x9ea504u, 0x9ea528u); /* VA 0064e98f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1240u, self, 0x1cu, 3u, 0x7d2u, 0x9ea530u, 0x9ea4b8u); /* VA 0064e9b4 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1250u, self, 0x1cu, 3u, 0x7d2u, 0x9ea4c4u, 0x9ea4dcu); /* VA 0064e9d9 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1260u, self, 0x1cu, 3u, 0x7d2u, 0x9ea4ccu, 0x9ea4e4u); /* VA 0064e9fe */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1270u, self, 0x1cu, 3u, 0x7d2u, 0x9ea50cu, 0x9ea4d4u); /* VA 0064ea23 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1280u, self, 0x1cu, 3u, 0x7d2u, 0x9ea4b0u, 0x9ea4f0u); /* VA 0064ea48 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1290u, self, 5u, 4u, 0x7d2u, 0x9ea528u, 0x9ea530u); /* VA 0064ea6d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x12a0u, self, 5u, 4u, 0x7d2u, 0x9ea4d4u, 0x9ea538u); /* VA 0064ea92 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x12b0u, self, 5u, 4u, 0x7d2u, 0x9ea4b8u, 0x9ea520u); /* VA 0064eab7 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x12c0u, self, 5u, 4u, 0x7d2u, 0x9ea4dcu, 0x9ea50cu); /* VA 0064eadc */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x12d0u, self, 5u, 4u, 0x7d2u, 0x9ea4f0u, 0x9ea4ccu); /* VA 0064eb01 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x12e0u, self, 5u, 4u, 0x7d2u, 0x9ea4e4u, 0x9ea4fcu); /* VA 0064eb26 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x12f0u, self, 5u, 4u, 0x7d2u, 0x9ea540u, 0x9ea4c4u); /* VA 0064eb4b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1300u, self, 5u, 4u, 0x7d2u, 0x9ea518u, 0x9ea504u); /* VA 0064eb70 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1310u, self, 5u, 4u, 0x7d2u, 0x9ea54cu, 0x9ea4b0u); /* VA 0064eb95 */
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

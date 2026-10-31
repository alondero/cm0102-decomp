/* Preferred-image VA 0x006545c0, base 0x00400000.
   CM0102/Game/cm0102.exe SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Every schedule call below preserves the original immediate pushes, receiver
   displacement and call order; each following receiver is a fresh DWORD read.
   Direct caller VA 0x00652f86 discards EAX before its next use. */
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

void CM_THISCALL cm_fn_006545c0(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x204];
    s32 half, factor, count;
    u32 allocation, receiver, table, cursor, index;

    frame.state = 0xffffffffu;
    frame.handler = 0x95bc4eu;
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
        cm_fn_00944cff(0x9ea654u, 0, 0, scratch + 0x104, scratch + 4);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text,
                       scratch + 0x104, scratch + 4, 0x3ceu);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0;
    } else {
    cm_fn_0068aff0(receiver + 0x0u, self, 2u, 8u, 0x7d1u, 0x9ea744u, 0x9ea750u); /* VA 006546c5 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x10u, self, 2u, 8u, 0x7d1u, 0x9ea730u, 0x9ea738u); /* VA 006546e7 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x20u, self, 2u, 8u, 0x7d1u, 0x9ea71cu, 0x9ea724u); /* VA 00654709 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x30u, self, 2u, 8u, 0x7d1u, 0x9ea704u, 0x9ea710u); /* VA 0065472b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x40u, self, 2u, 8u, 0x7d1u, 0x9ea6f4u, 0x9ea6fcu); /* VA 0065474d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x50u, self, 2u, 8u, 0x7d1u, 0x9ea6dcu, 0x9ea6e8u); /* VA 0065476f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x60u, self, 2u, 8u, 0x7d1u, 0x9ea6bcu, 0x9ea6ccu); /* VA 00654791 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x70u, self, 2u, 8u, 0x7d1u, 0x9ea6a8u, 0x9ea6b4u); /* VA 006547b3 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x80u, self, 2u, 8u, 0x7d1u, 0x9ea690u, 0x9ea69cu); /* VA 006547d8 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x90u, self, 9u, 8u, 0x7d1u, 0x9ea738u, 0x9ea6bcu); /* VA 006547fd */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa0u, self, 9u, 8u, 0x7d1u, 0x9ea6e8u, 0x9ea71cu); /* VA 00654822 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb0u, self, 9u, 8u, 0x7d1u, 0x9ea710u, 0x9ea6a8u); /* VA 00654847 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc0u, self, 9u, 8u, 0x7d1u, 0x9ea6b4u, 0x9ea744u); /* VA 0065486c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd0u, self, 9u, 8u, 0x7d1u, 0x9ea750u, 0x9ea730u); /* VA 00654891 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe0u, self, 9u, 8u, 0x7d1u, 0x9ea6fcu, 0x9ea6dcu); /* VA 006548b6 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf0u, self, 9u, 8u, 0x7d1u, 0x9ea69cu, 0x9ea704u); /* VA 006548db */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x100u, self, 9u, 8u, 0x7d1u, 0x9ea6ccu, 0x9ea6f4u); /* VA 00654900 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x110u, self, 9u, 8u, 0x7d1u, 0x9ea724u, 0x9ea690u); /* VA 00654925 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x120u, self, 0x10u, 8u, 0x7d1u, 0x9ea710u, 0x9ea69cu); /* VA 0065494a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x130u, self, 0x10u, 8u, 0x7d1u, 0x9ea744u, 0x9ea6dcu); /* VA 0065496f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x140u, self, 0x10u, 8u, 0x7d1u, 0x9ea730u, 0x9ea6ccu); /* VA 00654994 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x150u, self, 0x10u, 8u, 0x7d1u, 0x9ea71cu, 0x9ea738u); /* VA 006549b9 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x160u, self, 0x10u, 8u, 0x7d1u, 0x9ea704u, 0x9ea750u); /* VA 006549de */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x170u, self, 0x10u, 8u, 0x7d1u, 0x9ea6f4u, 0x9ea6e8u); /* VA 00654a03 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x180u, self, 0x10u, 8u, 0x7d1u, 0x9ea6bcu, 0x9ea6fcu); /* VA 00654a28 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x190u, self, 0x10u, 8u, 0x7d1u, 0x9ea6a8u, 0x9ea724u); /* VA 00654a4d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1a0u, self, 0x10u, 8u, 0x7d1u, 0x9ea690u, 0x9ea6b4u); /* VA 00654a72 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1b0u, self, 0x17u, 8u, 0x7d1u, 0x9ea738u, 0x9ea6a8u); /* VA 00654a97 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1c0u, self, 0x17u, 8u, 0x7d1u, 0x9ea6e8u, 0x9ea690u); /* VA 00654abc */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1d0u, self, 0x17u, 8u, 0x7d1u, 0x9ea6b4u, 0x9ea710u); /* VA 00654ae1 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1e0u, self, 0x17u, 8u, 0x7d1u, 0x9ea750u, 0x9ea6bcu); /* VA 00654b06 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1f0u, self, 0x17u, 8u, 0x7d1u, 0x9ea6fcu, 0x9ea730u); /* VA 00654b2b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x200u, self, 0x17u, 8u, 0x7d1u, 0x9ea69cu, 0x9ea71cu); /* VA 00654b50 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x210u, self, 0x17u, 8u, 0x7d1u, 0x9ea6dcu, 0x9ea6f4u); /* VA 00654b75 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x220u, self, 0x17u, 8u, 0x7d1u, 0x9ea6ccu, 0x9ea744u); /* VA 00654b9a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x230u, self, 0x17u, 8u, 0x7d1u, 0x9ea724u, 0x9ea704u); /* VA 00654bbf */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x240u, self, 0x1eu, 8u, 0x7d1u, 0x9ea710u, 0x9ea71cu); /* VA 00654be4 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x250u, self, 0x1eu, 8u, 0x7d1u, 0x9ea744u, 0x9ea724u); /* VA 00654c09 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x260u, self, 0x1eu, 8u, 0x7d1u, 0x9ea730u, 0x9ea6e8u); /* VA 00654c2e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x270u, self, 0x1eu, 8u, 0x7d1u, 0x9ea704u, 0x9ea6b4u); /* VA 00654c53 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x280u, self, 0x1eu, 8u, 0x7d1u, 0x9ea6f4u, 0x9ea750u); /* VA 00654c78 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x290u, self, 0x1eu, 8u, 0x7d1u, 0x9ea6fcu, 0x9ea6ccu); /* VA 00654c9d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x2a0u, self, 0x1eu, 8u, 0x7d1u, 0x9ea6bcu, 0x9ea69cu); /* VA 00654cc2 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x2b0u, self, 0x1eu, 8u, 0x7d1u, 0x9ea6a8u, 0x9ea6dcu); /* VA 00654ce7 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x2c0u, self, 0x1eu, 8u, 0x7d1u, 0x9ea690u, 0x9ea738u); /* VA 00654d0c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x2d0u, self, 7u, 9u, 0x7d1u, 0x9ea738u, 0x9ea710u); /* VA 00654d31 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x2e0u, self, 7u, 9u, 0x7d1u, 0x9ea6e8u, 0x9ea6a8u); /* VA 00654d56 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x2f0u, self, 7u, 9u, 0x7d1u, 0x9ea71cu, 0x9ea704u); /* VA 00654d7b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x300u, self, 7u, 9u, 0x7d1u, 0x9ea6f4u, 0x9ea730u); /* VA 00654da0 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x310u, self, 7u, 9u, 0x7d1u, 0x9ea750u, 0x9ea6fcu); /* VA 00654dc5 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x320u, self, 7u, 9u, 0x7d1u, 0x9ea69cu, 0x9ea744u); /* VA 00654dea */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x330u, self, 7u, 9u, 0x7d1u, 0x9ea6dcu, 0x9ea6bcu); /* VA 00654e0f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x340u, self, 7u, 9u, 0x7d1u, 0x9ea6ccu, 0x9ea690u); /* VA 00654e34 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x350u, self, 7u, 9u, 0x7d1u, 0x9ea724u, 0x9ea6b4u); /* VA 00654e59 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x360u, self, 0xeu, 9u, 0x7d1u, 0x9ea710u, 0x9ea6dcu); /* VA 00654e7e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x370u, self, 0xeu, 9u, 0x7d1u, 0x9ea744u, 0x9ea71cu); /* VA 00654ea3 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x380u, self, 0xeu, 9u, 0x7d1u, 0x9ea730u, 0x9ea724u); /* VA 00654ec8 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x390u, self, 0xeu, 9u, 0x7d1u, 0x9ea704u, 0x9ea6a8u); /* VA 00654eed */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x3a0u, self, 0xeu, 9u, 0x7d1u, 0x9ea6b4u, 0x9ea69cu); /* VA 00654f12 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x3b0u, self, 0xeu, 9u, 0x7d1u, 0x9ea750u, 0x9ea6ccu); /* VA 00654f37 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x3c0u, self, 0xeu, 9u, 0x7d1u, 0x9ea6fcu, 0x9ea738u); /* VA 00654f5c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x3d0u, self, 0xeu, 9u, 0x7d1u, 0x9ea6bcu, 0x9ea6e8u); /* VA 00654f81 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x3e0u, self, 0xeu, 9u, 0x7d1u, 0x9ea690u, 0x9ea6f4u); /* VA 00654fa6 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x3f0u, self, 0x15u, 9u, 0x7d1u, 0x9ea6e8u, 0x9ea750u); /* VA 00654fcb */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x400u, self, 0x15u, 9u, 0x7d1u, 0x9ea71cu, 0x9ea6b4u); /* VA 00654ff0 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x410u, self, 0x15u, 9u, 0x7d1u, 0x9ea6f4u, 0x9ea6bcu); /* VA 00655015 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x420u, self, 0x15u, 9u, 0x7d1u, 0x9ea69cu, 0x9ea6fcu); /* VA 0065503a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x430u, self, 0x15u, 9u, 0x7d1u, 0x9ea6dcu, 0x9ea738u); /* VA 0065505f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x440u, self, 0x15u, 9u, 0x7d1u, 0x9ea6a8u, 0x9ea730u); /* VA 00655084 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x450u, self, 0x15u, 9u, 0x7d1u, 0x9ea6ccu, 0x9ea704u); /* VA 006550a9 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x460u, self, 0x15u, 9u, 0x7d1u, 0x9ea690u, 0x9ea744u); /* VA 006550ce */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x470u, self, 0x15u, 9u, 0x7d1u, 0x9ea724u, 0x9ea710u); /* VA 006550f3 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x480u, self, 0x1cu, 9u, 0x7d1u, 0x9ea738u, 0x9ea6e8u); /* VA 00655118 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x490u, self, 0x1cu, 9u, 0x7d1u, 0x9ea710u, 0x9ea690u); /* VA 0065513d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x4a0u, self, 0x1cu, 9u, 0x7d1u, 0x9ea744u, 0x9ea6a8u); /* VA 00655162 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x4b0u, self, 0x1cu, 9u, 0x7d1u, 0x9ea730u, 0x9ea704u); /* VA 00655187 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x4c0u, self, 0x1cu, 9u, 0x7d1u, 0x9ea71cu, 0x9ea6f4u); /* VA 006551ac */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x4d0u, self, 0x1cu, 9u, 0x7d1u, 0x9ea6b4u, 0x9ea6fcu); /* VA 006551d1 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x4e0u, self, 0x1cu, 9u, 0x7d1u, 0x9ea750u, 0x9ea69cu); /* VA 006551f6 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x4f0u, self, 0x1cu, 9u, 0x7d1u, 0x9ea6ccu, 0x9ea6dcu); /* VA 0065521b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x500u, self, 0x1cu, 9u, 0x7d1u, 0x9ea724u, 0x9ea6bcu); /* VA 00655240 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x510u, self, 4u, 0xau, 0x7d1u, 0x9ea6e8u, 0x9ea6ccu); /* VA 00655265 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x520u, self, 4u, 0xau, 0x7d1u, 0x9ea704u, 0x9ea744u); /* VA 0065528a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x530u, self, 4u, 0xau, 0x7d1u, 0x9ea6f4u, 0x9ea724u); /* VA 006552af */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x540u, self, 4u, 0xau, 0x7d1u, 0x9ea6fcu, 0x9ea710u); /* VA 006552d4 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x550u, self, 4u, 0xau, 0x7d1u, 0x9ea69cu, 0x9ea738u); /* VA 006552f9 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x560u, self, 4u, 0xau, 0x7d1u, 0x9ea6dcu, 0x9ea750u); /* VA 0065531e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x570u, self, 4u, 0xau, 0x7d1u, 0x9ea6bcu, 0x9ea6b4u); /* VA 00655343 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x580u, self, 4u, 0xau, 0x7d1u, 0x9ea6a8u, 0x9ea71cu); /* VA 00655368 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x590u, self, 4u, 0xau, 0x7d1u, 0x9ea690u, 0x9ea730u); /* VA 0065538d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x5a0u, self, 0xbu, 0xau, 0x7d1u, 0x9ea738u, 0x9ea750u); /* VA 006553b2 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x5b0u, self, 0xbu, 0xau, 0x7d1u, 0x9ea710u, 0x9ea6ccu); /* VA 006553d7 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x5c0u, self, 0xbu, 0xau, 0x7d1u, 0x9ea744u, 0x9ea6f4u); /* VA 006553fc */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x5d0u, self, 0xbu, 0xau, 0x7d1u, 0x9ea730u, 0x9ea6bcu); /* VA 00655421 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x5e0u, self, 0xbu, 0xau, 0x7d1u, 0x9ea71cu, 0x9ea6fcu); /* VA 00655446 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x5f0u, self, 0xbu, 0xau, 0x7d1u, 0x9ea704u, 0x9ea690u); /* VA 0065546b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x600u, self, 0xbu, 0xau, 0x7d1u, 0x9ea6b4u, 0x9ea6dcu); /* VA 00655490 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x610u, self, 0xbu, 0xau, 0x7d1u, 0x9ea6a8u, 0x9ea69cu); /* VA 006554b5 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x620u, self, 0xbu, 0xau, 0x7d1u, 0x9ea724u, 0x9ea6e8u); /* VA 006554da */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x630u, self, 0x12u, 0xau, 0x7d1u, 0x9ea744u, 0x9ea710u); /* VA 006554ff */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x640u, self, 0x12u, 0xau, 0x7d1u, 0x9ea6f4u, 0x9ea704u); /* VA 00655524 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x650u, self, 0x12u, 0xau, 0x7d1u, 0x9ea750u, 0x9ea6b4u); /* VA 00655549 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x660u, self, 0x12u, 0xau, 0x7d1u, 0x9ea6fcu, 0x9ea6e8u); /* VA 0065556e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x670u, self, 0x12u, 0xau, 0x7d1u, 0x9ea69cu, 0x9ea724u); /* VA 00655593 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x680u, self, 0x12u, 0xau, 0x7d1u, 0x9ea6dcu, 0x9ea730u); /* VA 006555b8 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x690u, self, 0x12u, 0xau, 0x7d1u, 0x9ea6bcu, 0x9ea71cu); /* VA 006555dd */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x6a0u, self, 0x12u, 0xau, 0x7d1u, 0x9ea6ccu, 0x9ea738u); /* VA 00655602 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x6b0u, self, 0x12u, 0xau, 0x7d1u, 0x9ea690u, 0x9ea6a8u); /* VA 00655627 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x6c0u, self, 0x19u, 0xau, 0x7d1u, 0x9ea738u, 0x9ea6f4u); /* VA 0065564c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x6d0u, self, 0x19u, 0xau, 0x7d1u, 0x9ea6e8u, 0x9ea69cu); /* VA 00655671 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x6e0u, self, 0x19u, 0xau, 0x7d1u, 0x9ea710u, 0x9ea750u); /* VA 00655696 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x6f0u, self, 0x19u, 0xau, 0x7d1u, 0x9ea730u, 0x9ea744u); /* VA 006556bb */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x700u, self, 0x19u, 0xau, 0x7d1u, 0x9ea71cu, 0x9ea690u); /* VA 006556e0 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x710u, self, 0x19u, 0xau, 0x7d1u, 0x9ea704u, 0x9ea6bcu); /* VA 00655705 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x720u, self, 0x19u, 0xau, 0x7d1u, 0x9ea6b4u, 0x9ea6ccu); /* VA 0065572a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x730u, self, 0x19u, 0xau, 0x7d1u, 0x9ea6a8u, 0x9ea6fcu); /* VA 0065574f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x740u, self, 0x19u, 0xau, 0x7d1u, 0x9ea724u, 0x9ea6dcu); /* VA 00655774 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x750u, self, 2u, 0xbu, 0x7d1u, 0x9ea738u, 0x9ea724u); /* VA 00655799 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x760u, self, 2u, 0xbu, 0x7d1u, 0x9ea6e8u, 0x9ea6b4u); /* VA 006557be */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x770u, self, 2u, 0xbu, 0x7d1u, 0x9ea730u, 0x9ea69cu); /* VA 006557e3 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x780u, self, 2u, 0xbu, 0x7d1u, 0x9ea6f4u, 0x9ea710u); /* VA 00655808 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x790u, self, 2u, 0xbu, 0x7d1u, 0x9ea750u, 0x9ea6a8u); /* VA 0065582d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x7a0u, self, 2u, 0xbu, 0x7d1u, 0x9ea6fcu, 0x9ea704u); /* VA 00655852 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x7b0u, self, 2u, 0xbu, 0x7d1u, 0x9ea6dcu, 0x9ea690u); /* VA 00655877 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x7c0u, self, 2u, 0xbu, 0x7d1u, 0x9ea6bcu, 0x9ea744u); /* VA 0065589c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x7d0u, self, 2u, 0xbu, 0x7d1u, 0x9ea6ccu, 0x9ea71cu); /* VA 006558c1 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x7e0u, self, 9u, 0xbu, 0x7d1u, 0x9ea710u, 0x9ea730u); /* VA 006558e6 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x7f0u, self, 9u, 0xbu, 0x7d1u, 0x9ea744u, 0x9ea6e8u); /* VA 0065590b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x800u, self, 9u, 0xbu, 0x7d1u, 0x9ea71cu, 0x9ea750u); /* VA 00655930 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x810u, self, 9u, 0xbu, 0x7d1u, 0x9ea704u, 0x9ea6dcu); /* VA 00655955 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x820u, self, 9u, 0xbu, 0x7d1u, 0x9ea6b4u, 0x9ea738u); /* VA 0065597a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x830u, self, 9u, 0xbu, 0x7d1u, 0x9ea69cu, 0x9ea6f4u); /* VA 0065599f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x840u, self, 9u, 0xbu, 0x7d1u, 0x9ea6a8u, 0x9ea6bcu); /* VA 006559c4 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x850u, self, 9u, 0xbu, 0x7d1u, 0x9ea690u, 0x9ea6fcu); /* VA 006559e9 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x860u, self, 9u, 0xbu, 0x7d1u, 0x9ea724u, 0x9ea6ccu); /* VA 00655a0e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x870u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea738u, 0x9ea704u); /* VA 00655a33 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x880u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea6e8u, 0x9ea710u); /* VA 00655a58 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x890u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea730u, 0x9ea71cu); /* VA 00655a7d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x8a0u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea6f4u, 0x9ea6b4u); /* VA 00655aa2 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x8b0u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea750u, 0x9ea724u); /* VA 00655ac7 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x8c0u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea6fcu, 0x9ea744u); /* VA 00655aec */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x8d0u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea6dcu, 0x9ea69cu); /* VA 00655b11 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x8e0u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea6bcu, 0x9ea690u); /* VA 00655b36 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x8f0u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea6ccu, 0x9ea6a8u); /* VA 00655b5b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x900u, self, 0x17u, 0xbu, 0x7d1u, 0x9ea710u, 0x9ea6bcu); /* VA 00655b80 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x910u, self, 0x17u, 0xbu, 0x7d1u, 0x9ea744u, 0x9ea738u); /* VA 00655ba5 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x920u, self, 0x17u, 0xbu, 0x7d1u, 0x9ea71cu, 0x9ea6dcu); /* VA 00655bca */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x930u, self, 0x17u, 0xbu, 0x7d1u, 0x9ea704u, 0x9ea6e8u); /* VA 00655bef */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x940u, self, 0x17u, 0xbu, 0x7d1u, 0x9ea6b4u, 0x9ea730u); /* VA 00655c14 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x950u, self, 0x17u, 0xbu, 0x7d1u, 0x9ea69cu, 0x9ea6ccu); /* VA 00655c39 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x960u, self, 0x17u, 0xbu, 0x7d1u, 0x9ea6a8u, 0x9ea6f4u); /* VA 00655c5e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x970u, self, 0x17u, 0xbu, 0x7d1u, 0x9ea690u, 0x9ea750u); /* VA 00655c83 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x980u, self, 0x17u, 0xbu, 0x7d1u, 0x9ea724u, 0x9ea6fcu); /* VA 00655ca8 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x990u, self, 6u, 0u, 0x7d2u, 0x9ea750u, 0x9ea744u); /* VA 00655ccd */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x9a0u, self, 6u, 0u, 0x7d2u, 0x9ea738u, 0x9ea730u); /* VA 00655cf2 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x9b0u, self, 6u, 0u, 0x7d2u, 0x9ea724u, 0x9ea71cu); /* VA 00655d17 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x9c0u, self, 6u, 0u, 0x7d2u, 0x9ea710u, 0x9ea704u); /* VA 00655d3c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x9d0u, self, 6u, 0u, 0x7d2u, 0x9ea6fcu, 0x9ea6f4u); /* VA 00655d61 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x9e0u, self, 6u, 0u, 0x7d2u, 0x9ea6e8u, 0x9ea6dcu); /* VA 00655d86 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x9f0u, self, 6u, 0u, 0x7d2u, 0x9ea6ccu, 0x9ea6bcu); /* VA 00655dab */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa00u, self, 6u, 0u, 0x7d2u, 0x9ea6b4u, 0x9ea6a8u); /* VA 00655dd0 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa10u, self, 6u, 0u, 0x7d2u, 0x9ea69cu, 0x9ea690u); /* VA 00655df5 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa20u, self, 0xdu, 0u, 0x7d2u, 0x9ea6bcu, 0x9ea738u); /* VA 00655e1a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa30u, self, 0xdu, 0u, 0x7d2u, 0x9ea71cu, 0x9ea6e8u); /* VA 00655e3f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa40u, self, 0xdu, 0u, 0x7d2u, 0x9ea6a8u, 0x9ea710u); /* VA 00655e64 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa50u, self, 0xdu, 0u, 0x7d2u, 0x9ea744u, 0x9ea6b4u); /* VA 00655e89 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa60u, self, 0xdu, 0u, 0x7d2u, 0x9ea730u, 0x9ea750u); /* VA 00655eae */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa70u, self, 0xdu, 0u, 0x7d2u, 0x9ea6dcu, 0x9ea6fcu); /* VA 00655ed3 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa80u, self, 0xdu, 0u, 0x7d2u, 0x9ea704u, 0x9ea69cu); /* VA 00655ef8 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa90u, self, 0xdu, 0u, 0x7d2u, 0x9ea6f4u, 0x9ea6ccu); /* VA 00655f1d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xaa0u, self, 0xdu, 0u, 0x7d2u, 0x9ea690u, 0x9ea724u); /* VA 00655f42 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xab0u, self, 0x14u, 0u, 0x7d2u, 0x9ea69cu, 0x9ea710u); /* VA 00655f67 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xac0u, self, 0x14u, 0u, 0x7d2u, 0x9ea6dcu, 0x9ea744u); /* VA 00655f8c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xad0u, self, 0x14u, 0u, 0x7d2u, 0x9ea6ccu, 0x9ea730u); /* VA 00655fb1 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xae0u, self, 0x14u, 0u, 0x7d2u, 0x9ea738u, 0x9ea71cu); /* VA 00655fd6 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xaf0u, self, 0x14u, 0u, 0x7d2u, 0x9ea750u, 0x9ea704u); /* VA 00655ffb */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb00u, self, 0x14u, 0u, 0x7d2u, 0x9ea6e8u, 0x9ea6f4u); /* VA 00656020 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb10u, self, 0x14u, 0u, 0x7d2u, 0x9ea6fcu, 0x9ea6bcu); /* VA 00656045 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb20u, self, 0x14u, 0u, 0x7d2u, 0x9ea724u, 0x9ea6a8u); /* VA 0065606a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb30u, self, 0x14u, 0u, 0x7d2u, 0x9ea6b4u, 0x9ea690u); /* VA 0065608f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb40u, self, 0x1bu, 0u, 0x7d2u, 0x9ea6a8u, 0x9ea738u); /* VA 006560b4 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb50u, self, 0x1bu, 0u, 0x7d2u, 0x9ea690u, 0x9ea6e8u); /* VA 006560d9 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb60u, self, 0x1bu, 0u, 0x7d2u, 0x9ea710u, 0x9ea6b4u); /* VA 006560fe */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb70u, self, 0x1bu, 0u, 0x7d2u, 0x9ea6bcu, 0x9ea750u); /* VA 00656123 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb80u, self, 0x1bu, 0u, 0x7d2u, 0x9ea730u, 0x9ea6fcu); /* VA 00656148 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb90u, self, 0x1bu, 0u, 0x7d2u, 0x9ea71cu, 0x9ea69cu); /* VA 0065616d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xba0u, self, 0x1bu, 0u, 0x7d2u, 0x9ea6f4u, 0x9ea6dcu); /* VA 00656192 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xbb0u, self, 0x1bu, 0u, 0x7d2u, 0x9ea744u, 0x9ea6ccu); /* VA 006561b7 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xbc0u, self, 0x1bu, 0u, 0x7d2u, 0x9ea704u, 0x9ea724u); /* VA 006561dc */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xbd0u, self, 3u, 1u, 0x7d2u, 0x9ea71cu, 0x9ea710u); /* VA 00656201 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xbe0u, self, 3u, 1u, 0x7d2u, 0x9ea724u, 0x9ea744u); /* VA 00656226 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xbf0u, self, 3u, 1u, 0x7d2u, 0x9ea6e8u, 0x9ea730u); /* VA 0065624b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc00u, self, 3u, 1u, 0x7d2u, 0x9ea6b4u, 0x9ea704u); /* VA 00656270 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc10u, self, 3u, 1u, 0x7d2u, 0x9ea750u, 0x9ea6f4u); /* VA 00656295 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc20u, self, 3u, 1u, 0x7d2u, 0x9ea6ccu, 0x9ea6fcu); /* VA 006562ba */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc30u, self, 3u, 1u, 0x7d2u, 0x9ea69cu, 0x9ea6bcu); /* VA 006562df */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc40u, self, 3u, 1u, 0x7d2u, 0x9ea6dcu, 0x9ea6a8u); /* VA 00656304 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc50u, self, 3u, 1u, 0x7d2u, 0x9ea738u, 0x9ea690u); /* VA 00656329 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc60u, self, 0xau, 1u, 0x7d2u, 0x9ea710u, 0x9ea738u); /* VA 0065634e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc70u, self, 0xau, 1u, 0x7d2u, 0x9ea6a8u, 0x9ea6e8u); /* VA 00656373 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc80u, self, 0xau, 1u, 0x7d2u, 0x9ea704u, 0x9ea71cu); /* VA 00656398 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc90u, self, 0xau, 1u, 0x7d2u, 0x9ea730u, 0x9ea6f4u); /* VA 006563bd */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xca0u, self, 0xau, 1u, 0x7d2u, 0x9ea6fcu, 0x9ea750u); /* VA 006563e2 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xcb0u, self, 0xau, 1u, 0x7d2u, 0x9ea744u, 0x9ea69cu); /* VA 00656407 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xcc0u, self, 0xau, 1u, 0x7d2u, 0x9ea6bcu, 0x9ea6dcu); /* VA 0065642c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xcd0u, self, 0xau, 1u, 0x7d2u, 0x9ea690u, 0x9ea6ccu); /* VA 00656451 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xce0u, self, 0xau, 1u, 0x7d2u, 0x9ea6b4u, 0x9ea724u); /* VA 00656476 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xcf0u, self, 0x11u, 1u, 0x7d2u, 0x9ea6dcu, 0x9ea710u); /* VA 0065649b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd00u, self, 0x11u, 1u, 0x7d2u, 0x9ea71cu, 0x9ea744u); /* VA 006564c0 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd10u, self, 0x11u, 1u, 0x7d2u, 0x9ea724u, 0x9ea730u); /* VA 006564e5 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd20u, self, 0x11u, 1u, 0x7d2u, 0x9ea6a8u, 0x9ea704u); /* VA 0065650a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd30u, self, 0x11u, 1u, 0x7d2u, 0x9ea69cu, 0x9ea6b4u); /* VA 0065652f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd40u, self, 0x11u, 1u, 0x7d2u, 0x9ea6ccu, 0x9ea750u); /* VA 00656554 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd50u, self, 0x11u, 1u, 0x7d2u, 0x9ea738u, 0x9ea6fcu); /* VA 00656579 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd60u, self, 0x11u, 1u, 0x7d2u, 0x9ea6e8u, 0x9ea6bcu); /* VA 0065659e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd70u, self, 0x11u, 1u, 0x7d2u, 0x9ea6f4u, 0x9ea690u); /* VA 006565c3 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd80u, self, 0x18u, 1u, 0x7d2u, 0x9ea750u, 0x9ea6e8u); /* VA 006565e8 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd90u, self, 0x18u, 1u, 0x7d2u, 0x9ea6b4u, 0x9ea71cu); /* VA 0065660d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xda0u, self, 0x18u, 1u, 0x7d2u, 0x9ea6bcu, 0x9ea6f4u); /* VA 00656632 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xdb0u, self, 0x18u, 1u, 0x7d2u, 0x9ea6fcu, 0x9ea69cu); /* VA 00656657 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xdc0u, self, 0x18u, 1u, 0x7d2u, 0x9ea738u, 0x9ea6dcu); /* VA 0065667c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xdd0u, self, 0x18u, 1u, 0x7d2u, 0x9ea730u, 0x9ea6a8u); /* VA 006566a1 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xde0u, self, 0x18u, 1u, 0x7d2u, 0x9ea704u, 0x9ea6ccu); /* VA 006566c6 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xdf0u, self, 0x18u, 1u, 0x7d2u, 0x9ea744u, 0x9ea690u); /* VA 006566eb */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe00u, self, 0x18u, 1u, 0x7d2u, 0x9ea710u, 0x9ea724u); /* VA 00656710 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe10u, self, 3u, 2u, 0x7d2u, 0x9ea6e8u, 0x9ea738u); /* VA 00656735 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe20u, self, 3u, 2u, 0x7d2u, 0x9ea690u, 0x9ea710u); /* VA 0065675a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe30u, self, 3u, 2u, 0x7d2u, 0x9ea6a8u, 0x9ea744u); /* VA 0065677f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe40u, self, 3u, 2u, 0x7d2u, 0x9ea704u, 0x9ea730u); /* VA 006567a4 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe50u, self, 3u, 2u, 0x7d2u, 0x9ea6f4u, 0x9ea71cu); /* VA 006567c9 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe60u, self, 3u, 2u, 0x7d2u, 0x9ea6fcu, 0x9ea6b4u); /* VA 006567ee */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe70u, self, 3u, 2u, 0x7d2u, 0x9ea69cu, 0x9ea750u); /* VA 00656813 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe80u, self, 3u, 2u, 0x7d2u, 0x9ea6dcu, 0x9ea6ccu); /* VA 00656838 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe90u, self, 3u, 2u, 0x7d2u, 0x9ea6bcu, 0x9ea724u); /* VA 0065685d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xea0u, self, 0xau, 2u, 0x7d2u, 0x9ea6ccu, 0x9ea6e8u); /* VA 00656882 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xeb0u, self, 0xau, 2u, 0x7d2u, 0x9ea744u, 0x9ea704u); /* VA 006568a7 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xec0u, self, 0xau, 2u, 0x7d2u, 0x9ea724u, 0x9ea6f4u); /* VA 006568cc */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xed0u, self, 0xau, 2u, 0x7d2u, 0x9ea710u, 0x9ea6fcu); /* VA 006568f1 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xee0u, self, 0xau, 2u, 0x7d2u, 0x9ea738u, 0x9ea69cu); /* VA 00656916 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xef0u, self, 0xau, 2u, 0x7d2u, 0x9ea750u, 0x9ea6dcu); /* VA 0065693b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf00u, self, 0xau, 2u, 0x7d2u, 0x9ea6b4u, 0x9ea6bcu); /* VA 00656960 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf10u, self, 0xau, 2u, 0x7d2u, 0x9ea71cu, 0x9ea6a8u); /* VA 00656985 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf20u, self, 0xau, 2u, 0x7d2u, 0x9ea730u, 0x9ea690u); /* VA 006569aa */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf30u, self, 0x18u, 2u, 0x7d2u, 0x9ea750u, 0x9ea738u); /* VA 006569cf */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf40u, self, 0x18u, 2u, 0x7d2u, 0x9ea6ccu, 0x9ea710u); /* VA 006569f4 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf50u, self, 0x18u, 2u, 0x7d2u, 0x9ea6f4u, 0x9ea744u); /* VA 00656a19 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf60u, self, 0x18u, 2u, 0x7d2u, 0x9ea6bcu, 0x9ea730u); /* VA 00656a3e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf70u, self, 0x18u, 2u, 0x7d2u, 0x9ea6fcu, 0x9ea71cu); /* VA 00656a63 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf80u, self, 0x18u, 2u, 0x7d2u, 0x9ea690u, 0x9ea704u); /* VA 00656a88 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf90u, self, 0x18u, 2u, 0x7d2u, 0x9ea6dcu, 0x9ea6b4u); /* VA 00656aad */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xfa0u, self, 0x18u, 2u, 0x7d2u, 0x9ea69cu, 0x9ea6a8u); /* VA 00656ad2 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xfb0u, self, 0x18u, 2u, 0x7d2u, 0x9ea6e8u, 0x9ea724u); /* VA 00656af7 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xfc0u, self, 0x1eu, 2u, 0x7d2u, 0x9ea710u, 0x9ea744u); /* VA 00656b1c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xfd0u, self, 0x1eu, 2u, 0x7d2u, 0x9ea704u, 0x9ea6f4u); /* VA 00656b41 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xfe0u, self, 0x1eu, 2u, 0x7d2u, 0x9ea6b4u, 0x9ea750u); /* VA 00656b66 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xff0u, self, 0x1eu, 2u, 0x7d2u, 0x9ea6e8u, 0x9ea6fcu); /* VA 00656b8b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1000u, self, 0x1eu, 2u, 0x7d2u, 0x9ea724u, 0x9ea69cu); /* VA 00656bb0 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1010u, self, 0x1eu, 2u, 0x7d2u, 0x9ea730u, 0x9ea6dcu); /* VA 00656bd5 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1020u, self, 0x1eu, 2u, 0x7d2u, 0x9ea71cu, 0x9ea6bcu); /* VA 00656bfa */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1030u, self, 0x1eu, 2u, 0x7d2u, 0x9ea738u, 0x9ea6ccu); /* VA 00656c1f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1040u, self, 0x1eu, 2u, 0x7d2u, 0x9ea6a8u, 0x9ea690u); /* VA 00656c44 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1050u, self, 7u, 3u, 0x7d2u, 0x9ea6f4u, 0x9ea738u); /* VA 00656c69 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1060u, self, 7u, 3u, 0x7d2u, 0x9ea69cu, 0x9ea6e8u); /* VA 00656c8e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1070u, self, 7u, 3u, 0x7d2u, 0x9ea750u, 0x9ea710u); /* VA 00656cb3 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1080u, self, 7u, 3u, 0x7d2u, 0x9ea744u, 0x9ea730u); /* VA 00656cd8 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1090u, self, 7u, 3u, 0x7d2u, 0x9ea690u, 0x9ea71cu); /* VA 00656cfd */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x10a0u, self, 7u, 3u, 0x7d2u, 0x9ea6bcu, 0x9ea704u); /* VA 00656d22 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x10b0u, self, 7u, 3u, 0x7d2u, 0x9ea6ccu, 0x9ea6b4u); /* VA 00656d47 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x10c0u, self, 7u, 3u, 0x7d2u, 0x9ea6fcu, 0x9ea6a8u); /* VA 00656d6c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x10d0u, self, 7u, 3u, 0x7d2u, 0x9ea6dcu, 0x9ea724u); /* VA 00656d91 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x10e0u, self, 0xeu, 3u, 0x7d2u, 0x9ea724u, 0x9ea738u); /* VA 00656db6 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x10f0u, self, 0xeu, 3u, 0x7d2u, 0x9ea6b4u, 0x9ea6e8u); /* VA 00656ddb */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1100u, self, 0xeu, 3u, 0x7d2u, 0x9ea69cu, 0x9ea730u); /* VA 00656e00 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1110u, self, 0xeu, 3u, 0x7d2u, 0x9ea710u, 0x9ea6f4u); /* VA 00656e25 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1120u, self, 0xeu, 3u, 0x7d2u, 0x9ea6a8u, 0x9ea750u); /* VA 00656e4a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1130u, self, 0xeu, 3u, 0x7d2u, 0x9ea704u, 0x9ea6fcu); /* VA 00656e6f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1140u, self, 0xeu, 3u, 0x7d2u, 0x9ea690u, 0x9ea6dcu); /* VA 00656e94 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1150u, self, 0xeu, 3u, 0x7d2u, 0x9ea744u, 0x9ea6bcu); /* VA 00656eb9 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1160u, self, 0xeu, 3u, 0x7d2u, 0x9ea71cu, 0x9ea6ccu); /* VA 00656ede */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1170u, self, 0x15u, 3u, 0x7d2u, 0x9ea730u, 0x9ea710u); /* VA 00656f03 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1180u, self, 0x15u, 3u, 0x7d2u, 0x9ea6e8u, 0x9ea744u); /* VA 00656f28 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1190u, self, 0x15u, 3u, 0x7d2u, 0x9ea750u, 0x9ea71cu); /* VA 00656f4d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x11a0u, self, 0x15u, 3u, 0x7d2u, 0x9ea6dcu, 0x9ea704u); /* VA 00656f72 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x11b0u, self, 0x15u, 3u, 0x7d2u, 0x9ea738u, 0x9ea6b4u); /* VA 00656f97 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x11c0u, self, 0x15u, 3u, 0x7d2u, 0x9ea6f4u, 0x9ea69cu); /* VA 00656fbc */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x11d0u, self, 0x15u, 3u, 0x7d2u, 0x9ea6bcu, 0x9ea6a8u); /* VA 00656fe1 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x11e0u, self, 0x15u, 3u, 0x7d2u, 0x9ea6fcu, 0x9ea690u); /* VA 00657006 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x11f0u, self, 0x15u, 3u, 0x7d2u, 0x9ea6ccu, 0x9ea724u); /* VA 0065702b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1200u, self, 0x1cu, 3u, 0x7d2u, 0x9ea704u, 0x9ea738u); /* VA 00657050 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1210u, self, 0x1cu, 3u, 0x7d2u, 0x9ea710u, 0x9ea6e8u); /* VA 00657075 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1220u, self, 0x1cu, 3u, 0x7d2u, 0x9ea71cu, 0x9ea730u); /* VA 0065709a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1230u, self, 0x1cu, 3u, 0x7d2u, 0x9ea6b4u, 0x9ea6f4u); /* VA 006570bf */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1240u, self, 0x1cu, 3u, 0x7d2u, 0x9ea724u, 0x9ea750u); /* VA 006570e4 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1250u, self, 0x1cu, 3u, 0x7d2u, 0x9ea744u, 0x9ea6fcu); /* VA 00657109 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1260u, self, 0x1cu, 3u, 0x7d2u, 0x9ea69cu, 0x9ea6dcu); /* VA 0065712e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1270u, self, 0x1cu, 3u, 0x7d2u, 0x9ea690u, 0x9ea6bcu); /* VA 00657153 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1280u, self, 0x1cu, 3u, 0x7d2u, 0x9ea6a8u, 0x9ea6ccu); /* VA 00657178 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1290u, self, 5u, 4u, 0x7d2u, 0x9ea6bcu, 0x9ea710u); /* VA 0065719d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x12a0u, self, 5u, 4u, 0x7d2u, 0x9ea738u, 0x9ea744u); /* VA 006571c2 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x12b0u, self, 5u, 4u, 0x7d2u, 0x9ea6dcu, 0x9ea71cu); /* VA 006571e7 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x12c0u, self, 5u, 4u, 0x7d2u, 0x9ea6e8u, 0x9ea704u); /* VA 0065720c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x12d0u, self, 5u, 4u, 0x7d2u, 0x9ea730u, 0x9ea6b4u); /* VA 00657231 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x12e0u, self, 5u, 4u, 0x7d2u, 0x9ea6ccu, 0x9ea69cu); /* VA 00657256 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x12f0u, self, 5u, 4u, 0x7d2u, 0x9ea6f4u, 0x9ea6a8u); /* VA 0065727b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1300u, self, 5u, 4u, 0x7d2u, 0x9ea750u, 0x9ea690u); /* VA 006572a0 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1310u, self, 5u, 4u, 0x7d2u, 0x9ea6fcu, 0x9ea724u); /* VA 006572c5 */
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

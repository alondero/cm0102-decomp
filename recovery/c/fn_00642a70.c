/* Preferred-image VA 0x00642a70, base 0x00400000.
   CM0102/Game/cm0102.exe SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Every schedule call below preserves the original immediate pushes, receiver
   displacement and call order; each following receiver is a fresh DWORD read.
   Direct caller VA 0x006407c5 discards EAX before its next use. */
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

void CM_THISCALL cm_fn_00642a70(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x204];
    s32 half, factor, count;
    u32 allocation, receiver, table, cursor, index;

    frame.state = 0xffffffffu;
    frame.handler = 0x0095b9deu;
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
        cm_fn_00944cff(0x009ea2a8u, 0, 0, scratch + 0x104, scratch + 4);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text,
                       scratch + 0x104, scratch + 4, 0x60fu);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0;
    } else {
    cm_fn_0068aff0(receiver + 0x0u, self, 0x1cu, 7u, 0x7d1u, 0x9ea370u, 0x9ea378u); /* VA 00642b75 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x10u, self, 0x1cu, 7u, 0x7d1u, 0x9ea360u, 0x9ea368u); /* VA 00642b97 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x20u, self, 0x1cu, 7u, 0x7d1u, 0x9ea34cu, 0x9ea358u); /* VA 00642bb9 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x30u, self, 0x1cu, 7u, 0x7d1u, 0x9ea33cu, 0x9ea344u); /* VA 00642bdb */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x40u, self, 0x1cu, 7u, 0x7d1u, 0x9ea328u, 0x9ea334u); /* VA 00642bfd */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x50u, self, 0x1cu, 7u, 0x7d1u, 0x9ea314u, 0x9ea31cu); /* VA 00642c1f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x60u, self, 0x1cu, 7u, 0x7d1u, 0x9ea304u, 0x9ea30cu); /* VA 00642c41 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x70u, self, 0x1cu, 7u, 0x7d1u, 0x9ea2f4u, 0x9ea2fcu); /* VA 00642c63 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x80u, self, 0x1cu, 7u, 0x7d1u, 0x9ea2e4u, 0x9ea2ecu); /* VA 00642c88 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x90u, self, 9u, 8u, 0x7d1u, 0x9ea378u, 0x9ea328u); /* VA 00642cad */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa0u, self, 9u, 8u, 0x7d1u, 0x9ea358u, 0x9ea370u); /* VA 00642cd2 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb0u, self, 9u, 8u, 0x7d1u, 0x9ea368u, 0x9ea34cu); /* VA 00642cf7 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc0u, self, 9u, 8u, 0x7d1u, 0x9ea30cu, 0x9ea33cu); /* VA 00642d1c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd0u, self, 9u, 8u, 0x7d1u, 0x9ea344u, 0x9ea314u); /* VA 00642d41 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe0u, self, 9u, 8u, 0x7d1u, 0x9ea31cu, 0x9ea304u); /* VA 00642d66 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf0u, self, 9u, 8u, 0x7d1u, 0x9ea2ecu, 0x9ea2f4u); /* VA 00642d8b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x100u, self, 9u, 8u, 0x7d1u, 0x9ea2fcu, 0x9ea360u); /* VA 00642db0 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x110u, self, 9u, 8u, 0x7d1u, 0x9ea334u, 0x9ea2e4u); /* VA 00642dd5 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x120u, self, 0x10u, 8u, 0x7d1u, 0x9ea370u, 0x9ea30cu); /* VA 00642dfa */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x130u, self, 0x10u, 8u, 0x7d1u, 0x9ea360u, 0x9ea304u); /* VA 00642e1f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x140u, self, 0x10u, 8u, 0x7d1u, 0x9ea34cu, 0x9ea378u); /* VA 00642e44 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x150u, self, 0x10u, 8u, 0x7d1u, 0x9ea33cu, 0x9ea334u); /* VA 00642e69 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x160u, self, 0x10u, 8u, 0x7d1u, 0x9ea328u, 0x9ea358u); /* VA 00642e8e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x170u, self, 0x10u, 8u, 0x7d1u, 0x9ea314u, 0x9ea2fcu); /* VA 00642eb3 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x180u, self, 0x10u, 8u, 0x7d1u, 0x9ea31cu, 0x9ea2ecu); /* VA 00642ed8 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x190u, self, 0x10u, 8u, 0x7d1u, 0x9ea2f4u, 0x9ea368u); /* VA 00642efd */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1a0u, self, 0x10u, 8u, 0x7d1u, 0x9ea2e4u, 0x9ea344u); /* VA 00642f22 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1b0u, self, 0x17u, 8u, 0x7d1u, 0x9ea378u, 0x9ea2e4u); /* VA 00642f47 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1c0u, self, 0x17u, 8u, 0x7d1u, 0x9ea358u, 0x9ea31cu); /* VA 00642f6c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1d0u, self, 0x17u, 8u, 0x7d1u, 0x9ea304u, 0x9ea328u); /* VA 00642f91 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1e0u, self, 0x17u, 8u, 0x7d1u, 0x9ea368u, 0x9ea314u); /* VA 00642fb6 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1f0u, self, 0x17u, 8u, 0x7d1u, 0x9ea30cu, 0x9ea360u); /* VA 00642fdb */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x200u, self, 0x17u, 8u, 0x7d1u, 0x9ea344u, 0x9ea2f4u); /* VA 00643000 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x210u, self, 0x17u, 8u, 0x7d1u, 0x9ea2ecu, 0x9ea34cu); /* VA 00643025 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x220u, self, 0x17u, 8u, 0x7d1u, 0x9ea2fcu, 0x9ea33cu); /* VA 0064304a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x230u, self, 0x17u, 8u, 0x7d1u, 0x9ea334u, 0x9ea370u); /* VA 0064306f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x240u, self, 0x1eu, 8u, 0x7d1u, 0x9ea360u, 0x9ea378u); /* VA 00643094 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x250u, self, 0x1eu, 8u, 0x7d1u, 0x9ea34cu, 0x9ea334u); /* VA 006430b9 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x260u, self, 0x1eu, 8u, 0x7d1u, 0x9ea33cu, 0x9ea370u); /* VA 006430de */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x270u, self, 0x1eu, 8u, 0x7d1u, 0x9ea328u, 0x9ea2ecu); /* VA 00643103 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x280u, self, 0x1eu, 8u, 0x7d1u, 0x9ea314u, 0x9ea30cu); /* VA 00643128 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x290u, self, 0x1eu, 8u, 0x7d1u, 0x9ea344u, 0x9ea368u); /* VA 0064314d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x2a0u, self, 0x1eu, 8u, 0x7d1u, 0x9ea31cu, 0x9ea2fcu); /* VA 00643172 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x2b0u, self, 0x1eu, 8u, 0x7d1u, 0x9ea2f4u, 0x9ea358u); /* VA 00643197 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x2c0u, self, 0x1eu, 8u, 0x7d1u, 0x9ea2e4u, 0x9ea304u); /* VA 006431bc */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x2d0u, self, 0xau, 9u, 0x7d1u, 0x9ea378u, 0x9ea368u); /* VA 006431e1 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x2e0u, self, 0xau, 9u, 0x7d1u, 0x9ea370u, 0x9ea2f4u); /* VA 00643206 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x2f0u, self, 0xau, 9u, 0x7d1u, 0x9ea358u, 0x9ea314u); /* VA 0064322b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x300u, self, 0xau, 9u, 0x7d1u, 0x9ea34cu, 0x9ea328u); /* VA 00643250 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x310u, self, 0xau, 9u, 0x7d1u, 0x9ea33cu, 0x9ea2e4u); /* VA 00643275 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x320u, self, 0xau, 9u, 0x7d1u, 0x9ea304u, 0x9ea344u); /* VA 0064329a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x330u, self, 0xau, 9u, 0x7d1u, 0x9ea2ecu, 0x9ea360u); /* VA 006432bf */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x340u, self, 0xau, 9u, 0x7d1u, 0x9ea2fcu, 0x9ea30cu); /* VA 006432e4 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x350u, self, 0xau, 9u, 0x7d1u, 0x9ea334u, 0x9ea31cu); /* VA 00643309 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x360u, self, 0xeu, 9u, 0x7d1u, 0x9ea360u, 0x9ea358u); /* VA 0064332e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x370u, self, 0xeu, 9u, 0x7d1u, 0x9ea328u, 0x9ea2fcu); /* VA 00643353 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x380u, self, 0xeu, 9u, 0x7d1u, 0x9ea314u, 0x9ea378u); /* VA 00643378 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x390u, self, 0xeu, 9u, 0x7d1u, 0x9ea304u, 0x9ea34cu); /* VA 0064339d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x3a0u, self, 0xeu, 9u, 0x7d1u, 0x9ea368u, 0x9ea334u); /* VA 006433c2 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x3b0u, self, 0xeu, 9u, 0x7d1u, 0x9ea30cu, 0x9ea31cu); /* VA 006433e7 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x3c0u, self, 0xeu, 9u, 0x7d1u, 0x9ea344u, 0x9ea2ecu); /* VA 0064340c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x3d0u, self, 0xeu, 9u, 0x7d1u, 0x9ea2f4u, 0x9ea33cu); /* VA 00643431 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x3e0u, self, 0xeu, 9u, 0x7d1u, 0x9ea2e4u, 0x9ea370u); /* VA 00643456 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x3f0u, self, 0x15u, 9u, 0x7d1u, 0x9ea378u, 0x9ea2f4u); /* VA 0064347b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x400u, self, 0x15u, 9u, 0x7d1u, 0x9ea370u, 0x9ea328u); /* VA 006434a0 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x410u, self, 0x15u, 9u, 0x7d1u, 0x9ea358u, 0x9ea30cu); /* VA 006434c5 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x420u, self, 0x15u, 9u, 0x7d1u, 0x9ea34cu, 0x9ea2e4u); /* VA 006434ea */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x430u, self, 0x15u, 9u, 0x7d1u, 0x9ea33cu, 0x9ea368u); /* VA 0064350f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x440u, self, 0x15u, 9u, 0x7d1u, 0x9ea31cu, 0x9ea360u); /* VA 00643534 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x450u, self, 0x15u, 9u, 0x7d1u, 0x9ea2ecu, 0x9ea304u); /* VA 00643559 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x460u, self, 0x15u, 9u, 0x7d1u, 0x9ea2fcu, 0x9ea344u); /* VA 0064357e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x470u, self, 0x15u, 9u, 0x7d1u, 0x9ea334u, 0x9ea314u); /* VA 006435a3 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x480u, self, 0x1cu, 9u, 0x7d1u, 0x9ea360u, 0x9ea334u); /* VA 006435c8 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x490u, self, 0x1cu, 9u, 0x7d1u, 0x9ea358u, 0x9ea2fcu); /* VA 006435ed */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x4a0u, self, 0x1cu, 9u, 0x7d1u, 0x9ea328u, 0x9ea33cu); /* VA 00643612 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x4b0u, self, 0x1cu, 9u, 0x7d1u, 0x9ea304u, 0x9ea378u); /* VA 00643637 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x4c0u, self, 0x1cu, 9u, 0x7d1u, 0x9ea368u, 0x9ea370u); /* VA 0064365c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x4d0u, self, 0x1cu, 9u, 0x7d1u, 0x9ea30cu, 0x9ea2e4u); /* VA 00643681 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x4e0u, self, 0x1cu, 9u, 0x7d1u, 0x9ea344u, 0x9ea31cu); /* VA 006436a6 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x4f0u, self, 0x1cu, 9u, 0x7d1u, 0x9ea2ecu, 0x9ea314u); /* VA 006436cb */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x500u, self, 0x1cu, 9u, 0x7d1u, 0x9ea2f4u, 0x9ea34cu); /* VA 006436f0 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x510u, self, 4u, 0xau, 0x7d1u, 0x9ea378u, 0x9ea2ecu); /* VA 00643715 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x520u, self, 4u, 0xau, 0x7d1u, 0x9ea370u, 0x9ea34cu); /* VA 0064373a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x530u, self, 4u, 0xau, 0x7d1u, 0x9ea33cu, 0x9ea304u); /* VA 0064375f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x540u, self, 4u, 0xau, 0x7d1u, 0x9ea314u, 0x9ea360u); /* VA 00643784 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x550u, self, 4u, 0xau, 0x7d1u, 0x9ea30cu, 0x9ea344u); /* VA 006437a9 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x560u, self, 4u, 0xau, 0x7d1u, 0x9ea31cu, 0x9ea2f4u); /* VA 006437ce */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x570u, self, 4u, 0xau, 0x7d1u, 0x9ea2fcu, 0x9ea368u); /* VA 006437f3 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x580u, self, 4u, 0xau, 0x7d1u, 0x9ea334u, 0x9ea358u); /* VA 00643818 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x590u, self, 4u, 0xau, 0x7d1u, 0x9ea2e4u, 0x9ea328u); /* VA 0064383d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x5a0u, self, 0x12u, 0xau, 0x7d1u, 0x9ea378u, 0x9ea334u); /* VA 00643862 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x5b0u, self, 0x12u, 0xau, 0x7d1u, 0x9ea34cu, 0x9ea2fcu); /* VA 00643887 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x5c0u, self, 0x12u, 0xau, 0x7d1u, 0x9ea328u, 0x9ea30cu); /* VA 006438ac */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x5d0u, self, 0x12u, 0xau, 0x7d1u, 0x9ea304u, 0x9ea370u); /* VA 006438d1 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x5e0u, self, 0x12u, 0xau, 0x7d1u, 0x9ea368u, 0x9ea31cu); /* VA 006438f6 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x5f0u, self, 0x12u, 0xau, 0x7d1u, 0x9ea344u, 0x9ea360u); /* VA 0064391b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x600u, self, 0x12u, 0xau, 0x7d1u, 0x9ea2ecu, 0x9ea33cu); /* VA 00643940 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x610u, self, 0x12u, 0xau, 0x7d1u, 0x9ea2f4u, 0x9ea314u); /* VA 00643965 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x620u, self, 0x12u, 0xau, 0x7d1u, 0x9ea2e4u, 0x9ea358u); /* VA 0064398a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x630u, self, 0x19u, 0xau, 0x7d1u, 0x9ea370u, 0x9ea2ecu); /* VA 006439af */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x640u, self, 0x19u, 0xau, 0x7d1u, 0x9ea360u, 0x9ea2f4u); /* VA 006439d4 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x650u, self, 0x19u, 0xau, 0x7d1u, 0x9ea358u, 0x9ea344u); /* VA 006439f9 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x660u, self, 0x19u, 0xau, 0x7d1u, 0x9ea33cu, 0x9ea34cu); /* VA 00643a1e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x670u, self, 0x19u, 0xau, 0x7d1u, 0x9ea314u, 0x9ea328u); /* VA 00643a43 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x680u, self, 0x19u, 0xau, 0x7d1u, 0x9ea30cu, 0x9ea368u); /* VA 00643a68 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x690u, self, 0x19u, 0xau, 0x7d1u, 0x9ea31cu, 0x9ea378u); /* VA 00643a8d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x6a0u, self, 0x19u, 0xau, 0x7d1u, 0x9ea2fcu, 0x9ea2e4u); /* VA 00643ab2 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x6b0u, self, 0x19u, 0xau, 0x7d1u, 0x9ea334u, 0x9ea304u); /* VA 00643ad7 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x6c0u, self, 2u, 0xbu, 0x7d1u, 0x9ea378u, 0x9ea33cu); /* VA 00643afc */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x6d0u, self, 2u, 0xbu, 0x7d1u, 0x9ea370u, 0x9ea2fcu); /* VA 00643b21 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x6e0u, self, 2u, 0xbu, 0x7d1u, 0x9ea34cu, 0x9ea31cu); /* VA 00643b46 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x6f0u, self, 2u, 0xbu, 0x7d1u, 0x9ea328u, 0x9ea344u); /* VA 00643b6b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x700u, self, 2u, 0xbu, 0x7d1u, 0x9ea304u, 0x9ea314u); /* VA 00643b90 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x710u, self, 2u, 0xbu, 0x7d1u, 0x9ea368u, 0x9ea358u); /* VA 00643bb5 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x720u, self, 2u, 0xbu, 0x7d1u, 0x9ea2ecu, 0x9ea334u); /* VA 00643bda */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x730u, self, 2u, 0xbu, 0x7d1u, 0x9ea2f4u, 0x9ea30cu); /* VA 00643bff */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x740u, self, 2u, 0xbu, 0x7d1u, 0x9ea2e4u, 0x9ea360u); /* VA 00643c24 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x750u, self, 9u, 0xbu, 0x7d1u, 0x9ea360u, 0x9ea33cu); /* VA 00643c49 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x760u, self, 9u, 0xbu, 0x7d1u, 0x9ea358u, 0x9ea304u); /* VA 00643c6e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x770u, self, 9u, 0xbu, 0x7d1u, 0x9ea314u, 0x9ea34cu); /* VA 00643c93 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x780u, self, 9u, 0xbu, 0x7d1u, 0x9ea368u, 0x9ea328u); /* VA 00643cb8 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x790u, self, 9u, 0xbu, 0x7d1u, 0x9ea30cu, 0x9ea2ecu); /* VA 00643cdd */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x7a0u, self, 9u, 0xbu, 0x7d1u, 0x9ea344u, 0x9ea334u); /* VA 00643d02 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x7b0u, self, 9u, 0xbu, 0x7d1u, 0x9ea31cu, 0x9ea370u); /* VA 00643d27 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x7c0u, self, 9u, 0xbu, 0x7d1u, 0x9ea2fcu, 0x9ea378u); /* VA 00643d4c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x7d0u, self, 9u, 0xbu, 0x7d1u, 0x9ea2f4u, 0x9ea2e4u); /* VA 00643d71 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x7e0u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea378u, 0x9ea30cu); /* VA 00643d96 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x7f0u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea370u, 0x9ea344u); /* VA 00643dbb */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x800u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea34cu, 0x9ea360u); /* VA 00643de0 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x810u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea33cu, 0x9ea358u); /* VA 00643e05 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x820u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea328u, 0x9ea31cu); /* VA 00643e2a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x830u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea304u, 0x9ea2fcu); /* VA 00643e4f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x840u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea2ecu, 0x9ea368u); /* VA 00643e74 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x850u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea334u, 0x9ea2f4u); /* VA 00643e99 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x860u, self, 0x10u, 0xbu, 0x7d1u, 0x9ea2e4u, 0x9ea314u); /* VA 00643ebe */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x870u, self, 6u, 0u, 0x7d2u, 0x9ea360u, 0x9ea328u); /* VA 00643ee3 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x880u, self, 6u, 0u, 0x7d2u, 0x9ea358u, 0x9ea2ecu); /* VA 00643f08 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x890u, self, 6u, 0u, 0x7d2u, 0x9ea314u, 0x9ea370u); /* VA 00643f2d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x8a0u, self, 6u, 0u, 0x7d2u, 0x9ea368u, 0x9ea2e4u); /* VA 00643f52 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x8b0u, self, 6u, 0u, 0x7d2u, 0x9ea30cu, 0x9ea34cu); /* VA 00643f77 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x8c0u, self, 6u, 0u, 0x7d2u, 0x9ea344u, 0x9ea378u); /* VA 00643f9c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x8d0u, self, 6u, 0u, 0x7d2u, 0x9ea31cu, 0x9ea33cu); /* VA 00643fc1 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x8e0u, self, 6u, 0u, 0x7d2u, 0x9ea2fcu, 0x9ea334u); /* VA 00643fe6 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x8f0u, self, 6u, 0u, 0x7d2u, 0x9ea2f4u, 0x9ea304u); /* VA 0064400b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x900u, self, 0xdu, 0u, 0x7d2u, 0x9ea378u, 0x9ea358u); /* VA 00644030 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x910u, self, 0xdu, 0u, 0x7d2u, 0x9ea370u, 0x9ea360u); /* VA 00644055 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x920u, self, 0xdu, 0u, 0x7d2u, 0x9ea34cu, 0x9ea344u); /* VA 0064407a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x930u, self, 0xdu, 0u, 0x7d2u, 0x9ea33cu, 0x9ea314u); /* VA 0064409f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x940u, self, 0xdu, 0u, 0x7d2u, 0x9ea328u, 0x9ea2f4u); /* VA 006440c4 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x950u, self, 0xdu, 0u, 0x7d2u, 0x9ea304u, 0x9ea368u); /* VA 006440e9 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x960u, self, 0xdu, 0u, 0x7d2u, 0x9ea2ecu, 0x9ea2fcu); /* VA 0064410e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x970u, self, 0xdu, 0u, 0x7d2u, 0x9ea334u, 0x9ea30cu); /* VA 00644133 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x980u, self, 0xdu, 0u, 0x7d2u, 0x9ea2e4u, 0x9ea31cu); /* VA 00644158 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x990u, self, 0x14u, 0u, 0x7d2u, 0x9ea378u, 0x9ea370u); /* VA 0064417d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x9a0u, self, 0x14u, 0u, 0x7d2u, 0x9ea368u, 0x9ea360u); /* VA 006441a2 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x9b0u, self, 0x14u, 0u, 0x7d2u, 0x9ea358u, 0x9ea34cu); /* VA 006441c7 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x9c0u, self, 0x14u, 0u, 0x7d2u, 0x9ea344u, 0x9ea33cu); /* VA 006441ec */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x9d0u, self, 0x14u, 0u, 0x7d2u, 0x9ea334u, 0x9ea328u); /* VA 00644211 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x9e0u, self, 0x14u, 0u, 0x7d2u, 0x9ea31cu, 0x9ea314u); /* VA 00644236 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x9f0u, self, 0x14u, 0u, 0x7d2u, 0x9ea30cu, 0x9ea304u); /* VA 0064425b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa00u, self, 0x14u, 0u, 0x7d2u, 0x9ea2fcu, 0x9ea2f4u); /* VA 00644280 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa10u, self, 0x14u, 0u, 0x7d2u, 0x9ea2ecu, 0x9ea2e4u); /* VA 006442a5 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa20u, self, 0x1bu, 0u, 0x7d2u, 0x9ea328u, 0x9ea378u); /* VA 006442ca */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa30u, self, 0x1bu, 0u, 0x7d2u, 0x9ea370u, 0x9ea358u); /* VA 006442ef */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa40u, self, 0x1bu, 0u, 0x7d2u, 0x9ea34cu, 0x9ea368u); /* VA 00644314 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa50u, self, 0x1bu, 0u, 0x7d2u, 0x9ea33cu, 0x9ea30cu); /* VA 00644339 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa60u, self, 0x1bu, 0u, 0x7d2u, 0x9ea314u, 0x9ea344u); /* VA 0064435e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa70u, self, 0x1bu, 0u, 0x7d2u, 0x9ea304u, 0x9ea31cu); /* VA 00644383 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa80u, self, 0x1bu, 0u, 0x7d2u, 0x9ea2f4u, 0x9ea2ecu); /* VA 006443a8 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xa90u, self, 0x1bu, 0u, 0x7d2u, 0x9ea360u, 0x9ea2fcu); /* VA 006443cd */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xaa0u, self, 0x1bu, 0u, 0x7d2u, 0x9ea2e4u, 0x9ea334u); /* VA 006443f2 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xab0u, self, 3u, 1u, 0x7d2u, 0x9ea30cu, 0x9ea370u); /* VA 00644417 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xac0u, self, 3u, 1u, 0x7d2u, 0x9ea304u, 0x9ea360u); /* VA 0064443c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xad0u, self, 3u, 1u, 0x7d2u, 0x9ea378u, 0x9ea34cu); /* VA 00644461 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xae0u, self, 3u, 1u, 0x7d2u, 0x9ea334u, 0x9ea33cu); /* VA 00644486 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xaf0u, self, 3u, 1u, 0x7d2u, 0x9ea358u, 0x9ea328u); /* VA 006444ab */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb00u, self, 3u, 1u, 0x7d2u, 0x9ea2fcu, 0x9ea314u); /* VA 006444d0 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb10u, self, 3u, 1u, 0x7d2u, 0x9ea2ecu, 0x9ea31cu); /* VA 006444f5 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb20u, self, 3u, 1u, 0x7d2u, 0x9ea368u, 0x9ea2f4u); /* VA 0064451a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb30u, self, 3u, 1u, 0x7d2u, 0x9ea344u, 0x9ea2e4u); /* VA 0064453f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb40u, self, 0xau, 1u, 0x7d2u, 0x9ea2e4u, 0x9ea378u); /* VA 00644564 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb50u, self, 0xau, 1u, 0x7d2u, 0x9ea31cu, 0x9ea358u); /* VA 00644589 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb60u, self, 0xau, 1u, 0x7d2u, 0x9ea328u, 0x9ea304u); /* VA 006445ae */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb70u, self, 0xau, 1u, 0x7d2u, 0x9ea314u, 0x9ea368u); /* VA 006445d3 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb80u, self, 0xau, 1u, 0x7d2u, 0x9ea360u, 0x9ea30cu); /* VA 006445f8 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xb90u, self, 0xau, 1u, 0x7d2u, 0x9ea2f4u, 0x9ea344u); /* VA 0064461d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xba0u, self, 0xau, 1u, 0x7d2u, 0x9ea34cu, 0x9ea2ecu); /* VA 00644642 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xbb0u, self, 0xau, 1u, 0x7d2u, 0x9ea33cu, 0x9ea2fcu); /* VA 00644667 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xbc0u, self, 0xau, 1u, 0x7d2u, 0x9ea370u, 0x9ea334u); /* VA 0064468c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xbd0u, self, 0x11u, 1u, 0x7d2u, 0x9ea378u, 0x9ea360u); /* VA 006446b1 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xbe0u, self, 0x11u, 1u, 0x7d2u, 0x9ea334u, 0x9ea34cu); /* VA 006446d6 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xbf0u, self, 0x11u, 1u, 0x7d2u, 0x9ea370u, 0x9ea33cu); /* VA 006446fb */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc00u, self, 0x11u, 1u, 0x7d2u, 0x9ea2ecu, 0x9ea328u); /* VA 00644720 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc10u, self, 0x11u, 1u, 0x7d2u, 0x9ea30cu, 0x9ea314u); /* VA 00644745 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc20u, self, 0x11u, 1u, 0x7d2u, 0x9ea368u, 0x9ea344u); /* VA 0064476a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc30u, self, 0x11u, 1u, 0x7d2u, 0x9ea2fcu, 0x9ea31cu); /* VA 0064478f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc40u, self, 0x11u, 1u, 0x7d2u, 0x9ea358u, 0x9ea2f4u); /* VA 006447b4 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc50u, self, 0x11u, 1u, 0x7d2u, 0x9ea304u, 0x9ea2e4u); /* VA 006447d9 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc60u, self, 0x18u, 1u, 0x7d2u, 0x9ea368u, 0x9ea378u); /* VA 006447fe */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc70u, self, 0x18u, 1u, 0x7d2u, 0x9ea2f4u, 0x9ea370u); /* VA 00644823 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc80u, self, 0x18u, 1u, 0x7d2u, 0x9ea314u, 0x9ea358u); /* VA 00644848 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xc90u, self, 0x18u, 1u, 0x7d2u, 0x9ea328u, 0x9ea34cu); /* VA 0064486d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xca0u, self, 0x18u, 1u, 0x7d2u, 0x9ea2e4u, 0x9ea33cu); /* VA 00644892 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xcb0u, self, 0x18u, 1u, 0x7d2u, 0x9ea344u, 0x9ea304u); /* VA 006448b7 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xcc0u, self, 0x18u, 1u, 0x7d2u, 0x9ea360u, 0x9ea2ecu); /* VA 006448dc */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xcd0u, self, 0x18u, 1u, 0x7d2u, 0x9ea30cu, 0x9ea2fcu); /* VA 00644901 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xce0u, self, 0x18u, 1u, 0x7d2u, 0x9ea31cu, 0x9ea334u); /* VA 00644926 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xcf0u, self, 3u, 2u, 0x7d2u, 0x9ea358u, 0x9ea360u); /* VA 0064494b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd00u, self, 3u, 2u, 0x7d2u, 0x9ea2fcu, 0x9ea328u); /* VA 00644970 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd10u, self, 3u, 2u, 0x7d2u, 0x9ea378u, 0x9ea314u); /* VA 00644995 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd20u, self, 3u, 2u, 0x7d2u, 0x9ea34cu, 0x9ea304u); /* VA 006449ba */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd30u, self, 3u, 2u, 0x7d2u, 0x9ea334u, 0x9ea368u); /* VA 006449df */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd40u, self, 3u, 2u, 0x7d2u, 0x9ea31cu, 0x9ea30cu); /* VA 00644a04 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd50u, self, 3u, 2u, 0x7d2u, 0x9ea2ecu, 0x9ea344u); /* VA 00644a29 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd60u, self, 3u, 2u, 0x7d2u, 0x9ea33cu, 0x9ea2f4u); /* VA 00644a4e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd70u, self, 3u, 2u, 0x7d2u, 0x9ea370u, 0x9ea2e4u); /* VA 00644a73 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd80u, self, 0xau, 2u, 0x7d2u, 0x9ea2f4u, 0x9ea378u); /* VA 00644a98 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xd90u, self, 0xau, 2u, 0x7d2u, 0x9ea328u, 0x9ea370u); /* VA 00644abd */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xda0u, self, 0xau, 2u, 0x7d2u, 0x9ea30cu, 0x9ea358u); /* VA 00644ae2 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xdb0u, self, 0xau, 2u, 0x7d2u, 0x9ea2e4u, 0x9ea34cu); /* VA 00644b07 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xdc0u, self, 0xau, 2u, 0x7d2u, 0x9ea368u, 0x9ea33cu); /* VA 00644b2c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xdd0u, self, 0xau, 2u, 0x7d2u, 0x9ea360u, 0x9ea31cu); /* VA 00644b51 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xde0u, self, 0xau, 2u, 0x7d2u, 0x9ea304u, 0x9ea2ecu); /* VA 00644b76 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xdf0u, self, 0xau, 2u, 0x7d2u, 0x9ea344u, 0x9ea2fcu); /* VA 00644b9b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe00u, self, 0xau, 2u, 0x7d2u, 0x9ea314u, 0x9ea334u); /* VA 00644bc0 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe10u, self, 0x11u, 2u, 0x7d2u, 0x9ea334u, 0x9ea360u); /* VA 00644be5 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe20u, self, 0x11u, 2u, 0x7d2u, 0x9ea2fcu, 0x9ea358u); /* VA 00644c0a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe30u, self, 0x11u, 2u, 0x7d2u, 0x9ea33cu, 0x9ea328u); /* VA 00644c2f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe40u, self, 0x11u, 2u, 0x7d2u, 0x9ea378u, 0x9ea304u); /* VA 00644c54 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe50u, self, 0x11u, 2u, 0x7d2u, 0x9ea370u, 0x9ea368u); /* VA 00644c79 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe60u, self, 0x11u, 2u, 0x7d2u, 0x9ea2e4u, 0x9ea30cu); /* VA 00644c9e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe70u, self, 0x11u, 2u, 0x7d2u, 0x9ea31cu, 0x9ea344u); /* VA 00644cc3 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe80u, self, 0x11u, 2u, 0x7d2u, 0x9ea314u, 0x9ea2ecu); /* VA 00644ce8 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xe90u, self, 0x11u, 2u, 0x7d2u, 0x9ea34cu, 0x9ea2f4u); /* VA 00644d0d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xea0u, self, 0x18u, 2u, 0x7d2u, 0x9ea2ecu, 0x9ea378u); /* VA 00644d32 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xeb0u, self, 0x18u, 2u, 0x7d2u, 0x9ea34cu, 0x9ea370u); /* VA 00644d57 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xec0u, self, 0x18u, 2u, 0x7d2u, 0x9ea304u, 0x9ea33cu); /* VA 00644d7c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xed0u, self, 0x18u, 2u, 0x7d2u, 0x9ea360u, 0x9ea314u); /* VA 00644da1 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xee0u, self, 0x18u, 2u, 0x7d2u, 0x9ea344u, 0x9ea30cu); /* VA 00644dc6 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xef0u, self, 0x18u, 2u, 0x7d2u, 0x9ea2f4u, 0x9ea31cu); /* VA 00644deb */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf00u, self, 0x18u, 2u, 0x7d2u, 0x9ea368u, 0x9ea2fcu); /* VA 00644e10 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf10u, self, 0x18u, 2u, 0x7d2u, 0x9ea358u, 0x9ea334u); /* VA 00644e35 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf20u, self, 0x18u, 2u, 0x7d2u, 0x9ea328u, 0x9ea2e4u); /* VA 00644e5a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf30u, self, 0x1eu, 2u, 0x7d2u, 0x9ea334u, 0x9ea378u); /* VA 00644e7f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf40u, self, 0x1eu, 2u, 0x7d2u, 0x9ea2fcu, 0x9ea34cu); /* VA 00644ea4 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf50u, self, 0x1eu, 2u, 0x7d2u, 0x9ea30cu, 0x9ea328u); /* VA 00644ec9 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf60u, self, 0x1eu, 2u, 0x7d2u, 0x9ea370u, 0x9ea304u); /* VA 00644eee */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf70u, self, 0x1eu, 2u, 0x7d2u, 0x9ea31cu, 0x9ea368u); /* VA 00644f13 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf80u, self, 0x1eu, 2u, 0x7d2u, 0x9ea360u, 0x9ea344u); /* VA 00644f38 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xf90u, self, 0x1eu, 2u, 0x7d2u, 0x9ea33cu, 0x9ea2ecu); /* VA 00644f5d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xfa0u, self, 0x1eu, 2u, 0x7d2u, 0x9ea314u, 0x9ea2f4u); /* VA 00644f82 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xfb0u, self, 0x1eu, 2u, 0x7d2u, 0x9ea358u, 0x9ea2e4u); /* VA 00644fa7 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xfc0u, self, 7u, 3u, 0x7d2u, 0x9ea2ecu, 0x9ea370u); /* VA 00644fcc */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xfd0u, self, 7u, 3u, 0x7d2u, 0x9ea2f4u, 0x9ea360u); /* VA 00644ff1 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xfe0u, self, 7u, 3u, 0x7d2u, 0x9ea344u, 0x9ea358u); /* VA 00645016 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0xff0u, self, 7u, 3u, 0x7d2u, 0x9ea34cu, 0x9ea33cu); /* VA 0064503b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1000u, self, 7u, 3u, 0x7d2u, 0x9ea328u, 0x9ea314u); /* VA 00645060 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1010u, self, 7u, 3u, 0x7d2u, 0x9ea368u, 0x9ea30cu); /* VA 00645085 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1020u, self, 7u, 3u, 0x7d2u, 0x9ea378u, 0x9ea31cu); /* VA 006450aa */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1030u, self, 7u, 3u, 0x7d2u, 0x9ea2e4u, 0x9ea2fcu); /* VA 006450cf */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1040u, self, 7u, 3u, 0x7d2u, 0x9ea304u, 0x9ea334u); /* VA 006450f4 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1050u, self, 0xeu, 3u, 0x7d2u, 0x9ea33cu, 0x9ea378u); /* VA 00645119 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1060u, self, 0xeu, 3u, 0x7d2u, 0x9ea2fcu, 0x9ea370u); /* VA 0064513e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1070u, self, 0xeu, 3u, 0x7d2u, 0x9ea31cu, 0x9ea34cu); /* VA 00645163 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1080u, self, 0xeu, 3u, 0x7d2u, 0x9ea344u, 0x9ea328u); /* VA 00645188 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1090u, self, 0xeu, 3u, 0x7d2u, 0x9ea314u, 0x9ea304u); /* VA 006451ad */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x10a0u, self, 0xeu, 3u, 0x7d2u, 0x9ea358u, 0x9ea368u); /* VA 006451d2 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x10b0u, self, 0xeu, 3u, 0x7d2u, 0x9ea334u, 0x9ea2ecu); /* VA 006451f7 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x10c0u, self, 0xeu, 3u, 0x7d2u, 0x9ea30cu, 0x9ea2f4u); /* VA 0064521c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x10d0u, self, 0xeu, 3u, 0x7d2u, 0x9ea360u, 0x9ea2e4u); /* VA 00645241 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x10e0u, self, 0x15u, 3u, 0x7d2u, 0x9ea33cu, 0x9ea360u); /* VA 00645266 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x10f0u, self, 0x15u, 3u, 0x7d2u, 0x9ea304u, 0x9ea358u); /* VA 0064528b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1100u, self, 0x15u, 3u, 0x7d2u, 0x9ea34cu, 0x9ea314u); /* VA 006452b0 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1110u, self, 0x15u, 3u, 0x7d2u, 0x9ea328u, 0x9ea368u); /* VA 006452d5 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1120u, self, 0x15u, 3u, 0x7d2u, 0x9ea2ecu, 0x9ea30cu); /* VA 006452fa */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1130u, self, 0x15u, 3u, 0x7d2u, 0x9ea334u, 0x9ea344u); /* VA 0064531f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1140u, self, 0x15u, 3u, 0x7d2u, 0x9ea370u, 0x9ea31cu); /* VA 00645344 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1150u, self, 0x15u, 3u, 0x7d2u, 0x9ea378u, 0x9ea2fcu); /* VA 00645369 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1160u, self, 0x15u, 3u, 0x7d2u, 0x9ea2e4u, 0x9ea2f4u); /* VA 0064538e */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1170u, self, 0x1cu, 3u, 0x7d2u, 0x9ea30cu, 0x9ea378u); /* VA 006453b3 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1180u, self, 0x1cu, 3u, 0x7d2u, 0x9ea344u, 0x9ea370u); /* VA 006453d8 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1190u, self, 0x1cu, 3u, 0x7d2u, 0x9ea360u, 0x9ea34cu); /* VA 006453fd */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x11a0u, self, 0x1cu, 3u, 0x7d2u, 0x9ea358u, 0x9ea33cu); /* VA 00645422 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x11b0u, self, 0x1cu, 3u, 0x7d2u, 0x9ea31cu, 0x9ea328u); /* VA 00645447 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x11c0u, self, 0x1cu, 3u, 0x7d2u, 0x9ea2fcu, 0x9ea304u); /* VA 0064546c */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x11d0u, self, 0x1cu, 3u, 0x7d2u, 0x9ea368u, 0x9ea2ecu); /* VA 00645491 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x11e0u, self, 0x1cu, 3u, 0x7d2u, 0x9ea2f4u, 0x9ea334u); /* VA 006454b6 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x11f0u, self, 0x1cu, 3u, 0x7d2u, 0x9ea314u, 0x9ea2e4u); /* VA 006454db */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1200u, self, 5u, 4u, 0x7d2u, 0x9ea328u, 0x9ea360u); /* VA 00645500 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1210u, self, 5u, 4u, 0x7d2u, 0x9ea2ecu, 0x9ea358u); /* VA 00645525 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1220u, self, 5u, 4u, 0x7d2u, 0x9ea370u, 0x9ea314u); /* VA 0064554a */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1230u, self, 5u, 4u, 0x7d2u, 0x9ea2e4u, 0x9ea368u); /* VA 0064556f */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1240u, self, 5u, 4u, 0x7d2u, 0x9ea34cu, 0x9ea30cu); /* VA 00645594 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1250u, self, 5u, 4u, 0x7d2u, 0x9ea378u, 0x9ea344u); /* VA 006455b9 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1260u, self, 5u, 4u, 0x7d2u, 0x9ea33cu, 0x9ea31cu); /* VA 006455de */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1270u, self, 5u, 4u, 0x7d2u, 0x9ea334u, 0x9ea2fcu); /* VA 00645603 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1280u, self, 5u, 4u, 0x7d2u, 0x9ea304u, 0x9ea2f4u); /* VA 00645628 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1290u, self, 0xcu, 4u, 0x7d2u, 0x9ea358u, 0x9ea378u); /* VA 0064564d */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x12a0u, self, 0xcu, 4u, 0x7d2u, 0x9ea360u, 0x9ea370u); /* VA 00645672 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x12b0u, self, 0xcu, 4u, 0x7d2u, 0x9ea344u, 0x9ea34cu); /* VA 00645697 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x12c0u, self, 0xcu, 4u, 0x7d2u, 0x9ea314u, 0x9ea33cu); /* VA 006456bc */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x12d0u, self, 0xcu, 4u, 0x7d2u, 0x9ea2f4u, 0x9ea328u); /* VA 006456e1 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x12e0u, self, 0xcu, 4u, 0x7d2u, 0x9ea368u, 0x9ea304u); /* VA 00645706 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x12f0u, self, 0xcu, 4u, 0x7d2u, 0x9ea2fcu, 0x9ea2ecu); /* VA 0064572b */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1300u, self, 0xcu, 4u, 0x7d2u, 0x9ea30cu, 0x9ea334u); /* VA 00645750 */
    receiver = CM_GLOBAL(volatile u32, self + 0xeau);
    cm_fn_0068aff0(receiver + 0x1310u, self, 0xcu, 4u, 0x7d2u, 0x9ea31cu, 0x9ea2e4u); /* VA 00645775 */
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

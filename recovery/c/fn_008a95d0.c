/* Preferred-image VA 008a95d0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_005b5d10(u32 self, u32 path, u32 one, u32 mode);
extern u32 CM_THISCALL cm_fn_005fcb60(u32 self, u32 path, u32 person, u8 type);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_008a95d0(u32 self, u32 path, u32 mode)
{
    u8 work[0x200];
    u32 i, value, original, backup, receiver, result, version;
    u16 word;
    u8 byte;
    if (path == 0) {
        cm_fn_00944cff(0x00a8b94cu, 0, 0, work + 0x100, work);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x100, work, 0x17b9u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 0;
    }
    if (D(self + 0xe40u) != 0)
        return 0;
    original = self + 0xe44u;
    backup = self + 0x4a63u;
    for (i = 0; i < 0xf07u; ++i) {
        value = D(original + i * 4u);
        D(backup + i * 4u) = value;
    }
    word = W(original + 0x3c1cu);
    W(backup + 0x3c1cu) = word;
    byte = B(original + 0x3c1eu);
    B(backup + 0x3c1eu) = byte;
    D(self + 0xc4deu) = 1;
    result = cm_fn_005b5d10(original, path, 1, mode);
    if (result == 0) {
        for (i = 0; i < 0xf07u; ++i) {
            value = D(backup + i * 4u);
            D(original + i * 4u) = value;
        }
        word = W(backup + 0x3c1cu);
        W(original + 0x3c1cu) = word;
        byte = B(backup + 0x3c1eu);
        B(original + 0x3c1eu) = byte;
        D(self + 0xc4deu) = result;
        return result;
    }
    D(self + 0x4a5fu) = 1;
    if (mode == 0) {
        receiver = D(0x00b63c98u);
        cm_fn_005fcb60(receiver, path, mode, 1);
    }
    return 1;
}

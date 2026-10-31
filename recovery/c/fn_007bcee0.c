/* Preferred-image VA 007bcee0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_00945b00(u32 path, u32 mode);
extern u32 CM_CDECL cm_fn_00945bfe(u32 buffer, u32 size, u32 count, u32 stream);
extern u32 CM_CDECL cm_fn_00945a09(u32 stream);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_CDECL cm_fn_007bcee0(u32 path, u32 destination, u8 type)
{
    u8 work[0x200];
    u32 i, stream, count, version;
    u8 stored;
    for (i = 0; i < 0x5au; ++i)
        D(destination + i * 4u) = 0;
    W(destination + 0x168u) = 0;
    B(destination + 0x16au) = 0;
    if (path == 0) {
        cm_fn_00944cff(0x00a701b0u, 0, 0, work + 0x100, work);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x100, work, 0x1ef6u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 0;
    }
    stream = cm_fn_00945b00(path, 0x0098f928u);
    if (stream == 0)
        return 0;
    count = cm_fn_00945bfe(destination, 0x16bu, 1, stream);
    if (count != 1) {
        cm_fn_00945a09(stream);
        return 0;
    }
    cm_fn_00945a09(stream);
    if (D(destination) != 0x24a9eu)
        return 0;
    if (B(destination + 4u) != 1)
        return 0;
    stored = B(destination + 5u);
    return stored == type;
}

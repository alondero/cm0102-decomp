/* WIP: shutdown-uninitialized-find-output. Case 365 at -O2 reaches ExitProcess
   on both sides, but the shutdown file-enumeration import trace differs.
   Retry after a reviewed FindFirstFileA output boundary and harness revalidation;
   preserve the original uninitialized bytes. Two failed test invocations. */
#include "cmre.h"
/* Preferred-image VA 0x007c6a30, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern void CM_CDECL cm_fn_005a8f60(void);
extern void CM_CDECL cm_fn_005cc7d0(void);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern void CM_CDECL cm_fn_00945333(u32, u32, u32, u32);
extern unsigned long long CM_CDECL cm_fn_00945667(u32, u32);
extern void CM_CDECL cm_fn_00945934(u32);
#define D(a) (*(volatile u32 *)(u32)(a))

static void allocation_error(u8 *first, u8 *second, u32 line)
{
    char *version;
    cm_fn_00944cff(0x00a78310, 0, 0, first, second);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, first, second, line);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    cm_fn_005cc7d0();
    cm_fn_005a8f60();
    cm_fn_00945934(0xffffffffu);
}

/* EAX is dead at all nine direct sites. The two calls in 007c6ca0 forward
   it, but that wrapper's callers 005d3044/005d340a discard it. Full kills:
   005d352d/005d340f, 007ab4bc/007abe65, 007c0174/007c025d,
   007c0893/007c0912/007c09a5. Intervening loops replace AL/AX before narrow
   uses and 007c6930 consumes no incoming EAX. The original qsort preserves
   entry EAX for count<2; that incidental value has no caller-visible use. */
void CM_THISCALL cm_fn_007c6a30(u32 self, u32 club, u8 byte4, s8 byte5, u32 sort)
{
    u8 scratch[0x200];
    u32 pointer, capacity, count, id;
    if (D(self + 0x3cu) == 0) {
        pointer = (u32)cm_fn_00944e46(0x258);
        D(self + 0x3cu) = pointer;
        if (pointer == 0)
            allocation_error(scratch + 0x100, scratch, 0x11fd);
        D(self + 0x40u) = 100;
    }
    capacity = D(self + 0x40u);
    count = D(self + 0x44u);
    if (capacity == count) {
        pointer = D(self + 0x3cu);
        pointer = (u32)cm_fn_00945667(pointer, (capacity * 3u + 300u) * 2u);
        D(self + 0x3cu) = pointer;
        if (pointer == 0)
            allocation_error(scratch, scratch + 0x100, 0x1208);
        D(self + 0x40u) = D(self + 0x40u) + 100u;
    }
    count = D(self + 0x44u);
    pointer = D(self + 0x3cu);
    id = D(club);
    D(pointer + count * 6u) = id;
    count = D(self + 0x44u);
    pointer = D(self + 0x3cu);
    *(volatile u8 *)(pointer + count * 6u + 4u) = byte4;
    if (byte5 > 10) byte5 = 10;
    count = D(self + 0x44u);
    pointer = D(self + 0x3cu);
    *(volatile u8 *)(pointer + count * 6u + 5u) = (u8)byte5;
    count = D(self + 0x44u) + 1u;
    D(self + 0x44u) = count;
    if (sort != 0) {
        pointer = D(self + 0x3cu);
        cm_fn_00945333(pointer, count, 6, 0x007c6d40);
    }
}

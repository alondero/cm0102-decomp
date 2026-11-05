#include "cmre.h"
/* Preferred-image VA, base 00400000; CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Full EAX return, native thiscall. Original callees remain boundaries. */
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, int);

__attribute__((target("no-sse3")))
u32 CM_THISCALL cm_fn_0043f720(u32 self, u32 selection, u32 table, u32 count)
{
    /* L = ESP after SUB 208h; saved ESI is L-4. All escaped buffers
       share this uninitialized allocation: [ESP+0c]=L+8, [ESP+10c]=L+108. */
    union { long long converted; u8 bytes[0x208]; } scratch;
    long double value;
    u32 result, query;
    s8 index;
    if (!table) goto bad_table;
    index = (s8)selection;
    if ((s16)index >= (s16)count) goto bad_table;
    if (index < 0) goto bad_table;
    value = (long double)*(volatile s32 *)(table + (u32)(s32)index * 4u);
    value *= (long double)CM_GLOBAL(volatile double, 0x00ad9da0u);
    /* Original __ftol site 0043f759: full signed64 FISTP, then signed64 FILD. */
    scratch.converted = (long long)value;
    value = (long double)scratch.converted;
    value /= (long double)CM_GLOBAL(volatile double, 0x00ad9cb8u);
    /* Original __ftol site 0043f770: retain low DWORD for store and EAX. */
    result = (u32)(long long)value;
    *(volatile u32 *)self = result;
    return result;

bad_table:
    /* Push L+8, L+108, 0, 0, format: reverse to C argument order. */
    cm_fn_00944cff(0x0098b1fcu, 0u, 0u, scratch.bytes + 0x108, scratch.bytes + 8);
    /* Query takes no args; the earlier pushes belong to 944c9f. */
    query = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, query,
                  scratch.bytes + 0x108, scratch.bytes + 8, 0xc0u);
    cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
    CM_GLOBAL(volatile u32, 0x00b67a34u) = 0u;
    return *(volatile u32 *)self;
}

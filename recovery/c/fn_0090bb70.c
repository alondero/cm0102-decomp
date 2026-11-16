#include "cmre.h"

extern void cm_fn_005e8290(const char *, const char *, int);
extern char *cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);

/* Provisional: VAs 0090bbf3, 0090bc09, 0090bc18 and 0090bc20
   depend on inherited DF in SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   This C expresses only DF=0; DF=1 requires an evidenced entry contract. */
void cm_fn_0090bb70(u8 *p)
{
    u8 local[512];
    u32 n, count, i;
    u8 *s;
    if (!p) {
        char *v;
        cm_fn_00944cff(0x00ad4134, 0, 0, local + 256, local);
        v = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, v, local + 256, local, 0x18f);
        cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        CM_GLOBAL(u32, 0x00b67a34) = 0;
        return;
    }
    n = 0;
    while (p[n]) ++n;
    if (!n) return;
    while (*p < 0x20) {
        s = p + 1;
        count = 0;
        while (s[count]) ++count;
        ++count;
        for (i = 0; i < (count >> 2); ++i)
            *(u32 *)(p + i * 4) = *(u32 *)(s + i * 4);
        --n;
        for (i = count & ~3U; i < count; ++i) p[i] = s[i];
        if (!n) return;
    }
    while (n && p[n - 1] < 0x20) p[--n] = 0;
}

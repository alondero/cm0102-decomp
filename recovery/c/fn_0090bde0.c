/* fn_0090bde0 (VA 0x0090bde0), cdecl, ret 0, two stack arguments (arg1 at entry+4,
   arg2 at entry+8). Uses a 0x100-byte local buffer (listing: sub esp, 0x100).
   The buffer is one scratch array: the callee 0x0090cf20 writes at buffer+0 (listing:
   lea ecx,[esp+0xc] before two pushes), and the tail scans from the same buffer+0
   (listing: after add esp,0xc, esp = buffer-8, so lea edi,[esp+8] = buffer+0).

   The listing's repne scasb / rep movsd / rep movsb sequences are written as explicit
   byte scans and dword-then-byte copies in the same order. Dword copies read and
   write 4 bytes at a time, as rep movsd does. The return value is the constant the
   listing leaves in EAX (mov eax, 0xde279c) on every path. */
#include "cmre.h"

extern u32 CM_CDECL cm_fn_0090bc50(u8 *arg1);
extern void cm_fn_0090cf20(char *param_1, char *param_2);

/* repne scasb al over a NUL-terminated string: bytes scanned, including the NUL. */
static u32 cm_scan_count(u8 *p)
{
    volatile u8 *s = p;
    u32 n = 0;
    u8 c;

    do {
        c = s[n];
        n++;
    } while (c != 0);
    return n;
}

/* Index of the first NUL byte (repne scasb followed by dec edi). */
static u32 cm_first_nul(u8 *p)
{
    volatile u8 *s = p;
    u32 i = 0;

    while (s[i] != 0) {
        i++;
    }
    return i;
}

/* rep movsd for count >> 2 dwords, then rep movsb for count & 3 bytes. */
static void cm_copy_string(u8 *dst, u8 *src, u32 count)
{
    volatile u8 *d = dst;
    volatile u8 *s = src;
    u32 dwords = count >> 2;
    u32 i;

    for (i = 0; i < dwords; i++) {
        *(volatile u32 *)(d + 4 * i) = *(volatile u32 *)(s + 4 * i);
    }
    for (i = 4 * dwords; i < count; i++) {
        d[i] = s[i];
    }
}

u32 cm_fn_0090bde0(u8 *arg1, u8 *arg2)
{
    u8 scratch[0x100];
    u8 *edi;
    u8 *dest;
    u32 count;
    u32 at;

    CM_GLOBAL(u8, 0xde279c) = 0;
    edi = (u8 *)cm_fn_0090bc50(arg1);
    cm_fn_0090cf20((char *)arg2, (char *)scratch);

    dest = &CM_GLOBAL(u8, 0xde279c);

    if (edi != 0 && *(volatile u8 *)edi != 0) {
        /* Copy the first string (with its NUL) to the global buffer. */
        count = cm_scan_count(edi);
        cm_copy_string(dest, edi, count);
        /* Append the global string at 0x009c3730 at the first NUL. */
        count = cm_scan_count(&CM_GLOBAL(u8, 0x9c3730));
        at = cm_first_nul(dest);
        cm_copy_string(dest + at, &CM_GLOBAL(u8, 0x9c3730), count);
    }

    /* Common tail: append the string at scratch (written by 0x0090cf20) to the global buffer. */
    count = cm_scan_count(scratch);
    at = cm_first_nul(dest);
    cm_copy_string(dest + at, scratch, count);

    return 0xde279cu;
}

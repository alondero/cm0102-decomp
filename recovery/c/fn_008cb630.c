/* Preferred-image VA 0x008CB630, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: thiscall (ECX is the object, saved in ESI) with one stack argument, a
   pointer to a dword key, and ret 4. Same shape as fn_008cb590: the key indexes an
   8-byte-entry table pointed to by dword +0x86E, and if that entry's byte +0 has
   bit 2 set the byte is decremented by 4. Then count dword +0x8A0 (signed) and
   record base dword +0x898 are read. If count > 0, the key is read again and 13-byte
   records are scanned (dword +0 compared with the key). Not found returns with
   nothing changed. On a match at index f, records f+1 .. count-1 are copied down
   one slot (dwords +0..+7 and byte +0xC of each record) and count is decremented.
   Field and table meanings remain unknown. */
#include "cmre.h"

/* The original reads the key and table before any store and re-reads the key
   after the table byte update. Scheduling is off to keep that order, and the
   record copy uses dword-sized volatile accesses so -O2 does not merge them into
   one 8-byte access (see fn_00892380.c and fn_008cb590.c). */
void CM_THISCALL __attribute__((optimize("no-schedule-insns2"))) cm_fn_008cb630(void *self, u32 *arg)
{
    u8 *base = (u8 *)self;
    u32 idx;
    u32 key;
    u8 *entry;
    u8 bval;
    s32 n;
    u8 *rec;
    u8 *src;
    u8 *dst;
    u32 f;
    u32 k;

    idx = *arg;
    entry = *(u8 **)(base + 0x86e) + idx * 8;
    bval = *entry;
    if (bval & 4) {
        *entry = (u8)(bval - 4);
    }

    n = *(s32 *)(base + 0x8a0);
    rec = *(u8 **)(base + 0x898);
    if (n > 0) {
        key = *arg;
        f = 0;
        for (;;) {
            if (*(u32 *)rec == key) {
                break;
            }
            f++;
            rec += 0xd;
            if ((s32)f >= n) {
                return;
            }
        }

        k = f + 1;
        if ((s32)k < n) {
            do {
                u8 *b = *(u8 **)(base + 0x898);
                src = b + k * 0xd;
                dst = b + (k - 1) * 0xd;
                *(volatile u32 *)dst = *(volatile u32 *)src;
                *(volatile u32 *)(dst + 4) = *(volatile u32 *)(src + 4);
                *(volatile u32 *)(dst + 8) = *(volatile u32 *)(src + 8);
                dst[0xc] = src[0xc];
                k++;
            } while ((s32)k < *(s32 *)(base + 0x8a0));
        }
        *(s32 *)(base + 0x8a0) -= 1;
    }
}

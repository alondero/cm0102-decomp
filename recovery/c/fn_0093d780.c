/* Preferred-image VA 0x0093D780, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: cdecl (plain ret, the caller pops). Four stack arguments:
   arg1 = array of 32-bit element pointers (dword [arg1 + i*4]),
   arg2 = start index (also the lower bound of the upward pass),
   arg3 = upper bound (inclusive),
   arg4 = table of signed 16-bit words, indexed by the signed 16-bit value
   at each element pointer.
   Pass 1 walks down from arg2: child j = 2i+1 (and j+1 when j+1 <= arg3)
   is chosen when table[value(j+1)] > table[value(j)]; the elements at i and
   j are exchanged, and i = j, j = 2j+1, while j <= arg3.
   Pass 2 walks up from the final i: parent p = (i-1) >> 1 (arithmetic
   shift), continuing while p >= arg2 and p != i. It stops when
   table[value(p)] > table[value(i)]; otherwise it exchanges the elements at
   p and i, then i = p and p = (p-1) >> 1.
   Loads follow the listing order: both element pointers, both values, then
   the table entry for the later index before the entry for the earlier one.
   Array, value and table accesses are volatile so -O2 keeps that order at a
   fault. The listing spills the element during pass 2 into the arg3 stack
   slot; that is an internal temporary here, so it is kept in a local.
   Returns nothing (eax still holds arg1 but no caller reads it). */
#include "cmre.h"

void CM_CDECL cm_fn_0093d780(u32 *arg1, s32 arg2, s32 arg3, s16 *arg4)
{
    volatile u32 *arr = (volatile u32 *)arg1;
    volatile s16 *table = (volatile s16 *)arg4;
    s32 i;
    s32 j;
    s32 m;
    s32 p;
    u32 saved;
    u32 ptr_a;
    u32 ptr_b;
    s32 val_a;
    s32 val_b;
    s16 key_a;
    s16 key_b;

    i = arg2;
    j = i + i + 1;
    while (j <= arg3) {
        m = j + 1;
        if (m <= arg3) {
            ptr_a = arr[m];
            ptr_b = arr[j];
            val_a = *(volatile s16 *)ptr_a;
            val_b = *(volatile s16 *)ptr_b;
            key_a = table[val_a];
            key_b = table[val_b];
            if (key_a > key_b) {
                j = m;
            }
        }
        saved = arr[i];
        arr[i] = arr[j];
        arr[j] = saved;
        i = j;
        j = j + j + 1;
    }

    p = (i - 1) >> 1;
    while (p >= arg2 && p != i) {
        ptr_a = arr[p];
        ptr_b = arr[i];
        val_a = *(volatile s16 *)ptr_a;
        val_b = *(volatile s16 *)ptr_b;
        key_a = table[val_a];
        key_b = table[val_b];
        if (key_a > key_b) {
            break;
        }
        saved = arr[i];
        arr[i] = arr[p];
        arr[p] = saved;
        i = p;
        p = (p - 1) >> 1;
    }
}

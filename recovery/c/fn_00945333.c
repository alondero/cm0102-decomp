/* Preferred-image VA 0x00945333, base 0x00400000; CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL4, with incidental EDX:EAX retained (including both early exits).
   regparm(2) transports incoming EAX/EDX without adding stack arguments.
   The original small-sort and byte-swap routines remain boundaries.
   Their EDX:EAX results and swap's known ECX result carry the comparator's
   physical register inputs; the comparator still receives two stack args.
   Scratch is exactly F8 bytes: starts L0, ends L78, scan LF0, pending LF4.
   The two original first argument slots become pointers into those arrays.
   No depth clamp, frame padding, pointer check or overflow repair is added. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
typedef unsigned long long wide_945333;
extern wide_945333 CM_CDECL __attribute__((regparm(2))) cm_fn_009454d5(
    u32 eax_in, u32 edx_in, u32 first, u32 second, u32 width);
extern wide_945333 CM_CDECL __attribute__((regparm(2))) cm_fn_00945487(
    u32 eax_in, u32 edx_in, u32 first, u32 last, u32 width, u32 compare);
typedef wide_945333 (__attribute__((cdecl, regparm(3))) *compare_945333)(
    u32 eax_in, u32 edx_in, u32 ecx_in, u32 first, u32 second);

unsigned long long CM_CDECL __attribute__((regparm(2))) cm_fn_00945333(
    u32 eax_in, u32 edx_in, volatile u32 base_arg, volatile u32 count_arg,
    volatile u32 width_arg, volatile u32 compare_arg)
{
    u8 scratch[0xf8];
    u32 l = (u32)scratch, eax = eax_in, edx = edx_in;
    u32 first, last = count_arg, right, width, scan;
    u32 ecx, target, old_count;
    wide_945333 result;
    if (last < 2) goto done;
    if (width_arg == 0) goto done;
    old_count = D(l + 0xf4u);
    D(l + 0xf4u) = old_count & 0u;
    last = (last - 1u) * width_arg;
    first = base_arg;
    base_arg = l + 0x78u;
    last += first;
    count_arg = l;

partition:
    width = width_arg;
    eax = last - first;
    edx = eax % width;
    eax = eax / width + 1u;
    if (eax <= 8) {
        target = compare_arg;
        result = cm_fn_00945487(eax, edx, first, last, width, target);
        eax = (u32)result;
        edx = (u32)(result >> 32);
        /* ECX is dead here. A following swap writes ECX before a root
           comparator; a following small sort writes it before its callback. */
        goto pop_range;
    }
    eax = (eax >> 1) * width + first;
    scan = eax;
    result = cm_fn_009454d5(eax, edx, scan, first, width);
    eax = (u32)result;
    edx = (u32)(result >> 32);
    ecx = first + (scan != first ? width : 0u);
    D(l + 0xf0u) = first;
    right = width + last;

scan_left:
    eax = D(l + 0xf0u);
    eax += width_arg;
    D(l + 0xf0u) = eax;
    if (eax <= last) {
        target = compare_arg;
        result = ((compare_945333)target)(eax, edx, ecx, eax, first);
        eax = (u32)result;
        edx = (u32)(result >> 32);
        ecx = first; /* two original POP ECX instructions */
        if ((s32)eax <= 0) goto scan_left;
    }

scan_right:
    right -= width_arg;
    if (right > first) {
        target = compare_arg;
        result = ((compare_945333)target)(eax, edx, ecx, right, first);
        eax = (u32)result;
        edx = (u32)(result >> 32);
        ecx = first;
        if ((s32)eax >= 0) goto scan_right;
    }
    width = width_arg;
    scan = D(l + 0xf0u);
    if (right >= scan) {
        result = cm_fn_009454d5(eax, edx, scan, right, width);
        eax = (u32)result;
        edx = (u32)(result >> 32);
        ecx = right + (scan != right ? width : 0u);
        goto scan_left;
    }
    result = cm_fn_009454d5(eax, edx, first, right, width);
    eax = D(l + 0xf0u);
    ecx = right - first - 1u;
    edx = last - eax;
    if ((s32)ecx >= (s32)edx) {
        ecx = width_arg;
        edx = first + ecx;
        if (edx < right) {
            edx = count_arg;
            count_arg += 4u;
            right -= ecx;
            ecx = base_arg;
            D(l + 0xf4u) = D(l + 0xf4u) + 1u;
            base_arg += 4u;
            D(edx) = first;
            D(ecx) = right;
        }
        if (eax >= last) goto pop_range;
        first = eax;
        goto partition;
    }
    if (eax < last) {
        ecx = count_arg;
        D(l + 0xf4u) = D(l + 0xf4u) + 1u;
        count_arg += 4u;
        D(ecx) = eax;
        eax = base_arg;
        base_arg += 4u;
        D(eax) = last;
    }
    eax = width_arg;
    eax += first;
    if (eax >= right) goto pop_range;
    right -= width_arg;
    last = right;
    goto partition;

pop_range:
    D(l + 0xf4u) = D(l + 0xf4u) - 1u;
    count_arg -= 4u;
    base_arg -= 4u;
    if ((s32)D(l + 0xf4u) < 0) goto done;
    eax = count_arg;
    first = D(eax);
    eax = base_arg;
    last = D(eax);
    goto partition;

done:
    return ((wide_945333)edx << 32) | eax;
}

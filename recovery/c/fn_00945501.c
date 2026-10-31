/* Preferred-image VA 0x00945501, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: cdecl with two stack arguments (caller pops, plain RET).
   product = arg1 * arg2 (low 32 bits). The product is stored over the first argument
   slot and is used raw by the mode-3 and zero-fill paths. If product <= 0xFFFFFFE0,
   a zero product becomes 1 and the size is rounded up to 16 bytes; otherwise the
   unrounded product is kept. Then, while the size is <= 0xFFFFFFE0 (unsigned):
   - global dword 0x00DE4404 == 3: if product <= dword 0x00DE43FC, call 00948EE4
     (product); a nonzero result is zero-filled by 0094A640 (result, 0, product)
     and returned;
   - else if it == 2: if the size <= dword 0x00AD8D7C, call 00949987 (size >> 4);
     a nonzero result is zero-filled with the size and returned;
   - otherwise (and after a zero or over-limit result) call the IAT slot 0x00967154
     as HeapAlloc (handle = dword 0x00DE4400, flags 8, size); nonzero returns.
   On failure: return 0 if dword 0x00DE3008 is zero; else call 0094895B (size);
   zero means return 0, nonzero repeats the attempt.
   Callees and the import remain explicit boundaries; field and global meanings
   are unknown. */
#include "cmre.h"

#define CM_FN_00945501_HEAP_ALLOC_SLOT 0x00967154

typedef void *(CM_STDCALL *cm_fn_00945501_heap_alloc_t)(void *heap, u32 flags, u32 bytes);

extern u32 cm_fn_00948ee4(u32 size);
extern u32 cm_fn_00949987(u32 count);
extern u32 cm_fn_0094895b(u32 arg);
extern u32 cm_fn_0094a640(u32 dst, u32 value, u32 count);

void *CM_CDECL cm_fn_00945501(u32 arg1, u32 arg2)
{
    u32 product;
    u32 rounded;
    void *result;
    s32 mode;

    product = arg1 * arg2;
    rounded = product;
    if (product <= 0xffffffe0u) {
        if (product == 0) {
            rounded = 1;
        }
        rounded = (rounded + 0xf) & 0xfffffff0u;
    }

    for (;;) {
        result = 0;
        if (rounded <= 0xffffffe0u) {
            mode = CM_GLOBAL(s32, 0x00de4404);
            if (mode == 3) {
                if (product <= CM_GLOBAL(u32, 0x00de43fc)) {
                    result = (void *)(cm_fn_00948ee4(product));
                    if (result != 0) {
                        cm_fn_0094a640((u32)result, 0, product);
                        return result;
                    }
                }
            } else if (mode == 2) {
                if (rounded <= CM_GLOBAL(u32, 0x00ad8d7c)) {
                    result = (void *)(cm_fn_00949987(rounded >> 4));
                    if (result != 0) {
                        cm_fn_0094a640((u32)result, 0, rounded);
                        return result;
                    }
                }
            }
            result = (CM_GLOBAL(cm_fn_00945501_heap_alloc_t, CM_FN_00945501_HEAP_ALLOC_SLOT))(
                CM_GLOBAL(void *, 0x00de4400), 8u, rounded);
            if (result != 0) {
                return result;
            }
        }
        if (CM_GLOBAL(s32, 0x00de3008) == 0) {
            return 0;
        }
        if (cm_fn_0094895b(rounded) == 0) {
            return 0;
        }
    }
}

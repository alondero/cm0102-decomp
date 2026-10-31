/* Preferred-image VA 0x00951183, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: cdecl with four stack arguments (caller pops, plain RET). The
   leading push ecx instructions only reserve two local dwords; ECX is not an input.
   Returns a 64-bit value in EDX:EAX; the callers at 0x0094C803 and 0x0094C848 read
   EDX. Arg1 selects a bit: if arg1 is below dword 0x00DE43E0 (unsigned), the table
   entry at 0x00DE42E0 + (signed arg1 >> 5) * 4 points to a block whose byte at
   ((arg1 & 0x1F) * 8) + 4, bit 0, must be set. Then:
   - 0094FDED(arg1) returns the handle; -1 sets dword 0x00DE2F8C = 9 and returns -1;
   - otherwise the IAT slot 0x009670A0 (SetFilePointer) is called with (handle,
     arg2, &high, arg4), where high starts as arg3. If it returns -1 and the IAT
     slot 0x009670E8 (GetLastError) returns nonzero, 0094B2A0 (that error) is called
     and the result is -1. Otherwise the block byte's bit 1 is cleared and the
     result is high:low, with low being SetFilePointer's return value.
   - A failed bit test or arg1 at or above the limit sets dword 0x00DE2F90 = 0 and
     dword 0x00DE2F8C = 9, and returns -1.
   Field and global meanings remain unknown; callees and imports stay boundaries. */
#include "cmre.h"

typedef u32 (CM_STDCALL *cm_fn_00951183_set_file_pointer_t)(u32 file, s32 distance, s32 *high,
                                                            u32 method);
typedef u32 (CM_STDCALL *cm_fn_00951183_get_last_error_t)(void);

extern u32 cm_fn_0094fded(u32 index);
extern u32 cm_fn_0094b2a0(u32 arg);

unsigned long long CM_CDECL cm_fn_00951183(u32 arg1, s32 arg2, s32 arg3, u32 arg4)
{
    s32 high;
    u32 low;
    u32 error;
    u32 handle;
    u32 slot_addr;
    u32 bit_off;
    s32 index5;

    if (arg1 < CM_GLOBAL(u32, 0x00de43e0)) {
        index5 = (s32)arg1 >> 5;
        slot_addr = 0x00de42e0u + (u32)index5 * 4u;
        bit_off = (arg1 & 0x1fu) * 8u;
        if ((((u8 *)(u32)CM_GLOBAL(u32, slot_addr))[bit_off + 4] & 1) != 0) {
            high = arg3;
            handle = cm_fn_0094fded(arg1);
            if (handle != 0xffffffffu) {
                low = (CM_GLOBAL(cm_fn_00951183_set_file_pointer_t, 0x009670a0))(
                    handle, arg2, &high, arg4);
                if (low == 0xffffffffu) {
                    error = (CM_GLOBAL(cm_fn_00951183_get_last_error_t, 0x009670e8))();
                    if (error != 0) {
                        cm_fn_0094b2a0(error);
                        return 0xffffffffffffffffULL;
                    }
                }
                ((u8 *)(u32)CM_GLOBAL(u32, slot_addr))[bit_off + 4] &= 0xfd;
                return ((unsigned long long)(u32)high << 32) | (unsigned long long)low;
            }
            CM_GLOBAL(u32, 0x00de2f8c) = 9;
            return 0xffffffffffffffffULL;
        }
    }
    CM_GLOBAL(u32, 0x00de2f90) = 0;
    CM_GLOBAL(u32, 0x00de2f8c) = 9;
    return 0xffffffffffffffffULL;
}

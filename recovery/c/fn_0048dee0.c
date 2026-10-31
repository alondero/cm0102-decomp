#include "cmre.h"

/* Global table at 0x00b63d60: 35 records, 0x48 bytes apart. The dword at record+0 is
   an object pointer; the byte at 0x00b63d74 + index*0x48 is returned on a match. */
#define TABLE_PTR_VA 0x00b63d60u
#define TABLE_BYTE_VA 0x00b63d74u
#define RECORD_STRIDE 0x48u
#define TABLE_COUNT 0x23u

/* Callee at 0x0053b900: cdecl, one stack argument (the object). */
extern u32 CM_CDECL cm_fn_0053b900(u32 *input);

/* Cdecl with one stack argument (object pointer at entry+4). Returns EAX as the
   listing leaves it. Object field at 0x53 (dword) is compared with the table pointers. */
u32 cm_fn_0048dee0(u32 obj)
{
    u32 wanted;
    u8 index;
    u32 record;
    u8 flag;
    u32 ret;
    u32 negated;
    u32 low;

    wanted = *(volatile u32 *)(obj + 0x53u);
    if (wanted != 0u) {
        for (index = 0u; index < TABLE_COUNT; index = (u8)(index + 1u)) {
            record = *(volatile u32 *)(TABLE_PTR_VA + (u32)index * RECORD_STRIDE);
            if (record != 0u) {
                flag = *(volatile u8 *)(record + 0x11cu);
                if (flag != 0u && record == wanted) {
                    /* movsx leaves the upper 24 bits of EAX zero; only AL is loaded. */
                    return *(volatile u8 *)(TABLE_BYTE_VA + (u32)index * RECORD_STRIDE);
                }
            }
        }
    }

    ret = cm_fn_0053b900((u32 *)obj);

    /* neg eax sets CF when ret != 0; the sbb/and/add sequence only changes AL, so
       the upper 24 bits come from the negated value. */
    negated = 0u - ret;
    low = (ret != 0u) ? 0u : 3u;
    return (negated & 0xffffff00u) | low;
}

#include "cmre.h"

/* Object fields by byte offset from the object pointer argument:
   0x00 signed end index (rewritten from 0x08 at exit), 0x04 cleared at exit,
   0x08 start value, 0x0c flags word (low 16 bits tested, 0x80 and 0x20 bits
   touched), 0x10 handle passed to 0x0094b5d6. */
#define OBJ_U32(self, off) (*(volatile u32 *)((self) + (off)))

extern unsigned long long CM_CDECL cm_fn_0094b5d6(u32 arg1, u32 arg2, u32 arg3);

u32 cm_fn_0094af9e(u32 self)
{
    u32 result = 0;
    u32 flags;
    s32 count;
    u32 start;
    u32 handle;

    flags = OBJ_U32(self, 0x0c);
    if (((flags & 3u) == 2u) && ((flags & 0x108u) != 0u)) {
        start = OBJ_U32(self, 0x08);
        count = (s32)(OBJ_U32(self, 0x00) - start);
        if (count > 0) {
            handle = OBJ_U32(self, 0x10);
            if ((u32)cm_fn_0094b5d6(handle, start, (u32)count) == (u32)count) {
                flags = OBJ_U32(self, 0x0c);
                if ((flags & 0x80u) != 0u) {
                    OBJ_U32(self, 0x0c) = flags & 0xfffffffdu;
                }
            } else {
                OBJ_U32(self, 0x0c) = OBJ_U32(self, 0x0c) | 0x20u;
                result = 0xffffffffu;
            }
        }
    }

    OBJ_U32(self, 0x04) = 0;
    OBJ_U32(self, 0x00) = OBJ_U32(self, 0x08);
    return result;
}

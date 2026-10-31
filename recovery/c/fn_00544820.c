#include "cmre.h"

extern int CM_CDECL cm_fn_00540a50(u32 *object, u32 **result, int flag);

u8 CM_CDECL cm_fn_00544820(u32 *object)
{
    u32 *original_object = object;
    u32 doubled_window;
    u32 position;
    u32 upper;
    u32 *initial_list;

    if (object == 0)
        return 0;

    initial_list = (u32 *)*(volatile u32 *)((u8 *)object + 0x53);
    if (initial_list == 0)
        return 0;

    doubled_window = CM_GLOBAL(volatile u32, 0x00ae2350);
    position = *(volatile u32 *)object;
    upper = CM_GLOBAL(volatile u32, 0x00ae235c);
    if ((s32)position < (s32)(upper - doubled_window - doubled_window)) {
        u32 *mutable_argument_slot = object;

        if (*(volatile u32 *)((u8 *)object + 0x57) == 0)
            return 0;

        if (cm_fn_00540a50(object, &mutable_argument_slot, 1) != 0 &&
            mutable_argument_slot == 0)
            return 0;

        {
            u32 table = CM_GLOBAL(volatile u32, 0x00adadfc);
            u32 *list;
            u32 entry;
            u8 flags;

            if (table == 0)
                return 0;
            list = (u32 *)*(volatile u32 *)((u8 *)original_object + 0x57);
            entry = *(volatile u32 *)(table + *(volatile u32 *)list * 4u);
            if (entry == 0)
                return 0;
            list = (u32 *)*(volatile u32 *)((u8 *)original_object + 0x53);
            flags = *(volatile u8 *)((u8 *)list + 0x11c);
            if ((flags & 2u) == 0)
                return 0;
        }
        return 1;
    }

    upper -= doubled_window;
    if ((s32)position >= (s32)upper)
        return 0;

    {
        return *(volatile s32 *)((u8 *)initial_list + 0x88) > 0x96 ? 1 : 0;
    }
}

#include "cmre.h"

extern u32 CM_CDECL cm_fn_0053b900(u32 *input);
extern u32 CM_CDECL cm_fn_005409b0(u32 *input);

u32 CM_STDCALL cm_fn_0076d9b0(u32 record, u32 candidate)
{
    u32 owner = *(volatile u32 *)(candidate + 0x24u);
    u32 active = *(volatile u32 *)(record + 0x61u);
    if (active == 0 || owner == 0)
        return 0;

    if (cm_fn_0053b900((u32 *)owner) == 0)
        return 0;
    if (*(volatile u32 *)(record + 0x24u) == owner)
        return 1;

    u32 checked = cm_fn_005409b0((u32 *)owner);
    if (checked != 0 && *(volatile s8 *)(record + 0x18u) > 0x15)
        return 0;

    if (*(volatile u8 *)(record + 0x22u) != 0) {
        u32 record_owner = *(volatile u32 *)(record + 0x1au);
        u32 candidate_owner = *(volatile u32 *)(owner + 0x53u);
        if (record_owner != candidate_owner)
            return 0;
    }

    u32 id = *(volatile u32 *)record;
    u32 table_index = id * 79u;
    u32 table = CM_GLOBAL(volatile u32, 0x00ae2d0cu);
    if ((*(volatile u8 *)(table + table_index + 0xcu) & 2u) != 0) {
        u32 record_owner = *(volatile u32 *)(record + 0x1au);
        u32 candidate_owner = *(volatile u32 *)(owner + 0x53u);
        if (record_owner != candidate_owner)
            return 0;
    }

    u32 previous = *(volatile u32 *)(record + 0x24u);
    if (previous != 0) {
        u32 previous_owner = *(volatile u32 *)(previous + 0x53u);
        u32 candidate_owner = *(volatile u32 *)(owner + 0x53u);
        if (previous_owner != candidate_owner)
            return 0;
    }

    u32 candidate_owner = *(volatile u32 *)(owner + 0x53u);
    u32 first_owner = *(volatile u32 *)(record + 0x1au);
    if (first_owner != candidate_owner &&
        *(volatile u32 *)(record + 0x1eu) != candidate_owner)
        return 0;

    u32 forbidden_id = CM_GLOBAL(volatile u32, 0x009cf434u);
    if (*(volatile u32 *)candidate_owner == forbidden_id) {
        u32 other = *(volatile u32 *)(record + 0x1eu);
        if (other != 0 && *(volatile u32 *)other == forbidden_id)
            return 0;
    }
    return 1;
}

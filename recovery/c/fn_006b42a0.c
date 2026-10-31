/* WIP: events mismatch, case 32
   Import order differs (also case 90; memory page 0xb67000 at case 73). 0x00717cc0 reads only the
   nonzero test of its second argument on this path; the record selection below follows the listing,
   and the divergence is not yet explained. */
#include "cmre.h"

/* fn_006b42a0: thiscall (ECX = object, read before written), no stack arguments (ret 0).
   Preferred-image VA 0x006b42a0, image base 0x00400000.
   Object fields: 0x2b nonzero enables the scan; 0x15 signed record count; 0x0d record
   array base. Records are 0x54 bytes; each scan clears 16 dwords at record+0x14 (the
   listing's rep stosd), then tests record+0x10 and record+0x0c. */

extern void cm_fn_00717cc0(u32 param_1, int param_2);
extern u8 CM_FASTCALL cm_fn_007fc7e0(int param_1);

u32 CM_THISCALL cm_fn_006b42a0(u8 *self)
{
    u32 local_4;   /* return value; [esp+0x14] in the listing, set to 1 by a record with [+0x10] */
    s32 local_8;   /* loop index; [esp+0x10] in the listing */
    u32 ebp_rec;   /* selected record pointer, ebp in the listing */
    u32 base;
    u32 rec;
    u32 off;
    u32 k;

    local_4 = 0;
    if (*(volatile s32 *)(self + 0x2b) == 0) {
        return 0;
    }

    ebp_rec = 0;
    local_8 = 0;
    if (*(volatile s32 *)(self + 0x15) > 0) {
        base = *(volatile u32 *)(self + 0x0d);
        off = 0;
        do {
            /* rep stosd: 16 zero dwords at record + 0x14 */
            for (k = 0; k < 16u; k++) {
                *(volatile u32 *)(base + off + 0x14u + k * 4u) = 0;
            }
            base = *(volatile u32 *)(self + 0x0d);
            rec = base + off;
            if (*(volatile u32 *)(rec + 0x10u) != 0) {
                if (ebp_rec == 0 || *(volatile u32 *)(ebp_rec + 0x10u) == 0) {
                    ebp_rec = rec;
                }
                local_4 = 1;
            } else {
                if (*(volatile u32 *)(rec + 0x0cu) != 0 && ebp_rec == 0) {
                    ebp_rec = rec;
                }
            }
            local_8 = local_8 + 1;
            off = off + 0x54u;
        } while (local_8 < *(volatile s32 *)(self + 0x15));
    }

    CM_GLOBAL(u32, 0x00b67a48) = 1;
    cm_fn_00717cc0(0, (int)ebp_rec);
    cm_fn_007fc7e0(0x00b74450);
    CM_GLOBAL(u32, 0x00b67a48) = 0;
    return local_4;
}

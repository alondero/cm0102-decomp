#include "cmre.h"
/* Preferred-image VA 0085a850; CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   EAX is an input on the null paths. Three native stack DWORDs; RET 12. */
extern u32 CM_THISCALL cm_fn_004cf5a0(u32, u32);
extern u32 CM_THISCALL cm_fn_004cf6d0(u32, u32);
extern u32 CM_CDECL cm_fn_00540a50(u32, u32, u32);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))

u32 CM_STDCALL __attribute__((regparm(1)))
cm_fn_0085a850(u32 entry_eax, u32 person, u32 club, volatile u32 value)
{
    u8 scratch[512];
    u32 eax = entry_eax, record, id, club_id, base, mapped;
    u8 byte_value;
    if (!person) return eax;
    if (!club) return eax;
    byte_value = (u8)value;
    if ((s8)byte_value < -50 || (s8)byte_value > 50) {
        cm_fn_00944cff(0x00a82b88, 0, 0, scratch + 256, scratch);
        eax = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, eax,
                      scratch + 256, scratch, 0x332);
        eax = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return eax;
    }
    eax = cm_fn_004cf5a0(D(0x00ae1050), person);
    record = eax;
    if (record) {
        eax = D(record + 4u);
        club_id = D(club);
        if (eax == club_id) {
            B(record + 0x3au) = byte_value;
            return eax;
        }
        eax = cm_fn_00540a50(club, 0, 1);
        id = D(record + 4u);
        base = D(0x00ae23b4);
        mapped = base + id + ((id + id * 8u) * 16u + id) * 4u;
        byte_value = (u8)value;
        if (mapped == eax) {
            B(record + 0x3au) = byte_value;
            return eax;
        }
    }
    eax = cm_fn_004cf6d0(D(0x00ae1050), person);
    record = eax;
    if (!record) return eax;
    id = D(record + 4u);
    eax = D(club);
    if (id != eax) {
        eax = cm_fn_00540a50(club, 0, 1);
        id = D(record + 4u);
        base = D(0x00ae23b4);
        mapped = base + id + ((id + id * 8u) * 16u + id) * 4u;
        if (mapped != eax) return eax;
    }
    B(record + 0x3au) = byte_value;
    return eax;
}

#include "cmre.h"

/* Argument (ESI), fields by byte offset:
   0x69 pointer to a record; that record's +0x38 pointer leads to a
   text block whose string starts at +4. The text is read through
   the record chain again for each comparison (listing re-reads both pointers). */

extern s32 CM_CDECL cm_fn_00946fa0(u32 arg1, u32 arg2, u32 arg3);

#define FIELD_U32(base, off) (*(volatile u32 *)((base) + (off)))
#define BYTE_AT(addr) (*(volatile u8 *)(addr))

/* Inline two-byte-step comparison from the original: 1 when both strings match
   through the terminating NUL, 0 at the first differing byte. */
static u32 pattern_equal(u32 text, u32 pattern)
{
    u32 eax = text;
    u32 esi = pattern;
    u8 dl;
    u8 bl;
    u8 cl;

    for (;;) {
        dl = BYTE_AT(eax);
        bl = BYTE_AT(esi);
        cl = dl;
        if (dl != bl) {
            return 0u;
        }
        if (cl == 0u) {
            return 1u;
        }
        dl = BYTE_AT(eax + 1u);
        bl = BYTE_AT(esi + 1u);
        cl = dl;
        if (dl != bl) {
            return 0u;
        }
        eax += 2u;
        esi += 2u;
        if (cl == 0u) {
            return 1u;
        }
    }
}

u32 CM_CDECL cm_fn_006399c0(u32 arg1)
{
    u32 text;
    u32 rec;

    if (arg1 == 0u) {
        return 0u;
    }
    rec = FIELD_U32(arg1, 0x69);
    if (rec == 0u) {
        return 0u;
    }
    text = FIELD_U32(rec, 0x38);
    if (text == 0u) {
        return 0u;
    }
    if (cm_fn_00946fa0(text + 4u, 0x009ea078u, 4u) == 0) {
        return 1u;
    }

    text = FIELD_U32(FIELD_U32(arg1, 0x69), 0x38) + 4u;
    if (pattern_equal(text, 0x009ea070u)) {
        return 1u;
    }
    if (pattern_equal(text, 0x009ea064u)) {
        return 1u;
    }
    if (pattern_equal(text, 0x009ea05cu)) {
        return 1u;
    }
    if (pattern_equal(text, 0x009ea050u)) {
        return 1u;
    }
    if (pattern_equal(text, 0x009ea048u)) {
        return 1u;
    }
    return pattern_equal(text, 0x009ea03cu);
}

/* Primary 3.9.68 name helpers. No English-only substitution: call the recovered
 * localization wrapper, retaining original templates and shared buffers. */
#include "cm_date_names.h"
#include "../lang/cm_lang.h"
#define TEXT(va) ((const char *)(va))

char *CM_CDECL cm_date_time_name(cm_u32 time, cm_u32 full) {
    char *out = (char *)0x00ae2484u;
    static const cm_u32 short_text[3] = {0x009b8c9cu, 0x009b8ca4u, 0x009b8cb0u};
    static const cm_u32 full_text[3] = {0x009b8a84u, 0x009b8a8cu, 0x009b8a98u};
    unsigned int slot = time & 255u;
    if (slot > 2) out[0] = 0;
    else cm_lang_format(out, TEXT(full ? full_text[slot] : short_text[slot]));
    return out;
}

unsigned char CM_CDECL cm_date_format_order(cm_u32 variant) {
    if (cm_lang_current() == 9) return 1;
    if (cm_lang_current() == 3) return 1;
    if (cm_lang_current() == 5) return 2;
    if (cm_lang_current() == 4) {
        unsigned int value = variant & 255u;
        if (value <= 1) return 1;
        if (value == 2) return 3;
    }
    return 0;
}

char *CM_CDECL cm_date_weekday_name(cm_u32 weekday) {
    static const cm_u32 text[7] = {0x009b8790u, 0x009b8788u, 0x009b877cu,
        0x009b8770u, 0x009b8768u, 0x009b875cu, 0x009b8798u};
    char *out = (char *)0x00ae2434u;
    unsigned int slot = weekday & 65535u;
    if (slot > 6) out[0] = 0;
    else cm_lang_format(out, TEXT(text[slot]));
    return out;
}

char *CM_CDECL cm_date_short_month_name(cm_u32 month) {
    static const cm_u32 text[12] = {0x009b8d2cu, 0x009b8d28u, 0x009b8d00u,
        0x009b8cfcu, 0x009b8cf8u, 0x009b8cf4u, 0x009b8cf0u, 0x009b8cecu,
        0x009b8ce8u, 0x009b8ce4u, 0x009b8ce0u, 0x009b8cb8u};
    unsigned int slot = month & 255u;
    char *out = (char *)0x00ae24a0u;
    if (slot > 11) out[0] = 0;
    else if (slot == 2 || slot == 11)
        cm_lang_format(out, TEXT(text[slot]), TEXT(0x00ad9c64u));
    else cm_lang_format(out, TEXT(text[slot]));
    return out;
}

char *CM_CDECL cm_date_full_month_name(cm_u32 month) {
    static const cm_u32 text[12] = {0x009b8db8u, 0x009b8dacu, 0x009b8da4u,
        0x009b8d9cu, 0x009b8d74u, 0x009b8d6cu, 0x009b8d64u, 0x009b8d5cu,
        0x009b8d50u, 0x009b8d48u, 0x009b8d3cu, 0x009b8d30u};
    unsigned int slot = month & 255u;
    char *out = (char *)0x00ae2450u;
    if (slot > 11) out[0] = 0;
    else if (slot == 4) cm_lang_format(out, TEXT(text[slot]), TEXT(0x00ad9c64u));
    else cm_lang_format(out, TEXT(text[slot]));
    return out;
}

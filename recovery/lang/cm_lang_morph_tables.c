/* Primary official 3.9.68, VAs 006820A0 and 00682150, 171 bytes each.
 * Unsigned byte comparison, first matching record, low-byte replacement slot,
 * and original MOVSD-before-MOVSB copying. No string/case/encoding library.
 */
#include "cm_lang_morph_tables.h"

static int equal_bytes(const volatile unsigned char *key,
                       const volatile unsigned char *word) {
    for (;;) {
        unsigned char a = *key, b = *word;
        if (a != b) return 0;
        if (!a) return 1;
        a = key[1]; b = word[1];
        if (a != b) return 0;
        key += 2; word += 2;
        if (!a) return 1;
    }
}

static cm_lang_u32 replace_from_table(char *word, cm_lang_u32 property,
                                      cm_lang_u32 table) {
    const volatile unsigned char *key = *(const unsigned char *volatile *)table;
    for (;;) {
        /* The guard precedes every caller-string comparison. Misses return
         * this exact pointer, not false or the comparison result. */
        if (!*key) return (cm_lang_u32)key;
        if (equal_bytes(key, (const volatile unsigned char *)word)) break;
        table += 16u;
        key = *(const unsigned char *volatile *)table;
    }
    property &= 255u;
    if (property != 1u && property != 2u && property != 3u) return property;
    const volatile unsigned char *source =
        *(const unsigned char *volatile *)(table + property * 4u);
    const volatile unsigned char *cursor = source;
    cm_lang_u32 remaining = 0xffffffffu;
    do {
        unsigned char value = *cursor++;
        --remaining;
        if (!value) break;
    } while (remaining);
    cm_lang_u32 size = ~remaining;
    for (cm_lang_u32 n = size >> 2; n; --n) {
        *(volatile cm_lang_u32 *)word = *(const volatile cm_lang_u32 *)source;
        word += 4; source += 4;
    }
    for (cm_lang_u32 n = size & 3u; n; --n)
        *(volatile unsigned char *)word++ = *source++;
    return size;
}

cm_lang_u32 CM_LANG_CDECL cm_lang_morph_table_a(char *word, cm_lang_u32 property) {
    return replace_from_table(word, property, 0x009eb328u);
}
cm_lang_u32 CM_LANG_CDECL cm_lang_morph_table_b(char *word, cm_lang_u32 property) {
    return replace_from_table(word, property, 0x009eb7acu);
}

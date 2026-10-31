/* Instruction-derived primary 3.9.68, preferred VAs. See DATE-NUMERIC.md.
 * Original data pointers and explicit CRT/UI dependencies remain fixed.
 */
#include "cm_date_numeric.h"
#include "../lang/cm_lang.h"

typedef void (CM_CDECL *split_fn)(const char *, char *, char *, char *, char *);
typedef int (CM_CDECL *print_fn)(char *, const char *, ...);
typedef int (CM_CDECL *error_fn)(const char *, const char *, cm_u32);
#define SPLIT ((split_fn)0x00944cffu)
#define PRINT ((print_fn)0x00944c9fu)
#define ERROR ((error_fn)0x005e8290u)
#define TEXT(va) ((const char *)(va))
#define TIME_TEXT ((char *)0x00ae24acu)

static int signed_word(cm_u32 bits) {
    bits &= 65535u;
    return bits < 32768u ? (int)bits : (int)bits - 65536;
}

static cm_u32 diagnostic(cm_u32 line) {
    char filename[256], extension[256];
    SPLIT(TEXT(0x009b8478u), 0, 0, filename, extension);
    const char *version = cm_date_diagnostic_version(filename, extension, line);
    PRINT((char *)0x00ae24d0u, TEXT(0x009870e8u), version, filename, extension, line);
    ERROR(TEXT(0x009870e0u), TEXT(0x00ae24d0u), 0);
    *(volatile cm_u32 *)0x00b67a34u = 0;
    return 0;
}

static void time_name(cm_u32 time) {
    /* Reached only for the captured low byte 0/1/2. Original unreachable
     * fallback blocks copying empty text are retained in static evidence. */
    static const cm_u32 formats[3] = {0x009b8508u, 0x009b8524u, 0x009b8544u};
    cm_lang_format(TIME_TEXT, TEXT(formats[time]), TEXT(0x00ad9c64u));
}

/* Implementation support, not an additional original function. The assembly
 * entries supply the actual prior-frame seed. Unsigned address arithmetic
 * permits the original signed-byte index outside the twelve month entries. */
cm_u32 CM_CDECL cm_date_numeric_worker(cm_u32 variant, cm_date *self, char *output,
                                     cm_u32 size, cm_u32 time, cm_u32 seed) {
    if (!output) return diagnostic(variant ? 306 : 248);
    if (signed_word(size) < 9) return diagnostic(variant ? 313 : 255);
    const volatile cm_date *source = self;
    cm_u32 table = source->leap ? 0x009b8460u : 0x009b8448u;
    int ordinal = signed_word(source->day);
    cm_u32 month_bits = seed & 255u;
    for (cm_u32 i = 0; i < 12; ++i)
        if (signed_word(*(const volatile cm_u16 *)(table + 2u * i)) < ordinal)
            month_bits = i;
    int month = month_bits < 128u ? (int)month_bits : (int)month_bits - 256;
    cm_u32 offset = *(const volatile cm_u16 *)(table + 2u * (cm_u32)month);
    int day = signed_word((cm_u32)ordinal - offset);
    time &= 255u;
    if (!variant) {
        if (time <= 2) {
            time_name(time);
            cm_lang_format(output, TEXT(0x009b84a0u), day, month + 1,
                           (cm_u32)source->year, TIME_TEXT);
        } else cm_lang_format(output, TEXT(0x009b8560u), day, month + 1,
                              (cm_u32)source->year);
    } else {
        cm_u32 language = cm_lang_current();
        if (time <= 2) {
            time_name(time);
            cm_u32 year = source->year % 100u;
            if (language == 10) PRINT(output, TEXT(0x009b8610u), year, month + 1, day, TIME_TEXT);
            else cm_lang_format(output, TEXT(0x009b85acu), day, month + 1, year, TIME_TEXT);
        } else {
            cm_u32 year = source->year % 100u;
            if (language == 10) PRINT(output, TEXT(0x009b866cu), year, month + 1, day);
            else cm_lang_format(output, TEXT(0x009b8624u), day, month + 1, year);
        }
    }
    return 1;
}

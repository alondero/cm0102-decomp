/* Instruction-derived primary 3.9.68, preferred VAs; LANG-LIFECYCLE.md.
 * Loader, OS import, array destruction and CRT/UI remain explicit dependencies.
 */
#include "cm_lang_lifecycle.h"
#include "../date/cm_date.h"

typedef cm_lang_u32 (CM_LANG_CDECL *free_fn)(void *);
typedef void (__attribute__((stdcall)) *array_destroy_fn)(cm_lang_record *,
    cm_lang_u32, cm_lang_u32, cm_lang_u32 (CM_LANG_THISCALL *)(cm_lang_record *));
typedef cm_lang_u32 (__attribute__((stdcall)) *keyboard_fn)(cm_lang_u32);
typedef cm_lang_u32 (CM_LANG_CDECL *load_fn)(cm_lang_u32, cm_lang_u32);
typedef void (CM_LANG_CDECL *split_fn)(const char *, char *, char *, char *, char *);
typedef int (CM_LANG_CDECL *print_fn)(char *, const char *, ...);
typedef int (CM_LANG_CDECL *error_fn)(const char *, const char *, cm_lang_u32);
#define CURRENT (*(volatile cm_lang_u32 *)0x009eb2dcu)
#define TABLE (*(cm_lang_record *volatile *)0x00b679f0u)
#define FREE ((free_fn)0x00944c94u)
#define TEXT(va) ((const char *)(va))

static cm_lang_u32 string_size(const char *text) {
    const volatile unsigned char *p = (const volatile unsigned char *)text;
    cm_lang_u32 size = 1;
    while (*p++) ++size;
    return size;
}

/* Original REP MOVSD then REP MOVSB grouping, including the terminator. */
static void copy_words(char *dest, const char *source, cm_lang_u32 size) {
    for (cm_lang_u32 n = size >> 2; n; --n) {
        *(volatile cm_lang_u32 *)dest = *(const volatile cm_lang_u32 *)source;
        source += 4; dest += 4;
    }
    for (cm_lang_u32 n = size & 3u; n; --n)
        *(volatile char *)dest++ = *(const volatile char *)source++;
}

cm_lang_record *CM_LANG_THISCALL cm_lang_row_construct(cm_lang_record *self) {
    volatile cm_lang_record *row = self;
    row->key = 0;
    row->format = 0;
    row->text = 0;
    return self;
}

cm_lang_u32 CM_LANG_THISCALL cm_lang_row_destroy(cm_lang_record *self) {
    const volatile cm_lang_record *row = self;
    char *pointer = row->key;
    if (pointer) FREE(pointer);
    pointer = row->format;
    cm_lang_u32 result = (cm_lang_u32)pointer;
    if (pointer) result = FREE(pointer);
    pointer = row->text;
    if (pointer) result = FREE(pointer);
    return result;
}

cm_lang_u32 CM_LANG_CDECL cm_lang_teardown(void) {
    cm_lang_record *table = TABLE;
    cm_lang_u32 result = 0;
    if (table) {
        cm_lang_u32 header = (cm_lang_u32)table - 4u;
        cm_lang_u32 count = *(const volatile cm_lang_u32 *)header;
        ((array_destroy_fn)0x009462bbu)(table, 52u, count, cm_lang_row_destroy);
        result = FREE((void *)header);
        TABLE = 0;
    }
    CURRENT = 0xffffffffu;
    return result;
}

static cm_lang_u32 setup_error(void) {
    char filename[256], extension[256];
    ((split_fn)0x00944cffu)(TEXT(0x009ed840u), 0, 0, filename, extension);
    const char *version = cm_date_diagnostic_version(filename, extension, 422u);
    ((print_fn)0x00944c9fu)((char *)0x00ae24d0u, TEXT(0x009870e8u),
                          version, filename, extension, 422u);
    ((error_fn)0x005e8290u)(TEXT(0x009870e0u), TEXT(0x00ae24d0u), 0);
    cm_lang_u32 size = string_size(TEXT(0x009ed828u));
    *(volatile cm_lang_u32 *)0x00b67a34u = 0;
    copy_words((char *)0x00b65004u, TEXT(0x009ed828u), size);
    return 0;
}

cm_lang_u32 CM_LANG_CDECL cm_lang_available(cm_lang_u32 language) {
    if (CURRENT == 0xffffffffu) return setup_error();
    cm_lang_u32 count = *(const volatile cm_lang_u32 *)0x00b66170u;
    if (count == 0 || count >= 0x80000000u) return 0;
    for (cm_lang_u32 i = 0; i < count; ++i)
        if (*(const volatile cm_lang_u32 *)(0x00b64764u + i * 4u) == language) return 1;
    return 0;
}

cm_lang_u32 CM_LANG_CDECL cm_lang_setup(const char *filename, cm_lang_u32 memory_mode) {
    *(volatile cm_lang_u32 *)0x00b647b0u = memory_mode;
    copy_words((char *)0x00b6586cu, filename, string_size(filename));
    cm_lang_u32 layout = ((keyboard_fn)*(const volatile cm_lang_u32 *)0x009671d8u)(0) & 65535u;
    cm_lang_u32 primary = (layout & 1023u) - 4u;
    cm_lang_u32 target = 0x0066e94cu;
    if (primary <= 25u) {
        cm_lang_u32 index = *(const volatile unsigned char *)(0x0066e9a0u + primary);
        target = *(const volatile cm_lang_u32 *)(0x0066e960u + 4u * index);
    }
    cm_lang_u32 language = 0, secondary = layout & 0xfc00u;
    switch (target) {
        case 0x0066e7f0u: language = secondary == 0x400u; break;
        case 0x0066e812u: language = secondary == 0x800u ? 18u : 5u; break;
        case 0x0066e836u: language = 2; break;
        case 0x0066e84cu: language = 3; break;
        case 0x0066e862u: language = 4; break;
        case 0x0066e878u: language = 6; break;
        case 0x0066e88eu: language = 7; break;
        case 0x0066e8a4u: language = 8; break;
        case 0x0066e8bau: language = secondary == 0x400u ? 17u : 9u; break;
        case 0x0066e8deu: language = 10; break;
        case 0x0066e8f4u: language = 11; break;
        case 0x0066e90au: language = 12; break;
        case 0x0066e920u: language = 13; break;
        case 0x0066e936u: language = 14; break;
    }
    CURRENT = language;
    return ((load_fn)0x0066eb40u)(language, 0);
}

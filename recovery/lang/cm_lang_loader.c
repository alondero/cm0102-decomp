/* Primary official 3.9.68, VA 0066EB40..0066F4B5. Instruction-derived C.
 * Normal-return recovery; original transient SEH/unwind is not implemented.
 * Keep unchecked reads, widths, ownership and the two-stage key normalizer.
 */
#include "cm_lang_loader.h"
#include "../date/cm_date.h"

typedef void *(CM_LANG_CDECL *allocate_fn)(cm_lang_u32);
typedef cm_lang_u32 (CM_LANG_CDECL *free_fn)(void *);
typedef void *(CM_LANG_CDECL *open_fn)(const char *, const char *);
typedef cm_lang_u32 (CM_LANG_CDECL *read_fn)(void *, cm_lang_u32, cm_lang_u32, void *);
typedef int (CM_LANG_CDECL *close_fn)(void *);
typedef cm_lang_u32 (CM_LANG_CDECL *tell_fn)(void *);
typedef int (CM_LANG_CDECL *seek_fn)(void *, cm_lang_u32, cm_lang_u32);
typedef void (__attribute__((stdcall)) *construct_fn)(void *, cm_lang_u32, cm_lang_u32,
    cm_lang_record *(CM_LANG_THISCALL *)(cm_lang_record *),
    cm_lang_u32 (CM_LANG_THISCALL *)(cm_lang_record *));
typedef void (__attribute__((stdcall)) *destroy_fn)(void *, cm_lang_u32, cm_lang_u32,
    cm_lang_u32 (CM_LANG_THISCALL *)(cm_lang_record *));
typedef void (CM_LANG_CDECL *sort_fn)(void *, cm_lang_u32, cm_lang_u32, cm_lang_comparator);
typedef void (CM_LANG_THISCALL *progress_fn)(void *, const char *, cm_lang_u32, cm_lang_u32, cm_lang_u32);
typedef char *(CM_LANG_CDECL *format_fn)(char *, const char *, ...);
typedef void (CM_LANG_CDECL *split_fn)(const char *, char *, char *, char *, char *);
typedef int (CM_LANG_CDECL *print_fn)(char *, const char *, ...);
typedef int (CM_LANG_CDECL *error_fn)(const char *, const char *, cm_lang_u32);
#define ALLOCATE ((allocate_fn)0x00944cf1u)
#define FREE ((free_fn)0x00944c94u)
#define OPEN ((open_fn)0x00945b00u)
#define READ ((read_fn)0x00945bfeu)
#define CLOSE ((close_fn)0x00945a09u)
#define CURRENT (*(volatile cm_lang_u32 *)0x009eb2dcu)
#define MODE (*(volatile cm_lang_u32 *)0x00b647b0u)
#define COUNT (*(volatile cm_lang_u32 *)0x00b647b4u)
#define SELECTED (*(volatile cm_lang_u32 *)0x00b65868u)
#define LANGUAGES (*(volatile cm_lang_u32 *)0x00b6616cu)
#define AVAILABLE (*(volatile cm_lang_u32 *)0x00b66170u)
#define TABLE (*(cm_lang_record *volatile *)0x00b679f0u)
#define WORK ((char *)0x00b6619cu)
#define TEXT(va) ((const char *)(va))

static int signed_dword(cm_lang_u32 bits) {
    return bits < 0x80000000u ? (int)bits : -1 - (int)(0xffffffffu - bits);
}
static cm_lang_u32 swap_dword(cm_lang_u32 bits) {
    return (bits >> 24) | ((bits >> 8) & 0xff00u) |
           ((bits << 8) & 0xff0000u) | (bits << 24);
}
static cm_lang_u32 string_size(const char *text) {
    const volatile unsigned char *p = (const volatile unsigned char *)text;
    cm_lang_u32 size = 1;
    while (*p++) ++size;
    return size;
}
static void copy_string(char *dest, const char *source) {
    cm_lang_u32 size = string_size(source);
    for (cm_lang_u32 n = size >> 2; n; --n) {
        *(volatile cm_lang_u32 *)dest = *(const volatile cm_lang_u32 *)source;
        dest += 4; source += 4;
    }
    for (cm_lang_u32 n = size & 3u; n; --n)
        *(volatile char *)dest++ = *(const volatile char *)source++;
}
static void diagnostic(cm_lang_u32 line, cm_lang_u32 message) {
    char filename[256], extension[256];
    ((split_fn)0x00944cffu)(TEXT(0x009ed840u), 0, 0, filename, extension);
    const char *version = cm_date_diagnostic_version(filename, extension, line);
    ((print_fn)0x00944c9fu)((char *)0x00ae24d0u, TEXT(0x009870e8u),
                           version, filename, extension, line);
    ((error_fn)0x005e8290u)(TEXT(0x009870e0u), TEXT(0x00ae24d0u), 0);
    /* Original scans before clearing the flag, then MOVSD/MOVSB copies. */
    cm_lang_u32 size = string_size(TEXT(message));
    *(volatile cm_lang_u32 *)0x00b67a34u = 0;
    const char *source = TEXT(message);
    char *dest = (char *)0x00b65004u;
    for (cm_lang_u32 n = size >> 2; n; --n) {
        *(volatile cm_lang_u32 *)dest = *(const volatile cm_lang_u32 *)source;
        dest += 4; source += 4;
    }
    for (cm_lang_u32 n = size & 3u; n; --n)
        *(volatile char *)dest++ = *(const volatile char *)source++;
}
static void destroy_table(void) {
    cm_lang_record *table = TABLE;
    if (table) {
        cm_lang_u32 header = (cm_lang_u32)table - 4u;
        cm_lang_u32 count = *(const volatile cm_lang_u32 *)header;
        ((destroy_fn)0x009462bbu)(table, 52u, count, cm_lang_row_destroy);
        FREE((void *)header);
        TABLE = 0;
    }
}
static char *volatile *row_slot(cm_lang_u32 offset) {
    return (char *volatile *)((cm_lang_u32)TABLE + offset);
}
static void replace_work(char *volatile *slot) {
    char *old = *slot;
    if (old) FREE(old);
    char *value = ALLOCATE(string_size(WORK));
    *slot = value;
    copy_string(value, WORK); /* Rescan after allocator and publishing slot. */
}
static void strip_braces(cm_lang_u32 offset) {
    cm_lang_record *base = TABLE;
    cm_lang_u32 i = 0, n = 0;
    char *key = *(char *volatile *)((cm_lang_u32)base + offset);
    while (*(volatile char *)(key + i)) {
        key = *(char *volatile *)((cm_lang_u32)base + offset);
        char ch = *(volatile char *)(key + i);
        if (ch != '{' && ch != '}') {
            *(volatile char *)(key + n++) = ch;
            base = TABLE;
        }
        ++i;
        key = *(char *volatile *)((cm_lang_u32)base + offset);
    }
    *(volatile char *)(key + n) = 0;
}
static void normalize_key(cm_lang_u32 offset) {
    char *volatile *slot = row_slot(offset);
    cm_lang_u32 i = 0, n = 0;
    for (;;) {
        cm_lang_u32 low = i & 65535u;
        int index = low < 32768u ? (int)low : (int)low - 65536;
        char *key = *slot;
        char ch = *(volatile char *)(key + index);
        if (!ch) break;
        if (ch != ' ') {
            if (ch == '\\' && *(volatile char *)(key + index + 1) == 'n') {
                *(volatile char *)(WORK + n++) = '\n'; ++i;
            } else *(volatile char *)(WORK + n++) = ch;
        }
        ++i;
    }
    *(volatile char *)(WORK + n) = 0;
    replace_work(slot);
}
static void progress_update(char *label, cm_lang_u32 position, cm_lang_u32 count) {
    ((progress_fn)0x00802f80u)((void *)0x00b74450u, label, position, count, 0xffffffffu);
}

/* VA 0090C140..0090C19A: positive +4/SEEK_END, ignored seek results, strict
 * fread result == 1, restore saved position before testing the read marker. */
cm_lang_u32 CM_LANG_CDECL cm_lang_byte_order_worker(volatile cm_lang_u32 *argument) {
    void *file = (void *)*argument;
    cm_lang_u32 position = ((tell_fn)0x00946aaeu)(file);
    ((seek_fn)0x00946a22u)(file, 4u, 2u);
    cm_lang_u32 read = READ((void *)argument, 4u, 1u, file);
    ((seek_fn)0x00946a22u)(file, position, 0u);
    if (read != 1u) return 0xf3fffff3u;
    cm_lang_u32 marker = *argument;
    return marker == 0xf1fffff1u || marker == 0xf2fffff2u ? marker : 0xf3fffff3u;
}

cm_lang_u32 CM_LANG_CDECL cm_lang_load_worker(cm_lang_u32 requested, cm_lang_u32 progress,
                                            cm_lang_u32 prior_version, cm_lang_u32 prior_id) {
    volatile cm_lang_u32 version = prior_version, id = prior_id;
    char label[200];
    if (CURRENT == 0xffffffffu) { diagnostic(462u, 0x009ed828u); return 0; }
    if (progress) ((format_fn)0x0066f4e0u)(label, TEXT(0x009ed8fcu));
    else label[0] = 0;
    destroy_table();
    CURRENT = 0xffffffffu;
    void *file = OPEN(TEXT(0x00b6586cu), TEXT(0x0098f928u));
    if (!file) { diagnostic(479u, 0x009ed8dcu); return 0; }
    cm_lang_u32 native_order = cm_lang_byte_order(file) != 0xf1fffff1u;
    READ((void *)&version, 4u, 1u, file);
    if (!native_order) version = swap_dword(version);
    if (version != 1u && version != 2u) {
        diagnostic(495u, 0x009ed8c0u); CLOSE(file); return 0;
    }
    READ((void *)0x00b647b4u, 4u, 1u, file);
    READ((void *)0x00b6616cu, 4u, 1u, file);
    if (!native_order) { COUNT = swap_dword(COUNT); LANGUAGES = swap_dword(LANGUAGES); }
    cm_lang_u32 languages = LANGUAGES;
    SELECTED = 0xffffffffu;
    AVAILABLE = 0;
    if (signed_dword(languages) > 0) {
        cm_lang_u32 ordinal = 0;
        do {
            READ((void *)&id, 4u, 1u, file);
            if (!native_order) id = swap_dword(id);
            *(volatile cm_lang_u32 *)(0x00b64764u + AVAILABLE * 4u) = id;
            if (version != 2u || (id != 0u && id != 1u)) {
                if (id == requested) SELECTED = ordinal;
                AVAILABLE = AVAILABLE + 1u;
            }
            ++ordinal;
        } while (signed_dword(ordinal) < signed_dword(LANGUAGES));
        if (SELECTED != 0xffffffffu) {
            CURRENT = requested;
            if (MODE == 1u && requested != 0u) {
                cm_lang_u32 count = COUNT;
                void *header = ALLOCATE(count * 52u + 4u);
                cm_lang_record *table = 0;
                if (header) {
                    table = (cm_lang_record *)((cm_lang_u32)header + 4u);
                    *(volatile cm_lang_u32 *)header = count;
                    ((construct_fn)0x009463afu)(table, 52u, count,
                        cm_lang_row_construct, cm_lang_row_destroy);
                }
                TABLE = table;
                if (!table) {
                    diagnostic(565u, 0x009ed86cu);
                    destroy_table(); CURRENT = 0xffffffffu;
                    CLOSE(file); return 0;
                }
                if (progress) progress_update(label, 0, COUNT);
                cm_lang_u32 row = 0, offset = 0;
                count = COUNT;
                if (signed_dword(count) > 0) {
                    do {
                        READ(WORK, 1u, 2048u, file); replace_work(row_slot(offset));
                        READ(WORK, 1u, 30u, file); replace_work(row_slot(offset + 4u));
                        strip_braces(offset);
                        cm_lang_u32 ordinal = 1;
                        while (signed_dword(ordinal) < signed_dword(SELECTED)) {
                            READ(WORK, 1u, 2048u, file); READ(WORK, 4u, 10u, file); ++ordinal;
                        }
                        READ(WORK, 1u, 2048u, file); replace_work(row_slot(offset + 8u));
                        for (cm_lang_u32 n = 0; n < 10; ++n) {
                            cm_lang_u32 order_offset = offset + 12u + 4u * n;
                            READ((void *)((cm_lang_u32)TABLE + order_offset), 4u, 1u, file);
                            if (!native_order) {
                                volatile cm_lang_u32 *slot = (volatile cm_lang_u32 *)((cm_lang_u32)TABLE + order_offset);
                                *slot = swap_dword(*slot);
                            }
                        }
                        ordinal = SELECTED + 1u;
                        while (signed_dword(ordinal) < signed_dword(LANGUAGES)) {
                            READ(WORK, 1u, 2048u, file); READ(WORK, 4u, 10u, file); ++ordinal;
                        }
                        if (progress) {
                            count = COUNT;
                            if (signed_dword(row) % 250 == 0 || row == count - 1u)
                                progress_update(label, row + 1u, count);
                        }
                        normalize_key(offset);
                        offset += 52u; ++row; count = COUNT;
                    } while (signed_dword(row) < signed_dword(count));
                }
                ((sort_fn)0x00945333u)(TABLE, count, 52u, cm_lang_key_compare);
            }
            CLOSE(file); return 1;
        }
    }
    copy_string((char *)0x00b65004u, TEXT(0x009ed894u));
    CLOSE(file);
    if (AVAILABLE == 0) return 0;
    cm_lang_u32 first = *(const volatile cm_lang_u32 *)0x00b64764u;
    CURRENT = first;
    return cm_lang_load(first, progress);
}

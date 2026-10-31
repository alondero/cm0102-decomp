/* Official primary 3.9.68, preferred-image VAs. See LANG-LOOKUP.md.
 * Normal-return recovery; allocator, file I/O and diagnostics remain explicit
 * CRT/UI boundaries. Retain unchecked lengths, reads, seeks and original quirks.
 */
#include "cm_lookup.h"
#include "../date/cm_date.h"

typedef void *(CM_LANG_CDECL *allocate_fn)(cm_lang_u32);
typedef void (CM_LANG_CDECL *free_fn)(void *);
typedef void *(CM_LANG_CDECL *open_fn)(const char *, const char *);
typedef cm_lang_u32 (CM_LANG_CDECL *read_fn)(void *, cm_lang_u32, cm_lang_u32, void *);
typedef int (CM_LANG_CDECL *seek_fn)(void *, cm_lang_u32, cm_lang_u32);
typedef int (CM_LANG_CDECL *close_fn)(void *);
typedef void (CM_LANG_CDECL *split_fn)(const char *, char *, char *, char *, char *);
typedef int (CM_LANG_CDECL *print_fn)(char *, const char *, ...);
typedef int (CM_LANG_CDECL *error_fn)(const char *, const char *, cm_lang_u32);
#define ALLOCATE ((allocate_fn)0x00944cf1u)
#define FREE ((free_fn)0x00944c94u)
#define OPEN ((open_fn)0x00945b00u)
#define READ ((read_fn)0x00945bfeu)
#define SEEK ((seek_fn)0x00946a22u)
#define CLOSE ((close_fn)0x00945a09u)
#define MODE (*(volatile cm_lang_u32 *)0x00b647b0u)
#define COUNT (*(volatile cm_lang_u32 *)0x00b647b4u)
#define SELECTED (*(volatile cm_lang_u32 *)0x00b65868u)
#define LANGUAGES (*(volatile cm_lang_u32 *)0x00b6616cu)
#define TABLE (*(cm_lang_record *volatile *)0x00b679f0u)
#define WORK ((char *)0x00b6619cu)
#define TEXT(va) ((const char *)(va))

static int signed_dword(cm_lang_u32 bits) {
    return bits < 0x80000000u ? (int)bits : -1 - (int)(0xffffffffu - bits);
}

static cm_lang_u32 string_size(const char *text) {
    cm_lang_u32 size = 1;
    while (*text++) ++size;
    return size;
}

/* MOVSD/MOVSB copies with length including the terminator. Arbitrary partial
 * overlaps are outside validation; keep dword-before-tail access order. */
static void copy_string(char *dest, const char *source) {
    cm_lang_u32 size = string_size(source), words = size / 4u;
    while (words--) {
        *(volatile cm_lang_u32 *)dest = *(const volatile cm_lang_u32 *)source;
        dest += 4; source += 4;
    }
    for (cm_lang_u32 tail = size & 3u; tail; --tail) *dest++ = *source++;
}

static void replace_string(char *volatile *slot, const char *source) {
    char *old = *slot;
    if (old) FREE(old);
    char *copy = ALLOCATE(string_size(source));
    *slot = copy;
    /* Original recomputes the length AFTER allocation and publishing the slot. */
    copy_string(copy, source);
}

static int compare_bytes(const unsigned char *left, const unsigned char *right) {
    for (;;) {
        unsigned char a = *left++, b = *right++;
        if (a != b) return a < b ? -1 : 1;
        if (!a) return 0;
    }
}

/* VA 00682060..0068209E: cdecl, full EAX -1/0/+1, unsigned byte ordering. */
int CM_LANG_CDECL cm_lang_key_compare(const void *left, const void *right) {
    const volatile cm_lang_record *a = left, *b = right;
    const unsigned char *right_text = (const unsigned char *)b->key;
    const unsigned char *left_text = (const unsigned char *)a->key;
    return compare_bytes(left_text, right_text);
}

/* VA 00946464..009464EF. Identified bsearch-style CRT support. Even lengths
 * choose the lower middle entry. Encode every address operation as x86 bits. */
void *CM_LANG_CDECL cm_lang_bsearch(const void *key, const void *table,
                                  cm_lang_u32 count, cm_lang_u32 stride,
                                  cm_lang_comparator compare) {
    cm_lang_u32 low = (cm_lang_u32)table;
    cm_lang_u32 high = low + (count - 1u) * stride;
    while (low <= high) {
        cm_lang_u32 half = count >> 1;
        if (!half) {
            if (!count) return (void *)0;
            return compare(key, (const void *)low) == 0 ? (void *)low : (void *)0;
        }
        cm_lang_u32 odd = count & 1u;
        cm_lang_u32 middle = low + (odd ? half : half - 1u) * stride;
        int result = compare(key, (const void *)middle);
        if (!result) return (void *)middle;
        if (result < 0) {
            high = middle - stride;
            count = odd ? half : half - 1u;
        } else {
            low = middle + stride;
            count = half;
        }
    }
    return (void *)0;
}

/* VA 0066FBB0..006701EE. The optional index is written ONLY by memory hits.
 * Transient original SEH, stack probing and exception unwind are excluded.
 */
cm_lang_u32 CM_LANG_CDECL cm_lang_lookup(cm_lang_record *out, const char *source,
                                       cm_lang_u32 *index) {
    char key[2048], candidate[2048];
    cm_lang_u32 i = 0, n = 0;
    while (source[i]) {
        char ch = source[i];
        if (ch != '{' && ch != '}' && ch != ' ') {
            if (ch == '\\' && source[i + 1u] == 'n') { key[n++] = '\n'; ++i; }
            else if (ch == '\\' && source[i + 1u] == '"') { key[n++] = '"'; ++i; }
            else key[n++] = ch;
        }
        ++i;
    }
    key[n] = 0;
    while (n && key[n - 1u] == ' ') key[--n] = 0;
    cm_lang_u32 found = 0;
    volatile cm_lang_record *dest = out;
    if (MODE == 1) {
        cm_lang_record query;
        query.key = ALLOCATE(string_size(key));
        copy_string(query.key, key);
        cm_lang_u32 count = COUNT;
        cm_lang_record *base = TABLE;
        const volatile cm_lang_record *hit = cm_lang_bsearch(&query, base, count, 52, cm_lang_key_compare);
        if (hit) {
            replace_string(&dest->key, hit->key);
            replace_string(&dest->format, hit->format);
            replace_string(&dest->text, hit->text);
            for (i = 0; i < 10; ++i) dest->order[i] = hit->order[i];
            if (index) *index = ((cm_lang_u32)hit - (cm_lang_u32)TABLE) / 52u;
            found = 1;
        }
        if (query.key) FREE(query.key);
    } else {
        void *file = OPEN(TEXT(0x00b6586cu), TEXT(0x0098f928u));
        if (!file) {
            char filename[256], extension[256];
            ((split_fn)0x00944cffu)(TEXT(0x009ed840u), 0, 0, filename, extension);
            const char *version = cm_date_diagnostic_version(filename, extension, 3494);
            ((print_fn)0x00944c9fu)((char *)0x00ae24d0u, TEXT(0x009870e8u),
                                  version, filename, extension, 3494);
            ((error_fn)0x005e8290u)(TEXT(0x009870e0u), TEXT(0x00ae24d0u), 0);
            *(volatile cm_lang_u32 *)0x00b67a34u = 0;
            copy_string((char *)0x00b65004u, TEXT(0x009ed8dcu));
            return 0;
        }
        SEEK(file, LANGUAGES * 4u + 12u, 0);
        cm_lang_u32 row = 0;
        if (signed_dword(COUNT) > 0) {
            do {
                if (found) break;
                READ(WORK, 1, 2048, file);
                replace_string(&dest->key, WORK);
                i = n = 0;
                while (dest->key[i]) {
                    char ch = dest->key[i];
                    if (ch != ' ') {
                        if (ch == '\\' && dest->key[i + 1u] == 'n') { candidate[n++] = '\n'; ++i; }
                        else candidate[n++] = ch;
                    }
                    ++i;
                }
                candidate[n] = 0;
                if (compare_bytes((const unsigned char *)candidate, (const unsigned char *)key) == 0) {
                    READ(WORK, 1, 30, file);
                    replace_string(&dest->format, WORK);
                    cm_lang_u32 language = 1;
                    while (signed_dword(language) < signed_dword(SELECTED)) {
                        READ(WORK, 1, 2048, file);
                        READ(WORK, 4, 10, file);
                        ++language;
                    }
                    READ(WORK, 1, 2048, file);
                    replace_string(&dest->text, WORK);
                    for (i = 0; i < 10; ++i) READ((void *)&dest->order[i], 4, 1, file);
                    language = SELECTED + 1u;
                    while (signed_dword(language) < signed_dword(LANGUAGES)) {
                        READ(WORK, 1, 2048, file);
                        READ(WORK, 4, 10, file);
                        ++language;
                    }
                    found = 1;
                } else SEEK(file, LANGUAGES * 2088u - 2058u, 1);
                ++row;
            } while (signed_dword(row) < signed_dword(COUNT));
        }
        CLOSE(file);
    }
    return found;
}

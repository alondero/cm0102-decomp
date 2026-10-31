/* Instruction-derived primary 3.9.68. Preferred-image VAs; see LANG-FORMAT.md.
 * Preserve the unsafe original buffer/markup assumptions. No bounds checking
 * or invented translation rules. Transient SEH/unwinding is not recovered. */
#include "cm_lang.h"

typedef __builtin_va_list cm_args;
typedef struct translation {
    char *key, *format, *text;
    cm_lang_u32 order[10];
} translation;
_Static_assert(sizeof(translation) == 52, "original lookup record layout");
typedef int (CM_LANG_CDECL *lookup_fn)(translation *, const char *, cm_lang_u32);
typedef int (CM_LANG_CDECL *vformat_fn)(char *, const char *, cm_args);
typedef char *(CM_LANG_CDECL *find_fn)(const char *, const char *);
typedef char *(CM_LANG_CDECL *morph_fn)(const char *, const char *, const char *,
                                     unsigned char, unsigned char);
typedef void (CM_LANG_CDECL *free_fn)(void *);
#define LOOKUP ((lookup_fn)0x0066fbb0u)
#define VFORMAT ((vformat_fn)0x009470ceu)
#define FIND ((find_fn)0x00945a60u)
#define MORPH ((morph_fn)0x006701f0u)
#define FREE ((free_fn)0x00944c94u)
#define PREVIEW (*(volatile cm_lang_u32 *)0x00b679f4u)
#define WORK ((volatile char *)0x00b6619cu)
#define PARTS ((char *volatile *)0x00b66174u)
#define PREFIX ((volatile char *)0x00b669c4u)
#define SUFFIX ((volatile char *)0x00b671ecu)

cm_lang_u32 CM_LANG_CDECL cm_lang_current(void) {
    return *(volatile cm_lang_u32 *)0x009eb2dcu;
}

static void copy_bytes(volatile char *dest, const char *src) {
    do { *dest++ = *src; } while (*src++);
}

/* VA 0066F4E0..0066FBAE, cdecl variadic, returns output after leading spaces.
 * The three lookup allocations are released in key/format/text order on every
 * normal path, even lookup failure or empty translation. */
char *CM_LANG_CDECL cm_lang_format(char *output, const char *format, ...) {
    translation tr;
    cm_args args;
    char *dest;
    const char *source;
    cm_lang_u32 language, preview;
    unsigned int i, n, count;
    unsigned char properties[20];
    char last = ' ';
    tr.key = tr.format = tr.text = (char *)0;
    __builtin_va_start(args, format);
    i = 0;
    while (format[i] == ' ') { output[i] = ' '; ++i; }
    source = format + i;
    dest = output + i;
    language = cm_lang_current();
    if (language != 0xffffffffu && language != 0u &&
        LOOKUP(&tr, source, 0) && tr.text[0]) {
        preview = PREVIEW;
        for (i = 0; i < 20; ++i) properties[i] = 255;
        count = 0;
        for (i = 0; source[i]; ++i) {
            char ch = source[i];
            if (ch == '<') ++count;
            if (ch == '{' || ch == '[') {
                unsigned char value = preview ? 0 :
                    (unsigned char)__builtin_va_arg(args, cm_lang_u32);
                if (i > 0 && source[i - 1] == '>' && count > 0)
                    properties[2 * count - 1] = value;
                else properties[2 * count] = value;
            }
        }
        if (preview) {
            for (i = 0; i < 20; i += 2) { WORK[i] = ' '; WORK[i + 1] = '~'; }
            WORK[20] = 0;
        } else VFORMAT((char *)WORK, tr.format, args);
        PARTS[0] = (char *)WORK;
        n = 1;
        for (i = 0; WORK[i]; ++i) {
            if (WORK[i] == '~') {
                WORK[i] = 0;
                PARTS[n++] = (char *)WORK + i + 1;
            }
        }
        PREFIX[0] = 0;
        i = n = count = 0;
        while (tr.text[i]) {
            if (tr.text[i] == ' ') PREFIX[0] = 0;
            if (tr.text[i] == '{') {
                ++i;
                unsigned int p = 0;
                while (tr.text[i] != '}' && tr.text[i]) PREFIX[p++] = tr.text[i++];
                PREFIX[p] = 0;
                ++i;
            }
            if (tr.text[i] == 8) {
                cm_lang_u32 index = tr.order[count++];
                char *value;
                if (properties[2 * index] == 255 && properties[2 * index + 1] == 255)
                    value = PARTS[index];
                else {
                    SUFFIX[0] = 0;
                    if (tr.text[i + 1] == '{') {
                        i += 2;
                        unsigned int p = 0;
                        while (tr.text[i] != '}' && tr.text[i]) SUFFIX[p++] = tr.text[i++];
                        SUFFIX[p] = 0;
                    }
                    value = MORPH(PARTS[index], (const char *)PREFIX, (const char *)SUFFIX,
                                  properties[2 * index], properties[2 * index + 1]);
                }
                while (*value) dest[n++] = *value++;
                ++i;
                if (tr.text[i] == ' ') PREFIX[0] = 0;
            }
            char ch = tr.text[i];
            if (!ch) break;
            if (ch == '%' && last == '%') last = ' ';
            else { last = ch; dest[n++] = ch; }
            ++i;
        }
        dest[n] = 0;
    } else {
        preview = PREVIEW;
        i = n = 0;
        unsigned int annotated = 0;
        while (source[i]) {
            char ch = source[i];
            if (ch == '{' || ch == '[') {
                if (!preview) (void)__builtin_va_arg(args, cm_lang_u32);
                if (source[i + 1] == '}' || source[i + 1] == ']') goto next;
            }
            if (ch == '}' || ch == ']') {
                if (!i || source[i - 1] == '{' || source[i - 1] == '[') goto next;
            }
            if (ch == '<') {
                while (source[i] && source[i] != '%') ++i;
                annotated = 1;
            }
            ch = source[i];
            if (ch == ' ' && annotated) {
                while (source[i] && source[i] != '>') ++i;
                ++i;
                annotated = 0;
                ch = source[i];
                if (ch == '{' || ch == '[') {
                    if (!preview) (void)__builtin_va_arg(args, cm_lang_u32);
                    if (source[i + 1] == '}' || source[i + 1] == ']') goto next;
                }
                if (ch == '}' || ch == ']') {
                    if (!i || source[i - 1] == '{' || source[i - 1] == '[') goto next;
                }
            }
            WORK[n++] = ch;
            if (!ch) break;
next:
            ++i;
        }
        WORK[n] = 0;
        if (!preview) {
            VFORMAT(dest, (const char *)WORK, args);
            if (FIND(dest, (const char *)0x009ed914u) || FIND(dest, (const char *)0x009ed910u)) {
                unsigned int length = 0;
                while (dest[length]) ++length;
                if (length < 1000) {
                    char text[1000], before[100], word[100], after[100];
                    copy_bytes(text, dest);
                    i = n = 0;
                    while (text[i]) {
                        if (text[i] != '{' && text[i] != '[') dest[n++] = text[i++];
                        else {
                            before[0] = after[0] = 0;
                            unsigned int p = 0;
                            if (i && text[i - 1] != ' ' && text[i - 1] != '\n') {
                                unsigned int start = ++i;
                                while (text[i] != '}') after[p++] = text[i++];
                                ++i;
                                after[p] = 0;
                                while (start && n) {
                                    if (text[start] == ' ' || text[start] == '\n') break;
                                    --start; --n;
                                }
                                /* The original starts one byte BEFORE start;
                                 * do not fix this apparent backward-scan defect. */
                                const char *q = text + start - 1;
                                p = 0;
                                if (*q != '{' && *q != '[') {
                                    do { word[p++] = *q++; } while (*q != '{');
                                }
                                word[p] = 0;
                            } else {
                                ++i;
                                while (text[i] != '}') before[p++] = text[i++];
                                ++i; before[p] = 0;
                                p = 0;
                                while (text[i] && text[i] != ' ' && text[i] != '{' && text[i] != '[')
                                    word[p++] = text[i++];
                                word[p] = 0;
                                if (text[i] == '{' || text[i] == '[') {
                                    p = 0; ++i;
                                    while (text[i] != '}') after[p++] = text[i++];
                                    ++i; after[p] = 0;
                                }
                            }
                            char *value = MORPH(word, before, after, 100, 100);
                            while (*value) dest[n++] = *value++;
                        }
                    }
                    dest[n] = 0;
                }
            }
        }
    }
    if (tr.key) FREE(tr.key);
    if (tr.format) FREE(tr.format);
    if (tr.text) FREE(tr.text);
    __builtin_va_end(args);
    return dest;
}

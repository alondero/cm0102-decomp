/* Instruction-derived regions of primary VA 006701F0. Parent incomplete.
 * Keep partial initialization, callback ordering, fixed original data and the
 * shared language reread. Do not apply this file to the supplied executable.
 */
#include "cm_lang_morph_partial.h"
#include "cm_lang_morph_tables.h"
typedef int (CM_LANG_CDECL *print_fn)(char *, const char *, ...);
typedef int (CM_LANG_CDECL *upper_fn)(int);
typedef char *(CM_LANG_CDECL *find_fn)(const char *, const char *);
#define PRINT ((print_fn)0x00944c9fu)
#define UPPER ((upper_fn)0x009471cdu)
#define LOWER ((upper_fn)0x009465fcu)
#define FIND ((find_fn)0x00945a60u)
#define OUTPUT ((char *)0x00b64804u)
#define TEXT(va) ((const char *)(va))
static char *local(unsigned char *frame, cm_lang_u32 offset) {
    return (char *)(frame + offset);
}
static const char *argument(unsigned char *frame, cm_lang_u32 offset) {
    return *(const char *volatile *)(frame + offset);
}
static int signed_byte(unsigned char byte) {
    return byte < 128u ? (int)byte : (int)byte - 256;
}
static cm_lang_u32 size_of(const char *text) {
    const volatile unsigned char *cursor = (const volatile unsigned char *)text;
    cm_lang_u32 remaining = 0xffffffffu;
    do {
        unsigned char byte = *cursor++;
        --remaining;
        if (!byte) break;
    } while (remaining);
    return ~remaining;
}
static void copy_bytes(char *to, const char *from, cm_lang_u32 size) {
    for (cm_lang_u32 count = size >> 2; count; --count) {
        *(volatile cm_lang_u32 *)to = *(const volatile cm_lang_u32 *)from;
        to += 4; from += 4;
    }
    for (cm_lang_u32 count = size & 3u; count; --count)
        *(volatile char *)to++ = *(const volatile char *)from++;
}
static int equal(const char *left, const char *right) {
    const volatile unsigned char *a = (const volatile unsigned char *)left;
    const volatile unsigned char *b = (const volatile unsigned char *)right;
    for (;;) {
        unsigned char x = *a, y = *b;
        if (x != y) return 0;
        if (!x) return 1;
        x = a[1]; y = b[1];
        if (x != y) return 0;
        a += 2; b += 2;
        if (!x) return 1;
    }
}
cm_lang_u32 CM_LANG_CDECL cm_lang_morph_initialize(unsigned char *frame) {
    const char *word = argument(frame, 0x281cu);
    for (cm_lang_u32 index = 0; index < 512u; ++index)
        *(volatile cm_lang_u32 *)(frame + 0x1818u + index * 4u) = 0;
    *(volatile char *)OUTPUT = 0;
    *(volatile char *)local(frame, 0x18u) = 0;
    *(volatile char *)local(frame, 0x818u) = 0;
    cm_lang_u32 size = size_of(word);
    *(volatile cm_lang_u32 *)(frame + 0x10u) = 0;
    *(volatile cm_lang_u32 *)(frame + 0x14u) = 1;
    copy_bytes(local(frame, 0x1018u), word, size);
    return size;
}
static cm_lang_u32 word_length(const char *word) {
    if (!*(const volatile unsigned char *)word) return 0;
    cm_lang_u32 count = 0;
    do { ++count; } while (*(const volatile unsigned char *)(word + count));
    return count;
}
static char *compose(unsigned char *frame) {
    char *prefix = local(frame, 0x818u), *suffix = local(frame, 0x18u);
    if (!*(volatile char *)OUTPUT)
        PRINT(OUTPUT, TEXT(0x009f1584u), prefix, local(frame, 0x1018u), suffix);
    else {
        char *temporary = local(frame, 0x2018u);
        PRINT(temporary, TEXT(0x009f1584u), prefix, OUTPUT, suffix);
        copy_bytes(OUTPUT, temporary, size_of(temporary));
    }
    if (*(volatile cm_lang_u32 *)0x009eb2dcu == 2u) {
        cm_lang_u32 skip = 3;
        char *match = FIND(OUTPUT, TEXT(0x009f157cu));
        if (!match) match = FIND(OUTPUT, TEXT(0x009f1574u));
        if (!match) {
            match = FIND(OUTPUT, TEXT(0x009f156cu));
            if (!match) match = FIND(OUTPUT, TEXT(0x009f1564u));
            if (match) { *(volatile char *)match = 'a'; skip = 2; }
        }
        if (match) {
            *(volatile char *)(match + 1) = 'u';
            cm_lang_u32 index = 2;
            unsigned char byte = *(const volatile unsigned char *)(match + skip + 2);
            do {
                *(volatile unsigned char *)(match + index) = byte;
                byte = *(const volatile unsigned char *)(match + skip + index + 1);
                ++index;
            } while (byte);
            *(volatile unsigned char *)(match + index) = byte;
        }
    }
    return OUTPUT;
}
char *CM_LANG_CDECL cm_lang_morph_language_zero(unsigned char *frame) {
    const char *prefix = argument(frame, 0x2820u);
    char *word = local(frame, 0x1018u);
    if (equal(prefix, TEXT(0x009f15ecu)))
        *(volatile char *)word = (char)UPPER(signed_byte(*(const volatile unsigned char *)word));
    const char *suffix = argument(frame, 0x2824u);
    if (equal(suffix, TEXT(0x0099c368u)) || equal(suffix, TEXT(0x009f15e8u))) {
        cm_lang_u32 length = word_length(word);
        if (!length) return 0;
        unsigned char last = *(const volatile unsigned char *)(word + length - 1);
        PRINT(local(frame, 0x18u), TEXT(last == 'S' || last == 's' ? 0x009f15e0u : 0x009f15e4u));
    }
    cm_lang_u32 lower = equal(prefix, TEXT(0x009f15dcu)), capital = 0;
    if (!lower) capital = equal(prefix, TEXT(0x009f15d0u));
    if (lower || capital) {
        cm_lang_u32 index = (cm_lang_u32)signed_byte(*(const volatile unsigned char *)word) - 0x41u;
        cm_lang_u32 table = capital ? 0x0067fe0cu : 0x0067fdccu;
        cm_lang_u32 vowel = index <= 52u && *(const volatile unsigned char *)(table + index) == 0;
        PRINT(local(frame, 0x818u), TEXT(capital ? (vowel ? 0x009f15ccu : 0x009f15c8u)
                                              : (vowel ? 0x009f15d8u : 0x009f15d4u)));
    }
    return compose(frame);
}
char *CM_LANG_CDECL cm_lang_morph_language_seven(unsigned char *frame) {
    const char *suffix = argument(frame, 0x2824u);
    char *word = local(frame, 0x1018u);
    if (equal(suffix, TEXT(0x0099c368u)) || equal(suffix, TEXT(0x009f15e8u))) {
        cm_lang_u32 length = word_length(word);
        if (!length) return 0;
        unsigned char last = *(const volatile unsigned char *)(word + length - 1);
        PRINT(local(frame, 0x18u), TEXT(last == 'S' || last == 's' ? 0x00ad9c64u : 0x0099c368u));
    }
    return compose(frame);
}
/* Numeric language ten's three-pointer rows differ from the four-pointer
 * morphology helpers. Empty key terminates before comparing the local word. */
static void replace_ten_word(char *word, cm_lang_u32 column) {
    const char *volatile *row = (const char *volatile *)0x009eb5bcu;
    const char *key = row[0];
    while (*(const volatile unsigned char *)key) {
        if (equal(key, word)) {
            const char *replacement = row[column];
            copy_bytes(word, replacement, size_of(replacement));
            return;
        }
        row += 3;
        key = row[0];
    }
}
static cm_lang_u32 ending_last_byte_selects_zero(unsigned char byte, cm_lang_u32 table,
                                                cm_lang_u32 base, cm_lang_u32 bound) {
    cm_lang_u32 index = (cm_lang_u32)signed_byte(byte) - base;
    return index <= bound && *(const volatile unsigned char *)(table + index) == 0;
}
/* Languages ten/eight/six share the verified selection/copy/return loop.
 * Six's last-word table has a different index base and bound. */
static char *word_ending(unsigned char *frame, cm_lang_u32 ending_list,
                         cm_lang_u32 stem_dispatch, cm_lang_u32 word_dispatch,
                         cm_lang_u32 word_base, cm_lang_u32 word_bound) {
    char *word = local(frame, 0x1018u);
    const char *volatile *row = (const char *volatile *)ending_list;
    const char *key = row[0];
    cm_lang_u32 *position = (cm_lang_u32 *)(frame + 0x10u);
    if (!equal(key, TEXT(0x00989ad8u))) {
        do {
            if (*(volatile cm_lang_u32 *)position) break;
            cm_lang_u32 ending_size = size_of(key) - 1u;
            cm_lang_u32 word_size = size_of(word) - 1u;
            if (word_size - ending_size != 0u) {
                ending_size = size_of(key) - 1u;
                cm_lang_u32 caller = (cm_lang_u32)argument(frame, 0x281cu) - ending_size;
                word_size = size_of(word) - 1u;
                /* Original caller bytes, not the transformed local copy.
                 * Keep unsigned subtraction, including the short-word case. */
                if (equal((const char *)(caller + word_size), key)) {
                    ending_size = size_of(key) - 1u;
                    word_size = size_of(word) - 1u;
                    *(volatile cm_lang_u32 *)position = (cm_lang_u32)word - ending_size + word_size;
                }
            }
            key = *++row;
        } while (!equal(key, TEXT(0x00989ad8u)));
    }
    if (*(volatile cm_lang_u32 *)position) {
        const char *ending = (const char *)*(volatile cm_lang_u32 *)position;
        cm_lang_u32 index = 0;
        if (!equal(word, ending)) {
            do {
                unsigned char byte = *(const volatile unsigned char *)(word + index);
                if (!byte) break;
                *(volatile unsigned char *)(frame + 0x1818u + index) = byte;
                ++index;
            } while (!equal(word + index, ending));
        }
        if (!index) return 0;
        if (ending_last_byte_selects_zero(*(const volatile unsigned char *)(word + index - 1u), stem_dispatch, 0x53u, 39u))
            PRINT(OUTPUT, TEXT(0x00991944u), word);
        else {
            const char *suffix = argument(frame, 0x2824u);
            PRINT(OUTPUT, TEXT(0x009f1584u), local(frame, 0x1818u), suffix, ending);
        }
    } else {
        cm_lang_u32 length = word_length(word);
        if (!length) return 0;
        if (ending_last_byte_selects_zero(*(const volatile unsigned char *)(word + length - 1u), word_dispatch, word_base, word_bound))
            PRINT(OUTPUT, TEXT(0x00991944u), word);
        else {
            const char *suffix = argument(frame, 0x2824u);
            PRINT(OUTPUT, TEXT(0x0099ec04u), word, suffix);
        }
    }
    /* These original out-of-line returns bypass the common language reread. */
    return OUTPUT;
}
static void lower_word_in_place(char *word) {
    unsigned char byte = *(const volatile unsigned char *)word;
    if (byte) {
        char *cursor = word;
        do {
            *(volatile char *)cursor = (char)LOWER(signed_byte(byte));
            byte = *(const volatile unsigned char *)++cursor;
        } while (byte);
    }
}
char *CM_LANG_CDECL cm_lang_morph_language_ten(unsigned char *frame) {
    const char *prefix = argument(frame, 0x2820u);
    char *word = local(frame, 0x1018u);
    if (equal(prefix, TEXT(0x009f15ecu))) {
        *(volatile char *)word = (char)UPPER(signed_byte(*(const volatile unsigned char *)word));
        return compose(frame);
    }
    if (equal(prefix, TEXT(0x009f15c0u)) && !equal(word, TEXT(0x009f15b8u))
            && !equal(word, TEXT(0x0099c8fcu)) && !equal(word, TEXT(0x009f15acu))) {
        lower_word_in_place(word);
        return compose(frame);
    }
    cm_lang_u32 column = 0, uppercase = 0;
    if (equal(prefix, TEXT(0x009f15a0u))) { column = 1; uppercase = 1; }
    else if (equal(prefix, TEXT(0x009f1594u))) { column = 2; uppercase = 1; }
    else if (equal(prefix, TEXT(0x009f158cu))) column = 1;
    else if (equal(prefix, TEXT(0x009f155cu))) column = 2;
    if (column) {
        replace_ten_word(word, column);
        if (uppercase)
            *(volatile char *)word = (char)UPPER(signed_byte(*(const volatile unsigned char *)word));
        return compose(frame);
    }
    const char *suffix = argument(frame, 0x2824u);
    const char *literal = 0;
    if (equal(suffix, TEXT(0x009f1558u))) literal = TEXT(0x009f1558u);
    else if (equal(suffix, TEXT(0x0099c35cu))) literal = TEXT(0x0099c35cu);
    if (literal) copy_bytes(local(frame, 0x18u), literal, size_of(literal));
    else if (equal(suffix, TEXT(0x0099c368u)) || equal(suffix, TEXT(0x009f15e8u)))
        return word_ending(frame, 0x009eb2e0u, 0x0067fe4cu, 0x0067fe7cu, 0x53u, 39u);
    return compose(frame);
}
char *CM_LANG_CDECL cm_lang_morph_language_eight(unsigned char *frame) {
    const char *prefix = argument(frame, 0x2820u);
    char *word = local(frame, 0x1018u);
    if (equal(prefix, TEXT(0x009f15ecu))) {
        *(volatile char *)word = (char)UPPER(signed_byte(*(const volatile unsigned char *)word));
        return compose(frame);
    }
    if (equal(prefix, TEXT(0x009f15c0u)) && !equal(word, TEXT(0x009f15b8u))
            && !equal(word, TEXT(0x0099c8fcu)) && !equal(word, TEXT(0x009f15acu))) {
        lower_word_in_place(word);
        return compose(frame);
    }
    cm_lang_u32 article = 0, lowercase = 0;
    if (equal(prefix, TEXT(0x009f1554u))) article = 0x009f1550u;
    else if (equal(prefix, TEXT(0x009f1558u))) article = 0x009f1548u;
    else if (equal(prefix, TEXT(0x009f1538u))) { article = 0x009f1550u; lowercase = 1; }
    else if (equal(prefix, TEXT(0x009f152cu))) { article = 0x009f1548u; lowercase = 1; }
    if (article) {
        /* Only the low byte of the first property determines this choice. */
        if (*(const volatile unsigned char *)(frame + 0x2828u)) article -= 4u;
        copy_bytes(local(frame, 0x818u), TEXT(article), size_of(TEXT(article)));
        if (lowercase) lower_word_in_place(word);
        return compose(frame);
    }
    cm_lang_u32 column = 0, uppercase = 0;
    if (equal(prefix, TEXT(0x009f15a0u))) { column = 1; uppercase = 1; }
    else if (equal(prefix, TEXT(0x009f1594u))) { column = 2; uppercase = 1; }
    else if (equal(prefix, TEXT(0x009f1520u))) { column = 3; uppercase = 1; }
    else if (equal(prefix, TEXT(0x009f158cu))) column = 1;
    else if (equal(prefix, TEXT(0x009f155cu))) column = 2;
    else if (equal(prefix, TEXT(0x009f1518u))) column = 3;
    if (column) {
        /* This recovered helper also replaces the equivalent inline scans. */
        cm_lang_morph_table_a(word, column);
        if (uppercase)
            *(volatile char *)word = (char)UPPER(signed_byte(*(const volatile unsigned char *)word));
        return compose(frame);
    }
    const char *suffix = argument(frame, 0x2824u);
    if (equal(suffix, TEXT(0x009f1514u))) {
        cm_lang_u32 length = size_of(word) - 1u;
        if (length > 4u) {
            const char *tail = (const char *)((cm_lang_u32)word - 4u + (size_of(word) - 1u));
            cm_lang_u32 append = equal(tail, TEXT(0x009f150cu));
            if (!append) {
                tail = (const char *)((cm_lang_u32)word - 4u + (size_of(word) - 1u));
                append = equal(tail, TEXT(0x009f1504u));
            }
            if (append) {
                char *scratch = local(frame, 0x1818u);
                PRINT(scratch, TEXT(0x009f14fcu), word);
                copy_bytes(word, scratch, size_of(scratch));
            }
        }
        return compose(frame);
    }
    if (equal(suffix, TEXT(0x0099c368u)) || equal(suffix, TEXT(0x009f15e8u)))
        return word_ending(frame, 0x009eb2f8u, 0x0067feacu, 0x0067fedcu, 0x53u, 39u);
    return compose(frame);
}
char *CM_LANG_CDECL cm_lang_morph_language_six(unsigned char *frame) {
    const char *prefix = argument(frame, 0x2820u);
    char *word = local(frame, 0x1018u);
    if (equal(prefix, TEXT(0x009f15ecu))) {
        *(volatile char *)word = (char)UPPER(signed_byte(*(const volatile unsigned char *)word));
        return compose(frame);
    }
    if (equal(prefix, TEXT(0x009f15c0u)) && !equal(word, TEXT(0x009f15b8u))
            && !equal(word, TEXT(0x0099c8fcu)) && !equal(word, TEXT(0x009f15acu))) {
        lower_word_in_place(word);
        return compose(frame);
    }
    const char *suffix = argument(frame, 0x2824u);
    if (equal(suffix, TEXT(0x0099c360u))) {
        const char *literal = TEXT(0x0099c360u);
        copy_bytes(local(frame, 0x18u), literal, size_of(literal));
    } else if (equal(suffix, TEXT(0x0099c368u)) || equal(suffix, TEXT(0x009f15e8u)))
        return word_ending(frame, 0x009eb310u, 0x0067ff0cu, 0x0067ff3cu, 0x43u, 55u);
    return compose(frame);
}

#include "cm_lang_morph_two_data.inc"

static void copy_literal(char *destination, cm_lang_u32 literal) {
    copy_bytes(destination, TEXT(literal), size_of(TEXT(literal)));
}
static cm_lang_u32 byte_target(unsigned char byte, cm_lang_u32 base, cm_lang_u32 bound,
                              cm_lang_u32 selectors, cm_lang_u32 targets, cm_lang_u32 fallback) {
    cm_lang_u32 index = (cm_lang_u32)signed_byte(byte) - base;
    if (index > bound) return fallback;
    cm_lang_u32 selected = *(const volatile unsigned char *)(selectors + index);
    return *(const volatile cm_lang_u32 *)(targets + selected * 4u);
}
static void two_prefix_property(unsigned char *frame, cm_lang_u32 table, cm_lang_u32 property) {
    cm_lang_u32 value = (cm_lang_u32)signed_byte(*(const volatile unsigned char *)(frame + property));
    cm_lang_u32 target = value <= 5u ? *(const volatile cm_lang_u32 *)(table + value * 4u) : 0x00672953u;
    char *prefix = local(frame, 0x818u);
    cm_lang_u32 literal = 0x009f1308u;
    switch (target) {
    case 0x006720f2u: literal = 0x009f14b0u; break;
    case 0x00672103u: literal = 0x009f14e8u; break;
    case 0x00672114u: literal = 0x009f14a8u; break;
    case 0x00672125u: literal = 0x009f14ecu; break;
    case 0x0067226cu: {
        const char *caller = argument(frame, 0x281cu);
        cm_lang_u32 selected = byte_target(*(const volatile unsigned char *)caller,
            0x41u, 56u, 0x0068000cu, 0x00680004u, 0x00672773u);
        literal = selected == 0x0067228au ? 0x009f14c4u : 0x009f14c0u;
        break;
    }
    case 0x0067228au: literal = 0x009f14c4u; break;
    case 0x0067229bu: literal = 0x009f1454u; break;
    case 0x006722acu: literal = 0x009f144cu; break;
    case 0x006723e9u:
        copy_literal(prefix, 0x009f1424u);
        literal = 0x009f141cu; break;
    case 0x00672412u: literal = 0x009f141cu; break;
    case 0x006724dfu: literal = 0x009f1548u; break;
    case 0x0067253cu: literal = 0x009f13e8u; break;
    case 0x0067259du: {
        const char *caller = argument(frame, 0x281cu);
        cm_lang_u32 selected = byte_target(*(const volatile unsigned char *)caller,
            0x41u, 56u, 0x006800b0u, 0x006800a8u, 0x006725c5u);
        copy_literal(prefix, selected == 0x006725b7u ? 0x009f13d0u : 0x009f13c4u);
        /* Original fallthrough overwrites the prefix but retains trailing bytes. */
        literal = 0x009f13c4u; break;
    }
    case 0x006725eeu: literal = 0x009f13c4u; break;
    case 0x006725ffu: literal = 0x009f13b8u; break;
    case 0x00672610u: literal = 0x009f13d0u; break;
    case 0x00672621u: literal = 0x009f13acu; break;
    case 0x00672682u: {
        const char *caller = argument(frame, 0x281cu);
        cm_lang_u32 selected = byte_target(*(const volatile unsigned char *)caller,
            0x41u, 56u, 0x0068010cu, 0x00680104u, 0x006726aau);
        copy_literal(prefix, selected == 0x0067269cu ? 0x009f1394u : 0x009f1388u);
        literal = 0x009f1388u; break;
    }
    case 0x006726d3u: literal = 0x009f1388u; break;
    case 0x006726e4u: literal = 0x009f137cu; break;
    case 0x006726f5u: literal = 0x009f1394u; break;
    case 0x00672706u: literal = 0x009f1370u; break;
    case 0x00672773u: literal = 0x009f14c0u; break;
    case 0x00672784u: literal = 0x009f136cu; break;
    case 0x00672795u: literal = 0x009f1364u; break;
    case 0x006727beu: literal = 0x009f1360u; break;
    case 0x006727cfu: literal = 0x009f1358u; break;
    case 0x006727e0u: literal = 0x009f1350u; break;
    case 0x006728ebu: literal = 0x009f1424u; break;
    case 0x0067290du: literal = 0x00ad9c64u; break;
    case 0x0067291bu: literal = 0x009f14d4u; break;
    case 0x00672929u: literal = 0x009f130cu; break;
    case 0x00672937u: literal = 0x009f1330u; break;
    case 0x00672945u: literal = 0x009f14d8u; break;
    default: break; /* Verified default target 00672953: a single space. */
    }
    copy_literal(prefix, literal);
}
static void two_stadium_prefix(unsigned char *frame, const char *caller) {
    if (FIND(caller, TEXT(0x009f1348u)) == caller
            || FIND(caller, TEXT(0x009f1340u)) == caller
            || FIND(caller, TEXT(0x009f1338u)) == caller) {
        copy_literal(local(frame, 0x818u), 0x009f1424u);
        return;
    }
    cm_lang_u32 skip, format;
    if (FIND(caller, TEXT(0x009f1330u)) == caller) { skip = 4; format = 0x009f1328u; }
    else if (FIND(caller, TEXT(0x009f14a8u)) == caller) { skip = 4; format = 0x009f1320u; }
    else if (FIND(caller, TEXT(0x009f14d4u)) == caller) { skip = 3; format = 0x009f1318u; }
    else if (FIND(caller, TEXT(0x009f14e8u)) == caller) { skip = 3; format = 0x009f1310u; }
    else { copy_literal(local(frame, 0x818u), 0x009f1360u); return; }
    PRINT(OUTPUT, TEXT(format), caller + skip);
}
char *CM_LANG_CDECL cm_lang_morph_language_two(unsigned char *frame) {
    const char *prefix = argument(frame, 0x2820u);
    const char *caller = argument(frame, 0x281cu);
    char *word = local(frame, 0x1018u);
    if (equal(prefix, TEXT(0x009f15ecu)))
        *(volatile char *)word = (char)UPPER(signed_byte(*(const volatile unsigned char *)word));
    else if (equal(prefix, TEXT(0x009f15c0u)))
        lower_word_in_place(word); /* This body has no acronym exceptions. */
    else if (equal(prefix, TEXT(0x009f14f0u)) || equal(prefix, TEXT(0x009f14dcu))) {
        cm_lang_u32 capital = equal(prefix, TEXT(0x009f14f0u));
        cm_lang_u32 selected = byte_target(*(const volatile unsigned char *)word,
            0x61u, 20u, capital ? 0x0067ff7cu : 0x0067ff9cu,
            capital ? 0x0067ff74u : 0x0067ff94u, capital ? 0x00671f0bu : 0x00671f82u);
        cm_lang_u32 format = capital ? (selected == 0x00671f01u ? 0x009f14ecu : 0x009f14e8u)
                                    : (selected == 0x00671f78u ? 0x009f14d8u : 0x009f14d4u);
        PRINT(local(frame, 0x818u), TEXT(format));
    } else if (equal(prefix, TEXT(0x009f14c8u))) {
        unsigned char byte = (unsigned char)LOWER(signed_byte(*(const volatile unsigned char *)word));
        *(volatile unsigned char *)word = byte;
        cm_lang_u32 selected = byte_target(byte, 0x61u, 20u, 0x0067ffbcu, 0x0067ffb4u, 0x0067201cu);
        PRINT(local(frame, 0x818u), TEXT(selected == 0x00672002u ? 0x009f14c4u : 0x009f14c0u));
    } else if (equal(prefix, TEXT(0x009f14bcu)) || equal(prefix, TEXT(0x009f14b8u)))
        two_prefix_property(frame, 0x00680190u, 0x2828u);
    else if (equal(prefix, TEXT(0x009f14b4u)))
        two_prefix_property(frame, 0x0067ffd4u, 0x2828u);
    else if (equal(prefix, TEXT(0x009f149cu))) {
        cm_lang_u32 friendly = FIND(caller, TEXT(0x009f1494u)) == caller;
        if (!friendly) friendly = FIND(caller, TEXT(0x009f148cu)) == caller;
        copy_literal(local(frame, 0x818u), friendly ? 0x009f1478u : 0x009f1480u);
    } else if (equal(prefix, TEXT(0x009f1470u)) || equal(prefix, TEXT(0x009f1464u)))
        two_stadium_prefix(frame, caller);
    else if (equal(prefix, TEXT(0x009f145cu)))
        two_prefix_property(frame, 0x0067ffecu, 0x2828u);
    else if (equal(prefix, TEXT(0x009f1444u)) || equal(prefix, TEXT(0x009f1440u)))
        two_prefix_property(frame, 0x00680178u, 0x2828u);
    else if (equal(prefix, TEXT(0x009f1434u)) || equal(prefix, TEXT(0x009f1428u)))
        two_prefix_property(frame, 0x00680160u, 0x2828u);
    else if (equal(prefix, TEXT(0x009f1558u)))
        two_prefix_property(frame, 0x00680048u, 0x2828u);
    else if (equal(prefix, TEXT(0x009f1410u)) || equal(prefix, TEXT(0x009f1404u)))
        two_prefix_property(frame, 0x00680148u, 0x2828u);
    else if (equal(prefix, TEXT(0x009f13fcu)))
        two_prefix_property(frame, 0x00680060u, 0x2828u);
    else if (equal(prefix, TEXT(0x009f13f4u)))
        two_prefix_property(frame, 0x00680078u, 0x2828u);
    else if (equal(prefix, TEXT(0x009f13dcu)))
        two_prefix_property(frame, 0x00680090u, 0x282cu);
    else if (equal(prefix, TEXT(0x009f13a0u)))
        two_prefix_property(frame, 0x006800ecu, 0x282cu);
    else if (*(const volatile unsigned char *)prefix)
        copy_literal((char *)prefix, 0x00ad9c64u);

    const char *suffix = argument(frame, 0x2824u);
    if (equal(suffix, TEXT(0x0099c368u)))
        copy_literal(local(frame, 0x18u), 0x00ad9c64u);
    else {
        cm_lang_u32 matched = 0;
        for (cm_lang_u32 index = 0; index < sizeof(two_suffix_rules) / sizeof(two_suffix_rules[0]); ++index) {
            const two_suffix_rule *rule = &two_suffix_rules[index];
            if (!equal(suffix, TEXT(rule->qualifier))) continue;
            cm_lang_u32 value = (cm_lang_u32)signed_byte(*(const volatile unsigned char *)(frame + 0x282cu));
            cm_lang_u32 literal = value <= 5u ? two_suffix_literal(*(const volatile cm_lang_u32 *)(rule->dispatch + value * 4u)) : 0x009f1308u;
            if (literal) copy_literal(local(frame, 0x18u), literal);
            matched = 1;
            break;
        }
        if (!matched && *(const volatile unsigned char *)suffix)
            copy_literal((char *)suffix, 0x00ad9c64u);
    }
    return compose(frame);
}

#include "cm_lang_morph_four_data.inc"

static cm_lang_u32 four_property_literal(unsigned char *frame, cm_lang_u32 property,
                                        cm_lang_u32 table) {
    cm_lang_u32 value = (cm_lang_u32)signed_byte(*(const volatile unsigned char *)(frame + property));
    if (value > 8u) return 0x009f1308u;
    return four_literal(*(const volatile cm_lang_u32 *)(table + value * 4u));
}

char *CM_LANG_CDECL cm_lang_morph_language_four(unsigned char *frame) {
    const char *prefix = argument(frame, 0x2820u);
    char *word = local(frame, 0x1018u);
    if (equal(prefix, TEXT(0x009f15ecu)))
        *(volatile char *)word = (char)UPPER(signed_byte(*(const volatile unsigned char *)word));
    else if (equal(prefix, TEXT(0x009f15c0u)))
        lower_word_in_place(word); /* Original has no acronym exceptions here. */
    else {
        cm_lang_u32 matched = 0;
        for (cm_lang_u32 index = 0; index < sizeof(four_prefix_rules) / sizeof(four_prefix_rules[0]); ++index) {
            const four_rule *rule = &four_prefix_rules[index];
            if (!equal(prefix, TEXT(rule->qualifier))) continue;
            cm_lang_u32 literal = four_property_literal(frame, 0x2828u, rule->dispatch);
            if (literal) copy_literal(local(frame, 0x818u), literal);
            matched = 1;
            break;
        }
        if (!matched && *(const volatile unsigned char *)prefix)
            copy_literal((char *)prefix, 0x00ad9c64u);
    }

    const char *suffix = argument(frame, 0x2824u);
    if (equal(suffix, TEXT(0x0099c368u)))
        copy_literal(local(frame, 0x18u), 0x00ad9c64u);
    else {
        cm_lang_u32 matched = 0;
        for (cm_lang_u32 index = 0; index < sizeof(four_suffix_rules) / sizeof(four_suffix_rules[0]); ++index) {
            const four_rule *rule = &four_suffix_rules[index];
            /* The signed ban case occurs immediately before "Invitata". */
            if (rule->qualifier == 0x009f09a8u && equal(suffix, TEXT(0x009f09b4u))) {
                int value = signed_byte(*(const volatile unsigned char *)(frame + 0x282cu));
                copy_literal(local(frame, 0x18u), value <= 1 ? 0x009f09d0u : 0x009f09c4u);
                matched = 1;
                break;
            }
            if (!equal(suffix, TEXT(rule->qualifier))) continue;
            cm_lang_u32 literal = four_property_literal(frame, 0x282cu, rule->dispatch);
            if (literal) copy_literal(local(frame, 0x18u), literal);
            matched = 1;
            break;
        }
        /* Original 00674B47 repeats the earlier "partita" comparison. The first
         * matching branch always exits, so its later test is unreachable. */
        if (!matched && *(const volatile unsigned char *)suffix)
            copy_literal((char *)suffix, 0x00ad9c64u);
    }
    return compose(frame);
}
#include "cm_lang_morph_three_body.inc"

#include "cm_lang_morph_nine_body.inc"

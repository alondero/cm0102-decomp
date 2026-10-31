/* VA 0090C310..0090C50F, embedded si/code/utils.cpp ownership evidence.
 * Unsigned DIV classifies the full number; formatting passes its signed bits.
 * Original language IDs and strings are retained; no linguistic substitutions.
 */
#include "cm_ordinal.h"
#include "../date/cm_date.h"
typedef int (CM_LANG_CDECL *print_fn)(char *, const char *, ...);
typedef void (CM_LANG_CDECL *split_fn)(const char *, char *, char *, char *, char *);
typedef int (CM_LANG_CDECL *error_fn)(const char *, const char *, cm_lang_u32);
#define PRINT ((print_fn)0x00944c9fu)
#define TEXT(va) ((const char *)(va))

char *CM_LANG_CDECL cm_ordinal(cm_lang_u32 number, cm_lang_u32 gender) {
    char *out = (char *)0x00de2734u;
    unsigned char form = (unsigned char)gender;
    if (!number) {
        char file[256], extension[256];
        ((split_fn)0x00944cffu)(TEXT(0x00ad4134u), 0, 0, file, extension);
        const char *version = cm_date_diagnostic_version(file, extension, 966);
        PRINT((char *)0x00ae24d0u, TEXT(0x009870e8u), version, file, extension, 966);
        ((error_fn)0x005e8290u)(TEXT(0x009870e0u), (const char *)0x00ae24d0u, 0);
        *(volatile cm_lang_u32 *)0x00b67a34u = 0;
        out[0] = 0;
        return out;
    }
    if (cm_lang_current() == 2) {
        if (form == 1) {
            if (number == 1) PRINT(out, TEXT(0x00ad4210u));
            else PRINT(out, TEXT(0x0098806cu), number);
        } else if (number == 1) PRINT(out, TEXT(form == 2 ? 0x00ad421cu : 0x00ad4210u));
        else PRINT(out, TEXT(0x00ad4214u), number);
        return out;
    }
    cm_lang_u32 language = cm_lang_current();
    if (((language == 4 || language == 9 || language == 7 || language == 11) && form == 1) ||
        ((language == 10 || language == 11) && form == 3)) {
        PRINT(out, TEXT(0x0098806cu), number);
        return out;
    }
    cm_lang_u32 text = 0x00ad41d0u;
    unsigned int remainder = number % 100u;
    if (remainder < 10 || remainder > 20) {
        switch (number % 10u) {
            case 1: text = 0x00ad4200u; break;
            case 2: text = 0x00ad41f0u; break;
            case 3: text = 0x00ad41e0u; break;
        }
    }
    cm_lang_format(out, TEXT(text), number);
    return out;
}

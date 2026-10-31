#include "cmre.h"
/* Preferred-image VA00933320, base00400000; CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL3/fullEAX. Scratch314: archiveL0/namesL4/cursorL8/resultLc,
   diagnosticsL10/L110, constructed pathL210. Keep both 1000-entry loops,
   original allocation-failure continuation, copy widths and file boundaries. */
#define D(a) (*(volatile u32 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))
#define G(a) CM_GLOBAL(volatile u32, a)
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern unsigned long long CM_CDECL cm_fn_00944c94(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00945b00(u32, u32);
extern u32 CM_CDECL cm_fn_00945bfe(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00945a09(u32);
extern u32 CM_CDECL cm_fn_00933a00(u32, u32);
extern u32 CM_CDECL cm_fn_009326e0(u32, u32, u32, u32, u32);
extern void CM_CDECL cm_fn_00945b13(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00932c30(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00945ce6(u32, u32, u32, u32);
extern void CM_CDECL cm_fn_00932b00(u32);

u32 CM_CDECL cm_fn_00933320(volatile u32 filename_arg,
                          volatile u32 directory_arg, volatile u32 pattern_arg)
{
    u8 scratch[0x314];
    u32 l = (u32)scratch, eax, header, input, output = 0, directory;
    u32 names, index, p, q, count, remaining, source, destination, word, version;
    u8 byte;
    D(l) = 0; D(l + 0xc) = 0;
    eax = (u32)cm_fn_00944e46(0x3f7a0);
    D(l + 4) = eax;
    if (eax == 0) goto final_cleanup;
    header = (u32)cm_fn_00944cf1(0x416ec);
    if (header == 0) {
        cm_fn_00944cff(0x00ad6ab0, 0, 0, l + 0x10, l + 0x110);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version,
                      l + 0x10, l + 0x110, 0x282);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        G(0x00b67a34) = 0;
        goto extract;
    }
    p = filename_arg;
    input = cm_fn_00945b00(p, 0x0098f928);
    if (input == 0) goto free_header;
    eax = cm_fn_00945bfe(header, 12, 1, input);
    if (eax != 0) {
        count = 0;
        if (D(header) == 1) { D(header) = 3; count = 1; }
        if (D(header) == 2) { D(header) = 4; count = 1; }
        count = count != 0 ? 250 : 1000;
        cm_fn_00945bfe(header + 12, 0x10c, count, input);
    }
    cm_fn_00945a09(input);
    eax = D(header);
    if (eax != 3 && eax != 4) goto free_header;
    destination = D(l + 4);
    index = 0;
    D(l + 8) = header + 0x14;
    do {
        eax = D(header + 8);
        source = D(l + 8);
        if ((s32)index >= (s32)eax) source = 0x00ad6ad8;
        remaining = 0xffffffffu; p = source;
        while (remaining != 0) {
            byte = B(p); ++p; --remaining;
            if (byte == 0) break;
        }
        count = ~remaining;
        source = p - count;
        q = destination; destination += 0x104;
        remaining = count >> 2;
        while (remaining != 0) {
            word = D(source); D(q) = word;
            source += 4; q += 4; --remaining;
        }
        remaining = count & 3;
        ++index;
        while (remaining != 0) {
            byte = B(source); B(q) = byte;
            ++source; ++q; --remaining;
        }
        p = D(l + 8); p += 0x10c;
        D(l + 8) = p;
    } while (index < 1000);
    cm_fn_00944c94(header);
extract:
    names = D(l + 4);
    directory = directory_arg;
    index = 0;
    do {
        p = pattern_arg;
        eax = cm_fn_00933a00(p, names);
        if (eax != 0) {
            p = filename_arg;
            eax = cm_fn_009326e0(p, names, 0, 0x15, 4);
            D(l) = eax;
            if (eax != 0) {
                D(l + 0xc) = 1;
                cm_fn_00945b13(l + 0x210, 0, directory, names, 0);
                output = cm_fn_00945b00(l + 0x210, 0x009a7f28);
                if (output == 0) goto free_names;
                eax = D(l);
                eax = cm_fn_00932c30(eax + 0x72445, 1, 0x30d40, eax);
                while ((s32)eax > 0) {
                    p = D(l);
                    cm_fn_00945ce6(p + 0x72445, 1, eax, output);
                    p = D(l);
                    eax = cm_fn_00932c30(p + 0x72445, 1, 0x30d40, p);
                }
                cm_fn_00945a09(output);
                cm_fn_00932b00(l);
            }
        }
        ++index; names += 0x104;
    } while (index < 1000);
    eax = D(l + 4);
    cm_fn_009452ca(eax);
    return D(l + 0xc);
free_header:
    cm_fn_00944c94(header);
free_names:
    p = D(l + 4);
    cm_fn_009452ca(p);
    if (output != 0) cm_fn_00945a09(output);
final_cleanup:
    if (D(l) != 0) cm_fn_00932b00(l);
    return 0;
}

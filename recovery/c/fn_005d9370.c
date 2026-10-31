#include "cmre.h"

/* Preferred-image VA 0x005d9370, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Keep the two temporary-list passes, including the second pass's zero
   entries, and the signed low-word loop counters from the original. */
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_CDECL cm_fn_00540a50(u32, u32, u32);
extern void CM_CDECL cm_fn_004afbd0(u32, s16, s16);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);

#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))
#define COUNT D((u32)scratch)
#define TEMP D((u32)scratch + 4u)
#define ITER D((u32)scratch + 8u)
#define DIRECTION D((u32)scratch + 12u)

u32 CM_THISCALL cm_fn_005d9370(u32 self)
{
    u8 scratch[0x210];
    u32 eax, ecx, edx, ebx = 0, ebp, edi;
    u32 line, failure;
    int more;
    u8 *filename, *extension;
    char *version;

    COUNT = 0;
    W(self + 0x3eu) = 64;
    eax = (u32)cm_fn_00944e46(0x180);
    D(self + 0xa7u) = eax;
    if (eax == 0) {
        filename = scratch + 0x110; extension = scratch + 0x10;
        line = 0x13c; failure = 0xffffffffu;
        goto error;
    }
    eax = (u32)cm_fn_00944e46(0xaf0);
    TEMP = eax;
    if (eax == 0) {
        filename = scratch + 0x10; extension = scratch + 0x110;
        line = 0x144; failure = 0;
        goto error;
    }
    edx = D(0x00ae2350u);
    edi = D(0x00ae23b4u);
    ecx = edx * 2u;
    edx = D(0x00ae235cu) - ecx;
    ITER = 0;
    if ((s32)edx <= 0) goto sorted;
    ebp = 0;
scan_club:
    ecx = D(edi + 0x57u);
    if (ecx == 0) goto next_club;
    eax = D(edi + 0x60u);
    if (eax != 0) {
        eax = D(ecx);
        edx = D(0x009cf564u);
        if (eax == edx) goto eligible;
    }
    eax = D(edi + 0x53u);
    if (eax == 0) goto next_club;
    edx = D(eax);
    eax = D(0x009cf314u);
    if (edx != eax) goto next_club;
    eax = D(ecx);
    ecx = D(0x009cf764u);
    if (eax != ecx) goto next_club;
eligible:
    eax = cm_fn_00540a50(edi, (u32)scratch + 12u, 1);
    if (eax != 0) {
        eax = DIRECTION;
        if (eax == 0) goto next_club;
    }
    eax = D(edi + 0x5bu);
    if (eax == 0) goto temporary;
    edx = D(eax);
    eax = D(0x009cf550u);
    if (edx != eax || (s32)ebp >= 0x18) goto temporary;
    eax = D(self + 0xa7u);
    ++ebx;
    D(eax + ebp) = edi;
    ecx = D(self + 0xa7u);
    B(ecx + ebp + 4u) = 2;
    edx = D(self + 0xa7u);
    B(edx + ebp + 5u) = 0;
    ebp += 6u;
    goto next_club;
temporary:
    eax = COUNT;
    if ((s16)eax < 700) {
        edx = TEMP;
        ecx = (u32)(s32)(s16)eax;
        ++eax;
        D(edx + ecx * 4u) = edi;
        COUNT = eax;
    }
next_club:
    ecx = D(0x00ae2350u);
    eax = ITER + 1u;
    edi += 0x245u;
    edx = ecx * 2u;
    ecx = D(0x00ae235cu) - edx;
    ITER = eax;
    edx = (u32)(s32)(s16)eax;
    if ((s32)edx < (s32)ecx) goto scan_club;
    eax = TEMP;
sorted:
    edi = COUNT;
    cm_fn_004afbd0(eax, (s16)edi, 0x18);
    edx = 0;
    if ((s16)edi <= 0) goto second_pass;
    eax = ebx * 6u;
first_pass:
    if ((s32)eax >= 0xa8) goto second_pass;
    ecx = 4;
    if ((s32)eax > 0x18) {
        do {
            ++ecx;
            edi = (u32)(s32)(s16)ecx;
        } while ((s32)edi < (s32)ebx);
    }
    edi = TEMP;
    ++ebx;
    ecx = (u32)(s32)(s16)edx;
    ebp = D(edi + ecx * 4u);
    eax += 6u;
    ecx = edi + ecx * 4u;
    edi = D(self + 0xa7u);
    ++edx;
    D(eax + edi - 6u) = ebp;
    edi = D(self + 0xa7u);
    more = (s16)edx < (s16)COUNT;
    B(eax + edi - 1u) = 0;
    edi = D(self + 0xa7u);
    B(eax + edi - 2u) = 2;
    D(ecx) = 0;
    if (more) goto first_pass;
second_pass:
    ecx = 0;
    if ((s16)COUNT <= 0) goto free_list;
    eax = ebx * 6u;
second_item:
    if ((s32)eax >= 0xa8) goto free_list;
    ebp = TEMP;
    edi = D(self + 0xa7u);
    edx = (u32)(s32)(s16)ecx;
    ++ebx;
    eax += 6u;
    edx = D(ebp + edx * 4u);
    ++ecx;
    D(eax + edi - 6u) = edx;
    edx = D(self + 0xa7u);
    more = (s16)ecx < (s16)COUNT;
    B(eax + edx - 1u) = 0;
    edx = D(self + 0xa7u);
    B(eax + edx - 2u) = 2;
    if (more) goto second_item;
free_list:
    eax = TEMP;
    cm_fn_009452ca(eax);
    eax = D(0x00ae235cu);
    ecx = D(0x00ae23b4u);
    ebp = 0;
    if ((s32)eax <= 0) goto final_group;
    edi = D(0x009cf550u);
    eax = ebx * 6u;
third_item:
    edx = D(ecx + 0x57u);
    if (edx == 0 || D(edx) != edi) goto third_next;
    edx = (u32)(s32)(s16)W(self + 0x3eu);
    if ((s32)ebx >= (s32)edx) goto third_next;
    edx = D(ecx + 0x5bu);
    if (edx != 0) {
        edx = D(edx);
        if (edx != edi && edx != D(0x009cf54cu)) {
            edx = D(self + 0xa7u);
            B(eax + edx + 4u) = 2;
            goto third_store;
        }
    }
    edx = D(self + 0xa7u);
    B(eax + edx + 4u) = 1;
third_store:
    edx = D(self + 0xa7u);
    ++ebx;
    D(eax + edx) = ecx;
    edx = D(self + 0xa7u);
    B(eax + edx + 5u) = 0;
    eax += 6u;
    edx = (u32)(s32)(s16)W(self + 0x3eu);
    if (ebx == edx) goto final_group;
    edi = D(0x009cf550u);
third_next:
    ++ebp;
    ecx += 0x245u;
    edx = (u32)(s32)(s16)ebp;
    if ((s32)edx < (s32)D(0x00ae235cu)) goto third_item;
final_group:
    eax = D(0x00ae235cu);
    ecx = D(0x00ae23b4u);
    edi = 0;
    if ((s32)eax <= 0) goto done;
    eax = ebx * 6u;
fourth_item:
    edx = D(ecx + 0x57u);
    if (edx == 0) goto fourth_next;
    edx = D(edx);
    ebp = D(0x009cf54cu);
    if (edx != ebp) goto fourth_next;
    edx = (u32)(s32)(s16)W(self + 0x3eu);
    if ((s32)ebx >= (s32)edx) goto fourth_next;
    edx = D(self + 0xa7u);
    ++ebx;
    D(eax + edx) = ecx;
    edx = D(self + 0xa7u);
    B(eax + edx + 4u) = 1;
    edx = D(self + 0xa7u);
    B(eax + edx + 5u) = 0;
    eax += 6u;
    edx = (u32)(s32)(s16)W(self + 0x3eu);
    if (ebx == edx) goto done;
fourth_next:
    ebp = D(0x00ae235cu);
    ++edi;
    edx = (u32)(s32)(s16)edi;
    ecx += 0x245u;
    if ((s32)edx < (s32)ebp) goto fourth_item;
done:
    eax = (u32)(s32)(s16)W(self + 0x3eu);
    return ebx == eax;
error:
    cm_fn_00944cff(0x009cd3e0u, 0, 0, filename, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version,
                  filename, extension, line);
    cm_fn_005e8290((const char *)0x009870e0u,
                  (const char *)0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return failure;
}

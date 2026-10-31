#include "cmre.h"

/* Preferred-image VA0078f890, base00400000, CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS0/fullEAX: -1 on allocation failure, else count==signedWORD capacity.
   Scratch208 countL0/first scan indexL4/diagnosticsL8,L108. Three ordered
   scans use wrapping DWORD counters with signed low-WORD loop comparisons.
   Keep cached club bases, dynamic global limits and every array reload. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00945a60(u32, u32);

u32 CM_THISCALL cm_fn_0078f890(u32 self)
{
    u8 scratch[0x208];
    u32 l = (u32)scratch, count = 0, offset = 0, club, total, other;
    u32 eax, pointer, value, comparand, array, index, limit;
    D(l) = 0;
    W(self + 0x3e) = 0x80;
    eax = (u32)cm_fn_00944e46(0x300);
    D(self + 0xa7) = eax;
    if (eax == 0) {
        cm_fn_00944cff(0x00a69708, 0, 0, l + 0x108, l + 8);
        eax = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, eax,
                       l + 0x108, l + 8, 0x14a);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0xffffffffu;
    }
    total = D(0x00ae235c);
    club = D(0x00ae23b4);
    D(l + 4) = 0;
    if ((s32)total > 0) {
        do {
            pointer = D(club + 0x57);
            value = D(self + 4);
            if (pointer == value) D(club + 0x57) = 0;
            pointer = D(club + 0x57);
            if (pointer == 0) goto first_next;
            value = D(pointer);
            comparand = D(0x009cf6c4);
            if (value != comparand && value != D(0x009cf6c8) &&
                value != D(0x009cf6cc) && value != D(0x009cf6d0) &&
                value != D(0x009cf6d4) && value != D(0x009cf6d8) &&
                value != D(0x009cf6dc) && value != D(0x009cf6e0) &&
                value != D(0x009cf6c0) && value != D(0x009cf6bc))
                goto first_next;
            if ((s32)count >= (s32)(s16)W(self + 0x3e)) goto first_next;
            pointer = D(club + 0x53);
            if (pointer == 0) goto first_next;
            eax = cm_fn_00945a60(club + 0x38, 0x00a6973c);
            if (eax != 0) goto first_next;
            array = D(self + 0xa7);
            D(array + offset) = club;
            pointer = D(club + 0x57);
            comparand = D(0x009cf6bc);
            value = D(pointer);
            if (value == comparand) {
                array = D(self + 0xa7);
                B(array + offset + 4) = 1;
            } else {
                array = D(self + 0xa7);
                B(array + offset + 4) = 0;
            }
            array = D(self + 0xa7);
            count++;
            B(array + offset + 5) = 0;
            offset += 6;
first_next:
            index = D(l + 4);
            club += 0x245;
            index++;
            D(l + 4) = index;
            total = D(0x00ae235c);
        } while ((s32)(s16)(u16)index < (s32)total);
        D(l) = count;
    }
    club = D(0x00ae23b4);
    index = 0;
    if ((s32)total > 0) {
        count = D(l);
        offset = count * 6u;
        for (;;) {
            if ((s32)count >= (s32)(s16)W(self + 0x3e)) break;
            pointer = D(club + 0x53);
            if (pointer == 0) goto second_next;
            pointer = D(club + 0x57);
            if (pointer == 0) goto second_next;
            value = D(pointer);
            comparand = D(0x009cf738);
            if (value != comparand) goto second_next;
            eax = cm_fn_00945a60(club + 0x38, 0x00a6973c);
            if (eax != 0) goto second_next;
            array = D(self + 0xa7);
            D(array + offset) = club;
            array = D(self + 0xa7);
            B(array + offset + 4) = 0;
            array = D(self + 0xa7);
            B(array + offset + 5) = 0;
            count = D(l) + 1u;
            offset += 6;
            D(l) = count;
second_next:
            total = D(0x00ae235c);
            index++;
            club += 0x245;
            if ((s32)(s16)(u16)index >= (s32)total) break;
            count = D(l);
        }
    }
    other = D(0x00ae2350);
    club = D(0x00ae23b4);
    index = 0;
    limit = total - other * 2u;
    if ((s32)limit > 0) {
        count = D(l);
        offset = count * 6u;
        for (;;) {
            if ((s32)count >= (s32)(s16)W(self + 0x3e)) break;
            pointer = D(club + 0x53);
            if (pointer == 0) goto third_next;
            value = D(pointer);
            comparand = D(0x009cf408);
            if (value != comparand) goto third_next;
            pointer = D(club + 0x57);
            if (pointer != 0) goto third_next;
            eax = cm_fn_00945a60(club + 0x38, 0x00a6973c);
            if (eax != 0) goto third_next;
            array = D(self + 0xa7);
            D(array + offset) = club;
            array = D(self + 0xa7);
            count = D(l);
            B(array + offset + 4) = (u8)eax;
            array = D(self + 0xa7);
            count++;
            B(array + offset + 5) = 0;
            D(l) = count;
            offset += 6;
third_next:
            other = D(0x00ae2350);
            total = D(0x00ae235c);
            index++;
            club += 0x245;
            limit = total - other * 2u;
            if ((s32)(s16)(u16)index >= (s32)limit) break;
            count = D(l);
        }
    }
    limit = (u32)(s32)(s16)W(self + 0x3e);
    count = D(l);
    return count == limit;
}

#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern void CM_THISCALL cm_fn_00417c10(u32, u32, u32);
extern s32 CM_CDECL cm_fn_0090cfc0(s32);
extern u8 CM_THISCALL cm_fn_0062d140(u32, u8, u8, u32);
extern u32 CM_THISCALL cm_fn_0062d250(u32, u8, u32, u32, u32, u32, u32, u8, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

/* Sole direct caller overwrites full EAX at VA0062a880. */
void CM_FASTCALL cm_fn_0062cf70(u32 self)
{
    u8 scratch[0x21c];
    u32 local = (u32)scratch;
    u32 count, allocation, record, person, value, slot, index, text;
    u8 code, chosen;
    count = D(0x00ae2364);
    D(local + 4) = 0;
    D(local) = 0;
    allocation = (u32)cm_fn_00944e46(count * 31u);
    D(self) = allocation;
    if (allocation == 0) {
        cm_fn_00944cff(0x009e7f28, 0, 0, local + 0x11c, local + 0x1c);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                       local + 0x11c, local + 0x1c, 0x7d0);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return;
    }
    record = allocation;
    count = D(0x00ae2364);
    person = D(0x00ae23bc);
    D(local + 8) = 0;
    if ((s32)count > 0) {
        do {
            cm_fn_00417c10(record, local + 0x14, 0x00ae2c90);
            W(record + 0xc) = 0;
            W(record + 8) = 10000;
            W(record + 0xa) = 10000;
            B(record + 0x12) = 0xff;
            D(record + 0xe) = 0xffffffffu;
            D(record + 0x14) = 0xffffffffu;
            D(record + 0x18) = 0xffffffffu;
            B(record + 0x1d) = 0xff;
            B(record + 0x1e) = 50;
            value = D(person + 0x61);
            if (value != 0) {
                code = (u8)cm_fn_0090cfc0(82);
                value = D(0x009e7ae0u + (u32)(s32)(s8)code * 13u);
                if (value != 0 && cm_fn_0090cfc0(10) == 0) {
                    value = D(local + 4);
                    B(record + 0x12) = code;
                    D(local + 4) = value + 1u;
                }
                if (cm_fn_0090cfc0(100) == 0) {
                    value = (u32)cm_fn_0090cfc0(12);
                    B(local + 0xc) = (u8)value;
                    slot = D(local + 0xc);
                    chosen = cm_fn_0062d140(self, (u8)slot, 0xff, person);
                    B(local + 0x10) = chosen;
                    slot = D(local + 0x10);
                    /* 0062d250 consumes BYTE type; its DWORD forwarding at
                       0062dbd6/dbf5 reaches BYTE load0062e827 only. */
                    cm_fn_0062d250(self, (u8)slot, 1, person, 0, 1, 0xffffffffu, 0, 0);
                    value = D(local);
                    D(local) = value + 1u;
                }
            }
            index = D(local + 8);
            count = D(0x00ae2364);
            ++index;
            person += 110;
            record += 31;
            D(local + 8) = index;
        } while ((s32)index < (s32)count);
    }
    value = D(local + 4);
    cm_fn_00944c9f(0x00de1f64, 0x009e7f98, value);
    value = D(local);
    cm_fn_00944c9f(0x00de1f64, 0x009e7f74, value);
}

#include "cmre.h"

/* WIP: outcome mismatch, case 0 (original faults in helper path; C returns); iteration cap reached. */

typedef struct { volatile u32 prev; volatile u32 handler; volatile u32 state; } cm_seh_frame;
extern void CM_FASTCALL cm_fn_0054a110(void *buffer);
extern void CM_THISCALL cm_fn_00417c10(void *self, void *out, u16 *value);
extern u32 CM_THISCALL cm_fn_00549ef0(void *self, u32 first, u32 second);

u32 cm_fn_00669bd0(void *object, void *target_a, void *target_b)
{
    volatile u8 local[56];
    volatile cm_seh_frame frame;
    u32 previous = CM_FS(u32, 0);
    frame.prev = previous;
    frame.handler = 0x0095c658;
    frame.state = 0xffffffffu;
    CM_FS(u32, 0) = (u32)&frame;
    cm_fn_0054a110((void *)&local[16]);
    frame.state = 0;

    volatile u32 *entry = (volatile u32 *)0x00b63e38u;
    u32 id = *(volatile u32 *)((volatile u8 *)object + 0x53);
    if (id != 0) {
        u32 i = 0;
        volatile u32 *row = (volatile u32 *)0x00b63d60u;
        while (row < (volatile u32 *)0x00b64738u) {
            if (*row == id) {
                entry = row;
                volatile u8 *record = (volatile u8 *)row + 0x39;
                cm_fn_00417c10((void *)&local[16], (void *)&local[0], (u16 *)record);
                u16 a = *(volatile u16 *)(record + 0xb);
                u16 b = *(volatile u16 *)(record + 8);
                u8 kind = *(volatile u8 *)(record + 0xa);
                *(volatile u16 *)&local[7] = a;
                *(volatile u16 *)&local[4] = b;
                u16 c = *(volatile u16 *)(record + 0xd);
                u32 object_flag = *(volatile u32 *)((volatile u8 *)object + 0x1db);
                *(volatile u8 *)&local[6] = kind;
                *(volatile u16 *)&local[9] = c;
                if (object_flag != 0xffffffffu && (kind == 3 || kind == 4 || kind == 5)) {
                    *(volatile u8 *)&local[6] = 4;
                    *(volatile u16 *)&local[4] = 0x1e;
                }
                break;
            }
            row = (volatile u32 *)((volatile u8 *)row + 0x48);
            ++i;
        }
    }

    s16 record_value = *(volatile s16 *)((volatile u8 *)entry + 0x2a);
    s16 local_value = *(volatile s16 *)&local[0x1c];
    u32 global = *(volatile u32 *)0x00ae2c90u;
    s16 global_low = (s16)global;
    u32 result = 0;
    /* Remaining branch/call sequence is kept explicit below. */
    if (record_value > local_value) {
        if (record_value < global_low) {
            if (target_a) {
                u16 year = *(volatile u16 *)0x00ae2c92u;
                *(volatile u16 *)&local[0x20] = year;
                u32 converted = cm_fn_00549ef0((void *)&local[0], record_value, *(volatile u32 *)&local[0x20]);
                cm_fn_00417c10(target_a, (void *)&local[8], (u16 *)&converted);
            }
            if (!target_b) goto done_true;
            u16 year = *(volatile u16 *)0x00ae2c92u;
            *(volatile u16 *)&local[0x20] = year;
            u32 converted = cm_fn_00549ef0((void *)&local[0], (u32)(u16)local_value, (u32)year + 1u);
            cm_fn_00417c10(target_b, (void *)&local[0], (u16 *)&converted);
            goto done_true;
        }
    }
    (void)result;
    CM_FS(u32, 0) = frame.prev;
    return 0;
done_true:
    CM_FS(u32, 0) = frame.prev;
    return 1;
}

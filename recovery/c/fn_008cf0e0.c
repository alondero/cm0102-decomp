/* CM 01/02 3.9.68, preferred-image VA 0x008cf0e0 (900 bytes).
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Logical AX result; native upper EAX remains unspecified. Original callees and
   virtual calls remain explicit boundaries. Exception unwinding is unproved. */
#include "cmre.h"

extern u32 CM_THISCALL cm_fn_0054a110(void *self);
extern u32 CM_THISCALL cm_fn_0054c5b0(void *self, void *output, u32 days);
extern u32 CM_THISCALL cm_fn_00417c10(void *self, void *output, const void *source);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 argument);
extern void CM_STDCALL cm_fn_008d0bb0(void *target_a, void *target_b);

typedef u32 (CM_THISCALL *transfer_method)(u32 self, u32 event, u32 club,
                                         void *date, void *output);

#define DWORD_AT(address) (*(volatile u32 *)(u32)(address))
#define BYTE_AT(address) (*(volatile u8 *)(u32)(address))
#define LOCAL_DWORD(offset) (*(volatile u32 *)(local + (offset)))
#define LOCAL_WORD(offset) (*(volatile u16 *)(local + (offset)))

u16 CM_THISCALL cm_fn_008cf0e0(u32 self, u32 event, u32 date)
{
    /* L is ESP immediately after SUB ESP,34h. Four saved registers make
       subsequent unpushed ESP=L-10h. Keep all overlapping date/output fields
       in the same uninitialized allocation. The linked SEH record is separate. */
    volatile u8 local[0x34];
    volatile struct { u32 prev, handler, state; } frame;
    u32 club, row, found, value, table, cursor, object, other, key, vtable;
    u32 result = 0;
    u32 first, second;
    u16 year, bound_year;
    u8 index;
    void *copy_source, *copy_output;

    frame.state = 0xffffffffu;
    frame.handler = 0x00963fa8u;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    LOCAL_DWORD(4) = 0;
    cm_fn_0054a110((void *)(local + 0x14));
    frame.state = 0;
    cm_fn_0054c5b0((void *)0x00ae2c90u, (void *)(local + 0x24), 0x16d);
    *(volatile u8 *)&frame.state = 1;
    cm_fn_0054a110((void *)(local + 0x1c));
    *(volatile u8 *)&frame.state = 2;
    cm_fn_00417c10((void *)date, (void *)(local + 0x0c), (void *)0x00ae2c90u);

    if ((BYTE_AT(event + 0x30) & 0x0f) == 4) {
        value = DWORD_AT(event + 0x0c);
        row = CM_GLOBAL(volatile u32, 0x00ae23bc) + value * 0x6eu;
        found = cm_fn_004cf5a0(CM_GLOBAL(volatile u32, 0x00ae1050), row);
        if (found != 0) {
            cm_fn_00417c10((void *)(local + 0x1c), (void *)(local + 0x0c),
                           (void *)(found + 0x2d));
            first = CM_GLOBAL(volatile u32, 0x00ae2c90);
            second = CM_GLOBAL(volatile u32, 0x00ae2c94);
            year = LOCAL_WORD(0x1e);
            LOCAL_DWORD(0x0c) = first;
            LOCAL_DWORD(0x10) = second;
            bound_year = LOCAL_WORD(0x0e);
            if (bound_year < year ||
                (bound_year == year && (s16)first <= (s16)LOCAL_WORD(0x1c)))
                goto date_ready;
        }
    }
    cm_fn_00417c10((void *)(local + 0x1c), (void *)(local + 0x0c),
                   (void *)0x00ae2c90u);

date_ready:
    value = DWORD_AT(event + 8);
    club = CM_GLOBAL(volatile u32, 0x00ae23b4) + value * 0x245u;
    value = DWORD_AT(event + 0x0c);
    first = CM_GLOBAL(volatile u32, 0x00ae1050);
    row = CM_GLOBAL(volatile u32, 0x00ae23bc) + value * 0x6eu;
    found = cm_fn_004cf5a0(first, row);
    CM_GLOBAL(volatile u32, 0x00ae1080) = found;
    if (found != 0 && (BYTE_AT(found + 0x35) & 0x3f) != 5) {
        value = DWORD_AT(found + 4);
        LOCAL_DWORD(0x0c) = CM_GLOBAL(volatile u32, 0x00ae23b4) + value * 0x245u;
    } else {
        LOCAL_DWORD(0x0c) = 0;
    }

    if ((BYTE_AT(event + 0x30) & 0x0f) == 5) {
        cm_fn_008d0bb0((void *)date, (void *)0);
        cursor = self + 0x8ac;
        key = DWORD_AT(club + 0x53);
        index = 0;
        do {
            object = DWORD_AT(cursor);
            if (DWORD_AT(object + 0x0a) == key) {
                object = DWORD_AT(cursor);
                if (object != 0) {
                    vtable = DWORD_AT(object);
                    value = ((transfer_method)DWORD_AT(vtable + 0x0c))
                        (object, event, club, (void *)date, (void *)local);
                    if (value == 0) {
                        value = LOCAL_DWORD(0);
                        if ((u16)value != 9)
                            result = value;
                    }
                }
                goto return_result;
            }
            ++index;
            cursor += 4;
        } while ((s8)index < 0x1c);
        goto return_result;
    }

    if (DWORD_AT(row + 0x61) == 0) {
        cm_fn_00417c10((void *)date, (void *)(local + 0x0c), (void *)0x00ae2c90u);
        goto return_zero;
    }
    table = self + 0x8ac;
    cursor = table;
    key = DWORD_AT(club + 0x53);
    index = 0;
    object = 0;
    do {
        value = DWORD_AT(cursor);
        if (DWORD_AT(value + 0x0a) == key) {
            object = DWORD_AT(cursor);
            break;
        }
        ++index;
        cursor += 4;
    } while ((s8)index < 0x1c);

    value = LOCAL_DWORD(0x0c);
    if (value != 0 && DWORD_AT(event + 4) != 0) {
        key = DWORD_AT(value + 0x53);
        cursor = table;
        index = 0;
        do {
            value = DWORD_AT(cursor);
            if (DWORD_AT(value + 0x0a) == key) {
                other = DWORD_AT(cursor);
                goto groups_ready;
            }
            ++index;
            cursor += 4;
        } while ((s8)index < 0x1c);
        LOCAL_DWORD(4) = 0;
    }
    other = LOCAL_DWORD(4);

groups_ready:
    copy_source = (void *)(local + 0x1c);
    copy_output = (void *)(local + 4);
copy_date:
    cm_fn_00417c10((void *)(local + 0x14), copy_output, copy_source);
    bound_year = LOCAL_WORD(0x26);
    year = LOCAL_WORD(0x16);
    if (bound_year < year)
        goto return_result;
    if (bound_year == year) {
        if ((s16)LOCAL_WORD(0x24) <= (s16)LOCAL_WORD(0x14))
            goto return_result;
    }
    if (object != 0) {
        vtable = DWORD_AT(object);
        value = ((transfer_method)DWORD_AT(vtable + 0x0c))
            (object, event, club, (void *)(local + 0x14), (void *)local);
        if (value == 0) {
            value = LOCAL_DWORD(0);
            if ((u16)value != 9 && (u16)result == 0)
                result = value;
            if ((u16)result != 0x14 && (u16)result != 0x15 &&
                (u16)result != 0x1f && (u16)result != 0x11)
                goto next_date;
        }
    }
    if (other != 0) {
        vtable = DWORD_AT(other);
        first = LOCAL_DWORD(0x0c);
        value = ((transfer_method)DWORD_AT(vtable + 0x0c))
            (other, event, first, (void *)(local + 0x14), (void *)local);
        if (value == 0) {
            value = LOCAL_DWORD(0);
            if ((u16)value != 9 && (u16)result == 0)
                result = value;
            if ((u16)result != 0x14 && (u16)result != 0x15 &&
                (u16)result != 0x1f && (u16)result != 0x11)
                goto next_date;
        }
    }
    cm_fn_00417c10((void *)date, (void *)(local + 0x2c), (void *)(local + 0x14));
return_zero:
    result = 0;
return_result:
    CM_FS(u32, 0) = frame.prev;
    return (u16)result;

next_date:
    copy_source = (void *)cm_fn_0054c5b0((void *)(local + 0x14), (void *)(local + 4), 1);
    copy_output = (void *)(local + 0x2c);
    goto copy_date;
}

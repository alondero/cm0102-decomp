#include "cmre.h"

struct cmre_seh_registration {
    void *prev;
    u32 handler;
    u32 state;
};

typedef void (CM_THISCALL *cm_fn_00919910_slot68)(
    u32 self, u32 row, u32 arg2, u32 arg3, u32 arg4, u32 arg5);
typedef u32 (CM_THISCALL *cm_fn_00919910_slot3c)(
    u32 self, u8 mode, void *out14, void *out1c, u32 zero);

extern u32 CM_THISCALL cm_fn_0051c800(u32 self, u32 arg);
extern u32 *CM_THISCALL cm_fn_00522e00(
    u32 *receiver, u32 outer_self, s16 count, u32 *list, s16 out14,
    u32 self_plus4, u32 *slot3c_result, u16 self_plus40,
    u8 selected, u8 four, u16 out1c, u8 fourteen,
    u32 zero1, u32 zero2, u32 zero3, u32 zero4);
extern void cm_fn_005e8290(const char *arg1, const char *arg2, int arg3);
extern u32 CM_THISCALL cm_fn_006826d0(u32 self, u32 index);
extern void CM_THISCALL cm_fn_00775000(u32 self, u32 arg);
extern void CM_THISCALL cm_fn_0086bdd0(
    u32 self, u32 row, u32 target, u32 zero, u32 a0, u32 onee);
extern void CM_THISCALL cm_fn_0086c1d0(
    u32 self, u32 row_id, u32 self_plus4, u32 four1c, u32 sixtyfour);
extern int CM_CDECL cm_fn_0090cfc0(int count);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(
    u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 allocation);

void CM_FASTCALL cm_fn_00919910(u32 self)
{
    volatile u8 scratch[0x420];
    volatile struct cmre_seh_registration frame;
    void *previous = CM_FS(void *, 0);

    frame.state = 0xffffffffu;
    frame.handler = 0x009659feu;
    frame.prev = previous;
    CM_FS(void *, 0) = (void *)&frame;

    if (*(volatile u8 *)(self + 0x100u) == 0xffu) {
        s32 outer = -1;
        s8 bound = *(volatile s8 *)(self + 0xffu);
        if (bound >= -1) {
            do {
                u32 entity = self;
                if (outer != -1) {
                    u32 list = *(volatile u32 *)(self + 0x0cu);
                    entity = *(volatile u32 *)(list + (u32)outer * 4u);
                }
                s16 row_count = *(volatile s16 *)(entity + 0x3eu);
                if (row_count > 0) {
                    s32 row_index = 0;
                    u32 row_offset = 0;
                    do {
                        u32 row_list = *(volatile u32 *)(entity + 0xb1u);
                        u32 row = row_list + row_offset;
                        u8 state = *(volatile u8 *)(row + 0x37u);
                        if (state != 7u) {
                            u32 self_plus4 = *(volatile u32 *)(self + 4u);
                            u32 row_id = *(volatile u32 *)row;
                            u32 receiver = CM_GLOBAL(volatile u32, 0x00dd7f00);
                            cm_fn_0086c1d0(receiver, row_id, self_plus4,
                                           0x41cu, 0x64u);

                            row_list = *(volatile u32 *)(entity + 0xb1u);
                            row = row_list + row_offset;
                            u32 row_object = *(volatile u32 *)row;
                            u32 source = *(volatile u32 *)(row_object + 0x53u);
                            receiver = CM_GLOBAL(volatile u32, 0x00b74318);
                            cm_fn_00775000(receiver, source);
                        }
                        row_index++;
                        row_offset += 0x3bu;
                        row_count = *(volatile s16 *)(entity + 0x3eu);
                    } while (row_index < row_count);
                }
                outer++;
                bound = *(volatile s8 *)(self + 0xffu);
            } while (outer <= bound);
        }
    } else {
        u8 second_mode = *(volatile u8 *)(self + 0x101u);
        s8 initial_bound = *(volatile s8 *)(self + 0xffu);
        *(volatile u32 *)(scratch + 0x14u) = 0xffffffffu;
        if (second_mode == 0xffu) {
        s32 outer = -1;
        s8 bound = initial_bound;
        if (bound >= -1) {
            do {
                u32 entity = self;
                if (outer != -1) {
                    u32 list = *(volatile u32 *)(self + 0x0cu);
                    entity = *(volatile u32 *)(list + (u32)outer * 4u);
                }
                s16 row_count = *(volatile s16 *)(entity + 0x3eu);
                if (row_count > 0) {
                    s32 row_index = 0;
                    u32 row_offset = 0;
                    do {
                        u32 row_list = *(volatile u32 *)(entity + 0xb1u);
                        u32 row = row_list + row_offset;
                        u8 state = *(volatile u8 *)(row + 0x37u);
                        if (state != 7u) {
                            u32 self_plus4 = *(volatile u32 *)(self + 4u);
                            u32 row_id = *(volatile u32 *)row;
                            u32 receiver = CM_GLOBAL(volatile u32, 0x00dd7f00);
                            cm_fn_0086c1d0(receiver, row_id, self_plus4,
                                           0x41cu, 0x64u);

                            row_list = *(volatile u32 *)(entity + 0xb1u);
                            row = row_list + row_offset;
                            u32 row_object = *(volatile u32 *)row;
                            u32 source = *(volatile u32 *)(row_object + 0x53u);
                            receiver = CM_GLOBAL(volatile u32, 0x00b74318);
                            cm_fn_00775000(receiver, source);
                        }
                        row_index++;
                        row_offset += 0x3bu;
                        row_count = *(volatile s16 *)(entity + 0x3eu);
                    } while (row_index < row_count);
                }
                outer++;
                bound = *(volatile s8 *)(self + 0xffu);
            } while (outer <= bound);
        }
        } else {
        u8 mode = *(volatile u8 *)(self + 0xf6u);
        s32 count = (mode == 0u || mode == 1u) ? 8 : 6;
        *(volatile s32 *)(scratch + 0x00u) = count;
        s16 count_word = *(volatile s16 *)(scratch + 0x00u);
        s32 capacity = (s32)count_word + 1;
        u32 allocation = (u32)cm_fn_00944e46((u32)capacity * 4u);
        *(volatile u32 *)(scratch + 0x0cu) = allocation;
        if (allocation == 0u) {
            cm_fn_00944cff(0x00ad4a6cu, 0u, 0u,
                           (void *)(scratch + 0x220u), (void *)(scratch + 0x20u));
            u32 message = (u32)(unsigned long)cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, message,
                           (u32)(unsigned long)(scratch + 0x220u),
                           (u32)(unsigned long)(scratch + 0x20u), 0x444u);
            cm_fn_005e8290((const char *)0x009870e0u,
                           (const char *)0x00ae24d0u, 0);
            CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
        } else {
            s8 selected = *(volatile s8 *)(self + 0x100u);
            u32 object_list = *(volatile u32 *)(self + 0x0cu);
            u32 selected_entity = *(volatile u32 *)(object_list + (u32)(s32)selected * 4u);
            u32 *list = (u32 *)(unsigned long)allocation;
            for (s32 index = 0; index < capacity; ++index) {
                u32 item = cm_fn_006826d0(selected_entity, (u32)index);
                u32 item_id = *(volatile u32 *)(unsigned long)item;
                *(volatile u32 *)&list[index] = item_id;
            }

            s32 random_index = cm_fn_0090cfc0(capacity);
            u32 target = *(volatile u32 *)&list[(u32)random_index];
            *(volatile u32 *)(scratch + 0x18u) = target;
            s32 count_index = *(volatile s16 *)(scratch + 0x00u);
            u32 last_item = *(volatile u32 *)&list[(u32)count_index];
            *(volatile u32 *)&list[(u32)random_index] = last_item;

            s32 outer = -1;
            s8 bound = *(volatile s8 *)(self + 0xffu);
            if (bound >= -1) {
                do {
                    u32 entity = self;
                    if (outer != -1) {
                        object_list = *(volatile u32 *)(self + 0x0cu);
                        entity = *(volatile u32 *)(object_list + (u32)outer * 4u);
                    }
                    selected_entity = entity;
                    s16 row_count = *(volatile s16 *)(entity + 0x3eu);
                    if (row_count > 0) {
                        s32 row_index = 0;
                        u32 row_offset = 0;
                        do {
                            s16 count_for_scan = *(volatile s16 *)(scratch + 0x00u);
                            s32 match_index = 0;
                            u32 row = 0u;
                            u32 row_id = 0u;
                            u32 row_list = 0u;
                            int found = 0;
                            if (count_for_scan > 0) {
                                row_list = *(volatile u32 *)(entity + 0xb1u);
                                row = row_list + row_offset;
                                row_id = *(volatile u32 *)row;
                                u32 scan_address = (u32)(unsigned long)list;
                                for (;;) {
                                    if (*(volatile u32 *)(scan_address + (u32)match_index * 4u) == row_id) {
                                        found = 1;
                                        break;
                                    }
                                    match_index++;
                                    count_for_scan = *(volatile s16 *)(scratch + 0x00u);
                                    if (match_index >= count_for_scan) {
                                        break;
                                    }
                                }
                            }
                            if (found) {
                                *(volatile u8 *)(row + 0x37u) = 1;
                                u32 saved_target = *(volatile u32 *)(scratch + 0x18u);
                                u32 list_item = *(volatile u32 *)&list[match_index];
                                u32 call_target;
                                if (list_item == saved_target) {
                                    u32 global_a = CM_GLOBAL(volatile u32, 0x009cf774);
                                    u32 global_b = CM_GLOBAL(volatile u32, 0x00ae23d0);
                                    u32 product = global_a * 0x6bu;
                                    call_target = product + global_b;
                                } else {
                                    call_target = *(volatile u32 *)(self + 4u);
                                }
                                row_list = *(volatile u32 *)(entity + 0xb1u);
                                u32 matched_row = *(volatile u32 *)(row_list + row_offset);
                                u32 receiver = CM_GLOBAL(volatile u32, 0x00dd7f00);
                                cm_fn_0086bdd0(receiver, matched_row, call_target,
                                               0u, 0xa0u, 0x1eu);
                            } else {
                                row_list = *(volatile u32 *)(entity + 0xb1u);
                                row = row_list + row_offset;
                                u32 saved_target = *(volatile u32 *)(scratch + 0x18u);
                                row_id = *(volatile u32 *)row;
                                if (row_id == saved_target) {
                                    *(volatile u8 *)(row + 0x37u) = 1;
                                    u32 second_target = *(volatile u32 *)(scratch + 0x18u);
                                    u32 list_item = *(volatile u32 *)&list[match_index];
                                    u32 call_target;
                                    if (list_item == second_target) {
                                        u32 global_a = CM_GLOBAL(volatile u32, 0x009cf774);
                                        u32 global_b = CM_GLOBAL(volatile u32, 0x00ae23d0);
                                        u32 product = global_a * 0x6bu;
                                        call_target = product + global_b;
                                    } else {
                                        call_target = *(volatile u32 *)(self + 4u);
                                    }
                                    row_list = *(volatile u32 *)(entity + 0xb1u);
                                    u32 matched_row = *(volatile u32 *)(row_list + row_offset);
                                    u32 receiver = CM_GLOBAL(volatile u32, 0x00dd7f00);
                                    cm_fn_0086bdd0(receiver, matched_row, call_target,
                                                   0u, 0xa0u, 0x1eu);
                                } else if (*(volatile u8 *)(row + 0x37u) != 7u) {
                                    u32 self_plus4 = *(volatile u32 *)(self + 4u);
                                    u32 receiver = CM_GLOBAL(volatile u32, 0x00dd7f00);
                                    cm_fn_0086c1d0(receiver, row_id, self_plus4,
                                                   0x41cu, 0x64u);
                                    row_list = *(volatile u32 *)(entity + 0xb1u);
                                    row = row_list + row_offset;
                                    u32 row_object = *(volatile u32 *)row;
                                    u32 source = *(volatile u32 *)(row_object + 0x53u);
                                    receiver = CM_GLOBAL(volatile u32, 0x00b74318);
                                    cm_fn_00775000(receiver, source);
                                }
                            }
                            row_index++;
                            row_offset += 0x3bu;
                            row_count = *(volatile s16 *)(entity + 0x3eu);
                        } while (row_index < row_count);
                    }
                    outer++;
                    bound = *(volatile s8 *)(self + 0xffu);
                } while (outer <= bound);
            }

            s16 selected_rows = *(volatile s16 *)(selected_entity + 0x3eu);
            s16 selected_count = *(volatile s16 *)(scratch + 0x00u);
            s32 extra_count = (s32)selected_rows - (s32)selected_count;
            if (extra_count > 0) {
                u32 row_offset = 0;
                for (s32 index = 0; index < extra_count; ++index) {
                    u32 row_list = *(volatile u32 *)(selected_entity + 0xb1u);
                    u32 vtable = *(volatile u32 *)self;
                    u32 row = *(volatile u32 *)(row_list + row_offset);
                    u32 slot_address = *(volatile u32 *)(vtable + 0x68u);
                    cm_fn_00919910_slot68 slot =
                        (cm_fn_00919910_slot68)(unsigned long)
                        slot_address;
                    slot(self, row, 0xffffffffu, 1u, 1u, 1u);
                    selected_rows = *(volatile s16 *)(selected_entity + 0x3eu);
                    selected_count = *(volatile s16 *)(scratch + 0x00u);
                    extra_count = (s32)selected_rows - (s32)selected_count;
                    row_offset += 0x3bu;
                }
            }

            u32 vtable = *(volatile u32 *)self;
            u8 final_mode = *(volatile u8 *)(self + 0x101u);
            u32 slot3c_target = *(volatile u32 *)(vtable + 0x3cu);
            cm_fn_00919910_slot3c slot3c =
                (cm_fn_00919910_slot3c)(unsigned long)slot3c_target;
            u32 slot3c_result = slot3c(self, final_mode,
                                       (void *)(scratch + 0x14u),
                                       (void *)(scratch + 0x1cu), 0u);

            u32 object_allocation = (u32)cm_fn_00944cf1(0xb2u);
            frame.state = 0u;
            u32 constructed = 0u;
            if (object_allocation != 0u) {
                u32 out1c_raw = *(volatile u32 *)(scratch + 0x1cu);
                u8 selected_byte = *(volatile u8 *)(self + 0x101u);
                u16 self_plus40 = *(volatile u16 *)(self + 0x40u);
                u32 self_plus4 = *(volatile u32 *)(self + 4u);
                u32 out14_raw = *(volatile u32 *)(scratch + 0x14u);
                u32 allocated_list = *(volatile u32 *)(scratch + 0x0cu);
                u32 count_raw = *(volatile u32 *)(scratch + 0x00u);
                u32 *made = cm_fn_00522e00(
                    (u32 *)(unsigned long)object_allocation, self, (s16)count_raw,
                    (u32 *)(unsigned long)allocated_list, (s16)out14_raw, self_plus4,
                    (u32 *)(unsigned long)slot3c_result, self_plus40,
                    selected_byte, 4u, (u16)out1c_raw, 0x14u, 0u, 0u, 0u, 0u);
                constructed = (u32)(unsigned long)made;
            }
            s8 store_index = *(volatile s8 *)(self + 0x101u);
            object_list = *(volatile u32 *)(self + 0x0cu);
            frame.state = 0xffffffffu;
            *(volatile u32 *)(object_list + (u32)(s32)store_index * 4u) = constructed;
            cm_fn_009452ca(*(volatile u32 *)(scratch + 0x0cu));
            cm_fn_009452ca(slot3c_result);

            selected = *(volatile s8 *)(self + 0x101u);
            object_list = *(volatile u32 *)(self + 0x0cu);
            u32 stored_object = *(volatile u32 *)(object_list + (u32)(s32)selected * 4u);
            if (stored_object == 0u) {
                cm_fn_00944cff(0x00ad4a6cu, 0u, 0u,
                               (void *)(scratch + 0x320u),
                               (void *)(scratch + 0x120u));
                u32 message = (u32)(unsigned long)cm_fn_0090d130();
                cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, message,
                               (u32)(unsigned long)(scratch + 0x320u),
                               (u32)(unsigned long)(scratch + 0x120u), 0x49fu);
                cm_fn_005e8290((const char *)0x009870e0u,
                               (const char *)0x00ae24d0u, 0);
                CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
            } else {
                cm_fn_0051c800(stored_object, 0u);
                s8 assigned = *(volatile s8 *)(self + 0x101u);
                *(volatile u32 *)(self + 0x30u) = (u32)(s32)assigned;
            }
        }
    }
    }

    CM_FS(void *, 0) = frame.prev;
}

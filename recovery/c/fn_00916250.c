/* Recovered from the pinned 3.9.68 listing at preferred VA 0x00400000. */
#include "cmre.h"

extern u32 CM_CDECL cm_fn_004afd80(u32 arg1, u8 arg2);
extern int CM_CDECL cm_fn_0053b3d0(int *record);
extern void cm_fn_005e8290(const char *caption, const char *text, int flags);
extern u8 CM_THISCALL cm_fn_005fa730(u8 *object, s32 arg1, s16 arg2,
                                     s32 *arg3, s32 *arg4, s32 arg5);
extern char *cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3,
                                   void *arg4, void *arg5);
extern void CM_CDECL cm_fn_00945333(u32 arg1, u32 arg2, u32 arg3, u32 arg4);
extern void *CM_CDECL cm_fn_00945501(u32 size, u32 flags);

#define L8(off)  (*(volatile u8 *)(void *)&scratch[(off)])
#define L32(off) (*(volatile u32 *)(void *)&scratch[(off)])
#define GLOBAL32(va) CM_GLOBAL(volatile u32, (va))

/* Each initializer row reloads its index global, then the array base, calls
   the original helper, reloads self->b2, and stores one DWORD. */
#define INIT_ROW(index_va, output_off) do { \
    u32 row_index_ = GLOBAL32(index_va); \
    u32 row_base_ = GLOBAL32(0x00ae23a8); \
    u32 row_result_ = (u32)cm_fn_0053b3d0( \
        (int *)(row_base_ + row_index_ * 0x122u)); \
    u32 row_output_ = *(volatile u32 *)(self + 0xb2); \
    *(volatile u32 *)(row_output_ + (output_off)) = row_result_; \
} while (0)

u32 CM_THISCALL cm_fn_00916250(void *object)
{
    volatile u8 scratch[0x214];
    u32 self = (u32)object;
    u32 context;
    s8 helper_count;
    s8 candidate_index;
    u32 row_base;
    u32 year;
    u32 row_address;
    u32 linked;
    u32 row_value;
    u32 output;
    u32 result;
    u32 result_is_zero;
    u32 result_pointer;
    s32 counter;
    s32 output_offset;
    s32 loop_index;
    s32 loop_limit;

    *(volatile u8 *)(self + 0xba) = 0;
    *(volatile u8 *)(self + 0xbb) = 0;
    *(volatile u8 *)(self + 0xbc) = 0;

    /* The listing reads B5E134 before these local dword initializers. */
    context = GLOBAL32(0x00b5e134);
    L32(4) = 0;
    L32(0) = 0;
    L32(8) = 0xffffffffu;
    L32(0xc) = 0xffffffffu;

    if (context == 0) {
        (void)cm_fn_00944cff(0x00ad4850, 0, 0,
                             (void *)&scratch[0x114], (void *)&scratch[0x14]);
        result = (u32)cm_fn_0090d130();
        (void)cm_fn_00944c9f(0x00ae24d0, 0x009870e8, result,
                             (u32)&scratch[0x114], (u32)&scratch[0x14], 0x467u);
        cm_fn_005e8290((const char *)0x009870e0,
                       (const char *)0x00ae24d0, 0);
        GLOBAL32(0x00b67a34) = 0;
    } else {
        u16 year_word;
        s32 helper_arg1;
        u32 helper_arg3;
        u32 helper_arg4;

        year_word = *(volatile u16 *)(self + 0x40);
        year_word = (u16)(year_word + 2u);
        helper_arg3 = (u32)&scratch[8];
        helper_arg4 = (u32)&scratch[0xc];
        helper_arg1 = (s32)GLOBAL32(0x009cf79c);
        helper_count = (s8)cm_fn_005fa730((u8 *)context, helper_arg1,
                                          (s16)year_word,
                                          (s32 *)helper_arg3,
                                          (s32 *)helper_arg4, 1);
        candidate_index = 0;
        if (helper_count > 0) {
            while (candidate_index < helper_count) {
                u32 candidate = L32(8u + (u32)(s32)candidate_index * 4u);
                if (candidate == 0xffffffffu)
                    break;
                row_base = GLOBAL32(0x00ae23a8);
                row_address = row_base + candidate * 0x122u;
                linked = *(volatile u32 *)(row_address + 0x71u);
                if (linked != 0) {
                    row_value = *(volatile u32 *)linked;
                    if (row_value == GLOBAL32(0x009cfa14)) {
                        u8 count = *(volatile u8 *)(self + 0xba);
                        *(volatile u8 *)(self + 0xba) = (u8)(count + 1u);
                        row_base = GLOBAL32(0x00ae23a8);
                        if (*(volatile u8 *)(row_base + candidate * 0x122u + 0x75u) == 6) {
                            count = *(volatile u8 *)(self + 0xbb);
                            *(volatile u8 *)(self + 0xbb) = (u8)(count + 1u);
                        } else {
                            count = *(volatile u8 *)(self + 0xbc);
                            *(volatile u8 *)(self + 0xbc) = (u8)(count + 1u);
                        }
                    }
                }
                candidate_index = (s8)(candidate_index + 1);
            }
        }
    }

    year = GLOBAL32(0x009cf79c);
    row_base = GLOBAL32(0x00ae23d0);
    result = cm_fn_004afd80(year * 0x6bu + row_base, 0);
    result_is_zero = (result == 0);
    L32(0x10) = result;
    if (!result_is_zero) {
        u32 source = *(volatile u32 *)(result + 0x53u);
        if (source != 0) {
            linked = *(volatile u32 *)(source + 0x71u);
            if (linked != 0) {
                u32 context_value = *(volatile u32 *)linked;
                u32 expected = GLOBAL32(0x009cfa14);
                if (context_value == expected) {
                    row_value = *(volatile u32 *)source;
                    if (row_value != L32(8) && row_value != L32(0xc)) {
                        u8 count = *(volatile u8 *)(self + 0xba);
                        *(volatile u8 *)(self + 0xba) = (u8)(count + 1u);
                        source = *(volatile u32 *)(result + 0x53u);
                        if (*(volatile u8 *)(source + 0x75u) == 6) {
                            count = *(volatile u8 *)(self + 0xbb);
                            *(volatile u8 *)(self + 0xbb) = (u8)(count + 1u);
                        } else {
                            count = *(volatile u8 *)(self + 0xbc);
                            *(volatile u8 *)(self + 0xbc) = (u8)(count + 1u);
                        }
                    }
                }
            }
        }
    }

    *(volatile u32 *)(self + 0xb6) = 0x23;
    result_pointer = (u32)cm_fn_00945501(0x23, 4);
    *(volatile u32 *)(self + 0xb2) = result_pointer;
    if (result_pointer == 0) {
        (void)cm_fn_00944cff(0x00ad4850, 0, 0,
                             (void *)&scratch[0x14], (void *)&scratch[0x114]);
        result = (u32)cm_fn_0090d130();
        (void)cm_fn_00944c9f(0x00ae24d0, 0x009870e8, result,
                             (u32)&scratch[0x14], (u32)&scratch[0x114], 0x4aau);
        cm_fn_005e8290((const char *)0x009870e0,
                       (const char *)0x00ae24d0, 0);
        GLOBAL32(0x00b67a34) = 0;
        return 0;
    }

    if (*(volatile s16 *)(self + 0x40) == 2000) {
        INIT_ROW(0x009cf2b0, 0x00);
        INIT_ROW(0x009cf288, 0x04);
        INIT_ROW(0x009cf494, 0x08);
        INIT_ROW(0x009cf4a0, 0x0c);
        INIT_ROW(0x009cf220, 0x10);
        INIT_ROW(0x009cf438, 0x14);
        INIT_ROW(0x009cf23c, 0x18);
        INIT_ROW(0x009cf320, 0x1c);
        INIT_ROW(0x009cf498, 0x20);
        INIT_ROW(0x009cf53c, 0x24);
        INIT_ROW(0x009cf490, 0x28);
        INIT_ROW(0x009cf538, 0x2c);
        INIT_ROW(0x009cf330, 0x30);
        INIT_ROW(0x009cf214, 0x34);
        INIT_ROW(0x009cf534, 0x38);
        INIT_ROW(0x009cf250, 0x3c);
        INIT_ROW(0x009cf4d4, 0x40);
        INIT_ROW(0x009cf3f0, 0x44);
        INIT_ROW(0x009cf2cc, 0x48);
        INIT_ROW(0x009cf544, 0x4c);
        INIT_ROW(0x009cf2c8, 0x50);
        INIT_ROW(0x009cf334, 0x54);
        INIT_ROW(0x009cf540, 0x58);
        INIT_ROW(0x009cf230, 0x5c);
        INIT_ROW(0x009cf368, 0x60);
        INIT_ROW(0x009cf33c, 0x64);
        INIT_ROW(0x009cf2e0, 0x68);
        INIT_ROW(0x009cf324, 0x6c);
        INIT_ROW(0x009cf418, 0x70);
        INIT_ROW(0x009cf248, 0x74);
        INIT_ROW(0x009cf3f8, 0x78);
        INIT_ROW(0x009cf280, 0x7c);
        INIT_ROW(0x009cf2a8, 0x80);
        INIT_ROW(0x009cf3d0, 0x84);
        INIT_ROW(0x009cf4f8, 0x88);
        return 1;
    }

    loop_limit = (s32)GLOBAL32(0x00ae2350);
    row_base = GLOBAL32(0x00ae23a8);
    if (loop_limit <= 0)
        goto finish_scan;
    output_offset = 0x64;
    loop_index = 0;
    do {
        u32 row_link = *(volatile u32 *)(row_base + 0x71u);
        u32 row_is_eligible = (row_link == 0);
        if (row_link != 0) {
            u32 row_context = *(volatile u32 *)row_link;
            u32 expected_context = GLOBAL32(0x009cfa14);
            row_is_eligible = (row_context == expected_context);
        }
        if (row_is_eligible) {
            result = (u32)cm_fn_0053b3d0((int *)row_base);
            row_value = *(volatile u32 *)row_base;
            if (row_value != L32(8) && row_value != L32(0xc) &&
                result != L32(0x10)) {
                u8 type = *(volatile u8 *)(row_base + 0x75u);
                counter = (s32)L32(4);
                if (type == 6 && counter < 0x19) {
                    u32 next = (u32)(counter + 1);
                    output = *(volatile u32 *)(self + 0xb2);
                    L32(4) = next;
                    *(volatile u32 *)(output + next * 4u - 4u) = result;
                    counter = (s32)next;
                } else if (type == 7 && output_offset < 0x8c) {
                    output = *(volatile u32 *)(self + 0xb2);
                    *(volatile u32 *)(output + (u32)output_offset) = result;
                    L32(0) = L32(0) + 1u;
                    output_offset += 4;
                }
                if (counter == 0x19 - (s8)*(volatile u8 *)(self + 0xbb)) {
                    u8 count7 = *(volatile u8 *)(self + 0xbc);
                    s32 current_count7 = (s32)L32(0);
                    if (current_count7 == 10 - (s8)count7)
                        goto final_cached_count6;
                }
            }
        }
        loop_limit = (s32)GLOBAL32(0x00ae2350);
        loop_index = (s16)(loop_index + 1);
        row_base += 0x122u;
    } while (loop_index < loop_limit);

finish_scan:
    counter = (s32)L32(4);

final_cached_count6:
    if (counter != 0x19 - (s8)*(volatile u8 *)(self + 0xbb))
        return 0;
    {
        u8 count7 = *(volatile u8 *)(self + 0xbc);
        s32 current_count7 = (s32)L32(0);
        if (current_count7 != 10 - (s8)count7)
            return 0;
    }

    output = *(volatile u32 *)(self + 0xb2);
    result = (u32)counter;
    cm_fn_00945333(output, result, 4, 0x004b0810);
    output = *(volatile u32 *)(self + 0xb2);
    result = L32(0);
    cm_fn_00945333(output + 0x64u, result, 4, 0x004b0810);
    return 1;
}

#undef INIT_ROW
#undef GLOBAL32
#undef L32
#undef L8

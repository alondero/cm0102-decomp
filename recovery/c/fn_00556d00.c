#include "cmre.h"
/* Preferred-image VA 0x00556d00, image base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   thiscall: ECX carries the object, RET 8 pops two stack words. The first
   stack word is read as a dword (arg1), the second as a signed word (arg2):
   0x00556d06 is `movsx eax, word ptr [esp + 0x214]`, so arg2 is declared
   s16 here, the same signed word the peer wrapper's boundary uses.
   The original 0x20c frame is one block: record pointer +0x000, the two
   dwords of the search key struct +0x004/+0x008 (the key argument stored at
   0x00556d5e and the first dword of the table entry loaded at 0x00556d7a),
   and the two 0x100 path buffers +0x00c and +0x10c.
   Return: the routine has one exit, 0x00556e85. 0x00556e6e loads EAX from
   the record slot (+0x000) and the two finishing stores at 0x00556e75 and
   0x00556e78 do not write EAX, so every path leaves that record pointer in
   EAX. The entry EAX is dead: 0x00556d06 overwrites it before any read.
   0x00556e90 forwards the value unchanged (0x00556f67, 0x00556fe2), so the
   return is a full 32-bit EAX and not a void draft. */
extern u32 CM_THISCALL cm_fn_007e8590(u32 self, u32 key);
/* 0x00946464 is the frozen legacy cm_lang_bsearch; the original call at
   0x00556d8a/0x00556dc3 leaves the found entry pointer in EAX and the
   listing tests it with `test eax, eax`. */
extern void *CM_CDECL cm_fn_00946464(u32 key, u32 table, u32 count, u32 stride,
                                      void *compare);
/* 0x00945333: the physical call shape at 0x00556e5b is four stack words
   (compare at 0x00556e3f, width at 0x00556e44, count at 0x00556e59, base at
   0x00556e5a) plus the incidental register inputs EAX (count, written by
   0x00556e55) and EDX (the second dword of the entry, written by
   0x00556e4b). The accepted C keeps those two physical positions through
   regparm(2) and returns EDX:EAX. */
extern unsigned long long CM_CDECL __attribute__((regparm(2)))
    cm_fn_00945333(u32 eax_in, u32 edx_in, volatile u32 base_arg,
                   volatile u32 count_arg, volatile u32 width_arg,
                   volatile u32 compare_arg);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3,
                                    void *arg4, void *arg5);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern char *CM_CDECL cm_fn_0090d130(void);
/* 0x005e8290 writes EAX on every exit: 1 at 0x005e829d on the early exit
   0x005e82ca, the imported MessageBoxA result at 0x005e8890 and the last
   computed value at 0x005e8878. The physical return width is EAX, so the
   packet's void draft is not the contract. This routine overwrites EAX at
   0x00556e6e and never reads it before then. */
extern u32 CM_CDECL cm_fn_005e8290(const char *param_1, const char *param_2,
                                    int param_3);

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))

u32 CM_THISCALL cm_fn_00556d00(u32 self, u32 arg1, s16 arg2)
{
    u8 frame[0x20c];
    volatile u32 *record_slot = (volatile u32 *)(frame + 0x000);
    volatile u32 *key_id = (volatile u32 *)(frame + 0x008);
    u8 *name_buf = frame + 0x00c;
    u8 *ext_buf = frame + 0x10c;
    u32 counter, record, objects, table, base, off, entry, key, found, v0, v1;
    u8 tag;
    s16 count_arg;
    char *version;

    /* 0x00556d13..0x00556d26: record = this->f4 + (s16)arg2 * 20 */
    *record_slot = *(volatile u32 *)(self + 4u) + (u32)((s32)arg2 * 20);

    for (counter = 0u; counter != 4u; counter++) {
        /* 0x00556d2a..0x00556d31 */
        record = *record_slot;
        tag = *(volatile u8 *)(record + counter + 0x0cu);
        if (tag == 0xffu)
            continue;                        /* 0x00556d37 */

        objects = *(volatile u32 *)(self);   /* 0x00556d40 */
        /* 0x00556d43: ECX = objects[(s8)tag]; the stack argument is arg1 */
        cm_fn_007e8590(*(volatile u32 *)(objects + (u32)((s32)(s8)tag * 4)), arg1);
        if (counter != 3u)
            continue;                        /* 0x00556d4e */

        /* the fourth slot */
        base = D(0x00ae23bc);                       /* 0x00556d58 */
        *(volatile u32 *)(frame + 0x004) = arg1;    /* 0x00556d5e */
        off = (arg1 + (arg1 * 5u) * 2u) * 5u * 2u; /* 0x00556d62..0x00556d70 */
        /* 0x00556d72 loads a pointer from the table, 0x00556d7a loads its
           first dword; the recovered search key is that first dword. */
        entry = *(volatile u32 *)(base + off + 0x1au); /* 0x00556d72 */
        count_arg = (s16)W(self + 0x12u);           /* 0x00556d76 */
        key = *(volatile u32 *)entry;               /* 0x00556d7a */
        table = *(volatile u32 *)(self + 0x0cu);    /* 0x00556d7c */
        *key_id = key;                              /* 0x00556d7f */
        found = (u32)cm_fn_00946464((u32)(frame + 0x004), table,
                                     (u32)(s32)count_arg, 8u, (void *)0x0055e5f0u);
        if (found != 0u)
            goto found;

        base = D(0x00ae23bc);                       /* 0x00556d9a */
        entry = *(volatile u32 *)(base + off + 0x1eu); /* 0x00556da0 */
        if (entry != 0u) {                          /* 0x00556da4 tests the
                                                       loaded pointer */
            count_arg = (s16)W(self + 0x12u);       /* 0x00556da8 */
            key = *(volatile u32 *)entry;           /* 0x00556dac */
            table = *(volatile u32 *)(self + 0x0cu);        /* 0x00556dae */
            *key_id = key;                          /* 0x00556db6 */
            found = (u32)cm_fn_00946464((u32)(frame + 0x004), table,
                                         (u32)(s32)count_arg, 8u, (void *)0x0055e5f0u);
            if (found != 0u)
                goto found;
        }

        /* 0x00556dcf: the diagnostic path */
        cm_fn_00944cff(0x009b8f18u, 0u, 0u, ext_buf, name_buf);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, ext_buf, name_buf, 0x47du);
        cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
        D(0x00b67a34) = 0u;
        continue;                        /* 0x00556e32 -> 0x00556e63 */

    found:
        /* 0x00556e34 */
        W(self + 0x12u) = (u16)(W(self + 0x12u) - 1u);
        table = *(volatile u32 *)(self + 0x0cu);            /* 0x00556e38 */
        count_arg = (s16)W(self + 0x12u);                   /* 0x00556e3b */
        v0 = *(volatile u32 *)(table + (u32)((s32)count_arg * 8));          /* 0x00556e46 */
        *(volatile u32 *)found = v0;                        /* 0x00556e49 */
        v1 = *(volatile u32 *)(table + (u32)((s32)count_arg * 8) + 4u);     /* 0x00556e4b */
        *(volatile u32 *)(found + 4u) = v1;                  /* 0x00556e4f */
        table = *(volatile u32 *)(self + 0x0cu);            /* 0x00556e52 */
        count_arg = (s16)W(self + 0x12u);                   /* 0x00556e55 */
        /* 0x00556e5b: EAX = the slot, EDX = the second dword of that entry */
        cm_fn_00945333((u32)(s32)count_arg, v1, table, (u32)(s32)count_arg,
                       8u, 0x0055e5f0u);
    }

    /* 0x00556e6e: EAX = the record slot; the finished slot is marked in it */
    record = *record_slot;
    *(volatile u8 *)record = 0xffu;                 /* 0x00556e75 */
    *(volatile u16 *)(record + 2u) = 0xffffu;        /* 0x00556e78 */
    return record;                        /* EAX at 0x00556e85, every path */
}

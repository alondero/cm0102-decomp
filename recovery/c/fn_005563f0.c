#include "cmre.h"
/* Preferred-image VA 0x005563f0, image base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   thiscall: ECX carries the object, RET 4 pops one stack word (arg1).
   The original 0x210 frame is one block: the ten byte record struct at
   +0x000 (head dword, counter byte, record dword, record flag byte), the
   record pointer at +0x00c, and the two 0x100 path buffers at +0x010 and
   +0x110. */
extern u32 CM_THISCALL cm_fn_004cf6d0(u32 self, u32 argument);
extern unsigned long long CM_FASTCALL cm_fn_0068d0d0(volatile u8 *self,
                                                      u32 edx_in, const u8 *src,
                                                      s16 count);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3,
                                   void *arg4, void *arg5);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern char *CM_CDECL cm_fn_0090d130(void);
extern void CM_CDECL cm_fn_005e8290(const char *param_1, const char *param_2,
                                    int param_3);

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))

void CM_THISCALL cm_fn_005563f0(u32 self, u32 arg1)
{
    u8 frame[0x210];
    volatile u32 *record_slot = (volatile u32 *)(frame + 0x00c);
    u8 *name_buf = frame + 0x010;
    u8 *ext_buf = frame + 0x110;
    u32 obj, idx, base, head, array, counter, objects, entry, v17, p53;
    u32 head2, rec0, current_record, argument = arg1;
    u8 tag, rec4, applied;
    char *version;

    obj = cm_fn_004cf6d0(D(0x00ae1050), argument); /* 0x00556408 */
    if (obj == 0u)
        goto diagnostic;                          /* 0x0055640f */

    obj = cm_fn_004cf6d0(D(0x00ae1050), argument); /* 0x0055641e */
    idx = *(volatile u32 *)(obj + 4u);            /* 0x00556423 */
    base = D(0x00ae23b4);                         /* 0x00556426 */
    base += idx;                                 /* 0x0055642c */
    base += (idx * 145u) * 4u;                   /* 0x0055642e..0x00556438 */
    head = *(volatile u32 *)argument;            /* 0x00556436 */
    *record_slot = base;                         /* 0x00556444 */
    current_record = base;                       /* original cached EDX */
    array = head * 29u + *(volatile u32 *)(self + 0x14u);   /* 0x00556448..0x0055644e */

    for (counter = 0u; counter != 3u; counter++) {
        u8 *rec = (u8 *)(array + counter * 5u);

        tag = *(volatile u8 *)rec;                /* 0x0055645d */
        if (tag == 0xffu)
            continue;                             /* 0x00556461 */
        objects = *(volatile u32 *)(self);        /* 0x00556463 */
        entry = *(volatile u32 *)(objects + (u32)((s32)(s8)tag * 4));  /* 0x00556469 */
        v17 = *(volatile u32 *)(entry + 0x17u);   /* 0x0055646c */
        if (v17 == 0xffffffffu) {                 /* 0x0055646f */
            u32 v1b = *(volatile u32 *)(entry + 0x1bu);   /* 0x00556481 */
            if (v1b == 0xffffffffu)
                continue;                         /* 0x00556487 */
            p53 = *(volatile u32 *)(current_record + 0x53u); /* 0x00556489 */
            if (p53 == 0u)
                goto apply;                       /* 0x0055648e */
            p53 = *(volatile u32 *)(p53 + 0x71u); /* 0x00556490 */
            if (p53 == 0u)
                goto apply;                       /* 0x00556495 */
            if (*(volatile u32 *)p53 == v1b)
                continue;                         /* 0x00556499 */
            goto apply;                           /* 0x0055647f */
        }
        p53 = *(volatile u32 *)(current_record + 0x53u);    /* 0x00556474 */
        if (p53 == 0u)
            goto apply;                           /* 0x00556479 */
        if (*(volatile u32 *)p53 == v17)
            continue;                             /* 0x0055647d */
        goto apply;                               /* 0x0055647f */

    apply:
        /* 0x005564a6..0x005564c1: the ten byte struct */
        *(volatile u8 *)(frame + 0x004) = (u8)counter;       /* 0x005564a6 */
        head2 = *(volatile u32 *)argument;                    /* 0x005564aa */
        rec0 = *(volatile u32 *)rec;                          /* 0x005564ac */
        *(volatile u32 *)(frame + 0x000) = head2;             /* 0x005564ae */
        *(volatile u32 *)(frame + 0x005) = rec0;              /* 0x005564b2 */
        rec4 = *(volatile u8 *)(rec + 4u);                    /* 0x005564b6 */
        *(volatile u8 *)(frame + 0x009) = rec4;               /* 0x005564c1 */
        cm_fn_0068d0d0((volatile u8 *)(self + 0x1cu), rec0,
                       (const u8 *)(frame + 0x000), 10);       /* 0x005564c5 */
        *(volatile u32 *)rec = 0xffffffffu;                   /* 0x005564cf */
        *(volatile u8 *)(rec + 4u) = 0xffu;                   /* 0x005564d1 */
        applied = *(volatile u8 *)(array + 0x1cu);             /* 0x005564d4 */
        /* 0x005564d7: EDX is reloaded AFTER the earlier OR EDX,-1 and
           stays cached across the following byte store and loop backedge. */
        current_record = *record_slot;
        *(volatile u8 *)(array + 0x1cu) = (u8)(applied + 1u); /* 0x005564dd */
    }
    return;

diagnostic:
    /* 0x005564fb */
    cm_fn_00944cff(0x009b8f18u, 0u, 0u, ext_buf, name_buf);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, ext_buf, name_buf, 0x260u);
    cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
    D(0x00b67a34) = 0u;
}

#include "cmre.h"

extern u32 CM_CDECL cm_fn_004afbd0(u32, u32, u32);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, int);
extern u32 CM_THISCALL cm_fn_006830b0(void *, u32, u32, u32);
extern void CM_THISCALL cm_fn_006831a0(void *, u32, u32, u32);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);

void CM_THISCALL cm_fn_008fe3a0(void *self)
{
    u8 scratch[512];
    u32 count = CM_GLOBAL(volatile u32, 0x00ae235c);
    u32 list = (u32)cm_fn_00944e46(count * 4);
    u32 n = 0, i = 0, record, p, q, base, index, j, off;
    s8 mode;
    if (!list) {
        char *text;
        cm_fn_00944cff(0x00aa1840, 0, 0, scratch + 256, scratch);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, scratch + 256, scratch, 0x27c);
        cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
        return;
    }
    count = CM_GLOBAL(volatile u32, 0x00ae235c);
    record = CM_GLOBAL(volatile u32, 0x00ae23b4);
    if ((s32)count > 0) {
        do {
            p = *(volatile u32 *)(record + 0x57);
            if (p && *(volatile u32 *)(record + 0x53) &&
                *(volatile u32 *)p == CM_GLOBAL(volatile u32, 0x009cf8cc)) {
                *(volatile u32 *)(list + (u32)(s32)(s16)n * 4) = record;
                n++;
            }
            count = CM_GLOBAL(volatile u32, 0x00ae235c);
            i++;
            record += 0x245;
        } while ((s32)i < (s32)count);
        if ((s16)n >= 10) {
            cm_fn_004afbd0(list, n, 10);
            for (i = 0; i < 10; i++) {
                p = CM_GLOBAL(volatile u32, 0x009cf8e0);
                q = CM_GLOBAL(volatile u32, 0x00ae23d0) + p * 107;
                p = *(volatile u32 *)(list + i * 4);
                cm_fn_006830b0(self, p, q, 1);
            }
            mode = -1;
            do {
                base = CM_GLOBAL(volatile u32, 0x00adadfc);
                index = CM_GLOBAL(volatile u32, 0x009cf8e0);
                if (mode != -1) {
                    base = *(volatile u32 *)(base + index * 4);
                    index = (u32)(s32)mode;
                    base = *(volatile u32 *)(base + 0xc);
                }
                p = *(volatile u32 *)(base + index * 4);
                j = 0;
                if (*(volatile s16 *)(p + 0x3e) > 0) {
                    off = 0;
                    do {
                        q = *(volatile u32 *)(p + 0xb1) + off;
                        if (*(volatile u8 *)(q + 0x37) == 3) {
                            index = CM_GLOBAL(volatile u32, 0x009cf8cc);
                            base = *(volatile u32 *)q;
                            index = CM_GLOBAL(volatile u32, 0x00ae23d0) + index * 107;
                            cm_fn_006831a0(self, base, index, 1);
                            q = *(volatile u32 *)(p + 0xb1);
                            base = *(volatile u32 *)(q + off);
                            *(volatile u32 *)(base + 0x60) = 0;
                        }
                        index = (u32)(s32)*(volatile s16 *)(p + 0x3e);
                        j++;
                        off += 0x3b;
                    } while ((s32)j < (s32)index);
                }
                mode++;
            } while (mode < 4);
        }
    }
    cm_fn_009452ca(list);
}

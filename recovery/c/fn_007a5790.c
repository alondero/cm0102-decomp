#include "cmre.h"

extern void CM_FASTCALL cm_fn_0054a110(u16 *param_1);
extern int CM_THISCALL cm_fn_004cf5a0(void *self, u32 a);
extern u8 CM_THISCALL cm_fn_007a7310(void *self, u32 a, u32 b);
extern u32 CM_THISCALL cm_fn_007a8050(void *self, u32 a, u32 b);
extern void CM_THISCALL cm_fn_007a8510(void *self, u32 a);
extern void CM_CDECL cm_fn_00945333(u32 *param_1, u32 *param_2, u32 param_3, void *param_4);

typedef struct { void *prev; void *handler; s32 state; } cm_seh_record;

/* Thiscall (ECX = self), no stack arguments, `ret 0`, no return value.
   The SEH frame is one volatile record: state -1 at entry, 0 before the second
   fastcall, 1 before the third, and 2 before the record loop (it is not reset).
   A 40-byte scratch block `frame` holds the fastcall buffers at fixed offsets:
   frame+4, frame+20 and frame+28 are passed to 0x54a110 (ECX), and frame+12 is
   the output pointer for 0x7a7310 / 0x7a8050 / 0x7a8510.
   Records are 110 bytes apart from T = [0xae23bc] up to T + 110*N, N = [0xae2364]
   (re-read each pass). A record whose signed byte at +0x18 is below 30, with a
   non-null pointer at +0x39 whose +0x57 pointer's dword key indexes the table
   at 0x00adadfc (non-zero entry), is skipped. Otherwise (when the signed dword at
   +0 is below N - 16 and the byte at 0x00ae2d0c + 79*id + 0xb has no 0x30 bits)
   the record is searched in the self+4 array (count self+8, 26-byte entries):
   a record not found there is passed to 0x4cf5a0 on the global at 0x00ae1050
   (skip if the result's byte +0x49 is set), then 0x7a7310 (and, when it is zero,
   0x7a8050 followed by 0x7a8510 on success; otherwise 0x7a8510 directly).
   Finally 0x945333(self+4 array, self+8 count, 0x1a, 0x007ae2a0). */
void CM_THISCALL cm_fn_007a5790(void *self)
{
    volatile u8 *base = (volatile u8 *)self;
    volatile cm_seh_record seh;
    void *entry_prev;
    void *restore_prev;
    u8 frame[0x28];
    u32 n;
    u32 n_now;
    u32 t;
    u32 end;
    u32 rec;
    u32 id;
    u32 x;
    u32 y;
    u32 key;
    u32 g3;
    u32 g4;
    u32 m;
    u32 z;
    u32 k;
    u32 res;
    u8 al;
    u8 al2;
    s32 cnt;
    int skip;
    int skip2;

    seh.state = -1;
    seh.handler = (void *)0x0095f158;
    entry_prev = CM_FS(void *, 0);
    seh.prev = entry_prev;
    CM_FS(void *, 0) = (void *)&seh;

    cm_fn_0054a110((u16 *)(frame + 20));
    seh.state = 0;
    cm_fn_0054a110((u16 *)(frame + 28));
    seh.state = 1;
    cm_fn_0054a110((u16 *)(frame + 4));

    n = *(volatile u32 *)0x00ae2364;
    t = *(volatile u32 *)0x00ae23bc;
    seh.state = 2;
    end = t + 110u * n;

    if (t < end) {
        rec = t;
        n_now = n;   /* the first pass reuses the count read above (0x7a57d8) */
        for (;;) {
            skip = 0;
            if ((s8)*(volatile u8 *)(rec + 0x18) < 0x1e) {
                x = *(volatile u32 *)(rec + 0x39);
                if (x != 0) {
                    y = *(volatile u32 *)(x + 0x57);
                    if (y != 0) {
                        key = *(volatile u32 *)y;
                        g3 = *(volatile u32 *)0x00adadfc;
                        if (*(volatile u32 *)(g3 + key * 4u) != 0)
                            skip = 1;
                    }
                }
            }

            if (!skip) {
                id = *(volatile u32 *)rec;
                if (!((s32)id >= (s32)(n_now - 16u))) {
                    g4 = *(volatile u32 *)0x00ae2d0c;
                    if ((*(volatile u8 *)(g4 + id * 79u + 0xbu) & 0x30u) == 0) {
                        skip2 = 0;
                        if (rec != 0) {
                            cnt = *(volatile s32 *)(base + 8);
                            if (cnt > 0) {
                                z = *(volatile u32 *)(base + 4);
                                for (k = 0; k < (u32)cnt; k++) {
                                    if (*(volatile u32 *)z == id) {
                                        skip2 = 1;
                                        break;
                                    }
                                    z += 26u;
                                }
                            }
                        }
                        if (!skip2) {
                            res = (u32)cm_fn_004cf5a0((void *)*(volatile u32 *)0x00ae1050, rec);
                            skip = 0;
                            if (res != 0 && *(volatile u8 *)(res + 0x49) != 0)
                                skip = 1;
                            if (!skip) {
                                al = cm_fn_007a7310(self, rec, (u32)(frame + 12));
                                if (al == 0) {
                                    al2 = cm_fn_007a8050(self, rec, (u32)(frame + 12));
                                    if (al2 != 0)
                                        cm_fn_007a8510(self, (u32)(frame + 12));
                                } else {
                                    cm_fn_007a8510(self, (u32)(frame + 12));
                                }
                            }
                        }
                    }
                }
            }

            rec += 110u;
            if (!(rec < end))
                break;
            n_now = *(volatile u32 *)0x00ae2364;
        }
    }

    m = *(volatile u32 *)(base + 8);
    z = *(volatile u32 *)(base + 4);
    cm_fn_00945333((u32 *)z, (u32 *)m, 0x1au, (void *)0x007ae2a0);

    restore_prev = seh.prev;
    CM_FS(void *, 0) = restore_prev;
}

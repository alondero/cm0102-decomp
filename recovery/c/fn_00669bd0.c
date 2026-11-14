/* Translation of fn_00669bd0 (preferred-image VA 0x00669bd0, image base
   0x00400000).  cdecl: three stack arguments, EAX result (0 or 1).

   Original frame (L = base right after "sub esp, 0x20"):
     L+0x00  buf_a[8]    scratch buffer A
     L+0x08  buf_b[8]    scratch buffer B
     L+0x10  obj[16]     helper object (u32, u16, u16, u16, u8, u16, u16)
     L+0x20  SEH record  (prev, handler, state)
   The scratch area and the SEH record are kept in one allocation so that a
   helper writing past obj[] reaches the same bytes the original reaches. */
#include "cmre.h"

extern u32 CM_THISCALL cm_fn_00417c10(u32 *param_1, u32 *param_2, u16 *param_3);
extern unsigned long long CM_THISCALL cm_fn_00549ef0(void *self, u32 first, u32 second);
extern u32 CM_FASTCALL cm_fn_0054a110(u16 *param_1);

typedef struct {
    volatile u32 prev;
    volatile u32 handler;
    volatile u32 state;
} cm_seh_frame;

typedef struct {
    u32 buf_a[2];
    u32 buf_b[2];
    u32 obj[4];
    cm_seh_frame seh;
} cm_frame;

#define OBJ8(off)  (*(volatile u8 *)(objp + (off)))
#define OBJ16(off) (*(volatile u16 *)(objp + (off)))
#define OBJ32(off) (*(volatile u32 *)(objp + (off)))
#define REC8(off)  (*(volatile u8 *)(unsigned long)(rec + (off)))
#define REC16(off) (*(volatile u16 *)(unsigned long)(rec + (off)))

u32 cm_fn_00669bd0(void *object, void *target_a, void *target_b)
{
    cm_frame f;
    volatile u8 *objp;
    u32 eax_reg;
    u32 entry;
    u32 id;
    u16 val_a;
    u16 val_c;
    s16 val_d;
    u32 home2;
    u32 res;
    u32 year;
    u32 ret;

    objp = (volatile u8 *)f.obj;

    /* 0x00669bd0: push -1; push 0x95c658; push fs:[0]; mov fs:[0], esp */
    f.seh.prev = CM_FS(u32, 0);
    f.seh.handler = 0x0095c658u;
    f.seh.state = 0xffffffffu;
    CM_FS(u32, 0) = (u32)(unsigned long)&f.seh;

    /* 0x00669beb: lea ecx, [L+0x10]; call 0x54a110 */
    eax_reg = cm_fn_0054a110((u16 *)&f.obj[0]);

    /* 0x00669bfd: mov dword ptr [L+0x28], 0 (SEH state) */
    f.seh.state = 0;

    /* 0x00669bef: edi = 0x00b63e38 (default record) */
    entry = 0x00b63e38u;

    /* 0x00669c05: mov edx, dword ptr [arg1+0x53] */
    id = *(volatile u32 *)((volatile u8 *)object + 0x53u);

    if (id != 0u) {
        u32 scan = 0x00b63d60u;
        u32 idx = 0u;
        for (;;) {
            if (*(volatile u32 *)scan == id) {
                u32 rec = scan + 0x39u;
                u16 v_b;
                u16 v_8;
                u16 v_d;
                u8 v_a;
                u32 flag;

                /* 0x00669c2b: lea edi, [eax*8+0xb63d60] */
                entry = scan;
                /* 0x00669c24: lea eax, [ecx+ecx*8] */
                eax_reg = idx * 9u;
                /* 0x00669c35: push esi; push ecx; lea ecx, [L+0x10] */
                eax_reg = cm_fn_00417c10((u32 *)&f.obj[0], (u32 *)&f.buf_a[0],
                                         (u16 *)(unsigned long)rec);

                /* 0x00669c40: cx = [esi+0xb]; dx = [esi+8]; al = [esi+0xa] */
                v_b = REC16(0x0bu);
                v_8 = REC16(0x08u);
                v_a = REC8(0x0au);
                eax_reg = (eax_reg & 0xffffff00u) | v_a;
                /* 0x00669c4b: mov word ptr [L+0x1b], cx */
                OBJ16(0x0bu) = v_b;
                /* 0x00669c50: mov ecx, dword ptr [arg1+0x1db] */
                flag = *(volatile u32 *)((volatile u8 *)object + 0x1dbu);
                /* 0x00669c56: mov word ptr [L+0x18], dx */
                OBJ16(0x08u) = v_8;
                /* 0x00669c5b: mov dx, word ptr [esi+0xd] */
                v_d = REC16(0x0du);
                /* Both stores precede the JE at 0x00669c6b. */
                OBJ8(0x0au) = v_a;
                OBJ16(0x0du) = v_d;
                if (flag != 0xffffffffu) {
                    /* 0x00669c6d: cmp al, 3 / 4 / 5 */
                    if (v_a == 3u || v_a == 4u || v_a == 5u) {
                        /* 0x00669c79: mov byte ptr [L+0x1a], 4 */
                        OBJ8(0x0au) = 4u;
                        /* 0x00669c7e: mov word ptr [L+0x18], 0x1e */
                        OBJ16(0x08u) = 0x1eu;
                    }
                }
                break;
            }
            /* 0x00669c17: add eax, 0x48; inc ecx; cmp eax, 0xb64738 */
            scan += 0x48u;
            idx += 1u;
            if (scan >= 0x00b64738u) {
                eax_reg = scan;
                break;
            }
        }
    }

    /* 0x00669c85: mov ax, word ptr [edi+0x2a] */
    val_a = *(volatile u16 *)(unsigned long)(entry + 0x2au);
    /* 0x00669c89: mov cx, word ptr [L+0x10] */
    val_c = *(volatile u16 *)objp;
    /* 0x00669c8e: mov edx, dword ptr [0xae2c90] */
    val_d = (s16)*(volatile u32 *)0x00ae2c90u;
    eax_reg = (eax_reg & 0xffff0000u) | (u32)val_a;

    if ((s16)val_a <= (s16)val_c) {
        /* 0x00669de7: cmp dx, ax; jge 0x669e63 */
        if (val_d >= (s16)val_a) {
            /* 0x00669e63: cmp dx, cx; jle 0x669ecd */
            if (val_d <= (s16)val_c) {
                /* ---------------- 0x00669ecd ---------------- */
                if (target_a != 0) {
                    /* 0x00669ed1: dx = [0xae2c92]; [arg2 home] = dx */
                    year = *(volatile u16 *)0x00ae2c92u;
                    home2 = ((u32)(unsigned long)target_a & 0xffff0000u) | year;
                    /* 0x00669ee3: lea ecx, [L+0x08]; call 0x549ef0 */
                    res = (u32)cm_fn_00549ef0(&f.buf_b[0], eax_reg, home2);
                    /* 0x00669ef0: push eax; push edx(L+0x00); call 0x417c10 */
                    cm_fn_00417c10((u32 *)target_a, (u32 *)&f.buf_a[0],
                                   (u16 *)(unsigned long)res);
                }
                if (target_b == 0)
                    goto ret_one;
                /* 0x00669f01: ax = [0xae2c92]; [arg2 home] = ax */
                year = *(volatile u16 *)0x00ae2c92u;
                home2 = ((u32)(unsigned long)target_a & 0xffff0000u) | year;
                /* 0x00669f16: lea ecx, [L+0x08]; call 0x549ef0 */
                res = (u32)cm_fn_00549ef0(&f.buf_b[0], OBJ32(0x00u), home2);
                /* 0x00669f20: lea eax, [L+0x00]; push eax; call 0x417c10 */
                cm_fn_00417c10((u32 *)target_b, (u32 *)&f.buf_a[0],
                               (u16 *)(unsigned long)res);
                goto ret_one;
            } else {
                /* ---------------- 0x00669e6c ---------------- */
                if (target_a != 0) {
                    /* 0x00669e70: cx = [0xae2c92]; [arg2 home] = cx */
                    year = *(volatile u16 *)0x00ae2c92u;
                    home2 = ((u32)(unsigned long)target_a & 0xffff0000u) | year;
                    /* 0x00669e7c: lea ecx, [L+0x08]; inc edx; call 0x549ef0 */
                    res = (u32)cm_fn_00549ef0(&f.buf_b[0], eax_reg, home2 + 1u);
                    /* 0x00669e8d: lea eax, [L+0x00]; call 0x417c10 */
                    cm_fn_00417c10((u32 *)target_a, (u32 *)&f.buf_a[0],
                                   (u16 *)(unsigned long)res);
                }
                if (target_b == 0)
                    goto ret_one;
                /* 0x00669ea5: cx = [0xae2c92]; [arg2 home] = cx */
                year = *(volatile u16 *)0x00ae2c92u;
                home2 = ((u32)(unsigned long)target_a & 0xffff0000u) | year;
                /* 0x00669eb5: lea ecx, [L+0x08]; inc edx; call 0x549ef0 */
                res = (u32)cm_fn_00549ef0(&f.buf_b[0], OBJ32(0x00u), home2 + 1u);
                /* 0x00669ec5: lea ecx, [L+0x00]; call 0x417c10 */
                cm_fn_00417c10((u32 *)target_b, (u32 *)&f.buf_a[0],
                               (u16 *)(unsigned long)res);
                goto ret_one;
            }
        } else {
            /* ---------------- 0x00669dec ---------------- */
            if (target_a != 0) {
                /* 0x00669df4: dx = [0xae2c92]; [arg2 home] = dx */
                year = *(volatile u16 *)0x00ae2c92u;
                home2 = ((u32)(unsigned long)target_a & 0xffff0000u) | year;
                /* 0x00669e06: lea ecx, [L+0x08]; call 0x549ef0 */
                res = (u32)cm_fn_00549ef0(&f.buf_b[0], eax_reg, home2);
                /* 0x00669e0f: lea edx, [L+0x00]; call 0x417c10 */
                cm_fn_00417c10((u32 *)target_a, (u32 *)&f.buf_a[0],
                               (u16 *)(unsigned long)res);
            }
            if (target_b == 0)
                goto ret_zero;
            /* 0x00669e24: ax = [0xae2c92]; [arg2 home] = ax */
            year = *(volatile u16 *)0x00ae2c92u;
            home2 = ((u32)(unsigned long)target_a & 0xffff0000u) | year;
            /* 0x00669e39: lea ecx, [L+0x08]; call 0x549ef0 */
            res = (u32)cm_fn_00549ef0(&f.buf_b[0], OBJ32(0x00u), home2);
            /* 0x00669e43: lea eax, [L+0x00]; call 0x417c10 */
            cm_fn_00417c10((u32 *)target_b, (u32 *)&f.buf_a[0],
                           (u16 *)(unsigned long)res);
            goto ret_zero;
        }
    } else {
        /* 0x00669c9d: cmp dx, ax; jle 0x669d09 */
        if (val_d <= (s16)val_a) {
            /* 0x00669d09: cmp dx, cx; jge 0x669d73 */
            if (val_d >= (s16)val_c) {
                /* ---------------- 0x00669d73 ---------------- */
                if (target_a != 0) {
                    /* 0x00669d77: cx = [0xae2c92]; [arg2 home] = cx */
                    year = *(volatile u16 *)0x00ae2c92u;
                    home2 = ((u32)(unsigned long)target_a & 0xffff0000u) | year;
                    /* 0x00669d83: lea ecx, [L+0x08]; call 0x549ef0 */
                    res = (u32)cm_fn_00549ef0(&f.buf_b[0], eax_reg, home2);
                    /* 0x00669d93: lea eax, [L+0x00]; call 0x417c10 */
                    cm_fn_00417c10((u32 *)target_a, (u32 *)&f.buf_a[0],
                                   (u16 *)(unsigned long)res);
                }
                if (target_b == 0)
                    goto ret_zero;
                /* 0x00669da7: cx = [0xae2c92]; [arg2 home] = cx */
                year = *(volatile u16 *)0x00ae2c92u;
                home2 = ((u32)(unsigned long)target_a & 0xffff0000u) | year;
                /* 0x00669db7: lea ecx, [L+0x08]; call 0x549ef0 */
                res = (u32)cm_fn_00549ef0(&f.buf_b[0], OBJ32(0x00u), home2);
                /* 0x00669dc6: lea ecx, [L+0x00]; call 0x417c10 */
                cm_fn_00417c10((u32 *)target_b, (u32 *)&f.buf_a[0],
                               (u16 *)(unsigned long)res);
                goto ret_zero;
            } else {
                /* ---------------- 0x00669d12 ---------------- */
                if (target_a != 0) {
                    /* 0x00669d16: dx = [0xae2c92]; [arg2 home] = dx */
                    year = *(volatile u16 *)0x00ae2c92u;
                    home2 = ((u32)(unsigned long)target_a & 0xffff0000u) | year;
                    /* 0x00669d26: dec ecx; lea ecx, [L+0x08]; call 0x549ef0 */
                    res = (u32)cm_fn_00549ef0(&f.buf_b[0], eax_reg, home2 - 1u);
                    /* 0x00669d32: lea edx, [L+0x00]; call 0x417c10 */
                    cm_fn_00417c10((u32 *)target_a, (u32 *)&f.buf_a[0],
                                   (u16 *)(unsigned long)res);
                }
                if (target_b == 0)
                    goto ret_one;
                /* 0x00669d4b: ax = [0xae2c92]; [arg2 home] = ax */
                year = *(volatile u16 *)0x00ae2c92u;
                home2 = ((u32)(unsigned long)target_a & 0xffff0000u) | year;
                /* 0x00669d60: lea ecx, [L+0x08]; call 0x549ef0 */
                res = (u32)cm_fn_00549ef0(&f.buf_b[0], OBJ32(0x00u), home2);
                /* 0x00669d6a: lea eax, [L+0x00]; call 0x417c10 */
                cm_fn_00417c10((u32 *)target_b, (u32 *)&f.buf_a[0],
                               (u16 *)(unsigned long)res);
                goto ret_one;
            }
        } else {
            /* ---------------- 0x00669ca2 ---------------- */
            if (target_a != 0) {
                /* 0x00669caa: cx = [0xae2c92]; [arg2 home] = cx */
                year = *(volatile u16 *)0x00ae2c92u;
                home2 = ((u32)(unsigned long)target_a & 0xffff0000u) | year;
                /* 0x00669cb6: lea ecx, [L+0x00]; call 0x549ef0 */
                res = (u32)cm_fn_00549ef0(&f.buf_a[0], eax_reg, home2);
                /* 0x00669cc6: lea eax, [L+0x08]; call 0x417c10 */
                cm_fn_00417c10((u32 *)target_a, (u32 *)&f.buf_b[0],
                               (u16 *)(unsigned long)res);
            }
            if (target_b == 0)
                goto ret_one;
            /* 0x00669cde: cx = [0xae2c92]; [arg2 home] = cx */
            year = *(volatile u16 *)0x00ae2c92u;
            home2 = ((u32)(unsigned long)target_a & 0xffff0000u) | year;
            /* 0x00669cee: lea ecx, [L+0x08]; inc edx; call 0x549ef0 */
            res = (u32)cm_fn_00549ef0(&f.buf_b[0], OBJ32(0x00u), home2 + 1u);
            /* 0x00669cfe: lea ecx, [L+0x00]; call 0x417c10 */
            cm_fn_00417c10((u32 *)target_b, (u32 *)&f.buf_a[0],
                           (u16 *)(unsigned long)res);
            goto ret_one;
        }
    }

ret_zero:
    /* 0x00669dd5: xor eax, eax */
    ret = 0u;
    goto epilogue;
ret_one:
    /* 0x00669f32: mov eax, 1 */
    ret = 1u;
epilogue:
    /* 0x00669dd8 / 0x00669f2c: read the saved SEH predecessor, restore fs:[0] */
    CM_FS(u32, 0) = f.seh.prev;
    return ret;
}

/* fn_007e7ff0 -- plain-C translation of the preferred-image routine at
 * VA 0x007e7ff0 (image base 0x00400000, CM 01/02 3.9.68).
 *
 * Evidence used (all addresses are preferred-image VAs):
 *  - Entry reads ECX before writing it, and the epilogue is `ret 0x14`
 *    (0x007e82dd), so the routine is CM_THISCALL with `self` in ECX plus
 *    five stack words:
 *      arg1 = [ESP+0x04] = the match object (ESI at 0x007e7ff6)
 *      arg2 = [ESP+0x08] = the entry array base (0x007e81cc)
 *      arg3 = [ESP+0x0c] = first-block flag (0x007e8047) / outer step 0
 *      arg4 = [ESP+0x10] = second-block flag (0x007e80f3) / outer step 1
 *      arg5 = [ESP+0x14] = second-block flag (0x007e804b) and the home slot
 *                            reused as the inner loop counter (0x007e81d9,
 *                            0x007e82ae, 0x007e82ba).
 *  - The inner counter is really the caller's fifth-argument home: the
 *    listing writes it byte-wise and reads it back byte-wise, so the C
 *    keeps it in that home slot through a byte pointer.
 *  - The dispatch at 0x007e82ab is `call dword ptr [edx + 8]` with EDX =
 *    the object's first DWORD (its vtable), ECX = the object and two stack
 *    words (EBP pushed last, then the match object).  The stack is balanced
 *    again at 0x007e82ae ([ESP+0x34] still addresses the arg5 home), so the
 *    slot pops its two words (RET 8) and its EAX result is observable in
 *    the root's return value; the slot is therefore typed thiscall with
 *    two words and a u32 result, never void.
 *  - EAX is live at the epilogue: the root only writes AL at the end of the
 *    outer loop and the upper half of EAX survives, so EAX is tracked
 *    explicitly here and returned as u32.
 */
#include "cmre.h"

#ifndef NULL
#define NULL ((void *)0)
#endif

#define G_009CF684 CM_GLOBAL(int, 0x009cf684)
#define G_00AE2364 CM_GLOBAL(int, 0x00ae2364)
#define G_00AE23BC CM_GLOBAL(int, 0x00ae23bc)
#define G_00AE2C74 CM_GLOBAL(u32, 0x00ae2c74)
#define G_00AE28F0 CM_GLOBAL(u32, 0x00ae28f0)
#define G_00AE1050 CM_GLOBAL(u32, 0x00ae1050)

extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 argument);
extern u32 CM_CDECL cm_fn_0053b900(u32 *input);
extern u32 CM_CDECL cm_fn_005409b0(u32 *input);
extern u32 CM_THISCALL cm_fn_00556e90(u32 self, volatile u32 arg1, volatile u32 arg2, volatile u32 arg3);
extern void CM_THISCALL cm_fn_00557120(u32 self, u32 obj);
extern u32 CM_THISCALL cm_fn_0055e300(u32 self, volatile u32 person_arg, volatile u32 other_arg, u32 unused_arg);
extern unsigned long long CM_THISCALL cm_fn_005c6710(void *self, u32 arg);

/* Virtual slot +8 of the object's vtable, reached by the indirect call at
 * 0x007e82ab: ECX = the object, two stack words, callee pops them. */
typedef u32 (CM_THISCALL *cm_vtbl_slot2_fn)(u32 self, u32 arg1, u32 arg2);

u32 CM_THISCALL
cm_fn_007e7ff0(u32 self, u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5)
{
    u32 eax_reg = 0;             /* EAX: live at the epilogue */
    u32 flag = 0;                /* stack slot at L+0x18 (0x007e800f/0x007e803f, read at 0x007e824d) */
    u32 slot3 = 0;               /* stack slot at L+0x1c (0x007e800b/0x007e8027, read at 0x007e8270) */
    u32 edi = 0;                 /* EDI */
    u32 esi = arg1;              /* ESI */
    u32 ebx = 0;                 /* EBX */
    u32 ebp = 0;                 /* EBP */
    u8  outer = 0;               /* stack slot at L+0x0D (0x007e81a1/0x007e82cc) */
    u8 *ctr_home = (u8 *)&arg5;  /* the inner counter is arg5's caller-owned home */
    u32 in;

    /* 0x007e7fff: eax = arg1[5] */
    eax_reg = *(volatile u32 *)(arg1 + 0x14);
    /* 0x007e8013: cm_fn_005c6710(this = *0x00ae2c74, arg1[5]) */
    eax_reg = (u32)cm_fn_005c6710((void *)G_00AE2C74, eax_reg);
    /* 0x007e8018 .. 0x007e8027: stores to L+0x1c, the slot read at 0x007e8270 */
    if (eax_reg != 0) {
        slot3 = 1;
    } else {
        in = *(volatile u32 *)arg1;
        eax_reg = (u32)G_009CF684;
        if (in == eax_reg)
            slot3 = 1;
    }
    /* 0x007e802f .. 0x007e803f: cm_fn_0053b900(arg1[7]) stores to L+0x18 */
    in = *(volatile u32 *)(arg1 + 0x1c);
    eax_reg = cm_fn_0053b900((u32 *)in);
    if (eax_reg != 0)
        flag = 1;

    /* 0x007e8047 / 0x007e804b */
    eax_reg = arg3;
    ebx = arg5;
    /* 0x007e804f / 0x007e8057 (EDI is still zero here) */
    if (arg3 != 0 && ebx != 0) {
        int do_loop;
        /* 0x007e805f .. 0x007e806d */
        in = *(volatile u32 *)(arg1 + 0x1c);
        eax_reg = cm_fn_0053b900((u32 *)in);
        if (eax_reg == 0) {
            do_loop = 1;
        } else {
            /* 0x007e806f .. 0x007e807d */
            in = *(volatile u32 *)(arg1 + 0x1c);
            eax_reg = cm_fn_005409b0((u32 *)in);
            if (eax_reg != 0) {
                do_loop = 1;
            } else {
                /* 0x007e807f .. 0x007e8084: AL only */
                eax_reg = (eax_reg & 0xffffff00u) | *(volatile u8 *)(arg1 + 0x40);
                if ((eax_reg & 0xffu) == 0) {
                    do_loop = 1;
                } else {
                    /* 0x007e8086 .. 0x007e8090 */
                    in = *(volatile u32 *)(arg1 + 0x1c);
                    cm_fn_00557120(G_00AE28F0, in);
                    do_loop = 0;
                }
            }
        }
        if (do_loop) {
            /* 0x007e8097: fifty entries of arg1[7] */
            ebp = 0x32;
            edi = 0;
            while (ebp != 0) {
                u32 person;
                /* 0x007e809c / 0x007e809f */
                eax_reg = *(volatile u32 *)(arg1 + 0x1c);
                person = *(volatile u32 *)(eax_reg + edi + 0xd7);
                esi = person;
                if (person != 0) {
                    /* 0x007e80aa .. 0x007e80b4 */
                    in = *(volatile u32 *)person;
                    eax_reg = in;
                    if ((s32)in < (s32)G_00AE2364) {
                        u32 r = cm_fn_004cf5a0(G_00AE1050, person);
                        int skip;
                        eax_reg = r;
                        skip = 0;
                        /* 0x007e80c6 .. 0x007e80cf */
                        if (r != 0) {
                            if ((*(volatile u8 *)(r + 0x35) & 0x3fu) == 5u)
                                skip = 1;
                        }
                        if (!skip) {
                            /* 0x007e80d1 .. 0x007e80e4: cm_fn_00556e90.
                             * The original pushes ECX after writing only CL
                             * (0x007e80d9); cm_fn_00556e90 reads the low byte
                             * of that word only (0x00556e93), so the upper
                             * bytes of the original ECX are dead here. */
                            u32 id = *(volatile u32 *)person;
                            u32 mode = *(volatile u8 *)(self + 5);
                            eax_reg = cm_fn_00556e90(G_00AE28F0, id, mode, 1);
                        }
                    }
                }
                /* 0x007e80e9: ESI is restored from arg1 */
                esi = arg1;
                /* 0x007e80ed .. 0x007e80f1 */
                edi += 4;
                ebp -= 1;
            }
        }
    }

    /* 0x007e80f3 */
    eax_reg = arg4;
    /* 0x007e80f7 / 0x007e80ff */
    if (arg4 != 0 && ebx != 0) {
        int do_loop;
        /* 0x007e8107 .. 0x007e8115 */
        in = *(volatile u32 *)(arg1 + 0x20);
        eax_reg = cm_fn_0053b900((u32 *)in);
        if (eax_reg == 0) {
            do_loop = 1;
        } else {
            /* 0x007e8117 .. 0x007e8125 */
            in = *(volatile u32 *)(arg1 + 0x20);
            eax_reg = cm_fn_005409b0((u32 *)in);
            if (eax_reg != 0) {
                do_loop = 1;
            } else {
                /* 0x007e8127 .. 0x007e812c: AL only */
                eax_reg = (eax_reg & 0xffffff00u) | *(volatile u8 *)(arg1 + 0x41);
                if ((eax_reg & 0xffu) == 0) {
                    do_loop = 1;
                } else {
                    /* 0x007e812e .. 0x007e8138 */
                    in = *(volatile u32 *)(arg1 + 0x20);
                    cm_fn_00557120(G_00AE28F0, in);
                    do_loop = 0;
                }
            }
        }
        if (do_loop) {
            /* 0x007e813f: fifty entries of arg1[8] */
            ebp = 0x32;
            edi = 0;
            while (ebp != 0) {
                u32 person;
                /* 0x007e8148: ESI restored from arg1 */
                esi = arg1;
                /* 0x007e814c / 0x007e814f */
                eax_reg = *(volatile u32 *)(arg1 + 0x20);
                person = *(volatile u32 *)(eax_reg + edi + 0xd7);
                esi = person;
                if (person != 0) {
                    /* 0x007e815a .. 0x007e8164 */
                    in = *(volatile u32 *)person;
                    eax_reg = in;
                    if ((s32)in < (s32)G_00AE2364) {
                        u32 r = cm_fn_004cf5a0(G_00AE1050, person);
                        int skip;
                        eax_reg = r;
                        skip = 0;
                        /* 0x007e8176 .. 0x007e817f */
                        if (r != 0) {
                            if ((*(volatile u8 *)(r + 0x35) & 0x3fu) == 5u)
                                skip = 1;
                        }
                        if (!skip) {
                            /* 0x007e8181 .. 0x007e8194 */
                            u32 id = *(volatile u32 *)person;
                            u32 mode = *(volatile u8 *)(self + 5);
                            eax_reg = cm_fn_00556e90(G_00AE28F0, id, mode, 1);
                        }
                    }
                }
                /* 0x007e8199 .. 0x007e819d */
                edi += 4;
                ebp -= 1;
            }
        }
    }

    /* 0x007e819f: xor al, al -- only AL is cleared */
    eax_reg &= 0xffffff00u;
    outer = 0;
    for (;;) {
        /* 0x007e81a5 .. 0x007e81c1 */
        if (outer == 0) {
            if (arg3 == 0)
                goto next_outer;
        } else {
            if (arg4 == 0)
                goto next_outer;
        }
        {
            u32 base;
            /* 0x007e81c7: movsx eax, al */
            eax_reg = (u32)(s32)(s8)(u8)outer;
            /* 0x007e81cc / 0x007e81d7: esi = arg2 + outer */
            base = arg2 + eax_reg;
            esi = base;
            /* 0x007e81d5 / 0x007e81d9: the counter is reset in arg5's home */
            *ctr_home = 0;
            /* 0x007e81dd .. 0x007e81e3: edi = esi + ecx*4 + 0x1c2, where
             * ecx = 393*outer, so edi = (arg2 + outer) + 1572*outer + 0x1c2 */
            edi = base + eax_reg * 1572u + 0x1c2u;
            /* 0x007e81ea: twenty entries */
            for (;;) {
                u8 ctr;
                /* 0x007e81ea / 0x007e81ed */
                eax_reg = *(volatile u32 *)(edi - 0x32);
                ebp = edi - 0x32;
                /* 0x007e81f0 / 0x007e81f9 */
                if (eax_reg == 0xffffffffu)
                    goto inner_end;
                if (*(volatile u32 *)(edi - 0x2e) == 0xffffffffu)
                    goto inner_end;
                /* 0x007e8203 .. 0x007e820b */
                if (*ctr_home > 0x0au) {
                    if (*(volatile u8 *)edi == 0xffu)
                        goto inner_end;
                }
                {
                    u32 idx = eax_reg;
                    u32 t;
                    /* 0x007e8211 .. 0x007e8220 */
                    t = idx + idx * 4u;
                    idx = idx + t * 2u;
                    idx = idx + idx * 4u;
                    esi = (u32)G_00AE23BC + idx * 2u;
                    /* 0x007e8223 .. 0x007e8231 */
                    if (esi != 0) {
                        in = *(volatile u32 *)esi;
                        eax_reg = in;
                        if ((s32)in >= (s32)G_00AE2364)
                            goto inner_end;
                    }
                }
                /* 0x007e8233 .. 0x007e824b: cm_fn_0055e300 */
                ebx = self;
                eax_reg = arg1;
                {
                    u32 r = cm_fn_0055e300(G_00AE28F0, ebp, eax_reg, ebx);
                    eax_reg = r;
                    if (r != 0)
                        goto inner_end;
                }
                /* 0x007e824d .. 0x007e826e */
                eax_reg = flag;
                if (eax_reg == 0) {
                    u32 r = cm_fn_004cf5a0(G_00AE1050, esi);
                    eax_reg = r;
                    if (r != 0) {
                        if ((*(volatile u8 *)(r + 0x35) & 0x3fu) == 5u)
                            goto inner_end;
                    }
                }
                /* 0x007e8270 .. 0x007e829f */
                eax_reg = slot3;
                if (eax_reg != 0) {
                    in = *(volatile u32 *)arg1;
                    eax_reg = in;
                    if (in != (u32)G_009CF684)
                        goto inner_end;
                    /* 0x007e8288: AL only */
                    eax_reg = (eax_reg & 0xffffff00u) | *(volatile u8 *)(edi - 5);
                    if ((eax_reg & 0xffu) == 0)
                        goto inner_end;
                    if (*(volatile u8 *)(edi - 6) >= 2u)
                        goto inner_end;
                    if (*(volatile u8 *)(edi - 4) < 0x0au) {
                        if (*(volatile u8 *)(edi - 3) < 0x0au)
                            goto inner_end;
                    }
                }
                /* 0x007e82a1 .. 0x007e82ab: virtual slot +8 */
                eax_reg = arg1;
                {
                    u32 vtbl = *(volatile u32 *)ebx;
                    cm_vtbl_slot2_fn slot = *(cm_vtbl_slot2_fn *)(vtbl + 8);
                    eax_reg = slot(self, ebp, arg1);
                }
            inner_end:
                /* 0x007e82ae .. 0x007e82be */
                ctr = *ctr_home;
                edi += 0x49u;
                ctr = (u8)(ctr + 1u);
                *ctr_home = ctr;
                if (ctr < 0x14u)
                    continue;
                break;
            }
        }
        /* 0x007e82c4: mov al, [ESP+0x13] -- AL only */
        eax_reg = (eax_reg & 0xffffff00u) | outer;
    next_outer:
        /* 0x007e82c8: inc al -- AL only, the upper half of EAX survives */
        outer = (u8)(outer + 1u);
        eax_reg = (eax_reg & 0xffffff00u) | outer;
        /* 0x007e82ca .. 0x007e82d0 */
        if (outer < 2u)
            continue;
        break;
    }

    /* 0x007e82d6 .. 0x007e82dd */
    return eax_reg;
}

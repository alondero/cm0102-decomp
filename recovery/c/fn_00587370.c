/* fn_00587370 -> cm_fn_00587370
   Preferred-image VA 0x00587370, image base 0x00400000.
   Pinned binary: CM0102 3.9.68 cm0102.exe,
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.

   ABI, from the original instructions (all addresses are preferred-image VAs):
   - `ret` at 0x00587517 and 0x005875f3 carries no pop count, so the caller
     cleans: plain cdecl with three argument dwords, no SEH frame.
   - [E+4] object: 0x0058737f `mov esi,dword ptr [esp+0x1a8]`. Only three of the
     four saves are live at that instruction, so the displacement resolves to
     E+4 (L+0x19c). 0x00587389, 0x00587398, 0x005873a3, 0x005873ae, 0x005873bb,
     0x005873c8, 0x005873d3, 0x0058751f, 0x0058752a, 0x00587535, 0x00587540,
     0x0058754d and 0x00587556 each do `mov ecx,esi` before `call 0x785830`:
     object is the thiscall receiver.
   - [E+8] flag: `mov eax,dword ptr [esp+8]` at 0x00587370, tested by
     `test eax,eax` / `je 0x587518` at 0x0058737a / 0x0058738b. Nonzero takes
     the first path, zero takes the second.
   - [E+0xc] target: 0x0058748f `mov ecx,dword ptr [esp+0x1d0]` (esp = L-0x2c:
     four saves plus seven pushed arguments) and 0x0058758d
     `mov ecx,dword ptr [esp+0x1c0]` (esp = L-0x1c), and
     0x005874dd / 0x005874f5 / 0x005875d0 `mov ecx,dword ptr [esp+0x1b4]`
     (esp = L-0x10) all resolve to E+0xc (L+0x1a4). target is the thiscall
     receiver for 0x004ae660 and 0x004ae8a0.

   Frame: 0x00587374 `sub esp,0x198`, then 0x0058737c..0x00587386 save
   ebx/ebp/esi/edi. With L = esp right after the sub:
     [L+0]  index-2 result (store 0x005873b0, reloads 0x005873de and 0x005874aa)
            and, on the flag==0 path, the index-3 result (store 0x00587542,
            reload 0x0058755d)
     [L+4]  index-3 result (store 0x005873bd, reloads 0x005873da and 0x005874a6)
     [L+8]  0x190-byte buffer; the lea at 0x005873e7 (esp+0x20 with esp=L-0x18),
            0x00587579 (esp+0x18 with esp=L-0x10), 0x005874d7, 0x00587564 and
            0x005875d9 all resolve to L+8, and L+8+0x190 is E, the return-slot
            address, so the buffer is the whole tail of the frame.
   Nothing writes those slots before they are read, so nothing is initialised
   here either.

   Tables: 0x00ae23b4 is the base of a 581-byte-element (0x245) table with a
   signed byte field at +0x52 and the name bytes at +0x38; 0x00ae23bc is the
   base of a 110-byte-element (0x6e) table. Both bases are reloaded after the
   intervening calls (0x0058746d vs 0x005874f0 for 0x00ae23b4; 0x005874b3 vs
   0x005874d1 and 0x005875a8 vs 0x005875ca for 0x00ae23bc), so the loads are
   kept volatile and are not cached across a call. */

#include "cmre.h"

/* 0x00785830: ECX = object, one stacked index dword popped by the callee (the
   two paths reach the epilogue with no `add esp` for it). The caller reads only
   EAX of the returned pair, e.g. 0x0058739a `mov ebx,eax`. */
extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);

/* 0x004ae8a0: ECX = target, three stacked dwords popped by the callee (the
   sites 0x005874eb and 0x00587508 reach the epilogue with no `add esp`). */
extern u32 CM_THISCALL cm_fn_004ae8a0(u32 self, u32 buffer, u32 code, u32 entry);

/* 0x004ae660: ECX = target, one stacked dword popped by the callee (pushed at
   0x00587499 and 0x00587597). Its result is unused here. */
extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 arg1);

/* 0x0053e140: cdecl, seven stacked dwords, cleaned by 0x005873fb and
   0x00587576 `add esp,0x1c`. */
extern u32 CM_CDECL cm_fn_0053e140(u32 first, u32 second, u32 third, u32 output,
                                   u32 size, u32 kind, u32 person);

/* 0x0053e990: cdecl, five stacked dwords, cleaned by 0x005874ca and 0x005875c3
   `add esp,0x14`. */
extern s32 CM_CDECL cm_fn_0053e990(u32 person, u32 first, u32 second, u32 third,
                                   u32 date);

/* 0x0066f4e0: caller-cleanup formatting helper. 0x00587496 `add esp,0x1c`
   follows seven pushed dwords and 0x00587594 `add esp,0xc` follows three, so
   the argument count is per call site and the callee must not pop them. */
extern u32 CM_CDECL cm_fn_0066f4e0(u32 destination, u32 format, ...);

void CM_CDECL cm_fn_00587370(u32 object, u32 flag, u32 target)
{
    struct {
        u32 slot0;            /* [L+0] */
        u32 slot4;            /* [L+4] */
        u8 buffer[0x190];     /* [L+8], ends at the return slot */
    } frame;
    u32 r0;
    u32 r1;
    u32 r2;
    u32 r4;
    u32 r6;
    u32 base;
    u32 scaled;
    u32 entry;
    u32 format;
    s32 value;
    s32 result;

    if (flag != 0) {
        /* Indexes 0..6 on object, 0x00587391 .. 0x005873e2. */
        r0 = (u32)cm_fn_00785830((void *)object, 0u);          /* ebx */
        r1 = (u32)cm_fn_00785830((void *)object, 1u);          /* ebp */
        frame.slot0 = (u32)cm_fn_00785830((void *)object, 2u); /* 0x005873b0 */
        frame.slot4 = (u32)cm_fn_00785830((void *)object, 3u); /* 0x005873bd */
        r4 = (u32)cm_fn_00785830((void *)object, 4u);          /* edi, 0x005873ca */
        (void)cm_fn_00785830((void *)object, 5u);
        r6 = (u32)cm_fn_00785830((void *)object, 6u);          /* esi, 0x005873e2 */

        /* Pushes 0x005873e4..0x005873f4 in reverse: r1, [L+0], [L+4], buffer,
           0x190, 1, r0. */
        (void)cm_fn_0053e140(r1, frame.slot0, frame.slot4, (u32)frame.buffer,
                            0x190u, 1u, r0);

        /* `sub eax,0` / `je` / `dec eax` / `je` at 0x005873fe..0x00587404. */
        if (r6 == 0u) {
            format = 0x009c4b04u;
        } else if (r6 == 1u) {
            format = 0x009c4b9cu;
        } else {
            format = 0x009c4c50u;
        }

        /* 0x00587406: base load first, then the 581*index address, then the
           signed byte at +0x52 and the name address at +0x38. */
        base = CM_GLOBAL(volatile u32, 0x00ae23b4);
        scaled = r4 * 581u;
        value = (s32)*(volatile s8 *)(base + scaled + 0x52u);
        (void)cm_fn_0066f4e0(0x00de1f64u, format, (u32)value, (u32)value,
                             0x64u, base + scaled + 0x38u, (u32)frame.buffer);

        /* 0x0058748f reloads target into ECX; 0x00587499 pushes 0x00de1f64. */
        (void)cm_fn_004ae660((void *)target, 0x00de1f64u);

        /* 0x005874a3..0x005874c5: entry = 110*index + [0x00ae23bc]. */
        base = CM_GLOBAL(volatile u32, 0x00ae23bc);
        entry = r0 * 110u + base;
        result = cm_fn_0053e990(entry, r1, frame.slot0, frame.slot4, 0u);
        if (result != 0) {
            base = CM_GLOBAL(volatile u32, 0x00ae23bc);        /* 0x005874d1 */
            entry = r0 * 110u + base;
            (void)cm_fn_004ae8a0(target, (u32)frame.buffer, 0x7d7u, entry);
        }

        /* 0x005874f0 reloads 0x00ae23b4; 0x005874fc..0x00587508 push
           scaled+0x38 as the buffer, 0x7d5, then scaled. */
        base = CM_GLOBAL(volatile u32, 0x00ae23b4);
        scaled = r4 * 581u + base;
        (void)cm_fn_004ae8a0(target, scaled + 0x38u, 0x7d5u, scaled);
        return;
    }

    /* flag == 0, 0x00587518 .. 0x00587558. */
    r0 = (u32)cm_fn_00785830((void *)object, 0u);          /* edi */
    r1 = (u32)cm_fn_00785830((void *)object, 1u);          /* ebx */
    r2 = (u32)cm_fn_00785830((void *)object, 2u);          /* ebp */
    frame.slot0 = (u32)cm_fn_00785830((void *)object, 3u); /* 0x00587542 */
    (void)cm_fn_00785830((void *)object, 4u);
    (void)cm_fn_00785830((void *)object, 5u);
    (void)cm_fn_00785830((void *)object, 6u);

    /* Pushes 0x0058755d..0x00587570 in reverse: r1, r2, [L+0], buffer, 0x190,
       5, r0. */
    (void)cm_fn_0053e140(r1, r2, frame.slot0, (u32)frame.buffer, 0x190u, 5u, r0);

    /* 0x00587579..0x00587588 push buffer, 0x009c4ad4, 0x00de1f64. */
    (void)cm_fn_0066f4e0(0x00de1f64u, 0x009c4ad4u, (u32)frame.buffer);

    /* 0x0058758d reloads target into ECX; 0x00587597 pushes 0x00de1f64. */
    (void)cm_fn_004ae660((void *)target, 0x00de1f64u);

    /* 0x005875a1..0x005875bd: entry = 110*index + [0x00ae23bc]. */
    base = CM_GLOBAL(volatile u32, 0x00ae23bc);
    entry = r0 * 110u + base;
    result = cm_fn_0053e990(entry, r1, r2, frame.slot0, 0u);
    if (result != 0) {
        base = CM_GLOBAL(volatile u32, 0x00ae23bc);            /* 0x005875ca */
        entry = r0 * 110u + base;
        (void)cm_fn_004ae8a0(target, (u32)frame.buffer, 0x7dau, entry);
    }
}

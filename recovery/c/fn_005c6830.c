/* fn_005c6830 - authored reconstruction from CM0102 3.9.68.
   Preferred-image VA 0x005c6830, image base 0x00400000; executable SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.

   The incoming ECX value is a record base; there are no stack arguments.
   A signed 16-bit global selects a table entry at a 768-byte stride. After
   checking that entry and its target, the routine examines indices 0..126
   at a 105-byte record stride. It returns the address of the matching
   nonzero slot, or zero, in the full 32-bit EAX result.

   The complete original instruction listing and prior accepted source are
   retained in ignored local review evidence, not reproduced in this header.
   The behavior and ABI explanation comes from the pinned original
   instructions; it does not claim standalone or native-runtime parity. */
#include "cmre.h"

#define DAT_00b74c26 CM_GLOBAL(u32, 0x00b74c26)
#define DAT_00b77c7a CM_GLOBAL(s16, 0x00b77c7a)

/* ECX is read before written: the incoming record base. No stack argument is
   read, so the single incoming value travels in ECX (fastcall/thiscall form). */
int CM_FASTCALL cm_fn_005c6830(int param_1)
{
    s32 table_byte;      /* eax after the sign-extended global and the scaling */
    u32 entry;           /* eax after the table DWORD load */
    u32 target;          /* edi: the id read from entry + 0x39 */
    u32 base;            /* ecx */
    s32 i;               /* esi */
    u32 *rec;            /* edx inside the loop */
    u32 slot;            /* eax after the load at edx + 0xb1 */

    table_byte = (s32)DAT_00b77c7a;   /* 005c6830 movsx eax, word ptr [0xb77c7a] */
    table_byte = table_byte * 3;      /* 005c6839 lea eax, [eax + eax*2] */
    table_byte = table_byte * 256;    /* 005c683c shl eax, 8 */

    /* 005c683f mov eax, dword ptr [eax + 0xb74c26]: the DWORD load sits at
       0xb74c26 + the scaled byte offset, with 32-bit wrapping address math. */
    entry = *(u32 *)(u32)((u32)&DAT_00b74c26 + (u32)table_byte);
    if (entry == 0) {                 /* 005c6845 test eax, eax / 005c6847 je */
        return 0;                    /* 005c6886 xor eax, eax */
    }

    target = *(u32 *)(entry + 0x39);  /* 005c6849 mov edi, dword ptr [eax + 0x39] */
    if (target == 0) {               /* 005c684c test edi, edi / 005c684e je */
        return 0;
    }

    base = (u32)param_1;

    for (i = 0; i < 0x7f; i++) {      /* 005c6850 xor esi, esi .. 005c687f cmp si,0x7f */
        /* 005c6852..005c6864: edx = ecx + (21*i)*4 + 21*i = ecx + 105*i */
        rec = (u32 *)(base + (u32)(105 * (u32)i));

        slot = *(u32 *)((u32)rec + 0xb1);   /* 005c6866 mov eax, dword ptr [edx + 0xb1] */
        if (slot == 0) {                    /* 005c686c test eax, eax / 005c686e je */
            continue;                       /* 005c687e inc esi */
        }
        if (slot != target) {               /* 005c6870 cmp eax, edi / 005c6872 jne */
            continue;                       /* 005c687e inc esi */
        }
        if (*(u32 *)((u32)rec + 0xda) == 0) {
            continue;                       /* 005c687a test eax,eax / 005c687c jne */
        }

        /* 005c688a..005c689e: eax = ecx + 105*i + 0xb1 */
        return (int)(base + (u32)(105 * (u32)i) + 0xb1);
    }

    return 0;                    /* 005c6885..005c6889 */
}

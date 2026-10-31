#include "cmre.h"

/* Original table entries are fixed-text globals compared as prefixes with the
   C runtime compare (cdecl, three stack arguments, caller cleans up). Each entry
   tests in listing order; a zero compare result returns that entry's code. The
   final entry returns 0x4c on a zero compare and 0x4b otherwise (neg/sbb/add). */
#define TEXT_AT(va) (&CM_GLOBAL(char, (va)))

extern int CM_CDECL cm_fn_00946fa0(char *_Str1, char *_Str2, u32 _MaxCount);

u8 CM_CDECL cm_fn_007a32e0(char *arg)
{
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b840), 4u) == 0) return 0x00u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b83c), 3u) == 0) return 0x01u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b834), 5u) == 0) return 0x02u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b824), 10u) == 0) return 0x03u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b80c), 10u) == 0) return 0x04u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b7fc), 10u) == 0) return 0x05u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b7ec), 10u) == 0) return 0x06u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b7dc), 10u) == 0) return 0x07u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b7d0), 10u) == 0) return 0x08u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b7c0), 10u) == 0) return 0x09u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b7b4), 10u) == 0) return 0x0au;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b7a0), 15u) == 0) return 0x0bu;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b788), 15u) == 0) return 0x0cu;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b778), 10u) == 0) return 0x50u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b768), 10u) == 0) return 0x0du;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b758), 10u) == 0) return 0x0eu;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b74c), 10u) == 0) return 0x0fu;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b73c), 10u) == 0) return 0x10u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b730), 10u) == 0) return 0x11u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b71c), 10u) == 0) return 0x12u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b70c), 10u) == 0) return 0x13u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b6fc), 10u) == 0) return 0x14u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b6ec), 10u) == 0) return 0x15u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b6dc), 10u) == 0) return 0x16u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b6c8), 10u) == 0) return 0x17u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b6b4), 10u) == 0) return 0x18u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b6a0), 10u) == 0) return 0x19u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b690), 10u) == 0) return 0x1au;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b678), 10u) == 0) return 0x1bu;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b664), 10u) == 0) return 0x1cu;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b654), 10u) == 0) return 0x1du;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b644), 10u) == 0) return 0x1eu;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b634), 10u) == 0) return 0x1fu;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b628), 10u) == 0) return 0x20u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b618), 10u) == 0) return 0x21u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b608), 10u) == 0) return 0x22u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b5f8), 10u) == 0) return 0x23u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b5e4), 10u) == 0) return 0x24u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b5d8), 5u) == 0) return 0x25u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b5c8), 10u) == 0) return 0x26u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b5bc), 10u) == 0) return 0x27u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b5ac), 10u) == 0) return 0x28u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b5a0), 10u) == 0) return 0x29u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b590), 14u) == 0) return 0x2au;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b584), 10u) == 0) return 0x2bu;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b574), 10u) == 0) return 0x2cu;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b564), 10u) == 0) return 0x2du;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b554), 10u) == 0) return 0x2eu;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b548), 5u) == 0) return 0x2fu;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b538), 10u) == 0) return 0x30u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b52c), 10u) == 0) return 0x31u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b51c), 10u) == 0) return 0x32u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b50c), 10u) == 0) return 0x34u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b4fc), 15u) == 0) return 0x35u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b4f0), 5u) == 0) return 0x36u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b4e0), 13u) == 0) return 0x37u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b4d0), 10u) == 0) return 0x38u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b4c4), 10u) == 0) return 0x39u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b4b0), 10u) == 0) return 0x3au;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b49c), 10u) == 0) return 0x3bu;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b48c), 10u) == 0) return 0x3cu;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b47c), 10u) == 0) return 0x3du;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b46c), 10u) == 0) return 0x3eu;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b458), 10u) == 0) return 0x3fu;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b448), 10u) == 0) return 0x40u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b438), 10u) == 0) return 0x41u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b42c), 5u) == 0) return 0x42u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b420), 10u) == 0) return 0x43u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b414), 5u) == 0) return 0x44u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b408), 5u) == 0) return 0x45u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b3f8), 13u) == 0) return 0x46u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b3e8), 10u) == 0) return 0x47u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b3d4), 19u) == 0) return 0x48u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b3c0), 10u) == 0) return 0x49u;
    if (cm_fn_00946fa0(arg, TEXT_AT(0x00a6b3b4), 10u) == 0) return 0x4au;
    {
        /* neg eax sets CF when the compare result is nonzero; sbb al, al turns
           CF into 0x00 or 0xff in the low byte; add eax, 0x4c follows. */
        int last = cm_fn_00946fa0(arg, TEXT_AT(0x00a6b3a8), 8u);
        u8 low = (last != 0) ? 0xffu : 0x00u;
        return (u8)(low + 0x4cu);
    }
}

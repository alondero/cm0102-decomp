/* Preferred-image VA 0x005e81e0, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: no arguments, plain ret, so the convention is not visible from the
   callee side (CM_CDECL is used). The dword selector at 0x009CD804 is compared
   with 0, 1 and 2 in that order; any other value returns at once. Each case calls
   the USER32 slot at 0x009671C8 (GetCursor per the brief) with no arguments, compares
   the result with one global cursor (0x00AEBE24, 0x00AEBE1C or 0x00AEBE20), and when
   they differ calls the slot at 0x009671C0 with 0, the slot at 0x009671BC with the
   cursor global re-read after that call, and the slot at 0x009671C0 with 1. The
   slot at 0x009671C0 is named ShowCursor only by the Ghidra draft; the brief gives
   it no name, so it stays an address-named import boundary. Import slots are read
   through their IAT addresses at each call, as the listing does. Every callee pops
   its own stack arguments (no add esp), so each is modelled as CM_STDCALL. */
#include "cmre.h"

#define CM_FN_005E81E0_SLOT_GET_CURSOR 0x009671c8
#define CM_FN_005E81E0_SLOT_UNNAMED_C0 0x009671c0
#define CM_FN_005E81E0_SLOT_SET_CURSOR 0x009671bc

typedef u32 (CM_STDCALL *cm_fn_005e81e0_get_cursor_t)(void);
typedef s32 (CM_STDCALL *cm_fn_005e81e0_unnamed_c0_t)(s32 arg1);
typedef u32 (CM_STDCALL *cm_fn_005e81e0_set_cursor_t)(u32 arg1);

void CM_CDECL cm_fn_005e81e0(void)
{
    s32 selector;
    u32 cursor;

    selector = CM_GLOBAL(s32, 0x009cd804);
    if (selector == 0) {
        cursor = (CM_GLOBAL(cm_fn_005e81e0_get_cursor_t, CM_FN_005E81E0_SLOT_GET_CURSOR))();
        if (cursor != CM_GLOBAL(u32, 0x00aebe24)) {
            (CM_GLOBAL(cm_fn_005e81e0_unnamed_c0_t, CM_FN_005E81E0_SLOT_UNNAMED_C0))(0);
            (CM_GLOBAL(cm_fn_005e81e0_set_cursor_t, CM_FN_005E81E0_SLOT_SET_CURSOR))(
                CM_GLOBAL(u32, 0x00aebe24));
            (CM_GLOBAL(cm_fn_005e81e0_unnamed_c0_t, CM_FN_005E81E0_SLOT_UNNAMED_C0))(1);
        }
    } else if (selector == 1) {
        cursor = (CM_GLOBAL(cm_fn_005e81e0_get_cursor_t, CM_FN_005E81E0_SLOT_GET_CURSOR))();
        if (cursor != CM_GLOBAL(u32, 0x00aebe1c)) {
            (CM_GLOBAL(cm_fn_005e81e0_unnamed_c0_t, CM_FN_005E81E0_SLOT_UNNAMED_C0))(0);
            (CM_GLOBAL(cm_fn_005e81e0_set_cursor_t, CM_FN_005E81E0_SLOT_SET_CURSOR))(
                CM_GLOBAL(u32, 0x00aebe1c));
            (CM_GLOBAL(cm_fn_005e81e0_unnamed_c0_t, CM_FN_005E81E0_SLOT_UNNAMED_C0))(1);
        }
    } else if (selector == 2) {
        cursor = (CM_GLOBAL(cm_fn_005e81e0_get_cursor_t, CM_FN_005E81E0_SLOT_GET_CURSOR))();
        if (cursor != CM_GLOBAL(u32, 0x00aebe20)) {
            (CM_GLOBAL(cm_fn_005e81e0_unnamed_c0_t, CM_FN_005E81E0_SLOT_UNNAMED_C0))(0);
            (CM_GLOBAL(cm_fn_005e81e0_set_cursor_t, CM_FN_005E81E0_SLOT_SET_CURSOR))(
                CM_GLOBAL(u32, 0x00aebe20));
            (CM_GLOBAL(cm_fn_005e81e0_unnamed_c0_t, CM_FN_005E81E0_SLOT_UNNAMED_C0))(1);
        }
    }
}

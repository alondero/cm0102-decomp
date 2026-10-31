/* Preferred-image VA 0x0094e497, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The original lookup, callbacks and filter import remain boundaries.
   regparm(3) carries the EAX/EDX/ECX established at each callback site;
   the one or two signal arguments remain caller-cleaned stack arguments. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_CDECL cm_fn_0094e5d8(u32 code);
typedef void (__attribute__((cdecl, regparm(3))) *signal_one_94e497)(
    u32 eax_value, u32 edx_value, u32 ecx_value, u32 signal);
typedef void (__attribute__((cdecl, regparm(3))) *signal_two_94e497)(
    u32 eax_value, u32 edx_value, u32 ecx_value, u32 signal, u32 detail);
typedef u32 (CM_STDCALL *filter_94e497)(u32 pointers);

u32 CM_CDECL cm_fn_0094e497(volatile u32 code, u32 pointers)
{
    u32 input_code = code;
    u32 record = cm_fn_0094e5d8(input_code);
    u32 handler, signal;
    if (record == 0) goto fallback;
    handler = D(record + 8u);
    if (handler == 0) goto fallback;
    if (handler == 5) {
        u32 old_handler = D(record + 8u);
        D(record + 8u) = old_handler & 0u;
        return 1;
    }
    if (handler == 1) return 0xffffffffu;

    code = D(0x00de3148);
    D(0x00de3148) = pointers;
    signal = D(record + 4u);
    if (signal == 8) {
        u32 start = D(0x00ad95c8);
        u32 end = D(0x00ad95cc) + start;
        u32 remaining = end;
        u32 exception, saved_detail, detail;
        if ((s32)start < (s32)end) {
            u32 slot = 0x00ad9558u + start * 12u;
            remaining = end - start;
            do {
                u32 old_handler = D(slot);
                D(slot) = old_handler & 0u;
                slot += 12u;
                --remaining;
            } while (remaining != 0);
        }
        exception = D(record);
        saved_detail = D(0x00ad95d4);
        switch (exception) {
        case 0xc000008eu: D(0x00ad95d4) = 0x83; break;
        case 0xc0000090u: D(0x00ad95d4) = 0x81; break;
        case 0xc0000091u: D(0x00ad95d4) = 0x84; break;
        case 0xc0000093u: D(0x00ad95d4) = 0x85; break;
        case 0xc000008du: D(0x00ad95d4) = 0x82; break;
        case 0xc000008fu: D(0x00ad95d4) = 0x86; break;
        case 0xc0000092u: D(0x00ad95d4) = 0x8a; break;
        default: break;
        }
        detail = D(0x00ad95d4);
        ((signal_two_94e497)handler)(exception, remaining, start, 8, detail);
        D(0x00ad95d4) = saved_detail;
    } else {
        u32 old_handler = D(record + 8u);
        D(record + 8u) = old_handler & 0u;
        ((signal_one_94e497)handler)(record, input_code, signal, signal);
    }
    D(0x00de3148) = code;
    return 0xffffffffu;

fallback:
    return ((filter_94e497)D(0x00967090))(pointers);
}

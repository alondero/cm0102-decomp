/* Preferred-image VA 0x006ad930, base 0x00400000, CM 01/02 3.9.68.
   Target SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Six stack words, RET 0x18, no entry ECX dependency. Both indexed callers in
   fn_006ad610 discard the result. Field meanings remain unknown.
   Offline candidate: differential and computation-sensitivity checks pending. */
#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

void CM_STDCALL cm_fn_006ad930(u32 first, u32 second,
                              s8 third, s8 fourth, s8 fifth, s8 sixth)
{
    u32 previous = CM_FS(u32, 0);
    volatile struct { u32 prev, handler, state; } frame;
    /* L is the original ESP immediately after SUB ESP,0x2e4.
       L+0 is the FILD temporary, L+4 the message, L+e4 and L+1e4 the
       diagnostic buffers. Their adjacency and uninitialized bytes are kept. */
    u8 scratch[0x2e4];
    void *message = scratch + 4;
    u32 value, pointer;
    s8 grade;
    s16 word, threshold;

    frame.state = 0xffffffffu;
    frame.handler = 0x0095d10bu;
    frame.prev = previous;
    CM_FS(u32, 0) = (u32)&frame;

    if (first == 0 || second == 0) {
        u32 text;
        cm_fn_00944cff(0x009f18a4u, 0, 0, scratch + 0x1e4,
                      scratch + 0xe4);
        /* 006adb19..1f push arguments for the later formatter. The original
           0090d130 is MOV EAX,00ad4394 / RET and takes no arguments. */
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text,
                      scratch + 0x1e4, scratch + 0xe4, 0x5cd2u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
    } else {
        cm_fn_0077bbe0(message, 0x178fu, 0);
        value = *(volatile u32 *)first;
        frame.state = 0;
        cm_fn_00785790(message, 0, value);
        pointer = *(volatile u32 *)(first + 4u);
        value = *(volatile u32 *)(pointer + 0x33u);
        cm_fn_00785790(message, 1, value);
        pointer = *(volatile u32 *)(first + 8u);
        value = *(volatile u32 *)(pointer + 0x33u);
        cm_fn_00785790(message, 2, value);
        pointer = *(volatile u32 *)(first + 0xcu);
        value = *(volatile u32 *)(pointer + 0x33u);
        cm_fn_00785790(message, 3, value);
        value = *(volatile u32 *)second;
        cm_fn_00785790(message, 4, value);
        cm_fn_00785790(message, 5, (u32)(s32)third);
        cm_fn_00785790(message, 6, (u32)(s32)fourth);
        cm_fn_00785790(message, 7, (u32)(s32)fifth);
        cm_fn_00785790(message, 8, (u32)(s32)sixth);

        pointer = *(volatile u32 *)(second + 0x53u);
        grade = *(volatile s8 *)(pointer + 0x85u);
        word = *(volatile s16 *)(second + 0x80u);
        /* MOV CX retains unspecified upper ECX, but only resulting AX and
           signed CX are consumed. Unsigned arithmetic preserves low-word
           wrap without inventing the unobserved upper half. */
        if (grade > 14)
            threshold = (s16)((u32)(u16)word * 3u + 0x1388u);
        else if (grade > 8)
            threshold = (s16)((u32)(u16)word * 2u + 0x9c4u);
        else
            threshold = (s16)((u32)(u16)word * 2u + 0x4e2u);
        if (word < 0x109a && threshold > 0x1388)
            threshold = 0x1388;
        if (*(volatile u8 *)(second + 0x64u) == 3) {
            long double converted;
            *(volatile s32 *)scratch = (s32)threshold;
            converted = (long double)*(volatile s32 *)scratch;
            converted *= (long double)CM_GLOBAL(volatile double, 0x00968968);
            /* Original CALL 0x006ada86 -> __ftol at 0x00945640, ADR 0006.
               FILD32, FMUL m64, 64-bit truncation; original uses only AX. */
            threshold = (s16)(u32)(long long)converted;
        }
        pointer = *(volatile u32 *)(second + 0x69u);
        value = *(volatile s32 *)(pointer + 0x3cu) < (s32)threshold;
        cm_fn_00785790(message, 9, value);

        word = *(volatile s16 *)(second + 0x80u);
        value = 0;
        if (word > 0x109a && *(volatile s8 *)(second + 0x7fu) < 10)
            value = 1;
        else if (word > 0x1676 && *(volatile s8 *)(second + 0x7fu) < 14)
            value = 1;
        cm_fn_00785790(message, 10, value);
        pointer = CM_GLOBAL(volatile u32, 0x00b7431c);
        cm_fn_007861e0((void *)pointer, (u32)message, second);
    }
    previous = frame.prev;
    CM_FS(u32, 0) = previous;
}

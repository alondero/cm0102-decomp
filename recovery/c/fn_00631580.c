#include "cmre.h"
/* Whole-source independent static review complete; actual gates pending. */

/* Preferred-image VAs, base 00400000, original 3.9.68 SHA-256:
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Static authorship only. All callees remain original boundaries. */
extern u32 CM_THISCALL cm_fn_00785830(void *self, u32 index);
extern u32 CM_THISCALL cm_fn_00549ef0(void *self, u32 day, u32 year);
extern u32 CM_CDECL cm_fn_0053e140(u32 first, u32 second, u32 third,
    u32 output, u32 capacity, u32 kind, u32 person);
extern u32 CM_CDECL cm_fn_0053e990(u32 person, u32 first, u32 second,
    u32 third, u32 date);
extern u32 CM_CDECL cm_fn_0066f4e0(u32 output, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944c9f(u32 output, u32 format, ...);
extern u32 CM_THISCALL cm_fn_004ae660(void *self, u32 text);
extern u32 CM_THISCALL cm_fn_004ae770(void *self, u32 text);
extern u32 CM_THISCALL cm_fn_004ae8a0(void *self, u32 text, u32 kind, u32 person);
extern u32 CM_STDCALL cm_fn_0062ef30(u32 injury, u32 output, s32 capacity, u32 mode);
extern u32 CM_STDCALL cm_fn_006306e0(u32 injury);
extern u32 CM_STDCALL cm_fn_00630c50(u32 type, u32 output, u32 capacity);
extern u32 CM_CDECL cm_fn_007e5780(s32 duration, void *output, s32 capacity, u32 mode);

/* L is ESP immediately after SUB ESP,1c4. Bytes not explicitly stored below
   stay uninitialized, including selector DWORD's high three bytes. All local
   buffers and scalars share this one original 1c4-byte allocation. */
#define W(o) (scratch.words[(o) / 4])
#define B(o) (scratch.bytes[(o)])
#define P(o) ((u32)(scratch.bytes + (o)))
#define FORMAT(f) ((void)cm_fn_0066f4e0(0x00de1f64u, (f), P(0x24)))

u32 CM_THISCALL cm_fn_00631580(u32 self, u32 record, u32 detailed, u32 destination)
{
    volatile struct { u32 prev, handler, state; } seh;
    union { volatile u32 words[0x71]; volatile u8 bytes[0x1c4]; } scratch;
    u32 person, first, second, third, year, day, outcome, result;
    u32 format, person_offset, person_address, receiver, type_word;
    u8 selector, type;
    s32 grammar, duration;

    /* 00631580..00631595: linked original SEH predecessor/handler/state. */
    seh.state = 0xffffffffu;
    seh.handler = 0x0095b1ebu;
    seh.prev = CM_FS(volatile u32, 0);
    CM_FS(volatile u32, 0) = (u32)&seh;
    W(0x10) = self;                         /* 006315a5 */
    B(0x0b) = (u8)cm_fn_00785830((void *)record, 0);
    selector = (u8)cm_fn_00785830((void *)record, 1);
    B(0x0c) = selector;                     /* 006315c6: byte only */
    person = cm_fn_00785830((void *)record, 2);
    first = cm_fn_00785830((void *)record, 3);
    W(0x04) = cm_fn_00785830((void *)record, 4);
    W(0x00) = cm_fn_00785830((void *)record, 5);
    year = cm_fn_00785830((void *)record, 7);
    day = cm_fn_00785830((void *)record, 6);
    (void)cm_fn_00549ef0((void *)P(0x1c), day, year);
    seh.state = 0;                         /* 00631617 */
    W(0x18) = cm_fn_00785830((void *)record, 8);
    W(0x14) = cm_fn_00785830((void *)record, 9);
    outcome = cm_fn_00785830((void *)record, 10);

    if (B(0x0b) != 0)
        goto full_training;

    if (detailed == 0) {
        /* 0063165f..00631676: second is retained in native EBX across
           formatter and assignment; unlike the other three routes it
           must not be reloaded for the person check. */
        third = W(0x00);
        second = W(0x04);
        (void)cm_fn_0053e140(first, second, third, P(0x24), 0x99, 5, person);
        FORMAT(0x009e9bb8u);
        (void)cm_fn_004ae660((void *)destination, 0x00de1f64u);
        third = W(0x00);
        person_offset = person * 110u;
        person_address = CM_GLOBAL(volatile u32, 0x00ae23bc) + person_offset;
        result = cm_fn_0053e990(person_address, first, second, third, P(0x1c));
        if (result == 0)
            goto finished;
        person_address = CM_GLOBAL(volatile u32, 0x00ae23bc) + person_offset;
        goto annotate;
    }

    third = W(0x00);
    second = W(0x04);
    (void)cm_fn_0053e140(first, second, third, P(0x24), 0x99, 1, person);
    type = CM_GLOBAL(volatile u8, 0x009e7ad9u + (u32)(s32)(s8)selector * 13u);
    if (type == 12) {
        if (selector == 0x51) FORMAT(0x009e9b50u);
        else if (selector == 0x50) FORMAT(0x009e9a54u);
        else if (selector == 0x4d) FORMAT(0x009e99dcu);
        else if (selector == 0x4e) FORMAT(0x009e9954u);
        else if (selector == 0x4f) FORMAT(0x009e989cu);
        else (void)cm_fn_00944c9f(0x00de1f64u, 0x009e9838u, P(0x24));
    } else if (type == 0) {
        if (selector == 0) FORMAT(0x009e97e4u);
        else if (selector == 1) FORMAT(0x009e9788u);
        else if (selector == 2) FORMAT(0x009e9728u);
        else FORMAT(0x009e96c0u);
    } else {
        if (type != 6) {
            /* 0063186d..00631884: byte store, then full DWORD reload.
               High bytes are the original uninitialized scratch bytes.
               Native ECX is saved self; callee's needed inputs are stack. */
            B(0x0c) = type;
            type_word = W(0x0c);
            receiver = W(0x10);
            (void)receiver;
            grammar = (s8)cm_fn_00630c50(type_word, P(0xc0), 0x32);
        } else {
            /* 0063188b..006318aa: retain full injury DWORD across calls. */
            second = W(0x0c);
            receiver = W(0x10);
            (void)receiver; /* ECX is unused by this stdcall boundary. */
            (void)cm_fn_0062ef30(second, P(0xc0), 0x32, 0);
            grammar = (s8)cm_fn_006306e0(second);
        }
        (void)cm_fn_0066f4e0(0x00de1f64u, 0x009e9648u,
            grammar, grammar, P(0x24), P(0xc0));
    }
    /* 006318d3: receiver is cached across both formatter and append. */
    receiver = destination;
    (void)cm_fn_004ae660((void *)receiver, 0x00de1f64u);
    duration = (s16)*(volatile u16 *)(scratch.bytes + 0x18);
    (void)cm_fn_007e5780(duration, (void *)P(0xf4), 0x32, 1);
    (void)cm_fn_0066f4e0(0x00de1f64u, 0x009e95f0u, P(0xf4));
    (void)cm_fn_004ae770((void *)receiver, 0x00de1f64u);
    third = W(0x00);
    second = W(0x04);
    goto check_person;

full_training:
    third = W(0x00);
    second = W(0x04);
    if (detailed == 0) {
        (void)cm_fn_0053e140(first, second, third, P(0x24), 0x99, 5, person);
        if (selector == 0x51 && outcome == 3)
            format = 0x009e95a4u;
        else
            format = 0x009e9568u;
        FORMAT(format);
        receiver = destination;
        (void)cm_fn_004ae660((void *)receiver, 0x00de1f64u);
        third = W(0x00);
        second = W(0x04);
        goto check_person;
    }

    (void)cm_fn_0053e140(first, second, third, P(0x24), 0x99, 1, person);
    third = W(0x00);
    second = W(0x04);
    (void)cm_fn_0053e140(first, second, third, P(0x128), 0x99, 5, person);
    type = CM_GLOBAL(volatile u8, 0x009e7ad9u + (u32)(s32)(s8)selector * 13u);
    if (type == 12) {
        if (selector == 0x51) {
            if (outcome == 2) format = 0x009e94b4u;
            else if (outcome == 3) format = 0x009e9460u;
            else format = 0x009e9b50u;
        } else if (selector == 0x4d) {
            if (outcome == 0xffffffffu) format = 0x009e9344u;
            else if (outcome == 1) format = 0x009e921cu;
            else if (W(0x14) == 0) format = 0x009e9150u;
            else format = 0x009e9094u;
        } else if (selector == 0x50) {
            format = 0x009e8f9cu;
        } else if (selector == 0x4e) {
            if (W(0x14) == 0) {
                format = 0x009e8f1cu;
            } else {
                (void)cm_fn_0066f4e0(0x00de1f64u, 0x009e8e1cu, P(0x24), P(0x128));
                goto full_formatted;
            }
        } else if (selector == 0x4f) {
            if (W(0x14) == 2) {
                if (outcome == 1) format = 0x009e8d4cu;
                else format = 0x009e8ce4u;
            } else format = 0x009e8c1cu;
        } else format = 0x009e8b98u;
        FORMAT(format);
    } else if (type == 0) {
        if (selector == 0) FORMAT(0x009e8b48u);
        else if (selector == 1) FORMAT(0x009e8af0u);
        else if (selector == 2) FORMAT(0x009e8a94u);
        else FORMAT(0x009e8a30u);
    } else {
        if (type != 6) {
            /* 00631cdd..00631cf4: different overwrite site, L+18.
               This DWORD was initialized by getter(8), unlike L+0c. */
            B(0x18) = type;
            type_word = W(0x18);
            receiver = W(0x10);
            (void)receiver;
            grammar = (s8)cm_fn_00630c50(type_word, P(0xc0), 0x32);
        } else {
            second = W(0x0c);
            receiver = W(0x10);
            (void)receiver;
            (void)cm_fn_0062ef30(second, P(0xc0), 0x32, 0);
            grammar = (s8)cm_fn_006306e0(second);
        }
        (void)cm_fn_0066f4e0(0x00de1f64u, 0x009e89bcu,
            grammar, grammar, P(0x24), P(0xc0));
    }
full_formatted:
    receiver = destination;
    (void)cm_fn_004ae660((void *)receiver, 0x00de1f64u);
    third = W(0x00);
    second = W(0x04);
check_person:
    /* 00631926/006319dc/00631d56: 32-bit unsigned LEA/SHL wrap.
       0053e990 may mutate the global; preserve the subsequent reload. */
    person_offset = person * 110u;
    person_address = CM_GLOBAL(volatile u32, 0x00ae23bc) + person_offset;
    result = cm_fn_0053e990(person_address, first, second, third, P(0x1c));
    if (result == 0)
        goto finished;
    person_address = CM_GLOBAL(volatile u32, 0x00ae23bc) + person_offset;
annotate:
    result = cm_fn_004ae8a0((void *)destination, P(0x24), 0x7d7, person_address);
finished:
    /* 00631da0..00631db8: leave the final callee's entire EAX intact. */
    CM_FS(volatile u32, 0) = seh.prev;
    return result;
}
#undef FORMAT
#undef P
#undef B
#undef W

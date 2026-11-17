#include "cmre.h"
/* Original instruction evidence: preferred-image VAs 008c8fd0-008c91d3,
   CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   ret 14 => five stack arguments, stdcall. Receiver setup at 008c8fde,
   008c907b, 008c9091 and 008c91af supplies the thiscall boundaries below.
   Cdecl allocation/free calls have explicit add esp,4 at 008c9068/008c91bc. */
extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 argument);
extern u32 CM_THISCALL cm_fn_008e5680(u32 self);
extern u32 CM_THISCALL cm_fn_008e78f0(u32 self,u32 person,u32 club,u32 value);
extern void CM_THISCALL cm_fn_008e5710(u32 self);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *p);
#define M32(a) CM_GLOBAL(volatile u32,(a))
#define I32(a) CM_GLOBAL(volatile s32,(a))
#define I16(a) CM_GLOBAL(volatile s16,(a))
#define I8(a) CM_GLOBAL(volatile s8,(a))
#define M8(a) CM_GLOBAL(volatile u8,(a))

u32 CM_STDCALL cm_fn_008c8fd0(u32 club, volatile u32 person_home,
                             u32 out, u32 status, u32 value)
{
    u32 prev = CM_FS(u32,0);
    volatile struct { u32 prev, handler, state; } frame;
    u32 receiver, person, p, index, base, obj, result = 0;
    s32 other, own, delta;
    s16 cx;
    long double t;
    frame.state = 0xffffffffu;
    frame.handler = 0x00963e2b;
    frame.prev = prev;
    receiver = M32(0x00ae1050);
    CM_FS(u32,0) = (u32)&frame;
    person = person_home;
    p = cm_fn_004cf5a0(receiver,person);
    M32(0x00ae1080) = p;
    if (p) {
        index = M32(p+4);
        base = M32(0x00ae23b4);
        p = base + index * 0x245u;
    }
    other = I16(p+0x80);
    own = I16(club+0x80);
    if (own-other <= 3) goto done;
    p = M32(person+0x61);
    if (!p) goto done;
    delta = I16(p+7);
    delta -= I16(p+5);
    if (delta < 25) goto done;
    if (I8(person+0x18) > 30) goto done;
    obj = (u32)cm_fn_00944cf1(0x41);
    person_home = obj;
    frame.state = 0;
    if (obj) obj = cm_fn_008e5680(obj);
    frame.state = 0xffffffffu;
    M32(out) = obj;
    cm_fn_008e78f0(obj,person,club,value);
    M8(status) = 0x32;
    cx = I16(club+0x80);
    if (cx < 15) {
        p = M32(person+0x61);
        delta = I16(p+0xb) / 500;
        delta -= cx;
        if (delta > 2) {
            obj = M32(out);
            t = (long double)I32(obj+0x1b);
            t *= (long double)CM_GLOBAL(double,0x009689b0);
            /* __ftol at preferred-image VA 008c90e4. */
            M32(obj+0x1b) = (u32)(long long)t;
            p = M32(out);
            M8(p+0x3f) = 0x14;
            M8(status) = 0x50;
            result = 1;
            goto done;
        }
    }
    p = M32(person+0x1a);
    if (p && I16(p+0x8e) > 5000 && M8(person+0x22) == 0) {
        obj = M32(out);
        t = (long double)I32(obj+0x1b);
        t *= (long double)CM_GLOBAL(double,0x00968e80);
        /* __ftol at preferred-image VA 008c9124. */
        M32(obj+0x3a) = (u32)(long long)t;
        p = M32(out);
        M8(p+0x3e) = 10;
        obj = M32(out);
        t = (long double)I32(obj+0x1b);
        t *= (long double)CM_GLOBAL(double,0x009689b0);
        /* __ftol at preferred-image VA 008c913d. */
        M32(obj+0x1b) = (u32)(long long)t;
        M8(status) = 0x46;
        result = 1;
        goto done;
    }
    p = M32(person+0x61);
    delta = I16(p+0xb) / 500;
    delta -= cx;
    if (delta < -3) {
        obj = M32(out);
        t = (long double)I32(obj+0x1b);
        t *= (long double)CM_GLOBAL(double,0x00968e80);
        /* __ftol at preferred-image VA 008c917d. */
        M32(obj+0x35) = (u32)(long long)t;
        p = M32(out);
        M8(p+0x39) = 0x14;
        obj = M32(out);
        t = (long double)I32(obj+0x1b);
        t *= (long double)CM_GLOBAL(double,0x009689b0);
        /* __ftol at preferred-image VA 008c9196. */
        M32(obj+0x1b) = (u32)(long long)t;
        M8(status) = 0x3c;
        result = 1;
        goto done;
    }
    obj = M32(out);
    if (obj) {
        cm_fn_008e5710(obj);
        cm_fn_00944c94((void *)obj);
    }
done:
    CM_FS(u32,0) = frame.prev;
    return result;
}

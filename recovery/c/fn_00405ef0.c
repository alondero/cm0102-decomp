#include "cmre.h"

/* Preferred-image VA00405EF0/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS0/RET0/full EAX. Keep all twenty fixed records even when allocation's
   signed-WORD count is smaller. Pointer/year fields reload at every native
   site; stores may overlap the receiver. Callees remain original boundaries.
   L=afterSUB208; indexL0/baseL4/error buffersL8/L108. */
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_CDECL cm_fn_00945501(u32,u32);
extern u32 CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,void *,void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

static u32 fields(u32 self, u32 offset, u16 first, u16 second,
                   u8 first_byte, u8 second_byte)
{
    u32 p;
    u16 value;
    p = D(self + 0x18u); W(p + offset + 0xau) = first;
    p = D(self + 0x18u); B(p + offset + 0xcu) = first_byte;
    p = D(self + 0x18u); W(p + offset + 0xdu) = second;
    p = D(self + 0x18u); B(p + offset + 0xfu) = second_byte;
    p = D(self + 0x18u);
    value = W(self + 0x40u);
    W(p + offset + 8u) = value;
    return p;
}

u32 CM_THISCALL cm_fn_00405ef0(u32 self)
{
    static const struct {
        u32 source;
        u16 first, second;
        u8 first_byte, second_byte;
    } rows[] = {
        {0x009d00acu,0x56,0x4e,0x26,0x26},
        {0x009d00d8u,0x45,0x51,0x26,0x26},
        {0x009d00d0u,0x4a,0x47,0x26,0x26},
        {0x009d0108u,0x3a,0x3d,0x26,0x26},
        {0x009d00b0u,0x3d,0x38,0x26,0x26},
        {0x009d0104u,0,0x37,0,0x26},
        {0x009d00e0u,0x42,0x29,0x26,0x26},
        {0x009d00ecu,0x37,0x31,0x26,0x26},
        {0x009d00d4u,0x31,0x37,0x26,0x26},
        {0x009d00b4u,0x3d,0x2a,0x26,0x26},
        {0x009d00e4u,0x37,0x30,0x26,0x26},
        {0x009d01f4u,0x2d,0x38,0x26,0x26},
        {0x009d0100u,0x32,0x2e,0x26,0x26},
        {0x009d00f8u,0x27,0x38,0x26,0x26},
        {0x009d00ccu,0x30,0x2b,0x26,0x26},
        {0x009d00e8u,0x2d,0x28,0x26,0x26},
        {0x009d00dcu,0x27,0x2b,0x26,0x26},
        {0x009d010cu,0x27,0x25,0x26,0x26},
        {0x009d01e4u,0,0,0,0},
        {0x009d0200u,0,0,0,0}
    };
    volatile u8 scratch[0x208];
    u32 l = (u32)scratch, eax, p, value, i, a, b, line;
    s32 count;
    u16 word = W(self + 0x3eu);
    W(self + 0x34u) = word;
    eax = cm_fn_00945501((u32)(s32)(s16)word,0x13);
    D(self + 0x18u) = eax;
    if (eax == 0) {
        a = l + 0x108u;
        b = l + 8u;
        line = 0x36e;
        goto error;
    }
    for (i = 0; i < 20; ++i) {
        if (i == 0) p = eax;
        else p = D(self + 0x18u);
        value = D(rows[i].source);
        if (i == 17) D(l) = 0;
        D(p + i * 0x13u) = value;
        eax = fields(self,i * 0x13u,rows[i].first,rows[i].second,
                     rows[i].first_byte,rows[i].second_byte);
    }
    count = (s16)W(self + 0x34u);
    if (count <= 0) return eax;
    p = D(self + 0x18u);
    value = D(0x00ae23b4u);
    D(l + 4u) = p;
    for (;;) {
        u32 club = D(p);
        u32 owner = D(value + club * 581u + 0x57u);
        if (owner == 0) break;
        {
            u32 expected = D(0x009cf654u);
            if (D(owner) != expected) break;
        }
        eax = D(l);
        p += 0x13u;
        ++eax;
        D(l) = eax;
        if ((s32)eax >= count) return eax;
    }
    p = D(l + 4u);
    cm_fn_009452ca(p);
    count = (s16)W(self + 0x34u);
    eax = cm_fn_00945501((u32)count,0x13);
    D(self + 0x18u) = eax;
    if (eax == 0) {
        a = l + 8u;
        b = l + 0x108u;
        line = 0x3e7;
        goto error;
    }
    eax = 0;
    if ((s16)W(self + 0x34u) > 0) {
        u32 offset = 0;
        for (;;) {
            p = D(self + 0x18u);
            ++eax;
            D(p + offset) = 0xffffffffu;
            p = D(self + 0x18u);
            word = W(self + 0x40u);
            W(p + offset + 8u) = word;
            offset += 0x13u;
            count = (s16)W(self + 0x34u);
            if ((s32)eax >= count) break;
        }
    }
    return eax;
error:
    cm_fn_00944cff(0x009873e8u,0,0,(void *)a,(void *)b);
    eax = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u,0x009870e8u,eax,a,b,line);
    eax = cm_fn_005e8290(0x009870e0u,0x00ae24d0u,0);
    D(0x00b67a34u) = 0;
    return eax;
}

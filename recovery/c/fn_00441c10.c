/* Preferred-image VA 0x00441c10, base 0x00400000; binary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Cdecl, five stack arguments; signed WORD arg2, BYTE arg5. Full EAX
   is explicitly 0/1. Original arithmetic and formatting remain boundaries.
   L is ESP immediately after SUB 224h. The formatting labels retain the
   original push order: P records a pending DWORD, B resolves [ESP+disp]
   against L and the pending push count. No scratch byte is defaulted. */
#include "cmre.h"
#define V8(a) CM_GLOBAL(volatile u8, (u32)(a))
#define V32(a) CM_GLOBAL(volatile u32, (u32)(a))
#define SX(x) ((u32)(s32)(s8)(x))
extern unsigned long long CM_STDCALL cm_fn_00945f30(u32,u32,u32,u32);
extern unsigned long long CM_STDCALL cm_fn_00945e80(u32,u32,u32,u32);
extern unsigned long long CM_STDCALL cm_fn_00945e40(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,void *,void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern void CM_CDECL cm_fn_005e8290(u32,u32,u32);

#define P(v) do { pending[n++] = (u32)(v); } while (0)
#define B(disp) scratch[(disp) - 0x10 - 4*n]
#define FMT(f) do { P(0xad9ca8); P(f); P(output); goto print; } while (0)
#define A4 pending[3],pending[2],pending[1],pending[0]
#define A5 pending[4],A4
#define A6 pending[5],A5
#define A7 pending[6],A6
#define A8 pending[7],A7
#define A9 pending[8],A8
#define A10 pending[9],A9
#define A11 pending[10],A10
#define A12 pending[11],A11
#define A13 pending[12],A12
#define A14 pending[13],A13
#define A15 pending[14],A14
#define A16 pending[15],A15
#define A17 pending[16],A16
#define A18 pending[17],A17
#define A19 pending[18],A18
#define A20 pending[19],A19
#define A21 pending[20],A20
#define A22 pending[21],A21
#define A23 pending[22],A22
#define PRINT_CASE(k) case k: cm_fn_00944c9f(A##k); break

u32 CM_CDECL cm_fn_00441c10(u32 output, u32 size, u32 low, u32 high, u32 mode)
{
    volatile u8 scratch[0x224];
    u32 pending[23], n=0, a, c, d, table, target, i, cursor;
    u32 unit_low, unit_high, next_low, next_high;
    u32 total_low=0, total_high=0, old_low, add_low;
    u8 bl=0, digit;
    unsigned long long value;
    if (output == 0 || (s16)size < 0x19) {
        /* Only ESI/EDI saved on this edge: [ESP+2c]=L+24. */
        cm_fn_00944cff(0x98b1fc,0,0,(void *)(scratch+0x124),(void *)(scratch+0x24));
        a=cm_fn_0090d130();
        cm_fn_00944c9f(0xae24d0,0x9870e8,a,(u32)(scratch+0x124),(u32)(scratch+0x24),0x2f3);
        cm_fn_005e8290(0x9870e0,0xae24d0,0);
        V32(0xb67a34)=0;
        return 0;
    }
    V8(output)=0;
    scratch[0]=0;
    scratch[0x1b]=0;
    *(volatile u32 *)(scratch+0x1c)=0;
    *(volatile u32 *)(scratch+0x20)=0;
    for (;;) {
        /* CMP high is signed; the equal-high low comparison is unsigned. */
        if ((s32)total_high > (s32)high) break;
        if (total_high == high && total_low >= low) break;
        table=SX(scratch[0x1b])*8u;
        unit_low=V32(0x98b148+table);
        unit_high=V32(0x98b14c+table);
        next_high=V32(0x98b154+table);
        next_low=V32(0x98b150+table);
        value=cm_fn_00945f30(low,high,next_low,next_high);
        value=cm_fn_00945e80((u32)value,(u32)(value>>32),unit_low,unit_high);
        scratch[0x1b]=(u8)(scratch[0x1b]+1);
        if ((s8)scratch[0x1b]>0) {
            i=SX(scratch[0x1b]);
            do { bl=scratch[i-1]; scratch[i]=bl; --i; } while (i!=0);
        }
        digit=(u8)value;
        bl=(u8)(digit+0x30);
        scratch[0]=bl;
        a=SX(digit);
        value=cm_fn_00945e40(a,(u32)((s32)a>>31),unit_low,unit_high);
        /* Reload accumulator after the original helper, before ADD/ADC. */
        old_low=*(volatile u32 *)(scratch+0x1c);
        total_high=*(volatile u32 *)(scratch+0x20);
        add_low=(u32)value;
        total_low=old_low+add_low;
        digit=scratch[0x1b];
        total_high+=(u32)(value>>32)+(total_low<old_low);
        *(volatile u32 *)(scratch+0x1c)=total_low;
        *(volatile u32 *)(scratch+0x20)=total_high;
        scratch[0x1b]=digit;
        if ((s8)digit>=0xf) break;
    }
    cursor=0; c=0xffffffffu;
    do { --c; digit=scratch[cursor++]; } while (c!=0 && digit!=0);
    a=(~c)-2;
    if (a>0xe) goto l25ad;
    table=((mode&2)!=0)?0x44265c:0x442698;
    target=V32(table+4*a);
    switch(target) {
    case 0x441d3e: goto l1d3e;
    case 0x441dbe: goto l1dbe;
    case 0x441e38: goto l1e38;
    case 0x441eac: goto l1eac;
    case 0x441f19: goto l1f19;
    case 0x441f80: goto l1f80;
    case 0x441fe1: goto l1fe1;
    case 0x44203b: goto l203b;
    case 0x44208f: goto l208f;
    case 0x4420dd: goto l20dd;
    case 0x442124: goto l2124;
    case 0x442147: goto l2147;
    case 0x4421c2: goto l21c2;
    case 0x442212: goto l2212;
    case 0x442250: goto l2250;
    case 0x442292: goto l2292;
    case 0x4422cd: goto l22cd;
    case 0x442353: goto l2353;
    case 0x4423e5: goto l23e5;
    case 0x442404: goto l2404;
    case 0x442436: goto l2436;
    case 0x44245e: goto l245e;
    case 0x4424b5: goto l24b5;
    case 0x442506: goto l2506;
    case 0x442564: goto l2564;
    case 0x44257b: goto l257b;
    case 0x44259f: goto l259f;
    default: __builtin_unreachable(); /* Pinned read-only table above. */
    }
l1d3e:
    c=SX(B(0x1e)); d=SX(B(0x1d)); a=SX(V8(0xad9ca2));
    P(0xad9c70); P(c); c=SX(B(0x24)); P(d); P(c);
    d=SX(B(0x2b)); c=SX(B(0x2a)); P(a); P(d); d=SX(B(0x31)); P(c); P(d);
    c=SX(B(0x38)); d=SX(B(0x37)); P(a); P(c); c=SX(B(0x3e)); P(d); P(c);
    d=SX(B(0x45)); c=SX(B(0x44)); P(a); P(d); d=SX(B(0x4b)); P(c); P(d);
    c=SX(B(0x51)); P(a); a=SX(B(0x56)); d=SX(bl); P(a); P(c); P(d); FMT(0x98b600);
l1dbe:
    c=SX(B(0x1d)); d=SX(B(0x1c)); a=SX(V8(0xad9ca2));
    P(0xad9c70); P(c); c=SX(B(0x23)); P(d); P(c);
    d=SX(B(0x2a)); c=SX(B(0x29)); P(a); P(d); d=SX(B(0x30)); P(c); P(d);
    c=SX(B(0x37)); d=SX(B(0x36)); P(a); P(c); c=SX(B(0x3d)); P(d); P(c);
    d=SX(B(0x44)); c=SX(B(0x43)); P(a); P(d); d=SX(B(0x4a)); P(c); P(d);
    P(a); a=SX(B(0x55)); c=SX(bl); P(a); P(c); FMT(0x98b5d4);
l1e38:
    d=SX(B(0x1c)); c=SX(B(0x1b)); a=SX(V8(0xad9ca2));
    P(0xad9c70); P(d); d=SX(B(0x22)); P(c); P(d);
    c=SX(B(0x29)); d=SX(B(0x28)); P(a); P(c); c=SX(B(0x2f)); P(d); P(c);
    d=SX(B(0x36)); c=SX(B(0x35)); P(a); P(d); d=SX(B(0x3c)); P(c); P(d);
    c=SX(B(0x43)); d=SX(B(0x42)); P(a); P(c); c=SX(B(0x49)); P(d); P(c);
    d=SX(bl); P(a); P(d); FMT(0x98b5ac);
l1eac:
    c=SX(B(0x1b)); d=SX(B(0x1a)); a=SX(V8(0xad9ca2));
    P(0xad9c70); P(c); c=SX(B(0x21)); P(d); P(c);
    d=SX(B(0x28)); c=SX(B(0x27)); P(a); P(d); d=SX(B(0x2e)); P(c); P(d);
    c=SX(B(0x35)); d=SX(B(0x34)); P(a); P(c); c=SX(B(0x3b)); P(d); P(c);
    d=SX(B(0x42)); P(a); P(d); a=SX(B(0x49)); c=SX(bl); P(a); P(c); FMT(0x98b588);
l1f19:
    d=SX(B(0x1a)); c=SX(B(0x19)); a=SX(V8(0xad9ca2));
    P(0xad9c70); P(d); d=SX(B(0x20)); P(c); P(d);
    c=SX(B(0x27)); d=SX(B(0x26)); P(a); P(c); c=SX(B(0x2d)); P(d); P(c);
    d=SX(B(0x34)); c=SX(B(0x33)); P(a); P(d); d=SX(B(0x3a)); P(c); P(d);
    P(a); a=SX(B(0x45)); c=SX(bl); P(a); P(c); FMT(0x98b564);
l1f80:
    d=SX(B(0x19)); c=SX(B(0x18)); a=SX(V8(0xad9ca2));
    P(0xad9c70); P(d); d=SX(B(0x1f)); P(c); P(d);
    c=SX(B(0x26)); d=SX(B(0x25)); P(a); P(c); c=SX(B(0x2c)); P(d); P(c);
    d=SX(B(0x33)); c=SX(B(0x32)); P(a); P(d); d=SX(B(0x39)); P(c); P(d);
    P(a); a=SX(bl); P(a); FMT(0x98b544);
l1fe1:
    c=SX(B(0x18)); d=SX(B(0x17)); a=SX(V8(0xad9ca2));
    P(0xad9c70); P(c); c=SX(B(0x1e)); P(d); P(c);
    d=SX(B(0x25)); c=SX(B(0x24)); P(a); P(d); d=SX(B(0x2b)); P(c); P(d);
    c=SX(B(0x31)); P(a); a=SX(B(0x36)); d=SX(bl); P(a); P(c); P(d); FMT(0x98b528);
l203b:
    c=SX(B(0x17)); d=SX(B(0x16)); a=SX(V8(0xad9ca2));
    P(0xad9c70); P(c); c=SX(B(0x1d)); P(d); P(c);
    d=SX(B(0x24)); c=SX(B(0x23)); P(a); P(d); d=SX(B(0x2a)); P(c); P(d);
    P(a); a=SX(B(0x35)); c=SX(bl); P(a); P(c); FMT(0x98b50c);
l208f:
    d=SX(B(0x16)); c=SX(B(0x15)); a=SX(V8(0xad9ca2));
    P(0xad9c70); P(d); d=SX(B(0x1c)); P(c); P(d);
    c=SX(B(0x23)); d=SX(B(0x22)); P(a); P(c); c=SX(B(0x29)); P(d); P(c);
    d=SX(bl); P(a); P(d); FMT(0x98b4f4);
l20dd: P(0xad9c70);
l20e2:
    a=SX(B(0x19)); c=SX(B(0x18)); d=SX(B(0x17)); P(a); P(c);
    a=SX(V8(0xad9ca2)); c=SX(B(0x1e)); P(d); P(a);
    d=SX(B(0x25)); a=SX(bl); P(c); P(d); P(a); FMT(0x98b4e0);
l2124:
    c=SX(B(0x14)); d=SX(B(0x13)); a=SX(B(0x12));
    P(0xad9c70); P(c); c=SX(V8(0xad9ca2)); P(d); P(a); goto l2482;
l2147:
    a=V32(0xad9ca4); P(0xad9c70);
    if (a!=0) goto l2178;
    c=SX(B(0x17)); d=SX(B(0x16)); a=SX(B(0x15)); P(c); P(d);
    c=SX(V8(0xad9ca2)); d=SX(bl); P(a); P(c); P(d); goto l24de;
l2178:
    a=SX(B(0x17)); c=SX(B(0x16)); P(a);
l2183:
    d=SX(B(0x19)); P(c);
l2189:
    a=SX(bl); P(d); P(a);
l218e: FMT(0x98b4d0);
l21c2:
    a=B(0x14); if (a==0x30) goto l21d4; P(0xad9d34); goto l246b;
l21d4:
    a=B(0x13); P(0xad9d34); if (a==0x30) goto l2202;
l21e1:
    d=SX(V8(0xad9cc8)); c=SX(a); a=SX(B(0x16)); P(c); P(d);
    c=SX(B(0x1d)); d=SX(bl); P(a); P(c); P(d); goto l24de;
l2202:
    a=SX(B(0x16)); c=SX(B(0x15)); P(a); goto l254a;
l2212:
    a=B(0x13); if (a==0x30) goto l2224; P(0xad9d34); goto l24c2;
l2224:
    a=B(0x12); P(0xad9d34); if (a==0x30) goto l2427;
l2235:
    c=SX(B(0x15)); d=SX(a); a=SX(V8(0xad9cc8)); P(d); P(a);
    d=SX(bl); P(c); P(d); goto l218e;
l2250:
    a=B(0x12); if (a==0x30) goto l2278;
    c=SX(V8(0xad9cc8)); d=SX(a); a=SX(B(0x11)); P(0xad9d34); P(d);
    d=SX(bl); P(a); P(c); P(d); goto l218e;
l2278:
    a=B(0x11); P(0xad9d34); if (a!=0x30) goto l253f;
l2289: a=SX(bl); P(a); goto l23a5;
l2292:
    a=V8(0xad9ccc); if (a==0) { P(0xad9d68); goto l20e2; }
    a=B(0x14); if (a==0x30) goto l22ad; P(0xad9ccc); goto l246b;
l22ad: a=B(0x13); P(0xad9ccc);
l22b8: if (a==0x30) goto l2202; goto l21e1;
l22cd:
    a=V8(0xad9ccc); if (a==0) goto l2325;
    a=B(0x13); if (a==0x30) goto l2304;
    d=SX(B(0x12)); c=SX(a); a=SX(V8(0xad9cc8)); P(0xad9ccc); P(c);
    c=SX(B(0x19)); P(d); P(a); d=SX(bl); P(c); P(d); goto l24de;
l2304:
    a=B(0x12); P(0xad9ccc); if (a==0x30) goto l2580;
    c=SX(V8(0xad9cc8)); a=SX(a); P(a); goto l2183;
l2325:
    a=SX(B(0x14)); c=SX(B(0x13)); d=SX(B(0x12)); P(0xad9d68); P(a);
    a=SX(V8(0xad9ca2)); P(c); P(d); c=SX(B(0x21)); d=SX(bl);
    P(a); P(c); P(d); goto l248d;
l2353:
    a=V8(0xad9ccc); if (a==0) goto l23bd;
    a=B(0x12); if (a==0x30) goto l237f;
    c=SX(B(0x11)); d=SX(V8(0xad9cc8)); a=SX(a);
    P(0xad9ccc); P(a); P(c); goto l2189;
l237f:
    a=B(0x11); P(0xad9ccc); if (a==0x30) goto l23a1;
    d=SX(V8(0xad9cc8)); c=SX(a); a=SX(bl); P(c); P(d); P(a); goto l254f;
l23a1: c=SX(bl); P(c);
l23a5: FMT(0x98b4c8);
l23bd:
    d=SX(B(0x13)); a=SX(B(0x12)); c=SX(B(0x11)); P(0xad9d68); P(d);
    d=SX(V8(0xad9ca2)); P(a); P(c); a=SX(bl); P(d); P(a); goto l24de;
l23e5:
    a=B(0x14); if (a==0x30) goto l23f4; P(0xad9d68); goto l246b;
l23f4: a=B(0x13); P(0xad9d68); goto l22b8;
l2404:
    a=B(0x13); if (a==0x30) goto l2416; P(0xad9d68); goto l24c2;
l2416: a=B(0x12); P(0xad9d68); if (a!=0x30) goto l2235;
l2427: a=SX(B(0x15)); c=SX(bl); P(a); P(c); goto l258a;
l2436:
    a=B(0x12); if (a==0x30) goto l2448; P(0xad9d68); goto l2513;
l2448: a=B(0x11); P(0xad9d68); if (a==0x30) goto l2289; goto l253f;
l245e:
    a=B(0x14); if (a==0x30) goto l24a5; P(0xad9d00);
l246b:
    d=SX(B(0x17)); c=SX(a); a=SX(V8(0xad9cc8)); P(c); P(d);
    c=SX(B(0x1e)); P(a);
l2482: d=SX(B(0x21)); a=SX(bl); P(c); P(d); P(a);
l248d: FMT(0x98b4b4);
l24a5: a=B(0x13); P(0xad9d00); goto l22b8;
l24b5:
    a=B(0x13); if (a==0x30) goto l24f6; P(0xad9d00);
l24c2:
    c=SX(B(0x16)); d=SX(V8(0xad9cc8)); a=SX(a); P(a); P(c);
    a=SX(B(0x1d)); c=SX(bl); P(d); P(a); P(c);
l24de: FMT(0x98b4a4);
l24f6: a=B(0x12); P(0xad9d00); if (a!=0x30) goto l2235; goto l2427;
l2506:
    a=B(0x12); if (a==0x30) goto l252e; P(0xad9d00);
l2513:
    c=SX(V8(0xad9cc8)); d=SX(a); a=SX(B(0x15)); P(d); P(a);
    d=SX(bl); P(c); P(d); goto l218e;
l252e: a=B(0x11); P(0xad9d00); if (a==0x30) goto l2289;
l253f: c=SX(V8(0xad9cc8)); a=SX(a); P(a);
l254a: d=SX(bl); P(c); P(d);
l254f: FMT(0x98b498);
l2564:
    c=SX(B(0x12)); d=SX(B(0x11)); a=SX(bl); P(0xad9c70); P(c); P(d); P(a); goto l254f;
l257b: P(0xad9c70);
l2580: c=SX(B(0x15)); d=SX(bl); P(c); P(d);
l258a: FMT(0x98b48c);
l259f: a=SX(bl); P(0xad9c70); P(a); goto l23a5;
l25ad: P(0xad9c70); FMT(0x98b484);
print:
    /* Exact number of original pushed DWORDs, in cdecl order. */
    switch(n) {
    PRINT_CASE(4); PRINT_CASE(5); PRINT_CASE(6); PRINT_CASE(7);
    PRINT_CASE(8); PRINT_CASE(9); PRINT_CASE(10); PRINT_CASE(11);
    PRINT_CASE(13); PRINT_CASE(14); PRINT_CASE(15); PRINT_CASE(17);
    PRINT_CASE(18); PRINT_CASE(19); PRINT_CASE(21); PRINT_CASE(22); PRINT_CASE(23);
    default: __builtin_unreachable();
    }
    a=V8(output); c=0;
    if ((u8)a!=0) {
        a=output;
        do {
            if (V8(a)==0x20) V8(a)=0x7c;
            ++c; d=(u32)(s32)(s16)c; a=output+d; digit=V8(output+d);
        } while (digit!=0);
    }
    return 1;
}

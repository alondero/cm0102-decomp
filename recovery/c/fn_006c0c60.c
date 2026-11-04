/* Root independent delta/lowering review complete; actual gates and controls pending. */
#include "cmre.h"
/* Independent full source review complete; canonical gate and physical-site controls pending. */
/* Preferred-image VA 006c0c60, base 00400000; original SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   P is ESP after original nonvolatile saves. One uninitialized allocation
   preserves outgoing words, saved-slot spacing and original local adjacency.
   Original buffers are P+30h and P+130h. Fixed f0..f3 denote compile-time
   x87 value positions; there is no runtime instruction or stack interpreter.
   FNSTSW AX has no live consumer other than its subsequent AH predicate;
   subsequent FTOL, or 006c1aba on the no-role path, overwrites EAX.
   tail_side projects BL after 006c1d26: upper EBX bits have no consumer.
   Dead upper register bits are projected only after static bit-liveness proof.
*/
#define M(type, address) (*(volatile type *)(u32)(address))
#define S(type, offset) (*(volatile type *)(storage.bytes + 0x30 + (offset)))
#define B(a) M(u8, a)
#define W(a) M(u16, a)
#define I(a) M(u32, a)
#define SB(a) M(s8, a)
#define SW(a) M(s16, a)
#define F(a) M(float, a)
#define D(a) M(double, a)
extern void CM_THISCALL cm_fn_006f56c0(void *, float, s16);
extern u32 CM_THISCALL cm_fn_00717410(void *);
extern unsigned long long CM_THISCALL cm_fn_007165b0(void *, u32);
extern u32 CM_CDECL cm_fn_0090cfc0(s32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_005c6710(void *, u32);
__attribute__((target("no-sse3")))
u32 CM_THISCALL cm_fn_006c0c60(void *self, u32 arg)
{
    union { long double alignment; u8 bytes[0x260]; } storage;
    u32 a, b, c, d, record, object, side;
    u8 tail_side;
    long double f0, f1, f2, f3, exchange;
    unsigned long long native_result;
    long long converted;
    int fp_above, take;
    c = (u32)self;
    side = (s32)(s8)arg;
    object = c;
    a = 0;
    S(u8, 0x2b) = 0;
    W(object + side*2u + 0x1dau) = (u16)a;
    W(object + side*2u + 0x1deu) = (u16)a;
    W(object + side*2u + 0x1e2u) = (u16)a;
    W(object + side*2u + 0x1e6u) = (u16)a;
    W(object + side*2u + 0x1eau) = (u16)a;
    W(object + side*2u + 0x1eeu) = (u16)a;
    W(object + side*2u + 0x1f2u) = (u16)a;
    a = side*8u;
    a = a - side;
    a = a << 5;
    a = a - side;
    a = a + a*4u;
    record = object + a*8u + 0x4796u;
L0ccb:
    a = (u8)B(record + 0x19u);
    take = (s8)((u8)a & (u8)a) < 0;
    if (take) goto L1aa2;
    a = I(record + 0x69u);
    take = (a & a) == 0;
    if (take) goto L1b42;
    c = I(record + 0x6du);
    a = 0x1eu;
    S(u32, -4) = (u32)-1;
    S(u32, -8) = c;
    d = (s32)SB(c + 0x3cu);
    a = a - d;
    c = record;
    S(u32, 0x1c) = a;
    f0 = S(s32, 0x1c);
    S(float, -8) = (float)f0;
    cm_fn_006f56c0((void *)c, S(float, -8), (s16)S(u32, -4));
    c = record;
    a = cm_fn_00717410((void *)c);
    f0 = F(record + 0xddu);
    f0 += F(record + 0xcdu);
    b = a;
    a = I(record + 0x6du);
    take = (a & a) == 0;
    f0 += F(record + 0xc1u);
    S(u32, 0x24) = b;
    f0 += F(record + 0x7du);
    f0 += F(0x969560u);
    f0 *= F(0x968f18u);
    f0 += F(0x96d3c8u);
    f1 = F(record + 0x4du);
    f1 *= D(0x967898u);
    f0 += f1;
    f0 -= D(0x9688a8u);
    S(float, 0x14) = (float)f0;
    if (take) goto L0fc3;
    c = (u8)B(a + 0x2fu);
    c = (u8)((s8)(u8)c >> 2);
    d = (s32)(s8)(u8)c;
    S(u32, 0x1c) = d;
    f0 = S(s32, 0x1c);
    take = ((u8)b & 4u) == 0;
    f0 += S(float, 0x14);
    if (take) goto L0d86;
    c = (s32)SW(a + 5u);
    S(u32, 0x1c) = c;
    f1 = S(s32, 0x1c);
    f1 *= D(0x967880u);
    goto L0dca;
L0d86:
    take = ((u8)b & 2u) == 0;
    if (take) goto L0d9f;
    d = (s32)SW(a + 5u);
    S(u32, 0x1c) = d;
    f1 = S(s32, 0x1c);
    f1 *= D(0x967880u);
    goto L0dca;
L0d9f:
    take = ((u8)b & 8u) == 0;
    if (take) goto L0db8;
    c = (s32)SW(a + 5u);
    S(u32, 0x1c) = c;
    f1 = S(s32, 0x1c);
    f1 *= D(0x96d3c0u);
    goto L0dca;
L0db8:
    d = (s32)SW(a + 5u);
    S(u32, 0x1c) = d;
    f1 = S(s32, 0x1c);
    f1 *= D(0x967898u);
L0dca:
    c = (u8)B(a + 0x32u);
    f1 += f0;
    take = (s8)(u8)c <= 15;
    S(float, 0x14) = (float)f1;
    if (take) goto L0de6;
    f0 = S(float, 0x14);
    f0 *= F(0x96d3b8u);
    goto L0e06;
L0de6:
    take = (s8)(u8)c >= 6;
    if (take) goto L0df7;
    f0 = S(float, 0x14);
    f0 *= F(0x96b014u);
    goto L0e06;
L0df7:
    take = (s8)(u8)c >= 10;
    if (take) goto L0e0a;
    f0 = S(float, 0x14);
    f0 *= F(0x96d3b4u);
L0e06:
    S(float, 0x14) = (float)f0;
L0e0a:
    c = (u8)B(a + 0x3cu);
    take = (s8)(u8)c <= 15;
    if (take) goto L0e1e;
    f0 = S(float, 0x14);
    f0 *= F(0x96d3b8u);
    goto L0e3e;
L0e1e:
    take = (s8)(u8)c >= 6;
    if (take) goto L0e2f;
    f0 = S(float, 0x14);
    f0 *= F(0x96a32cu);
    goto L0e3e;
L0e2f:
    take = (s8)(u8)c >= 10;
    if (take) goto L0e42;
    f0 = S(float, 0x14);
    f0 *= F(0x96b014u);
L0e3e:
    S(float, 0x14) = (float)f0;
L0e42:
    c = (u8)B(a + 0x44u);
    take = (s8)(u8)c <= 15;
    if (take) goto L0e56;
    f0 = S(float, 0x14);
    f0 *= F(0x96d3b0u);
    goto L0e76;
L0e56:
    take = (s8)(u8)c >= 6;
    if (take) goto L0e67;
    f0 = S(float, 0x14);
    f0 *= F(0x96b014u);
    goto L0e76;
L0e67:
    take = (s8)(u8)c >= 10;
    if (take) goto L0e7a;
    f0 = S(float, 0x14);
    f0 *= F(0x96d3b4u);
L0e76:
    S(float, 0x14) = (float)f0;
L0e7a:
    c = (u8)B(a + 0x3eu);
    take = (s8)(u8)c <= 15;
    if (take) goto L0e8e;
    f0 = S(float, 0x14);
    f0 *= F(0x96d3b8u);
    goto L0eae;
L0e8e:
    take = (s8)(u8)c >= 6;
    if (take) goto L0e9f;
    f0 = S(float, 0x14);
    f0 *= F(0x96b014u);
    goto L0eae;
L0e9f:
    take = (s8)(u8)c >= 10;
    if (take) goto L0eb2;
    f0 = S(float, 0x14);
    f0 *= F(0x96d3b4u);
L0eae:
    S(float, 0x14) = (float)f0;
L0eb2:
    c = (u8)B(a + 0x1eu);
    take = (s8)(u8)c <= 15;
    if (take) goto L0ec6;
    f0 = S(float, 0x14);
    f0 *= F(0x96d3b8u);
    goto L0ee6;
L0ec6:
    take = (s8)(u8)c >= 6;
    if (take) goto L0ed7;
    f0 = S(float, 0x14);
    f0 *= F(0x96b014u);
    goto L0ee6;
L0ed7:
    take = (s8)(u8)c >= 10;
    if (take) goto L0eea;
    f0 = S(float, 0x14);
    f0 *= F(0x96d3b4u);
L0ee6:
    S(float, 0x14) = (float)f0;
L0eea:
    c = (u8)B(a + 0x2eu);
    take = (s8)(u8)c <= 15;
    if (take) goto L0efe;
    f0 = S(float, 0x14);
    f0 *= F(0x96d3b0u);
    goto L0f1e;
L0efe:
    take = (s8)(u8)c >= 6;
    if (take) goto L0f0f;
    f0 = S(float, 0x14);
    f0 *= F(0x96b014u);
    goto L0f1e;
L0f0f:
    take = (s8)(u8)c >= 10;
    if (take) goto L0f22;
    f0 = S(float, 0x14);
    f0 *= F(0x96d3b4u);
L0f1e:
    S(float, 0x14) = (float)f0;
L0f22:
    c = (u8)B(a + 0x2bu);
    take = (s8)(u8)c <= 15;
    if (take) goto L0f36;
    f0 = S(float, 0x14);
    f0 *= F(0x96d3b0u);
    goto L0f56;
L0f36:
    take = (s8)(u8)c >= 6;
    if (take) goto L0f47;
    f0 = S(float, 0x14);
    f0 *= F(0x96b014u);
    goto L0f56;
L0f47:
    take = (s8)(u8)c >= 10;
    if (take) goto L0f5a;
    f0 = S(float, 0x14);
    f0 *= F(0x96d3b4u);
L0f56:
    S(float, 0x14) = (float)f0;
L0f5a:
    c = (u8)B(a + 0x36u);
    b = (u8)B(a + 0x1bu);
    c = (u8)((u8)c + (u8)b);
    take = (s8)(u8)c <= 35;
    if (take) goto L0f73;
    f0 = S(float, 0x14);
    f0 *= F(0x96d3b0u);
    goto L0f93;
L0f73:
    take = (s8)(u8)c >= 10;
    if (take) goto L0f84;
    f0 = S(float, 0x14);
    f0 *= F(0x96a32cu);
    goto L0f93;
L0f84:
    take = (s8)(u8)c >= 20;
    if (take) goto L0f97;
    f0 = S(float, 0x14);
    f0 *= F(0x96d3b4u);
L0f93:
    S(float, 0x14) = (float)f0;
L0f97:
    a = (s32)SB(a + 0x21u);
    S(u32, -4) = a;
    a = cm_fn_0090cfc0((s32)S(u32, -4));
    S(u32, -8) = 20;
    b = a;
    a = cm_fn_0090cfc0((s32)S(u32, -8));
    take = (s32)b >= (s32)a;
    if (take) goto L0fbf;
    f0 = S(float, 0x14);
    f0 *= F(0x96d248u);
    S(float, 0x14) = (float)f0;
L0fbf:
    b = S(u32, 0x24);
L0fc3:
    f0 = F(record + 0xf1u);
    f0 += f0;
    f1 = F(record + 0xc5u);
    f1 += f1;
    c = I(record + 0x6du);
    f0 += f1;
    take = (c & c) == 0;
    f0 += F(record + 0x9du);
    f0 += F(record + 0xa9u);
    f1 = F(record + 0x8du);
    f1 *= D(0x968940u);
    f0 += f1;
    f0 += D(0x968ff8u);
    f0 *= D(0x968968u);
    f1 = F(record + 0x4du);
    f1 *= D(0x967898u);
    f0 += f1;
    f0 += D(0x96d3a8u);
    f0 -= D(0x9688a8u);
    S(float, 0x10) = (float)f0;
    if (take) goto L12a3;
    take = ((u8)b & 0x40u) == 0;
    if (take) goto L103a;
    d = (s32)SW(c + 5u);
    S(u32, 0x1c) = d;
    f0 = S(s32, 0x1c);
    f0 *= D(0x967880u);
    goto L1097;
L103a:
    take = ((u8)b & 0x20u) == 0;
    if (take) goto L1053;
    a = (s32)SW(c + 5u);
    S(u32, 0x1c) = a;
    f0 = S(s32, 0x1c);
    f0 *= D(0x96d3a0u);
    goto L1097;
L1053:
    take = ((u8)b & 0x10u) == 0;
    if (take) goto L106c;
    d = (s32)SW(c + 5u);
    S(u32, 0x1c) = d;
    f0 = S(s32, 0x1c);
    f0 *= D(0x96d398u);
    goto L1097;
L106c:
    take = ((u8)b & 8u) == 0;
    if (take) goto L1085;
    a = (s32)SW(c + 5u);
    S(u32, 0x1c) = a;
    f0 = S(s32, 0x1c);
    f0 *= D(0x96a740u);
    goto L1097;
L1085:
    d = (s32)SW(c + 5u);
    S(u32, 0x1c) = d;
    f0 = S(s32, 0x1c);
    f0 *= D(0x967898u);
L1097:
    a = (u8)B(c + 0x44u);
    f0 += S(float, 0x10);
    take = (s8)(u8)a <= 15;
    S(float, 0x10) = (float)f0;
    if (take) goto L10b2;
    f0 = S(float, 0x10);
    f0 *= F(0x96d3b8u);
    goto L10c0;
L10b2:
    take = (s8)(u8)a > 5;
    if (take) goto L10c4;
    f0 = S(float, 0x10);
    f0 *= F(0x96d394u);
L10c0:
    S(float, 0x10) = (float)f0;
L10c4:
    a = (u8)B(c + 0x3cu);
    take = (s8)(u8)a <= 15;
    if (take) goto L10d7;
    f0 = S(float, 0x10);
    f0 *= F(0x96d3b0u);
    goto L10e5;
L10d7:
    take = (s8)(u8)a > 5;
    if (take) goto L10e9;
    f0 = S(float, 0x10);
    f0 *= F(0x96b014u);
L10e5:
    S(float, 0x10) = (float)f0;
L10e9:
    a = (u8)B(c + 0x35u);
    take = (s8)(u8)a <= 15;
    if (take) goto L10fc;
    f0 = S(float, 0x10);
    f0 *= F(0x96a33cu);
    goto L110a;
L10fc:
    take = (s8)(u8)a > 5;
    if (take) goto L110e;
    f0 = S(float, 0x10);
    f0 *= F(0x96a32cu);
L110a:
    S(float, 0x10) = (float)f0;
L110e:
    a = (u8)B(c + 0x37u);
    take = (s8)(u8)a <= 15;
    if (take) goto L1121;
    f0 = S(float, 0x10);
    f0 *= F(0x96d390u);
    goto L113f;
L1121:
    take = (s8)(u8)a >= 6;
    if (take) goto L1131;
    f0 = S(float, 0x10);
    f0 *= F(0x96d38cu);
    goto L113f;
L1131:
    take = (s8)(u8)a >= 10;
    if (take) goto L1143;
    f0 = S(float, 0x10);
    f0 *= F(0x96d394u);
L113f:
    S(float, 0x10) = (float)f0;
L1143:
    a = (u8)B(c + 0x43u);
    take = (s8)(u8)a <= 15;
    if (take) goto L1156;
    f0 = S(float, 0x10);
    f0 *= F(0x96d390u);
    goto L1174;
L1156:
    take = (s8)(u8)a >= 6;
    if (take) goto L1166;
    f0 = S(float, 0x10);
    f0 *= F(0x96d38cu);
    goto L1174;
L1166:
    take = (s8)(u8)a >= 10;
    if (take) goto L1178;
    f0 = S(float, 0x10);
    f0 *= F(0x96d394u);
L1174:
    S(float, 0x10) = (float)f0;
L1178:
    a = (u8)B(c + 0x40u);
    b = (u8)B(c + 0x36u);
    d = (u8)B(c + 0x26u);
    a = (u8)((u8)a + (u8)b);
    b = (u8)B(c + 0x1bu);
    a = (u8)((u8)a + (u8)d);
    a = (u8)((u8)a + (u8)b);
    take = (s8)(u8)a <= 75;
    if (take) goto L119a;
    f0 = S(float, 0x10);
    f0 *= F(0x96d388u);
    goto L11d4;
L119a:
    take = (s8)(u8)a <= 65;
    if (take) goto L11aa;
    f0 = S(float, 0x10);
    f0 *= F(0x96d3b8u);
    goto L11d4;
L11aa:
    take = (s8)(u8)a <= 50;
    if (take) goto L11ba;
    f0 = S(float, 0x10);
    f0 *= F(0x96d3b0u);
    goto L11d4;
L11ba:
    take = (s8)(u8)a > 35;
    if (take) goto L11d8;
    f0 = S(float, 0x10);
    take = (s8)(u8)a <= 25;
    if (take) goto L11ce;
    f0 *= F(0x96d3b4u);
    goto L11d4;
L11ce:
    f0 *= F(0x96d394u);
L11d4:
    S(float, 0x10) = (float)f0;
L11d8:
    a = (u8)B(c + 0x1eu);
    take = (s8)(u8)a <= 15;
    if (take) goto L11eb;
    f0 = S(float, 0x10);
    f0 *= F(0x96a33cu);
    goto L1209;
L11eb:
    take = (s8)(u8)a >= 10;
    if (take) goto L11fb;
    f0 = S(float, 0x10);
    f0 *= F(0x96b014u);
    goto L1209;
L11fb:
    take = (s8)(u8)a >= 6;
    if (take) goto L120d;
    f0 = S(float, 0x10);
    f0 *= F(0x96a32cu);
L1209:
    S(float, 0x10) = (float)f0;
L120d:
    a = (u8)B(c + 0x33u);
    take = (s8)(u8)a <= 15;
    if (take) goto L1220;
    f0 = S(float, 0x10);
    f0 *= F(0x96d3b8u);
    goto L123e;
L1220:
    take = (s8)(u8)a >= 6;
    if (take) goto L1230;
    f0 = S(float, 0x10);
    f0 *= F(0x96b014u);
    goto L123e;
L1230:
    take = (s8)(u8)a >= 10;
    if (take) goto L1242;
    f0 = S(float, 0x10);
    f0 *= F(0x96d3b4u);
L123e:
    S(float, 0x10) = (float)f0;
L1242:
    a = (u8)B(c + 0x3fu);
    take = (s8)(u8)a <= 15;
    if (take) goto L1255;
    f0 = S(float, 0x10);
    f0 *= F(0x96d3b0u);
    goto L1273;
L1255:
    take = (s8)(u8)a >= 10;
    if (take) goto L1265;
    f0 = S(float, 0x10);
    f0 *= F(0x96b014u);
    goto L1273;
L1265:
    take = (s8)(u8)a >= 5;
    if (take) goto L1277;
    f0 = S(float, 0x10);
    f0 *= F(0x96a32cu);
L1273:
    S(float, 0x10) = (float)f0;
L1277:
    a = (s32)SB(c + 0x21u);
    S(u32, -4) = a;
    a = cm_fn_0090cfc0((s32)S(u32, -4));
    S(u32, -8) = 20;
    b = a;
    a = cm_fn_0090cfc0((s32)S(u32, -8));
    take = (s32)b >= (s32)a;
    if (take) goto L129f;
    f0 = S(float, 0x10);
    f0 *= F(0x96d248u);
    S(float, 0x10) = (float)f0;
L129f:
    b = S(u32, 0x24);
L12a3:
    f0 = F(record + 0x95u);
    f0 += F(record + 0x71u);
    f1 = F(record + 0xb9u);
    f1 *= D(0x968940u);
    c = I(record + 0x6du);
    take = (c & c) == 0;
    f0 += f1;
    f1 = F(record + 0x99u);
    f1 += f1;
    f0 += f1;
    f1 = F(record + 0x7du);
    f1 *= D(0x968940u);
    f0 += f1;
    f0 += F(record + 0xa9u);
    f0 += F(record + 0xc1u);
    f0 += D(0x968ff8u);
    f0 *= D(0x968968u);
    f1 = F(record + 0x4du);
    f1 *= D(0x967898u);
    f0 += f1;
    f0 += D(0x96d380u);
    f0 -= D(0x9688a8u);
    S(float, 0x20) = (float)f0;
    if (take) goto L151a;
    take = ((u8)b & 0x40u) == 0;
    if (take) goto L1326;
    d = (s32)SW(c + 5u);
    S(u32, 0x1c) = d;
    f0 = S(s32, 0x1c);
    f0 *= D(0x968918u);
    goto L1383;
L1326:
    take = ((u8)b & 0x20u) == 0;
    if (take) goto L133f;
    a = (s32)SW(c + 5u);
    S(u32, 0x1c) = a;
    f0 = S(s32, 0x1c);
    f0 *= D(0x96d378u);
    goto L1383;
L133f:
    take = ((u8)b & 0x10u) == 0;
    if (take) goto L1358;
    d = (s32)SW(c + 5u);
    S(u32, 0x1c) = d;
    f0 = S(s32, 0x1c);
    f0 *= D(0x96d1e0u);
    goto L1383;
L1358:
    take = ((u8)b & 8u) == 0;
    if (take) goto L1371;
    a = (s32)SW(c + 5u);
    S(u32, 0x1c) = a;
    f0 = S(s32, 0x1c);
    f0 *= D(0x96d370u);
    goto L1383;
L1371:
    d = (s32)SW(c + 5u);
    S(u32, 0x1c) = d;
    f0 = S(s32, 0x1c);
    f0 *= D(0x967898u);
L1383:
    a = (u8)B(c + 0x44u);
    f0 += S(float, 0x20);
    take = (s8)(u8)a <= 15;
    if (take) goto L1396;
    f0 *= F(0x96d3b0u);
    goto L13ac;
L1396:
    take = (s8)(u8)a >= 6;
    if (take) goto L13a2;
    f0 *= F(0x96b014u);
    goto L13ac;
L13a2:
    take = (s8)(u8)a >= 10;
    if (take) goto L13ac;
    f0 *= F(0x96d3b4u);
L13ac:
    a = (u8)B(c + 0x3cu);
    take = (s8)(u8)a <= 15;
    if (take) goto L13bb;
    f0 *= F(0x96d3b0u);
    goto L13d1;
L13bb:
    take = (s8)(u8)a >= 6;
    if (take) goto L13c7;
    f0 *= F(0x96b014u);
    goto L13d1;
L13c7:
    take = (s8)(u8)a >= 10;
    if (take) goto L13d1;
    f0 *= F(0x96d3b4u);
L13d1:
    a = (u8)B(c + 0x35u);
    take = (s8)(u8)a <= 15;
    if (take) goto L13e0;
    f0 *= F(0x96d3b8u);
    goto L13f6;
L13e0:
    take = (s8)(u8)a <= 10;
    if (take) goto L13ec;
    f0 *= F(0x96d3b0u);
    goto L13f6;
L13ec:
    take = (s8)(u8)a >= 6;
    if (take) goto L13f6;
    f0 *= F(0x96d394u);
L13f6:
    a = (u8)B(c + 0x31u);
    take = (s8)(u8)a <= 15;
    if (take) goto L1405;
    f0 *= F(0x96d368u);
    goto L141b;
L1405:
    take = (s8)(u8)a <= 10;
    if (take) goto L1411;
    f0 *= F(0x96d3b0u);
    goto L141b;
L1411:
    take = (s8)(u8)a >= 6;
    if (take) goto L141b;
    f0 *= F(0x96d3b4u);
L141b:
    a = (u8)B(c + 0x36u);
    b = (u8)B(c + 0x26u);
    d = (u8)B(c + 0x40u);
    a = (u8)((u8)a + (u8)b);
    a = (u8)((u8)a + B(c + 0x1bu));
    a = (u8)((u8)a + (u8)d);
    take = (s8)(u8)a <= 75;
    if (take) goto L1437;
    f0 *= F(0x96d388u);
    goto L146d;
L1437:
    take = (s8)(u8)a <= 65;
    if (take) goto L1443;
    f0 *= F(0x96d3b8u);
    goto L146d;
L1443:
    take = (s8)(u8)a <= 50;
    if (take) goto L144f;
    f0 *= F(0x96d3b0u);
    goto L146d;
L144f:
    take = (s8)(u8)a >= 10;
    if (take) goto L145b;
    f0 *= F(0x96d320u);
    goto L146d;
L145b:
    take = (s8)(u8)a >= 25;
    if (take) goto L1467;
    f0 *= F(0x96d364u);
    goto L146d;
L1467:
    f0 *= F(0x96b014u);
L146d:
    a = (u8)B(c + 0x1eu);
    S(float, 0x20) = (float)f0;
    take = (s8)(u8)a <= 15;
    if (take) goto L1484;
    f0 = S(float, 0x20);
    f0 *= F(0x96d388u);
    goto L14a2;
L1484:
    take = (s8)(u8)a >= 6;
    if (take) goto L1494;
    f0 = S(float, 0x20);
    f0 *= F(0x96a32cu);
    goto L14a2;
L1494:
    take = (s8)(u8)a >= 10;
    if (take) goto L14a6;
    f0 = S(float, 0x20);
    f0 *= F(0x96d360u);
L14a2:
    S(float, 0x20) = (float)f0;
L14a6:
    a = (u8)B(c + 0x33u);
    take = (s8)(u8)a <= 15;
    if (take) goto L14b9;
    f0 = S(float, 0x20);
    f0 *= F(0x96d388u);
    goto L14d7;
L14b9:
    take = (s8)(u8)a <= 10;
    if (take) goto L14c9;
    f0 = S(float, 0x20);
    f0 *= F(0x96d3b0u);
    goto L14d7;
L14c9:
    take = (s8)(u8)a >= 6;
    if (take) goto L14db;
    f0 = S(float, 0x20);
    f0 *= F(0x96d394u);
L14d7:
    S(float, 0x20) = (float)f0;
L14db:
    take = (s8)(u8)d <= 15;
    if (take) goto L14ee;
    f0 = S(float, 0x20);
    f0 *= F(0x96d3b8u);
    S(float, 0x20) = (float)f0;
L14ee:
    a = (s32)SB(c + 0x21u);
    S(u32, -4) = a;
    a = cm_fn_0090cfc0((s32)S(u32, -4));
    S(u32, -8) = 20;
    b = a;
    a = cm_fn_0090cfc0((s32)S(u32, -8));
    take = (s32)b >= (s32)a;
    if (take) goto L1516;
    f0 = S(float, 0x20);
    f0 *= F(0x96d248u);
    S(float, 0x20) = (float)f0;
L1516:
    b = S(u32, 0x24);
L151a:
    f0 = F(record + 0xd1u);
    f0 += F(record + 0xcdu);
    S(u32, -4) = 0;
    c = record;
    f0 += F(record + 0xa5u);
    f0 += F(0x969560u);
    f0 *= F(0x968f18u);
    f0 += F(0x969560u);
    f1 = F(record + 0x4du);
    f1 *= D(0x967898u);
    f0 += f1;
    f0 -= D(0x9688a8u);
    S(float, 0x2c) = (float)f0;
    f0 = F(record + 0x4du);
    S(float, 0x24) = (float)f0;
    native_result = cm_fn_007165b0((void *)c, S(u32, -4));
    a = (u32)native_result;
    c = (s32)(s8)(u8)a;
    S(u32, 0x1c) = c;
    f0 = S(s32, 0x1c);
    take = ((u8)b & 1u) == 0;
    f0 += F(record + 0x75u);
    f0 += F(0x969560u);
    f0 *= F(0x968f18u);
    f0 += F(0x969560u);
    f1 = S(float, 0x24);
    f1 *= D(0x967898u);
    f0 += f1;
    f0 -= D(0x9688a8u);
    f1 = F(record + 0xadu);
    f1 += F(record + 0xcdu);
    f2 = F(record + 0x4du);
    f2 *= D(0x967898u);
    f3 = f1;
    f3 += F(record + 0xa9u);
    f3 += F(0x969560u);
    f3 *= F(0x968f18u);
    f3 += F(0x969560u);
    f3 += f2;
    f3 -= D(0x9688a8u);
    S(float, 0x1c) = (float)f3;
    exchange = f2;
    f2 = f1;
    f1 = exchange;
    f2 += F(record + 0xa5u);
    f2 += F(0x969560u);
    f2 *= F(0x968f18u);
    f2 += F(0x969560u);
    f2 += f1;
    f2 -= D(0x9688a8u);
    S(float, 0x24) = (float)f2;
    if (take) goto L1687;
    W(record + 0x35u) = W(record + 0x35u) + (u32)-3;
    c = (c & 0xffff0000u) | (u16)W(record + 0x35u);
    d = (s32)(s16)(u16)c;
    S(u32, 0x1c) = d;
    f1 = S(s32, 0x1c);
    fp_above = f1 > F(0x96d35cu);
    take = !fp_above;
    if (take) goto L1627;
    c = c + (u32)-5;
    goto L162a;
L1627:
    c = c + 5u;
L162a:
    f1 = S(float, 0x14);
    f1 *= D(0x96d330u);
    W(record + 0x35u) = (u16)c;
    /* Original __ftol physical site 006c1638. */
    converted = (long long)f1;
    a = (u32)converted;
    f1 = S(float, 0x2c);
    W(object + side*2u + 0x1dau) = W(object + side*2u + 0x1dau) + (u16)a;
    f1 += f1;
    /* Original __ftol physical site 006c164b. */
    converted = (long long)f1;
    a = (u32)converted;
    f0 *= D(0x967880u);
    W(object + side*2u + 0x1e6u) = W(object + side*2u + 0x1e6u) + (u16)a;
    /* Original __ftol physical site 006c165e. */
    converted = (long long)f0;
    a = (u32)converted;
    f0 = S(float, 0x24);
    f0 *= D(0x96d330u);
    W(object + side*2u + 0x1eau) = W(object + side*2u + 0x1eau) + (u16)a;
    /* Original __ftol physical site 006c1675. */
    converted = (long long)f0;
    a = (u32)converted;
    W(object + side*2u + 0x1f2u) = W(object + side*2u + 0x1f2u) + (u16)a;
    goto L1aa2;
L1687:
    take = ((u8)b & 2u) == 0;
    if (take) goto L175f;
    W(record + 0x35u) = W(record + 0x35u) + (u32)-13;
    c = (c & 0xffff0000u) | (u16)W(record + 0x35u);
    a = (s32)(s16)(u16)c;
    S(u32, 0x18) = a;
    f1 = S(s32, 0x18);
    fp_above = f1 > F(0x96d358u);
    take = !fp_above;
    if (take) goto L16b6;
    c = c + (u32)-5;
    goto L16b9;
L16b6:
    c = c + 5u;
L16b9:
    f1 = S(float, 0x14);
    f1 *= D(0x9689b8u);
    W(record + 0x35u) = (u16)c;
    /* Original __ftol physical site 006c16c7. */
    converted = (long long)f1;
    a = (u32)converted;
    f1 = S(float, 0x10);
    f1 *= D(0x968918u);
    W(object + side*2u + 0x1dau) = W(object + side*2u + 0x1dau) + (u16)a;
    /* Original __ftol physical site 006c16de. */
    converted = (long long)f1;
    a = (u32)converted;
    f1 = S(float, 0x20);
    f1 *= D(0x967880u);
    W(object + side*2u + 0x1deu) = W(object + side*2u + 0x1deu) + (u16)a;
    /* Original __ftol physical site 006c16f5. */
    converted = (long long)f1;
    a = (u32)converted;
    f1 = S(float, 0x2c);
    W(object + side*2u + 0x1e2u) = W(object + side*2u + 0x1e2u) + (u16)a;
    f1 *= D(0x9689e0u);
L170c:
    /* Original __ftol physical site 006c170c. */
    converted = (long long)f1;
    a = (u32)converted;
    f0 *= D(0x968968u);
    W(object + side*2u + 0x1e6u) = W(object + side*2u + 0x1e6u) + (u16)a;
    /* Original __ftol physical site 006c171f. */
    converted = (long long)f0;
    a = (u32)converted;
    f0 = S(float, 0x1c);
    f0 *= D(0x969040u);
    W(object + side*2u + 0x1eau) = W(object + side*2u + 0x1eau) + (u16)a;
    /* Original __ftol physical site 006c1736. */
    converted = (long long)f0;
    a = (u32)converted;
    f0 = S(float, 0x24);
    f0 *= D(0x968968u);
    W(object + side*2u + 0x1eeu) = W(object + side*2u + 0x1eeu) + (u16)a;
    /* Original __ftol physical site 006c174d. */
    converted = (long long)f0;
    a = (u32)converted;
    W(object + side*2u + 0x1f2u) = W(object + side*2u + 0x1f2u) + (u16)a;
    goto L1aa2;
L175f:
    take = ((u8)b & 4u) == 0;
    if (take) goto L17df;
    W(record + 0x35u) = W(record + 0x35u) + (u32)-13;
    c = (c & 0xffff0000u) | (u16)W(record + 0x35u);
    d = (s32)(s16)(u16)c;
    S(u32, 0x18) = d;
    f1 = S(s32, 0x18);
    fp_above = f1 > F(0x96d358u);
    take = !fp_above;
    if (take) goto L178a;
    c = c + (u32)-5;
    goto L178d;
L178a:
    c = c + 5u;
L178d:
    f1 = S(float, 0x14);
    W(record + 0x35u) = (u16)c;
    /* Original __ftol physical site 006c1795. */
    converted = (long long)f1;
    a = (u32)converted;
    f1 = S(float, 0x10);
    f1 *= D(0x968f20u);
    W(object + side*2u + 0x1dau) = W(object + side*2u + 0x1dau) + (u16)a;
    /* Original __ftol physical site 006c17ac. */
    converted = (long long)f1;
    a = (u32)converted;
    f1 = S(float, 0x20);
    f1 *= D(0x967880u);
    W(object + side*2u + 0x1deu) = W(object + side*2u + 0x1deu) + (u16)a;
    /* Original __ftol physical site 006c17c3. */
    converted = (long long)f1;
    a = (u32)converted;
    f1 = S(float, 0x2c);
    W(object + side*2u + 0x1e2u) = W(object + side*2u + 0x1e2u) + (u16)a;
    f1 *= D(0x968918u);
    goto L170c;
L17df:
    take = ((u8)b & 8u) == 0;
    if (take) goto L1869;
    W(record + 0x35u) = W(record + 0x35u) + (u32)-12;
    c = (c & 0xffff0000u) | (u16)W(record + 0x35u);
    a = (s32)(s16)(u16)c;
    S(u32, 0x18) = a;
    f1 = S(s32, 0x18);
    fp_above = f1 > F(0x96d354u);
    take = !fp_above;
    if (take) goto L180e;
    c = c + (u32)-5;
    goto L1811;
L180e:
    c = c + 5u;
L1811:
    f1 = S(float, 0x14);
    f1 *= D(0x969520u);
    W(record + 0x35u) = (u16)c;
    /* Original __ftol physical site 006c181f. */
    converted = (long long)f1;
    a = (u32)converted;
    f1 = S(float, 0x10);
    f1 *= D(0x96d328u);
    W(object + side*2u + 0x1dau) = W(object + side*2u + 0x1dau) + (u16)a;
    /* Original __ftol physical site 006c1836. */
    converted = (long long)f1;
    a = (u32)converted;
    f1 = S(float, 0x20);
    f1 *= D(0x96d340u);
    W(object + side*2u + 0x1deu) = W(object + side*2u + 0x1deu) + (u16)a;
    /* Original __ftol physical site 006c184d. */
    converted = (long long)f1;
    a = (u32)converted;
    f1 = S(float, 0x2c);
    W(object + side*2u + 0x1e2u) = W(object + side*2u + 0x1e2u) + (u16)a;
    f1 *= D(0x968f20u);
    goto L170c;
L1869:
    take = ((u8)b & 0x10u) == 0;
    if (take) goto L18f3;
    W(record + 0x35u) = W(record + 0x35u) + (u32)-8;
    c = (c & 0xffff0000u) | (u16)W(record + 0x35u);
    d = (s32)(s16)(u16)c;
    S(u32, 0x18) = d;
    f1 = S(s32, 0x18);
    fp_above = f1 > F(0x96d350u);
    take = !fp_above;
    if (take) goto L1898;
    c = c + (u32)-5;
    goto L189b;
L1898:
    c = c + 5u;
L189b:
    f1 = S(float, 0x14);
    f1 *= D(0x968e00u);
    W(record + 0x35u) = (u16)c;
    /* Original __ftol physical site 006c18a9. */
    converted = (long long)f1;
    a = (u32)converted;
    f1 = S(float, 0x10);
    f1 *= D(0x969040u);
    W(object + side*2u + 0x1dau) = W(object + side*2u + 0x1dau) + (u16)a;
    /* Original __ftol physical site 006c18c0. */
    converted = (long long)f1;
    a = (u32)converted;
    f1 = S(float, 0x20);
    f1 *= D(0x96d338u);
    W(object + side*2u + 0x1deu) = W(object + side*2u + 0x1deu) + (u16)a;
    /* Original __ftol physical site 006c18d7. */
    converted = (long long)f1;
    a = (u32)converted;
    f1 = S(float, 0x2c);
    W(object + side*2u + 0x1e2u) = W(object + side*2u + 0x1e2u) + (u16)a;
    f1 *= D(0x967888u);
    goto L170c;
L18f3:
    take = ((u8)b & 0x20u) == 0;
    if (take) goto L19cb;
    W(record + 0x35u) = W(record + 0x35u) + (u32)-11;
    c = (c & 0xffff0000u) | (u16)W(record + 0x35u);
    a = (s32)(s16)(u16)c;
    S(u32, 0x18) = a;
    f1 = S(s32, 0x18);
    fp_above = f1 > F(0x96d34cu);
    take = !fp_above;
    if (take) goto L1922;
    c = c + (u32)-5;
    goto L1925;
L1922:
    c = c + 5u;
L1925:
    f1 = S(float, 0x14);
    f1 *= D(0x968f20u);
    W(record + 0x35u) = (u16)c;
    /* Original __ftol physical site 006c1933. */
    converted = (long long)f1;
    a = (u32)converted;
    f1 = S(float, 0x10);
    f1 *= D(0x968978u);
    W(object + side*2u + 0x1dau) = W(object + side*2u + 0x1dau) + (u16)a;
    /* Original __ftol physical site 006c194a. */
    converted = (long long)f1;
    a = (u32)converted;
    f1 = S(float, 0x20);
    f1 *= D(0x9689d0u);
    W(object + side*2u + 0x1deu) = W(object + side*2u + 0x1deu) + (u16)a;
    /* Original __ftol physical site 006c1961. */
    converted = (long long)f1;
    a = (u32)converted;
    f1 = S(float, 0x2c);
    f1 *= D(0x968a98u);
    W(object + side*2u + 0x1e2u) = W(object + side*2u + 0x1e2u) + (u16)a;
    /* Original __ftol physical site 006c1978. */
    converted = (long long)f1;
    a = (u32)converted;
    f0 *= D(0x968968u);
    W(object + side*2u + 0x1e6u) = W(object + side*2u + 0x1e6u) + (u16)a;
    /* Original __ftol physical site 006c198b. */
    converted = (long long)f0;
    a = (u32)converted;
    f0 = S(float, 0x1c);
    f0 *= D(0x969040u);
    W(object + side*2u + 0x1eau) = W(object + side*2u + 0x1eau) + (u16)a;
    /* Original __ftol physical site 006c19a2. */
    converted = (long long)f0;
    a = (u32)converted;
    f0 = S(float, 0x24);
    f0 *= D(0x969510u);
    W(object + side*2u + 0x1eeu) = W(object + side*2u + 0x1eeu) + (u16)a;
    /* Original __ftol physical site 006c19b9. */
    converted = (long long)f0;
    a = (u32)converted;
    W(object + side*2u + 0x1f2u) = W(object + side*2u + 0x1f2u) + (u16)a;
    goto L1aa2;
L19cb:
    take = ((u8)b & 0x40u) == 0;
    if (take) goto L1aa0;
    W(record + 0x35u) = W(record + 0x35u) + (u32)-25;
    c = (c & 0xffff0000u) | (u16)W(record + 0x35u);
    d = (s32)(s16)(u16)c;
    S(u32, 0x18) = d;
    f1 = S(s32, 0x18);
    fp_above = f1 > F(0x96d348u);
    take = !fp_above;
    if (take) goto L19fa;
    c = c + (u32)-5;
    goto L19fd;
L19fa:
    c = c + 5u;
L19fd:
    f1 = S(float, 0x14);
    f1 *= D(0x967888u);
    W(record + 0x35u) = (u16)c;
    /* Original __ftol physical site 006c1a0b. */
    converted = (long long)f1;
    a = (u32)converted;
    f1 = S(float, 0x10);
    f1 *= D(0x968f88u);
    W(object + side*2u + 0x1dau) = W(object + side*2u + 0x1dau) + (u16)a;
    /* Original __ftol physical site 006c1a22. */
    converted = (long long)f1;
    a = (u32)converted;
    f1 = S(float, 0x20);
    f1 *= D(0x968940u);
    W(object + side*2u + 0x1deu) = W(object + side*2u + 0x1deu) + (u16)a;
    /* Original __ftol physical site 006c1a39. */
    converted = (long long)f1;
    a = (u32)converted;
    f1 = S(float, 0x2c);
    f1 *= D(0x968a98u);
    W(object + side*2u + 0x1e2u) = W(object + side*2u + 0x1e2u) + (u16)a;
    /* Original __ftol physical site 006c1a50. */
    converted = (long long)f1;
    a = (u32)converted;
    f0 *= D(0x968968u);
    W(object + side*2u + 0x1e6u) = W(object + side*2u + 0x1e6u) + (u16)a;
    /* Original __ftol physical site 006c1a63. */
    converted = (long long)f0;
    a = (u32)converted;
    f0 = S(float, 0x1c);
    f0 *= D(0x968940u);
    W(object + side*2u + 0x1eau) = W(object + side*2u + 0x1eau) + (u16)a;
    /* Original __ftol physical site 006c1a7a. */
    converted = (long long)f0;
    a = (u32)converted;
    f0 = S(float, 0x24);
    f0 *= D(0x968918u);
    W(object + side*2u + 0x1eeu) = W(object + side*2u + 0x1eeu) + (u16)a;
    /* Original __ftol physical site 006c1a91. */
    converted = (long long)f0;
    a = (u32)converted;
    W(object + side*2u + 0x1f2u) = W(object + side*2u + 0x1f2u) + (u16)a;
    goto L1aa2;
L1aa0:
L1aa2:
    a = (u8)S(u8, 0x2b);
    record = record + 0x1beu;
    a = (u8)((u8)a + 1u);
    take = (s8)(u8)a < 20;
    S(u8, 0x2b) = (u8)a;
    if (take) goto L0ccb;
    a = I(object + 0x4792u);
    take = (a & a) == 0;
    if (take) goto L1c32;
    c = I(a + 0x1cu);
    take = (c & c) == 0;
    if (take) goto L1c32;
    a = I(a + 0x14u);
    take = (a & a) == 0;
    if (take) goto L1c32;
    c = I(c + 0x53u);
    d = I(a + 0x5du);
    take = d != c;
    if (take) goto L1c32;
    take = (c & c) == 0;
    if (take) goto L1c32;
    a = I(c);
    c = I(0x9cf314u);
    take = a == c;
    if (take) goto L1c10;
    take = a == I(0x9cf45cu);
    if (take) goto L1c10;
    take = a == I(0x9cf244u);
    if (take) goto L1c10;
    take = a != I(0x9cf4a8u);
    if (take) goto L1bb2;
    c = (s32)SW(object + 0x1deu);
    S(u32, 0x18) = c;
    f0 = S(s32, 0x18);
    f0 *= F(0x96b010u);
    goto L1c25;
L1b42:
    a = (u32)(storage.bytes + 0x30 + 0x30);
    c = (u32)(storage.bytes + 0x30 + 0x130);
    S(u32, -4) = a;
    S(u32, -8) = c;
    S(u32, -12) = 0;
    S(u32, -16) = 0;
    S(u32, -20) = 0xa15884u;
    (void)cm_fn_00944cff(S(u32, -20), S(u32, -16), S(u32, -12), (void *)S(u32, -8), (void *)S(u32, -4));
    d = (u32)(storage.bytes + 0x30 + 0x30);
    a = (u32)(storage.bytes + 0x30 + 0x130);
    S(u32, -4) = 0x1860u;
    S(u32, -8) = d;
    S(u32, -12) = a;
    a = cm_fn_0090d130();
    S(u32, -16) = a;
    S(u32, -20) = 0x9870e8u;
    S(u32, -24) = 0xae24d0u;
    a = cm_fn_00944c9f(S(u32, -24), S(u32, -20), S(u32, -16), S(u32, -12), S(u32, -8), S(u32, -4));
    S(u32, -28) = 0;
    S(u32, -32) = 0xae24d0u;
    S(u32, -36) = 0x9870e0u;
    a = cm_fn_005e8290(S(u32, -36), S(u32, -32), S(u32, -28));
    I(0xb67a34u) = 0;
    return a;
L1bb2:
    take = a == I(0x9cf408u);
    if (take) goto L1bf9;
    take = a == I(0x9cf2e4u);
    if (take) goto L1bf9;
    take = a == I(0x9cf338u);
    if (take) goto L1be2;
    take = a == I(0x9cf2c0u);
    if (take) goto L1be2;
    take = a == I(0x9cf434u);
    if (take) goto L1be2;
    take = a != I(0x9cf300u);
    if (take) goto L1c32;
L1be2:
    d = (s32)SW(object + 0x1deu);
    S(u32, 0x18) = d;
    f0 = S(s32, 0x18);
    f0 *= F(0x96a33cu);
    goto L1c25;
L1bf9:
    a = (s32)SW(object + 0x1deu);
    S(u32, 0x18) = a;
    f0 = S(s32, 0x18);
    f0 *= F(0x96d3b8u);
    goto L1c25;
L1c10:
    c = (s32)SW(object + 0x1deu);
    S(u32, 0x18) = c;
    f0 = S(s32, 0x18);
    f0 *= F(0x96d390u);
L1c25:
    /* Original __ftol physical site 006c1c25. */
    converted = (long long)f0;
    a = (u32)converted;
    W(object + side*2u + 0x1deu) = (u16)a;
L1c32:
    d = (s32)SW(object + side*2u + 0x1deu);
    S(u32, 0x18) = d;
    f0 = S(s32, 0x18);
    f0 *= F(0x96d3b8u);
    /* Original __ftol physical site 006c1c48. */
    converted = (long long)f0;
    a = (u32)converted;
    W(object + side*2u + 0x1deu) = (u16)a;
    a = (s32)SW(object + side*2u + 0x1e2u);
    S(u32, 0x18) = a;
    f0 = S(s32, 0x18);
    f0 *= F(0x96d3b0u);
    /* Original __ftol physical site 006c1c6b. */
    converted = (long long)f0;
    a = (u32)converted;
    W(object + side*2u + 0x1e2u) = (u16)a;
    c = I(object + 0x4792u);
    a = I(c + 0x14u);
    take = (a & a) == 0;
    if (take) goto L1c99;
    c = I(0xae2c74u);
    take = (c & c) == 0;
    if (take) goto L1cbb;
    S(u32, -4) = a;
    native_result = cm_fn_005c6710((void *)c, S(u32, -4));
    a = (u32)native_result;
    take = (a & a) == 0;
    if (take) goto L1cbb;
L1c99:
    d = (s32)SW(object + 0x1deu);
    S(u32, 0x18) = d;
    f0 = S(s32, 0x18);
    f0 *= F(0x96a33cu);
    /* Original __ftol physical site 006c1cae. */
    converted = (long long)f0;
    a = (u32)converted;
    W(object + side*2u + 0x1deu) = (u16)a;
L1cbb:
    a = I(object + 0x4792u);
    c = I(a + 0x1cu);
    take = (c & c) == 0;
    if (take) goto L1d20;
    d = I(a + 0x20u);
    take = (d & d) == 0;
    if (take) goto L1d20;
    take = SW(c + 0x80u) >= (s16)0x9c4u;
    if (take) goto L1d20;
    a = (s32)SW(object + side*2u + 0x1deu);
    S(u32, 0x18) = a;
    f0 = S(s32, 0x18);
    f0 *= F(0x96d368u);
    /* Original __ftol physical site 006c1cf0. */
    converted = (long long)f0;
    a = (u32)converted;
    W(object + side*2u + 0x1deu) = (u16)a;
    c = (s32)SW(object + side*2u + 0x1e2u);
    S(u32, 0x18) = c;
    f0 = S(s32, 0x18);
    f0 *= F(0x96d390u);
    /* Original __ftol physical site 006c1d13. */
    converted = (long long)f0;
    a = (u32)converted;
    W(object + side*2u + 0x1e2u) = (u16)a;
L1d20:
    record = I(object + 0x4792u);
    tail_side = (u8)arg;
    take = (record & record) == 0;
    if (take) goto L1e7c;
    take = (tail_side & tail_side) != 0;
    if (take) goto L1e7c;
    a = I(record + 0x1cu);
    take = (a & a) == 0;
    if (take) goto L1e7c;
    c = I(record + 0x20u);
    take = (c & c) == 0;
    if (take) goto L1e7c;
    a = (u16)W(a + 0x80u);
    a = (u16)((u16)a - W(c + 0x80u));
    take = (s16)(u16)a <= 1000;
    if (take) goto L1dcf;
    d = (s32)SW(object + 0x1deu);
    S(u32, 0x18) = d;
    f0 = S(s32, 0x18);
    f0 *= D(0x968f88u);
    /* Original __ftol physical site 006c1d7c. */
    converted = (long long)f0;
    a = (u32)converted;
    W(object + 0x1deu) = (u16)a;
    a = (s32)SW(object + 0x1e2u);
    S(u32, 0x18) = a;
    f0 = S(s32, 0x18);
    f0 *= D(0x968f88u);
    /* Original __ftol physical site 006c1d9d. */
    converted = (long long)f0;
    a = (u32)converted;
    c = (s32)SW(object + 0x1dau);
    S(u32, 0x18) = c;
    W(object + 0x1e2u) = (u16)a;
    f0 = S(s32, 0x18);
    f0 *= D(0x968f88u);
    /* Original __ftol physical site 006c1dbe. */
    converted = (long long)f0;
    a = (u32)converted;
    W(object + 0x1dau) = (u16)a;
    goto L1e7c;
L1dcf:
    take = (s16)(u16)a <= (s16)0xff06u;
    if (take) goto L1e3a;
    d = (s32)SW(object + 0x1deu);
    S(u32, 0x18) = d;
    f0 = S(s32, 0x18);
    f0 *= D(0x968f88u);
    /* Original __ftol physical site 006c1dea. */
    converted = (long long)f0;
    a = (u32)converted;
    W(object + 0x1deu) = (u16)a;
    a = (s32)SW(object + 0x1e2u);
    S(u32, 0x18) = a;
    f0 = S(s32, 0x18);
    f0 *= D(0x96afa8u);
    /* Original __ftol physical site 006c1e0b. */
    converted = (long long)f0;
    a = (u32)converted;
    c = (s32)SW(object + 0x1dau);
    S(u32, 0x18) = c;
    W(object + 0x1e2u) = (u16)a;
    f0 = S(s32, 0x18);
    f0 *= D(0x96afa8u);
    /* Original __ftol physical site 006c1e2c. */
    converted = (long long)f0;
    a = (u32)converted;
    W(object + 0x1dau) = (u16)a;
    goto L1e7c;
L1e3a:
    d = (s32)SW(object + 0x1dau);
    S(u32, 0x18) = d;
    f0 = S(s32, 0x18);
    f0 *= D(0x968f88u);
    /* Original __ftol physical site 006c1e4f. */
    converted = (long long)f0;
    a = (u32)converted;
    W(object + 0x1dau) = (u16)a;
    a = (s32)SW(object + 0x1deu);
    S(u32, 0x18) = a;
    f0 = S(s32, 0x18);
    f0 *= D(0x96a6d8u);
    /* Original __ftol physical site 006c1e70. */
    converted = (long long)f0;
    a = (u32)converted;
    W(object + 0x1deu) = (u16)a;
L1e7c:
    take = (record & record) == 0;
    if (take) goto L2037;
    a = I(record + 0x1cu);
    take = (a & a) == 0;
    if (take) goto L2037;
    record = I(record + 0x20u);
    take = (record & record) == 0;
    if (take) goto L2037;
    take = (tail_side & tail_side) != 0;
    if (take) goto L1fd1;
    a = (a & 0xffff0000u) | (u16)W(a + 0x80u);
    a = (a & 0xffff0000u) | (u16)((u16)a - W(record + 0x80u));
    take = (s16)(u16)a <= 1250;
    if (take) goto L1f0f;
    a = (u8)B(object + 0xf5bdu);
    take = ((u8)a & (u8)a) != 0;
    if (take) goto L1ee1;
    c = (s32)SW(object + 0x1e2u);
    S(u32, 0x18) = c;
    f0 = S(s32, 0x18);
    f0 *= D(0x968f88u);
    /* Original __ftol physical site 006c1ed5. */
    converted = (long long)f0;
    a = (u32)converted;
    W(object + 0x1e2u) = (u16)a;
L1ee1:
    d = (s32)SW(object + 0x1dau);
    S(u32, 0x18) = d;
    f0 = S(s32, 0x18);
    f0 *= D(0x9689b8u);
    /* Original __ftol physical site 006c1ef6. */
    converted = (long long)f0;
    a = (u32)converted;
    W(object + 0x1dau) = (u16)a;
    return a;
L1f0f:
    take = (s16)(u16)a <= 750;
    if (take) goto L1f6e;
    a = (u8)B(object + 0xf5bdu);
    take = ((u8)a & (u8)a) != 0;
    if (take) goto L1f40;
    a = (s32)SW(object + 0x1e2u);
    S(u32, 0x18) = a;
    f0 = S(s32, 0x18);
    f0 *= D(0x968f88u);
    /* Original __ftol physical site 006c1f34. */
    converted = (long long)f0;
    a = (u32)converted;
    W(object + 0x1e2u) = (u16)a;
L1f40:
    c = (s32)SW(object + 0x1dau);
    S(u32, 0x18) = c;
    f0 = S(s32, 0x18);
    f0 *= D(0x969098u);
    /* Original __ftol physical site 006c1f55. */
    converted = (long long)f0;
    a = (u32)converted;
    W(object + 0x1dau) = (u16)a;
    return a;
L1f6e:
    take = (s16)(u16)a <= 250;
    if (take) goto L2037;
    a = (u8)B(object + 0xf5bdu);
    take = ((u8)a & (u8)a) != 0;
    if (take) goto L1fa3;
    d = (s32)SW(object + 0x1e2u);
    S(u32, 0x18) = d;
    f0 = S(s32, 0x18);
    f0 *= D(0x96afa8u);
    /* Original __ftol physical site 006c1f97. */
    converted = (long long)f0;
    a = (u32)converted;
    W(object + 0x1e2u) = (u16)a;
L1fa3:
    a = (s32)SW(object + 0x1dau);
    S(u32, 0x18) = a;
    f0 = S(s32, 0x18);
    f0 *= D(0x968f88u);
    /* Original __ftol physical site 006c1fb8. */
    converted = (long long)f0;
    a = (u32)converted;
    W(object + 0x1dau) = (u16)a;
    return a;
L1fd1:
    take = tail_side != 1u;
    if (take) goto L2037;
    c = (u16)W(a + 0x80u);
    c = (u16)((u16)c - W(record + 0x80u));
    take = (s16)(u16)c >= (s16)0xfd12u;
    if (take) goto L2037;
    a = (a & 0xffffff00u) | (u8)B(object + 0xf5f3u);
    take = ((u8)a & (u8)a) != 0;
    if (take) goto L2037;
    d = (s32)SW(object + 0x1e4u);
    S(u32, 0x18) = d;
    f0 = S(s32, 0x18);
    f0 *= D(0x968f88u);
    /* Original __ftol physical site 006c200a. */
    converted = (long long)f0;
    a = (u32)converted;
    W(object + 0x1e4u) = (u16)a;
    a = (s32)SW(object + 0x1dcu);
    S(u32, 0x18) = a;
    f0 = S(s32, 0x18);
    f0 *= D(0x96afa8u);
    /* Original __ftol physical site 006c202b. */
    converted = (long long)f0;
    a = (u32)converted;
    W(object + 0x1dcu) = (u16)a;
L2037:
    return a;
}
#undef B
#undef W
#undef I
#undef SB
#undef SW
#undef F
#undef D
#undef M
#undef S

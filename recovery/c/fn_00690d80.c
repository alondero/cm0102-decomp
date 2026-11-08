/* CM0102 3.9.68; preferred-image VA 00690d80, image base 00400000.
   Target SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The original has a 0xa2c-byte uninitialized allocation. S is ESP after the
   four saved registers: L=S+16 is its base. Address expressions retain the
   original live argument PUSH adjustments. Native callees remain boundaries.
   Every finite jump-table read retains its original address and guard.
   Original EAX is retained at all exits, including unknown-class exits.
   Four stack dwords are popped; the fourth is unused by the instructions. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_THISCALL cm_fn_00690bd0(u32 self,u32 club);
extern u32 CM_THISCALL cm_fn_00690b70(u32 self,u32 club);
extern void CM_STDCALL cm_fn_00690c30(u32 club,u32 first,u32 second);
extern u32 CM_CDECL cm_fn_0053b900(u32 *club);
extern u32 CM_THISCALL cm_fn_005a0590(u32 self,u32 club);
extern u8 CM_THISCALL cm_fn_005986b0(u32 self);
extern unsigned long long CM_CDECL cm_fn_006181d0(u32 object,u32 output,u32 capacity,u32 selector);
extern u32 CM_CDECL cm_fn_0066f4e0(u32 output,u32 format,...);
extern s32 CM_THISCALL cm_fn_0054cdb0(u32 self,u32 date_low,u32 date_high);
extern u32 CM_CDECL cm_fn_00944c9f(u32 output,u32 format,...);

u32 CM_THISCALL cm_fn_00690d80(u32 self,u32 club,s32 age,u32 output,u32 unused4)
{
    u8 scratch[0xa2c];
    u32 S=(u32)scratch-16u;
    u32 eax,ecx=self,edx,esi=club,edi=output,ebp=self;
    u8 rating;
    u32 p0,p1,p2,p3,p4,p5,target;
    u8 zero,less,greater,above;
    (void)unused4;

    p0=esi;
    B(edi)=0u;
    B((S-4u) + 0x658u)=0u;
    /* Native 00690da6: call 0x690bd0. */
    eax=(u32)cm_fn_00690bd0(ecx,p0);
    p0=esi;
    ecx=ebp;
    B((S-4u) + 0x17u)=((u8)eax);
    /* Native 00690db2: call 0x690b70. */
    eax=(u32)cm_fn_00690b70(ecx,p0);
    rating=(u8)(((u8)eax));
    eax=(S-0u) + 0x12u;
    ecx=(S-0u) + 0x11u;
    p0=eax;
    p1=ecx;
    p2=esi;
    ecx=ebp;
    /* Native 00690dc6: call 0x690c30. */
    cm_fn_00690c30(p2,p1,p0);
    p0=esi;
    /* Native 00690dcc: call 0x53b900. */
    eax=(u32)cm_fn_0053b900((u32 *)p0);
    zero=((u32)(eax)&(u32)(eax))==0;
    if (!zero) goto block_00693189;
    ecx=D(0xae2a38u);
    p0=esi;
    /* Native 00690de3: call 0x5a0590. */
    eax=(u32)cm_fn_005a0590(ecx,p0);
    ecx=eax;
    /* Native 00690dea: call 0x5986b0. */
    eax=(eax&0xffffff00u)|cm_fn_005986b0(ecx);
    ecx=(u32)age;
    less=(s32)(ecx)<(s32)(0x1eu);
    if (!less) goto block_00690ece;
    eax=(u32)(s32)(s8)(((u8)eax));
    eax=eax+2u;
    above=(u32)(eax)>(u32)(4u);
    if (above) goto block_006933f9;
    target=D(eax*4u + 0x693408u);
    switch(target) {
    case 0x00690e15u: goto block_00690e15;
    case 0x00690e3au: goto block_00690e3a;
    case 0x00690e5fu: goto block_00690e5f;
    case 0x00690e84u: goto block_00690e84;
    case 0x00690ea9u: goto block_00690ea9;
    /* Guarded immutable original table: all entries enumerated. */
    default: __builtin_unreachable();
    }
block_00690e15:
    /* Native 00690e15: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0xa018bcu;
    p4=edi;
    /* Native 00690e25: call 0x66f4e0. */
    eax=(u32)cm_fn_0066f4e0(p4,p3,p2,p1,p0);
    return eax;
block_00690e3a:
    /* Native 00690e3a: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0xa01818u;
    p4=edi;
    /* Native 00690e4a: call 0x66f4e0. */
    eax=(u32)cm_fn_0066f4e0(p4,p3,p2,p1,p0);
    return eax;
block_00690e5f:
    /* Native 00690e5f: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0xa0179cu;
    p4=edi;
    /* Native 00690e6f: call 0x66f4e0. */
    eax=(u32)cm_fn_0066f4e0(p4,p3,p2,p1,p0);
    return eax;
block_00690e84:
    /* Native 00690e84: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0xa016c8u;
    p4=edi;
    /* Native 00690e94: call 0x66f4e0. */
    eax=(u32)cm_fn_0066f4e0(p4,p3,p2,p1,p0);
    return eax;
block_00690ea9:
    /* Native 00690ea9: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0xa0160cu;
    p4=edi;
    /* Native 00690eb9: call 0x66f4e0. */
    eax=(u32)cm_fn_0066f4e0(p4,p3,p2,p1,p0);
    return eax;
block_00690ece:
    /* Native 00690ece: cmp ecx, 0x3c. */
    less=(s32)(ecx)<(s32)(0x3cu);
    eax=(u32)(s32)(s8)(((u8)eax));
    if (!less) goto block_006914c0;
    eax=eax+2u;
    above=(u32)(eax)>(u32)(4u);
    if (above) goto block_006933f9;
    target=D(eax*4u + 0x69341cu);
    switch(target) {
    case 0x00690eedu: goto block_00690eed;
    case 0x00691051u: goto block_00691051;
    case 0x00691140u: goto block_00691140;
    case 0x0069123cu: goto block_0069123c;
    case 0x00691338u: goto block_00691338;
    /* Guarded immutable original table: all entries enumerated. */
    default: __builtin_unreachable();
    }
block_00690eed:
    /* Native 00690eed: movsx eax, bl. */
    eax=(u32)(s32)(s8)(rating);
    above=(u32)(eax)>(u32)(6u);
    if (above) goto block_006933f9;
    target=D(eax*4u + 0x693430u);
    switch(target) {
    case 0x00690f00u: goto block_00690f00;
    case 0x00690f31u: goto block_00690f31;
    case 0x00690f62u: goto block_00690f62;
    case 0x00690f87u: goto block_00690f87;
    case 0x00690fc0u: goto block_00690fc0;
    case 0x00690ffbu: goto block_00690ffb;
    case 0x0069102cu: goto block_0069102c;
    /* Guarded immutable original table: all entries enumerated. */
    default: __builtin_unreachable();
    }
block_00690f00:
    /* Native 00690f00: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    if (zero) goto block_00691030;
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0xa01558u;
    p4=edi;
    /* Native 00690f1c: call 0x66f4e0. */
    eax=(u32)cm_fn_0066f4e0(p4,p3,p2,p1,p0);
    return eax;
block_00690f31:
    /* Native 00690f31: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    if (zero) goto block_00691030;
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0xa01494u;
    p4=edi;
    /* Native 00690f4d: call 0x66f4e0. */
    eax=(u32)cm_fn_0066f4e0(p4,p3,p2,p1,p0);
    return eax;
block_00690f62:
    /* Native 00690f62: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0xa013f4u;
    p4=edi;
    /* Native 00690f72: call 0x66f4e0. */
    eax=(u32)cm_fn_0066f4e0(p4,p3,p2,p1,p0);
    return eax;
block_00690f87:
    /* Native 00690f87: mov al, byte ptr [esp + 0x11]. */
    eax=(eax&0xffffff00u)|(u8)(B((S-0u) + 0x11u));
    zero=((u8)(((u8)eax))&(u8)(((u8)eax)))==0;
    less=(s8)((u8)(((u8)eax))&(u8)(((u8)eax)))<0;
    greater=!zero&&!less;
    if (!greater) goto block_0069102c;
    greater=(s8)(((u8)eax))>(s8)(2u);
    if (greater) goto block_0069102c;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0xa01360u;
    p4=edi;
    /* Native 00690fab: call 0x66f4e0. */
    eax=(u32)cm_fn_0066f4e0(p4,p3,p2,p1,p0);
    return eax;
block_00690fc0:
    /* Native 00690fc0: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    above=(u32)(eax)>(u32)(4u);
    if (above) goto block_006933f9;
    target=D(eax*4u + 0x69344cu);
    switch(target) {
    case 0x00690fd6u: goto block_00690fd6;
    case 0x0069102cu: goto block_0069102c;
    /* Guarded immutable original table: all entries enumerated. */
    default: __builtin_unreachable();
    }
block_00690fd6:
    /* Native 00690fd6: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0xa012c8u;
    p4=edi;
    /* Native 00690fe6: call 0x66f4e0. */
    eax=(u32)cm_fn_0066f4e0(p4,p3,p2,p1,p0);
    return eax;
block_00690ffb:
    /* Native 00690ffb: mov al, byte ptr [esp + 0x11]. */
    eax=(eax&0xffffff00u)|(u8)(B((S-0u) + 0x11u));
    zero=((u8)(((u8)eax))&(u8)(((u8)eax)))==0;
    less=(s8)((u8)(((u8)eax))&(u8)(((u8)eax)))<0;
    greater=!zero&&!less;
    if (!greater) goto block_0069102c;
    greater=(s8)(((u8)eax))>(s8)(2u);
    if (greater) goto block_0069102c;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0xa01230u;
    p4=edi;
    /* Native 00691017: call 0x66f4e0. */
    eax=(u32)cm_fn_0066f4e0(p4,p3,p2,p1,p0);
    return eax;
block_0069102c:
    /* Native 0069102c: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
block_00691030:
    /* Native 00691030: add esi, 4. */
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0xa01194u;
block_0069103b:
    /* Native 0069103b: push edi. */
    p4=edi;
    /* Native 0069103c: call 0x66f4e0. */
    eax=(u32)cm_fn_0066f4e0(p4,p3,p2,p1,p0);
    return eax;
block_00691051:
    /* Native 00691051: movsx eax, bl. */
    eax=(u32)(s32)(s8)(rating);
    above=(u32)(eax)>(u32)(6u);
    if (above) goto block_006933f9;
    target=D(eax*4u + 0x693460u);
    switch(target) {
    case 0x00691064u: goto block_00691064;
    case 0x0069107du: goto block_0069107d;
    case 0x00691096u: goto block_00691096;
    case 0x006910a7u: goto block_006910a7;
    case 0x006910c7u: goto block_006910c7;
    case 0x006910d3u: goto block_006910d3;
    case 0x006910fbu: goto block_006910fb;
    /* Guarded immutable original table: all entries enumerated. */
    default: __builtin_unreachable();
    }
block_00691064:
    /* Native 00691064: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    if (zero) goto block_00691030;
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0xa010e4u;
    goto block_0069103b;
block_0069107d:
    /* Native 0069107d: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    if (zero) goto block_00691030;
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0xa01024u;
    goto block_0069103b;
block_00691096:
    /* Native 00691096: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0xa00f88u;
    goto block_0069103b;
block_006910a7:
    /* Native 006910a7: mov al, byte ptr [esp + 0x11]. */
    eax=(eax&0xffffff00u)|(u8)(B((S-0u) + 0x11u));
    zero=((u8)(((u8)eax))&(u8)(((u8)eax)))==0;
    less=(s8)((u8)(((u8)eax))&(u8)(((u8)eax)))<0;
    greater=!zero&&!less;
    if (!greater) goto block_006910d3;
    greater=(s8)(((u8)eax))>(s8)(2u);
    if (greater) goto block_006910d3;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0xa00ef8u;
    goto block_0069103b;
block_006910c7:
    /* Native 006910c7: mov al, byte ptr [esp + 0x11]. */
    eax=(eax&0xffffff00u)|(u8)(B((S-0u) + 0x11u));
    less=(s8)(((u8)eax))<(s8)(3u);
    if (less) goto block_006910e7;
    greater=(s8)(((u8)eax))>(s8)(5u);
    if (greater) goto block_006910e7;
block_006910d3:
    /* Native 006910d3: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0xa00e60u;
    goto block_0069103b;
block_006910e7:
    /* Native 006910e7: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0xa00dccu;
    goto block_0069103b;
block_006910fb:
    /* Native 006910fb: mov al, byte ptr [esp + 0x11]. */
    eax=(eax&0xffffff00u)|(u8)(B((S-0u) + 0x11u));
    less=(s8)(((u8)eax))<(s8)(3u);
    if (less) goto block_0069111b;
    greater=(s8)(((u8)eax))>(s8)(5u);
    if (greater) goto block_0069111b;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0xa00e60u;
    goto block_0069103b;
block_0069111b:
    /* Native 0069111b: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0xa00d38u;
    p4=edi;
    /* Native 0069112b: call 0x66f4e0. */
    eax=(u32)cm_fn_0066f4e0(p4,p3,p2,p1,p0);
    return eax;
block_00691140:
    /* Native 00691140: movsx eax, bl. */
    eax=(u32)(s32)(s8)(rating);
    above=(u32)(eax)>(u32)(6u);
    if (above) goto block_006933f9;
    target=D(eax*4u + 0x69347cu);
    switch(target) {
    case 0x00691153u: goto block_00691153;
    case 0x00691167u: goto block_00691167;
    case 0x0069117bu: goto block_0069117b;
    case 0x0069118fu: goto block_0069118f;
    case 0x006911c3u: goto block_006911c3;
    case 0x006911cfu: goto block_006911cf;
    case 0x006911f7u: goto block_006911f7;
    /* Guarded immutable original table: all entries enumerated. */
    default: __builtin_unreachable();
    }
block_00691153:
    /* Native 00691153: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0xa00cb0u;
    goto block_0069103b;
block_00691167:
    /* Native 00691167: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0xa00c18u;
    goto block_0069103b;
block_0069117b:
    /* Native 0069117b: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0xa00ba4u;
    goto block_0069103b;
block_0069118f:
    /* Native 0069118f: mov al, byte ptr [esp + 0x11]. */
    eax=(eax&0xffffff00u)|(u8)(B((S-0u) + 0x11u));
    less=(s8)(((u8)eax))<(s8)(3u);
    if (less) goto block_006911af;
    greater=(s8)(((u8)eax))>(s8)(5u);
    if (greater) goto block_006911af;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0xa00b34u;
    goto block_0069103b;
block_006911af:
    /* Native 006911af: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0xa00accu;
    goto block_0069103b;
block_006911c3:
    /* Native 006911c3: mov al, byte ptr [esp + 0x11]. */
    eax=(eax&0xffffff00u)|(u8)(B((S-0u) + 0x11u));
    less=(s8)(((u8)eax))<(s8)(3u);
    if (less) goto block_006911e3;
    greater=(s8)(((u8)eax))>(s8)(5u);
    if (greater) goto block_006911e3;
block_006911cf:
    /* Native 006911cf: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0xa00b34u;
    goto block_0069103b;
block_006911e3:
    /* Native 006911e3: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0xa00a60u;
    goto block_0069103b;
block_006911f7:
    /* Native 006911f7: mov al, byte ptr [esp + 0x11]. */
    eax=(eax&0xffffff00u)|(u8)(B((S-0u) + 0x11u));
    less=(s8)(((u8)eax))<(s8)(3u);
    if (less) goto block_00691217;
    greater=(s8)(((u8)eax))>(s8)(5u);
    if (greater) goto block_00691217;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0xa00b34u;
    goto block_0069103b;
block_00691217:
    /* Native 00691217: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0xa009f4u;
    p4=edi;
    /* Native 00691227: call 0x66f4e0. */
    eax=(u32)cm_fn_0066f4e0(p4,p3,p2,p1,p0);
    return eax;
block_0069123c:
    /* Native 0069123c: movsx eax, bl. */
    eax=(u32)(s32)(s8)(rating);
    above=(u32)(eax)>(u32)(6u);
    if (above) goto block_006933f9;
    target=D(eax*4u + 0x693498u);
    switch(target) {
    case 0x0069124fu: goto block_0069124f;
    case 0x00691263u: goto block_00691263;
    case 0x00691277u: goto block_00691277;
    case 0x0069128bu: goto block_0069128b;
    case 0x006912bfu: goto block_006912bf;
    case 0x006912cbu: goto block_006912cb;
    case 0x006912f3u: goto block_006912f3;
    /* Guarded immutable original table: all entries enumerated. */
    default: __builtin_unreachable();
    }
block_0069124f:
    /* Native 0069124f: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0xa00910u;
    goto block_0069103b;
block_00691263:
    /* Native 00691263: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0xa0081cu;
    goto block_0069103b;
block_00691277:
    /* Native 00691277: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0xa00750u;
    goto block_0069103b;
block_0069128b:
    /* Native 0069128b: mov al, byte ptr [esp + 0x11]. */
    eax=(eax&0xffffff00u)|(u8)(B((S-0u) + 0x11u));
    less=(s8)(((u8)eax))<(s8)(3u);
    if (less) goto block_006912ab;
    greater=(s8)(((u8)eax))>(s8)(5u);
    if (greater) goto block_006912ab;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0xa00684u;
    goto block_0069103b;
block_006912ab:
    /* Native 006912ab: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0xa005c0u;
    goto block_0069103b;
block_006912bf:
    /* Native 006912bf: mov al, byte ptr [esp + 0x11]. */
    eax=(eax&0xffffff00u)|(u8)(B((S-0u) + 0x11u));
    less=(s8)(((u8)eax))<(s8)(3u);
    if (less) goto block_006912df;
    greater=(s8)(((u8)eax))>(s8)(5u);
    if (greater) goto block_006912df;
block_006912cb:
    /* Native 006912cb: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0xa00684u;
    goto block_0069103b;
block_006912df:
    /* Native 006912df: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0xa004f8u;
    goto block_0069103b;
block_006912f3:
    /* Native 006912f3: mov al, byte ptr [esp + 0x11]. */
    eax=(eax&0xffffff00u)|(u8)(B((S-0u) + 0x11u));
    less=(s8)(((u8)eax))<(s8)(3u);
    if (less) goto block_00691313;
    greater=(s8)(((u8)eax))>(s8)(5u);
    if (greater) goto block_00691313;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0xa00684u;
    goto block_0069103b;
block_00691313:
    /* Native 00691313: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0xa00438u;
    p4=edi;
    /* Native 00691323: call 0x66f4e0. */
    eax=(u32)cm_fn_0066f4e0(p4,p3,p2,p1,p0);
    return eax;
block_00691338:
    /* Native 00691338: movsx eax, bl. */
    eax=(u32)(s32)(s8)(rating);
    above=(u32)(eax)>(u32)(6u);
    if (above) goto block_006933f9;
    target=D(eax*4u + 0x6934b4u);
    switch(target) {
    case 0x0069134bu: goto block_0069134b;
    case 0x00691370u: goto block_00691370;
    case 0x00691395u: goto block_00691395;
    case 0x006913bau: goto block_006913ba;
    case 0x006913f3u: goto block_006913f3;
    case 0x00691451u: goto block_00691451;
    case 0x00691465u: goto block_00691465;
    /* Guarded immutable original table: all entries enumerated. */
    default: __builtin_unreachable();
    }
block_0069134b:
    /* Native 0069134b: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0xa0036cu;
    p4=edi;
    /* Native 0069135b: call 0x66f4e0. */
    eax=(u32)cm_fn_0066f4e0(p4,p3,p2,p1,p0);
    return eax;
block_00691370:
    /* Native 00691370: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0xa00290u;
    p4=edi;
    /* Native 00691380: call 0x66f4e0. */
    eax=(u32)cm_fn_0066f4e0(p4,p3,p2,p1,p0);
    return eax;
block_00691395:
    /* Native 00691395: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0xa001dcu;
    p4=edi;
    /* Native 006913a5: call 0x66f4e0. */
    eax=(u32)cm_fn_0066f4e0(p4,p3,p2,p1,p0);
    return eax;
block_006913ba:
    /* Native 006913ba: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax+-2;
    above=(u32)(eax)>(u32)(3u);
    if (above) goto block_006913ce;
    target=D(eax*4u + 0x6934d0u);
    switch(target) {
    case 0x00691465u: goto block_00691465;
    case 0x0069148au: goto block_0069148a;
    /* Guarded immutable original table: all entries enumerated. */
    default: __builtin_unreachable();
    }
block_006913ce:
    /* Native 006913ce: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0xa00130u;
    p4=edi;
    /* Native 006913de: call 0x66f4e0. */
    eax=(u32)cm_fn_0066f4e0(p4,p3,p2,p1,p0);
    return eax;
block_006913f3:
    /* Native 006913f3: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax+-2;
    above=(u32)(eax)>(u32)(3u);
    if (above) goto block_0069142c;
    target=D(eax*4u + 0x6934e0u);
    switch(target) {
    case 0x00691407u: goto block_00691407;
    case 0x00691465u: goto block_00691465;
    /* Guarded immutable original table: all entries enumerated. */
    default: __builtin_unreachable();
    }
block_00691407:
    /* Native 00691407: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0xa00064u;
    p4=edi;
    /* Native 00691417: call 0x66f4e0. */
    eax=(u32)cm_fn_0066f4e0(p4,p3,p2,p1,p0);
    return eax;
block_0069142c:
    /* Native 0069142c: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0x9fffb4u;
    p4=edi;
    /* Native 0069143c: call 0x66f4e0. */
    eax=(u32)cm_fn_0066f4e0(p4,p3,p2,p1,p0);
    return eax;
block_00691451:
    /* Native 00691451: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax+-2;
    above=(u32)(eax)>(u32)(3u);
    if (above) goto block_006914af;
    target=D(eax*4u + 0x6934f0u);
    switch(target) {
    case 0x00691465u: goto block_00691465;
    case 0x0069148au: goto block_0069148a;
    /* Guarded immutable original table: all entries enumerated. */
    default: __builtin_unreachable();
    }
block_00691465:
    /* Native 00691465: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0x9fff00u;
block_00691474:
    /* Native 00691474: push edi. */
    p4=edi;
    /* Native 00691475: call 0x66f4e0. */
    eax=(u32)cm_fn_0066f4e0(p4,p3,p2,p1,p0);
    return eax;
block_0069148a:
    /* Native 0069148a: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0x9ffe3cu;
    p4=edi;
    /* Native 0069149a: call 0x66f4e0. */
    eax=(u32)cm_fn_0066f4e0(p4,p3,p2,p1,p0);
    return eax;
block_006914af:
    /* Native 006914af: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0x9ffd8cu;
    goto block_00691474;
block_006914c0:
    /* Native 006914c0: add eax, 2. */
    eax=eax+2u;
    above=(u32)(eax)>(u32)(4u);
    if (above) goto block_00693172;
    target=D(eax*4u + 0x693500u);
    switch(target) {
    case 0x006914d3u: goto block_006914d3;
    case 0x00691aedu: goto block_00691aed;
    case 0x00692123u: goto block_00692123;
    case 0x00692855u: goto block_00692855;
    case 0x00692ccau: goto block_00692cca;
    /* Guarded immutable original table: all entries enumerated. */
    default: __builtin_unreachable();
    }
block_006914d3:
    /* Native 006914d3: movsx eax, byte ptr [esp + 0x13]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x13u));
    above=(u32)(eax)>(u32)(6u);
    if (above) goto block_00691ad6;
    target=D(eax*4u + 0x693514u);
    switch(target) {
    case 0x006914e8u: goto block_006914e8;
    case 0x00691742u: goto block_00691742;
    case 0x006919bau: goto block_006919ba;
    case 0x00691a91u: goto block_00691a91;
    /* Guarded immutable original table: all entries enumerated. */
    default: __builtin_unreachable();
    }
block_006914e8:
    /* Native 006914e8: movsx eax, bl. */
    eax=(u32)(s32)(s8)(rating);
    above=(u32)(eax)>(u32)(6u);
    if (above) goto block_00693267;
    target=D(eax*4u + 0x693530u);
    switch(target) {
    case 0x006914fbu: goto block_006914fb;
    case 0x0069154au: goto block_0069154a;
    case 0x0069159eu: goto block_0069159e;
    case 0x006915f2u: goto block_006915f2;
    case 0x00691646u: goto block_00691646;
    case 0x0069169au: goto block_0069169a;
    case 0x006916eeu: goto block_006916ee;
    /* Guarded immutable original table: all entries enumerated. */
    default: __builtin_unreachable();
    }
block_006914fb:
    /* Native 006914fb: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    if (zero) goto block_00691530;
    eax=eax-1u;
    zero=(eax==0);
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    if (zero) goto block_0069151a;
    edx=esi + 4u;
    p0=edx;
    p1=eax;
    p2=eax;
    p3=0x9ffcf4u;
    goto block_0069325a;
block_0069151a:
    /* Native 0069151a: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9ffbecu;
    goto block_0069198f;
block_00691530:
    /* Native 00691530: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9ffae8u;
    goto block_0069198f;
block_0069154a:
    /* Native 0069154a: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    if (zero) goto block_00691584;
    eax=eax-1u;
    zero=(eax==0);
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    if (zero) goto block_0069156e;
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9ffa28u;
    p4=edx;
    goto block_0069325f;
block_0069156e:
    /* Native 0069156e: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9ff8f4u;
    goto block_0069198f;
block_00691584:
    /* Native 00691584: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9ff7c8u;
    goto block_0069198f;
block_0069159e:
    /* Native 0069159e: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    if (zero) goto block_006915d8;
    eax=eax-1u;
    zero=(eax==0);
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    if (zero) goto block_006915c2;
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9ff6f8u;
    p4=edx;
    goto block_0069325f;
block_006915c2:
    /* Native 006915c2: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9ff5a4u;
    goto block_0069198f;
block_006915d8:
    /* Native 006915d8: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9ff450u;
    goto block_0069198f;
block_006915f2:
    /* Native 006915f2: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    if (zero) goto block_0069162c;
    eax=eax-1u;
    zero=(eax==0);
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    if (zero) goto block_00691616;
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9ff364u;
    p4=edx;
    goto block_0069325f;
block_00691616:
    /* Native 00691616: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9ff1f4u;
    goto block_0069198f;
block_0069162c:
    /* Native 0069162c: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9ff084u;
    goto block_0069198f;
block_00691646:
    /* Native 00691646: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    if (zero) goto block_00691680;
    eax=eax-1u;
    zero=(eax==0);
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    if (zero) goto block_0069166a;
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9fefa4u;
    p4=edx;
    goto block_0069325f;
block_0069166a:
    /* Native 0069166a: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9fee44u;
    goto block_0069198f;
block_00691680:
    /* Native 00691680: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9fece4u;
    goto block_0069198f;
block_0069169a:
    /* Native 0069169a: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    if (zero) goto block_006916d4;
    eax=eax-1u;
    zero=(eax==0);
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    if (zero) goto block_006916be;
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9fec00u;
    p4=edx;
    goto block_0069325f;
block_006916be:
    /* Native 006916be: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9feaa4u;
    goto block_0069198f;
block_006916d4:
    /* Native 006916d4: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9fe948u;
    goto block_0069198f;
block_006916ee:
    /* Native 006916ee: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    if (zero) goto block_00691728;
    eax=eax-1u;
    zero=(eax==0);
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    if (zero) goto block_00691712;
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9fe86cu;
    p4=edx;
    goto block_0069325f;
block_00691712:
    /* Native 00691712: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9fe72cu;
    goto block_0069198f;
block_00691728:
    /* Native 00691728: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9fe5f8u;
    goto block_0069198f;
block_00691742:
    /* Native 00691742: movsx eax, bl. */
    eax=(u32)(s32)(s8)(rating);
    above=(u32)(eax)>(u32)(6u);
    if (above) goto block_00693267;
    target=D(eax*4u + 0x69354cu);
    switch(target) {
    case 0x00691755u: goto block_00691755;
    case 0x0069176eu: goto block_0069176e;
    case 0x006917e6u: goto block_006917e6;
    case 0x0069185eu: goto block_0069185e;
    case 0x006918a1u: goto block_006918a1;
    case 0x006918f6u: goto block_006918f6;
    case 0x0069194fu: goto block_0069194f;
    /* Guarded immutable original table: all entries enumerated. */
    default: __builtin_unreachable();
    }
block_00691755:
    /* Native 00691755: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9ffcf4u;
    p4=edx;
    goto block_0069325f;
block_0069176e:
    /* Native 0069176e: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    above=(u32)(eax)>(u32)(3u);
    if (above) goto block_006917cd;
    target=D(eax*4u + 0x693568u);
    switch(target) {
    case 0x00691780u: goto block_00691780;
    case 0x00691799u: goto block_00691799;
    case 0x006917b3u: goto block_006917b3;
    /* Guarded immutable original table: all entries enumerated. */
    default: __builtin_unreachable();
    }
block_00691780:
    /* Native 00691780: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9fe514u;
    p4=edx;
    goto block_0069325f;
block_00691799:
    /* Native 00691799: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9fe410u;
    goto block_0069198f;
block_006917b3:
    /* Native 006917b3: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9fe2fcu;
    goto block_0069198f;
block_006917cd:
    /* Native 006917cd: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9fe240u;
    p4=edx;
    goto block_0069325f;
block_006917e6:
    /* Native 006917e6: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    above=(u32)(eax)>(u32)(3u);
    if (above) goto block_00691845;
    target=D(eax*4u + 0x693578u);
    switch(target) {
    case 0x006917f8u: goto block_006917f8;
    case 0x00691811u: goto block_00691811;
    case 0x0069182bu: goto block_0069182b;
    /* Guarded immutable original table: all entries enumerated. */
    default: __builtin_unreachable();
    }
block_006917f8:
    /* Native 006917f8: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9fe154u;
    p4=edx;
    goto block_0069325f;
block_00691811:
    /* Native 00691811: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9fe040u;
    goto block_0069198f;
block_0069182b:
    /* Native 0069182b: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9fdf1cu;
    goto block_0069198f;
block_00691845:
    /* Native 00691845: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9fde50u;
    p4=edx;
    goto block_0069325f;
block_0069185e:
    /* Native 0069185e: mov al, byte ptr [esp + 0x11]. */
    eax=(eax&0xffffff00u)|(u8)(B((S-0u) + 0x11u));
    zero=((u8)(((u8)eax))&(u8)(((u8)eax)))==0;
    less=(s8)((u8)(((u8)eax))&(u8)(((u8)eax)))<0;
    greater=!zero&&!less;
    if (!greater) goto block_00691888;
    greater=(s8)(((u8)eax))>(s8)(2u);
    if (!greater) goto block_006917f8;
    zero=(u8)(((u8)eax))==(u8)(3u);
    if (!zero) goto block_00691888;
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9fdd14u;
    goto block_0069198f;
block_00691888:
    /* Native 00691888: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9fdc2cu;
    p4=edx;
    goto block_0069325f;
block_006918a1:
    /* Native 006918a1: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    if (zero) goto block_006918dd;
    eax=eax-2u;
    zero=(eax==0);
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    if (zero) goto block_006918c7;
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9fdb50u;
    p4=edx;
    goto block_0069325f;
block_006918c7:
    /* Native 006918c7: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9fda20u;
    goto block_0069198f;
block_006918dd:
    /* Native 006918dd: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9fd93cu;
    p4=edx;
    goto block_0069325f;
block_006918f6:
    /* Native 006918f6: mov al, byte ptr [esp + 0x11]. */
    eax=(eax&0xffffff00u)|(u8)(B((S-0u) + 0x11u));
    zero=((u8)(((u8)eax))&(u8)(((u8)eax)))==0;
    less=(s8)((u8)(((u8)eax))&(u8)(((u8)eax)))<0;
    greater=!zero&&!less;
    if (!greater) goto block_00691936;
    greater=(s8)(((u8)eax))>(s8)(2u);
    if (!greater) goto block_0069191d;
    zero=(u8)(((u8)eax))==(u8)(3u);
    if (!zero) goto block_00691936;
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9fd808u;
    goto block_0069198f;
block_0069191d:
    /* Native 0069191d: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9fd768u;
    p4=edx;
    goto block_0069325f;
block_00691936:
    /* Native 00691936: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9fd688u;
    p4=edx;
    goto block_0069325f;
block_0069194f:
    /* Native 0069194f: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    above=(u32)(eax)>(u32)(4u);
    if (above) goto block_006919a1;
    target=D(eax*4u + 0x693588u);
    switch(target) {
    case 0x00691961u: goto block_00691961;
    case 0x0069197au: goto block_0069197a;
    case 0x006919a1u: goto block_006919a1;
    /* Guarded immutable original table: all entries enumerated. */
    default: __builtin_unreachable();
    }
block_00691961:
    /* Native 00691961: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9fd5ecu;
    p4=edx;
    goto block_0069325f;
block_0069197a:
    /* Native 0069197a: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9fd4ccu;
block_0069198f:
    /* Native 0069198f: lea eax, [esp + 0x28]. */
    eax=(S-20u) + 0x28u;
    p5=eax;
    /* Native 00691994: call 0x66f4e0. */
    eax=(u32)cm_fn_0066f4e0(p5,p4,p3,p2,p1,p0);
    goto block_00693267;
block_006919a1:
    /* Native 006919a1: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9fd3f8u;
    p4=edx;
    goto block_0069325f;
block_006919ba:
    /* Native 006919ba: movsx eax, bl. */
    eax=(u32)(s32)(s8)(rating);
    above=(u32)(eax)>(u32)(6u);
    if (above) goto block_00693267;
    target=D(eax*4u + 0x69359cu);
    switch(target) {
    case 0x00691755u: goto block_00691755;
    case 0x006919cdu: goto block_006919cd;
    case 0x006919e6u: goto block_006919e6;
    case 0x00691a16u: goto block_00691a16;
    case 0x00691a46u: goto block_00691a46;
    case 0x00691a5fu: goto block_00691a5f;
    case 0x00691a78u: goto block_00691a78;
    /* Guarded immutable original table: all entries enumerated. */
    default: __builtin_unreachable();
    }
block_006919cd:
    /* Native 006919cd: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9fd368u;
    p4=edx;
    goto block_0069325f;
block_006919e6:
    /* Native 006919e6: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    ecx=esi + 4u;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p0=ecx;
    p1=eax;
    p2=eax;
    if (zero) goto block_00691a07;
    edx=(S-12u) + 0x20u;
    p3=0x9fd298u;
    p4=edx;
    goto block_0069325f;
block_00691a07:
    /* Native 00691a07: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9fd1acu;
    p4=edx;
    goto block_0069325f;
block_00691a16:
    /* Native 00691a16: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    ecx=esi + 4u;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p0=ecx;
    p1=eax;
    p2=eax;
    if (zero) goto block_00691a37;
    edx=(S-12u) + 0x20u;
    p3=0x9fd0c0u;
    p4=edx;
    goto block_0069325f;
block_00691a37:
    /* Native 00691a37: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9fcfc4u;
    p4=edx;
    goto block_0069325f;
block_00691a46:
    /* Native 00691a46: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9fcf24u;
    p4=edx;
    goto block_0069325f;
block_00691a5f:
    /* Native 00691a5f: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9fce88u;
    p4=edx;
    goto block_0069325f;
block_00691a78:
    /* Native 00691a78: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9fcde8u;
    p4=edx;
    goto block_0069325f;
block_00691a91:
    /* Native 00691a91: movsx eax, bl. */
    eax=(u32)(s32)(s8)(rating);
    above=(u32)(eax)>(u32)(6u);
    if (above) goto block_00693267;
    target=D(eax*4u + 0x6935b8u);
    switch(target) {
    case 0x00691755u: goto block_00691755;
    case 0x0069191du: goto block_0069191d;
    case 0x00691961u: goto block_00691961;
    case 0x006919cdu: goto block_006919cd;
    case 0x00691a5fu: goto block_00691a5f;
    case 0x00691aa4u: goto block_00691aa4;
    case 0x00691abdu: goto block_00691abd;
    /* Guarded immutable original table: all entries enumerated. */
    default: __builtin_unreachable();
    }
block_00691aa4:
    /* Native 00691aa4: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9fcd4cu;
    p4=edx;
    goto block_0069325f;
block_00691abd:
    /* Native 00691abd: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9fccacu;
    p4=edx;
    goto block_0069325f;
block_00691ad6:
    /* Native 00691ad6: push 0x9fcc8c. */
    p0=0x9fcc8cu;
block_00691adb:
    /* Native 00691adb: lea eax, [esp + 0x18]. */
    eax=(S-4u) + 0x18u;
    p1=eax;
    /* Native 00691ae0: call 0x944c9f. */
    eax=(u32)cm_fn_00944c9f(p1,p0);
    goto block_00693267;
block_00691aed:
    /* Native 00691aed: movsx eax, byte ptr [esp + 0x13]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x13u));
    above=(u32)(eax)>(u32)(6u);
    if (above) goto block_00691ad6;
    target=D(eax*4u + 0x6935d4u);
    switch(target) {
    case 0x00691afeu: goto block_00691afe;
    case 0x00691d5du: goto block_00691d5d;
    case 0x00691fd5u: goto block_00691fd5;
    case 0x006920acu: goto block_006920ac;
    /* Guarded immutable original table: all entries enumerated. */
    default: __builtin_unreachable();
    }
block_00691afe:
    /* Native 00691afe: movsx eax, bl. */
    eax=(u32)(s32)(s8)(rating);
    above=(u32)(eax)>(u32)(6u);
    if (above) goto block_00693267;
    target=D(eax*4u + 0x6935f0u);
    switch(target) {
    case 0x00691b11u: goto block_00691b11;
    case 0x00691b65u: goto block_00691b65;
    case 0x00691bb9u: goto block_00691bb9;
    case 0x00691c0du: goto block_00691c0d;
    case 0x00691c61u: goto block_00691c61;
    case 0x00691cb5u: goto block_00691cb5;
    case 0x00691d09u: goto block_00691d09;
    /* Guarded immutable original table: all entries enumerated. */
    default: __builtin_unreachable();
    }
block_00691b11:
    /* Native 00691b11: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    if (zero) goto block_00691b4b;
    eax=eax-1u;
    zero=(eax==0);
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    if (zero) goto block_00691b35;
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9fcbf8u;
    p4=edx;
    goto block_0069325f;
block_00691b35:
    /* Native 00691b35: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9fcb00u;
    goto block_0069198f;
block_00691b4b:
    /* Native 00691b4b: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9fca0cu;
    goto block_0069198f;
block_00691b65:
    /* Native 00691b65: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    if (zero) goto block_00691b9f;
    eax=eax-1u;
    zero=(eax==0);
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    if (zero) goto block_00691b89;
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9fc94cu;
    p4=edx;
    goto block_0069325f;
block_00691b89:
    /* Native 00691b89: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9fc828u;
    goto block_0069198f;
block_00691b9f:
    /* Native 00691b9f: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9fc70cu;
    goto block_0069198f;
block_00691bb9:
    /* Native 00691bb9: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    if (zero) goto block_00691bf3;
    eax=eax-1u;
    zero=(eax==0);
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    if (zero) goto block_00691bdd;
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9fc63cu;
    p4=edx;
    goto block_0069325f;
block_00691bdd:
    /* Native 00691bdd: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9fc4e8u;
    goto block_0069198f;
block_00691bf3:
    /* Native 00691bf3: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9fc394u;
    goto block_0069198f;
block_00691c0d:
    /* Native 00691c0d: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    if (zero) goto block_00691c47;
    eax=eax-1u;
    zero=(eax==0);
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    if (zero) goto block_00691c31;
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9fc2a8u;
    p4=edx;
    goto block_0069325f;
block_00691c31:
    /* Native 00691c31: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9fc138u;
    goto block_0069198f;
block_00691c47:
    /* Native 00691c47: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9fbfc8u;
    goto block_0069198f;
block_00691c61:
    /* Native 00691c61: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    if (zero) goto block_00691c9b;
    eax=eax-1u;
    zero=(eax==0);
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    if (zero) goto block_00691c85;
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9fbeecu;
    p4=edx;
    goto block_0069325f;
block_00691c85:
    /* Native 00691c85: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9fbd8cu;
    goto block_0069198f;
block_00691c9b:
    /* Native 00691c9b: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9fbc2cu;
    goto block_0069198f;
block_00691cb5:
    /* Native 00691cb5: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    if (zero) goto block_00691cef;
    eax=eax-1u;
    zero=(eax==0);
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    if (zero) goto block_00691cd9;
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9fbb48u;
    p4=edx;
    goto block_0069325f;
block_00691cd9:
    /* Native 00691cd9: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9fb9ecu;
    goto block_0069198f;
block_00691cef:
    /* Native 00691cef: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9fb890u;
    goto block_0069198f;
block_00691d09:
    /* Native 00691d09: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    if (zero) goto block_00691d43;
    eax=eax-1u;
    zero=(eax==0);
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    if (zero) goto block_00691d2d;
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9fb7b8u;
    p4=edx;
    goto block_0069325f;
block_00691d2d:
    /* Native 00691d2d: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9fb680u;
    goto block_0069198f;
block_00691d43:
    /* Native 00691d43: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9fb550u;
    goto block_0069198f;
block_00691d5d:
    /* Native 00691d5d: movsx eax, bl. */
    eax=(u32)(s32)(s8)(rating);
    above=(u32)(eax)>(u32)(6u);
    if (above) goto block_00693267;
    target=D(eax*4u + 0x69360cu);
    switch(target) {
    case 0x00691d70u: goto block_00691d70;
    case 0x00691d89u: goto block_00691d89;
    case 0x00691e01u: goto block_00691e01;
    case 0x00691e79u: goto block_00691e79;
    case 0x00691ebcu: goto block_00691ebc;
    case 0x00691f11u: goto block_00691f11;
    case 0x00691f80u: goto block_00691f80;
    /* Guarded immutable original table: all entries enumerated. */
    default: __builtin_unreachable();
    }
block_00691d70:
    /* Native 00691d70: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9fcbf8u;
    p4=edx;
    goto block_0069325f;
block_00691d89:
    /* Native 00691d89: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    above=(u32)(eax)>(u32)(3u);
    if (above) goto block_00691de8;
    target=D(eax*4u + 0x693628u);
    switch(target) {
    case 0x00691d9bu: goto block_00691d9b;
    case 0x00691db4u: goto block_00691db4;
    case 0x00691dceu: goto block_00691dce;
    /* Guarded immutable original table: all entries enumerated. */
    default: __builtin_unreachable();
    }
block_00691d9b:
    /* Native 00691d9b: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9fb470u;
    p4=edx;
    goto block_0069325f;
block_00691db4:
    /* Native 00691db4: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9fb36cu;
    goto block_0069198f;
block_00691dce:
    /* Native 00691dce: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9fb258u;
    goto block_0069198f;
block_00691de8:
    /* Native 00691de8: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9fb19cu;
    p4=edx;
    goto block_0069325f;
block_00691e01:
    /* Native 00691e01: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    above=(u32)(eax)>(u32)(3u);
    if (above) goto block_00691e60;
    target=D(eax*4u + 0x693638u);
    switch(target) {
    case 0x00691e13u: goto block_00691e13;
    case 0x00691e2cu: goto block_00691e2c;
    case 0x00691e46u: goto block_00691e46;
    /* Guarded immutable original table: all entries enumerated. */
    default: __builtin_unreachable();
    }
block_00691e13:
    /* Native 00691e13: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9fb0b4u;
    p4=edx;
    goto block_0069325f;
block_00691e2c:
    /* Native 00691e2c: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9fafa0u;
    goto block_0069198f;
block_00691e46:
    /* Native 00691e46: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9fae7cu;
    goto block_0069198f;
block_00691e60:
    /* Native 00691e60: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9fadb0u;
    p4=edx;
    goto block_0069325f;
block_00691e79:
    /* Native 00691e79: mov al, byte ptr [esp + 0x11]. */
    eax=(eax&0xffffff00u)|(u8)(B((S-0u) + 0x11u));
    zero=((u8)(((u8)eax))&(u8)(((u8)eax)))==0;
    less=(s8)((u8)(((u8)eax))&(u8)(((u8)eax)))<0;
    greater=!zero&&!less;
    if (!greater) goto block_00691ea3;
    greater=(s8)(((u8)eax))>(s8)(2u);
    if (!greater) goto block_00691e13;
    zero=(u8)(((u8)eax))==(u8)(3u);
    if (!zero) goto block_00691ea3;
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9fac74u;
    goto block_0069198f;
block_00691ea3:
    /* Native 00691ea3: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9fab8cu;
    p4=edx;
    goto block_0069325f;
block_00691ebc:
    /* Native 00691ebc: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    if (zero) goto block_00691ef8;
    eax=eax-2u;
    zero=(eax==0);
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    if (zero) goto block_00691ee2;
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9faab4u;
    p4=edx;
    goto block_0069325f;
block_00691ee2:
    /* Native 00691ee2: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9fa988u;
    goto block_0069198f;
block_00691ef8:
    /* Native 00691ef8: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9fa8a4u;
    p4=edx;
    goto block_0069325f;
block_00691f11:
    /* Native 00691f11: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    if (zero) goto block_00691f67;
    eax=eax-1u;
    zero=(eax==0);
    if (zero) goto block_00691f4e;
    eax=eax-1u;
    zero=(eax==0);
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    if (zero) goto block_00691f38;
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9fa7c4u;
    p4=edx;
    goto block_0069325f;
block_00691f38:
    /* Native 00691f38: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9fa690u;
    goto block_0069198f;
block_00691f4e:
    /* Native 00691f4e: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9fa5acu;
    p4=edx;
    goto block_0069325f;
block_00691f67:
    /* Native 00691f67: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9fa4c8u;
    p4=edx;
    goto block_0069325f;
block_00691f80:
    /* Native 00691f80: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    if (zero) goto block_00691fbc;
    eax=eax-2u;
    zero=(eax==0);
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    if (zero) goto block_00691fa6;
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9fa3f4u;
    p4=edx;
    goto block_0069325f;
block_00691fa6:
    /* Native 00691fa6: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9fa2d8u;
    goto block_0069198f;
block_00691fbc:
    /* Native 00691fbc: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9fa208u;
    p4=edx;
    goto block_0069325f;
block_00691fd5:
    /* Native 00691fd5: movsx eax, bl. */
    eax=(u32)(s32)(s8)(rating);
    above=(u32)(eax)>(u32)(6u);
    if (above) goto block_00693267;
    target=D(eax*4u + 0x693648u);
    switch(target) {
    case 0x00691d70u: goto block_00691d70;
    case 0x00691fe8u: goto block_00691fe8;
    case 0x00692001u: goto block_00692001;
    case 0x00692031u: goto block_00692031;
    case 0x00692061u: goto block_00692061;
    case 0x0069207au: goto block_0069207a;
    case 0x00692093u: goto block_00692093;
    /* Guarded immutable original table: all entries enumerated. */
    default: __builtin_unreachable();
    }
block_00691fe8:
    /* Native 00691fe8: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9fa178u;
    p4=edx;
    goto block_0069325f;
block_00692001:
    /* Native 00692001: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    ecx=esi + 4u;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p0=ecx;
    p1=eax;
    p2=eax;
    if (zero) goto block_00692022;
    edx=(S-12u) + 0x20u;
    p3=0x9fa0a8u;
    p4=edx;
    goto block_0069325f;
block_00692022:
    /* Native 00692022: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f9fbcu;
    p4=edx;
    goto block_0069325f;
block_00692031:
    /* Native 00692031: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    ecx=esi + 4u;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p0=ecx;
    p1=eax;
    p2=eax;
    if (zero) goto block_00692052;
    edx=(S-12u) + 0x20u;
    p3=0x9f9ed4u;
    p4=edx;
    goto block_0069325f;
block_00692052:
    /* Native 00692052: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f9dd8u;
    p4=edx;
    goto block_0069325f;
block_00692061:
    /* Native 00692061: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9f9d38u;
    p4=edx;
    goto block_0069325f;
block_0069207a:
    /* Native 0069207a: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9f9c9cu;
    p4=edx;
    goto block_0069325f;
block_00692093:
    /* Native 00692093: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9f9c00u;
    p4=edx;
    goto block_0069325f;
block_006920ac:
    /* Native 006920ac: movsx eax, bl. */
    eax=(u32)(s32)(s8)(rating);
    above=(u32)(eax)>(u32)(6u);
    if (above) goto block_00693267;
    target=D(eax*4u + 0x693664u);
    switch(target) {
    case 0x00691d70u: goto block_00691d70;
    case 0x00691fe8u: goto block_00691fe8;
    case 0x0069207au: goto block_0069207a;
    case 0x006920bfu: goto block_006920bf;
    case 0x006920d8u: goto block_006920d8;
    case 0x006920f1u: goto block_006920f1;
    case 0x0069210au: goto block_0069210a;
    /* Guarded immutable original table: all entries enumerated. */
    default: __builtin_unreachable();
    }
block_006920bf:
    /* Native 006920bf: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9f9b68u;
    p4=edx;
    goto block_0069325f;
block_006920d8:
    /* Native 006920d8: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9f9ac8u;
    p4=edx;
    goto block_0069325f;
block_006920f1:
    /* Native 006920f1: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9f9a28u;
    p4=edx;
    goto block_0069325f;
block_0069210a:
    /* Native 0069210a: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9f998cu;
    p4=edx;
    goto block_0069325f;
block_00692123:
    /* Native 00692123: movsx eax, byte ptr [esp + 0x13]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x13u));
    above=(u32)(eax)>(u32)(6u);
    if (above) goto block_00691ad6;
    target=D(eax*4u + 0x693680u);
    switch(target) {
    case 0x00692138u: goto block_00692138;
    case 0x00692397u: goto block_00692397;
    case 0x00692628u: goto block_00692628;
    case 0x00692744u: goto block_00692744;
    /* Guarded immutable original table: all entries enumerated. */
    default: __builtin_unreachable();
    }
block_00692138:
    /* Native 00692138: movsx eax, bl. */
    eax=(u32)(s32)(s8)(rating);
    above=(u32)(eax)>(u32)(6u);
    if (above) goto block_00693267;
    target=D(eax*4u + 0x69369cu);
    switch(target) {
    case 0x0069214bu: goto block_0069214b;
    case 0x0069219fu: goto block_0069219f;
    case 0x006921f3u: goto block_006921f3;
    case 0x00692247u: goto block_00692247;
    case 0x0069229bu: goto block_0069229b;
    case 0x006922efu: goto block_006922ef;
    case 0x00692343u: goto block_00692343;
    /* Guarded immutable original table: all entries enumerated. */
    default: __builtin_unreachable();
    }
block_0069214b:
    /* Native 0069214b: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    if (zero) goto block_00692185;
    eax=eax-1u;
    zero=(eax==0);
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    if (zero) goto block_0069216f;
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9f9920u;
    p4=edx;
    goto block_0069325f;
block_0069216f:
    /* Native 0069216f: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9f9850u;
    goto block_0069198f;
block_00692185:
    /* Native 00692185: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9f9788u;
    goto block_0069198f;
block_0069219f:
    /* Native 0069219f: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    if (zero) goto block_006921d9;
    eax=eax-1u;
    zero=(eax==0);
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    if (zero) goto block_006921c3;
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9f96f4u;
    p4=edx;
    goto block_0069325f;
block_006921c3:
    /* Native 006921c3: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9f95fcu;
    goto block_0069198f;
block_006921d9:
    /* Native 006921d9: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9f9508u;
    goto block_0069198f;
block_006921f3:
    /* Native 006921f3: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    if (zero) goto block_0069222d;
    eax=eax-1u;
    zero=(eax==0);
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    if (zero) goto block_00692217;
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9f9464u;
    p4=edx;
    goto block_0069325f;
block_00692217:
    /* Native 00692217: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9f933cu;
    goto block_0069198f;
block_0069222d:
    /* Native 0069222d: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9f9214u;
    goto block_0069198f;
block_00692247:
    /* Native 00692247: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    if (zero) goto block_00692281;
    eax=eax-1u;
    zero=(eax==0);
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    if (zero) goto block_0069226b;
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9f9158u;
    p4=edx;
    goto block_0069325f;
block_0069226b:
    /* Native 0069226b: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9f901cu;
    goto block_0069198f;
block_00692281:
    /* Native 00692281: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9f8ee0u;
    goto block_0069198f;
block_0069229b:
    /* Native 0069229b: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    if (zero) goto block_006922d5;
    eax=eax-1u;
    zero=(eax==0);
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    if (zero) goto block_006922bf;
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9f8e34u;
    p4=edx;
    goto block_0069325f;
block_006922bf:
    /* Native 006922bf: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9f8d04u;
    goto block_0069198f;
block_006922d5:
    /* Native 006922d5: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9f8bd4u;
    goto block_0069198f;
block_006922ef:
    /* Native 006922ef: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    if (zero) goto block_00692329;
    eax=eax-1u;
    zero=(eax==0);
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    if (zero) goto block_00692313;
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9f8b24u;
    p4=edx;
    goto block_0069325f;
block_00692313:
    /* Native 00692313: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9f89f8u;
    goto block_0069198f;
block_00692329:
    /* Native 00692329: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9f88ccu;
    goto block_0069198f;
block_00692343:
    /* Native 00692343: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    if (zero) goto block_0069237d;
    eax=eax-1u;
    zero=(eax==0);
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    if (zero) goto block_00692367;
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9f8824u;
    p4=edx;
    goto block_0069325f;
block_00692367:
    /* Native 00692367: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9f871cu;
    goto block_0069198f;
block_0069237d:
    /* Native 0069237d: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9f8620u;
    goto block_0069198f;
block_00692397:
    /* Native 00692397: movsx eax, bl. */
    eax=(u32)(s32)(s8)(rating);
    above=(u32)(eax)>(u32)(6u);
    if (above) goto block_00693267;
    target=D(eax*4u + 0x6936b8u);
    switch(target) {
    case 0x006923aau: goto block_006923aa;
    case 0x006923c3u: goto block_006923c3;
    case 0x0069243bu: goto block_0069243b;
    case 0x006924b3u: goto block_006924b3;
    case 0x0069250fu: goto block_0069250f;
    case 0x00692564u: goto block_00692564;
    case 0x006925d3u: goto block_006925d3;
    /* Guarded immutable original table: all entries enumerated. */
    default: __builtin_unreachable();
    }
block_006923aa:
    /* Native 006923aa: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    p0=ecx;
    p1=eax;
    p2=eax;
block_006923b4:
    /* Native 006923b4: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f9920u;
    p4=edx;
    goto block_0069325f;
block_006923c3:
    /* Native 006923c3: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    above=(u32)(eax)>(u32)(3u);
    if (above) goto block_00692422;
    target=D(eax*4u + 0x6936d4u);
    switch(target) {
    case 0x006923d5u: goto block_006923d5;
    case 0x006923eeu: goto block_006923ee;
    case 0x00692408u: goto block_00692408;
    /* Guarded immutable original table: all entries enumerated. */
    default: __builtin_unreachable();
    }
block_006923d5:
    /* Native 006923d5: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9f8568u;
    p4=edx;
    goto block_0069325f;
block_006923ee:
    /* Native 006923ee: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9f8490u;
    goto block_0069198f;
block_00692408:
    /* Native 00692408: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9f83a8u;
    goto block_0069198f;
block_00692422:
    /* Native 00692422: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9f8318u;
    p4=edx;
    goto block_0069325f;
block_0069243b:
    /* Native 0069243b: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    above=(u32)(eax)>(u32)(3u);
    if (above) goto block_0069249a;
    target=D(eax*4u + 0x6936e4u);
    switch(target) {
    case 0x0069244du: goto block_0069244d;
    case 0x00692466u: goto block_00692466;
    case 0x00692480u: goto block_00692480;
    /* Guarded immutable original table: all entries enumerated. */
    default: __builtin_unreachable();
    }
block_0069244d:
    /* Native 0069244d: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9f8258u;
    p4=edx;
    goto block_0069325f;
block_00692466:
    /* Native 00692466: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9f8170u;
    goto block_0069198f;
block_00692480:
    /* Native 00692480: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9f8078u;
    goto block_0069198f;
block_0069249a:
    /* Native 0069249a: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9f7fd8u;
    p4=edx;
    goto block_0069325f;
block_006924b3:
    /* Native 006924b3: mov al, byte ptr [esp + 0x11]. */
    eax=(eax&0xffffff00u)|(u8)(B((S-0u) + 0x11u));
    zero=((u8)(((u8)eax))&(u8)(((u8)eax)))==0;
    less=(s8)((u8)(((u8)eax))&(u8)(((u8)eax)))<0;
    greater=!zero&&!less;
    if (!greater) goto block_006924f6;
    greater=(s8)(((u8)eax))>(s8)(2u);
    if (!greater) goto block_006924dd;
    zero=(u8)(((u8)eax))==(u8)(3u);
    if (!zero) goto block_006924f6;
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9f7eccu;
    goto block_0069198f;
block_006924dd:
    /* Native 006924dd: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9f7e10u;
    p4=edx;
    goto block_0069325f;
block_006924f6:
    /* Native 006924f6: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9f7d58u;
    p4=edx;
    goto block_0069325f;
block_0069250f:
    /* Native 0069250f: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    if (zero) goto block_0069254b;
    eax=eax-2u;
    zero=(eax==0);
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    if (zero) goto block_00692535;
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9f7cb0u;
    p4=edx;
    goto block_0069325f;
block_00692535:
    /* Native 00692535: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9f7bb4u;
    goto block_0069198f;
block_0069254b:
    /* Native 0069254b: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9f7b04u;
    p4=edx;
    goto block_0069325f;
block_00692564:
    /* Native 00692564: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    if (zero) goto block_006925ba;
    eax=eax-1u;
    zero=(eax==0);
    if (zero) goto block_006925a1;
    eax=eax-1u;
    zero=(eax==0);
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    if (zero) goto block_0069258b;
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9f7a58u;
    p4=edx;
    goto block_0069325f;
block_0069258b:
    /* Native 0069258b: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9f7958u;
    goto block_0069198f;
block_006925a1:
    /* Native 006925a1: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9f78a4u;
    p4=edx;
    goto block_0069325f;
block_006925ba:
    /* Native 006925ba: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9f77f0u;
    p4=edx;
    goto block_0069325f;
block_006925d3:
    /* Native 006925d3: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    if (zero) goto block_0069260f;
    eax=eax-2u;
    zero=(eax==0);
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    if (zero) goto block_006925f9;
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9f774cu;
    p4=edx;
    goto block_0069325f;
block_006925f9:
    /* Native 006925f9: movsx ecx, byte ptr [esp + 0x12]. */
    ecx=(u32)(s32)(s8)(B((S-0u) + 0x12u));
    edx=esi + 4u;
    p0=ecx;
    p1=edx;
    p2=eax;
    p3=eax;
    p4=0x9f7660u;
    goto block_0069198f;
block_0069260f:
    /* Native 0069260f: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9f75c4u;
    p4=edx;
    goto block_0069325f;
block_00692628:
    /* Native 00692628: movsx eax, bl. */
    eax=(u32)(s32)(s8)(rating);
    above=(u32)(eax)>(u32)(6u);
    if (above) goto block_00693267;
    target=D(eax*4u + 0x6936f4u);
    switch(target) {
    case 0x006923aau: goto block_006923aa;
    case 0x0069263bu: goto block_0069263b;
    case 0x00692654u: goto block_00692654;
    case 0x00692684u: goto block_00692684;
    case 0x006926b4u: goto block_006926b4;
    case 0x006926e4u: goto block_006926e4;
    case 0x00692714u: goto block_00692714;
    /* Guarded immutable original table: all entries enumerated. */
    default: __builtin_unreachable();
    }
block_0069263b:
    /* Native 0069263b: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9f7560u;
    p4=edx;
    goto block_0069325f;
block_00692654:
    /* Native 00692654: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    ecx=esi + 4u;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p0=ecx;
    p1=eax;
    p2=eax;
    if (zero) goto block_00692675;
    edx=(S-12u) + 0x20u;
    p3=0x9f74bcu;
    p4=edx;
    goto block_0069325f;
block_00692675:
    /* Native 00692675: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f73fcu;
    p4=edx;
    goto block_0069325f;
block_00692684:
    /* Native 00692684: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    ecx=esi + 4u;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p0=ecx;
    p1=eax;
    p2=eax;
    if (zero) goto block_006926a5;
    edx=(S-12u) + 0x20u;
    p3=0x9f7344u;
    p4=edx;
    goto block_0069325f;
block_006926a5:
    /* Native 006926a5: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f7278u;
    p4=edx;
    goto block_0069325f;
block_006926b4:
    /* Native 006926b4: mov al, byte ptr [esp + 0x11]. */
    eax=(eax&0xffffff00u)|(u8)(B((S-0u) + 0x11u));
    ecx=esi + 4u;
    zero=(u8)(((u8)eax))==(u8)(3u);
    p0=ecx;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p1=eax;
    p2=eax;
    if (zero) goto block_006926d5;
    edx=(S-12u) + 0x20u;
    p3=0x9f7208u;
    p4=edx;
    goto block_0069325f;
block_006926d5:
    /* Native 006926d5: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f715cu;
    p4=edx;
    goto block_0069325f;
block_006926e4:
    /* Native 006926e4: mov al, byte ptr [esp + 0x11]. */
    eax=(eax&0xffffff00u)|(u8)(B((S-0u) + 0x11u));
    ecx=esi + 4u;
    zero=(u8)(((u8)eax))==(u8)(3u);
    p0=ecx;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p1=eax;
    p2=eax;
    if (zero) goto block_00692705;
    edx=(S-12u) + 0x20u;
    p3=0x9f70f4u;
    p4=edx;
    goto block_0069325f;
block_00692705:
    /* Native 00692705: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f7050u;
    p4=edx;
    goto block_0069325f;
block_00692714:
    /* Native 00692714: mov al, byte ptr [esp + 0x11]. */
    eax=(eax&0xffffff00u)|(u8)(B((S-0u) + 0x11u));
    ecx=esi + 4u;
    zero=(u8)(((u8)eax))==(u8)(3u);
    p0=ecx;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p1=eax;
    p2=eax;
    if (zero) goto block_00692735;
    edx=(S-12u) + 0x20u;
    p3=0x9f6fe4u;
    p4=edx;
    goto block_0069325f;
block_00692735:
    /* Native 00692735: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f6f3cu;
    p4=edx;
    goto block_0069325f;
block_00692744:
    /* Native 00692744: movsx eax, bl. */
    eax=(u32)(s32)(s8)(rating);
    above=(u32)(eax)>(u32)(6u);
    if (above) goto block_00693267;
    target=D(eax*4u + 0x693710u);
    switch(target) {
    case 0x006926e4u: goto block_006926e4;
    case 0x00692757u: goto block_00692757;
    case 0x0069277cu: goto block_0069277c;
    case 0x006927acu: goto block_006927ac;
    case 0x006927dcu: goto block_006927dc;
    case 0x006927f5u: goto block_006927f5;
    case 0x00692825u: goto block_00692825;
    /* Guarded immutable original table: all entries enumerated. */
    default: __builtin_unreachable();
    }
block_00692757:
    /* Native 00692757: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    ecx=esi + 4u;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p0=ecx;
    p1=eax;
    p2=eax;
    if (!zero) goto block_006923b4;
    edx=(S-12u) + 0x20u;
    p3=0x9f6e8cu;
    p4=edx;
    goto block_0069325f;
block_0069277c:
    /* Native 0069277c: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    ecx=esi + 4u;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p0=ecx;
    p1=eax;
    p2=eax;
    if (zero) goto block_0069279d;
    edx=(S-12u) + 0x20u;
    p3=0x9f7560u;
    p4=edx;
    goto block_0069325f;
block_0069279d:
    /* Native 0069279d: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f6de0u;
    p4=edx;
    goto block_0069325f;
block_006927ac:
    /* Native 006927ac: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    ecx=esi + 4u;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p0=ecx;
    p1=eax;
    p2=eax;
    if (zero) goto block_006927cd;
    edx=(S-12u) + 0x20u;
    p3=0x9f6d70u;
    p4=edx;
    goto block_0069325f;
block_006927cd:
    /* Native 006927cd: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f6cbcu;
    p4=edx;
    goto block_0069325f;
block_006927dc:
    /* Native 006927dc: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9f6c50u;
    p4=edx;
    goto block_0069325f;
block_006927f5:
    /* Native 006927f5: mov al, byte ptr [esp + 0x11]. */
    eax=(eax&0xffffff00u)|(u8)(B((S-0u) + 0x11u));
    ecx=esi + 4u;
    zero=(u8)(((u8)eax))==(u8)(3u);
    p0=ecx;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p1=eax;
    p2=eax;
    if (zero) goto block_00692816;
    edx=(S-12u) + 0x20u;
    p3=0x9f6be4u;
    p4=edx;
    goto block_0069325f;
block_00692816:
    /* Native 00692816: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f6b3cu;
    p4=edx;
    goto block_0069325f;
block_00692825:
    /* Native 00692825: mov al, byte ptr [esp + 0x11]. */
    eax=(eax&0xffffff00u)|(u8)(B((S-0u) + 0x11u));
    ecx=esi + 4u;
    zero=(u8)(((u8)eax))==(u8)(3u);
    p0=ecx;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p1=eax;
    p2=eax;
    if (zero) goto block_00692846;
    edx=(S-12u) + 0x20u;
    p3=0x9f6ad0u;
    p4=edx;
    goto block_0069325f;
block_00692846:
    /* Native 00692846: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f6a28u;
    p4=edx;
    goto block_0069325f;
block_00692855:
    /* Native 00692855: movsx eax, byte ptr [esp + 0x13]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x13u));
    above=(u32)(eax)>(u32)(6u);
    if (above) goto block_00691ad6;
    target=D(eax*4u + 0x69372cu);
    switch(target) {
    case 0x0069286au: goto block_0069286a;
    case 0x006929b6u: goto block_006929b6;
    case 0x00692b07u: goto block_00692b07;
    case 0x00692bdeu: goto block_00692bde;
    /* Guarded immutable original table: all entries enumerated. */
    default: __builtin_unreachable();
    }
block_0069286a:
    /* Native 0069286a: movsx eax, bl. */
    eax=(u32)(s32)(s8)(rating);
    above=(u32)(eax)>(u32)(6u);
    if (above) goto block_00693267;
    target=D(eax*4u + 0x693748u);
    switch(target) {
    case 0x0069287du: goto block_0069287d;
    case 0x00692896u: goto block_00692896;
    case 0x006928c6u: goto block_006928c6;
    case 0x006928f6u: goto block_006928f6;
    case 0x00692926u: goto block_00692926;
    case 0x00692956u: goto block_00692956;
    case 0x00692986u: goto block_00692986;
    /* Guarded immutable original table: all entries enumerated. */
    default: __builtin_unreachable();
    }
block_0069287d:
    /* Native 0069287d: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9f6988u;
    p4=edx;
    goto block_0069325f;
block_00692896:
    /* Native 00692896: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    ecx=esi + 4u;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p0=ecx;
    p1=eax;
    p2=eax;
    if (zero) goto block_006928b7;
    edx=(S-12u) + 0x20u;
    p3=0x9f68bcu;
    p4=edx;
    goto block_0069325f;
block_006928b7:
    /* Native 006928b7: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f67ccu;
    p4=edx;
    goto block_0069325f;
block_006928c6:
    /* Native 006928c6: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    ecx=esi + 4u;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p0=ecx;
    p1=eax;
    p2=eax;
    if (zero) goto block_006928e7;
    edx=(S-12u) + 0x20u;
    p3=0x9f66f0u;
    p4=edx;
    goto block_0069325f;
block_006928e7:
    /* Native 006928e7: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f65fcu;
    p4=edx;
    goto block_0069325f;
block_006928f6:
    /* Native 006928f6: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    ecx=esi + 4u;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p0=ecx;
    p1=eax;
    p2=eax;
    if (zero) goto block_00692917;
    edx=(S-12u) + 0x20u;
    p3=0x9f6510u;
    p4=edx;
    goto block_0069325f;
block_00692917:
    /* Native 00692917: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f6400u;
    p4=edx;
    goto block_0069325f;
block_00692926:
    /* Native 00692926: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    ecx=esi + 4u;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p0=ecx;
    p1=eax;
    p2=eax;
    if (zero) goto block_00692947;
    edx=(S-12u) + 0x20u;
    p3=0x9f6324u;
    p4=edx;
    goto block_0069325f;
block_00692947:
    /* Native 00692947: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f6224u;
    p4=edx;
    goto block_0069325f;
block_00692956:
    /* Native 00692956: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    ecx=esi + 4u;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p0=ecx;
    p1=eax;
    p2=eax;
    if (zero) goto block_00692977;
    edx=(S-12u) + 0x20u;
    p3=0x9f6148u;
    p4=edx;
    goto block_0069325f;
block_00692977:
    /* Native 00692977: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f6048u;
    p4=edx;
    goto block_0069325f;
block_00692986:
    /* Native 00692986: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    ecx=esi + 4u;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p0=ecx;
    p1=eax;
    p2=eax;
    if (zero) goto block_006929a7;
    edx=(S-12u) + 0x20u;
    p3=0x9f5f70u;
    p4=edx;
    goto block_0069325f;
block_006929a7:
    /* Native 006929a7: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f5e8cu;
    p4=edx;
    goto block_0069325f;
block_006929b6:
    /* Native 006929b6: movsx eax, bl. */
    eax=(u32)(s32)(s8)(rating);
    above=(u32)(eax)>(u32)(6u);
    if (above) goto block_00693267;
    target=D(eax*4u + 0x693764u);
    switch(target) {
    case 0x0069287du: goto block_0069287d;
    case 0x006929c9u: goto block_006929c9;
    case 0x006929f9u: goto block_006929f9;
    case 0x00692a29u: goto block_00692a29;
    case 0x00692a68u: goto block_00692a68;
    case 0x00692aa7u: goto block_00692aa7;
    case 0x00692ad7u: goto block_00692ad7;
    /* Guarded immutable original table: all entries enumerated. */
    default: __builtin_unreachable();
    }
block_006929c9:
    /* Native 006929c9: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    ecx=esi + 4u;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p0=ecx;
    p1=eax;
    p2=eax;
    if (zero) goto block_006929ea;
    edx=(S-12u) + 0x20u;
    p3=0x9f5dc4u;
    p4=edx;
    goto block_0069325f;
block_006929ea:
    /* Native 006929ea: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f5ce8u;
    p4=edx;
    goto block_0069325f;
block_006929f9:
    /* Native 006929f9: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    ecx=esi + 4u;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p0=ecx;
    p1=eax;
    p2=eax;
    if (zero) goto block_00692a1a;
    edx=(S-12u) + 0x20u;
    p3=0x9f5c20u;
    p4=edx;
    goto block_0069325f;
block_00692a1a:
    /* Native 00692a1a: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f5b30u;
    p4=edx;
    goto block_0069325f;
block_00692a29:
    /* Native 00692a29: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    if (zero) goto block_00692a4f;
    eax=eax-4u;
    zero=(eax==0);
    if (zero) goto block_00692a4f;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9f5a70u;
    p4=edx;
    goto block_0069325f;
block_00692a4f:
    /* Native 00692a4f: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9f5978u;
    p4=edx;
    goto block_0069325f;
block_00692a68:
    /* Native 00692a68: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    if (zero) goto block_00692a8e;
    eax=eax-4u;
    zero=(eax==0);
    if (zero) goto block_00692a8e;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9f58bcu;
    p4=edx;
    goto block_0069325f;
block_00692a8e:
    /* Native 00692a8e: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9f57ccu;
    p4=edx;
    goto block_0069325f;
block_00692aa7:
    /* Native 00692aa7: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    ecx=esi + 4u;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p0=ecx;
    p1=eax;
    p2=eax;
    if (zero) goto block_00692ac8;
    edx=(S-12u) + 0x20u;
    p3=0x9f56f0u;
    p4=edx;
    goto block_0069325f;
block_00692ac8:
    /* Native 00692ac8: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f55f8u;
    p4=edx;
    goto block_0069325f;
block_00692ad7:
    /* Native 00692ad7: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    ecx=esi + 4u;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p0=ecx;
    p1=eax;
    p2=eax;
    if (zero) goto block_00692af8;
    edx=(S-12u) + 0x20u;
    p3=0x9f56f0u;
    p4=edx;
    goto block_0069325f;
block_00692af8:
    /* Native 00692af8: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f54fcu;
    p4=edx;
    goto block_0069325f;
block_00692b07:
    /* Native 00692b07: movsx eax, bl. */
    eax=(u32)(s32)(s8)(rating);
    above=(u32)(eax)>(u32)(6u);
    if (above) goto block_00693267;
    target=D(eax*4u + 0x693780u);
    switch(target) {
    case 0x0069287du: goto block_0069287d;
    case 0x00692b1au: goto block_00692b1a;
    case 0x00692b33u: goto block_00692b33;
    case 0x00692b63u: goto block_00692b63;
    case 0x00692b93u: goto block_00692b93;
    case 0x00692bacu: goto block_00692bac;
    case 0x00692bc5u: goto block_00692bc5;
    /* Guarded immutable original table: all entries enumerated. */
    default: __builtin_unreachable();
    }
block_00692b1a:
    /* Native 00692b1a: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9f5460u;
    p4=edx;
    goto block_0069325f;
block_00692b33:
    /* Native 00692b33: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    ecx=esi + 4u;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p0=ecx;
    p1=eax;
    p2=eax;
    if (zero) goto block_00692b54;
    edx=(S-12u) + 0x20u;
    p3=0x9f5384u;
    p4=edx;
    goto block_0069325f;
block_00692b54:
    /* Native 00692b54: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f5288u;
    p4=edx;
    goto block_0069325f;
block_00692b63:
    /* Native 00692b63: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    ecx=esi + 4u;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p0=ecx;
    p1=eax;
    p2=eax;
    if (zero) goto block_00692b84;
    edx=(S-12u) + 0x20u;
    p3=0x9f519cu;
    p4=edx;
    goto block_0069325f;
block_00692b84:
    /* Native 00692b84: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f509cu;
    p4=edx;
    goto block_0069325f;
block_00692b93:
    /* Native 00692b93: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9f4ff4u;
    p4=edx;
    goto block_0069325f;
block_00692bac:
    /* Native 00692bac: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9f4f50u;
    p4=edx;
    goto block_0069325f;
block_00692bc5:
    /* Native 00692bc5: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9f4eacu;
    p4=edx;
    goto block_0069325f;
block_00692bde:
    /* Native 00692bde: movsx eax, bl. */
    eax=(u32)(s32)(s8)(rating);
    above=(u32)(eax)>(u32)(6u);
    if (above) goto block_00693267;
    target=D(eax*4u + 0x69379cu);
    switch(target) {
    case 0x0069287du: goto block_0069287d;
    case 0x00692b1au: goto block_00692b1a;
    case 0x00692bf1u: goto block_00692bf1;
    case 0x00692c0au: goto block_00692c0a;
    case 0x00692c3au: goto block_00692c3a;
    case 0x00692c6au: goto block_00692c6a;
    case 0x00692c9au: goto block_00692c9a;
    /* Guarded immutable original table: all entries enumerated. */
    default: __builtin_unreachable();
    }
block_00692bf1:
    /* Native 00692bf1: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9f4e08u;
    p4=edx;
    goto block_0069325f;
block_00692c0a:
    /* Native 00692c0a: mov al, byte ptr [esp + 0x11]. */
    eax=(eax&0xffffff00u)|(u8)(B((S-0u) + 0x11u));
    ecx=esi + 4u;
    zero=(u8)(((u8)eax))==(u8)(3u);
    p0=ecx;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p1=eax;
    p2=eax;
    if (zero) goto block_00692c2b;
    edx=(S-12u) + 0x20u;
    p3=0x9f4d64u;
    p4=edx;
    goto block_0069325f;
block_00692c2b:
    /* Native 00692c2b: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f4c84u;
    p4=edx;
    goto block_0069325f;
block_00692c3a:
    /* Native 00692c3a: mov al, byte ptr [esp + 0x11]. */
    eax=(eax&0xffffff00u)|(u8)(B((S-0u) + 0x11u));
    ecx=esi + 4u;
    zero=(u8)(((u8)eax))==(u8)(3u);
    p0=ecx;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p1=eax;
    p2=eax;
    if (zero) goto block_00692c5b;
    edx=(S-12u) + 0x20u;
    p3=0x9f4f50u;
    p4=edx;
    goto block_0069325f;
block_00692c5b:
    /* Native 00692c5b: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f4ba8u;
    p4=edx;
    goto block_0069325f;
block_00692c6a:
    /* Native 00692c6a: mov al, byte ptr [esp + 0x11]. */
    eax=(eax&0xffffff00u)|(u8)(B((S-0u) + 0x11u));
    ecx=esi + 4u;
    zero=(u8)(((u8)eax))==(u8)(3u);
    p0=ecx;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p1=eax;
    p2=eax;
    if (zero) goto block_00692c8b;
    edx=(S-12u) + 0x20u;
    p3=0x9f4b00u;
    p4=edx;
    goto block_0069325f;
block_00692c8b:
    /* Native 00692c8b: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f4a1cu;
    p4=edx;
    goto block_0069325f;
block_00692c9a:
    /* Native 00692c9a: mov al, byte ptr [esp + 0x11]. */
    eax=(eax&0xffffff00u)|(u8)(B((S-0u) + 0x11u));
    ecx=esi + 4u;
    zero=(u8)(((u8)eax))==(u8)(3u);
    p0=ecx;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p1=eax;
    p2=eax;
    if (zero) goto block_00692cbb;
    edx=(S-12u) + 0x20u;
    p3=0x9f4978u;
    p4=edx;
    goto block_0069325f;
block_00692cbb:
    /* Native 00692cbb: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f4898u;
    p4=edx;
    goto block_0069325f;
block_00692cca:
    /* Native 00692cca: movsx eax, byte ptr [esp + 0x13]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x13u));
    above=(u32)(eax)>(u32)(6u);
    if (above) goto block_00693168;
    target=D(eax*4u + 0x6937b8u);
    switch(target) {
    case 0x00692cdfu: goto block_00692cdf;
    case 0x00692e2bu: goto block_00692e2b;
    case 0x00692f5eu: goto block_00692f5e;
    case 0x00693061u: goto block_00693061;
    /* Guarded immutable original table: all entries enumerated. */
    default: __builtin_unreachable();
    }
block_00692cdf:
    /* Native 00692cdf: movsx eax, bl. */
    eax=(u32)(s32)(s8)(rating);
    above=(u32)(eax)>(u32)(6u);
    if (above) goto block_00693267;
    target=D(eax*4u + 0x6937d4u);
    switch(target) {
    case 0x00692cf2u: goto block_00692cf2;
    case 0x00692d0bu: goto block_00692d0b;
    case 0x00692d3bu: goto block_00692d3b;
    case 0x00692d6bu: goto block_00692d6b;
    case 0x00692d9bu: goto block_00692d9b;
    case 0x00692dcbu: goto block_00692dcb;
    case 0x00692dfbu: goto block_00692dfb;
    /* Guarded immutable original table: all entries enumerated. */
    default: __builtin_unreachable();
    }
block_00692cf2:
    /* Native 00692cf2: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9f47e4u;
    p4=edx;
    goto block_0069325f;
block_00692d0b:
    /* Native 00692d0b: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    ecx=esi + 4u;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p0=ecx;
    p1=eax;
    p2=eax;
    if (zero) goto block_00692d2c;
    edx=(S-12u) + 0x20u;
    p3=0x9f4704u;
    p4=edx;
    goto block_0069325f;
block_00692d2c:
    /* Native 00692d2c: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f45e4u;
    p4=edx;
    goto block_0069325f;
block_00692d3b:
    /* Native 00692d3b: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    ecx=esi + 4u;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p0=ecx;
    p1=eax;
    p2=eax;
    if (zero) goto block_00692d5c;
    edx=(S-12u) + 0x20u;
    p3=0x9f44f4u;
    p4=edx;
    goto block_0069325f;
block_00692d5c:
    /* Native 00692d5c: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f43d8u;
    p4=edx;
    goto block_0069325f;
block_00692d6b:
    /* Native 00692d6b: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    ecx=esi + 4u;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p0=ecx;
    p1=eax;
    p2=eax;
    if (zero) goto block_00692d8c;
    edx=(S-12u) + 0x20u;
    p3=0x9f42c0u;
    p4=edx;
    goto block_0069325f;
block_00692d8c:
    /* Native 00692d8c: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f4190u;
    p4=edx;
    goto block_0069325f;
block_00692d9b:
    /* Native 00692d9b: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    ecx=esi + 4u;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p0=ecx;
    p1=eax;
    p2=eax;
    if (zero) goto block_00692dbc;
    edx=(S-12u) + 0x20u;
    p3=0x9f4098u;
    p4=edx;
    goto block_0069325f;
block_00692dbc:
    /* Native 00692dbc: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f3f88u;
    p4=edx;
    goto block_0069325f;
block_00692dcb:
    /* Native 00692dcb: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    ecx=esi + 4u;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p0=ecx;
    p1=eax;
    p2=eax;
    if (zero) goto block_00692dec;
    edx=(S-12u) + 0x20u;
    p3=0x9f3e90u;
    p4=edx;
    goto block_0069325f;
block_00692dec:
    /* Native 00692dec: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f3d6cu;
    p4=edx;
    goto block_0069325f;
block_00692dfb:
    /* Native 00692dfb: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    ecx=esi + 4u;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p0=ecx;
    p1=eax;
    p2=eax;
    if (zero) goto block_00692e1c;
    edx=(S-12u) + 0x20u;
    p3=0x9f3c78u;
    p4=edx;
    goto block_0069325f;
block_00692e1c:
    /* Native 00692e1c: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f3b58u;
    p4=edx;
    goto block_0069325f;
block_00692e2b:
    /* Native 00692e2b: movsx eax, bl. */
    eax=(u32)(s32)(s8)(rating);
    above=(u32)(eax)>(u32)(6u);
    if (above) goto block_00693267;
    target=D(eax*4u + 0x6937f0u);
    switch(target) {
    case 0x00692cf2u: goto block_00692cf2;
    case 0x00692e3eu: goto block_00692e3e;
    case 0x00692e6eu: goto block_00692e6e;
    case 0x00692e9eu: goto block_00692e9e;
    case 0x00692eceu: goto block_00692ece;
    case 0x00692efeu: goto block_00692efe;
    case 0x00692f2eu: goto block_00692f2e;
    /* Guarded immutable original table: all entries enumerated. */
    default: __builtin_unreachable();
    }
block_00692e3e:
    /* Native 00692e3e: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    ecx=esi + 4u;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p0=ecx;
    p1=eax;
    p2=eax;
    if (zero) goto block_00692e5f;
    edx=(S-12u) + 0x20u;
    p3=0x9f3a7cu;
    p4=edx;
    goto block_0069325f;
block_00692e5f:
    /* Native 00692e5f: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f3968u;
    p4=edx;
    goto block_0069325f;
block_00692e6e:
    /* Native 00692e6e: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    ecx=esi + 4u;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p0=ecx;
    p1=eax;
    p2=eax;
    if (zero) goto block_00692e8f;
    edx=(S-12u) + 0x20u;
    p3=0x9f387cu;
    p4=edx;
    goto block_0069325f;
block_00692e8f:
    /* Native 00692e8f: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f376cu;
    p4=edx;
    goto block_0069325f;
block_00692e9e:
    /* Native 00692e9e: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    ecx=esi + 4u;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p0=ecx;
    p1=eax;
    p2=eax;
    if (zero) goto block_00692ebf;
    edx=(S-12u) + 0x20u;
    p3=0x9f3668u;
    p4=edx;
    goto block_0069325f;
block_00692ebf:
    /* Native 00692ebf: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f3548u;
    p4=edx;
    goto block_0069325f;
block_00692ece:
    /* Native 00692ece: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    ecx=esi + 4u;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p0=ecx;
    p1=eax;
    p2=eax;
    if (zero) goto block_00692eef;
    edx=(S-12u) + 0x20u;
    p3=0x9f3454u;
    p4=edx;
    goto block_0069325f;
block_00692eef:
    /* Native 00692eef: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f3344u;
    p4=edx;
    goto block_0069325f;
block_00692efe:
    /* Native 00692efe: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    ecx=esi + 4u;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p0=ecx;
    p1=eax;
    p2=eax;
    if (zero) goto block_00692f1f;
    edx=(S-12u) + 0x20u;
    p3=0x9f3e90u;
    p4=edx;
    goto block_0069325f;
block_00692f1f:
    /* Native 00692f1f: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f3230u;
    p4=edx;
    goto block_0069325f;
block_00692f2e:
    /* Native 00692f2e: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    ecx=esi + 4u;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p0=ecx;
    p1=eax;
    p2=eax;
    if (zero) goto block_00692f4f;
    edx=(S-12u) + 0x20u;
    p3=0x9f3148u;
    p4=edx;
    goto block_0069325f;
block_00692f4f:
    /* Native 00692f4f: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f3048u;
    p4=edx;
    goto block_0069325f;
block_00692f5e:
    /* Native 00692f5e: movsx eax, bl. */
    eax=(u32)(s32)(s8)(rating);
    above=(u32)(eax)>(u32)(6u);
    if (above) goto block_00693267;
    target=D(eax*4u + 0x69380cu);
    switch(target) {
    case 0x00692f71u: goto block_00692f71;
    case 0x00692fa1u: goto block_00692fa1;
    case 0x00692fd1u: goto block_00692fd1;
    case 0x00693001u: goto block_00693001;
    case 0x00693031u: goto block_00693031;
    case 0x00693074u: goto block_00693074;
    case 0x0069308du: goto block_0069308d;
    /* Guarded immutable original table: all entries enumerated. */
    default: __builtin_unreachable();
    }
block_00692f71:
    /* Native 00692f71: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    ecx=esi + 4u;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p0=ecx;
    p1=eax;
    p2=eax;
    if (zero) goto block_00692f92;
    edx=(S-12u) + 0x20u;
    p3=0x9f2f58u;
    p4=edx;
    goto block_0069325f;
block_00692f92:
    /* Native 00692f92: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f2e4cu;
    p4=edx;
    goto block_0069325f;
block_00692fa1:
    /* Native 00692fa1: movsx eax, byte ptr [esp + 0x11]. */
    eax=(u32)(s32)(s8)(B((S-0u) + 0x11u));
    eax=eax-1u;
    zero=(eax==0);
    ecx=esi + 4u;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p0=ecx;
    p1=eax;
    p2=eax;
    if (zero) goto block_00692fc2;
    edx=(S-12u) + 0x20u;
    p3=0x9f2d48u;
    p4=edx;
    goto block_0069325f;
block_00692fc2:
    /* Native 00692fc2: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f2c2cu;
    p4=edx;
    goto block_0069325f;
block_00692fd1:
    /* Native 00692fd1: mov al, byte ptr [esp + 0x11]. */
    eax=(eax&0xffffff00u)|(u8)(B((S-0u) + 0x11u));
    ecx=esi + 4u;
    zero=(u8)(((u8)eax))==(u8)(3u);
    p0=ecx;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p1=eax;
    p2=eax;
    if (zero) goto block_00692ff2;
    edx=(S-12u) + 0x20u;
    p3=0x9f2b78u;
    p4=edx;
    goto block_0069325f;
block_00692ff2:
    /* Native 00692ff2: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f2a84u;
    p4=edx;
    goto block_0069325f;
block_00693001:
    /* Native 00693001: mov al, byte ptr [esp + 0x11]. */
    eax=(eax&0xffffff00u)|(u8)(B((S-0u) + 0x11u));
    ecx=esi + 4u;
    zero=(u8)(((u8)eax))==(u8)(3u);
    p0=ecx;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p1=eax;
    p2=eax;
    if (zero) goto block_00693022;
    edx=(S-12u) + 0x20u;
    p3=0x9f29d4u;
    p4=edx;
    goto block_0069325f;
block_00693022:
    /* Native 00693022: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f28e8u;
    p4=edx;
    goto block_0069325f;
block_00693031:
    /* Native 00693031: mov al, byte ptr [esp + 0x11]. */
    eax=(eax&0xffffff00u)|(u8)(B((S-0u) + 0x11u));
    ecx=esi + 4u;
    zero=(u8)(((u8)eax))==(u8)(3u);
    p0=ecx;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p1=eax;
    p2=eax;
    if (zero) goto block_00693052;
    edx=(S-12u) + 0x20u;
    p3=0x9f2834u;
    p4=edx;
    goto block_0069325f;
block_00693052:
    /* Native 00693052: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f2744u;
    p4=edx;
    goto block_0069325f;
block_00693061:
    /* Native 00693061: movsx eax, bl. */
    eax=(u32)(s32)(s8)(rating);
    above=(u32)(eax)>(u32)(6u);
    if (above) goto block_00693267;
    target=D(eax*4u + 0x693828u);
    switch(target) {
    case 0x00693074u: goto block_00693074;
    case 0x0069308du: goto block_0069308d;
    case 0x006930a6u: goto block_006930a6;
    case 0x006930bfu: goto block_006930bf;
    case 0x006930d8u: goto block_006930d8;
    case 0x00693108u: goto block_00693108;
    case 0x00693138u: goto block_00693138;
    /* Guarded immutable original table: all entries enumerated. */
    default: __builtin_unreachable();
    }
block_00693074:
    /* Native 00693074: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9f47e4u;
    p4=edx;
    goto block_0069325f;
block_0069308d:
    /* Native 0069308d: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9f2694u;
    p4=edx;
    goto block_0069325f;
block_006930a6:
    /* Native 006930a6: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9f25dcu;
    p4=edx;
    goto block_0069325f;
block_006930bf:
    /* Native 006930bf: movsx eax, byte ptr [esi + 0x37]. */
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    ecx=esi + 4u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=eax;
    p2=eax;
    p3=0x9f252cu;
    p4=edx;
    goto block_0069325f;
block_006930d8:
    /* Native 006930d8: mov al, byte ptr [esp + 0x11]. */
    eax=(eax&0xffffff00u)|(u8)(B((S-0u) + 0x11u));
    ecx=esi + 4u;
    zero=(u8)(((u8)eax))==(u8)(3u);
    p0=ecx;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p1=eax;
    p2=eax;
    if (zero) goto block_006930f9;
    edx=(S-12u) + 0x20u;
    p3=0x9f29d4u;
    p4=edx;
    goto block_0069325f;
block_006930f9:
    /* Native 006930f9: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f2444u;
    p4=edx;
    goto block_0069325f;
block_00693108:
    /* Native 00693108: mov al, byte ptr [esp + 0x11]. */
    eax=(eax&0xffffff00u)|(u8)(B((S-0u) + 0x11u));
    ecx=esi + 4u;
    zero=(u8)(((u8)eax))==(u8)(3u);
    p0=ecx;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p1=eax;
    p2=eax;
    if (zero) goto block_00693129;
    edx=(S-12u) + 0x20u;
    p3=0x9f2390u;
    p4=edx;
    goto block_0069325f;
block_00693129:
    /* Native 00693129: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f22a4u;
    p4=edx;
    goto block_0069325f;
block_00693138:
    /* Native 00693138: mov al, byte ptr [esp + 0x11]. */
    eax=(eax&0xffffff00u)|(u8)(B((S-0u) + 0x11u));
    ecx=esi + 4u;
    zero=(u8)(((u8)eax))==(u8)(3u);
    p0=ecx;
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    p1=eax;
    p2=eax;
    if (zero) goto block_00693159;
    edx=(S-12u) + 0x20u;
    p3=0x9f21f4u;
    p4=edx;
    goto block_0069325f;
block_00693159:
    /* Native 00693159: lea edx, [esp + 0x20]. */
    edx=(S-12u) + 0x20u;
    p3=0x9f210cu;
    p4=edx;
    goto block_0069325f;
block_00693168:
    /* Native 00693168: push 0x9f20ec. */
    p0=0x9f20ecu;
    goto block_00691adb;
block_00693172:
    /* Native 00693172: lea ecx, [esp + 0x14]. */
    ecx=(S-0u) + 0x14u;
    p0=0x9f20ccu;
    p1=ecx;
    /* Native 0069317c: call 0x944c9f. */
    eax=(u32)cm_fn_00944c9f(p1,p0);
    goto block_00693267;
block_00693189:
    /* Native 00693189: mov eax, dword ptr [esi + 0x53]. */
    eax=D(esi + 0x53u);
    zero=((u32)(eax)&(u32)(eax))==0;
    if (zero) goto block_006931a5;
    p0=9u;
    edx=(S-4u) + 0x5f4u;
    p1=0x64u;
    p2=edx;
    p3=eax;
    /* Native 0069319d: call 0x6181d0. */
    eax=(u32)cm_fn_006181d0(p3,p2,p1,p0);
block_006931a5:
    /* Native 006931a5: mov eax, dword ptr [esi]. */
    eax=D(esi);
    ecx=eax + eax*8u;
    edx=eax + ecx*8u;
    eax=D(ebp);
    eax=(eax&0xffff0000u)|(u16)(W(edx + eax + 6u));
    greater=(s16)(((u16)eax))>(s16)(0x251cu);
    if (!greater) goto block_006931d6;
    ecx=(S-0u) + 0x5f0u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=0x64u;
    p2=0x64u;
    p3=0x9f205cu;
    p4=edx;
    goto block_0069325f;
block_006931d6:
    /* Native 006931d6: cmp ax, 0x2134. */
    greater=(s16)(((u16)eax))>(s16)(0x2134u);
    if (!greater) goto block_006931f4;
    eax=(S-0u) + 0x5f0u;
    ecx=(S-0u) + 0x14u;
    p0=eax;
    p1=0x64u;
    p2=0x64u;
    p3=0x9f1fe8u;
    p4=ecx;
    goto block_0069325f;
block_006931f4:
    /* Native 006931f4: cmp ax, 0x1964. */
    greater=(s16)(((u16)eax))>(s16)(0x1964u);
    if (!greater) goto block_0069320d;
    edx=(S-0u) + 0x5f0u;
    p0=edx;
    p1=0x64u;
    p2=0x64u;
    p3=0x9f1f7cu;
    goto block_0069325a;
block_0069320d:
    /* Native 0069320d: cmp ax, 0x10b3. */
    greater=(s16)(((u16)eax))>(s16)(0x10b3u);
    if (!greater) goto block_0069322b;
    ecx=(S-0u) + 0x5f0u;
    edx=(S-0u) + 0x14u;
    p0=ecx;
    p1=0x64u;
    p2=0x64u;
    p3=0x9f1f0cu;
    p4=edx;
    goto block_0069325f;
block_0069322b:
    /* Native 0069322b: cmp ax, 0xdac. */
    greater=(s16)(((u16)eax))>(s16)(0xdacu);
    if (!greater) goto block_00693249;
    eax=(S-0u) + 0x5f0u;
    ecx=(S-0u) + 0x14u;
    p0=eax;
    p1=0x64u;
    p2=0x64u;
    p3=0x9f1e9cu;
    p4=ecx;
    goto block_0069325f;
block_00693249:
    /* Native 00693249: lea edx, [esp + 0x5f0]. */
    edx=(S-0u) + 0x5f0u;
    p0=edx;
    p1=0x64u;
    p2=0x64u;
    p3=0x9f1e24u;
block_0069325a:
    /* Native 0069325a: lea eax, [esp + 0x24]. */
    eax=(S-16u) + 0x24u;
    p4=eax;
block_0069325f:
    /* Native 0069325f: call 0x66f4e0. */
    eax=(u32)cm_fn_0066f4e0(p4,p3,p2,p1,p0);
block_00693267:
    /* Native 00693267: mov eax, dword ptr [esi]. */
    eax=D(esi);
    ecx=eax + eax*8u;
    edx=eax + ecx*8u;
    eax=D(ebp);
    ecx=D(edx + eax + 0x45u);
    p0=ecx;
    ecx=0xae2c90u;
    edx=D(edx + eax + 0x41u);
    p1=edx;
    /* Native 00693281: call 0x54cdb0. */
    eax=(u32)cm_fn_0054cdb0(ecx,p1,p0);
    less=(s32)(eax)<(s32)(0x78u);
    if (!less) goto block_00693338;
    eax=D(esi);
    ecx=eax + eax*8u;
    edx=eax + ecx*8u;
    eax=D(ebp);
    ecx=edx + eax;
    eax=(eax&0xffff0000u)|(u16)(W(edx + eax + 6u));
    less=(s16)(((u16)eax))<(s16)(0x1964u);
    if (!less) goto block_00693338;
    greater=(s16)(((u16)eax))>(s16)(0x10b3u);
    if (!greater) goto block_006932f8;
    greater=(s16)(W(ecx + 0x16u))>(s16)(0x5dcu);
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    if (!greater) goto block_006932db;
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0x9f1cfcu;
block_006932c9:
    /* Native 006932c9: lea ecx, [esp + 0x24]. */
    ecx=(S-16u) + 0x24u;
    p4=ecx;
    /* Native 006932ce: call 0x66f4e0. */
    eax=(u32)cm_fn_0066f4e0(p4,p3,p2,p1,p0);
    goto block_006933b3;
block_006932db:
    /* Native 006932db: add esi, 4. */
    esi=esi+4u;
    edx=(S-0u) + 0x14u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0x9f1c54u;
    p4=edx;
    /* Native 006932eb: call 0x66f4e0. */
    eax=(u32)cm_fn_0066f4e0(p4,p3,p2,p1,p0);
    goto block_006933b3;
block_006932f8:
    /* Native 006932f8: cmp ax, 0xdac. */
    greater=(s16)(((u16)eax))>(s16)(0xdacu);
    if (!greater) goto block_006933b3;
    greater=(s16)(W(ecx + 0x16u))>(s16)(0x5dcu);
    eax=(u32)(s32)(s8)(B(esi + 0x37u));
    if (!greater) goto block_0069332b;
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    eax=(S-12u) + 0x20u;
    p3=0x9f1b28u;
    p4=eax;
    /* Native 0069331e: call 0x66f4e0. */
    eax=(u32)cm_fn_0066f4e0(p4,p3,p2,p1,p0);
    goto block_006933b3;
block_0069332b:
    /* Native 0069332b: add esi, 4. */
    esi=esi+4u;
    p0=esi;
    p1=eax;
    p2=eax;
    p3=0x9f1a7cu;
    goto block_006932c9;
block_00693338:
    /* Native 00693338: mov al, byte ptr [esp + 0x654]. */
    eax=(eax&0xffffff00u)|(u8)(B((S-0u) + 0x654u));
    zero=((u8)(((u8)eax))&(u8)(((u8)eax)))==0;
    if (!zero) goto block_006933be;
    esi=D(esi);
    ecx=D(ebp);
    edx=esi + esi*8u;
    eax=esi + edx*8u;
    eax=eax+ecx;
    ecx=(ecx&0xffff0000u)|(u16)(W(eax + 0x16u));
    less=(s16)(((u16)ecx))<(s16)(0xfd12u);
    if (!less) goto block_00693381;
    eax=(eax&0xffff0000u)|(u16)(W(eax + 6u));
    less=(s16)(((u16)eax))<(s16)(0xea6u);
    if (!less) goto block_0069336c;
    p0=0x9f1a00u;
    goto block_006933a3;
block_0069336c:
    /* Native 0069336c: cmp ax, 0x157c. */
    less=(s16)(((u16)eax))<(s16)(0x157cu);
    if (!less) goto block_006933e6;
    eax=(S-0u) + 0x654u;
    p0=0x9f1988u;
    p1=eax;
    goto block_006933ab;
block_00693381:
    /* Native 00693381: cmp cx, 0xfe0c. */
    less=(s16)(((u16)ecx))<(s16)(0xfe0cu);
    if (!less) goto block_00693397;
    ecx=(S-0u) + 0x654u;
    p0=0x9f192cu;
    p1=ecx;
    goto block_006933ab;
block_00693397:
    /* Native 00693397: cmp cx, 0x5dc. */
    greater=(s16)(((u16)ecx))>(s16)(0x5dcu);
    if (!greater) goto block_006933e6;
    p0=0x9f18e8u;
block_006933a3:
    /* Native 006933a3: lea edx, [esp + 0x658]. */
    edx=(S-4u) + 0x658u;
    p1=edx;
block_006933ab:
    /* Native 006933ab: call 0x66f4e0. */
    eax=(u32)cm_fn_0066f4e0(p1,p0);
block_006933b3:
    /* Native 006933b3: mov al, byte ptr [esp + 0x654]. */
    eax=(eax&0xffffff00u)|(u8)(B((S-0u) + 0x654u));
    zero=((u8)(((u8)eax))&(u8)(((u8)eax)))==0;
    if (zero) goto block_006933e6;
block_006933be:
    /* Native 006933be: lea eax, [esp + 0x654]. */
    eax=(S-0u) + 0x654u;
    ecx=(S-0u) + 0x14u;
    p0=eax;
    p1=ecx;
    p2=0x991948u;
    p3=edi;
    /* Native 006933d1: call 0x944c9f. */
    eax=(u32)cm_fn_00944c9f(p3,p2,p1,p0);
    return eax;
block_006933e6:
    /* Native 006933e6: lea edx, [esp + 0x14]. */
    edx=(S-0u) + 0x14u;
    p0=edx;
    p1=0x991944u;
    p2=edi;
    /* Native 006933f1: call 0x944c9f. */
    eax=(u32)cm_fn_00944c9f(p2,p1,p0);
block_006933f9:
    /* Native 006933f9: pop edi. */
    return eax;
}
#undef D
#undef W
#undef B

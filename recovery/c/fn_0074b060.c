/* Preferred-image VA 0074B060, base00400000, binary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS6/RET18h. Full incidental EAX is retained on every exit. B denotes
   ESP after the four saved registers; work starts at B+10h, immediately
   after SUB128h. The original SEH state belongs to the linked frame.
   Callees, including all RNG and decision routines, remain original code. */
#include "cmre.h"
#define D(p) CM_GLOBAL(volatile u32,(u32)(p))
#define W(p) CM_GLOBAL(volatile u16,(u32)(p))
#define C(p) CM_GLOBAL(volatile u8,(u32)(p))
#define LD(b) (*(volatile u32 *)(work+(b)-0x10))
#define LW(b) (*(volatile u16 *)(work+(b)-0x10))
#define LB(b) work[(b)-0x10]
#define PTR(b) ((u32)(work+(b)-0x10))
#define SX(v) ((u32)(s32)(s8)(v))
extern u32 CM_THISCALL cm_fn_0054a110(u32);
extern u32 CM_CDECL cm_fn_0053b900(u32);
extern u32 CM_THISCALL cm_fn_00757da0(u32,u32,u32,u32,s16,u32);
extern u32 CM_THISCALL cm_fn_0077bbe0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00785790(u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
/* The two paths using MOVSX DX retain the original EDX high word after
   600A50. This declaration captures physical EDX:EAX, not a semantic64 result. */
extern unsigned long long CM_CDECL cm_fn_00600a50(u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00735870(u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_007751e0(u32,u32);
extern u32 CM_THISCALL cm_fn_0076d940(u32,u32);
extern u32 CM_CDECL cm_fn_007e4750(u32,u32,u32);
extern u32 CM_CDECL cm_fn_007e4de0(u32,u32,u32);
extern u32 CM_CDECL cm_fn_004afd80(u32,u32);
extern u32 CM_THISCALL cm_fn_005fa7d0(u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_007dab90(u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_007861e0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_007862d0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_007863f0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00786640(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00786780(u32,u32);
extern u32 CM_THISCALL cm_fn_00757a50(u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0073e870(u32,u32,u32,u32,u32);
#define SET(k,v) do { u32 setter_value=(u32)(v); a=cm_fn_00785790(PTR(0x38),(u32)(k),setter_value); } while(0)

u32 CM_THISCALL cm_fn_0074b060(u32 self,u32 person,u32 mode,u32 other,u32 data,u32 attribute,u32 last)
{
    volatile struct { u32 prev,handler,state; } frame;
    volatile u8 work[0x128];
    u32 a,c,d,edi=0,ebp,ebx,t,index,target,receiver,local_mode=mode;
    u8 bl,b;
    u16 year;
    unsigned long long physical;
    frame.state=0xffffffffu;
    frame.handler=0x95d883;
    frame.prev=CM_FS(u32,0);
    CM_FS(u32,0)=(u32)&frame;
    LD(0x20)=self;
    LD(0x34)=0;
    LD(0x30)=0xffffffffu;
    LB(0x1b)=0;
    LB(0x1a)=0;
    a=cm_fn_0054a110(PTR(0x118));
    a=D(0xdd6ec8); c=D(0xdd6ecc); year=W(0xae2c92);
    LD(0x28)=a; LD(0x2c)=c;
    t=LW(0x2a);
    frame.state=0;
    if (t>year) goto done;
    if (t==year) {
        a=(u32)((s16)a>(s16)W(0xae2c90));
        if (a!=0) goto done;
    }
    if (person==0) goto done;
    if (D(person+0x57)==0) { a=cm_fn_0053b900(person); if (a==0) goto done; }
    if (D(person+0x53)==0) goto done;
    if (D(person+0x69)==0) { a=cm_fn_0053b900(person); if (a==0) goto done; }
    a=cm_fn_0053b900(person);
    if (a==0) { a=D(person+0x69); if (D(a+0x38)==0) goto done; }
    a=D(person+0xcf); if (a==0) goto done;
    if (D(a+0x69)==0) goto done;
    a=cm_fn_0053b900(person); bl=(u8)local_mode;
    if (a!=0 && bl!=0x5a && bl!=0x44) goto done;
    ebp=other;
    a=cm_fn_00757da0(LD(0x20),ebp,person,0xbdf,(s16)(s8)bl,0);
    if (a!=0) goto done;
    if (ebp!=0) { a=D(ebp+0x61); if (a!=0 && (s16)W(a+0xb)<0xdac) goto done; }
    a=cm_fn_0077bbe0(PTR(0x38),0xbdf,0);
    *(volatile u8 *)&frame.state=1;
    SET(0,0xffffffffu);
    a=SX(bl)-0x3cu;
    if (a>0x27) goto random_default;
    d=C(0x74bb38+a); target=D(0x74bb20+d*4);
    switch(target) {
    case 0x74b227: a=cm_fn_0090cfc0(3); edi=a; break;
    case 0x74b1fd: a=cm_fn_0090cfc0(4); edi=a; break;
    case 0x74b20b:
        if ((s16)W(person+0x80)>0x1482) { a=cm_fn_0090cfc0(2); a=(a&0xffffff00u)|(u8)(a+1); edi=SX(a); }
        else { a=cm_fn_0090cfc0(3); edi=a; }
        break;
    case 0x74b249: a=cm_fn_0090cfc0(3); edi=a; if (edi!=0) ebp=0; break;
    case 0x74b235: a=cm_fn_0090cfc0(2); edi=a; if (edi!=0) ebp=0; break;
    case 0x74b25d: goto random_default;
    default: __builtin_unreachable();
    }
    goto random_done;
random_default:
    edi=0; local_mode=(local_mode&0xffffff00u)|0x63;
    a=cm_fn_0090cfc0(2); if (a!=0) ebp=0; bl=(u8)local_mode;
random_done:
    SET(1,edi);
    a=cm_fn_0090cfc0(0x5dc); c=(u32)(s32)(s16)W(person+0x80); a+=0x1194;
    if ((s32)c<=(s32)a) {
        a=cm_fn_0053b900(person);
        if (a==0) {
            a=cm_fn_0090cfc0(3);
            LD(0x14)=((s32)a>=0 && (s32)a<=1)?2:4;
            goto category_done;
        }
    }
    a=cm_fn_0090cfc0(0x64);
    if ((u8)a==0) LD(0x14)=6;
    else if ((s8)a<8) LD(0x14)=3;
    else if ((s8)a<0x23) LD(0x14)=5;
    else if ((s8)a<0x3c) LD(0x14)=0;
    else {
        a=cm_fn_0053b900(person);
        if (a==0) { a=cm_fn_0090cfc0(3); LD(0x14)=2; if (a==0) goto category_done; }
        LD(0x14)=1;
    }
category_done:
    d=LD(0x14); SET(2,d);
    a=D(person+0x53); c=D(a); SET(3,c);
    d=D(person); SET(4,d);
    edi=SX(bl); SET(9,edi);
    edi-=0x3e; LD(0x28)=edi;
    if (edi<=0x20) {
        a=C(0x74bb68+edi); target=D(0x74bb60+a*4);
        if (target==0x74b3d2) { LD(0x1c)=0; goto decision_done; }
        if (target!=0x74b3a1) __builtin_unreachable();
    }
    physical=cm_fn_00600a50(person,0,1,0,0); a=(u32)physical;
    if (a==0) {
        a=D(person+0xcf);
        d=((u32)(physical>>32)&0xffff0000u)|(u16)(s16)(s8)bl;
        a=cm_fn_00735870(LD(0x20),a,ebp,d); LD(0x1c)=a;
    } else LD(0x1c)=0;
decision_done:
    c=LD(0x1c); SET(0xa,c);
    a=D(person+0xcf); edi=0x1676;
    if ((s8)C(a+0x57)>0xf && (s8)C(a+0x5d)<6) {
        d=D(a+0x69); b=1; if ((s16)W(d+0xa)<=0x1676) b=LB(0x1b);
    } else b=LB(0x1b);
    SET(5,SX(b));
    a=D(person+0xcf);
    if ((s8)C(a+0x57)<0xf && (s8)C(a+0x5d)>0xe) {
        c=D(a+0x69); b=1; if ((s16)W(c+0xa)<=0x1676) b=LB(0x1a);
    } else b=LB(0x1a);
    SET(6,SX(b));
    if (bl==0x5a || bl==0x44) {
        c=D(person+0x53); receiver=D(0xb74318); a=cm_fn_007751e0(receiver,c);
        receiver=D(0xb74318); a=cm_fn_0076d940(receiver,a); if (a==0) goto done;
        d=D(person+0x53); receiver=D(0xb74318); a=cm_fn_007751e0(receiver,d);
        edi=D(a);
    } else if (bl==0x5c) { edi=D(person+0x1db); if (edi==0xffffffffu) goto done; }
    else { a=D(person+0x57); edi=D(a); }
    SET(7,edi);
    a=D(person+0xcf); c=D(a); SET(0xb,c);
    d=D(person+0xcf); a=D(d+4); c=D(a+0x33); SET(0xc,c);
    d=D(person+0xcf); a=D(d+8); c=D(a+0x33); SET(0xd,c);
    d=D(person+0xcf); a=D(d+0xc); c=D(a+0x33); SET(0xe,c);
    if (ebp!=0) {
        d=D(ebp); SET(0xf,d);
        a=D(ebp+4); c=D(a+0x33); SET(0x10,c);
        d=D(ebp+8); a=D(d+0x33); SET(0x11,a);
        c=D(ebp+0xc); d=D(c+0x33); SET(0x12,d);
    } else { SET(0xf,0xffffffffu); SET(0x10,0xffffffffu); SET(0x11,0xffffffffu); SET(0x12,0xffffffffu); }
    ebx=data; LB(0x13)=0x14;
    if (ebx!=0) {
        SET(0x13,1);
        a=cm_fn_007e4750(ebx,person,PTR(0x118));
        a=cm_fn_007e4de0(PTR(0x118),PTR(0x38),PTR(0x13));
    } else SET(0x13,0);
    index=LB(0x13); LB(0x13)=(u8)(index+1);
    a=SX(attribute); LD(0x24)=a; SET(index,a);
    index=LB(0x13); d=D(0xae23d0); LB(0x13)=(u8)(index+1);
    c=edi*107u+d; a=cm_fn_004afd80(c,0);
    d=(u32)(a==person); SET(index,d);
    a=cm_fn_005fa7d0(D(0xb5e134),edi,PTR(0x34),PTR(0x30));
    index=LB(0x13); d=LD(0x34); LB(0x13)=(u8)(index+1); SET(index,d);
    index=LB(0x13); LB(0x13)=(u8)(index+1); a=LD(0x30); SET(index,a);
    index=LB(0x13); d=last; LB(0x13)=(u8)(index+1); SET(index,d);
    if (ebp!=0) {
        d=D(ebp+0x39);
        if (d!=0) {
            index=LB(0x13); LB(0x13)=(u8)(index+1); a=D(d); SET(index,a);
            a=SX(C(ebp+0x3d)); c=a-1;
            if (c<=0xe) {
                d=C(0x74bb94+c); target=D(0x74bb8c+d*4);
                if (target==0x74b6e5) { index=LB(0x13); LB(0x13)=(u8)(index+1); SET(index,a); goto other_boolean; }
                if (target!=0x74b6f5) __builtin_unreachable();
            }
            index=LB(0x13); LB(0x13)=(u8)(index+1); SET(index,0xffffffffu);
            goto other_boolean;
        }
        d=D(ebp+0x24);
        if (d!=0 && C(ebp+0x28)!=0xb) {
            index=LB(0x13); LB(0x13)=(u8)(index+1); a=D(d); SET(index,a);
            index=LB(0x13); d=SX(C(ebp+0x28)); LB(0x13)=(u8)(index+1); SET(index,d);
            goto other_boolean;
        }
    }
    index=LB(0x13); LB(0x13)=(u8)(index+1); SET(index,0xffffffffu);
    index=LB(0x13); LB(0x13)=(u8)(index+1); SET(index,0xffffffffu);
    if (ebp!=0 && C(ebp+0x22)!=0) a=1; else a=0;
    goto boolean_done;
other_boolean:
    bl=C(ebp+0x22); a=(u32)(bl!=0);
boolean_done:
    index=LB(0x13); LB(0x13)=(u8)(index+1); SET(index,a);
    d=data;
    if (d!=0) {
        b=C(d+0x49);
        if (b==0xff) { index=LB(0x13); LB(0x13)=(u8)(index+1); SET(index,0); }
        else {
            u8 second=C(d+0x4a);
            if ((s8)b>(s8)second) {
                a=D(d+0x1c); index=LB(0x13); LB(0x13)=(u8)(index+1); SET(index,(a==person)?1:2);
            } else if ((s8)b<(s8)second) {
                a=D(d+0x20); index=LB(0x13); LB(0x13)=(u8)(index+1); SET(index,(a==person)?1:2);
            }
        }
    }
    a=D(person+0x53);
    if (a!=0) { a=D(a); c=LD(0x14); } else { c=LD(0x14); a=0xffffffffu; }
    a=cm_fn_007dab90(D(0xb74430),person,c,a);
    index=LB(0x13); LB(0x13)=(u8)(index+1); SET(index,a);
    physical=cm_fn_00600a50(person,1,1,0,0); a=(u32)physical;
    if (a==0) {
        a=cm_fn_0090cfc0(2);
        if (a!=0) { a=cm_fn_007861e0(D(0xb7431c),PTR(0x38),person); goto record; }
    }
    c=(u8)local_mode;
    if (c==0x5a || c==0x44) { a=cm_fn_00786780(D(0xb7431c),PTR(0x38)); goto record; }
    if (c==0x5c) goto event_6640;
    a=D(person+0x57);
    if (a==0) goto event_select;
    if ((s16)W(person+0x80)>=0x2328 && (s16)W(a+0x69)>0xf) goto event_6640;
    if (a==0) goto event_select;
    if ((s16)W(a+0x69)<=0xf) goto event_select;
    bl=(u8)attribute;
    if ((s8)bl>0x14) edi=LD(0x24)*30u+0x3e8; else edi=0;
    a=cm_fn_0090cfc0(0xbb8); c=(u32)(s32)(s16)W(person+0x80); d=0x1f40-edi; a+=d;
    if ((s32)c<=(s32)a) goto reload_event;
    b=(u8)local_mode;
    if (b==0x50 || b==0x51) { if ((s8)bl>0xf) goto event_6640; goto event_other; }
    if (b==0x41 || b==0x62 || b==0x63 || b==0x61 || b==0x5d) goto event_6640;
reload_event: c=(u8)local_mode;
event_select:
    if ((u8)c!=0x3d) goto event_other;
    a=D(person+0x57); if (a==0) goto event_default;
    a=cm_fn_0090cfc0(3); if (a!=0) goto event_default;
    d=D(person+0x57); a=cm_fn_007862d0(D(0xb7431c),PTR(0x38),d); goto record;
event_6640:
    d=D(person+0x53); a=D(d+0x71); a=cm_fn_00786640(D(0xb7431c),PTR(0x38),a); goto record;
event_other:
    a=D(person+0x57); if (a==0) goto event_default;
    if ((s16)W(person+0x80)<0x1388) {
        if (a!=0) { a=cm_fn_007862d0(D(0xb7431c),PTR(0x38),a); goto record; }
        goto event_default;
    }
    d=D(person+0x53); a=cm_fn_007863f0(D(0xb7431c),PTR(0x38),d);
    c=D(person+0x53); a=D(0x9cf2e4); c=D(c);
    if (c==a && (s16)W(person+0x80)>=0x1770) {
        a=D(0x9cf45c); d=a*290u; a=D(0xae23a8); c=a+d;
        a=cm_fn_007863f0(D(0xb7431c),PTR(0x38),c); goto record;
    }
    if (c==D(0x9cf45c) && (s16)W(person+0x80)>=0x1964) {
        d=D(0xae23a8); c=a*290u; a=d+c;
        a=cm_fn_007863f0(D(0xb7431c),PTR(0x38),a); goto record;
    }
event_default:
    a=cm_fn_007861e0(D(0xb7431c),PTR(0x38),person);
record:
    edi=LD(0x1c); a=local_mode; ebx=LD(0x20);
    a=cm_fn_00757a50(ebx,ebp,person,0xbdf,a,edi);
    a=LD(0x28);
    if (a<=0x20) {
        c=C(0x74bbac+a); target=D(0x74bba4+c*4);
        if (target==0x74bb03) goto done;
        if (target!=0x74bada) __builtin_unreachable();
    }
    physical=cm_fn_00600a50(person,0,1,0,0); a=(u32)physical;
    if (a!=0) goto done;
    d=((u32)(physical>>32)&0xffff0000u)|(u16)(s16)(s8)local_mode;
    a=cm_fn_0073e870(ebx,person,d,edi,ebp);
done:
    CM_FS(u32,0)=frame.prev;
    return a;
}

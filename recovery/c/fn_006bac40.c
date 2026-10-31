/* Preferred-image VA006BAC40, base00400000; target SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Original ECX receiver, no stack inputs/RET0. regparm3 carries original
   EAX/unusedEDX/ECX so early XOR AL exits retain incoming upper EAX.
   Scratch L0/L4 are the two original DWORDs after SUB8; calculation helpers
   write L4 before any input-record read. All callees remain original.
   Names denote routing slots, not recovered semantic field identities. */
#include "cmre.h"
#define B(p) CM_GLOBAL(volatile u8,(u32)(p))
#define W(p) CM_GLOBAL(volatile u16,(u32)(p))
#define D(p) CM_GLOBAL(volatile u32,(u32)(p))
#define Q(p) CM_GLOBAL(volatile double,(u32)(p))
#define SX(v) ((u32)(s32)(s8)(v))
extern u32 CM_THISCALL cm_fn_006d6f70(u32,u32,u32,u32,u32,u32,u32,u32);
extern void CM_THISCALL cm_fn_006e9630(u32,u32,u32,u32,u32,u32,u32);
extern void CM_THISCALL cm_fn_006ea4e0(u32,u32,u32,u32,u32,u32,u32,u32,u32);
extern void CM_THISCALL cm_fn_006cda20(u32,u32,u32);
/* The float parameter is carried as its original pushed DWORD bits. */
extern void CM_THISCALL cm_fn_00716450(u32,u32);
extern void CM_THISCALL cm_fn_00716e10(u32,u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
#define SEND(code,x,y,z,w) ((void)cm_fn_006d6f70(receiver,(u32)(code),self,(u32)(x),(u32)(y),(u32)(z),(u32)(w),0))

u32 CM_CDECL __attribute__((regparm(3)))
cm_fn_006bac40(u32 eax_in,u32 edx_in,u32 self)
{
    volatile union { u32 words[2]; float floats[2]; } scratch;
    volatile union { u32 word; float value; } argument;
    volatile long double number;
    u32 a,first,second,receiver,notice,code,t,target,state,record,main,i;
    u32 x,y,z,w;
    u8 c,d,byte_value;
    int paired;
    (void)edx_in;
    a=(eax_in&0xffffff00u)|B(self+0x475a);
    if ((u8)a==0) return a;
    if (B(self+0x8eb2)!=0) {
        a=D(self+0x4761);
        d=B(self+0x8eae);
        byte_value=B(a+0x27);
        a=(a&0xffffff00u)|byte_value;
        if ((s8)d==(s32)(byte_value==0)) {
            c=B(self+0x8ea8); d=B(self+0x475c);
            if (((s8)c>(s8)d && byte_value==1) || ((s8)c<(s8)d && byte_value==0)) {
                D(self+0x4761)=0;
                D(self+0x4765)=0;
                W(self+0x475f)=0xffff;
                B(self+0x4769)=0;
                B(self+0x475b)=0xff;
                B(self+0x475c)=0xff;
                B(self+0x475d)=0xff;
                B(self+0x475e)=0xff;
                B(self+0x475a)=0;
                return a&0xffffff00u;
            }
        }
        receiver=D(self+0x8f7a);
        SEND(0x20f0,0xffffffffu,byte_value,0xffffffffu,0xffffffffu);
    }
    state=B(self+0x475a);
    first=D(self+0x4761);
    if (state==8) {
        c=B(first+0x27); d=B(first+0x2e); receiver=D(self+0x8f7a);
        SEND(0x20ee,d,c,0xffffffffu,0xffffffffu);
        a=D(self+0x4761); a=SX(B(a+0x27));
        t=self+a*54u+0xf5ca; c=B(t); B(t)=(u8)(c+1);
        a=D(self+0x4761); c=B(a+0x13); B(a+0x13)=(u8)(c+1);
        d=B(self+0x475b); c=B(self+0x475c); a=D(self+0x4761);
        B(self+0x8ea7)=d; B(self+0x8ea8)=c; B(self+0x8eb2)=7;
        c=B(a+0x27); B(self+0x8eae)=(u8)(c==0);
        D(self+0xf582)=0;
        goto reset;
    }
    if (first==0) goto reset;
    second=D(self+0x4765);
    paired=(second!=0);
    if (!paired) {
        d=B(self+0x4769); c=B(self+0x475e); byte_value=B(self+0x475d);
        cm_fn_006e9630(self+0xf5b2,first,self+0x475a,byte_value,c,d,(u32)&scratch.words[1]);
        a=D(self+0x4761); c=B(a+0x27); d=B(a+0x2e); receiver=D(self+0x8f7a);
        SEND(0x20fb,d,c,0xffffffffu,0xffffffffu);
    } else {
        d=B(self+0x4769); c=B(self+0x475e); byte_value=B(self+0x475d);
        cm_fn_006ea4e0(self+0xf5b2,first,second,self+0x475a,byte_value,c,d,(u32)&scratch.words[1],0);
        a=(u32)(s32)(s16)W(self+0x475f)-1u;
        code=0x20fc;
        if (a<=0x2d) {
            t=B(0x6bb848+a); target=D(0x6bb818+t*4);
            switch(target) {
            case 0x6bb303: code=0x2104; break;
            case 0x6bb378: code=0x2107; break;
            case 0x6bb288: code=0x2101; break;
            case 0x6bb32c: code=0x2103; break;
            case 0x6bb2b1: code=0x2105; break;
            case 0x6bb2da: code=0x2102; break;
            case 0x6bb20d: code=0x20fe; break;
            case 0x6bb236: code=0x20ff; break;
            case 0x6bb25f: code=0x2100; break;
            case 0x6bb1e4: code=0x20fd; break;
            case 0x6bb352: code=0x2106; break;
            case 0x6bb39e: break;
            default: __builtin_unreachable();
            }
        }
        second=D(self+0x4765); first=D(self+0x4761);
        w=B(second+0x27); z=B(second+0x2e); y=B(first+0x27); x=B(first+0x2e);
        receiver=D(self+0x8f7a); SEND(code,x,y,z,w);
    }
    a=SX(B(self+0x475a))-2u;
    notice=0xffffffffu;
    if (a<=(paired?3u:4u)) {
        target=D((paired?0x6bb878u:0x6bb7bcu)+a*4);
        switch(target) {
        case 0x6bae2a: notice=0x1ff9; break;
        case 0x6bb3e3: notice=0x1ff6; break;
        case 0x6bae23: case 0x6bb3ea: notice=0x1ff7; break;
        case 0x6bae31: case 0x6bb3f8: notice=0x1ff8; break;
        case 0x6bb3f1: notice=0x1ff9; break;
        case 0x6bae1c: notice=0x1ff5; break;
        case 0x6bae38: goto choose_extra;
        default: __builtin_unreachable();
        }
    } else {
choose_extra:
        a=cm_fn_0090cfc0(7); c=B(self+0x475e); a+=2;
        if ((s32)SX(c)>(s32)a) notice=0x210a;
    }
    if (B(self+0x475a)==7) {
        code=0x1f79; B(self+0x8eb2)=7;
    } else {
        first=D(self+0x4761);
        d=B(self+0x475c); c=B(self+0x475b);
        byte_value=B(first+0x27);
        if ((s8)c>=2 && (s8)c<=6 &&
            ((byte_value==1 && (s8)d>=0xa && (s8)d<=0xb) ||
             (byte_value!=1 && (s8)d>=0 && (s8)d<=1))) {
            if (paired) {
                second=D(self+0x4765);
                B(self+0x8eb2)=5; D(self+0x478a)=second; code=0x1f7a;
                W(second+0x35)=(u16)(W(second+0x35)+0xfe0c);
            } else {
                B(self+0x8eb2)=5; D(self+0x478a)=0;
                W(first+0x35)=(u16)(W(first+0x35)+0xfe0c); code=0x1f7a;
            }
        } else { code=0x1f79; B(self+0x8eb2)=6; }
    }
    if (!paired) {
        c=B(self+0x475c); d=B(self+0x475b);
        B(self+0x8ea8)=c; first=D(self+0x4761); B(self+0x8ea7)=d;
        byte_value=B(first+0x27); D(self+0xf582)=0;
        B(self+0x8eae)=(u8)(byte_value==0);
    } else {
        c=B(self+0x475b); d=B(self+0x475c); first=D(self+0x4761);
        B(self+0x8ea7)=c; B(self+0x8ea8)=d;
        byte_value=B(first+0x27); D(self+0xf582)=0;
        B(self+0x8eae)=(u8)(byte_value==0);
    }
    if (code==0x1f7a) {
        a=paired?D(self+0x4765):first; c=B(a+0x27); receiver=D(self+0x8f7a);
        y=paired?(u32)c:(u32)(c==0);
        SEND(0x1f7a,0xffffffffu,y,0xffffffffu,0xffffffffu);
    }
    if ((s16)notice>-1) {
        a=cm_fn_0090cfc0(4);
        if (a==0) {
            a=SX(B(self+0x475e)); t=cm_fn_0090cfc0(a); a=cm_fn_0090cfc0(0x14);
            if ((s32)t>(s32)a) {
                receiver=D(self+0x8f7a); SEND(0x20f5,0,0,0xffffffffu,0xffffffffu);
            }
            a=D(self+0x4761); c=B(a+0x27); d=B(a+0x2e); receiver=D(self+0x8f7a);
            SEND(0x2109,d,c,0xffffffffu,0xffffffffu);
        }
        a=D(self+0x4761); c=B(a+0x27); d=B(a+0x2e); receiver=D(self+0x8f7a);
        SEND(notice,d,c,0xffffffffu,0xffffffffu);
        state=B(self+0x475a);
        if (state==6 || state==3 || state==4 || state==5) {
            a=D(self+0x4761); cm_fn_006cda20(self,a,0);
        }
    }
    if (code!=0x1f7a) {
        a=D(self+(paired?0x4765u:0x4761u)); c=B(a+0x27); receiver=D(self+0x8f7a);
        y=paired?(u32)c:(u32)(c==0); SEND(code,0xffffffffu,y,0xffffffffu,0xffffffffu);
    }
    a=SX(B(self+0x475f))-0x25u;
    if (a<=0x3c) {
        t=B((paired?0x6bb890u:0x6bb7d8u)+a);
        target=D((paired?0x6bb888u:0x6bb7d0u)+t*4);
        if (target==0x6bb772) goto reset;
        if (target!=(paired?0x6bb5d4u:0x6bb013u)) __builtin_unreachable();
    }
    a=cm_fn_0090cfc0(9);
    if (a==0) {
        a=D(self+(paired?0x4765u:0x4761u)); receiver=D(self+0x8f7a); c=B(a+0x27);
        y=paired?(u32)(c==0):(u32)c; SEND(0x20df,0xffffffffu,y,0xffffffffu,0xffffffffu);
        a=cm_fn_0090cfc0(3);
        if (a==0) {
            a=D(self+0x4761); c=B(a+0x27); receiver=D(self+0x8f7a);
            y=paired?(u32)(c==0):(u32)c; SEND(0x20e0,0xffffffffu,y,0xffffffffu,0xffffffffu);
        }
        state=B(self+0x475a);
        if (state==6 || state==3 || state==4 || state==5 || (u16)code==0x1f7a) {
            a=D(self+0x4761); B(a+0x16)=1;
            a=cm_fn_0090cfc0(7);
            if (a==0) {
                a=D(self+0x4761); c=B(a+0x27); receiver=D(self+0x8f7a);
                y=paired?(u32)(c==0):(u32)c; SEND(0x20d9,0xffffffffu,y,0xffffffffu,0xffffffffu);
            }
        }
    }
    main=D(self+0x4761);
    scratch.words[0]=scratch.words[1];
    c=B(main+0x27);
    record=self+SX(c)*8920u+0x4796;
    for(i=0;i<20;i++) {
        a=D(record+0x69);
        if (a!=0) {
            if (record==main) argument.word=scratch.words[0];
            else {
                c=B(record+0x19);
                number=(long double)scratch.floats[0];
                if ((s8)c<0) {
                    number=number*(long double)Q(0x96a7a8);
                    number=number+(long double)Q(0x96d0e8);
                } else {
                    number=number+(long double)Q(0x967890);
                    number=number*(long double)Q(0x968968);
                }
                argument.value=(float)number;
            }
            cm_fn_00716450(record,argument.word);
        }
        record+=0x1be;
    }
    a=D(self+0x4761); cm_fn_00716e10(a,0);
reset:
    W(self+0x475f)=0xffff;
    D(self+0x4761)=0;
    D(self+0x4765)=0;
    B(self+0x4769)=0;
    B(self+0x475a)=0;
    B(self+0x475b)=0xff;
    B(self+0x475c)=0xff;
    B(self+0x475d)=0xff;
    B(self+0x475e)=0xff;
    return 1;
}

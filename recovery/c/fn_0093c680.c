/* Preferred-image VA 0093C680, base 00400000; CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL2: arrays, object. Original calls and allocator remain boundaries.
   Native B=L-10 after SUB70 and four saves; vXX names original B+XX slots.
   All external memory operations retain native widths and order. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
#define SW(a) ((u32)(s32)(s16)W(a))
#define SAR(a,n) ((u32)((s32)(a)>>(n)))
extern void CM_CDECL cm_fn_005a8f60(u32);
extern u32 CM_CDECL cm_fn_009443a0(u32,u32);
extern u32 CM_CDECL cm_fn_009445d0(u32,u32);
extern void CM_CDECL cm_fn_00934ab0(u32);
extern void CM_CDECL cm_fn_00934ae0(u32);
extern void CM_CDECL cm_fn_009340a0(u32);
extern u16 CM_CDECL cm_fn_00933b20(u32,u32);
extern void CM_CDECL cm_fn_00933da0(u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32);
/* arg4 is not consumed; selector and the final three indices are words. */
extern void CM_CDECL cm_fn_00933e50(u32,u32,u16,u32,u16,u16,u16);
extern void CM_CDECL cm_fn_00933fa0(u32,u32,u32,u32,u32,u32);
extern void CM_CDECL cm_fn_009340d0(u32,u32,u16,u16,u16);
extern void CM_CDECL cm_fn_009341e0(u32,u32,u16,u16,u16);
extern void CM_CDECL cm_fn_00934220(u32,u32,u16,u16,u16);
extern void CM_CDECL cm_fn_00934260(u32,u32,u16,u16,u16,u16);
extern u32 CM_CDECL cm_fn_009342c0(u32,u32,u32,u32,u32,u32,u32,u32,u32,u32);
extern void CM_CDECL cm_fn_009345e0(u32,u32);
extern void CM_CDECL cm_fn_009347a0(u32,u32);
extern void CM_CDECL cm_fn_0093d780(u32,u32,u32,u32);
typedef void (CM_CDECL *call4)(u32,u32,u16,u32);
typedef void (CM_CDECL *call5word)(u32,u32,u16,u32,u16);
typedef void (CM_CDECL *call5)(u32,u32,u32,u32,u32);

/* Literal 93CA6E..93CC37 polynomial; each multiply/LEA wraps to DWORD,
   every SAR applies after that wrap. Inputs have already been loaded. */
static u32 polynomial(u32 cap,u32 last,u32 count)
{
    u32 a,c,d,si,di,bp,bx,t38,t6c,t70;
    c=cap*354u-0x4e9u;
    si=last*18u-0x55cu;
    d=(count&0x3fu)*81u-0x4f6u;
    bx=si*c; a=c*c; di=d*d; t70=SAR(di,10);
    di=SAR(d*si,10); bx=SAR(bx,10);
    bp=di*370u; t38=bx; t6c=SAR(a,10);
    a=si*si; bp=SAR(bp,10);
    di=SAR(bx*d,10); bx=SAR(di*78u,10);
    di=SAR(t70*d,10); bp-=bx;
    di=SAR(di*87u,10); a=SAR(a,10); bp+=di;
    di=SAR(d*c,10); bp+=SAR(di*100u,10);
    di=SAR(t6c*c,10); bp+=SAR(di*57u,10);
    bp-=SAR(t70*685u,10);
    bp-=SAR(t6c*307u,10);
    di=a*140u; a*=si; bp-=SAR(di,10);
    bp+=SAR(a,18); bp+=SAR(d*940u,10);
    si=SAR(si,3); bp+=SAR(c*364u,10); bp-=si;
    a=SAR(t38,4)+bp+0x237u;
    if((s32)a>0 && (s32)a<0x14d && (s32)cap<=3 &&
       (count&0x3fu)<=14 && (s32)last<=0x56 &&
       (s32)(cap*5u)>(s32)(count&0x3fu)) a=0;
    return (s32)a>0;
}

u32 CM_CDECL cm_fn_0093c680(u32 arrays,u32 object)
{
    u32 a,c,d,si,di,bx,p,q,r,temp;
    u32 v14,v18,v1c,v20,v24,v28,v2c,v30,v34,v38,v3c,v40,v44;
    u32 v48,v4c,v50,v54=0,v58,v5c,v60,v64,v68=0,v6c,v70,v74,v78,v7c;
    u8 v13;
    v2c=D(object+0x28);
    c=SW(object+0x29c);
    c=(u32)((s32)c/100);
    v74=(u32)((s32)c/10)+1u; v78=c*3u+1u;
    d=SW(object+4); a=D(object+0xa); v4c=D(object+0x3c);
    v24=SW(a+d*2u-2u); d=D(object+0x444); si=v24*2u+0x12u;
    W(object+0x3fc)=0; W(object+0x3fa)=0; W(object+0x3f8)=0;
    W(object+0x43e)=1; W(object+0x440)=0;
    v40=cm_fn_009443a0(d,si*14u);
    c=D(object+0x444); bx=cm_fn_009443a0(c,si*4u); v58=bx;
pass:
    v48=0; v18=0;
    cm_fn_005a8f60(object);
    if(v68==0){
        if(W(object+0x1a)!=0 || D(object+0x2a6)!=1){
            si=(u32)(s32)(s16)(v24+1u);
            ((call4)0x934440)(object,arrays,1,si);
            c=(u32)(s32)(s16)(v24+2u);
            cm_fn_009342c0(object,arrays,0xffffffffu,0,0,1,0x47,0x72,si,c);
        }
        v18=1; cm_fn_00934ab0(object);
        d=D(object+0x12); a=D(object+0x16);
        v30=0x10; v64=1; v28=4; v70=0x5000; v13=1; v1c=d; v44=a;
    }else{
        v48=1; cm_fn_00934ae0(object); cm_fn_00934ab0(object);
        c=D(object+0x16); d=D(object+0x12);
        v30=0x20; v64=2; v28=8; v70=0x2800; v13=2; v1c=c; v44=d;
    }
    a=D(object+0x50); si=0; di=0; v50=0; v20=0;
    if((s32)a>0){
        c=v40; v50=bx;
        do{
            a=D(object+0x58); temp=B(a+di*8u); a+=di*8u;
            if(temp==3 && B(a+1)==v13){
                a=v50; D(a)=c;
                d=D(object+0x58); a=W(d+di*8u+4); W(c)=(u16)a;
                d=D(object+0x58); a=(u32)(s32)(s16)a; d=SW(d+di*8u+6);
                v5c=d; a*=2u; di=SW(a+v1c); v6c=di;
                di=SW(v1c+d*2u); d=v6c-di; v60=d;
                if((s32)d<0) v60=0u-d;
                d=v44; di=v5c; a=SW(a+d); d=SW(d+di*2u); a-=d;
                if((s32)a<0) a=0u-a;
                d=v60; a=SAR(a,3);
                a=(s32)d>(s32)a?di:0xffffffffu;
                d=v50; di=v20; W(c+2)=(u16)a; d+=4;
                W(c+0xc)=0; ++si; v50=d; c+=14;
            }
            a=D(object+0x50); ++di; v20=di;
        }while((s32)di<(s32)a);
        v50=si;
    }
    a=v1c; c=0; d=0; v60=0;
    a=SW(a); v6c=a; v20=a; v5c=0; v14=0;
    if((s32)v24>=0){
        a=v40+si*14u; v38=bx+si*4u; d=v2c;
        v34=d; v3c=v1c-d;
        do{
            if((u16)v48!=0 && (B(d)&0x40)!=0){
                di=v38; ++si; D(di)=a; W(a)=(u16)c; W(a+2)=0xffff;
                di+=4; W(a+0xc)=4; v38=di; a+=14;
            }
            di=W(d); v50=di; di&=v28;
            if((u16)di!=0){
                ++v14; d=SW(v3c+d);
                if((s32)d>(s32)v20){v20=d; v5c=c;}
                else if((s32)d<(s32)v6c){v6c=d; v60=c;}
                d=v50&v70;
                if((u16)d!=0){
                    d=v38; ++si; D(d)=a; W(a)=(u16)c; W(a+2)=0xffff;
                    d+=4; W(a+0xc)=0; v38=d; a+=14;
                }
                d=v34;
            }
            ++c; d+=2; v34=d;
        }while((s32)c<=(s32)v24);
        di=v40; d=v14; v50=si;
    }else di=v40;
    if((u16)v18!=0 && D(object+0x2a6)==2){
        c=SW(arrays); v54=c;
        if((s32)c>16){c=16;v54=c;}
        a=v24; v3c=a;
        if((s32)a>0x12f){a=0x12f;v3c=a;}
        v34=d&0x3f;
        v54=polynomial(c,a,v34);
        si=v50; bx=v58; di=v40;
    }
    c=di+si*14u; a=v54!=0?4:0;
    D(bx+si*4u)=c; W(c)=(u16)v60; ++si;
    W(c+2)=0xffff; d=v5c; W(c+0xc)=(u16)a;
    c=di+si*14u; ++si; D(bx+si*4u-4)=c;
    W(c)=(u16)d; W(c+2)=0xffff; W(c+0xc)=(u16)a;
    if((u16)v18!=0 && W(object+0x1a)==0 && D(object+0x2a6)==1){
        c=v24; a=di+si*14u; d=c+1u; c+=2;
        D(bx+si*4u)=a; W(a)=(u16)d; ++si; W(a+2)=0xffff; W(a+0xc)=0;
        a=di+si*14u; ++si; D(bx+si*4u-4)=a;
        W(a)=(u16)c; W(a+2)=0xffff; W(a+0xc)=0;
    }
    a=si-2u; d=(s32)a<0?0xffffffffu:0;
    di=SAR(a-d,1);
    while((s32)di>=0){cm_fn_0093d780(bx,di,si-1u,v1c);--di;}
    di=si-1u;
    while((s32)di>0){
        d=D(bx); a=D(bx+di*4u); D(bx+di*4u)=d; D(bx)=a;
        --di; cm_fn_0093d780(bx,0,di,v1c);
    }
    a=v24+2u;
    if((s32)a>=0){
        di=v4c; c=(a+1u)>>1;
        while(c!=0){D(di)=0xffffffffu;di+=4;--c;}
        if(((a+1u)&1u)!=0) W(di)=0xffff;
    }
    d=D(bx); v14=0; W(d+6)=0;
    c=D(bx); d=SW(c); c=v4c; W(c+d*2u)=0;
    if((s32)si>1){
        d=si-1u; a=bx+4; v70=d;
        do{
            c=D(a); d=D(a-4); di=SW(d); c=SW(c);
            d=v1c; di=SW(d+di*2u); c=SW(d+c*2u); c-=di;
            if((s32)c<0)c=0u-c;
            if((s32)c>(s32)v74)++v14;
            c=D(a);d=v14;di=v4c;a+=4;W(c+6)=(u16)d;
            c=D(a-4);c=SW(c);W(di+c*2u)=(u16)d;
        }while(--v70!=0);
    }
    if((s32)si>0){
        di=bx;v70=si;
        do{
            c=D(di);a=W(c+2);
            if((s16)a>=0){d=(u32)(s32)(s16)a;a=SW(v4c+d*2u);}
            else a=0xffffffffu;
            W(c+8)=(u16)a;a=D(di);
            if((B(a+0xc)&4)!=0){
                if(v54!=0){d=D(di);W(d+4)=9999;}
                else{c=SW(a);a=cm_fn_00933b20(object,c);d=D(di);W(d+4)=(u16)a;}
            }else W(a+4)=0xffff;
            di+=4;
        }while(--v70!=0);
    }
    v14=0;v20=1;
    if((s32)si>1){
        c=4;v34=c;
merge:
        a=D(bx+v14*4u);d=D(c+bx);v70=a;v6c=d;
        if(W(a+6)!=W(d+6))goto merge_advance;
        c=SW(d);a=SW(a+2);di=v44;
        c*=2;a*=2; temp=(s16)W(c+di)>(s16)W(a+di);
        d=v1c; q=W(c+d);r=W(a+d);di=temp;c=v34;
        if((u16)q!=(u16)r){
            if(di!=0){if(c!=0 && v20!=si-1u)goto merge_choice;goto merge_into_b;}
            else{a=v14;if(a==0 || a==si-1u)goto merge_keep_a;c=v34;goto merge_into_b;}
        }else c=v34;
merge_choice:
        if(di!=0){a=v14;goto merge_keep_a;}
merge_into_b:
        a=v14;d=v70;bx=v58;temp=W(d+0xc);d=v6c;W(d+0xc)|=(u16)temp;
        d=D(c+bx);
        if((s16)W(d+4)<0){di=D(bx+a*4u);temp=W(di+4);W(d+4)=(u16)temp;}
        d=D(bx+a*4u);temp=W(d+8);
        if((s16)temp>=0){d=D(c+bx);if((s16)W(d+8)<0)W(d+8)=(u16)temp;}
        a=D(bx+a*4u);B(a+0xc)|=1;
merge_advance:
        a=v20;v14=a;goto merge_next;
merge_keep_a:
        d=v6c;bx=v58;temp=W(d+0xc);d=v70;W(d+0xc)|=(u16)temp;
        d=D(bx+a*4u);
        if((s16)W(d+4)<0){di=D(c+bx);temp=W(di+4);W(d+4)=(u16)temp;}
        d=D(c+bx);temp=W(d+8);
        if((s16)temp>=0){d=D(bx+a*4u);if((s16)W(d+8)<0)W(d+8)=(u16)temp;}
        d=D(c+bx);B(d+0xc)|=1;
merge_next:
        c+=4;++v20;v34=c;
        if((s32)v20<(s32)si)goto merge;
    }
    di=0;v14=0;
    if((s32)si>0){
        c=bx;d=bx;v70=si;
        do{
            a=D(c);
            if((B(a+0xc)&1)==0){W(a+0xa)=0xffff;a=D(c);D(d)=a;++di;d+=4;}
            else{a=SW(a);si=v2c;a=si+a*2u;W(a)|=(u16)v64;}
            c+=4;
        }while(--v70!=0);
        v14=di;
    }
    if((s32)di>0){
        a=0;d=bx;
        do{
            c=D(d);temp=W(c+8);
            if((s16)temp>=0){c=(u32)(s32)(s16)temp;c=D(bx+c*4u);si=SW(c+8);if(si!=a)W(c+0xa)=(u16)a;}
            ++a;d+=4;
        }while((s32)a<(s32)di);
        if((s32)di>0){
            a=D(bx);v38=0;v3c=0;c=W(a);temp=W(a+4);
            si=(u32)(s32)(s16)c;
            if((s16)temp>=0)((call5word)((u16)v18?0x934480:0x9344e0))(object,arrays,1,si,(u16)temp);
            else ((call4)((u16)v18?0x934440:0x934460))(object,arrays,1,si);
            a=v2c;W(a+si*2u)|=(u16)v30;a=D(bx);B(a+0xc)|=2;
            cm_fn_009340a0(object);si=0;v20=0;
            if((s32)di>0)goto process;
        }
    }
    goto pass_end;
process_reload:
    si=v20;
process:
    a=D(bx+si*4u);d=0xffffffffu;v28=0xffffffffu;
    v34=(u16)W(a);c=SW(a+8);
    if((s32)c>=0){di=D(bx+c*4u);if((B(di+0xc)&2)!=0)d=c;else v28=c;}
    c=SW(a+0xa);
    if((s32)c>=0){
        di=D(bx+c*4u);
        if((B(di+0xc)&2)!=0){if((s32)d<0 || (s32)d<=(s32)c)d=c;}
        else if((s32)v28<0 || (s32)v28>=(s32)c)v28=c;
    }
    if((B(a+0xc)&2)==0){
        if((s32)d>=0){
            c=D(bx+d*4u);a=(u32)(s32)(s16)v34;d=SW(c);
            cm_fn_00933da0(object,arrays,v1c,v18,v48,0x40,1,0x42,0x6c,d,a);
        }else{
            a=D(bx+si*4u-4);a=(u16)W(a);v50=a;
            if((s32)si>1){
                c=D(bx+si*4u-8);di=v1c;a=(u32)(s32)(s16)a;
                temp=W(c);c=(u32)(s32)(s16)v34; c*=2;a*=2;v6c=temp;
                d=SW(c+di);di=SW(a+di);d-=di;if((s32)d<0)d=0u-d;
                di=v44;c=SW(c+di);di=SW(a+di);a=c-di;v70=a;
                if((s32)a<0)v70=0u-a;
                a=(u32)(s32)(s16)v6c;di=v44;a=SW(di+a*2u);c-=a;
                if((s32)c<0)c=0u-c;
                if((s32)d>(s32)v78){
                    a=v70;d+=d;
                    if((s32)a>(s32)d && (s32)c<(s32)(a/8u)){a=v6c;goto chosen_previous;}
                }
                a=v50;
            }
chosen_previous:
            c=v34;d=v38;temp=D(bx+d*4u);temp=W(temp);
            cm_fn_00933e50(object,arrays,(u16)v18,v48,(u16)temp,(u16)a,(u16)c);
        }
        d=D(bx+si*4u-4);c=v34;temp=W(d);
        cm_fn_009341e0(object,arrays,(u16)v18,(u16)temp,(u16)c);
        a=v3c;
        if((s32)a>(s32)si){d=v34;c=D(bx+a*4u);temp=W(c);cm_fn_00934220(object,arrays,(u16)v18,(u16)temp,(u16)d);}
        c=(u32)(s32)(s16)v34;d=v2c;a=d+c*2u;W(a)|=(u16)v30;
        a=D(bx+si*4u);B(a+0xc)|=2;
    }
    d=D(bx+si*4u);a=W(d+4);
    if((s16)a>=0 && si!=v38){
        cm_fn_009340d0(object,arrays,(u16)v18,(u16)a,(u16)v34);
        a=D(object+0x432);
        if(a!=0){
            c=v14;a=v38;v3c=a;si=c-1u;v20=si;
            if((s32)si<=(s32)a)goto process;
            c=si;d=bx+si*4u;c-=a;si-=c;v20=si;
            do{a=D(d);d-=4;W(a+0xc)&=0xfffd;}while(--c!=0);
            goto process;
        }
        a=D(object+0x42e);
        if(a!=0){
            a=v38;d=D(bx+a*4u);++a;c=SW(d);d=D(bx+si*4u);v7c=c;
            c=SW(d);v38=c;
            if((s32)a<(s32)si){
                c=si;d=bx+a*4u;c-=a;v50=d;v6c=c;
                do{
                    a=v50;c=D(a);d=SW(c);v70=d;c=v38;d=v70;
                    ((call5)((u16)v18?0x934540:0x934590))(object,arrays,v7c,d,c);
                    a=D(object+0x448);
                    if(a==0)((call4)((u16)v18?0x934440:0x934460))(object,arrays,1,v70);
                    v50+=4;
                }while(--v6c!=0);
            }
        }
        v38=si;
    }
    a=v34;v50=a;a=v28;
    if((s32)a>=0){
        c=a;a=D(bx+c*4u);
        if((B(a+0xc)&2)==0){
            temp=W(a);d=(u32)(s32)(s16)v50;si=(u32)(s32)(s16)temp;
            cm_fn_00933da0(object,arrays,v1c,v18,v48,0x40,1,0x42,0x6c,d,si);
            c=v28;d=v30;a=D(bx+c*4u);B(a+0xc)|=2;a=v2c;W(a+si*2u)|=(u16)d;
            a=v3c;
            if((s32)c>(s32)a){
                si=v38;v3c=c;a=c;
                if((s32)c>(s32)si){
                    d=bx+c*4u;
                    do{di=D(d);if((s16)W(di+4)>=0)break;--a;d-=4;}while((s32)a>(s32)si);
                }
                --a;
                if((s32)a>(s32)si){
                    d=bx+a*4u;a-=si;
                    do{di=D(d);d-=4;W(di+4)=0xffff;}while(--a!=0);
                }
            }else{
                d=v3c;temp=D(bx+d*4u);temp=W(temp);
                cm_fn_00934260(object,arrays,(u16)v18,(u16)temp,(u16)si,(u16)v50);
                c=v28;
            }
            a=v20;d=D(bx+a*4u);++a;d=SW(d);v6c=d;
            d=D(bx+c*4u);d=SW(d);v7c=d;
            if((s32)a<(s32)c){
                c-=a;di=bx+a*4u;v70=c;
                do{
                    a=D(di);temp=B(a+0xc);si=SW(a);
                    if((temp&2)==0 && (s16)W(a+8)<0 && (s16)W(a+0xa)<0){
                        ((call5)((u16)v18?0x934540:0x934590))(object,arrays,v6c,si,v7c);
                        ((call4)((u16)v18?0x934440:0x934460))(object,arrays,1,si);
                        a=D(di);B(a+0xc)|=2;a=v2c;W(a+si*2u)|=(u16)v30;
                    }
                    di+=4;
                }while(--v70!=0);
            }
        }
    }
    ++v20;c=v14;
    if((s32)v20<(s32)c)goto process_reload;
    di=c;
    if((u16)v18!=0 && W(object+0x1a)==0 && D(object+0x2a6)==1){
        c=v24;d=v5c;a=c+2u;++c;
        cm_fn_00933fa0(object,arrays,c,v60,d,a);
    }
    d=D(bx);a=W(d);
    if((s32)di>1){
        c=bx+4;--di;v5c=c;v6c=di;
        do{
            d=v5c;v70=a;si=0;a=D(d);c=W(a);a=v24;v60=c;
            if((s32)a>=0){
                di=v2c;a=v1c-di;v3c=a;
                do{
                    c=W(a+di);a=W(di);d=a&v64;
                    if((u16)d!=0 && (u16)(a&v30)==0){
                        a=(u32)(s32)(s16)v70;d=v1c;
                        if((s16)c>=(s16)W(d+a*2u)){
                            a=(u32)(s32)(s16)v60;
                            if((s16)c<=(s16)W(d+a*2u)){
                                a=D(object+0x20);c=D(object+0x1c);temp=W(a+si*2u);d=W(c+si*2u);
                                c=v2c;v7c=d;a=(u32)(s32)(s16)temp;a*=2;
                                temp=W(a+c);c=v30;
                                if((u16)(temp&c)!=0){c=W(v3c+di);d=v1c;if((u16)c==W(a+d))goto fill_next;}
                                a=(u32)(s32)(s16)v7c;c=v2c;a*=2;temp=W(a+c);c=v30;
                                if((u16)(temp&c)!=0){c=W(v3c+di);d=v1c;if((u16)c==W(a+d))goto fill_next;}
                                c=(u32)(s32)(s16)v60;d=(u32)(s32)(s16)si;p=(u32)(s32)(s16)v70;
                                ((call5)((u16)v18?0x934540:0x934590))(object,arrays,p,d,c);
                                W(di)|=(u16)v30;
                            }
                        }
                    }
fill_next:
                    a=v24;++si;di+=2;
                    if((s32)si>(s32)a)break;
                    a=v3c;
                }while(1);
            }
            v5c+=4;--v6c;a=v60;
        }while(v6c!=0);
    }
pass_end:
    ++v68;
    if((s32)v68<2)goto pass;
    cm_fn_009345e0(object,arrays);cm_fn_009347a0(object,arrays);
    c=v40;d=D(object+0x444);cm_fn_009445d0(d,c);
    a=D(object+0x444);cm_fn_009445d0(a,bx);
    return 0;
}

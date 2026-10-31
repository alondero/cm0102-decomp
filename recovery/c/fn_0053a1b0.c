#include "cmre.h"
/* Preferred-image VA0053a1b0/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL1/full pointer EAX. Original DWORD table53a528/32 entries retained.
   Actual allmul/alldiv are STD4/RET16, returning physical EDX:EAX.
   Preserve cached versus reloaded bases, signed comparisons, RNG order,
   original count-slot reassignment and the accepted final fallback candidate. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_CDECL cm_fn_00946a04(void);
extern unsigned long long CM_STDCALL cm_fn_00945e40(u32,u32,u32,u32);
extern unsigned long long CM_STDCALL cm_fn_00945e80(u32,u32,u32,u32);

u32 CM_CDECL cm_fn_0053a1b0(volatile u32 input_arg)
{
    u8 scratch[4];
    u32 l=(u32)scratch,input,base,statistics,id,count,start,eax,offset;
    u32 target,global,candidate,counter,factor,position,threshold;
    unsigned long long product;
    u8 score,best=0xff,attempt,kind,other;
    input=input_arg;D(l)=0;
    if (!input) goto any;
    id=D(input);statistics=D(0x00ae2338);position=statistics+id*36u;
    count=D(position+0x1c);
    if ((s32)count>100) {
        start=D(position+4);input_arg=20;
        do {
            eax=cm_fn_00946a04();
            product=cm_fn_00945e40(eax,(s32)eax<0?0xffffffffu:0,
                count,(s32)count<0?0xffffffffu:0);
            eax=(u32)cm_fn_00945e80((u32)product,(u32)(product>>32),0x8000,0);
            base=D(0x00ae23f4);eax+=start;
            score=B(base+eax*60u+0x3b);
            if ((s8)score>(s8)best) {best=score;D(l)=eax;}
            counter=input_arg;--counter;input_arg=counter;
        } while (counter);
        eax=D(l);return base+eax*60u;
    }
    kind=B(input+0x77);if(!kind) goto any;
    offset=(u32)(s32)(s8)kind-1u;
    if (offset>31) goto choose_country;
    target=D(0x0053a528+offset*4u);
    switch (target) {
    case 0x0053a2d7:global=0x009cf204;break;
    case 0x0053a2f1:global=0x009cf458;break;
    case 0x0053a2fb:global=0x009cf298;break;
    case 0x0053a305:global=0x009cf2b8;break;
    case 0x0053a30f:global=0x009cf338;break;
    case 0x0053a319:global=0x009cf2e4;break;
    case 0x0053a323:global=0x009cf2fc;break;
    case 0x0053a32d:global=0x009cf300;break;
    case 0x0053a337:global=0x009cf314;break;
    case 0x0053a341:global=0x009cf31c;break;
    case 0x0053a34b:global=0x009cf35c;break;
    case 0x0053a355:global=0x009cf344;break;
    case 0x0053a35f:global=0x009cf360;break;
    case 0x0053a369:global=0x009cf36c;break;
    case 0x0053a373:
        eax=cm_fn_00946a04();
        product=cm_fn_00945e40(eax,(s32)eax<0?0xffffffffu:0,2,0);
        eax=(u32)cm_fn_00945e80((u32)product,(u32)(product>>32),0x8000,0);
        global=eax?0x009cf484:0x009cf404;break;
    case 0x0053a3a4:global=0x009cf430;break;
    case 0x0053a3ab:global=0x009cf434;break;
    case 0x0053a3b2:global=0x009cf444;break;
    case 0x0053a3b9:global=0x009cf448;break;
    case 0x0053a3c0:global=0x009cf408;break;
    case 0x0053a3c7:global=0x009cf2ac;break;
    case 0x0053a3ce:global=0x009cf488;break;
    case 0x0053a3d5:global=0x009cf4dc;break;
    case 0x0053a3dc:global=0x009cf270;break;
    case 0x0053a3e3:global=0x009cf3b8;break;
    case 0x0053a3ea:global=0x009cf428;break;
    case 0x0053a3f1:global=0x009cf470;break;
    case 0x0053a3f8:global=0x009cf4a8;break;
    case 0x0053a3ff:global=0x009cf348;break;
    case 0x0053a406:global=0x009cf2c0;break;
    default:__builtin_unreachable();
    }
    id=D(global);base=D(0x00ae23a8);candidate=base+id*290u;
    if (!candidate) goto choose_country;
    eax=cm_fn_00946a04();
    product=cm_fn_00945e40(eax,(s32)eax<0?0xffffffffu:0,20,0);
    eax=(u32)cm_fn_00945e80((u32)product,(u32)(product>>32),0x8000,0);
    statistics=D(0x00ae2338);threshold=eax+15;id=D(candidate);
    if ((s32)D(statistics+id*36u+0x1c)<(s32)threshold) goto choose_country;
    goto selected;
choose_country:
    attempt=0;
    do {
        eax=cm_fn_00946a04();factor=D(0x00ae2350);
        product=(unsigned long long)((long long)(s32)eax*(long long)(s32)factor);
        eax=(u32)cm_fn_00945e80((u32)product,(u32)(product>>32),0x8000,0);
        base=D(0x00ae23a8);offset=eax*290u;
        kind=B(base+offset+0x77);candidate=base+offset;
        other=B(input+0x77);statistics=D(0x00ae2338);
        if (kind==other) {
            id=D(candidate);
            if ((s32)D(statistics+id*36u+0x18)>5) break;
        }
        attempt=(u8)(attempt+1);
    } while ((s8)attempt<40);
    if (!candidate) goto any;
selected:
    id=D(candidate);position=statistics+id*36u;count=D(position+0x1c);
    if ((s32)count<5 || !candidate) goto any;
    start=D(position+4);eax=cm_fn_00946a04();
    product=(unsigned long long)((long long)(s32)eax*(long long)(s32)count);
    eax=(u32)cm_fn_00945e80((u32)product,(u32)(product>>32),0x8000,0);
    eax+=start;base=D(0x00ae23f4);return base+eax*60u;
any:
    factor=D(0x00ae239c);if((s32)factor>0x7fff) factor=0x7fff;
    eax=cm_fn_00946a04();
    product=(unsigned long long)((long long)(s32)eax*(long long)(s32)factor);
    eax=(u32)cm_fn_00945e80((u32)product,(u32)(product>>32),0x8000,0);
    base=D(0x00ae23f4);return base+eax*60u;
}

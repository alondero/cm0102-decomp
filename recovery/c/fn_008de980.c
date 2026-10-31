/* Preferred-image VA 008DE980, base 00400000; CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   RET0C; three stack arguments. Original callees remain boundaries.
   The two text buffers occupy B+30 and B+15C (B=L-10 after SUB1B0).
   Preserve their adjacency and leave them uninitialized. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32,(a))
#define SB(a) ((s32)CM_GLOBAL(volatile s8,(a)))
extern u32 CM_THISCALL cm_fn_00785830(u32,u32);
extern u32 CM_CDECL cm_fn_0053e140(u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0053e990(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_006181d0(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32,u32,...);
extern u32 CM_THISCALL cm_fn_004ae660(u32,u32);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32,u32,u32,u32);

u32 CM_STDCALL cm_fn_008de980(u32 news,u32 mode,u32 output)
{
    u8 scratch[400];
    u32 text=(u32)scratch, extra=text+300u;
    u32 si,person,first,second,third,bp,di,kind,flag,fmt;
    u32 base,s,b,t,offset,result;
    s32 vs,vb,vt;
    si=cm_fn_00785830(news,1);
    person=cm_fn_00785830(news,2);
    first=cm_fn_00785830(news,3);
    second=cm_fn_00785830(news,4);
    third=cm_fn_00785830(news,5);
    flag=(u8)cm_fn_00785830(news,8);
    if(mode==0) goto mode_zero;
    bp=cm_fn_00785830(news,0);
    di=cm_fn_00785830(news,6);
    kind=(u16)cm_fn_00785830(news,7);
    cm_fn_0053e140(first,second,third,text,300,1,person);
    if(kind==0x0b){
        if(flag==0) fmt=bp==0xffffffffu?0xa9b608:0xa9b6bc;
        else fmt=bp==0xffffffffu?0xa9b47c:0xa9b52c;
        if(bp==0xffffffffu) goto pair_si_di;
        goto triple;
    }
    if(kind==0x24){
        fmt=bp==0xffffffffu?0xa9b328:0xa9b3bc;
        if(bp==0xffffffffu) goto single_di;
        goto pair_di_bp;
    }
    if(kind==0x17 || kind==0x18){
        if(di==si){
            if(kind==0x17) fmt=bp==0xffffffffu?0xa9b1b8:0xa9b25c;
            else fmt=bp==0xffffffffu?0xa9af78:0xa9b020;
            if(bp==0xffffffffu) goto single_di;
            goto pair_di_bp;
        }
        fmt=kind==0x17?0xa9b0f0:0xa9aeac;
        base=D(0xae23b4); s=base+si*581u; t=base+di*581u;
        vt=SB(t+0x52); vs=SB(s+0x52);
        cm_fn_0066f4e0(0xde1f64,fmt,vt,vt,100,vs,vs,t+0x38,text,s+0x38);
        goto finish_mode;
    }
    if(kind==0x13){
        fmt=bp==0xffffffffu?0xa9ad4c:0xa9ade8;
        if(bp==0xffffffffu) goto single_si;
        goto pair_si_bp;
    }
    if(kind==0x0e || kind==0x14 || kind==0x15 || kind==0x11){
        if(kind==0x0e) fmt=bp==0xffffffffu?0xa9ab68:0xa9ac44;
        else if(kind==0x14) fmt=bp==0xffffffffu?0xa9a9a4:0xa9aa70;
        else if(kind==0x15) fmt=bp==0xffffffffu?0xa9a7e0:0xa9a8ac;
        else fmt=bp==0xffffffffu?0xa9a220:0xa9a2e4;
        if(bp==0xffffffffu) goto pair_si_di;
        goto triple;
    }
    if(kind==0x0c || kind==0x10){fmt=kind==0x0c?0xa9a6d8:0xa9a3d4;goto triple;}
    if(kind==0x0d){
        base=D(0xae23b4); offset=si*581u;
        b=D(base+offset+0x53);
        cm_fn_006181d0(b,extra,100,2);
        base=D(0xae23b4); t=base+di*581u; b=base+bp*581u; s=base+offset;
        vt=SB(t+0x52); vs=SB(s+0x52); vb=SB(b+0x52);
        cm_fn_0066f4e0(0xde1f64,0xa9a5b4,100,vb,vb,vs,vs,vt,vt,100,100,
                       text,b+0x38,s+0x38,t+0x38,extra);
        goto finish_mode;
    }
    if(kind==0x0f){fmt=0xa9a4d0;goto pair_si_di;}
    if(kind==3){
        if(bp==0xffffffffu) goto default_single;
        base=D(0xae23b4); s=base+si*581u; b=base+bp*581u;
        vs=SB(s+0x52); vb=SB(b+0x52);
        cm_fn_0066f4e0(0xde1f64,0xa9a138,100,vb,vb,vs,vs,vs,vs,
                       text,b+0x38,s+0x38,s+0x38);
        goto finish_mode;
    }
    if(kind==4 || kind==0x22){
        base=D(0xae23b4); s=base+si*581u;
        if(bp!=0xffffffffu){
            b=base+bp*581u; vb=SB(b+0x52); vs=SB(s+0x52);
            fmt=kind==4?0xa9a028:0xa99e54;
            cm_fn_0066f4e0(0xde1f64,fmt,100,vb,vb,vs,vs,vs,vs,
                           text,b+0x38,s+0x38,s+0x38);
        }else{
            vs=SB(s+0x52); fmt=kind==4?0xa99f44:0xa99d8c;
            cm_fn_0066f4e0(0xde1f64,fmt,100,vs,vs,vs,vs,text,s+0x38,s+0x38);
        }
        goto finish_mode;
    }
    if(kind==5 || kind==0x23){
        if(bp==0xffffffffu) goto default_single;
        base=D(0xae23b4); b=base+bp*581u; s=base+si*581u;
        vs=SB(s+0x52); vb=SB(b+0x52); fmt=kind==5?0xa99c7c:0xa99b88;
        cm_fn_0066f4e0(0xde1f64,fmt,100,vb,vb,vs,vs,vb,vb,
                       text,b+0x38,s+0x38,b+0x38);
        goto finish_mode;
    }
    if(bp==0xffffffffu) goto default_single;
    fmt=0xa99ae8;
pair_si_bp:
    base=D(0xae23b4); s=base+si*581u; b=base+bp*581u;
    vb=SB(b+0x52); vs=SB(s+0x52);
    cm_fn_0066f4e0(0xde1f64,fmt,100,vb,vb,vs,vs,text,b+0x38,s+0x38);
    goto finish_mode;
default_single:
    fmt=0xa99a70;
single_si:
    base=D(0xae23b4); s=base+si*581u; vs=SB(s+0x52);
    cm_fn_0066f4e0(0xde1f64,fmt,100,vs,vs,text,s+0x38);
    goto finish_mode;
single_di:
    base=D(0xae23b4); t=base+di*581u; vt=SB(t+0x52);
    cm_fn_0066f4e0(0xde1f64,fmt,vt,vt,100,t+0x38,text);
    goto finish_mode;
pair_di_bp:
    base=D(0xae23b4); b=base+bp*581u; t=base+di*581u;
    vt=SB(t+0x52); vb=SB(b+0x52);
    cm_fn_0066f4e0(0xde1f64,fmt,vt,vt,100,vb,vb,t+0x38,text,b+0x38);
    goto finish_mode;
pair_si_di:
    base=D(0xae23b4); t=base+di*581u; s=base+si*581u;
    vs=SB(s+0x52); vt=SB(t+0x52);
    cm_fn_0066f4e0(0xde1f64,fmt,100,vs,vs,vt,vt,text,s+0x38,t+0x38);
    goto finish_mode;
triple:
    base=D(0xae23b4); t=base+di*581u; s=base+si*581u; b=base+bp*581u;
    vs=SB(s+0x52); vb=SB(b+0x52); vt=SB(t+0x52);
    cm_fn_0066f4e0(0xde1f64,fmt,100,vb,vb,vs,vs,vt,vt,text,b+0x38,s+0x38,t+0x38);
finish_mode:
    cm_fn_004ae660(output,0xde1f64);
    offset=person*110u;
    result=cm_fn_0053e990(D(0xae23bc)+offset,first,second,third,0);
    if(result!=0) cm_fn_004ae8a0(output,text,0x7d7,D(0xae23bc)+offset);
    s=D(0xae23b4)+si*581u;
    result=cm_fn_004ae8a0(output,s+0x38,0x7d5,s);
    if(bp==0xffffffffu) return result;
    b=D(0xae23b4)+bp*581u;
    return cm_fn_004ae8a0(output,b+0x38,0x7d5,b);
mode_zero:
    cm_fn_0053e140(first,second,third,text,300,5,person);
    if(flag==0){
        s32 rem=(s32)si%3;
        s32 rem4=(s32)person%4;
        if(rem==rem4) cm_fn_0066f4e0(0xde1f64,0xa99a40,100,text);
        else if((s32)si%4==(s32)person%2) cm_fn_0066f4e0(0xde1f64,0xa99a08,100,text);
        else if(rem==(s32)person%2) cm_fn_0066f4e0(0xde1f64,0xa999d8,text);
        else {fmt=0xa99980;goto zero_single;}
    }else{
        fmt=0xa99928;
zero_single:
        s=D(0xae23b4)+si*581u; vs=SB(s+0x52);
        cm_fn_0066f4e0(0xde1f64,fmt,100,vs,vs,text,s+0x38);
    }
    cm_fn_004ae660(output,0xde1f64);
    offset=person*110u;
    result=cm_fn_0053e990(D(0xae23bc)+offset,first,second,third,0);
    if(result!=0) cm_fn_004ae8a0(output,text,0x7d7,D(0xae23bc)+offset);
    s=D(0xae23b4)+si*581u;
    return cm_fn_004ae8a0(output,s+0x38,0x7d5,s);
}

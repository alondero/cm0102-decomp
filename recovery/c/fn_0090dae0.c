#include "cmre.h"
/* Preferred-image VA0090dae0/base00400000, SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Native THIS5/RET20. The full EAX is retained, including partial AL writes
   used as the seventh argument to 0090e0a0. Original callees remain boundaries. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
#define LOW(a,b) (((a)&0xffffff00u)|(u8)(b))
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern u32 CM_CDECL cm_fn_0053b900(u32);
extern u32 CM_CDECL cm_fn_005409b0(u32);
extern u32 CM_CDECL cm_fn_0053b3d0(u32);
extern u32 CM_CDECL cm_fn_00600a50(u32,u32,u8,u32,u32);
extern u8 CM_THISCALL cm_fn_007754c0(u32,u32,u32);
extern u8 CM_THISCALL cm_fn_00775520(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_005fd6b0(u32,u32,u32);
extern u32 CM_CDECL cm_fn_00542740(u32);
extern u32 CM_THISCALL cm_fn_0090d8f0(u32,u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_STDCALL cm_fn_0090e0a0(u32,u32,u32,u32,u32,u32,u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_CDECL cm_fn_009469fa(u32);

u32 CM_THISCALL cm_fn_0090dae0(u32 self,u32 club,u32 detail_index,
                             u32 person_index,u32 selection,u32 check)
{
    /* L is the address immediately after native SUB ESP,2c. */
    u8 scratch[0x2c];
    u32 l=(u32)scratch;
    u32 a,c,d,person=0,detail=0,flag=0x33,limit;
    D(l+0xc)=self; D(l+8)=0;
    B(l+1)=0;B(l+5)=0;B(l)=0;B(l+4)=0;
    B(l+2)=0xff;B(l+3)=0xff;B(l+6)=0;B(l+7)=0xff;
    /* Positive 30000 takes native0090cfc0 small-bound path, independent
       of incoming EAX/EDX, and only low EAX is consumed here. */
    a=cm_fn_0090cfc0(30000);++a;D(l+0x20)=a;
    a=D(club+0x53);if(!a)return a;
    a=D(club);c=D(0x009d00a8);if(a==c)return a;
    if(a==D(0x009d00a4))return a;
    a=cm_fn_0090d8f0(self,club,check,selection,l+8,l+1,l+5,l);
    if(!a){a=D(l+8);if(a)a=(u32)cm_fn_009452ca(a);return a;}
    a=0;D(l+0x24)=a;D(l+0x28)=a;
    a=cm_fn_0053b900(club);
    if(a){
        a=cm_fn_005409b0(club);
        if(a)selection=1;else{selection=0;B(l+6)=1;}
    }else{
        a=D(club+0x53);
        if(a){
            c=B(a+0x11c);
            if((u8)c){c=D(club+0x57);if(c){c=D(c);d=D(0x00adadfc);if(D(d+c*4u))goto selected;}}
            if(a){
                a=D(a);c=D(0x009cf36c);
                if(a==c){
                    a=D(club+0x57);
                    if(a){a=D(a);c=D(0x009cf678);
                        if(a!=c && a!=D(0x009cf674) && a!=D(0x009cf670) &&
                           a!=D(0x009cf768) && a!=D(0x009cf65c))goto selected;
                    }
                }
            }
        }
        selection=0;goto selection_done;
selected:selection=1;
    }
selection_done:
    a=cm_fn_0053b900(club);
    if(a){
        a=cm_fn_00600a50(club,1,1,0,0);if(a)return a;
        c=D(0x00b74318);B(l+2)=cm_fn_007754c0(c,club,0);
        c=D(0x00b74318);a=cm_fn_007754c0(c,club,1);
        c=B(l+2);B(l+3)=(u8)a;a=LOW(a,(u8)a+(u8)c);limit=(u8)a;
        a=cm_fn_005409b0(club);
        if(!a){
            c=D(0x00b74318);a=cm_fn_00775520(c,club,1);
            if(!(u8)a){c=B(l+2);B(l+3)=(u8)a;limit=(u8)c;}
        }else{
            d=D(club+0x53);a=cm_fn_0053b3d0(d);
            a=cm_fn_00600a50(a,0,1,0,0);
            if(a){
                c=D(0x00b63c98);a=cm_fn_005fd6b0(c,0,l+0x18);
                if(a)return a;
                if(!D(l+0x18))return a;
                if(!check){a=cm_fn_005409b0(club);if(!a)return a;}
            }
            c=D(0x00b74318);a=cm_fn_00775520(c,club,0);
            if((u8)a){c=D(0x00b74318);a=cm_fn_00775520(c,club,0);if((s8)B(l+1)<=(s8)a)goto counts_done;}
            c=D(club+0x53);
            if((s32)D(c+0x88)>500){B(l+2)=0;B(l+3)=0;limit=0;}
        }
    }else{
        limit=0x32;a=cm_fn_00600a50(club,1,1,0,0);if(!a)B(l+7)=0x14;
    }
counts_done:
    a=cm_fn_00542740(club);d=B(l+5);D(l+0x10)=a;
    a=LOW(a,limit);if((s8)d>=(s8)a)goto cleanup;
    person=person_index;detail=detail_index;
outer:
    if((s8)B(l+4)>=25)goto cleanup;
    a=LOW(a,0);D(l+0x14)=0;person_index=LOW(person_index,a);
inner:
    if((s8)B(l+5)>=50 || (s8)B(l+4)>=25)goto sweep_done;
    c=(u32)(s32)(s8)B(l+6);a=(u32)(s32)(s8)a;
    detail_index=a;d=a+c*8u;c=D(l+8);a=LOW(a,B(a+c));c=D(l+0xc);
    if((s8)a>=(s8)B(d+c))goto advance;
    a=LOW(a,person_index);
    if((u8)a && (s8)B(l+1)>20){a=D(l+0x10);if(!a)D(l+0x10)=1;}
    c=detail_index;a=l+0x24+c;c+=1000;D(l+0x1c)=a;
    a=LOW(a,B(a));d=(u32)(s32)(s8)a;check=LOW(check,a);
    a=D(club);d+=10000;++a;d*=a;d*=c;
    a=cm_fn_009469fa(d);a=cm_fn_0053b900(club);
    if(a){
        c=B(l+2);a=LOW(a,B(l));
        if((s8)c>(s8)a){
            d=selection;a=LOW(a,flag);c=D(l+0xc);++flag;
            a=cm_fn_0090e0a0(club,person_index,detail,person,d,0,a);goto count_flagged;
        }
        a=cm_fn_005409b0(club);
        if(a)goto processed;
        a=LOW(a,B(l+3));if(!(u8)a)goto processed;
        c=(u32)(s32)(s8)B(l);d=(u32)(s32)(s8)B(l+1);a=(u32)(s32)(s8)a;d-=c;
        if((s32)a<=(s32)d)goto processed;
        c=selection;d=person_index;a=LOW(a,flag);++flag;
        c=D(l+0x18); /* Native ECX scratch read, not a callee receiver. */
        a=cm_fn_0090e0a0(club,d,detail,person,selection,1,a);
        a=(u32)(s32)(s8)B(l);c=(u32)(s32)(s8)B(l+1);c-=a;
        if((s32)c>20)goto cleanup;
        goto processed;
    }
    a=D(club+0x57);if(a){d=D(a);a=D(0x00adadfc);if(D(a+d*4u))goto country_selected;}
    c=D(l+8);a=detail_index;d=B(a+c);c=D(l+0xc);
    if((s8)d<(s8)B(a+c+0x10) || (s8)B(l)<20)goto create_flagged;
    goto country_ready;
country_selected:c=D(l+0xc);
country_ready:
    a=LOW(a,B(l+7));if((u8)a!=255 && (s8)B(l+1)<(s8)a)goto create_flagged;
    d=D(l+0x10);
    if(d){
        a=LOW(a,flag);++flag;
        a=cm_fn_0090e0a0(club,person_index,detail,person,selection,d,a);goto processed;
    }
    if((s8)B(l)>=20)goto processed;
create_flagged:
    d=person_index;a=LOW(a,flag);++flag;
    a=cm_fn_0090e0a0(club,d,detail,person,selection,0,a);
count_flagged:B(l)=(u8)(B(l)+1u);
processed:
    a=D(l+8);c=detail_index;D(l+0x14)=1;d=B(c+a);++d;++detail;B(c+a)=(u8)d;
    a=LOW(a,B(l+1));d=B(l+4);c=D(l+0x1c);++person;
    a=LOW(a,(u8)a+1u);B(l+1)=(u8)a;a=LOW(a,check);++d;a=LOW(a,(u8)a+1u);
    B(l+4)=(u8)d;B(c)=(u8)a;
advance:
    a=LOW(a,person_index);a=LOW(a,(u8)a+1u);person_index=LOW(person_index,a);
    if((s8)a<8)goto inner;
sweep_done:
    a=D(l+0x14);if(!a)goto cleanup;
    d=limit;a=LOW(a,B(l+5));if((s8)a<(s8)d)goto outer;
cleanup:
    a=D(l+8);a=(u32)cm_fn_009452ca(a);c=D(l+0x20);a=cm_fn_009469fa(c);
    return a;
}

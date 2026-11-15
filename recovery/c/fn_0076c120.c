/* Preferred-image VA0076C120, base00400000; CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   ECX receiver, five full stack homes, RET20; indexed callers discard EAX.
   Original helpers remain boundaries. L/WL/BL use original post-prologue
   ESP offsets, in one uninitialized local block; the linked SEH state is
   a separate actual record. No stack-local initialization is inferred. */
#include "cmre.h"
typedef unsigned long long u64;
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
#define Q(a) CM_GLOBAL(volatile double,(a))
#define L(off) D(l+(off)-0x10u)
#define WL(off) W(l+(off)-0x10u)
#define BL(off) B(l+(off)-0x10u)
#define P(off) (l+(off)-0x10u)
extern u32 CM_CDECL cm_fn_0053b3d0(u32);
extern u32 CM_CDECL cm_fn_0053b4f0(u32);
extern u32 CM_CDECL cm_fn_0053b620(u32);
extern u64 CM_THISCALL cm_fn_005c6710(u32,u32);
extern u64 CM_THISCALL cm_fn_00549ef0(u32,u32,u32);
extern u64 CM_THISCALL cm_fn_0054c8e0(u32,u32,u32);
extern u64 CM_CDECL __attribute__((regparm(3))) cm_fn_00944cf1(u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0068d020(u32);
extern u32 CM_THISCALL cm_fn_0054a110(u32);
extern u64 CM_THISCALL cm_fn_00417c10(u32,u32,u32);
extern u64 CM_FASTCALL cm_fn_0068d0d0(u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0076d880(u32,u32);
extern u32 CM_STDCALL cm_fn_0076d430(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00600a50(u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_005fd6b0(u32,u32,u32);
extern u32 CM_CDECL cm_fn_005409b0(u32);
extern u32 CM_THISCALL cm_fn_0062c400(u32,u32);
extern u32 CM_THISCALL cm_fn_0062ed70(u32,u32);
extern u32 CM_THISCALL cm_fn_00896c60(u32,u32,u32,u32,u32);
extern u64 CM_THISCALL cm_fn_0076f1a0(u32,u32,u32,u32);
extern u32 CM_STDCALL cm_fn_007a7eb0(u32);
extern u32 CM_CDECL cm_fn_00547d30(u32,u32);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32,u32);
extern u32 CM_THISCALL cm_fn_0055d270(u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0076db00(u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_006e2000(u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32);
extern u64 CM_CDECL __attribute__((regparm(2))) cm_fn_005b7d20(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_008968f0(u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00896e90(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);
extern u32 CM_CDECL cm_fn_00540980(u32,u32);
extern u64 CM_CDECL __attribute__((regparm(2))) cm_fn_00945333(u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0076e0b0(u32,u32,u32,u32,u32,u32,u32,u32);

void CM_THISCALL __attribute__((target("no-sse3")))
cm_fn_0076c120(u32 self,volatile u32 nation_home,volatile u32 mode_home,
              volatile u32 date_home,volatile u32 match_home,volatile u32 skip_home)
{
    volatile struct {u32 previous,handler,state;} frame;
    u8 scratch[0x8c0];u32 l=(u32)scratch;
    u32 eax,ecx,edx=0,ebx=0,ebp=1,esi,edi,a,b,pointer;
    u32 category,start,last,remaining,match,mode;
    u64 pair;
    long double fp,other;
    frame.state=0xffffffffu;frame.handler=0x0095dd7au;
    frame.previous=CM_FS(u32,0);CM_FS(u32,0)=(u32)&frame;
    esi=nation_home;eax=D(esi);L(0x14)=self;edx=D(self+4u);
    edi=match_home;BL(0x34)=0;L(0x58)=0;L(0x44)=0xffffffffu;L(0x2c)=10001;
    L(0x18)=edx+eax*110u;ecx=D(0x009cf224u);
    if(eax==ecx) {
        if(!edi)goto default_limits;
        ecx=D(0x00ae2c74u);
        if(ecx) {
            edx=D(edi+0x14u);pair=cm_fn_005c6710(ecx,edx);eax=(u32)pair;
            if(eax)goto default_limits;
        }
        eax=D(edi+0x14u);ecx=D(0x009cf8a0u);eax=D(eax);
        if(eax==ecx)goto default_limits;
        if(eax==D(0x009cf964u)){L(0x44)=0xabe;L(0x2c)=0x222e;}
        else if(eax==D(0x009cf76cu)) {
            a=B(edi+0x42u);L(0x44)=0x8ca;L(0x2c)=9000;
            if(a==1)L(0x2c)=8000;else if(a==2)L(0x2c)=10001;
        }else if(eax==D(0x009cf79cu)){L(0x44)=0xabe;L(0x2c)=10000;}
        goto mode_setup;
default_limits:
        L(0x44)=0x8ca;L(0x2c)=0x2134;
    }
mode_setup:
    mode=(u8)mode_home;
    if(mode==1){eax=cm_fn_0053b3d0(esi);L(0x10)=eax;BL(0x48)=8;L(0x28)=1;}
    else {
        eax=mode?cm_fn_0053b4f0(esi):cm_fn_0053b3d0(esi);
        L(0x10)=eax;BL(0x48)=4;L(0x28)=0;
    }
    if(edi) {
        eax=D(edi+0x14u);
        if(eax) {
            eax=D(eax);
            if(eax==D(0x009cf79cu) || eax==D(0x009cf7a0u) || eax==D(0x009cf7a4u) ||
               eax==D(0x009cf7a8u) || eax==D(0x009cf7acu) || eax==D(0x009cf790u) ||
               eax==D(0x009cf8a0u) || eax==D(0x009cf78cu) || eax==D(0x009cf964u))L(0x58)=1;
        }
    }
    eax=skip_home;ebp=12;
    if(eax)goto patterns;
    eax=L(0x58);edi=match_home;
    if(eax) {
        ecx=(ecx&0xffff0000u)|W(edi+0x28u);
        edx=(edx&0xffff0000u)|W(edi+0x2au);
        pair=cm_fn_00549ef0(P(0x60),edx,ecx);eax=(u32)pair;edx=(u32)(pair>>32);
        frame.state=0;
        pair=cm_fn_0054c8e0(eax,P(0x50),10);eax=(u32)pair;edx=(u32)(pair>>32);
        /* Both native epilogues store their live ECX in out+4. The
           allocation call below forwards that physical ECX, not the owner. */
        ecx=L(0x54);
        ebx=L(0x14);esi=D(edi+0x14u);*(volatile u8 *)&frame.state=2;
        if(!D(ebx)) {
            pair=cm_fn_00944cf1(eax,edx,ecx,14);eax=(u32)pair;edx=(u32)(pair>>32);
            L(0x3c)=eax;*(volatile u8 *)&frame.state=3;
            if(eax)eax=cm_fn_0068d020(eax);
            *(volatile u8 *)&frame.state=2;D(ebx)=eax;
        }
        cm_fn_0054a110(P(0xdc));edx=L(0x10);ecx=D(esi);L(0xd8)=ecx;
        eax=D(edx);L(0xd4)=eax;
        *(volatile u8 *)&frame.state=4;
        pair=cm_fn_00417c10(P(0xdc),P(0x60),P(0x50));edx=(u32)(pair>>32);
        WL(0xe4)=1000;
        cm_fn_0068d0d0(D(ebx),edx,P(0xd4),18);
        frame.state=0xffffffffu;
    }
    if(edi) {
        ebx=(u8)mode_home;
        if(ebx!=1 && ebx!=2) {
            eax=match_home;edx=(edx&0xffff0000u)|W(eax+0x28u);
            eax=(eax&0xffff0000u)|W(eax+0x2au);
            pair=cm_fn_00549ef0(P(0x50),eax,edx);eax=(u32)pair;edx=(u32)(pair>>32);
            edi=L(0x18)+(u32)(s32)(s8)ebx*20u;
            cm_fn_00417c10(edi+0x3au,P(0x60),eax);
        }else {
            ecx=D(0x009cf7a0u);eax=D(edi);
            if(eax==ecx) {
                edx=D(edi+0x1cu);eax=D(edx+0x53u);ecx=D(eax);
                eax=L(0x14);edx=D(eax+0x36u);B(ecx+edx)=1;
                ecx=D(edi+0x20u);edx=D(ecx+0x53u);ecx=D(edx);edx=D(eax+0x36u);B(ecx+edx)=1;
            }else {
                eax=D(edi+0x1cu);esi=L(0x14);eax=D(eax+0x53u);
                edx=D(esi+0x36u);ecx=D(eax);
                if(B(ecx+edx))cm_fn_0076d880(esi,eax);
                eax=D(edi+0x20u);edx=D(esi+0x36u);eax=D(eax+0x53u);ecx=D(eax);
                if(B(ecx+edx))cm_fn_0076d880(esi,eax);
                eax=D(edi+0x1cu);ecx=D(eax+0x53u);eax=D(esi+0x36u);edx=D(ecx);B(edx+eax)=0;
                ecx=D(edi+0x20u);edx=D(ecx+0x53u);ecx=D(esi+0x36u);eax=D(edx);B(eax+ecx)=0;
            }
            eax=match_home;edx=(edx&0xffff0000u)|W(eax+0x28u);eax=(eax&0xffff0000u)|W(eax+0x2au);
            pair=cm_fn_00549ef0(P(0x50),eax,edx);eax=(u32)pair;edx=(u32)(pair>>32);
            frame.state=5;pair=cm_fn_0054c8e0(eax,P(0x60),1);
            eax=(u32)pair;edx=(u32)(pair>>32);
            edi=L(0x18)+(u32)(s32)(s8)ebx*20u;
            cm_fn_00417c10(edi+0x3au,P(0x3c),eax);frame.state=0xffffffffu;
        }
    }else {
        pair=cm_fn_0054c8e0(date_home,P(0x50),1);eax=(u32)pair;edx=(u32)(pair>>32);
        edi=L(0x18)+(u32)(s32)(s8)mode_home*20u;
        cm_fn_00417c10(edi+0x3au,P(0x60),eax);
    }
    cm_fn_00417c10(edi+0x32u,P(0x50),date_home);
    edx=mode_home;eax=match_home;ecx=L(0x14);
    cm_fn_0076d430(eax,edx,edi+0x42u,edi+0x43u);
    eax=cm_fn_0053b3d0(nation_home);esi=L(0x10);ebx=D(eax+0xcfu);
    eax=cm_fn_00600a50(esi,0,1,0,0);
    if(eax) {
        if(!(u8)mode_home)goto clear_roster;
        if(cm_fn_005fd6b0(D(0x00b63c98u),ebx,0))goto clear_roster;
    }
    if(cm_fn_005409b0(esi)) {
        edx=L(0x10);eax=D(edx+0xcfu);
        if(!eax) {
            eax=cm_fn_0053b3d0(D(edx+0x53u));
            if(cm_fn_00600a50(eax,0,1,0,0) && cm_fn_005fd6b0(D(0x00b63c98u),ebx,0))goto clear_roster;
        }
    }
    goto maybe_complete;
clear_roster:
    edx=L(0x10);ebp=50;esi=edx+0xd7u;
    do {
        eax=D(esi);
        if(eax) {
            if((u8)cm_fn_0062c400(D(0x00b63d28u),eax)==2) {
                eax=D(esi);ecx=BL(0x48);
                if(B(eax+0x5eu)&ecx) {
                    eax=cm_fn_0062ed70(D(0x00b63d28u),eax);
                    if(eax && (s16)W(eax+0x2eu)>10) {
                        edx=L(0x28);eax=D(esi);ecx=L(0x10);
                        cm_fn_00896c60(D(0x00de1d08u),ecx,eax,edx,1);
                        eax=D(esi);B(eax+0x5eu)&=3u;
                        edx=D(esi);D(edx+0x24u)=0;
                        eax=D(esi);B(eax+0x28u)=B(eax+0x28u)==12?5:0;
                        D(esi)=0;
                    }
                }
            }
        }
        esi+=4u;
    }while(--ebp);
    ebp=12;
maybe_complete:
    esi=L(0x10);
    eax=cm_fn_00600a50(esi,0,1,0,0);
    if(eax) {
        if(!(u8)mode_home)goto complete;
        if(cm_fn_005fd6b0(D(0x00b63c98u),ebx,0))goto complete;
    }
    if(!cm_fn_005409b0(esi))goto patterns;
    eax=D(esi+0xcfu);if(eax)goto patterns;
    eax=cm_fn_0053b3d0(D(esi+0x53u));
    if(!cm_fn_00600a50(eax,0,1,0,0))goto patterns;
    if(!cm_fn_005fd6b0(D(0x00b63c98u),ebx,0))goto patterns;
complete:
    edx=match_home;eax=L(0x28);ecx=L(0x14);B(edi+0x44u)=0;
    cm_fn_0076f1a0(ecx,esi,eax,edx);goto done;
patterns:
    edx=mode_home;eax=L(0x18)+(u32)(s32)(s8)edx*20u+0x42u;
    L(0x6c)=eax;a=B(eax);
    if(a==26) {
        WL(0x80)=0x84;WL(0x70)=1;WL(0x72)=1;WL(0x74)=1;
        WL(0x76)=0x202;WL(0x78)=0x202;WL(0x82)=0x804;
        WL(0x7a)=0x204;WL(0x7c)=0x204;WL(0x7e)=0x204;
        WL(0x8a)=0x210;WL(0x8c)=0x210;WL(0x8e)=0x210;
        WL(0x84)=0x88;WL(0x86)=0x808;WL(0x88)=(u16)ebp;
        WL(0x90)=0x220;WL(0x92)=0x90;WL(0x94)=0x810;WL(0x96)=0xa0;WL(0x98)=0x820;
        WL(0x9a)=0x240;WL(0x9c)=0x240;WL(0x9e)=0x240;WL(0xa0)=0x240;WL(0xa2)=0x240;
    }else {
        WL(0x70)=1;WL(0x72)=1;
        if(a==24) {
            WL(0x7e)=0x84;WL(0x74)=0x202;WL(0x76)=0x202;WL(0x80)=0x804;
            WL(0x78)=0x204;WL(0x7a)=0x204;WL(0x7c)=0x204;
            WL(0x88)=0x210;WL(0x8a)=0x210;WL(0x8c)=0x210;
            WL(0x82)=0x88;WL(0x84)=0x808;WL(0x86)=(u16)ebp;
            WL(0x8e)=0x220;WL(0x90)=0x90;WL(0x92)=0x810;WL(0x94)=0xa0;WL(0x96)=0x820;
            WL(0x98)=0x240;WL(0x9a)=0x240;WL(0x9c)=0x240;WL(0x9e)=0x240;
        }else {
            WL(0x74)=1;WL(0x78)=0x204;WL(0x7a)=0x204;WL(0x7c)=0x204;WL(0x7e)=0x204;
            WL(0x76)=0x202;WL(0x86)=0x210;WL(0x88)=0x210;
            WL(0x80)=0x84;WL(0x82)=0x804;WL(0x84)=(u16)ebp;
            WL(0x8a)=0x220;WL(0x8c)=0x90;WL(0x8e)=0x810;WL(0x90)=0xa0;WL(0x92)=0x820;
            WL(0x94)=0x240;WL(0x96)=0x240;WL(0x98)=0x240;WL(0x9a)=0x240;
        }
    }
    ebp=L(0x10);ebx=L(0x28);edi=50;esi=ebp+0xd7u;
    do {
        eax=D(esi);
        if(eax) {
            if(D(eax+0x24u)!=ebp) {
                cm_fn_00896c60(D(0x00de1d08u),ebp,eax,ebx,1);goto drop_slot;
            }
            ecx=BL(0x48);
            if(B(eax+0x5eu)&ecx) {
                cm_fn_00896c60(D(0x00de1d08u),ebp,eax,ebx,1);
                eax=D(esi);B(eax+0x5eu)&=3u;edx=D(esi);D(edx+0x24u)=0;
                eax=D(esi);B(eax+0x28u)=B(eax+0x28u)==12?5:0;goto drop_slot;
            }
            if((u8)edx==0) {
                ecx=match_home;
                if(B(ecx+0x4eu)&1u) {
                    cm_fn_00896c60(D(0x00de1d08u),ebp,eax,ebx,1);
                    eax=D(esi);B(eax+0x5eu)&=3u;edx=D(esi);D(edx+0x24u)=0;
                    eax=D(esi);B(eax+0x28u)=B(eax+0x28u)==12?11:0;
drop_slot:
                    edx=mode_home;D(esi)=0;
                }
            }
        }
        esi+=4u;
    }while(--edi);
    L(0x24)=esi;
    if((u8)edx==2) {
        eax=match_home;
        if(eax && D(eax)==D(0x009cf7a0u)){start=0;last=1;}
        else {start=1;last=1;}
    }else {start=0;last=0;}
    edi=L(0x18);ebp=nation_home;
    for(category=start;category<=last;++category) {
        L(0x38)=category;pointer=edi+category*21u+13u;
        L(0x20)=pointer;L(0x4c)=last-category+1u;
        BL(0x1f)=0;L(0x5c)=0;L(0x30)=pointer;
        do {
            ecx=D(L(0x20)-5u);
            if((s8)BL(0x1f)>0) {
                eax=L(0x20)+1u;remaining=L(0x5c);
                do {ecx+=B(eax);eax+=4u;}while(--remaining);
            }
            esi=L(0x30);a=L(0x38);eax=B(esi);esi=L(0x14);
            eax+=ecx;esi=D(esi+a*4u+0xcu);esi+=ecx*4u;L(0x24)=esi;
            if((s32)ecx<(s32)eax) {
                L(0x68)=eax-ecx;
                do {
                    ecx=L(0x24);esi=D(ecx);
                    if(!esi)goto next_candidate;
                    edx=(edx&0xffffff00u)|B(esi+0x3du);
                    if(!cm_fn_0053b620(edx))goto next_candidate;
                    if((u8)cm_fn_0062c400(D(0x00b63d28u),esi))goto next_candidate;
                    ecx=D(0x00b7434cu);
                    if(ecx && (u8)cm_fn_007a7eb0(esi))goto next_candidate;
                    if(!D(esi+0x39u))goto next_candidate;
                    eax=cm_fn_0053b3d0(D(esi+0x1au));
                    if(cm_fn_00547d30(esi,eax))goto next_candidate;
                    eax=D(esi+0x24u);
                    if(eax) {
                        if(B(esi+0x28u)!=11)goto next_candidate;
                        if(eax && D(eax+0x53u)!=ebp)goto next_candidate;
                    }
                    match=match_home;
                    if(match) {
                        edi=D(0x009cf668u);ebx=match;edx=D(ebx);
                        if(edx==edi || edx==D(0x009cf794u) || edx==D(0x009cf798u))goto special_match;
                        eax=D(esi+0x39u);
                        if(eax) {
                            ecx=D(eax+0x53u);
                            if(ecx) {
                                ecx=D(ecx+0x71u);edx=D(ebp+0x71u);
                                if(ecx!=edx) {
                                    eax=D(eax+0x57u);
                                    if(eax) {
                                        edx=D(ebx+0x14u);a=(u32)(s32)(s16)W(edx+0x69u);
                                        b=(u32)(s32)(s16)W(eax+0x69u);L(0x3c)=a;fp=(long double)(s32)L(0x3c);
                                        L(0x3c)=b;other=(long double)(s32)L(0x3c);other*=Q(0x00968e40u);
                                        if(other>fp)goto next_candidate;
                                    }
                                }
                            }
                        }
                        goto match_allowed;
special_match:
                        ecx=D(esi+0x39u);
                        if(ecx) {
                            eax=D(ecx+0x53u);
                            if(eax) {
                                eax=D(eax+0x71u);ebx=D(ebp+0x71u);
                                if(eax!=ebx) {
                                    if(edx!=edi)goto next_candidate;
                                    edx=match_home;edi=L(0x10);eax=D(edx+0x1cu);
                                    if(eax==edi){edx=D(edx+0x20u);a=(u32)(s32)(s16)W(edx+0x80u);}
                                    else a=(u32)(s32)(s16)W(eax+0x80u);
                                    b=(u32)(s32)(s16)W(ecx+0x80u);
                                    L(0x3c)=a;fp=(long double)(s32)L(0x3c);L(0x3c)=b;
                                    other=(long double)(s32)L(0x3c);other*=Q(0x009689b0u);
                                    if(other>fp)goto next_candidate;
                                }
                            }
                            eax=L(0x14);edx=D(eax+0x24u);eax=D(ecx);
                            if(B(edx+eax)) {
                                ecx=D(0x00ae1050u);eax=cm_fn_004cf5a0(ecx,esi);
                                if(eax) {
                                    eax=cm_fn_004cf5a0(D(0x00ae1050u),esi);
                                    if((s8)B(eax+0x3du)>-10)goto next_candidate;
                                }
                            }
                        }
match_allowed:
                        ecx=match_home;edx=D(ecx+0x14u);
                        if(cm_fn_0055d270(D(0x00ae28f0u),esi,edx,1,0))goto next_candidate;
                        ebx=(ebx&0xffffff00u)|(u8)mode_home;
                        if(!(u8)ebx) {
                            eax=match_home;a=W(eax+0x4du)&0x300u;
                            if(a!=0x100u) {
                                ecx=L(0x10);edx=D(esi+0x24u);
                                if(edx==ecx && (B(esi+0x5eu)&8u))goto next_candidate;
                            }
                            if(a!=0x200u) {
                                eax=D(esi+0x24u);
                                if(eax && eax!=L(0x10))goto next_candidate;
                            }
                        }
                    }else ebx=(ebx&0xffffff00u)|(u8)mode_home;
                    eax=cm_fn_0053b3d0(ebp);ecx=D(esi+0x24u);
                    if(ecx==eax && (B(esi+0x5eu)&4u)) {
                        eax=cm_fn_0076db00(L(0x14),ebp,0,esi,0,0,0);
                        if(!eax)goto next_candidate;
                        eax=match_home;
                        if(!eax || D(eax)!=D(0x009cf7a0u)) {
                            if((u8)ebx)goto next_candidate;
                        }
                    }
                    if(B(esi+0x5eu)&8u) {
                        eax=date_home;edx=L(0x18);ecx=D(eax);eax=D(eax+4u);
                        L(0x64)=eax;a=W(edx+0x50u);L(0x60)=ecx;b=WL(0x62);
                        if(b<a || (b==a && (s16)ecx<=(s16)W(L(0x18)+0x4eu)))goto next_candidate;
                    }
                    edi=D(esi+0x24u);eax=cm_fn_0053b4f0(ebp);
                    if(edi==eax && (B(esi+0x5eu)&4u)) {
                        eax=date_home;edx=L(0x18);ecx=D(eax);eax=D(eax+4u);
                        L(0x54)=eax;a=W(edx+0x64u);L(0x50)=ecx;b=WL(0x52);
                        if(b<a || (b==a && (s16)ecx<=(s16)W(L(0x18)+0x62u)))goto next_candidate;
                    }
                    if((u8)ebx==2 && (s8)B(esi+0x18u)>23)goto next_candidate;
                    edx=D(esi+0x1au);
                    if(edx!=ebp) {
                        if(edi || B(esi+0x22u))goto next_candidate;
                        eax=D(esi);ecx=eax*79u;eax=D(0x00ae2d0cu);
                        if(B(ecx+eax+0xcu)&2u)goto next_candidate;
                        if(D(esi+0x1eu)!=ebp)goto next_candidate;
                        if(edx!=ebp) {
                            ecx=0;if(!edx)goto next_candidate;
                            eax=D(esi+0x39u);if(eax)ecx=D(eax+0x53u);
                            a=W(ebp+0x8eu);
                            if((s16)a<=(s16)W(edx+0x8eu)) {
                                if(ecx && D(ebp+0x71u)!=D(ecx+0x71u))goto next_candidate;
                                if((s8)B(ebp+0x85u)<5)goto next_candidate;
                            }
                        }
                    }
                    eax=D(esi+0x61u);a=W(eax+9u);
                    if((s16)a<=(s16)WL(0x44) || (s16)a>(s16)WL(0x2c))goto next_candidate;
                    a=BL(0x34);ecx=(u32)(s32)(s8)a;
                    D(P(0x5b0)+ecx*4u)=esi;BL(0x34)=(u8)(a+1u);
next_candidate:
                    ecx=L(0x24);eax=L(0x68);L(0x24)=ecx+4u;L(0x68)=eax-1u;
                }while(eax!=1);
                edx=mode_home;
            }
            a=BL(0x1f);edi=L(0x5c);esi=L(0x30);
            a=(u8)(a+1u);BL(0x1f)=(u8)a;L(0x5c)=edi+1u;L(0x30)=esi+4u;
        }while((s8)a<=3);
        /* Original category loop advances the original pointer by21. */
        L(0x38)=category+1u;L(0x20)+=21u;L(0x4c)-=1u;edi=L(0x18);
    }
    if((u8)edx==1)eax=0x02100914u;
    else eax=((u8)edx==2?0u:0x02000000u)+0x814u;
    edi=L(0x10);ebx=L(0x6c);a=(eax&0xffffff00u)|B(ebx);
    edx=L(0x34);
    cm_fn_006e2000(P(0x5b0),edx,P(0x70),a,0,P(0xe8),eax,edi,0,0,0,0,0,0,0);
    a=B(ebx);L(0x4c)=0;
    if((s8)a<=0)goto sort;
    L(0x30)=P(0x70);L(0x38)=P(0xe8);
    do {
        eax=L(0x38);esi=D(eax);
        if(!esi)goto next_selected;
        ecx=D(esi+0x61u);pair=cm_fn_005b7d20(eax,edx,ecx);eax=(u32)pair;edx=(u32)(pair>>32);
        pointer=L(0x30);
        if(!(W(pointer)&(u16)eax))goto next_selected;
        eax=0;ecx=edi+0xd7u;
        while(eax<50 && D(ecx)){++eax;ecx+=4u;}
        if(eax<50)D(edi+eax*4u+0xd7u)=esi;
        eax=D(esi+0x24u);
        if(eax!=edi) {
            if(!eax)goto maybe_same_roster;
            edi=(B(esi+0x5eu)&8u)>>3;L(0x3c)=edi;
            eax=cm_fn_005409b0(eax);
            BL(0x20)=eax?(edi?1:2):(edi?1:0);
            ebp=D(esi+0x24u);ebx=L(0x20);edi=0;ebp+=0xd7u;L(0x24)=ebp;
            do {
                if(D(ebp)==esi) {
                    D(ebp)=0;eax=D(esi+0x24u);ecx=D(eax+0x53u);
                    eax=cm_fn_0076db00(L(0x14),ecx,ebx,0,0,0,0);
                    if(eax) {
                        a=(u32)(s32)(s8)BL(0x20);ecx=L(0x18);L(0x24)=ebp;
                        B(ecx+a*20u+0x43u)=0;goto removed_old;
                    }
                    cm_fn_00944cff(0x00a5b2a4u,0,0,P(0x1b0),P(0x4b0));
                    eax=cm_fn_0090d130();
                    cm_fn_00944c9f(0x00ae24d0u,0x009870e8u,eax,P(0x1b0),P(0x4b0),0x748u);
                    cm_fn_005e8290(0x009870e0u,0x00ae24d0u,0);D(0x00b67a34u)=0;
                }
                ++edi;ebp+=4u;
            }while(edi<50);
            L(0x24)=ebp;
removed_old:
            edx=L(0x3c);eax=D(esi+0x24u);
            cm_fn_00896c60(D(0x00de1d08u),eax,esi,edx,1);goto restore_roster;
        }
        goto same_roster;
maybe_same_roster:
        if(eax!=edi)goto register_selected;
same_roster:
        ecx=BL(0x48);a=B(esi+0x5eu);
        if(a&ecx)goto register_selected;
        ebx=L(0x28);eax=L(0x10);ebp=ebx==0;
        eax=cm_fn_005409b0(eax);
        ebx=(ebx&0xffffff00u)|(eax?(ebp?1u:2u):(ebp?1u:0u));
        BL(0x34)=(u8)ebx;edi=0;
        do {
            ecx=D(esi+0x24u);eax=cm_fn_008968f0(D(0x00de1d08u),ecx,edi,ebp);
            if(esi==eax) {
                edx=D(esi+0x24u);cm_fn_00896e90(D(0x00de1d08u),edx,edi,ebp);
                if((u8)ebx) {
                    eax=L(0x24);edx=L(0x34);ecx=D(eax);eax=D(esi+0x24u);a=D(eax+0x53u);
                    if(cm_fn_0076db00(L(0x14),a,edx,ecx,0,0,0)) {
                        ecx=L(0x18);a=(u32)(s32)(s8)ebx;B(ecx+a*20u+0x43u)=0;goto restore_roster;
                    }
                }
                cm_fn_00944cff(0x00a5b2a4u,0,0,P(0x3b0),P(0x2b0));
                eax=cm_fn_0090d130();
                cm_fn_00944c9f(0x00ae24d0u,0x009870e8u,eax,P(0x3b0),P(0x2b0),0x76bu);
                cm_fn_005e8290(0x009870e0u,0x00ae24d0u,0);D(0x00b67a34u)=0;
            }
        }while(++edi<20);
restore_roster:
        edi=L(0x10);
register_selected:
        edx=L(0x48);cm_fn_00540980(esi,edx);
        a=B(esi+0x28u);D(esi+0x24u)=edi;B(esi+0x28u)=(a==5 || a==12)?12:11;
        esi=D(esi);ecx=D(0x00ae2d0cu);ecx+=esi*79u+9u;a=B(ecx);
        if((s8)a<0)eax=0;else if((s8)a>100)eax=100;else eax=(u32)(s32)(s8)a;
        ebx=L(0x6c);B(ecx)=(u8)eax;
next_selected:
        edx=L(0x30);eax=L(0x4c);esi=L(0x38);L(0x30)=edx+2u;
        ++eax;edx=(u32)(s32)(s8)B(ebx);L(0x4c)=eax;L(0x38)=esi+4u;
    }while((s32)eax<(s32)edx);
sort:
    eax=edi+0xd7u;cm_fn_00945333(eax,edx,eax,50,4,0x00778ef0u);
    ecx=match_home;
    if(ecx){eax=D(ecx+0x14u);eax=D(eax);}else eax=0xffffffffu;
    edx=L(0x58);ebx=L(0x28);
    cm_fn_0076e0b0(L(0x14),edi,ebx,ecx,eax,edx,0,0);
    esi=0;
    do {
        eax=cm_fn_008968f0(D(0x00de1d08u),edi,esi,ebx);
        if(eax && D(eax+0x24u)!=edi)cm_fn_00896e90(D(0x00de1d08u),edi,esi,ebx);
    }while(++esi<20);
done:
    CM_FS(u32,0)=frame.previous;
}

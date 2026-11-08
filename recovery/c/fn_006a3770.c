/* CM0102 3.9.68, preferred-image VA006a3770, base00400000.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Private candidate: not validated or accepted. Native callees remain boundaries.
   Register names retain the distinct physical prefixes of the seven virtual calls.
   L is the original ESP immediately after SUB ESP,10h; output slots are L+3
   and L+20. The record cached in ESI does not change when L+20 is overwritten. */
#include "cmre.h"

#define R8(a) (*(volatile u8 *)(u32)(a))
#define R16(a) (*(volatile u16 *)(u32)(a))
#define R32(a) (*(volatile u32 *)(u32)(a))
#define R64(a) (*(volatile double *)(u32)(a))
#define S8(a) ((s32)(s8)R8(a))
#define S16(a) ((s32)(s16)R16(a))
#define LOW8(v,x) (((v)&0xffffff00u)|(u8)(x))
#define LOW16(v,x) (((v)&0xffff0000u)|(u16)(x))
/* Two actual listing orders: neither macro caches a record or global read. */
#define ROW_D() do { eax=R32(esi); ecx=eax*9u; edx=eax+ecx*8u; eax=R32(edi); } while(0)
#define ROW_A() do { eax=R32(esi); ecx=R32(edi); edx=eax*9u; eax=eax+edx*8u; } while(0)

extern u32 CM_THISCALL cm_fn_004a4850(u32 self,u32 target);
extern u32 CM_CDECL cm_fn_0053b900(u32 *input);
extern u32 CM_CDECL cm_fn_005409b0(u32 *input);
extern u32 CM_THISCALL cm_fn_0054cdb0(u32 self,u32 first,u32 second);
extern u8 CM_THISCALL cm_fn_005986b0(u32 self);
extern long long CM_THISCALL cm_fn_00598800(u32 self,u32 person,u32 include,u32 output,u32 adjust,u32 extra);
extern u32 CM_THISCALL cm_fn_005a0590(u32 self,u32 arg1);
extern u32 CM_THISCALL cm_fn_005a5930(u32 self,u32 input);
extern unsigned long long CM_FASTCALL cm_fn_0068d0d0(volatile u8 *self,u32 edx_in,const u8 *src,s16 count);
extern void CM_STDCALL cm_fn_006a44a0(u32 *record,s16 flags,u32 value);
/* Native EAX is forwarded by the early return, despite accepted callee's void. */
extern u32 CM_STDCALL cm_fn_006a4a20(u32 *record,s16 flags);
extern u32 CM_THISCALL cm_fn_007751e0(u32 self,u32 entry);
extern void CM_THISCALL cm_fn_00905c60(u32 self,u32 key,u32 first,u32 second);
extern unsigned long long CM_CDECL __attribute__((regparm(2))) cm_fn_0090cfc0(u32 entry_eax,u32 entry_edx,u32 bound);
typedef u32 __attribute__((regparm(3))) (*physical0)(u32,u32,u32);
typedef u32 __attribute__((regparm(3),stdcall)) (*physical1)(u32,u32,u32,u32);

/* Select the x87 FISTP64 lowering used by native __ftol. FISTTP differs for
   exceptional values in the pinned execution environment. No optimization
   setting is changed; ordinary O0 and O2 both retain this ISA constraint. */
u32 CM_THISCALL __attribute__((target("no-sse3")))
cm_fn_006a3770(u32 edi,u32 record,u32 flag)
{
    u8 scratch[28];
    u32 L=(u32)scratch;
    u32 esi=record;
    u32 eax,ecx,edx,ebx=0,ebp=0,target,first,second;
    u32 value,slot;
    unsigned long long wide;
    register long double fp;
    R32(L+20u)=record;
    R32(L+24u)=flag;
    R32(L+4u)=0;

    ROW_D(); R16(edx+eax+0x0eu)=0;
    ROW_D(); R32(edx+eax+0x2cu)=0;
    eax=(u32)S16(esi+0x80u); ecx=eax*eax;
    eax=R32(esi); R32(L+8u)=ecx;
    fp=(long double)(s32)ecx;
    ecx=R32(edi); edx=eax*9u;
    fp*=R64(0x009689e0u);
    eax=eax+edx*8u; R64(eax+ecx+0x30u)=(double)fp;
    ROW_A(); fp=R64(eax+ecx+0x38u);
    fp*=R64(0x009689e0u); eax=eax+ecx+0x38u; R64(eax)=(double)fp;
    ROW_A(); R8(eax+ecx)=0;
    ROW_A(); if(R8(eax+ecx+2u)&1u) goto after_command;
    if(R32(esi+0xcfu)==0) goto after_command;
    eax=cm_fn_0053b900((u32 *)esi); if(eax!=0) goto after_command;
    edx=R32(esi+0xcfu); second=R32(edx+0x42u); first=R32(edx+0x3eu);
    eax=cm_fn_0054cdb0(0x00ae2c90u,first,second); ebx=eax;
    if((s32)ebx<=0xe42) goto command_short;
    ROW_A(); if((s16)R16(eax+ecx+6u)>=0xdac) goto command_short;
    eax=R32(esi+0xbfu); if(eax==0) goto command_long_default;
    if(R32(eax+0x69u)==0) goto command_long_default;
    eax=(u32)cm_fn_0090cfc0(eax,edx,0x1eu);
    edx=R32(esi+0xbfu); ecx=R32(edx+0x69u); edx=(u32)S8(ecx+0x1du);
    if((s32)eax<=(s32)edx) goto after_command;
    eax=R32(esi); R32(L+10u)=eax; R8(L+8u)=1; R8(L+9u)=6;
    goto append_command;
command_long_default:
    eax=(u32)cm_fn_0090cfc0(eax,edx,0x1eu);
    if((s32)eax<=10) goto after_command;
    goto command_store_edx;
command_short:
    if((s32)ebx<=0x721) goto after_command;
    ROW_D(); if((s16)R16(edx+eax+6u)>=3000) goto after_command;
    eax=R32(esi+0xbfu); if(eax==0) goto command_short_default;
    if(R32(eax+0x69u)==0) goto command_short_default;
    eax=(u32)cm_fn_0090cfc0(eax,edx,0x14u);
    ecx=R32(esi+0xbfu); edx=R32(ecx+0x69u); ecx=(u32)S8(edx+0x1du);
    if((s32)eax<=(s32)ecx) goto after_command;
command_store_edx:
    edx=R32(esi); R32(L+10u)=edx; R8(L+8u)=1; R8(L+9u)=6;
    goto append_command;
command_short_default:
    eax=(u32)cm_fn_0090cfc0(eax,edx,0x14u);
    if((s32)eax<=10) goto after_command;
    ecx=R32(esi); edx=L+8u; R32(L+10u)=ecx; R8(L+8u)=1; R8(L+9u)=6;
append_command:
    ecx=R32(edi+4u);
    (void)cm_fn_0068d0d0((volatile u8 *)ecx,edx,(const u8 *)(L+8u),6);
after_command:
    eax=cm_fn_0053b900((u32 *)esi); if(eax==0) goto ordinary;
    eax=cm_fn_005409b0((u32 *)esi); if(eax!=0) goto special_notify;
    eax=R32(esi+0x53u); ecx=R32(0x00b74318u);
    ebp=cm_fn_007751e0(ecx,eax); if(ebp==0) goto special_missing;
    ecx=R32(ebp); edx=R32(0x00adadfcu); ecx=R32(edx+ecx*4u);
    if(ecx==0) goto special_missing;
    eax=R32(ecx); target=R32(eax+0x70u);
    eax=((physical1)target)(eax,edx,ecx,esi);
    ecx=R32(esi); edx=ecx*9u; ecx=ecx+edx*8u; edx=R32(edi);
    R16(ecx+edx+0x0eu)=(u16)eax;
    ROW_D(); ecx=R32(ebp); R32(edx+eax+0x2cu)=ecx;
    if(R8(L+24u)==0) goto update;
    ROW_A(); edx=R32(eax+ecx+0x2cu); eax+=ecx;
    eax=LOW16(eax,R16(eax+0x0eu));
    cm_fn_006a44a0((u32 *)esi,(s16)eax,edx); goto update;
special_missing:
    ROW_D(); R16(edx+eax+0x0eu)=0;
    ROW_D(); R32(edx+eax+0x2cu)=0xffffffffu;
special_notify:
    if(R8(L+24u)!=0) cm_fn_006a44a0((u32 *)esi,0,0xffffffffu);
    goto update;
ordinary:
    eax=R32(esi+0x53u); ecx=R32(0x00de1f58u);
    cm_fn_00905c60(ecx,eax,L+20u,L+3u);
    if(R32(esi+0xcfu)==ebp) goto update;
    ecx=R32(esi+0x57u); if(ecx==0) goto missing_club;
    eax=R32(esi+0x53u); if(eax==0) goto missing_club;
    if(R8(eax+0x11cu)==0) goto missing_club;
    ecx=R32(ecx); edx=R32(0x00adadfcu); eax=R32(edx+ecx*4u);
    if(eax==0) goto missing_club;
    ecx=R32(eax+8u); ebp=cm_fn_004a4850(ecx,esi);
    eax=R32(esi+0x1dbu); R32(L+8u)=ebp;
    if((s32)eax>-1 && eax!=R32(0x009cf6f8u)) {
        edx=R32(0x00ae23d0u); ecx=eax*107u;
        eax=R32(ecx+edx); ecx=R32(0x00adadfcu); edx=R32(ecx+eax*4u);
        ecx=R32(edx+8u); R32(L+4u)=cm_fn_004a4850(ecx,esi);
    }
    if(ebp==0) goto early;
    value=R8(ebp+0x72u); if(value<1u) goto early;
    ROW_D(); R32(edx+eax+0x2cu)=value;
    eax=R32(esi+0x57u); edx=R32(0x00adadfcu); ecx=R32(eax); eax=R32(edx+ecx*4u);
    ecx=(u32)S16(eax+0x3eu);
    eax=R32(esi); edx=eax*9u; eax=eax+edx*8u; edx=R32(edi); eax=eax+edx+0x2cu;
    if(R32(eax)==ecx) R32(eax)=ecx-1u;
    ecx=R32(0x00ae2a38u); ebp=cm_fn_005a0590(ecx,esi);
    eax=(u32)(s32)(s8)cm_fn_005986b0(ebp); eax+=2u;
    if(eax>4u) goto adjust_club;
    target=R32(0x006a4484u+eax*4u); /* retain original jump-table read */
    if(target==0x006a3be0u) goto rating_plus;
    if(target==0x006a3bc7u) goto rating_plus_low;
    if(target==0x006a3b7bu) goto rating_money;
    if(target==0x006a3b5fu) goto rating_one;
    /* Read-only pinned table, index4 ->006a3b40. */
    ROW_D(); ecx=R32(edx+eax+0x2cu); eax=edx+eax+0x2cu;
    if((s32)ecx<=5) ecx-=2u; else ecx-=3u;
    R32(eax)=ecx; goto adjust_club;
rating_one:
    ROW_D(); ecx=R32(edx+eax+0x2cu); eax=edx+eax+0x2cu;
    if((s32)ecx<=5) --ecx; else ecx-=2u;
    R32(eax)=ecx; goto adjust_club;
rating_money:
    wide=(unsigned long long)cm_fn_00598800(ebp,0,0,0,1,0);
    if((long long)wide<=(long long)(S16(esi+0x80u)*250)) goto adjust_club;
    ROW_D(); ecx=R32(edx+eax+0x2cu); eax=edx+eax+0x2cu;
    if((s32)ecx>5) R32(eax)=ecx-1u;
    goto adjust_club;
rating_plus_low:
    ROW_D(); ecx=R32(edx+eax+0x2cu); eax=edx+eax+0x2cu;
    if((s32)ecx<5) R32(eax)=ecx+1u;
    goto adjust_club;
rating_plus:
    ROW_D(); ecx=R32(edx+eax+0x2cu); eax=edx+eax+0x2cu; R32(eax)=ecx+1u;
adjust_club:
    eax=R32(esi+0x57u); if(eax==0) goto adjust_age;
    ecx=R32(esi+0x5bu); if(ecx==0 || eax==ecx) goto adjust_age;
    value=R16(eax+0x69u); eax=R32(esi); slot=R16(ecx+0x69u);
    ecx=eax*9u; edx=eax+ecx*8u; eax=R32(edi);
    if((s16)value<=(s16)slot) {
        ecx=edx+eax+0x2cu; eax=R32(edx+eax+0x2cu);
        if((s32)eax>10) R32(ecx)=eax-7u;
        else if((s32)eax>5) R32(ecx)=eax-3u;
        else if((s32)eax>2) R32(ecx)=eax-2u;
        goto clamp;
    }
    ecx=R32(edx+eax+0x2cu); eax=edx+eax+0x2cu;
    if((s32)ecx>10) R32(eax)=ecx+1u;
    ROW_D(); ecx=R32(edx+eax+0x2cu); eax=edx+eax+0x2cu;
    R32(eax)=ecx+((s32)ecx<=5?4u:2u); goto clamp;
adjust_age:
    if((s32)ebx<0x16d) {
        ROW_D(); ecx=R32(edx+eax+0x2cu); eax=edx+eax+0x2cu; R32(eax)=ecx+1u;
    } else if((s32)ebx>0x447) {
        ROW_D(); ecx=R32(edx+eax+0x2cu); eax=edx+eax+0x2cu;
        if((s32)ecx>10) R32(eax)=ecx-1u;
    } else if((s32)ebx>0x721) { /* original unreachable age branch */
        ROW_D(); ecx=R32(edx+eax+0x2cu); eax=edx+eax+0x2cu;
        if((s32)ecx>10) R32(eax)=ecx-2u;
        ROW_D(); ecx=R32(edx+eax+0x2cu); eax=edx+eax+0x2cu;
        if((s32)ecx>5) R32(eax)=ecx-1u;
    }
clamp:
    ROW_D(); ecx=edx+eax+0x2cu; eax=R32(edx+eax+0x2cu);
    if((s32)eax<1) eax=1;
    else {
        edx=R32(esi+0x57u); ebx=R32(0x00adadfcu); edx=R32(edx);
        edx=R32(ebx+edx*4u); edx=(u32)S16(edx+0x3eu)-1u;
        if((s32)edx<(s32)eax) eax=edx;
    }
    R32(ecx)=(u32)(s32)(s8)eax;
    ROW_D(); ebp=edx+eax; value=R8(edx+eax+2u);
    if((value&1u)!=0) {
        ebx=R32(L+8u);
        if(R8(ebx+0x74u)>=1u && R8(ebx+0x75u)>=1u) {
            eax=R32(ebp+0x2cu); ecx=R8(ebx+0x73u); ecx+=eax; R32(L+8u)=ecx;
            fp=(long double)(s32)ecx; fp*=R64(0x00968968u);
            eax=(u32)(long long)fp; /* native __ftol VA006a3d89 */
            R32(ebp+0x2cu)=(u32)(s32)(s8)eax;
            ROW_D(); ecx=R8(ebx+0x74u); edx=edx+eax+0x2cu; eax=R32(edx);
            if((s32)ecx>(s32)eax) eax=ecx;
            else { ecx=R8(ebx+0x75u); if((s32)ecx<(s32)eax) eax=ecx; }
            R32(edx)=(u32)(s32)(s8)eax;
        }
    }
    edx=R32(esi+0x57u); ecx=R32(0x00adadfcu); eax=R32(edx);
    ecx=R32(ecx+eax*4u); edx=R32(ecx); target=R32(edx+0x98u);
    eax=((physical0)target)(eax,edx,ecx);
    if(eax!=0) goto flag_other;
    ROW_D(); ecx=(u32)S8(L+20u); eax+=edx;
    if((s32)R32(eax+0x2cu)>(s32)ecx) goto flag_other;
    R8(eax+0x0eu)|=2u; goto flags_done;
flag_other:
    eax=R32(esi+0x53u); if(eax==0) goto flag_third;
    edx=R32(eax); eax=R32(0x009cf224u); if(edx==eax) goto flag_third;
    eax=R32(esi+0x57u); edx=R32(0x00adadfcu); ecx=R32(eax);
    ecx=R32(edx+ecx*4u); eax=R32(ecx); target=R32(eax+0x98u);
    eax=((physical0)target)(eax,edx,ecx); if(eax!=0) goto flag_third;
    ROW_D(); ecx=(u32)S8(L+20u); eax+=edx; edx=(u32)S8(L+3u); ecx+=edx;
    edx=R32(eax+0x2cu); if((s32)edx>(s32)ecx) goto flag_third;
    R8(eax+0x0eu)|=1u; goto flags_done;
flag_third:
    eax=R32(esi+0x57u); edx=R32(0x00adadfcu); ecx=R32(eax);
    ecx=R32(edx+ecx*4u); eax=R32(ecx); target=R32(eax+0x98u);
    eax=((physical0)target)(eax,edx,ecx); ecx=R32(0x00adadfcu);
    if(eax!=0) {
        eax=R32(esi); edx=eax*9u; eax=eax+edx*8u; edx=R32(edi); eax+=edx;
        edx=R32(esi+0x57u); edx=R32(edx); ebx=R32(eax+0x2cu);
        edx=R32(ecx+edx*4u); edx=(u32)S8(edx+0xbeu);
        if((s32)ebx<=(s32)edx) { R8(eax+0x0eu)|=4u; goto flags_done; }
    }
    eax=R32(esi+0x57u); edx=R32(eax); ecx=R32(ecx+edx*4u);
    value=R8(ecx+0xbfu);
    if(value!=0) {
        eax=R32(esi); edx=(u32)(s32)(s8)value; ebx=eax*9u; eax=eax+ebx*8u;
        ebx=R32(edi); eax+=ebx;
        if((s32)R32(eax+0x2cu)<=(s32)edx) { R8(eax+0x0eu)|=8u; goto flags_done; }
    }
    eax=R32(esi); edx=eax*9u; eax=eax+edx*8u; edx=R32(edi); ebp=eax+edx;
    eax=(u32)S16(ecx+0x3eu); edx=R32(ebp+0x2cu);
    if((s32)edx<=(s32)eax/2) { R16(ebp+0x0eu)=0x40; goto flags_done; }
    eax=R32(ecx); target=R32(eax+0x9cu); eax=((physical0)target)(eax,edx,ecx);
    edx=R32(0x00adadfcu);
    if(eax!=0) {
        ecx=R32(esi+0x57u); eax=R32(ecx); ecx=R32(edx+eax*4u);
        eax=R32(esi); ebx=eax*9u; eax=eax+ebx*8u; ebx=R32(edi); eax+=ebx;
        ebx=(u32)S8(ecx+0xc1u); ecx=(u32)S16(ecx+0x3eu); ecx-=ebx;
        ebx=R32(eax+0x2cu);
        if((s32)ebx>=(s32)ecx) { R8(eax+0x0eu)|=0x20u; goto flags_done; }
    }
    eax=R32(esi+0x57u); ecx=R32(eax); ecx=R32(edx+ecx*4u);
    edx=R32(ecx); target=R32(edx+0x9cu); value=((physical0)target)(eax,edx,ecx);
    ROW_D(); R16(edx+eax+0x0eu)=value!=0?0x10:0;
flags_done:
    ROW_D(); if((R8(edx+eax+2u)&1u)!=0) goto notify;
    ecx=R32(esi+0x57u); eax=R32(0x00adadfcu); edx=R32(ecx);
    ecx=R32(eax+edx*4u); edx=R32(ecx); target=R32(edx+0x98u);
    eax=((physical0)target)(eax,edx,ecx); if(eax!=0) goto notify;
    eax=R32(esi+0xbfu); if(eax==0) goto notify;
    ecx=R32(eax+0x69u); if(ecx==0) goto notify;
    eax=R32(esi); ecx=eax*9u; edx=eax+ecx*8u; eax=R32(edi); ebp=edx+eax;
    eax=(u32)cm_fn_0090cfc0(eax,edx,0x32u);
    ecx=R32(esi+0xbfu); edx=R32(ecx+0x69u); ecx=R32(ebp+0x2cu);
    edx=(u32)S8(edx+0x1du); ecx*=5u; edx+=ecx;
    if((s32)eax>(s32)edx) R8(ebp+0x0eu)|=0x80u;
    else {
        ROW_A(); ebp=eax+ecx; eax=(u32)cm_fn_0090cfc0(eax,edx,0x4bu);
        edx=R32(esi+0xbfu); ecx=R32(edx+0x69u); edx=(u32)S8(ecx+0x1du);
        ecx=R32(ebp+0x2cu); ecx*=5u; edx+=ecx;
        if((s32)eax>(s32)edx) R8(ebp+0x0fu)|=1u;
        else {
            ROW_A(); ebp=eax+ecx; eax=(u32)cm_fn_0090cfc0(eax,edx,100u);
            edx=R32(esi+0xbfu); ecx=R32(edx+0x69u); edx=(u32)S8(ecx+0x1du);
            ecx=R32(ebp+0x2cu); ecx*=5u; edx+=ecx;
            if((s32)eax>(s32)edx) R8(ebp+0x0fu)|=2u;
        }
    }
    ebx=R32(L+4u); if(ebx==0) goto notify;
    if(R8(ebx+0x74u)>1u) goto notify;
    edx=R32(esi+0xbfu); eax=R32(edx+0x69u); edx=(u32)S8(eax+0x1du);
    edx=(u32)((s32)(29u-edx)/5); eax=R8(ebx+0x72u);
    if((s32)eax<=(s32)edx) { ROW_D(); R8(edx+eax+0x0fu)|=4u; goto notify; }
    edx+=4u; if((s32)eax>(s32)edx) goto notify;
    ROW_D(); R16(edx+eax+0x0eu)^=0x80u;
    ROW_D(); R16(edx+eax+0x0eu)^=0x200u;
    ROW_D(); R8(edx+eax+0x0fu)|=1u;
    goto notify;
early:
    ROW_D(); R32(edx+eax+0x2cu)=8u;
    ROW_D(); R8(edx+eax+0x0eu)|=0x40u;
    ROW_D(); ecx=LOW16(ecx,R16(edx+eax+0x0eu));
    return cm_fn_006a4a20((u32 *)esi,(s16)ecx);
missing_club:
    ROW_A(); R16(eax+ecx+0x0eu)=0x10u;
notify:
    if(R8(L+24u)!=0) {
        ROW_A(); edx=LOW16(edx,R16(eax+ecx+0x0eu));
        (void)cm_fn_006a4a20((u32 *)esi,(s16)edx);
    }
update:
    ROW_D(); ebp=edx+eax+6u; value=R16(edx+eax+6u);
    if((s16)value<0x1964) {
        ecx=(u32)(s32)(s16)value; R32(L+24u)=ecx;
        fp=(long double)(s32)ecx; fp*=R64(0x009688a0u); fp+=R64(0x0096d288u); fp*=R64(0x00967880u);
        eax=(u32)(long long)fp; /* native __ftol VA006a4199 */
        R16(ebp)=(u16)eax;
        ROW_A(); edx=(u32)S16(eax+ecx+8u); ebp=eax+ecx+8u; R32(L+24u)=edx;
        fp=(long double)(s32)edx; fp*=R64(0x009688a0u); fp+=R64(0x009688d0u); fp*=R64(0x00967880u);
        eax=(u32)(long long)fp; /* native __ftol VA006a41cf */
        R16(ebp)=(u16)eax;
        ROW_D(); ecx=(u32)S16(edx+eax+0x0au); ebp=edx+eax+0x0au; R32(L+24u)=ecx;
        fp=(long double)(s32)ecx; fp*=R64(0x009688a0u); fp+=R64(0x009688d0u); fp*=R64(0x00967880u);
        eax=(u32)(long long)fp; /* native __ftol VA006a4205 */
        R16(ebp)=(u16)eax;
        ROW_A(); edx=(u32)S16(eax+ecx+0x0cu); ebp=eax+ecx+0x0cu; R32(L+24u)=edx;
        fp=(long double)(s32)edx; fp*=R64(0x009688a0u); fp+=R64(0x009688d0u); fp*=R64(0x00967880u);
        eax=(u32)(long long)fp; /* native __ftol VA006a423b */
        R16(ebp)=(u16)eax;
    }
    eax=cm_fn_0053b900((u32 *)esi); if(eax!=0) goto clear_result;
    ecx=R32(0x00ae2a38u); eax=cm_fn_005a0590(ecx,esi); if(eax==0) goto clear_result;
    value=cm_fn_005986b0(eax); ecx=R32(esi); edx=ecx*9u; ecx=ecx+edx*8u;
    edx=R32(edi); R8(ecx+edx+1u)=(u8)value; goto training;
clear_result:
    ROW_D(); R8(edx+eax+1u)=0;
training:
    eax=R32(esi+0xcfu); if(eax==0) goto training_again;
    ebx=R32(eax+0x69u); if(ebx==0) goto training_again;
    value=R16(ebx+0x0au); slot=R16(esi+0x80u);
    if((s16)value>=(s16)slot) goto training_again;
    ecx=(u32)(s32)(s16)slot; edx=(u32)(s32)(s16)value; ecx-=edx; R32(L+24u)=ecx;
    fp=(long double)(s32)ecx; fp*=R64(0x00969518u);
    eax=(u32)(long long)fp; /* native __ftol VA006a42ca */
    R16(ebx+0x0au)=(u16)(value-eax);
    eax=R32(esi+0xcfu); eax=R32(eax+0x69u);
    if((s16)R16(eax+0x0au)>10000) R16(eax+0x0au)=10000;
    ecx=R32(esi+0xcfu); edx=(u32)S16(esi+0x80u); ebx=R32(ecx+0x69u);
    value=R16(ebx+0x0cu); eax=(u32)(s32)(s16)value; edx-=eax; R32(L+24u)=edx;
    fp=(long double)(s32)edx; fp*=R64(0x0096d280u);
    eax=(u32)(long long)fp; /* native __ftol VA006a4314 */
    R16(ebx+0x0cu)=(u16)(value-eax);
    ecx=R32(esi+0xcfu); eax=R32(ecx+0x69u);
    if((s16)R16(eax+0x0cu)>10000) R16(eax+0x0cu)=10000;
training_again:
    eax=R32(esi+0xcfu); if(eax==0) goto cleanup;
    ebx=R32(eax+0x69u); if(ebx==0) goto cleanup;
    slot=R16(esi+0x80u); value=R16(ebx+0x0au);
    if((s16)slot<=(s16)value) goto cleanup;
    edx=(u32)(s32)(s16)slot; eax=(u32)(s32)(s16)value; edx-=eax; R32(L+24u)=edx;
    fp=(long double)(s32)edx; fp*=R64(0x00969518u);
    eax=(u32)(long long)fp; /* native __ftol VA006a436e */
    R16(ebx+0x0au)=(u16)(value-eax);
cleanup:
    eax=R32(esi+0xcfu);
    if(eax!=0) {
        ecx=R32(0x00ae2364u); ebx=R32(eax); ecx-=16u;
        if((s32)ebx>=(s32)ecx) {
            value=R8(eax+0x59u); if((s8)value<20) R8(eax+0x59u)=(u8)(value+1u);
        }
    }
    eax=R32(esi); ecx=eax*9u; eax=eax+ecx*8u; ecx=R32(edi); eax=eax+ecx+2u; R8(eax)&=0xfeu;
    eax=R32(esi); ecx=eax*9u; eax=eax+ecx*8u; ecx=R32(edi); R8(eax+ecx+4u)=0;
    eax=R32(esi); ecx=eax*9u; eax=eax+ecx*8u; ecx=R32(edi); R8(eax+ecx+3u)&=0xe1u;
    eax=R32(esi); ecx=eax*9u; eax=eax+ecx*8u; ecx=R32(edi); eax=eax+ecx+2u; R8(eax)&=0xdfu;
    eax=R32(esi); ecx=eax*9u; eax=eax+ecx*8u; ecx=R32(edi); R32(eax+ecx+0x28u)=0;
    eax=R32(esi); ecx=eax*9u; eax=eax+ecx*8u; ecx=R32(edi); R8(eax+ecx+5u)=0;
    eax=R32(esi); ecx=eax*9u; eax=eax+ecx*8u; ecx=R32(edi); R8(eax+ecx+3u)&=0xfeu;
    eax=R32(esi); ecx=eax*9u; eax=eax+ecx*8u; ecx=R32(edi); eax=eax+ecx+0x40u; R8(eax)&=7u;
    eax=R32(esi); ecx=eax*9u; eax=eax+ecx*8u; ecx=R32(edi); R16(eax+ecx+0x12u)=0;
    eax=R32(esi); ecx=eax*9u; eax=eax+ecx*8u; ecx=R32(edi); R16(eax+ecx+0x14u)=0;
    eax=R32(esi); ecx=eax*9u; eax=eax+ecx*8u; ecx=R32(edi); R16(eax+ecx+0x16u)=0;
    ecx=R32(0x00ae2a3cu); if(ecx!=0) eax=cm_fn_005a5930(ecx,esi);
    return eax;
}

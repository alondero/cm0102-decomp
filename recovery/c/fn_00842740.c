#include "cmre.h"
#define M(t,a) CM_GLOBAL(volatile t,(u32)(a))
#define P(p) ((u32)(p))
typedef u16 alias16 __attribute__((__may_alias__));
/* ABI evidence: preferred-image VAs 00842764-0084277a,
   008427e2-008427eb, 00842844-00842859, 00842987-00842990,
   00842a17-00842a34, 00842a63-00842a6b and 00842ada-00842aec.
   CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Calls consume the displayed pushes with ECX as receiver; date helpers'
   returned EAX pointers are consumed at 00842a39 and 00842a70. */
extern u32 CM_THISCALL cm_fn_00849ce0(u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32,u32);
extern u32 CM_THISCALL cm_fn_00843e10(u32,u32,u32,u32,u32);
extern long double CM_THISCALL cm_fn_00846920(u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0054cfb0(u32);
extern u32 CM_THISCALL cm_fn_0054cf70(u32);
extern u32 CM_THISCALL cm_fn_008d0590(u32,u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00549f70(u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_008d0530(u32,u32,u32,u32,u32,u32);
extern u8 CM_CDECL cm_fn_00669360(u32);
extern u32 CM_THISCALL cm_fn_0054cab0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0054c770(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_008bed60(u32,u32,u32,u32,u32,u32,u32);

u32 CM_THISCALL cm_fn_00842740(u32 self, volatile u32 person,
                                             volatile u32 club)
{
    volatile struct { u32 prev, handler, state; } seh;
    u8 scratch[36];
    u32 edi=person, esi=club, ebx, eax, edx, rec, result=0xffffffffu;
    u32 span, lower, base, tmp, value, year;
    u8 month;
    long double f;
    seh.prev=CM_FS(u32,0);
    seh.state=0xffffffffu;
    seh.handler=0x9624b8;
    CM_FS(u32,0)=P(&seh);
    ebx=cm_fn_00849ce0(self,esi,edi,0,0);
    eax=cm_fn_004cf5a0(M(u32,0xae1050),edi);
    M(u32,0xae1080)=eax;
    if (eax) {
        eax=M(u32,eax+4);
        edx=M(u32,0xae23b4)+eax;
        rec=edx+eax*580u;
        if (rec) goto found;
    }
    if(ebx) cm_fn_00843e10(self,edi,esi,1,0);
    goto done;
found:
    month=(M(u8,ebx+0x24)>>1)&15;
    base=M(u32,0xde1f54);
    base=M(u32,base+0x856);
    f=cm_fn_00846920(base,edi,esi,0,1,(edx&0xffffff00u)|month,0);
    if (!(f>=M(double,0x968e38))) goto done;
    eax=cm_fn_0054cfb0(0xae2c90);
    month=(u8)eax;
    M(u8,P(&person))=month;
    eax=cm_fn_004cf5a0(M(u32,0xae1050),edi);
    M(u32,0xae1080)=eax;
    if(eax) {
        eax=M(u32,eax+4);
        edx=M(u32,0xae23b4);
        eax=edx+eax*581u;
    }
    eax=cm_fn_008d0590(M(u32,0xde1f54),esi,eax,edi,
                       P(scratch+3),P(scratch+6),P(&club),P(scratch+4));
    edx=M(u16,0xae2c92);
    lower=1;
    span=(u16)(M(u16,P(scratch+4))-edx)*12u
        -(u32)(s32)(s8)month+(u32)(s32)(s8)M(u8,P(&club));
    value=(u16)(M(u16,P(scratch+6))-edx)*12u
        -(u32)(s32)(s8)month+(u32)(s32)M(s8,P(scratch+3));
    if((s16)value>=1) lower=value;
    M(u32,P(scratch+8))=lower;
    if((s16)lower>(s16)span) goto done;
    eax=M(u32,esi+0x53);
    if(eax) {
        edx=M(u32,eax);
        eax=M(u32,0x9cf408);
        if(edx==eax) {
            s8 m=M(s8,P(&person));
            if(m>=10 || m<=2) goto done;
        }
    }
    f=cm_fn_00846920(self,edi,esi,0,0,1,0);
    if(f>=M(double,0x9690a8)) {
        eax=M(u32,esi+0x53);
        base=M(u32,0x9cf488);
        eax=M(u32,eax);
        if(eax!=base && eax!=M(u32,0x9cf434) && eax!=M(u32,0x9cf360))
            span=(span&0xffffff00u)|(u8)lower;
    }
    month=(u8)(span+M(u8,P(&person)));
    M(alias16,P(&person))=M(u16,0xae2c92);
    M(u8,P(scratch+8))=month;
    if((s8)month>11) {
        edx=(u32)(s32)(s8)month/12u;
        month=(u8)(month+(u8)edx*-12);
        M(u8,P(scratch+8))=month;
        person=person+edx;
    }
    eax=cm_fn_0054cf70(0xae2c90);
    if((s16)eax>28) eax=28;
    else eax=(u32)(s32)(s16)cm_fn_0054cf70(0xae2c90);
    year=person;
    value=M(u32,P(scratch+8));
    cm_fn_00549f70(P(scratch+20),eax,value,year,0xffffffffu);
    base=M(u32,0xde1f54);
    seh.state=0;
    eax=cm_fn_008d0530(base,esi,edi,0,P(scratch+20),1);
    if(!eax) goto done;
    eax=cm_fn_004cf5a0(M(u32,0xae1050),edi);
    if(!eax) goto done;
    value=M(u32,eax+0x2d);
    tmp=M(u32,eax+0x31);
    M(u32,P(scratch+8))=value;
    M(u32,P(scratch+12))=tmp;
    year=M(u16,P(scratch+22));
    if((value>>16)<year) goto done;
    if((value>>16)==year && (s16)value<M(s16,P(scratch+20))) goto done;
    eax=(u32)(s32)(s8)cm_fn_00669360(esi);
    ebx=M(u32,0xb63d8a+eax*72u);
    M(u32,P(scratch+8))=ebx;
    edx=M(u32,0xb63d8e + eax*72u);
    M(u32,P(scratch+12))=edx;
    eax=cm_fn_0054cab0(P(scratch+20),P(scratch+28),4);
    year=M(u16,eax+2);
    tmp=M(u16,P(scratch+10));
    if(tmp>year) goto done;
    if(tmp==year && (s16)ebx>M(s16,eax)) goto done;
    eax=cm_fn_0054c770(0xae2c90,P(scratch+28),3);
    value=M(u32,eax);
    tmp=M(u32,eax+4);
    edx=M(u16,P(scratch+22));
    M(u32,P(scratch+8))=value;
    M(u32,P(scratch+12))=tmp;
    if((value>>16)>edx) goto done;
    if((value>>16)==edx && (s16)value>M(s16,P(scratch+20))) goto done;
    {
        s32 difference=(s32)M(s16,esi+0x80)-(s32)M(s16,rec+0x80);
        if(difference>2500) M(u8,P(&person))=100;
        else M(u8,P(&person))=difference<=-1500?0:50;
    }
    value=person;
    result=cm_fn_008bed60(M(u32,0xde1f54),edi,esi,P(scratch+20),value,1,1);
done:
    CM_FS(u32,0)=seh.prev;
    return result;
}

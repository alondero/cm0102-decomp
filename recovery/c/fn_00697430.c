/* Preferred-image VA00697430/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   ECX receiver, RET8. The final EAX is the last field address. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32,(a))
#define B(a) CM_GLOBAL(volatile s8,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
extern u32 CM_THISCALL cm_fn_004cf5f0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00697240(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0062ed70(u32,u32);
extern u32 CM_CDECL cm_fn_00547790(u32,u32);
extern u32 CM_CDECL cm_fn_00547810(u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,void *,void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

static u32 field(u32 self,u32 club,u32 offset)
{
    u32 index=D(club),base=D(self);
    return base+index*73u+offset;
}
static void subtract_at(u32 address,u32 amount)
{
    W(address)=(u16)(W(address)-amount);
}
static void subtract_three(u32 self,u32 club,u32 first,u32 second,u32 third)
{
    subtract_at(field(self,club,6),first);
    subtract_at(field(self,club,0x14),second);
    subtract_at(field(self,club,0x16),third);
}
u32 CM_THISCALL cm_fn_00697430(u32 self,u32 person,u32 club)
{
    u8 scratch[0x200];
    u32 roster,code,other,p,result,base,index;
    s8 age,rate;
    roster=cm_fn_004cf5f0(D(0x00ae1050),person,club);
    if(!roster || !club || !person) {
        u32 line,version;
        void *first,*second;
        if(!roster){line=0x20c5;first=scratch+0x100;second=scratch;}
        else {line=!club?0x20cc:0x20d1;first=scratch;second=scratch+0x100;}
        cm_fn_00944cff(0x009f18a4,0,0,first,second);
        version=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,first,second,line);
        result=cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
        D(0x00b67a34)=0;
        return result;
    }
    code=cm_fn_00697240(self,person,club);
    other=D(0x00b63d28);
    age=B(person+0x18);
    other=cm_fn_0062ed70(other,person);
    if(code==0x400) {
        if(age>31 && other && (s16)W(other+0x28)>90)
            subtract_three(self,club,125,250,200);
        rate=B(roster+0x3d);
        if(rate>50) subtract_three(self,club,500,1000,1000);
        else {
            /* Native0069760c reads the receiver before the club index. */
            base=D(self);index=D(club);p=base+index*73u+6;
            if(rate>0) {
                subtract_at(p,250);
                subtract_at(field(self,club,0x14),500);
                subtract_at(field(self,club,0x16),500);
            } else {
                subtract_at(p,125);
                subtract_at(field(self,club,0x14),250);
            }
        }
    } else if(code==2) {
        if(age>31 && other && (s16)W(other+0x28)>90)
            subtract_three(self,club,25,50,50);
        else {
            rate=B(roster+0x3d);
            if(rate>50) subtract_three(self,club,125,250,250);
            else if(rate>0) subtract_three(self,club,25,50,50);
            else {
                subtract_at(field(self,club,6),10);
                subtract_at(field(self,club,0x14),10);
            }
        }
    } else if(code==1) {
        if(age>31 && other && (s16)W(other+0x28)>90)
            subtract_three(self,club,25,25,150);
        rate=B(roster+0x3d);
        if(rate>50) subtract_three(self,club,250,500,500);
        else {
            /* Native006977da reads the receiver before the club index. */
            base=D(self);index=D(club);p=base+index*73u+6;
            if(rate>0) {
                subtract_at(p,125);
                subtract_at(field(self,club,0x14),250);
                subtract_at(field(self,club,0x16),250);
            } else {
                subtract_at(p,50);
                subtract_at(field(self,club,0x14),100);
            }
        }
    }
    if(cm_fn_00547790(person,club)) subtract_at(field(self,club,0x16),500);
    other=D(club+0xbf);
    if(cm_fn_00547810(person,other)) subtract_at(field(self,club,0x16),500);
    p=field(self,club,6);
    if((s16)W(p)<1)W(p)=1;
    p=field(self,club,0x14);
    if((s16)W(p)<=-10000)W(p)=0xd8f0;
    p=field(self,club,0x16);
    if((s16)W(p)<=-10000)W(p)=0xd8f0;
    return p;
}

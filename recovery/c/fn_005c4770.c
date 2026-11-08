/* Preferred-image VA005C4770, base00400000; CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   ECX receiver, RET0C. The direct caller at005C2926 discards EAX.
   Native original callees remain boundaries; register-prefix C projections of
   the RNG/sort are not their native stack ABI. Scratch uses original L offsets,
   where L is ESP immediately after SUB ESP,250h. */
#include "cmre.h"
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))
#define SB(a) (*(volatile s8 *)(u32)(a))
extern void CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,void *,void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern void CM_CDECL cm_fn_005e8290(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0054cab0(u32,u32,u8);
extern u32 CM_CDECL cm_fn_00946464(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00600a50(u32,u32,u8,u32,u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern u32 CM_THISCALL cm_fn_005c4cb0(u32,u32,u32,u32,s8,u32);
extern u32 CM_THISCALL cm_fn_005c4380(u32,u32,u32,u32);
extern void CM_CDECL cm_fn_00945333(u32,u32,u32,u32);
void CM_THISCALL cm_fn_005c4770(u32 self,u32 p2,u32 array,u32 count)
{
    u8 scratch[0x250];
    u32 L=(u32)scratch, old,allocated,version,table,n,left,dates,date,returned;
    u32 found,end,element,index,club,nation,other,target,result;
    u16 year;
    u8 draw,limit,tries,rate;
    s32 q,hi;
    long long product;
    long double fp;
    old=D(self+0x34d8);
    if(old)cm_fn_009452ca(old);
    D(self+0x34e4)=100;
    D(self+0x34e0)=0;
    D(self+0x34dc)=0;
    allocated=cm_fn_00944e46(2000);
    D(self+0x34d8)=allocated;
    if(!allocated) {
        cm_fn_00944cff(0x009cd0a0,0,0,(void *)(L+0x150),(void *)(L+0x50));
        version=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,L+0x150,L+0x50,0x6f1);
        cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
        D(0x00b67a34)=0;
        return;
    }
    year=W(0x00ae2c92);
    table=0x00b63d60;
    left=0x23;
    do {
        nation=D(table);
        if(!nation)goto next;
        if(!B(nation+0x11c))goto next;
        date=D(table+0x2a);
        dates=D(table+0x2e);
        D(L+0x28)=date;
        D(L+0x2c)=dates;
        if((u16)(date>>16)<year)goto next;
        if((u16)(date>>16)==year && (s16)date<(s16)W(0x00ae2c90))goto next;
        returned=cm_fn_0054cab0(table+0x39,L+0x48,0x34);
        date=D(returned);
        dates=D(returned+4);
        year=W(0x00ae2c92);
        D(L+0x20)=date;
        D(L+0x24)=dates;
        if((u16)(date>>16)>year)goto next;
        if((u16)(date>>16)==year && (s16)date>(s16)W(0x00ae2c90))goto next;
        nation=D(table);
        D(L+0x30)=0;
        B(L+0x34)=0;
        B(L+0x35)=0xff;
        n=D(nation);
        D(L+0x36)=n;
        D(L+0x3a)=0;
        D(L+0x3e)=0;
        D(L+0x42)=0;
        D(L+0x18)=L+0x30;
        found=cm_fn_00946464(L+0x18,array,count,4,0x005c5290);
        if(!found)goto reload;
        if(found>array) {
            n=D(table);
            target=D(n);
            do {
                element=D(found-4);
                found-=4;
                if(D(element+6)!=target) {found+=4;break;}
            }while(found>array);
        }
        end=array+count*4u-4u;
        if(found>=end)goto reload;
        do {
            element=D(found);
            index=D(element);
            club=D(0x00ae23b4)+index*0x245u;
            nation=D(club+0x53);
            n=D(table);
            if(nation!=n)goto reload;
            result=cm_fn_00600a50(club,1,1,0,0);
            if(result)goto advance;
            element=D(found);
            n=D(0x00ae23b4);
            index=D(element);
            other=D(n+index*0x245u+0x1db);
            target=D(0x009cf6f8);
            if(other==target)goto advance;
            index=D(element);
            club=n+index*0x245u;
            other=D(club+0x57);
            if(!other)goto advance;
            index=D(other);
            n=D(0x00adadfc);
            if(!D(n+index*4u))goto advance;
            draw=(u8)(cm_fn_0090cfc0(3)+1u);
            if(draw==3 && cm_fn_0090cfc0(4)==0)draw=2;
            q=(s16)W(club+0x80);
            product=(long long)q*0x10624dd3;
            hi=(s32)(product>>32);
            q=hi>>5;
            q+=(u32)q>>31;
            rate=(u8)(q+draw);
            limit=(u8)(cm_fn_0090cfc0(3)+4u);
            tries=0;
            if(SB(element+4)>=(s8)limit)goto advance;
            while((s8)tries<(s8)limit) {
                result=cm_fn_005c4cb0(self,element,p2,count,(s8)rate,0);
                if(result) {
                    u8 first=B(element+5);
                    u8 second=B(result+5);
                    if((s8)first>(s8)second)result=cm_fn_005c4380(self,element,result,element);
                    else result=cm_fn_005c4380(self,element,element,result);
                    if(!result)goto attempted;
                }
                fp=(long double)(s32)(s8)rate;
                fp*=CM_GLOBAL(double,0x009689b0);
                /* Original __ftol call at005C4AC2. */
                rate=(u8)(long long)fp;
                if((s8)rate<1)rate=1;
attempted:
                tries++;
                if(SB(element+4)>=(s8)limit)break;
            }
advance:
            found+=4;
        }while(found<end);
reload:
        year=W(0x00ae2c92);
next:
        table+=0x48;
        left--;
    }while(left);
    n=D(self+0x34e0);
    old=D(self+0x34d8);
    cm_fn_00945333(old,n,0x14,0x00778e90);
}


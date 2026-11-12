/* Preferred-image VA008d1130/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS4 RET16, u32 result. Original normal return calls 0068d040, then
   replaces only AL with the compact count (008d1d84), preserving the upper
   24 bits from the actual destructor result. Original callees remain numeric boundaries. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32,(u32)(a))
#define W(a) CM_GLOBAL(volatile u16,(u32)(a))
#define B(a) CM_GLOBAL(volatile u8,(u32)(a))
#define Q(a) CM_GLOBAL(volatile double,(u32)(a))
extern u32 CM_THISCALL cm_fn_004cf5a0(u32,u32);
extern u32 CM_THISCALL cm_fn_004cf6d0(u32,u32);
extern u32 CM_CDECL cm_fn_0053b620(u32);
extern u32 CM_CDECL cm_fn_005407e0(u32,u32,u32);
extern u32 CM_CDECL cm_fn_00540a50(u32,u32,u32);
extern void CM_FASTCALL cm_fn_0054a110(u32);
extern void CM_CDECL cm_fn_005e8290(const char *,const char *,u32);
extern u32 CM_THISCALL cm_fn_0068d020(u32);
extern u32 CM_THISCALL cm_fn_0068d040(u32);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern u32 CM_STDCALL cm_fn_007875e0(u32);
extern long double CM_THISCALL cm_fn_007c1e30(u32,u32,u32,u32,float);
extern void CM_THISCALL cm_fn_00842180(u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00849ce0(u32,u32,u32,u32,u32);
extern long long CM_THISCALL cm_fn_008e7b50(u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,void *,void *);

static void report(u32 first,u32 second,u32 line)
{
    u32 version;
    cm_fn_00944cff(0xa8d514u,0,0,(void *)first,(void *)second);
    version=cm_fn_0090d130();
    cm_fn_00944c9f(0xae24d0u,0x9870e8u,version,first,second,line);
    cm_fn_005e8290((const char *)0x9870e0u,(const char *)0xae24d0u,0);
    D(0xb67a34u)=0;
}
static s32 capped(s32 x) {return x<0x2616 ? x : 0x2616;}

__attribute__((target("no-sse3")))
u32 CM_THISCALL cm_fn_008d1130(u32 self,u32 offer,u32 flags,u32 output,s8 capacity)
{
    volatile struct {u32 prev,handler,state;} frame;
    /* Base corresponds to original ESP+4c after saving the four registers.
       List, packed temporary, date records and both report buffers stay adjacent. */
    u8 scratch[0x288];
    u32 work=(u32)scratch,person,mapped,club,base,ptr,node,owner;
    u32 rating_club,owner_club=0,area_a=0,area_b=0,area_c=0;
    u32 nation_a=0,nation_b=0,nation_c=0,other,other_nation,area,nation;
    u32 index,first,second,limit,value,version;
    s32 a=0,b=0,c=0,offset,threshold,count,remaining;
    s8 used=0,position,category,compact;
    u8 any=0,same_area=0,same_nation=0,same_owner=0,same_rating=0,add;
    long double x,y;
    frame.state=0xffffffffu;frame.handler=0x9640e2u;
    frame.prev=CM_FS(u32,0);CM_FS(u32,0)=(u32)&frame;
    index=D(offer+0xcu);ptr=D(0xae1050u);base=D(0xae23bcu);
    person=base+index*110u;
    mapped=cm_fn_004cf5a0(ptr,person);
    base=D(0xae23b4u);D(0xae1080u)=mapped;
    if(mapped && (B(mapped+0x35u)&0x3fu)!=5u) mapped=base+D(mapped+4u)*581u;
    else mapped=0;
    club=base+D(offer+8u)*581u;
    cm_fn_0068d020(work);frame.state=0;
    if((flags&2u) && mapped && cm_fn_007875e0(mapped)) {
        used=1;D(output)=mapped;
    }
    if((flags&1u) && cm_fn_007875e0(club)) {
        D(output+(u32)(s32)used*4u)=club;used=(s8)(used+1);
    }
    if(flags&4u) {
        ptr=cm_fn_004cf6d0(D(0xae1050u),person);
        if(ptr && (!club || D(cm_fn_004cf6d0(D(0xae1050u),person)+4u)!=D(club))) {
            ptr=cm_fn_00540a50(D(person+0x39u),work+0x14u,1);
            if(!ptr || D(work+0x14u))ptr=D(person+0x39u);
            D(output+(u32)(s32)used*4u)=ptr;used=(s8)(used+1);
        }
        ptr=D(offer);
        if(ptr) {
            value=D(ptr+2u);D(ptr+10u)=value;
            node=cm_fn_0068d1b0(D(offer));
            while(node) {
                index=D(node);ptr=D(D(self)+index*4u);category=(s8)B(ptr+0x2cu);
                if(category!=2 && category!=3 && category!=20 && category!=16 && category!=4 &&
                   (category!=15 || B(ptr+0x2eu)<3u)) {
                    if(index!=D(offer+0x20u)) {
                        value=D(ptr+8u);base=D(0xae23b4u);
                        if(cm_fn_007875e0(base+value*581u)) {
                        if(used<capacity) {
                            index=D(node);ptr=D(D(self)+index*4u);
                            base=D(0xae23b4u);value=D(ptr+8u);value=base+value*581u;
                            D(output+(u32)(s32)used*4u)=value;used=(s8)(used+1);
                        } else report(work+0x188u,work+0x88u,0x3936u);
                        }
                    }
                }
                node=cm_fn_0068d1b0(D(offer));
            }
        }
    }
    if(flags&8u) {
        if(D(person+0x39u) && cm_fn_004cf6d0(D(0xae1050u),person) &&
           (!club || D(cm_fn_004cf6d0(D(0xae1050u),person)+4u)!=D(club))) {
            D(output+(u32)(s32)used*4u)=D(person+0x39u);used=(s8)(used+1);
        }
        cm_fn_00842180(D(self+0x856u),person,work,1,1,1);
        value=D(work+2u);D(work+10u)=value;
        node=cm_fn_0068d1b0(work);
        while(node) {
            base=D(0xae23b4u);
            if(((flags&1u) || base+D(node+4u)*581u!=club) &&
               (value=D(node+4u),value!=D(club)) &&
               cm_fn_007875e0(base+value*581u)) {
                if(used<capacity) {
                    value=D(node+4u);base=D(0xae23b4u);value=base+value*581u;
                    D(output+(u32)(s32)used*4u)=value;used=(s8)(used+1);
                } else report(work+0x88u,work+0x188u,0x3959u);
            }
            node=cm_fn_0068d1b0(work);
        }
    }
    if(!(flags&0x10u) || ((flags&0x20u) && !B(offer+0x2du)))goto deduplicate;
    ptr=cm_fn_00849ce0(D(self+0x856u),club,person,0,0);
    value=cm_fn_0053b620(B(offer+0x11u));
    if(value && ptr && (B(ptr+0x24u)&0x1eu)<6u && (s32)D(offer+0x1bu)>=1000000)same_rating=1;
    ptr=D(person+0x39u);if(ptr)owner_club=D(ptr+0x57u);
    rating_club=D(club+0x57u);ptr=D(person+0x61u);
    if(ptr) {a=(s16)W(ptr+0xbu);b=(s16)W(ptr+9u);c=(s16)W(ptr+0xdu);}
    else {
        ptr=D(person+0x69u);
        if(ptr) {
            a=(s16)W(ptr+0xau);b=(s16)W(ptr+8u);c=(s16)W(ptr+0xcu);
            category=(s8)B(offer+0x11u);
            if(category==9 || category==8 || category==10) {
                x=(long double)a; x=x*(long double)Q(0x9689e0u);
                a=(s16)(u32)(long long)x; /* original __ftol CALL008d16be, AX later. */
                x=(long double)b; x=x*(long double)Q(0x9689e0u);
                b=(s16)(u32)(long long)x; /* original __ftol CALL008d16d6, AX later. */
                x=(long double)c; x=x*(long double)Q(0x9689e0u);
                c=(s16)(u32)(long long)x; /* original __ftol CALL008d16ee, AX later. */
            }
        }
    }
    if(!B(offer+0x2fu)) {
        value=(u32)cm_fn_008e7b50(offer,0,0,0,0,1);
        x=(long double)(s32)value; x=x*(long double)Q(0x96a610u);
        x=(long double)Q(0x96d200u)-x;
        offset=(s16)(u32)(long long)x; /* original __ftol CALL008d1726, AX. */
    } else offset=500;
    if(c>capped(offset+0x23f0))any=1;
    else if(c>capped(offset+0x206c)) {
        ptr=D(person+0x1au);if(ptr)area_a=D(ptr+0x71u);
        if(mapped) {ptr=D(mapped+0x53u);if(ptr)area_b=D(ptr+0x71u);}
        ptr=D(club+0x53u);if(ptr)area_c=D(ptr+0x71u);
        same_area=1;
    } else {
        threshold=offset+0x1c20;
        if(b>capped(threshold) || a>capped(threshold)) {
            same_nation=1;
            if(b>capped(threshold))nation_a=D(person+0x1au);
            if(a>capped(threshold) && mapped)nation_b=D(mapped+0x53u);
            nation_c=D(club+0x53u);
            if(nation_c!=nation_a && nation_c!=nation_b) {
                if(c<=capped(threshold)) {
                    nation_c=D(club+0x53u);
                    value=(u32)cm_fn_008e7b50(offer,0,0,0,0,1);
                    limit=(u32)((s32)(s8)B(nation_c+0x85u)-10)*500000u;
                    if((s32)value<(s32)limit) {
                        ptr=D(person+0x24u);
                        if(!ptr || B(person+0x22u)<=20u || (s16)W(ptr+0x80u)<=0x157c) nation_c=0;
                    }
                }
            } else nation_c=0;
        } else {
            ptr=D(person+0x61u);
            if(ptr) {
                y=cm_fn_007c1e30(D(0xb74350u),ptr,2,0x12u,6.5f);
                x=(long double)a*(long double)Q(0x96a680u);x=(long double)Q(0x970428u)-x;
                if(!(x>=y) && a>0xbb8 && (flags&0x80u)) {
                    same_nation=1;if(mapped)nation_b=D(mapped+0x53u);goto select;
                }
            }
            if(flags&0x40u) {
                if(owner_club && a>(s16)W(owner_club+0x69u)*350 && a>0x9c4)same_owner=1;
                else {
                    ptr=D(person+0x61u);
                    if(ptr) {
                        y=cm_fn_007c1e30(D(0xb74350u),ptr,2,0x12u,6.5f);
                        x=(long double)a*(long double)Q(0x96a680u);x=(long double)Q(0x96d230u)-x;
                        if(!(x>=y))same_owner=1;
                    }
                }
                if(rating_club) {
                    cm_fn_0054a110(work+0x28u);B((u32)&frame.state)=1;
                    cm_fn_0054a110(work+0x41u);B((u32)&frame.state)=2;
                    cm_fn_0054a110(work+0x49u);B((u32)&frame.state)=3;
                    cm_fn_0054a110(work+0x56u);B((u32)&frame.state)=4;
                    cm_fn_0054a110(work+0x5eu);
                    D(work+0x51u)=club;ptr=D(club+0x53u);D(work+0x32u)=ptr;
                    ptr=D(person+0x61u);B((u32)&frame.state)=5;
                    D(work+0x81u)=0;D(work+0x79u)=0;D(work+0x36u)=0;D(work+0x18u)=0;
                    value=cm_fn_005407e0(person,work+0x18u,ptr==0);
                    if((s16)value>(s16)W(rating_club+0x69u)*350 && a>0x5dc)same_rating=1;
                    B((u32)&frame.state)=0;
                }
            }
        }
    }
select:
    for(position=0;position<16;position=(s8)(position+1)) {
        if(used==capacity) {report(work+0x88u,work+0x188u,0x39f3u);break;}
        index=D(0xae2364u)-(u32)(s32)position-1u;
        base=D(0xae23bcu);ptr=base+index*110u;
        if(!B(ptr+0x5fu))continue;
        other=D(ptr+0x39u);if(!other)continue;
        if(!(flags&1u) && other==club)continue;
        if(!(flags&2u) && other==mapped)continue;
        nation=D(ptr+0x1au);other_nation=D(other+0x53u);
        area=nation ? D(nation+0x71u) : 0;
        owner=other_nation ? D(other_nation+0x71u) : 0;
        add=any;
        if(!add && same_area) {
            if(area && (area==area_a || area==area_b || area==area_c))add=1;
            else if(owner && (owner==area_a || owner==area_b || owner==area_c))add=1;
        } else if(!add && same_nation) {
            if(nation && (nation==nation_a || nation==nation_b || nation==nation_c))add=1;
            else if(other_nation && (other_nation==nation_a || other_nation==nation_b || other_nation==nation_c))add=1;
            else if(nation) {
                value=D(nation);first=D(0x9cf2e4u);
                if(value==first) {
                    second=D(0x9cf45cu);
                    if((nation_a && D(nation_a)==second) || (nation_b && D(nation_b)==second) || (nation_c && D(nation_c)==second))add=1;
                } else second=D(0x9cf45cu);
                if(!add && value==second && ((nation_a && D(nation_a)==first) || (nation_b && D(nation_b)==first) || (nation_c && D(nation_c)==first)))add=1;
            }
        } else if(!add && !same_area && !same_nation) {
            if(same_owner && D(other+0x57u)==owner_club)add=1;
            else if(same_rating && D(other+0x57u)==rating_club)add=1;
        }
        if(add) {D(output+(u32)(s32)used*4u)=other;used=(s8)(used+1);}
    }
deduplicate:
    count=used;
    for(position=1;position<count;position=(s8)(position+1)) {
        index=(u32)(s32)(s8)(position-1);
        if(D(output+index*4u)!=0 && position<used) {
            remaining=count-position;ptr=output+(u32)(s32)position*4u;
            do {value=D(output+index*4u);if(value==D(ptr))D(ptr)=0;ptr+=4u;}while(--remaining);
        }
    }
    compact=0;
    if(used>0) {
        ptr=output;remaining=count;
        do {value=D(ptr);if(value) {D(output+(u32)(s32)compact*4u)=value;compact=(s8)(compact+1);}ptr+=4u;}while(--remaining);
    }
    frame.state=0xffffffffu;version=cm_fn_0068d040(work);
    CM_FS(u32,0)=frame.prev;
    return (version&0xffffff00u)|(u8)compact;
}

#include "cmre.h"
/* CM0102 3.9.68, SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Preferred-image VA00527240, base00400000. Original callees remain boundaries. */
#define D(a) CM_GLOBAL(volatile u32,(u32)(a))
#define B(a) CM_GLOBAL(volatile u8,(u32)(a))
#define W(a) CM_GLOBAL(volatile s16,(u32)(a))
extern u32 CM_CDECL cm_fn_00539c60(u32);
extern u32 CM_CDECL cm_fn_0053a1b0(u32);
extern u32 CM_CDECL cm_fn_00540a50(u32,u32,u32);
extern void CM_THISCALL cm_fn_0054a130(u32,u32,u32,u32,u32);
extern void CM_CDECL cm_fn_005e8290(u32,u32,u32);
extern unsigned long long __attribute__((regparm(2))) CM_CDECL cm_fn_0090cfc0(u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,void *,void *);

/* The two buffers are adjacent in the original 0x218-byte allocation.
   Their contents and the direction word are deliberately uninitialized. */
static void error_527240(u8 *scratch,u32 line,u32 swapped)
{
    u8 *a=scratch+(swapped?256:0), *b=scratch+(swapped?0:256);
    u32 version;
    cm_fn_00944cff(0x009b5ac8,0,0,a,b);
    version=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,a,b,line);
    cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
}

u32 CM_CDECL cm_fn_00527240(void)
{
    u8 scratch[512];
    u32 direction,created=0,limit,index,record,owner,link,offset,row,phase;
    u32 slot,occupied,target,count,name,names,base,version,year,month,day;
    s32 computed;
    /* The signed word bounds make the quota product fit in s32. All address
       computations, record offsets, counters and id thresholds wrap at 32 bits. */
    static const u32 slots[3]={0x19f,0x1b3,0x1cf};
    static const u32 capacities[3]={5,7,3};
    static const u32 lines[3]={0x65f,0x6aa,0x6f5};
    limit=D(0x00ae2364);index=0;
    if ((s32)limit>0) do {
        base=D(0x00ae23bc);record=base+index*110u;
        if (B(record+0x3d)==8) {
            link=D(record+0x39);
            if (link) {
                slot=0;link+=0x19fu;
                do {if(D(link)==record)break;++slot;link+=4u;} while((s32)slot<5);
                if(slot==5) {
                    /* First error reverses both buffers: L+118 precedes L+18. */
                    error_527240(scratch,0x62f,1);
                    limit=D(0x00ae2364);D(0x00b67a34)=0;index=limit-1u;
                }
            }
        }
        ++index;
    } while((s32)index<(s32)limit);
    count=D(0x00ae235c);
    for(phase=0;phase<3;++phase) {
        row=0;offset=0;
        if((s32)count>0) do {
            base=D(0x00ae23b4);owner=base+offset;
            link=D(owner+0x53);
            if(link) {
                name=D(0x009cf2e4);names=D(link);
                if(names==name && W(owner+0x80)<=500)goto next_owner;
            }
            if(!owner) {
                error_527240(scratch,0x349f,0);
                count=D(0x00ae235c);D(0x00b67a34)=0;
            } else {
                name=D(0x00ae2350);
                name=count-name*2u;
                names=D(owner);
                if((s32)names>=(s32)name)goto next_owner;
            }
            link=D(owner+0x53);if(!link)goto next_owner;
            if(!B(link+0x11c))goto next_owner;
            name=cm_fn_00540a50(owner,(u32)&direction,1);
            if(name && direction)goto refreshed;
            slot=owner+slots[phase];occupied=0;
            for(index=capacities[phase];index;--index) {
                if(D(slot))++occupied;
                slot+=4u;
            }
            computed=((s32)W(owner+0x80)+500)*(s32)capacities[phase];
            computed/=10000;
            target=(u32)computed;
            if(computed<=1)target=1;
            else if(computed>=(s32)capacities[phase])target=capacities[phase];
            index=occupied++;
            if((s32)index>=(s32)target)goto refreshed;
            do {
                if((s32)created>=0x59d8) {error_527240(scratch,lines[phase],0);D(0x00b67a34)=0;return 0;}
                name=D(0x00ae2364);name-=created;
                base=D(0x00ae23bc);name-=17u;
                link=D(owner+0x53);record=base+name*110u;
                name=cm_fn_00539c60(link);D(record+4)=name;
                link=D(owner+0x53);name=cm_fn_0053a1b0(link);D(record+8)=name;
                name=D(0x009b59b4);
                if(name==0xffffffffu) {
                    names=D(0x00ae23a0);name=0;
                    if((s32)names>0) {
                        base=D(0x00ae23f8);slot=base;
                        do {
                            if(B(slot)==0) {
                                D(0x009b59b4)=name;
                                if(name!=0xffffffffu) {name=base+name*60u;goto store_name;}
                                break;
                            }
                            ++name;slot+=60u;
                        } while((s32)name<(s32)names);
                    }
                    name=(u32)cm_fn_0090cfc0(0,names,names);
                    base=D(0x00ae23f8);name=base+name*60u;
                } else {base=D(0x00ae23f8);name=base+name*60u;}
store_name:
                D(record+0xc)=name;
                link=D(owner+0x53);D(record+0x1a)=link;B(record+0x5f)=1;
                year=(u32)cm_fn_0090cfc0(0,0,26)+1940u;
                month=(u32)cm_fn_0090cfc0(0,0,11);
                day=(u32)cm_fn_0090cfc0(0,0,27)+1u;
                cm_fn_0054a130(record+0x10,day,month,year,0xffffffffu);
                index=0;slot=owner+slots[phase];
                do {if(!D(slot))break;++index;slot+=4u;} while((s32)index<(s32)capacities[phase]);
                if(index<capacities[phase]) {
                    D(owner+slots[phase]+index*4u)=record;D(record+0x39)=owner;B(record+0x3d)=(u8)(8u+phase);
                }
                ++created;index=occupied++;
            } while((s32)index<(s32)target);
refreshed:
            count=D(0x00ae235c);
next_owner:
            ++row;offset+=581u;
        } while((s32)row<(s32)count);
    }
    row=0;offset=0;
    if((s32)count>0) do {
        base=D(0x00ae23b4);owner=base+offset;link=D(owner+0x53);
        if(link) {
            name=D(0x009cf2e4);
            if(D(link)==name && W(owner+0x80)<=500)goto fourth_next;
        }
        if(!owner) {error_527240(scratch,0x349f,0);count=D(0x00ae235c);D(0x00b67a34)=0;}
        else {
            name=D(0x00ae2350);name=count-name*2u;names=D(owner);
            if((s32)names>=(s32)name)goto fourth_create_check;
        }
        base=D(0x00ae23b4);owner=base+offset;link=D(owner+0x53);
        if(link && D(owner+0x57) && B(link+0x11c) && W(owner+0x80)!=0)goto fourth_create_check;
        if(W(owner+0x80)<=7500)goto fourth_next;
fourth_create_check:
        if(D(owner+0xcf))goto fourth_next;
        if((s32)created>=0x59d8) {error_527240(scratch,0x734,0);D(0x00b67a34)=0;return 0;}
        link=D(owner+0x53);name=D(0x00ae2364);name-=created;name-=17u;
        base=D(0x00ae23bc);record=base+name*110u;
        name=cm_fn_00539c60(link);D(record+4)=name;
        name=cm_fn_0053a1b0(link);D(record+8)=name;
        name=D(0x009b59b4);
        if(name==0xffffffffu) {
            names=D(0x00ae23a0);name=0;
            if((s32)names>0) {
                base=D(0x00ae23f8);slot=base;
                do {
                    if(!B(slot)) {
                        D(0x009b59b4)=name;
                        if(name!=0xffffffffu) {name=base+name*60u;goto fourth_store_name;}
                        break;
                    }
                    ++name;slot+=60u;
                } while((s32)name<(s32)names);
            }
            name=(u32)cm_fn_0090cfc0(0,names,names);
        }
        base=D(0x00ae23f8);name=base+name*60u;
fourth_store_name:
        D(record+0xc)=name;
        B(record+0x5f)=1;B(record+0x3d)=5;D(record+0x39)=0;D(record+0x24)=0;B(record+0x28)=5;
        year=(u32)cm_fn_0090cfc0(0,0,26)+1940u;
        month=(u32)cm_fn_0090cfc0(0,0,11);
        day=(u32)cm_fn_0090cfc0(0,0,27)+1u;
        cm_fn_0054a130(record+0x10,day,month,year,0xffffffffu);
        count=D(0x00ae235c);++created;
fourth_next:
        ++row;offset+=581u;
    } while((s32)row<(s32)count);
    version=cm_fn_00944c9f(0x00de1f64,0x009b5b10,created);(void)version;
    return 1;
}

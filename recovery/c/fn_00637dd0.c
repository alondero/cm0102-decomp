#include "cmre.h"
/* Preferred-image VA00637dd0/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS0/full boolean; scratch3a0 scanL0/poolcountL4/selectedL8/selfLc,
   diagnosticsL10,L110/poolL210. Keep both grouping tables independent,
   counted pool prefix, signed WORD indices, global reloads and RNG call order. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
extern u32 CM_CDECL cm_fn_00945501(u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);
extern u32 CM_CDECL cm_fn_004afbd0(u32,u32,u32);
extern u32 CM_CDECL cm_fn_004afb40(u32,u32);

u32 CM_THISCALL cm_fn_00637dd0(u32 self)
{
    u8 scratch[0x3a0];
    u32 l=(u32)scratch,root=self,pool_count=0,selected=0,eax,limit,club;
    u32 pointer,id,other,first,last,index,slots,scan,value,capacity,candidate;
    u32 country,base,cursor,seen,version;
    D(l+0xc)=root;D(l+8)=0;D(root+0xf2)=26;D(l+4)=0;
    eax=cm_fn_00945501(4,26);D(root+0xee)=eax;
    if(eax==0){
        cm_fn_00944cff(0x009e9f4c,0,0,l+0x10,l+0x110);
        version=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,l+0x10,l+0x110,0x2af);
        cm_fn_005e8290(0x009870e0,0x00ae24d0,0);D(0x00b67a34)=0;
        return 0;
    }
    limit=D(0x00ae235c);club=D(0x00ae23b4);D(l)=0;
    if((s32)limit>0){
        do{
            pointer=D(club+0x53);
            if(pointer!=0){
                id=D(pointer);other=D(0x009cf2dc);
                if(id==other){
                    pointer=D(club+0x60);
                    if(pointer!=0 && (id=D(pointer))!=0){
                        if(id==D(0x009cf814)){first=0;last=6;}
                        else if(id==D(0x009cf818)){first=6;last=12;}
                        else if(id==D(0x009cf81c)){first=12;last=18;}
                        else if(id==D(0x009cf820)){first=18;last=22;}
                        else if(id==D(0x009cf824)){first=22;last=26;}
                        else goto next_club;
                        if((s16)(u16)first<(s16)(u16)last){
                            slots=D(root+0xee);index=first;
                            do{
                                if(D(slots+(u32)(s32)(s16)(u16)index*4u)==0){
                                    pool_count=D(l+4);++selected;
                                    D(slots+(u32)(s32)(s16)(u16)index*4u)=club;
                                    goto next_club;
                                }
                                ++index;
                            }while((s16)(u16)index<(s16)(u16)last);
                            pool_count=D(l+4);
                        }
                    }else{
                        pointer=D(club+0x57);
                        if(pointer!=0 && (s16)(u16)pool_count<100){
                            index=(u32)(s32)(s16)(u16)pool_count;++pool_count;
                            D(l+0x210+index*4u)=club;D(l+4)=pool_count;
                        }
                    }
                }
            }
next_club:
            scan=D(l);club+=581u;++scan;D(l)=scan;
            limit=D(0x00ae235c);
        }while((s32)(s16)(u16)scan<(s32)limit);
        D(l+8)=selected;
    }
    cm_fn_004afbd0(l+0x210,pool_count,20);index=0;
    if((s16)W(l+4)>0){
        do{
            if((s32)selected>=(s32)D(root+0xf2)) break;
            club=D(l+0x210+(u32)(s32)(s16)(u16)index*4u);
            pointer=D(club+0x57);id=D(pointer);other=D(0x009cf854);
            if(id==other){first=0;last=6;}
            else if(id==D(0x009cf848)){first=12;last=18;}
            else if(id==D(0x009cf850)){first=18;last=22;}
            else if(id==D(0x009cf84c)){first=22;last=26;}
            else{first=6;last=12;}
            if((s16)(u16)first<(s16)(u16)last){
                slots=D(root+0xee);scan=first;
                do{
                    if(D(slots+(u32)(s32)(s16)(u16)scan*4u)==0){
                        selected=D(l+8);++selected;
                        D(slots+(u32)(s32)(s16)(u16)scan*4u)=club;
                        D(l+8)=selected;goto next_pool;
                    }
                    ++scan;
                }while((s16)(u16)scan<(s16)(u16)last);
                selected=D(l+8);
            }
next_pool:
            ++index;
        }while((s16)(u16)index<(s16)W(l+4));
    }
    slots=D(root+0xee);cm_fn_004afb40(slots,6);
    slots=D(root+0xee);cm_fn_004afb40(slots+0x18,6);
    slots=D(root+0xee);cm_fn_004afb40(slots+0x30,6);
    capacity=D(root+0xf2);
    if((s32)selected<(s32)capacity){
        index=0;D(l)=0;
        if((s32)capacity>0){
            do{
                slots=D(root+0xee);
                if(D(slots+index*4u)==0){
                    limit=D(0x00ae235c);candidate=D(0x00ae23b4);scan=0;
                    if((s32)limit>0){
                        for(;;){
                            pointer=D(candidate+0x57);
                            if(pointer!=0){
                                country=D(0x009cf2dc);base=D(0x00ae23a8);
                                value=base+country*290u;other=D(candidate+0x53);
                                if(other==value){
                                    seen=0;
                                    if((s32)capacity>0){
                                        cursor=slots;
                                        while(D(cursor)!=candidate){
                                            ++seen;cursor+=4;
                                            if((s32)seen>=(s32)capacity) goto insert;
                                        }
                                    }else{
insert:
                                        D(slots+index*4u)=candidate;break;
                                    }
                                }
                            }
                            limit=D(0x00ae235c);++scan;candidate+=581u;
                            if((s32)scan>=(s32)limit) break;
                        }
                    }
                    root=D(l+0xc);
                }
                scan=D(l);capacity=D(root+0xf2);++scan;D(l)=scan;
                index=(u32)(s32)(s16)(u16)scan;
            }while((s32)index<(s32)capacity);
        }
    }
    return 1;
}

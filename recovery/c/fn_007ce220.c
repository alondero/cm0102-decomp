#include "cmre.h"
/* Preferred-image VA007ce220/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS0/fullEAX -1 or boolean; scratch208 indexL0/directionL4/diagL8,L108.
   Preserve four separate scans, cached inter-scan limits, signed WORD scan
   indices, all output-base reloads and RNG18/final signed-WORD exchange.
   540a50 fully writes direction before a nonzero return at original
   VAs005412a7/0054187c (prior proof); do not initialize L4. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00540a50(u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_007ce220(u32 self)
{
    u8 scratch[0x208];
    u32 l=(u32)scratch,eax,version,limit,club,pointer,tag,count=0,offset=0;
    u32 output,index,other,value;
    W(self+0x3e)=164;
    eax=(u32)cm_fn_00944e46(0x3d8);D(self+0xa7)=eax;
    if(eax==0){
        cm_fn_00944cff(0x00a786f0,0,0,l+0x108,l+8);
        version=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,l+0x108,l+8,0x159);
        cm_fn_005e8290(0x009870e0,0x00ae24d0,0);D(0x00b67a34)=0;
        return 0xffffffffu;
    }
#define APPEND() do { \
    output=D(self+0xa7);++count;D(output+offset)=club; \
    output=D(self+0xa7);B(output+offset+4)=0; \
    output=D(self+0xa7);B(output+offset+5)=0;offset+=6u; \
} while(0)
    limit=D(0x00ae235c);club=D(0x00ae23b4);D(l)=0;
    if((s32)limit>0){
        do{
            pointer=D(club+0x57);
            if(pointer!=0){
                tag=D(pointer);
                if((tag==D(0x009cf61c) || tag==D(0x009cf618) ||
                    tag==D(0x009cf614) || tag==D(0x009cf7c0)) && (s32)offset<0x300){
                    eax=cm_fn_00540a50(club,l+4,1);
                    if(eax==0 || D(l+4)!=0) APPEND();
                }
            }
            index=D(l);club+=581u;++index;D(l)=index;limit=D(0x00ae235c);
        }while((s32)(s16)(u16)index<(s32)limit);
    }
    club=D(0x00ae23b4);D(l)=0;
    if((s32)limit>0){
        offset=count*6u;
        do{
            pointer=D(club+0x57);
            if(pointer!=0){
                tag=D(pointer);
                if(tag==D(0x009cf620)) goto second_candidate;
                if(tag==D(0x009cf764)){
                    pointer=D(club+0x53);
                    if(pointer!=0){
                        tag=D(pointer);
                        if(tag==D(0x009cf434)){
second_candidate:
                            if((s32)offset<0x300){
                                eax=cm_fn_00540a50(club,l+4,1);
                                if(eax==0 || D(l+4)!=0) APPEND();
                            }
                        }
                    }
                }
            }
            index=D(l);club+=581u;++index;D(l)=index;limit=D(0x00ae235c);
        }while((s32)(s16)(u16)index<(s32)limit);
    }
    club=D(0x00ae23b4);index=0;
    if((s32)limit>0){
        offset=count*6u;
        do{
            pointer=D(club+0x57);
            if(pointer!=0){
                tag=D(pointer);other=D(0x009cf610);
                if(tag==other && (s32)count<(s32)(s16)W(self+0x3e)){
                    APPEND();
                    if((s32)count==(s32)(s16)W(self+0x3e)) break;
                }
            }
            limit=D(0x00ae235c);++index;club+=581u;
        }while((s32)(s16)(u16)index<(s32)limit);
    }
    limit=D(0x00ae235c);club=D(0x00ae23b4);index=0;
    if((s32)limit>0){
        offset=count*6u;
        do{
            pointer=D(club+0x57);
            if(pointer!=0){
                tag=D(pointer);other=D(0x009cf60c);
                if(tag==other && (s32)count<(s32)(s16)W(self+0x3e)){
                    APPEND();
                    if((s32)count==(s32)(s16)W(self+0x3e)) break;
                }
            }
            limit=D(0x00ae235c);++index;club+=581u;
        }while((s32)(s16)(u16)index<(s32)limit);
    }
#undef APPEND
    eax=cm_fn_0090cfc0(18);eax=count-eax-1u;
    output=D(self+0xa7);offset=(u32)(s32)(s16)(u16)eax*6u;
    other=count*6u-6u;value=D(output+other);pointer=output+other;
    tag=D(output+offset);D(pointer)=tag;
    output=D(self+0xa7);D(output+offset)=value;
    return ((s32)count==(s32)(s16)W(self+0x3e));
}

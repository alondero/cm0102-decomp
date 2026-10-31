#include "cmre.h"
/* Preferred-image VA00902b90/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS0/full -1,0,1; scratch200 diagnosticsL0,L100. Retain signed BYTE
   halves, signed WORD scan/count wraps, six-byte records and late reloads.
   Output record byte5 is unwritten; only the input table is calloc-filled. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern u32 CM_CDECL cm_fn_00945501(u32,u32);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL __attribute__((regparm(1))) cm_fn_00945333(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_00902b90(u32 self)
{
    u8 scratch[0x200];
    u32 l=(u32)scratch,eax,count=0,scan=0,club,limit,base,id,other,offset;
    u32 pointer,half,adjusted,capacity,remaining,first,second,line,version;
    u8 byte;
    W(self+0x3e)=0x91;eax=cm_fn_00945501(6,0xa1);D(self+0xa7)=eax;
    if(eax==0){first=l+0x100;second=l;line=0x1bf;goto error;}
    B(self+0xb2)=0;limit=D(0x00ae235c);club=D(0x00ae23b4);
    if((s32)limit>0){
        do{
            id=D(club+0x1db);other=D(0x009cf6f0);
            if(id==other){
                base=D(self+0xa7);offset=(u32)(s32)(s16)(u16)count*6u;++count;
                D(base+offset)=club;
                base=D(self+0xa7);byte=B(club+0x1df);B(base+offset+4)=byte;
                base=D(self+0xa7);B(base+offset+5)=0;
                byte=B(club+0x1df);
                if((s8)byte>=10) B(self+0xb2)=(u8)(B(self+0xb2)+1u);
            }
            limit=D(0x00ae235c);++scan;club+=581u;
        }while((s32)(s16)(u16)scan<(s32)limit);
    }
    half=(u32)((s32)(s8)B(self+0xb2)/2);
    adjusted=(u32)(s32)(s16)(u16)count-half;
    if((s32)adjusted<118){
        other=D(0x00ae2350);club=D(0x00ae23b4);scan=0;
        limit=D(0x00ae235c)-other*2u;
        if((s32)limit>0){
            do{
                half=(u32)((s32)(s8)B(self+0xb2)/2);
                adjusted=(u32)(s32)(s16)(u16)count-half;
                if((s32)adjusted>=118) break;
                pointer=D(club+0x53);
                if(pointer!=0 && B(pointer+0x11c)==0 &&
                   (s16)W(pointer+0x8e)<2000 && D(club+0x1db)==0xffffffffu){
                    base=D(self+0xa7);offset=(u32)(s32)(s16)(u16)count*6u;++count;
                    D(base+offset)=club;
                    base=D(self+0xa7);B(base+offset+4)=3;
                    base=D(self+0xa7);B(base+offset+5)=0;
                    pointer=D(self+4);id=D(pointer);D(club+0x1db)=id;
                }
                other=D(0x00ae2350);++scan;club+=581u;
                limit=D(0x00ae235c)-other*2u;
            }while((s32)(s16)(u16)scan<(s32)limit);
        }
    }
    half=(u32)((s32)(s8)B(self+0xb2)/2);
    adjusted=(u32)(s32)(s16)(u16)count-half;
    while((s32)adjusted>118){
        offset=(u32)(s32)(s16)(u16)count*6u;base=D(self+0xa7);--count;
        D(base+offset-6u)=0;
        half=(u32)((s32)(s8)B(self+0xb2)/2);
        adjusted=(u32)(s32)(s16)(u16)count-half;
    }
    eax=D(self+0xa7);remaining=(u32)(s32)(s16)(u16)count;
    cm_fn_00945333(eax,eax,remaining,6,0x00522190);
    byte=B(self+0xb2);
    if((s8)byte<0 || (byte&1u)!=0) return 0;
    W(self+0x36)=0;capacity=count+27u;W(self+0x38)=(u16)capacity;
    eax=(u32)cm_fn_00944e46((u32)(s32)(s16)(u16)capacity*6u);D(self+0x14)=eax;
    if(eax==0){first=l;second=l+0x100;line=0x21a;goto error;}
    if((s16)(u16)count>0){
        offset=0;
        do{
            base=D(self+0xa7);pointer=base+offset;
            if(B(pointer+4)==1){
                id=D(pointer);
                if(id!=0){
                    other=(u32)(s32)(s16)W(self+0x36)*6u;base=D(self+0x14);
                    D(base+other)=id;
                    base=D(self+0x14);other=(u32)(s32)(s16)W(self+0x36)*6u;
                    B(base+other+4)=3;W(self+0x36)=(u16)(W(self+0x36)+1u);
                }
            }
            offset+=6;--remaining;
        }while(remaining!=0);
    }
    W(self+0x3e)=(u16)capacity;
    return 1;
error:
    cm_fn_00944cff(0x00aa19e0,0,0,first,second);
    version=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,first,second,line);
    cm_fn_005e8290(0x009870e0,0x00ae24d0,0);D(0x00b67a34)=0;
    return line==0x1bf?0xffffffffu:0;
}

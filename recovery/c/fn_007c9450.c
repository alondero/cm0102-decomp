#include "cmre.h"

/* Preferred-image VA 007c9450, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Ranking BYTEs and all table reloads follow the original. Original calloc,
   club initialization and financial helper remain explicit boundaries. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_CDECL cm_fn_00945501(u32,u32);
extern u32 CM_THISCALL cm_fn_00687430(u32,u32,u32,u32,u32);
extern void CM_THISCALL cm_fn_00683010(u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_007c9450(u32 self)
{
    u8 scratch[0x210];
    u32 local=(u32)scratch,result,count,club,scan,country,key,wanted;
    u32 division,competition,index,table,value,line,name,ext,version;
    u8 rank;
    B(local)=1;
    W(self+0x3e)=8;
    B(local+1)=4; B(local+2)=5; B(local+3)=8;
    B(local+4)=9; B(local+5)=12; B(local+6)=13; B(local+7)=16;
    B(local+8)=2; B(local+9)=3; B(local+10)=6; B(local+11)=7;
    B(local+12)=10; B(local+13)=11; B(local+14)=14; B(local+15)=15;
    result=cm_fn_00944e46(0x1d8);
    D(self+0xb1)=result;
    if (result==0) {
        name=local+0x110; ext=local+0x10; line=0x13b;
        goto error;
    }
    result=D(self+0xee);
    if (result!=0) cm_fn_009452ca(result);
    result=cm_fn_00945501(16,4);
    D(self+0xee)=result;
    if (result==0) {
        name=local+0x10; ext=local+0x110; line=0x146;
        goto error;
    }
    count=D(0x00ae235c);
    club=D(0x00ae23b4);
    scan=0;
    if ((s32)count>0) {
        do {
            country=D(club+0x57);
            if (country!=0) {
                key=D(country);
                wanted=D(0x009cf858);
                if (key==wanted) {
                    division=D(club+0x5b);
                    competition=D(self+4);
                    rank=B(club+0x5f);
                    if (division!=competition) {
                        if (rank==1 || rank==2) rank=(u8)(rank+14);
                        else if (rank==3) rank=14;
                        else if (rank==4) rank=13;
                    }
                    for (index=0;index<16;++index) {
                        if (B(local+index)==rank) {
                            table=D(self+0xee);
                            if (D(table+index*4)==0) break;
                        }
                    }
                    if (index==16) {
                        table=D(self+0xee);
                        for (index=0;index<16;++index)
                            if (D(table+index*4)==0) break;
                    }
                    if (index<16) {
                        table=D(self+0xee);
                        D(table+index*4)=club;
                    }
                }
            }
            count=D(0x00ae235c);
            ++scan;
            club+=0x245;
        } while ((s32)(s16)scan<(s32)count);
    }
    index=0;
    if ((s16)W(self+0x3e)>0) {
        do {
            table=D(self+0xee);
            value=D(table+(u32)(s32)(s16)index*4);
            cm_fn_00687430(self,index,value,0,0);
            ++index;
        } while ((s16)index<(s16)W(self+0x3e));
    }
    cm_fn_00683010(self,0x7a120,0);
    return 1;
error:
    cm_fn_00944cff(0x00a78598,0,0,name,ext);
    version=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,name,ext,line);
    cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
    D(0x00b67a34)=0;
    return 0;
}

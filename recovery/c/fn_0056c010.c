#include "cmre.h"

/* Preferred-image VA 0056c010, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The per-stage award flag is cached before the participant loop, while its
   BYTE accumulator advances after each call. DWORD flag loads retain the
   unwritten upper bytes; original 4a2540 consumes only its low BYTE. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_009452ca(u32);
extern void CM_CDECL cm_fn_00945333(u32,u32,u32,u32);
extern void CM_THISCALL cm_fn_004a2540(u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_0056c010(u32 self)
{
    u8 scratch[0x210];
    u32 local=(u32)scratch, clubs, index, table, other, record, value;
    u32 count, owner, first, second, first_flag, second_flag;
    u32 award_index, stage, offset, participants, raw_flag, name, ext, line;
    u32 version, result;
    u8 cycle, flag;
    u16 score;
    if (D(self+8)==0) {
        name=local+0x110; ext=local+0x10; line=0x3bb;
        goto error;
    }
    count=(u32)(s32)(s16)W(self+0x3e);
    clubs=cm_fn_00944e46(count<<3);
    D(local+4)=clubs;
    if (clubs==0) {
        name=local+0x10; ext=local+0x110; line=0x3c3;
        goto error;
    }
    index=0;
    if ((s16)W(self+0x3e)>0) {
        do {
            table=D(self+0xa7);
            record=(u32)(s32)(s16)index;
            offset=record*6;
            value=D(table+offset);
            D(clubs+record*4)=value;
            table=D(self+0xc);
            count=(u32)(s32)(s16)W(self+0x3e);
            other=D(table);
            count+=record;
            ++index;
            table=D(other+0xa7);
            value=D(table+offset);
            D(clubs+count*4)=value;
        } while ((s16)index<(s16)W(self+0x3e));
    }
    count=(u32)(s32)(s16)W(self+0x3e);
    cm_fn_00945333(clubs,count,4,0x004b07d0);
    count=(u32)(s32)(s16)W(self+0x3e);
    cm_fn_00945333(clubs+count*4,count,4,0x004b07d0);
    count=(u32)(s32)(s16)W(self+0x3e);
    first=D(clubs);
    second=D(clubs+count*4);
    score=W(first+0x80);
    first_flag=(s16)score>(s16)W(second+0x80)?1:2;
    second_flag=first_flag==1?2:1;
    owner=D(self+8);
    cm_fn_004a2540(owner,first,first_flag);
    count=(u32)(s32)(s16)W(self+0x3e);
    value=D(clubs+count*4);
    owner=D(self+8);
    cm_fn_004a2540(owner,value,second_flag);
    cycle=0;
    B(local+3)=cycle;
    do {
        if (cycle==0) award_index=1;
        else award_index=(u16)(W(self+0x3e)+1);
        stage=(u16)(W(self+0x3c)-1);
        B(local+2)=3;
        if ((s16)stage>=0) {
            stage=(u32)(s32)(s16)stage;
            offset=stage*0x68;
            D(local+8)=stage+1;
            do {
                table=D(self+0xa3);
                flag=B(local+2);
                participants=0;
                B(local+0xc)=flag;
                if ((s16)W(table+offset+0x1a)>0) {
                    do {
                        table=D(local+4);
                        index=(u32)(s32)(s16)award_index;
                        ++award_index;
                        value=D(table+index*4);
                        raw_flag=D(local+0xc);
                        owner=D(self+8);
                        cm_fn_004a2540(owner,value,raw_flag);
                        flag=B(local+2);
                        flag=(u8)(flag+2);
                        ++participants;
                        B(local+2)=flag;
                        table=D(self+0xa3);
                    } while ((s16)participants<(s16)W(table+offset+0x1a));
                }
                stage=D(local+8);
                offset-=0x68;
                --stage;
                D(local+8)=stage;
            } while (stage!=0);
            clubs=D(local+4);
            cycle=B(local+3);
        }
        cycle=(u8)(cycle+1);
        B(local+3)=cycle;
    } while ((s8)cycle<2);
    return cm_fn_009452ca(clubs);
error:
    cm_fn_00944cff(0x009c4180,0,0,name,ext);
    version=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,name,ext,line);
    result=cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
    D(0x00b67a34)=0;
    return result;
}

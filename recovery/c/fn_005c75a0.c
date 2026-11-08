#include "cmre.h"
/* Preferred-image VA 005c75a0, base 00400000; CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   B denotes original ESP after the four saved registers. Scratch B+10..457
   remains one allocation; B+458/45c/460 is the separately linked SEH frame.
   Date helpers consume only the low WORD of their count (54c930/54c5fd).
   Keep original helpers and all three original jump-table reads explicit. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B8(a) CM_GLOBAL(volatile u8,(a))
typedef struct {u32 prev,handler,state;} frame_5c75a0;
extern u32 CM_THISCALL cm_fn_0054c8e0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0054c5b0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00417c10(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0054a110(u32);
extern u32 CM_STDCALL cm_fn_005c6b70(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern u32 CM_THISCALL cm_fn_0068d1f0(u32,u32);
extern u32 CM_THISCALL cm_fn_005ae870(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

static __attribute__((always_inline)) inline u32 mode_5c75a0(u32 value,u32 table,u32 first,u32 second)
{
    u32 target,version;
    value=(u32)(s32)(s8)value;
    if (value>11) goto diagnostic;
    target=D(table+value*4); (void)target;
    switch (value) {
    case 1: case 10: return 2;
    case 2: return 3;
    case 0: case 3: case 4: case 11: return 1;
    default: break;
    }
diagnostic:
    cm_fn_00944cff(0x009cd0a0,0,0,first,second);
    version=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,first,second,0xf18);
    cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
    D(0x00b67a34)=0;
    return 1;
}

u8 CM_THISCALL cm_fn_005c75a0(u32 self,volatile u32 club_arg,volatile u32 date_arg,
    volatile u32 output1_arg,volatile u32 output2_arg,volatile u32 output3_arg,
    volatile u32 table_arg,volatile u32 count_arg,volatile u32 mode_arg,volatile u32 retry_arg)
{
    volatile frame_5c75a0 frame;
    u8 scratch[0x448];
    u32 b=(u32)scratch-0x10,date,club,row,offset,value,metadata,index,days,other,node,list;
    u32 year,lower,upper,source,first,result;
    s8 iterator;
    frame.state=0xffffffffu;
    frame.handler=0x0095a22f;
    frame.prev=CM_FS(volatile u32,0);
    CM_FS(volatile u32,0)=(u32)&frame;
    /* Entry loads precede all three output stores. */
    lower=output1_arg; upper=output3_arg; date=date_arg; value=output2_arg;
    D(lower)=0;
    iterator=0;
    D(value)=0;
    D(b+0x38)=self;
    D(upper)=0;
    if ((s16)count_arg>0) {
        value=D(date); metadata=D(date+4);
        D(b+0x20)=value; year=W(b+0x22); D(b+0x24)=metadata;
        do {
            source=table_arg; index=(u32)(s32)iterator;
            if (year==W(source+index*8+2) && (u16)value==W(source+index*8)) {
                result=4; goto done;
            }
            iterator=(s8)((u8)iterator+1);
        } while ((s16)iterator<(s16)count_arg);
    }
    club=club_arg; index=0; D(b+0x28)=0;
row_first:
    offset=(u32)(s32)(s16)index*105u;
    value=D(self+offset+0xb1);
    if (value!=0) {
        iterator=0;
        do {
            if (D(self+offset+(u32)(s32)iterator*4+0xc6)==club) {
                days=mode_5c75a0(B8(self+offset+0xbd),0x005c81cc,b+0x58,b+0x158);
                value=D(self+offset+0xb5); metadata=D(self+offset+0xb9);
                D(b+0x18)=value; D(b+0x1c)=metadata; frame.state=0;
                D(b+0x20)=value; D(b+0x24)=metadata;
                B8((u32)&frame.state)=1;
                value=cm_fn_0054c8e0(b+0x18,b+0x40,1);
                cm_fn_00417c10(b+0x18,b+0x10,value);
                value=cm_fn_0054c5b0(b+0x20,b+0x50,days);
                cm_fn_00417c10(b+0x20,b+0x48,value);
                year=W(date+2); lower=W(b+0x1a);
                if (lower<year || (lower==year && (s16)W(b+0x18)<(s16)W(date))) {
                    upper=W(b+0x22);
                    if (upper>year || (upper==year && (s16)W(b+0x20)>(s16)W(date))) {
                        offset=(u32)(s32)(s16)D(b+0x28)*105u;
                        row=self+offset;
                        D(output1_arg)=row+0xb1;
                        value=D(row+0x116);
                        result=(value==0)+2; goto done;
                    }
                }
                index=D(b+0x28); club=club_arg; frame.state=0xffffffffu;
                break;
            }
            iterator=(s8)((u8)iterator+1);
        } while (iterator<4);
    }
    ++index; D(b+0x28)=index;
    if ((s16)index<127) goto row_first;
    index=0; D(b+0x28)=0;
row_second:
    offset=(u32)(s32)(s16)index*105u;
    value=D(self+offset+0xb1);
    if (value!=0) {
        iterator=0;
        do {
            if (D(self+offset+(u32)(s32)iterator*4+0xc6)==club) {
                days=mode_5c75a0(B8(self+offset+0xbd),0x005c81fc,b+0x158,b+0x58);
                other=mode_5c75a0(mode_arg,0x005c822c,b+0x358,b+0x258);
                source=self+offset+0xb5;
                cm_fn_0054c8e0(source,b+0x20,other+1);
                frame.state=2;
                cm_fn_0054c5b0(source,b+0x18,days+1);
                year=W(date+2); lower=W(b+0x22);
                if (lower<year || (lower==year && (s16)W(b+0x20)<(s16)W(date))) {
                    upper=W(b+0x1a);
                    if (upper>year || (upper==year && (s16)W(b+0x18)>(s16)W(date))) {
                        result=2; goto done;
                    }
                }
                index=D(b+0x28); club=club_arg; frame.state=0xffffffffu;
                break;
            }
            iterator=(s8)((u8)iterator+1);
        } while (iterator<4);
    }
    ++index; D(b+0x28)=index;
    if ((s16)index<127) goto row_second;
    list=D(self+0x34c8);
    if (list!=0) {
        value=D(list+2); D(list+10)=value;
        node=cm_fn_0068d1b0(D(self+0x34c8));
        while (node!=0) {
            value=D(node+0x10);
            if (value!=0) {
                other=D(node+0x14);
                if (other!=0 && (value==club || other==club)) {
                    if (value==club) club=other; else club=value;
                    value=D(node); metadata=D(node+4);
                    D(b+0x28)=value; D(b+0x2c)=metadata; frame.state=3;
                    D(b+0x18)=value; D(b+0x1c)=metadata;
                    B8((u32)&frame.state)=4;
                    value=cm_fn_0054c8e0(b+0x28,b+0x48,1);
                    cm_fn_00417c10(b+0x28,b+0x50,value);
                    value=cm_fn_0054c5b0(b+0x18,b+0x40,1);
                    cm_fn_00417c10(b+0x18,b+0x30,value);
                    value=D(date); metadata=D(date+4);
                    D(b+0x20)=value; year=W(b+0x22); D(b+0x24)=metadata;
                    lower=W(b+0x2a);
                    if (year>lower || (year==lower && (s16)value>=(s16)W(b+0x28))) {
                        value=D(date); metadata=D(date+4); upper=W(b+0x1a);
                        D(b+0x10)=value; D(b+0x14)=metadata; year=W(b+0x12);
                        if (year<upper || (year==upper && (s16)value<=(s16)W(b+0x18))) {
                            value=D(node); metadata=D(node+4);
                            D(b+0x10)=value; D(b+0x14)=metadata;
                            year=W(b+0x12);
                            if (year==W(date+2) && (u16)value==W(date)) {
                                self=D(b+0x38);
                                value=cm_fn_005c6b70(date,club,0);
                                if (value==0) {
                                    cm_fn_0068d1f0(D(self+0x34c8),node);
                                    result=cm_fn_005c75a0(D(0x00ae2c74),club_arg,date_arg,
                                        output1_arg,output2_arg,output3_arg,table_arg,count_arg,mode_arg,1);
                                    goto done;
                                }
                                lower=W(b+0x2a); upper=W(b+0x1a);
                            }
                            year=W(date+2);
                            if (lower==year && W(b+0x28)==W(date)) {result=2;goto done;}
                            if (upper==year && W(b+0x18)==W(date)) {result=2;goto done;}
                            D(output2_arg)=node; result=5;goto done;
                        }
                    }
                    self=D(b+0x38); club=club_arg; frame.state=0xffffffffu;
                }
            }
            node=cm_fn_0068d1b0(D(self+0x34c8));
        }
    }
    list=D(self+0x34cc);
    if (list!=0) {
        value=D(list+2); D(list+10)=value;
        node=cm_fn_0068d1b0(D(self+0x34cc));
        while (node!=0) {
            if (D(node)==club) {
                cm_fn_0054c8e0(date,b+0x10,1);
                frame.state=5;
                cm_fn_0054c5b0(date,b+0x20,1);
                value=D(b+0x10); metadata=D(b+0x14);
                D(b+0x18)=value; D(b+0x1c)=metadata;
                B8((u32)&frame.state)=7;
                for (;;) {
                    year=W(b+0x22); lower=W(b+0x1a);
                    if (year<lower || (year==lower && (s16)W(b+0x20)<(s16)W(b+0x18))) break;
                    days=B8(node+8); iterator=0;
                    if ((s8)days>0) do {
                        row=node+(u32)(s32)iterator*25;
                        if (D(row+0x15)!=0) {
                            value=D(row+9); metadata=D(row+0xd);
                            D(b+0x28)=value; D(b+0x2c)=metadata;
                            if (W(b+0x2a)==lower && (u16)value==W(b+0x18)) {
                                value=D(date); metadata=D(date+4);
                                D(b+0x10)=value; D(b+0x14)=metadata;
                                if (W(b+0x12)!=lower || (u16)value!=W(b+0x18)) {result=2;goto done;}
                                D(output3_arg)=node; result=6;goto done;
                            }
                        }
                        iterator=(s8)((u8)iterator+1);
                    } while (iterator<(s8)days);
                    value=cm_fn_0054c5b0(b+0x18,b+0x30,1);
                    cm_fn_00417c10(b+0x18,b+0x48,value);
                }
                self=D(b+0x38); club=club_arg; frame.state=0xffffffffu;
            }
            node=cm_fn_0068d1b0(D(self+0x34cc));
        }
    }
    days=mode_arg;
    if ((u8)days!=11) {
        club=club_arg;
        value=cm_fn_005c6b70(date,club,days);
        if (value==0) {result=2;goto done;}
        if (retry_arg!=0 && ((u8)days==1 || (u8)days==2 || (u8)days==0)) {
            year=W(date+2); lower=W(date); value=D(club+0x69);
            value=cm_fn_005ae870(D(0x00ae2a58),value,lower,year,0xffffffffu);
            if (value!=0) {result=2;goto done;}
        }
        metadata=D(0x00ae2c94); first=D(0x00ae2c90);
        D(b+0x14)=metadata; D(b+0x18)=first;
        value=cm_fn_0054c8e0(date,b+0x30,7);
        year=W(value+2); lower=W(b+0x12);
        if (lower>year || (lower==year && (s16)first>(s16)W(value))) result=2;
        else result=1;
        goto done;
    }
    if ((s16)count_arg==0) {
        club=club_arg; D(output1_arg)=0;
        value=cm_fn_005c6b70(date,club,0);
        if (value==0) {result=2;goto done;}
        first=D(0x00ae2c90); metadata=D(0x00ae2c94);
        D(b+0x48)=first; D(b+0x4c)=metadata;
        value=cm_fn_0054c8e0(date,b+0x48,7);
        year=W(value+2); lower=W(b+0x42);
        if (lower>year || (lower==year && (s16)first>(s16)W(value))) result=2;
        else result=1;
        goto done;
    }
    cm_fn_0054a110(b+0x20); frame.state=8;
    cm_fn_0054a110(b+0x38);
    source=table_arg; year=W(date+2);
    value=D(source); metadata=D(source+4);
    D(b+0x10)=value; D(b+0x14)=metadata; lower=W(b+0x12);
    B8((u32)&frame.state)=9;
    if (lower>year || (lower==year && (s16)value>(s16)W(date))) {
        cm_fn_00417c10(b+0x20,b+0x30,date);
        value=cm_fn_0054c8e0(source,b+0x30,1);
        cm_fn_00417c10(b+0x38,b+0x48,value);
    } else {
        index=(u32)(s32)(s16)count_arg;
        metadata=D(table_arg+index*8-4); D(b+0x14)=metadata;
        source=table_arg+index*8-8; value=D(source); D(b+0x10)=value;
        lower=W(b+0x12);
        if (lower>year || (lower==year && (s16)value>=(s16)W(date))) goto table_adjacent;
        value=cm_fn_0054c5b0(source,b+0x30,1);
        cm_fn_00417c10(b+0x20,b+0x48,value);
        value=cm_fn_0054c5b0(date,b+0x30,1);
        cm_fn_00417c10(b+0x38,b+0x48,value);
    }
    value=D(b+0x20); metadata=D(b+0x24);
    D(b+0x18)=value; D(b+0x1c)=metadata;
    club=club_arg; B8((u32)&frame.state)=10;
    for (;;) {
        upper=W(b+0x3a); lower=W(b+0x1a);
        if (upper<lower || (upper==lower && (s16)W(b+0x38)<=(s16)W(b+0x18))) break;
        value=cm_fn_005c6b70(b+0x18,club,0);
        if (value==0) {result=2;goto done;}
        value=cm_fn_0054c5b0(b+0x18,b+0x30,1);
        cm_fn_00417c10(b+0x18,b+0x48,value);
    }
    B8((u32)&frame.state)=9;
table_adjacent:
    cm_fn_0054c8e0(date,b+0x28,1);
    B8((u32)&frame.state)=11;
    cm_fn_0054c5b0(date,b+0x40,1);
    iterator=0; B8((u32)&frame.state)=12;
    if ((s16)count_arg>0) {
        lower=W(b+0x2a); first=D(b+0x28); upper=W(b+0x42);
        do {
            source=table_arg+(u32)(s32)iterator*8;
            if (lower==W(source+2) && (u16)first==W(source)) {result=2;goto done;}
            if (upper==W(source+2) && W(b+0x40)==W(source)) {result=2;goto done;}
            value=D(date); metadata=D(date+4);
            D(b+0x10)=value; D(b+0x14)=metadata; year=W(b+0x12);
            if (year==W(source+2) && (u16)value==W(source)) {result=4;goto done;}
            iterator=(s8)((u8)iterator+1);
        } while ((s16)iterator<(s16)count_arg);
    }
    source=table_arg;
    value=cm_fn_0054c8e0(source,b+0x30,3);
    first=D(value); metadata=D(value+4); year=W(date+2);
    D(b+0x10)=first; D(b+0x14)=metadata; lower=W(b+0x12);
    if (lower>year || (lower==year && (s16)first>(s16)W(date))) {result=2;goto done;}
    index=(u32)(s32)(s16)count_arg;
    value=cm_fn_0054c5b0(source+index*8-8,b+0x30,3);
    first=D(value); metadata=D(value+4); year=W(date+2);
    D(b+0x10)=first; D(b+0x14)=metadata; lower=W(b+0x12);
    if (lower<year || (lower==year && (s16)first<(s16)W(date))) {result=2;goto done;}
    first=D(0x00ae2c90); metadata=D(0x00ae2c94);
    D(b+0x18)=first; D(b+0x1c)=metadata;
    value=cm_fn_0054c8e0(date,b+0x30,7);
    year=W(value+2); lower=W(b+0x12);
    if (lower>year || (lower==year && (s16)first>(s16)W(value))) result=2;
    else result=1;
done:
    CM_FS(volatile u32,0)=frame.prev;
    return (u8)result;
}

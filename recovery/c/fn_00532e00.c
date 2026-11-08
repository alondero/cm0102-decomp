/* CM0102 3.9.68, preferred-image VA00532e00, base00400000.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL0; indexed caller008288ae overwrites EAX before observation.
   The two original passes and every original helper/RNG call remain ordered.
   Numeric callees remain original-code boundaries. The RNG declaration here
   describes its native stack argument and observed EAX; its separate recovered
   register-prefix interface is not composed into this translation. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32,(u32)(a))
#define W(a) CM_GLOBAL(volatile u16,(u32)(a))
#define B(a) CM_GLOBAL(volatile u8,(u32)(a))
extern void CM_CDECL cm_fn_00533080(void);
extern void CM_CDECL cm_fn_00533710(void);
extern u32 CM_CDECL cm_fn_00539c60(u32);
extern u32 CM_CDECL cm_fn_0053a1b0(u32);
extern u32 CM_CDECL cm_fn_0053ab90(u32,u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);

void CM_CDECL cm_fn_00532e00(void)
{
    u32 count,base,row,ordinal,value,index,limit,table,cursor,selected,record;
    u8 current,secondary,step;
    s8 elapsed;

    cm_fn_00533080();
    count=D(0x00ae2364u);
    base=D(0x00ae23bcu);
    ordinal=0;
    if((s32)count>0) {
        row=base+0x1au;
        do {
            value=D(row);
            if(value) {
                value=cm_fn_00539c60(value);
                if(value && B(value))D(row-0x16u)=value;
                value=D(row);
                value=cm_fn_0053a1b0(value);
                if(value && B(value))D(row-0x12u)=value;
                index=D(0x009b59b4u);
                if(index==0xffffffffu) {
                    limit=D(0x00ae23a0u);
                    index=0;
                    if((s32)limit>0) {
                        table=D(0x00ae23f8u);
                        cursor=table;
                        do {
                            if(B(cursor)==0) {
                                D(0x009b59b4u)=index;
                                if(index!=0xffffffffu) {
                                    selected=table+index*60u;
                                    goto store_name;
                                }
                                break;
                            }
                            ++index;
                            cursor+=60u;
                        } while((s32)index<(s32)limit);
                    }
                    index=cm_fn_0090cfc0(limit);
                }
                table=D(0x00ae23f8u);
                selected=table+index*60u;
store_name:
                D(row-0xeu)=selected;
            }
            count=D(0x00ae2364u);
            ++ordinal;
            row+=0x6eu;
        } while((s32)ordinal<(s32)count);
    }
    cm_fn_00533710();
    count=D(0x00ae2364u);
    row=D(0x00ae23bcu);
    ordinal=0;
    if((s32)count<=0)return;
    do {
        value=D(row+0x1au);
        if(value==0)goto next_record;
        record=D(row+0x61u);
        if(record==0)goto next_record;
        current=B(row+0x22u);
        if(current) {
            elapsed=(s8)(u8)cm_fn_0053ab90(row,0x00ae2c90u);
            if(elapsed<20) {
                B(row+0x22u)=0;
                goto update_secondary;
            }
            elapsed=(s8)(u8)cm_fn_0053ab90(row,0x00ae2c90u);
            if(elapsed<30) {
                record=D(row+0x61u);
                if((s16)W(record+5u)<80) {
                    B(row+0x22u)=0;
                    goto update_secondary;
                }
            }
            elapsed=(s8)(u8)cm_fn_0053ab90(row,0x00ae2c90u);
            if(elapsed<26) {
                record=D(row+0x61u);
                if((s16)W(record+9u)<4750) {
                    B(row+0x22u)=0;
                    goto update_secondary;
                }
            }
            elapsed=(s8)(u8)cm_fn_0053ab90(row,0x00ae2c90u);
            if(elapsed<25 && B(row+0x22u)>10) {
                B(row+0x22u)=(u8)cm_fn_0090cfc0(5);
            } else {
                value=B(row+0x22u);
                step=(u8)cm_fn_0090cfc0(value);
                current=B(row+0x22u);
                B(row+0x22u)=(u8)(current-step);
            }
        } else {
            if((s16)W(record+5u)>140 && (s8)B(row+0x18u)>20) {
                value=cm_fn_0090cfc0(4);
                if(value==0)B(row+0x22u)=(u8)cm_fn_0090cfc0(3);
            }
        }
update_secondary:
        secondary=B(row+0x23u);
        if(secondary) {
            current=B(row+0x22u);
            if(current==0) {
                B(row+0x23u)=current;
                goto final_fields;
            }
            if(secondary>4) {
                record=D(row+0x61u);
                if(B(record+0x15u)!=20 && B(record+0x14u)!=20) {
                    B(row+0x23u)=1;
                    goto final_fields;
                }
            }
            if(secondary>current) {
                record=D(row+0x61u);
                if(B(record+0x15u)!=20)B(row+0x23u)=0;
                else {
                    value=B(row+0x22u);
                    B(row+0x23u)=(u8)cm_fn_0090cfc0(value);
                }
            } else {
                value=cm_fn_0090cfc0(3);
                if(value) {
                    value=B(row+0x23u);
                    step=(u8)cm_fn_0090cfc0(value);
                    step=(u8)(0xffu-step);
                    secondary=B(row+0x23u);
                    B(row+0x23u)=(u8)(secondary+step);
                } else {
                    step=(u8)cm_fn_0090cfc0(3);
                    secondary=B(row+0x23u);
                    step=(u8)(step+1u);
                    B(row+0x23u)=(u8)(secondary+step);
                }
            }
        }
final_fields:
        current=B(row+0x22u);
        if(current==0)B(row+0x23u)=0;
        D(row+0x4eu)=0;
        D(row+0x52u)=0;
next_record:
        count=D(0x00ae2364u);
        ++ordinal;
        row+=0x6eu;
    } while((s32)ordinal<(s32)count);
}

#include "cmre.h"
/* Preferred-image VA008afa40/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS4/RET16/full boolean; scratch8 limitBYTEL2/auxBYTEL3/maskDWORDL4.
   Keep signed BYTE loops, mask propagation, WORD subset comparisons,
   ordered-greater float decision, 25-byte forward displacement copy and
   original formal BYTE overwrite used as the outer scan counter. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define B(a) (*(volatile u8 *)(a))
typedef float float_8afa40 __attribute__((may_alias));
#define F(a) (*(volatile float_8afa40 *)(a))
extern u32 CM_CDECL cm_fn_005b7d20(u32);
extern u32 CM_STDCALL cm_fn_0085add0(u32,u32);
extern u32 CM_STDCALL cm_fn_0085a760(u32,u32);

u32 CM_THISCALL cm_fn_008afa40(u32 self,volatile u32 person_arg,
    volatile u32 club_arg,volatile u32 score_arg,volatile u32 mask_arg)
{
    u8 scratch[8];
    u32 l=(u32)scratch,person=person_arg,club,id,index,row,mask,old_mask;
    u32 pointer,context,eax,required,other,outer,inner,source,destination;
    u32 value,raw,second,third,count;
    u8 entry_limit=(u8)(B(self+0x253)+11u),limit,byte,aux;
    volatile long double incoming,existing;
    B(l+3)=0;B(l+2)=entry_limit;
    if(person==0) goto unchanged;
    pointer=D(person+0x61);if(pointer==0) goto unchanged;
    club=club_arg;if(club==0) goto unchanged;
    index=0;
    if((s8)entry_limit>0){
        id=D(person);
        do{
            row=self+(u32)(s32)(s8)(u8)index*25u;
            if(D(row+0x4e)==id){
                incoming=(long double)F((u32)&score_arg);
                existing=(long double)F(row+0x63);
                if(incoming>existing){raw=score_arg;D(row+0x63)=raw;}
                goto unchanged;
            }
            index=(u8)(index+1u);
        }while((s8)(u8)index<(s8)entry_limit);
    }
    mask=mask_arg;
    if((u16)mask!=0) D(l+4)=mask;
    else{pointer=D(person+0x61);mask=cm_fn_005b7d20(pointer);D(l+4)=mask;}
    if(mask&2u){mask|=4u;D(l+4)=mask;}
    if(mask&8u){mask|=0x10u;D(l+4)=mask;}
    if(mask&0x20u){mask|=0x10u;D(l+4)=mask;}
    if(mask&0x40u){
        if(mask&0x80u){mask|=0x200u;D(l+4)=mask;}
        if((mask&0x40u)!=0 && (u16)(mask&0x800u)!=0){mask|=0x200u;D(l+4)=mask;}
    }
    club=club_arg;context=D(0x00dd7ef0);(void)context;eax=cm_fn_0085add0(club,0);
    if(eax!=0){
        club=club_arg;context=D(0x00dd7ef0);(void)context;eax=cm_fn_0085a760(person,club);
        other=(u32)(s32)(s8)(u8)eax;
        if((s32)other<0) other=0u-other;
        B(l+3)=(u8)other;
    }
    limit=B(l+2);index=0;
    if((s8)limit>0){
        do{
            row=self+(u32)(s32)(s8)(u8)index*25u;
            if(D(row+0x4e)==0xffffffffu){
                required=W(self+(u32)(s32)(s8)(u8)index*2u+0x26);
                if((u16)(required&mask)==(u16)required) goto free_slot;
            }
            limit=B(l+2);index=(u8)(index+1u);
        }while((s8)(u8)index<(s8)limit);
    }
    limit=B(l+2);outer=0;B((u32)&mask_arg)=0;
    if((s8)limit>0){
        do{
            row=self+(u32)(s32)(s8)(u8)outer*25u;id=D(row+0x4e);
            if(id!=0xffffffffu){
                required=W(self+(u32)(s32)(s8)(u8)outer*2u+0x26);
                if((u16)(required&mask)==(u16)required){
                    pointer=D(0x00ae23bc);other=D(pointer+id*110u+0x61);
                    old_mask=cm_fn_005b7d20(other);
                    if(old_mask&2u) old_mask|=4u;
                    if(old_mask&8u) old_mask|=0x10u;
                    if(old_mask&0x20u) old_mask|=0x10u;
                    if(old_mask&0x40u){
                        if(old_mask&0x80u) old_mask|=0x200u;
                        if((old_mask&0x40u)!=0 && (u16)(old_mask&0x800u)!=0) old_mask|=0x200u;
                    }
                    inner=0;
                    do{
                        pointer=self+(u32)(s32)(s8)(u8)inner*25u;
                        if(D(pointer+0x4e)==0xffffffffu){
                            required=W(self+(u32)(s32)(s8)(u8)inner*2u+0x26);
                            if((u16)(required&old_mask)==(u16)required) goto displace;
                        }
                        limit=B(l+2);inner=(u8)(inner+1u);
                    }while((s8)(u8)inner<(s8)limit);
                    outer=B((u32)&mask_arg);mask=D(l+4);
                }
            }
            limit=B(l+2);outer=(u8)(outer+1u);B((u32)&mask_arg)=(u8)outer;
        }while((s8)(u8)outer<(s8)limit);
    }
    entry_limit=B(l+2);goto unchanged;
free_slot:
    id=D(person);raw=score_arg;D(row+0x4e)=id;
    club=club_arg;D(row+0x63)=raw;goto fields;
displace:
    outer=B((u32)&mask_arg);row=self+(u32)(s32)(s8)(u8)outer*25u;
    source=row+0x4e;destination=self+(u32)(s32)(s8)(u8)inner*25u+0x4e;
    for(index=0;index<6;++index){value=D(source);D(destination)=value;source+=4;destination+=4;}
    byte=B(source);B(destination)=byte;
    person=person_arg;id=D(person);D(row+0x4e)=id;
    raw=score_arg;club=club_arg;D(row+0x63)=raw;
fields:
    other=D(club);aux=B(l+3);D(row+0x5e)=other;B(row+0x62)=aux;
    pointer=D(person+4);
    if(pointer!=0){other=D(pointer+0x33);D(row+0x52)=other;}else D(row+0x52)=0xffffffffu;
    pointer=D(person+8);
    if(pointer!=0){second=D(pointer+0x33);D(row+0x56)=second;}else D(row+0x56)=0xffffffffu;
    pointer=D(person+0xc);third=0xffffffffu;
    if(pointer!=0) third=D(pointer+0x33);
    D(row+0x5a)=third;count=B(self+0x252);limit=B(l+2);count=(u8)(count+1u);
    B(self+0x252)=(u8)count;
    return (u8)count==limit;
unchanged:
    return B(self+0x252)==entry_limit;
}

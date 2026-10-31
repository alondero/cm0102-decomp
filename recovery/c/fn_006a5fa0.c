#include "cmre.h"
/* Preferred-image VA006a5fa0/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS8/RET32/full incidental EAX, SEH95cf16/scratch118:
   counterL1/sixBYTEoutputsL2..L7/selfL8/threeDWORDsLc,L10,L14/
   date-and-serializerL18/newsrecordL38. Keep adjacency and late input reads.
   Original785790 reads only the signed index BYTE (valid0..49), so partial
   CL/DL pushes need no guessed upper register bits. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_THISCALL cm_fn_0077bbe0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0054a110(u32);
extern u32 CM_THISCALL cm_fn_00785790(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_006aaee0(u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern u32 CM_CDECL cm_fn_007e4750(u32,u32,u32);
extern u32 CM_CDECL cm_fn_007e4de0(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_007861e0(u32,u32,u32);

u32 CM_THISCALL cm_fn_006a5fa0(u32 self,volatile u32 first_arg,
    volatile u32 second_arg,volatile u32 third_arg,volatile u16 fourth_arg,
    volatile u32 fifth_arg,volatile u32 sixth_arg,
    volatile u32 seventh_arg,volatile u32 eighth_arg)
{
    volatile struct {u32 prev,handler,state;} frame;
    u8 scratch[0x118];
    u32 l=(u32)scratch,eax,first,second,third,value,index,fifth,sixth,context;
    eax=CM_FS(volatile u32,0);frame.state=0xffffffffu;
    frame.handler=0x0095cf16;frame.prev=eax;CM_FS(volatile u32,0)=(u32)&frame;
    first=first_arg;D(l+8)=self;
    B(l+2)=0;B(l+5)=0;B(l+7)=0;B(l+6)=0;B(l+3)=0;B(l+4)=0;
    if (!first) goto done;
    second=second_arg;if(!second) goto done;
    third=third_arg;if(!third) goto done;
    eax=D(third+0x14);if(!eax) goto done;
    cm_fn_0077bbe0(l+0x38,0x138c,0);frame.state=0;B(l+1)=0;
    cm_fn_0054a110(l+0x18);B((u32)&frame.state)=1;
    D(l+0xc)=0xffffffffu;D(l+0x10)=0xffffffffu;D(l+0x14)=0xffffffffu;
    index=B(l+1);B(l+1)=(u8)(index+1);value=(u32)(s32)(s16)fourth_arg;
    cm_fn_00785790(l+0x38,index,value);
    index=B(l+1);value=D(first);B(l+1)=(u8)(index+1);
    cm_fn_00785790(l+0x38,index,value);
    index=B(l+1);B(l+1)=(u8)(index+1);value=D(second);
    cm_fn_00785790(l+0x38,index,value);
    index=B(l+1);fifth=fifth_arg;B(l+1)=(u8)(index+1);
    cm_fn_00785790(l+0x38,index,fifth);
    index=B(l+1);sixth=sixth_arg;B(l+1)=(u8)(index+1);
    cm_fn_00785790(l+0x38,index,sixth);
    index=B(l+1);value=seventh_arg;B(l+1)=(u8)(index+1);
    cm_fn_00785790(l+0x38,index,value);
    index=B(l+1);B(l+1)=(u8)(index+1);value=eighth_arg;
    cm_fn_00785790(l+0x38,index,value);
    index=B(l+1);value=D(third+8);B(l+1)=(u8)(index+1);
    cm_fn_00785790(l+0x38,index,value);
    value=D(third+8);B(l+2)=(s32)fifth>(s32)sixth;
    context=D(l+8);
    cm_fn_006aaee0(context,third,first,l+0xc,value,l+2,l+5,l+7,l+6,l+3,l+4);
    index=B(l+1);value=D(l+0xc);B(l+1)=(u8)(index+1);
    cm_fn_00785790(l+0x38,index,value);
    index=B(l+1);B(l+1)=(u8)(index+1);value=D(l+0x10);
    cm_fn_00785790(l+0x38,index,value);
    index=B(l+1);value=D(l+0x14);B(l+1)=(u8)(index+1);
    cm_fn_00785790(l+0x38,index,value);
    value=cm_fn_0090cfc0(4);index=B(l+1);B(l+1)=(u8)(index+1);
    cm_fn_00785790(l+0x38,index,value);
    index=B(l+1);B(l+1)=(u8)(index+1);value=(u32)(s32)(s8)B(l+2);
    cm_fn_00785790(l+0x38,index,value);
    value=(u32)(s32)(s8)B(l+5);index=B(l+1);B(l+1)=(u8)(index+1);
    cm_fn_00785790(l+0x38,index,value);
    index=B(l+1);B(l+1)=(u8)(index+1);value=(u32)(s32)(s8)B(l+7);
    cm_fn_00785790(l+0x38,index,value);
    value=(u32)(s32)(s8)B(l+6);index=B(l+1);B(l+1)=(u8)(index+1);
    cm_fn_00785790(l+0x38,index,value);
    index=B(l+1);B(l+1)=(u8)(index+1);value=(u32)(s32)(s8)B(l+3);
    cm_fn_00785790(l+0x38,index,value);
    value=(u32)(s32)(s8)B(l+4);index=B(l+1);B(l+1)=(u8)(index+1);
    cm_fn_00785790(l+0x38,index,value);
    cm_fn_007e4750(third,first,l+0x18);
    cm_fn_007e4de0(l+0x18,l+0x38,l+1);
    context=D(0x00b7431c);eax=cm_fn_007861e0(context,l+0x38,first);
done:
    CM_FS(volatile u32,0)=frame.prev;
    return eax;
}

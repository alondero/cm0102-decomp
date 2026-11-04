/* OFFLINE C CANDIDATE: no build, test or acceptance claim.
   Preferred-image VA004ad9c0, 2267 inventory body bytes, base00400000.
   CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Derived from complete original listing, not a shape clone of00453120.
   cdecl: four physical stack slots; full EAX is explicitly0 or1.
   Setter observes signed low byte of index; its high24 bits are unobserved.
   All noncompiler callees remain original-code boundaries. */
#include "cmre.h"
extern unsigned long long CM_THISCALL cm_fn_00785790(void *, u32, u32);
extern unsigned long long CM_CDECL cm_fn_007e4de0(void *, void *, void *);
extern unsigned long long CM_CDECL cm_fn_007e5160(void *, void *, void *);
extern unsigned long long CM_CDECL cm_fn_007e5420(void *, void *, void *);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,void *,void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern void CM_CDECL cm_fn_005e8290(const char *,const char *,int);

u32 CM_CDECL cm_fn_004ad9c0(u32 first, u8 *record, s8 selector, void *destination)
{
    /* L = ESP immediately after SUB204; buffers remain uninitialized. */
    u8 scratch[0x204];
    volatile u8 *counter = scratch + 3;
    u8 *low = scratch + 4, *high = scratch + 0x104;
    u8 *error_first, *error_second;
    u32 line, index, value, target;
    long double converted;
    if (!record) {
        cm_fn_00944cff(0x0099ca94,0,0,high,low);
        error_first=high; error_second=low; line=0x1ab7;
        goto error;
    }
    if (!destination) {
        cm_fn_00944cff(0x0099ca94,0,0,low,high);
        error_first=low; error_second=high; line=0x1abf;
        goto error;
    }
    *counter=1;
    cm_fn_00785790(destination,0,first);
    index=*counter;
    *counter=(u8)(index+1);
    cm_fn_00785790(destination,index,(u32)(s32)selector);
    index=(u32)(s32)selector-1u;
    if (index>0x15) goto invalid;
    target=CM_GLOBAL(volatile u32,0x004ae29c+index*4);
    switch(target) {
    case 0x004ada8a: /* selector 1 */
        index=*counter;
        *counter=(u8)(index+1);
        value=(u32)(s32)*(volatile s8 *)(record+0x0);
        cm_fn_00785790(destination,index,value);
        index=*counter;
        value=*(volatile u32 *)(record+0x1);
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        return 1;
    case 0x004adac9: /* selector 2 */
        index=*counter;
        *counter=(u8)(index+1);
        value=(u32)(s32)*(volatile s16 *)(record+0x5);
        cm_fn_00785790(destination,index,value);
        index=*counter;
        value=*(volatile u32 *)(record+0x7);
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        index=*counter;
        *counter=(u8)(index+1);
        value=*(volatile u16 *)(record+0xb);
        cm_fn_00785790(destination,index,value);
        return 1;
    case 0x004adb24: /* selector 3 */
        value=(u32)(s32)*(volatile s16 *)(record+0xd);
        index=*counter;
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        index=*counter;
        *counter=(u8)(index+1);
        value=*(volatile u32 *)(record+0xf);
        cm_fn_00785790(destination,index,value);
        index=*counter;
        value=*(volatile u16 *)(record+0x13);
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        return 1;
    case 0x004adb7f: /* selector 4 */
        index=*counter;
        *counter=(u8)(index+1);
        value=(u32)(s32)*(volatile s16 *)(record+0x15);
        cm_fn_00785790(destination,index,value);
        index=*counter;
        value=*(volatile u32 *)(record+0x17);
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        index=*counter;
        *counter=(u8)(index+1);
        value=*(volatile u16 *)(record+0x1b);
        cm_fn_00785790(destination,index,value);
        return 1;
    case 0x004adbda: /* selector 5 */
        value=(u32)(s32)*(volatile s8 *)(record+0x1d);
        index=*counter;
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        index=*counter;
        *counter=(u8)(index+1);
        value=(u32)(s32)*(volatile s8 *)(record+0x1e);
        cm_fn_00785790(destination,index,value);
        index=*counter;
        value=*(volatile u32 *)(record+0x1f);
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        index=*counter;
        *counter=(u8)(index+1);
        value=*(volatile u16 *)(record+0x23);
        cm_fn_00785790(destination,index,value);
        return 1;
    case 0x004adc4e: /* selector 6 */
        index=*counter;
        value=*(volatile u32 *)(record+0x25);
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        cm_fn_007e4de0(record+0x29,destination,(void *)counter);
        return 1;
    case 0x004adc87: /* selector 7 */
        index=*counter;
        value=*(volatile u32 *)(record+0x48);
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        cm_fn_007e4de0(record+0x4c,destination,(void *)counter);
        return 1;
    case 0x004adcc0: /* selector 8 */
        index=*counter;
        value=*(volatile u32 *)(record+0x6b);
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        index=*counter;
        *counter=(u8)(index+1);
        value=*(volatile u32 *)(record+0x6f);
        cm_fn_00785790(destination,index,value);
        index=*counter;
        value=*(volatile u16 *)(record+0x73);
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        return 1;
    case 0x004add1a: /* selector 9 */
        cm_fn_007e4de0(record+0x75,destination,(void *)counter);
        return 1;
    case 0x004add3b: /* selector 10 */
        cm_fn_007e4de0(record+0x94,destination,(void *)counter);
        return 1;
    case 0x004add5f: /* selector 11 */
        cm_fn_007e5420(record+0xb3,destination,(void *)counter);
        index=*counter;
        value=*(volatile u32 *)(record+0xc7);
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        return 1;
    case 0x004add9e: /* selector 12 */
        cm_fn_007e5420(record+0xcb,destination,(void *)counter);
        index=*counter;
        value=*(volatile u32 *)(record+0xdf);
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        return 1;
    case 0x004adddd: /* selector 13 */
        cm_fn_007e5420(record+0xe3,destination,(void *)counter);
        index=*counter;
        value=*(volatile u32 *)(record+0xf7);
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        return 1;
    case 0x004ade1c: /* selector 14 */
        cm_fn_007e5420(record+0xfb,destination,(void *)counter);
        index=*counter;
        value=*(volatile u32 *)(record+0x10f);
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        return 1;
    case 0x004ade5b: /* selector 15 */
        index=*counter;
        *counter=(u8)(index+1);
        value=(u32)(s32)*(volatile s8 *)(record+0x113);
        cm_fn_00785790(destination,index,value);
        cm_fn_007e5160(record+0x114,destination,(void *)counter);
        index=*counter;
        *counter=(u8)(index+1);
        value=*(volatile u32 *)(record+0x12c);
        cm_fn_00785790(destination,index,value);
        index=*counter;
        value=*(volatile u16 *)(record+0x130);
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        return 1;
    case 0x004aded4: /* selector 16 */
        index=*counter;
        *counter=(u8)(index+1);
        value=(u32)(s32)*(volatile s8 *)(record+0x132);
        cm_fn_00785790(destination,index,value);
        cm_fn_007e5160(record+0x133,destination,(void *)counter);
        cm_fn_007e4de0(record+0x14b,destination,(void *)counter);
        return 1;
    case 0x004adf26: /* selector 17 */
        value=(u32)(s32)*(volatile s8 *)(record+0x16a);
        index=*counter;
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        cm_fn_007e5160(record+0x16b,destination,(void *)counter);
        index=*counter;
        value=*(volatile u32 *)(record+0x183);
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        index=*counter;
        *counter=(u8)(index+1);
        value=*(volatile u16 *)(record+0x187);
        cm_fn_00785790(destination,index,value);
        return 1;
    case 0x004adf9f: /* selector 18 */
        index=*counter;
        converted=(long double)*(volatile float *)(record+0x189);
        converted*= (long double)CM_GLOBAL(volatile float,0x00968960);
        *counter=(u8)(index+1);
        /* Original __ftol CALL004adfb7: signed64 truncation, use EAX low32. */
        value=(u32)(long long)converted;
        cm_fn_00785790(destination,index,value);
        cm_fn_007e5160(record+0x18d,destination,(void *)counter);
        index=*counter;
        *counter=(u8)(index+1);
        value=(u32)(s32)*(volatile s8 *)(record+0x1a5);
        cm_fn_00785790(destination,index,value);
        index=*counter;
        value=*(volatile u32 *)(record+0x1a6);
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        index=*counter;
        *counter=(u8)(index+1);
        value=*(volatile u16 *)(record+0x1aa);
        cm_fn_00785790(destination,index,value);
        return 1;
    case 0x004ae03e: /* selector 19 */
        value=(u32)(s32)*(volatile s8 *)(record+0x1ac);
        index=*counter;
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        cm_fn_007e5160(record+0x1ad,destination,(void *)counter);
        index=*counter;
        value=*(volatile u32 *)(record+0x1c5);
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        index=*counter;
        *counter=(u8)(index+1);
        value=*(volatile u16 *)(record+0x1c9);
        cm_fn_00785790(destination,index,value);
        return 1;
    case 0x004ae0b7: /* selector 20 */
        value=(u32)(s32)*(volatile s8 *)(record+0x1cb);
        index=*counter;
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        index=*counter;
        *counter=(u8)(index+1);
        value=(u32)(s32)*(volatile s8 *)(record+0x1cc);
        cm_fn_00785790(destination,index,value);
        cm_fn_007e5160(record+0x1cd,destination,(void *)counter);
        index=*counter;
        *counter=(u8)(index+1);
        value=*(volatile u32 *)(record+0x1e5);
        cm_fn_00785790(destination,index,value);
        index=*counter;
        value=*(volatile u16 *)(record+0x1e9);
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        return 1;
    case 0x004ae14c: /* selector 21 */
        index=*counter;
        *counter=(u8)(index+1);
        value=(u32)(s32)*(volatile s16 *)(record+0x1eb);
        cm_fn_00785790(destination,index,value);
        index=*counter;
        value=*(volatile u16 *)(record+0x1ed);
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        cm_fn_007e5160(record+0x1f3,destination,(void *)counter);
        cm_fn_007e4de0(record+0x20b,destination,(void *)counter);
        return 1;
    case 0x004ae1bc: /* selector 22 */
        index=*counter;
        *counter=(u8)(index+1);
        value=(u32)(s32)*(volatile s16 *)(record+0x22a);
        cm_fn_00785790(destination,index,value);
        index=*counter;
        value=*(volatile u16 *)(record+0x22c);
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        cm_fn_007e5160(record+0x232,destination,(void *)counter);
        cm_fn_007e4de0(record+0x24a,destination,(void *)counter);
        return 1;
    }
invalid:
    cm_fn_00944cff(0x0099ca94,0,0,low,high);
    error_first=low; error_second=high; line=0x1bc0;
error:
    /* Three pushes before no-argument getter belong to the formatter. */
    value=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,value,error_first,error_second,line);
    cm_fn_005e8290((const char *)0x009870e0,(const char *)0x00ae24d0,0);
    CM_GLOBAL(volatile u32,0x00b67a34)=0;
    return 0;
}

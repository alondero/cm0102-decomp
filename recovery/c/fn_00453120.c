/* Preferred-image VA 00453120, 2539 original bytes, base 00400000.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   cdecl: four stack slots; original explicitly returns full DWORD 0 or 1.
   Setter index high24 are unobserved: original785790 reads only BYTE[ESP+4],
   tests AL, sign-extends AL; diagnostic path overwrites EAX/ECX via LEA.
   Callees execute original code. The __ftol source construct follows ADR0006. */
#include "cmre.h"
extern unsigned long long CM_THISCALL cm_fn_00785790(void *, u32, u32);
extern unsigned long long CM_CDECL cm_fn_007e4de0(void *, void *, void *);
extern unsigned long long CM_CDECL cm_fn_007e5160(void *, void *, void *);
extern unsigned long long CM_CDECL cm_fn_007e5420(void *, void *, void *);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,void *,void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern void CM_CDECL cm_fn_005e8290(const char *,const char *,int);

u32 CM_CDECL cm_fn_00453120(u32 first, u8 *record, s8 selector, void *destination)
{
    /* L is ESP immediately after SUB204: counter L+3, buffers L+4/L+104. */
    u8 scratch[0x204];
    u8 *counter = scratch + 3;
    u8 *low = scratch + 4, *high = scratch + 0x104;
    u8 *error_first, *error_second;
    u32 line, index, value, target;
    long double converted;
    if (!record) {
        cm_fn_00944cff(0x0098b6a0,0,0,high,low);
        error_first=high; error_second=low; line=0x1829;
        goto error;
    }
    if (!destination) {
        cm_fn_00944cff(0x0098b6a0,0,0,low,high);
        error_first=low; error_second=high; line=0x1831;
        goto error;
    }
    *counter=1;
    cm_fn_00785790(destination,0,first);
    index=*counter;
    *counter=(u8)(index+1);
    cm_fn_00785790(destination,index,(u32)(s32)selector);
    index=(u32)(s32)selector-1u;
    if (index>0x1f) goto invalid;
    target=CM_GLOBAL(volatile u32,0x00453b0c+index*4);
    switch(target) {
    case 0x004531eb: /* selector 1 */
        index=*counter;
        *counter=(u8)(index+1);
        value=*(volatile u8 *)(record+0x0);
        cm_fn_00785790(destination,index,value);
        index=*counter;
        value=*(volatile u32 *)(record+0x1);
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        index=*counter;
        *counter=(u8)(index+1);
        value=*(volatile u16 *)(record+0x5);
        cm_fn_00785790(destination,index,value);
        return 1;
    case 0x00453246: /* selector 2 */
        index=*counter;
        value=*(volatile u8 *)(record+0x7);
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        index=*counter;
        *counter=(u8)(index+1);
        value=*(volatile u32 *)(record+0x8);
        cm_fn_00785790(destination,index,value);
        index=*counter;
        value=*(volatile u16 *)(record+0xc);
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        return 1;
    case 0x004532a2: /* selector 3 */
        index=*counter;
        *counter=(u8)(index+1);
        value=*(volatile u32 *)(record+0xe);
        cm_fn_00785790(destination,index,value);
        cm_fn_007e4de0(record+0x12,destination,counter);
        return 1;
    case 0x004532db: /* selector 4 */
        index=*counter;
        value=*(volatile u32 *)(record+0x31);
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        cm_fn_007e4de0(record+0x35,destination,counter);
        return 1;
    case 0x00453314: /* selector 5 */
        index=*counter;
        value=*(volatile u32 *)(record+0x54);
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        cm_fn_007e4de0(record+0x58,destination,counter);
        return 1;
    case 0x0045334d: /* selector 6 */
        index=*counter;
        value=*(volatile u32 *)(record+0x77);
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        index=*counter;
        *counter=(u8)(index+1);
        value=*(volatile u16 *)(record+0x7b);
        cm_fn_00785790(destination,index,value);
        return 1;
    case 0x0045338f: /* selector 7 */
        cm_fn_007e4de0(record+0x7d,destination,counter);
        return 1;
    case 0x004533b0: /* selector 8 */
        cm_fn_007e4de0(record+0x9c,destination,counter);
        return 1;
    case 0x004533d4: /* selector 9 */
        cm_fn_007e4de0(record+0xbb,destination,counter);
        return 1;
    case 0x004533f8: /* selector 10 */
        cm_fn_007e4de0(record+0xda,destination,counter);
        return 1;
    case 0x0045341c: /* selector 11 */
        cm_fn_007e4de0(record+0xf9,destination,counter);
        return 1;
    case 0x00453440: /* selector 12 */
        cm_fn_007e5420(record+0x118,destination,counter);
        return 1;
    case 0x00453464: /* selector 13 */
        cm_fn_007e5420(record+0x12c,destination,counter);
        return 1;
    case 0x00453488: /* selector 14 */
        cm_fn_007e5420(record+0x140,destination,counter);
        return 1;
    case 0x004534ac: /* selector 15 */
        cm_fn_007e5420(record+0x154,destination,counter);
        return 1;
    case 0x004534d0: /* selector 16 */
        index=*counter;
        value=(u32)(s32)*(volatile s8 *)(record+0x168);
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        cm_fn_007e5160(record+0x169,destination,counter);
        index=*counter;
        value=*(volatile u16 *)(record+0x181);
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        return 1;
    case 0x0045352e: /* selector 17 */
        index=*counter;
        *counter=(u8)(index+1);
        value=(u32)(s32)*(volatile s8 *)(record+0x183);
        cm_fn_00785790(destination,index,value);
        cm_fn_007e5160(record+0x184,destination,counter);
        index=*counter;
        *counter=(u8)(index+1);
        value=*(volatile u16 *)(record+0x19c);
        cm_fn_00785790(destination,index,value);
        return 1;
    case 0x0045358c: /* selector 18 */
        index=*counter;
        value=(u32)(s32)*(volatile s8 *)(record+0x19e);
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        cm_fn_007e5160(record+0x19f,destination,counter);
        cm_fn_007e4de0(record+0x1b7,destination,counter);
        return 1;
    case 0x004535de: /* selector 19 */
        index=*counter;
        *counter=(u8)(index+1);
        value=(u32)(s32)*(volatile s8 *)(record+0x1d6);
        cm_fn_00785790(destination,index,value);
        cm_fn_007e5160(record+0x1d7,destination,counter);
        index=*counter;
        *counter=(u8)(index+1);
        value=*(volatile u16 *)(record+0x1ef);
        cm_fn_00785790(destination,index,value);
        return 1;
    case 0x0045363c: /* selector 20 */
        index=*counter;
        converted=(long double)*(volatile float *)(record+0x1f1);
        converted*= (long double)CM_GLOBAL(volatile float,0x00968960);
        *counter=(u8)(index+1);
        /* Original __ftol CALL00453654: EAX low32 of signed64 truncation. */
        value=(u32)(long long)converted;
        cm_fn_00785790(destination,index,value);
        cm_fn_007e5160(record+0x1f5,destination,counter);
        index=*counter;
        *counter=(u8)(index+1);
        value=(u32)(s32)*(volatile s8 *)(record+0x20d);
        cm_fn_00785790(destination,index,value);
        index=*counter;
        value=*(volatile u16 *)(record+0x20e);
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        return 1;
    case 0x004536c0: /* selector 21 */
        index=*counter;
        *counter=(u8)(index+1);
        value=(u32)(s32)*(volatile s8 *)(record+0x210);
        cm_fn_00785790(destination,index,value);
        cm_fn_007e5160(record+0x211,destination,counter);
        index=*counter;
        *counter=(u8)(index+1);
        value=*(volatile u16 *)(record+0x229);
        cm_fn_00785790(destination,index,value);
        return 1;
    case 0x0045371e: /* selector 22 */
        index=*counter;
        value=(u32)(s32)*(volatile s8 *)(record+0x22b);
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        index=*counter;
        *counter=(u8)(index+1);
        value=(u32)(s32)*(volatile s8 *)(record+0x22c);
        cm_fn_00785790(destination,index,value);
        cm_fn_007e5160(record+0x22d,destination,counter);
        index=*counter;
        *counter=(u8)(index+1);
        value=*(volatile u16 *)(record+0x245);
        cm_fn_00785790(destination,index,value);
        return 1;
    case 0x00453798: /* selector 23 */
        index=*counter;
        value=(u32)(s32)*(volatile s16 *)(record+0x247);
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        cm_fn_007e5160(record+0x249,destination,counter);
        return 1;
    case 0x004537d8: /* selector 24 */
        index=*counter;
        value=(u32)(s32)*(volatile s16 *)(record+0x261);
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        cm_fn_007e5160(record+0x263,destination,counter);
        return 1;
    case 0x00453818: /* selector 25 */
        index=*counter;
        value=(u32)(s32)*(volatile s16 *)(record+0x27b);
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        index=*counter;
        *counter=(u8)(index+1);
        value=*(volatile u16 *)(record+0x27d);
        cm_fn_00785790(destination,index,value);
        cm_fn_007e5160(record+0x283,destination,counter);
        cm_fn_007e4de0(record+0x29b,destination,counter);
        return 1;
    case 0x00453888: /* selector 26 */
        index=*counter;
        value=(u32)(s32)*(volatile s16 *)(record+0x2ba);
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        index=*counter;
        *counter=(u8)(index+1);
        value=*(volatile u16 *)(record+0x2bc);
        cm_fn_00785790(destination,index,value);
        cm_fn_007e5160(record+0x2c2,destination,counter);
        cm_fn_007e4de0(record+0x2da,destination,counter);
        return 1;
    case 0x004538f8: /* selector 27 */
        index=*counter;
        value=(u32)(s32)*(volatile s16 *)(record+0x2f9);
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        cm_fn_007e5160(record+0x2fb,destination,counter);
        return 1;
    case 0x00453938: /* selector 28 */
        index=*counter;
        value=*(volatile u32 *)(record+0x313);
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        cm_fn_007e5160(record+0x317,destination,counter);
        index=*counter;
        value=*(volatile u32 *)(record+0x32f);
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        index=*counter;
        *counter=(u8)(index+1);
        value=(u32)(s32)*(volatile s8 *)(record+0x33b);
        cm_fn_00785790(destination,index,value);
        return 1;
    case 0x004539ae: /* selector 29 */
        index=*counter;
        value=*(volatile u32 *)(record+0x33c);
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        cm_fn_007e5160(record+0x340,destination,counter);
        index=*counter;
        value=*(volatile u32 *)(record+0x358);
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        index=*counter;
        *counter=(u8)(index+1);
        value=(u32)(s32)*(volatile s8 *)(record+0x364);
        cm_fn_00785790(destination,index,value);
        return 1;
    case 0x00453a24: /* selector 30 */
        index=*counter;
        value=*(volatile u32 *)(record+0x365);
        *counter=(u8)(index+1);
        cm_fn_00785790(destination,index,value);
        return 1;
    case 0x00453a4e: /* selector 31 */
        index=*counter;
        *counter=(u8)(index+1);
        value=*(volatile u32 *)(record+0x36b);
        cm_fn_00785790(destination,index,value);
        return 1;
    case 0x00453a78: /* selector 32 */
        cm_fn_007e5160(record+0x371,destination,counter);
        return 1;
    }
invalid:
    cm_fn_00944cff(0x0098b6a0,0,0,low,high);
    error_first=low; error_second=high; line=0x1989;
error:
    /* Retained pushes before the no-argument call belong to sprintf below. */
    value=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,value,error_first,error_second,line);
    cm_fn_005e8290((const char *)0x009870e0,(const char *)0x00ae24d0,0);
    CM_GLOBAL(volatile u32,0x00b67a34)=0;
    return 0;
}

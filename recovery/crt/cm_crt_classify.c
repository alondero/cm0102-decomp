/* Primary 0094C322..0094C396, preferred-image VA; instruction-derived.
 * Caller argument high word is the external classification output buffer.
 * 0094ED85 remains an explicit dependency. Not standalone locale support.
 */
#include <stdint.h>
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;
#define R32(a) (*(volatile u32 *)(uintptr_t)(a))
#define R16(a) (*(volatile u16 *)(uintptr_t)(a))
#define R8(a) (*(volatile u8 *)(uintptr_t)(a))
typedef u32 (__attribute__((cdecl)) *classify_text_fn)(u32,volatile u8 *,u32,volatile u16 *,u32,u32,u32);
u32 cm_crt_classify_frame(u32 frame)
{
    u32 value=R32(frame+8u), result, table;
    if (value+1u<=0x100u) {
        table=R32(0x00ad8fd0u);
        result=R16(table+value*2u);
    } else {
        volatile u8 *text=(volatile u8 *)(uintptr_t)(frame-4u);
        u32 length;
        table=R32(0x00ad8fd0u);
        if ((R8(table+((value>>8)&255u)*2u+1u)&0x80u)!=0u) {
            text[2]&=0u;
            text[0]=(u8)(value>>8);
            text[1]=(u8)value;
            length=2u;
        } else {
            text[1]&=0u;
            text[0]=(u8)value;
            length=1u;
        }
        result=((classify_text_fn)(uintptr_t)0x0094ed85u)(1u,text,length,
            (volatile u16 *)(uintptr_t)(frame+10u),0u,0u,1u);
        if (result==0u) return 0u;
        result=R16(frame+10u);
    }
    return result&R32(frame+12u);
}

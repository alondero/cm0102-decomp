#include "cmre.h"

/* Preferred-image VA 0084c830, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   L is ESP after SUB 9c. PRINTDLG Lc, DOCINFO L50, metrics L64;
   original imports, nested arguments, uninitialized output and cleanup stay. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
typedef u32 (CM_STDCALL *gdi1)(u32);
typedef u32 (CM_STDCALL *gdi2)(u32,u32);
typedef u32 (CM_STDCALL *gdi5)(u32,u32,u32,u32,u32);
typedef u32 (CM_STDCALL *font14)(u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_STDCALL cm_fn_00944c16(u32);

u32 CM_STDCALL cm_fn_0084c830(volatile u32 lines,volatile u32 count,volatile u32 divisor)
{
    u8 scratch[0x9c];
    u32 local=(u32)scratch,result,hdc,font,metrics,table,item,position,index;
    u32 height,margin,bottom,page_height,remaining,scan,length,target,value;
    s32 width;
    gdi2 caps;
    value=D(0x00b67a2c);
    D(local+0x10)=value;
    D(local+0xc)=0x42;
    D(local+0x14)=0;
    D(local+0x18)=0;
    D(local+0x1c)=0;
    D(local+0x20)=0x100;
    W(local+0x24)=1;
    W(local+0x26)=1;
    W(local+0x28)=0;
    W(local+0x2a)=0;
    W(local+0x2c)=1;
    for (value=0x2e;value<=0x4a;value+=4) D(local+value)=0;
    result=cm_fn_00944c16(local+0xc);
    if (result==0) return result;
    hdc=D(local+0x1c);
    D(local+0x50)=0x14;
    D(local+0x54)=0x00a827dc;
    D(local+0x58)=0;
    D(local+0x5c)=0;
    D(local+0x60)=0;
    target=D(0x00967040);
    result=((gdi2)target)(hdc,local+0x50);
    if ((s32)result<=0) return result;
    table=lines;
    if (table==0) goto end_doc;
    hdc=D(local+0x1c);
    caps=(gdi2)D(0x0096701c);
    result=caps(hdc,8);
    width=(s32)result/(s32)(s16)divisor;
    target=D(0x00967020);
    font=((font14)target)(0,(u32)width,0,0,0,0,0,0,0,0,0,0,0,0x00a827d4);
    D(local+4)=font;
    if (font==0) goto end_doc;
    hdc=D(local+0x1c);
    target=D(0x00967024);
    metrics=((gdi2)target)(hdc,local+0x64);
    if (metrics==0) goto delete_font;
    value=count;
    index=0;
    D(local+8)=0;
    if ((s32)value<=0) goto delete_font;
    D(local)=table;
    position=D(local+4);
    do {
        if (index==0) goto new_page;
        table=D(local);
        item=D(table);
        if (item==0) {
            if ((s32)index<(s32)(value-1)) goto new_page;
            goto print_line;
        }
        hdc=D(local+0x1c);
        margin=caps(hdc,0x71);
        height=D(local+0x64);
        hdc=D(local+0x1c);
        bottom=height+position-margin-1;
        page_height=caps(hdc,10);
        if ((s32)bottom<(s32)page_height) goto print_line;
new_page:
        if (D(local+8)!=0) {
            hdc=D(local+0x1c);
            target=D(0x00967028);
            ((gdi1)target)(hdc);
        }
        hdc=D(local+0x1c);
        target=D(0x0096702c);
        result=((gdi1)target)(hdc);
        value=(s32)result>0;
        D(local+8)=value;
        if (value==0) goto delete_font;
        font=D(local+4);
        hdc=D(local+0x1c);
        target=D(0x00967030);
        ((gdi2)target)(hdc,font);
        hdc=D(local+0x1c);
        position=caps(hdc,0x71);
print_line:
        table=D(local);
        item=D(table);
        if (item!=0) {
            scan=item;
            remaining=0xffffffffu;
            do {
                if (remaining==0) break;
                --remaining;
                value=B(scan);
                ++scan;
            } while (value!=0);
            length=~remaining-1;
            hdc=D(local+0x1c);
            result=caps(hdc,0x70);
            hdc=D(local+0x1c);
            target=D(0x00967034);
            ((gdi5)target)(hdc,result,position,item,length);
            height=D(local+0x64);
            position+=height;
        }
        table=D(local);
        ++index;
        D(local)=table+4;
        value=count;
    } while ((s32)index<(s32)value);
    if (D(local+8)!=0) {
        hdc=D(local+0x1c);
        target=D(0x00967028);
        ((gdi1)target)(hdc);
    }
delete_font:
    font=D(local+4);
    target=D(0x00967038);
    ((gdi1)target)(font);
end_doc:
    hdc=D(local+0x1c);
    target=D(0x0096703c);
    return ((gdi1)target)(hdc);
}

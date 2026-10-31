#include "cmre.h"

/* Preferred-image VA 006336c0, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Keep capped selection, signed WORD scan indices, packed six-byte rows,
   pointer reloads, original sort/free and distinct failure returns/leaks. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_CDECL cm_fn_00945333(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_006336c0(u32 self)
{
    u8 scratch[0x204];
    u32 local=(u32)scratch,array,result,selected=0,total,club,scan,cursor;
    u32 count,value,key,table,offset,item,line,version,failure=0;
    D(local)=0;
    W(self+0x3e)=60;
    array=cm_fn_00944e46(240);
    if (array==0) {
        cm_fn_00944cff(0x009e9e14,0,0,local+0x104,local+4);
        line=0x161;
        goto diagnostic_first;
    }
    total=D(0x00ae235c);
    club=D(0x00ae23b4);
    scan=0;
    if ((s32)total>0) {
        cursor=array;
        do {
            count=(u32)(s32)(s16)W(self+0x3e);
            if ((s32)selected>=(s32)count) break;
            value=D(club+0x1db);
            key=D(0x009cf6f8);
            if (value==key) {
                D(cursor)=club;
                ++selected;
                cursor+=4;
            }
            ++scan;
            club+=581;
            total=D(0x00ae235c);
        } while ((s32)(s16)scan<(s32)total);
        D(local)=selected;
    }
    count=(u32)(s32)(s16)W(self+0x3e);
    if ((s32)selected<(s32)count) {
        line=0x172;
        goto diagnostic_second;
    }
    result=cm_fn_00944e46(count*6);
    D(self+0xa7)=result;
    if (result==0) {
        line=0x17c;
        failure=0xffffffffu;
        goto diagnostic_second;
    }
    scan=0;
    if ((s16)W(self+0x3e)>0) {
        do {
            table=D(self+0xa7);
            value=(u32)(s32)(s16)scan;
            item=D(array+value*4);
            offset=value*6;
            ++scan;
            D(table+offset)=item;
            item=D(array+value*4);
            table=D(self+0xa7);
            value=B(item+0x1df);
            B(table+offset+4)=(u8)value;
            table=D(self+0xa7);
            B(table+offset+5)=0;
        } while ((s16)scan<(s16)W(self+0x3e));
        selected=D(local);
    }
    count=(u32)(s32)(s16)W(self+0x3e);
    table=D(self+0xa7);
    cm_fn_00945333(table,count,6,0x00522190);
    cm_fn_009452ca(array);
    count=(u32)(s32)(s16)W(self+0x3e);
    return selected==count;
diagnostic_second:
    cm_fn_00944cff(0x009e9e14,0,0,local+4,local+0x104);
    version=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,local+4,local+0x104,line);
    cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
    D(0x00b67a34)=0;
    return failure;
diagnostic_first:
    version=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,local+0x104,local+4,line);
    cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
    D(0x00b67a34)=0;
    return 0;
}

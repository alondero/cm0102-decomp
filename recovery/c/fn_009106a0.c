#include "cmre.h"

/* Preferred-image VA 009106a0, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Scratch20c: two IDs L0/L4, host pointer L8, diagnostics Lc/L10c.
   Keep signed BYTE exclusion count, signed WORD nation-loop index, 52-entry
   cap and original allocation retention on an insufficient-candidate return. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u8 CM_THISCALL cm_fn_005fa730(u32,u32,s16,u32,u32,u32);
extern u32 CM_CDECL cm_fn_004afd80(u32,u8);
extern u32 CM_CDECL cm_fn_0053b3d0(u32);
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00945333(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

#define DIAG(name,extension,line) do { \
    cm_fn_00944cff(0x00ad443c,0,0,(name),(extension)); \
    value=cm_fn_0090d130(); \
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,value,(name),(extension),(line)); \
    cm_fn_005e8290(0x009870e0,0x00ae24d0,0); \
    D(0x00b67a34)=0; \
} while (0)

u32 CM_THISCALL cm_fn_009106a0(u32 self)
{
    u8 scratch[0x20c];
    u32 local=(u32)scratch,owner,year,id,value,base,entry,index,nation,count,selected,allocation;
    u8 returned,scan;
    B(self+0xba)=0;
    owner=D(0x00b5e134);
    D(local)=0xffffffffu;
    D(local+4)=0xffffffffu;
    if (owner==0) {
        DIAG(local+0x10c,local+0xc,0x3b7);
    } else {
        year=(u16)(W(self+0x40)+2);
        id=D(0x009cf79c);
        returned=cm_fn_005fa730(owner,id,(s16)year,local,local+4,1);
        scan=0;
        if ((s8)returned>0) {
            do {
                id=D(local+(u32)(s32)(s8)scan*4);
                if (id==0xffffffffu) break;
                base=D(0x00ae23a8);
                entry=D(base+id*290+0x71);
                if (entry!=0) {
                    value=D(entry);
                    id=D(0x009cfa08);
                    if (value==id) B(self+0xba)=(u8)(B(self+0xba)+1);
                }
                scan=(u8)(scan+1);
            } while ((s8)scan<(s8)returned);
        }
    }
    id=D(0x009cf79c);
    base=D(0x00ae23d0);
    value=cm_fn_004afd80(base+id*107,0);
    D(local+8)=value;
    if (value!=0) {
        nation=D(value+0x53);
        if (nation!=0) {
            entry=D(nation+0x71);
            if (entry!=0) {
                id=D(entry);
                value=D(0x009cfa08);
                if (id==value) {
                    id=D(nation);
                    value=D(local);
                    if (id!=value && id!=D(local+4)) B(self+0xba)=(u8)(B(self+0xba)+1);
                }
            }
        }
    }
    value=(u32)(s32)(s8)B(self+0xba);
    D(self+0xb6)=50-value;
    allocation=cm_fn_00944e46(0xd0);
    D(self+0xb2)=allocation;
    if (allocation==0) {
        DIAG(local+0xc,local+0x10c,0x3e6);
        return 0;
    }
    count=D(0x00ae2350);
    nation=D(0x00ae23a8);
    selected=0;
    index=0;
    if ((s32)count>0) {
        do {
            entry=D(nation+0x71);
            if (entry!=0) {
                id=D(entry);
                value=D(0x009cfa08);
                if (id==value && (s32)selected<52) {
                    value=cm_fn_0053b3d0(nation);
                    id=D(nation);
                    base=D(local);
                    if (id!=base && id!=D(local+4) && value!=D(local+8)) {
                        base=D(self+0xb2);
                        ++selected;
                        D(base+selected*4-4)=value;
                    }
                }
            }
            count=D(0x00ae2350);
            ++index;
            nation+=0x122;
        } while ((s32)(s16)index<(s32)count);
    }
    if ((s32)selected<(s32)D(self+0xb6)) return 0;
    allocation=D(self+0xb2);
    cm_fn_00945333(allocation,selected,4,0x004b07d0);
    return 1;
}

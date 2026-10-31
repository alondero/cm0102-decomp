#include "cmre.h"
/* Preferred-image VA 007cff70, base00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS0/full EAX. Scratch214: added BYTE L3,selfL4,countL8,competitionLc,
   allocationL10,diagnosticsL14/L114. 6831a0 is STD3 (RET c), not THIS2.
   Candidate and added counts retain their original signed WORD/BYTE uses.
   The excess-added diagnostic deliberately retains the allocation. */
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_STDCALL cm_fn_006831a0(u32,u32,u32);
extern u32 CM_STDCALL cm_fn_006830b0(u32,u32,u32);
extern u32 CM_CDECL cm_fn_004afbd0(u32,s16,s16);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);
#define DIAG(name,extension,line) do { \
    cm_fn_00944cff(0x00a78724,0,0,(name),(extension)); \
    value=cm_fn_0090d130(); \
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,value,(name),(extension),(line)); \
    value=cm_fn_005e8290(0x009870e0,0x00ae24d0,0); \
    D(0x00b67a34)=0; \
} while (0)

u32 CM_THISCALL cm_fn_007cff70(u32 self)
{
    u8 scratch[0x214];
    u32 l=(u32)scratch,base,id,competition,total,allocation,value,count,scan;
    u32 entry,secondary,primary,nation,index,offset,person,remaining;
    u8 selector,added;
    base=D(0x00adadfc);
    D(l+4)=self;
    id=D(0x009cf7c0);
    competition=D(base+id*4);
    total=D(0x00ae235c);
    D(l+8)=0;
    D(l+0xc)=competition;
    B(l+3)=0;
    allocation=cm_fn_00944e46(total*4);
    D(l+0x10)=allocation;
    if (allocation==0) {
        DIAG(l+0x114,l+0x14,0x2ee);
        return value;
    }
    count=0; scan=0;
    total=D(0x00ae235c);
    entry=D(0x00ae23b4);
    if ((s32)total>0) {
        nation=D(0x009cf620);
        do {
            secondary=D(entry+0x57);
            if (secondary!=0) {
                primary=D(entry+0x53);
                if (primary!=0) {
                    id=D(primary);
                    if (id==D(0x009cf434)) {
                        id=D(secondary);
                        if (id==nation || id==D(0x009cf764)) {
                            index=(u32)(s32)(s16)count;
                            ++count;
                            D(allocation+index*4)=entry;
                            nation=D(0x009cf620);
                        }
                    }
                }
            }
            total=D(0x00ae235c);
            ++scan;
            entry+=0x245;
        } while ((s32)scan<(s32)total);
        D(l+8)=count;
        if ((u16)count!=0) goto groups;
    }
    cm_fn_009452ca(allocation);
    DIAG(l+0x14,l+0x114,0x301);
    return value;

groups:
    selector=0xff;
    do {
        competition=D(l+0xc);
        if (selector!=0xff) {
            base=D(competition+0xc);
            competition=D(base+(u32)(s32)(s8)selector*4);
        }
        index=0;
        if ((s16)W(competition+0x3e)>0) {
            offset=0;
            do {
                entry=D(competition+0xb1)+offset;
                if (B(entry+0x37)==3) {
                    person=D(entry);
                    base=D(0x00ae23d0);
                    (void)D(l+4);
                    cm_fn_006831a0(person,base+nation*107,1);
                    added=B(l+3);
                    nation=D(0x009cf620);
                    B(l+3)=(u8)(added+1);
                }
                total=(u32)(s32)(s16)W(competition+0x3e);
                ++index; offset+=0x3b;
            } while ((s32)index<(s32)total);
        }
        selector=(u8)(selector+1);
    } while ((s8)selector<2);
    added=B(l+3);
    count=D(l+8);
    if ((s16)(s8)added>(s16)count) {
        DIAG(l+0x14,l+0x114,0x320);
        return value;
    }
    allocation=D(l+0x10);
    cm_fn_004afbd0(allocation,(s16)count,(s16)(s8)added);
    remaining=(u32)(s32)(s8)added;
    if ((s32)remaining>0) {
        entry=allocation;
        do {
            id=D(0x009cf7c0);
            (void)D(l+4);
            base=D(0x00ae23d0);
            person=D(entry);
            cm_fn_006830b0(person,base+id*107,1);
            entry+=4;
            --remaining;
        } while (remaining!=0);
    }
    return cm_fn_009452ca(allocation);
}

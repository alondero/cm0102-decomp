#include "cmre.h"
/* Original instruction evidence: preferred-image VAs 006a4d40-006a4f76,
 * CM0102 3.9.68 SHA-256
 * 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
 * ECX is saved at 006a4d4e; ret 12 at 006a4f76.
 * Scratch offsets below are relative to the allocation after stack probing,
 * before the four register saves (not relative to the saved-register ESP).
 */
extern unsigned long long CM_THISCALL cm_fn_00785830(void *,u32);
extern u32 CM_CDECL cm_fn_0053b900(u32 *);
extern char * CM_CDECL cm_fn_0066f4e0(u32,char *);
extern u32 CM_THISCALL cm_fn_00690d80(u32,u32,s32,u32,u32);
extern u8 CM_STDCALL cm_fn_00693850(u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern unsigned long long CM_THISCALL cm_fn_004ae660(void *,u32);
extern u32 CM_CDECL cm_fn_0053dba0(u32,u32,u32,u32);
extern u32 * CM_CDECL cm_fn_00945a60(u32 *,char *);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32,const char *,u32,u32);

u32 CM_THISCALL cm_fn_006a4d40(u32 self,u32 input,u32 mode,u32 output)
{
    u32 scratch[0x27e8/4];
    u8 *b=(u8 *)scratch;
    u32 index,age,offset,club,r,i,id,person,base;
    s32 count=0;
    for(i=0;i<15;i++) scratch[1+i]=0xffffffffu;
    index=(u32)cm_fn_00785830((void *)input,0);
    age=(u32)cm_fn_00785830((void *)input,1);
    base=CM_GLOBAL(volatile u32,0x00ae23b4);
    offset=index*581u;
    scratch[0]=offset;
    club=offset+base;
    if(mode==0) {
        r=cm_fn_0053b900((u32 *)club);
        cm_fn_0066f4e0((u32)(b+0xd8),(char *)(r ? 0x00a0ac00u : 0x00a0abe8u));
    } else {
        cm_fn_00690d80(self,club,(s32)age,(u32)(b+0x1a3c),0x1388);
        count=(s8)cm_fn_00693850((u32)(b+0x1460),club,(u32)(b+4));
        if(count)
            cm_fn_00944c9f((u32)(b+0xd8),0x991948,(u32)(b+0x1a3c),(u32)(b+0x1460));
        else
            cm_fn_00944c9f((u32)(b+0xd8),0x991944,(u32)(b+0x1a3c));
    }
    cm_fn_004ae660((void *)output,(u32)(b+0xd8));
    for(i=0; count>0; --count,++i) {
        id=*(u32 *)(b+4+4*i);
        if(id!=0xffffffffu) {
            base=CM_GLOBAL(volatile u32,0x00ae23bc);
            person=base+id*110u;
            cm_fn_0053dba0(person,(u32)(b+0x40),0x96,1);
            r=(u32)cm_fn_00945a60((u32 *)(b+0xd8),(char *)(b+0x40));
            if(r) {
                id=*(u32 *)(b+4+4*i);
                base=CM_GLOBAL(volatile u32,0x00ae23bc);
                person=base+id*110u;
                cm_fn_004ae8a0(output,(char *)(b+0x40),0x7db,person);
            }
        }
    }
    base=CM_GLOBAL(volatile u32,0x00ae23b4);
    offset=scratch[0];
    r=(u32)cm_fn_00945a60((u32 *)(b+0xd8),(char *)(offset+base+4));
    if(r) {
        base=CM_GLOBAL(volatile u32,0x00ae23b4);
        r=cm_fn_004ae8a0(output,(char *)(offset+base+4),0x7d5,offset+base);
    }
    return r;
}

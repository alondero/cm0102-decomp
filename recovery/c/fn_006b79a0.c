#include "cmre.h"
extern u32 CM_THISCALL cm_fn_004cf5a0(u32,u32);
extern u32 CM_CDECL cm_fn_0053b900(u32 *);
extern u32 CM_CDECL cm_fn_00547d60(u32,u32);
extern void CM_CDECL cm_fn_00547d90(u32,u32,u32);
extern void CM_CDECL cm_fn_005896d0(u32 *,u32 *,u32 *,u32);
extern void CM_CDECL cm_fn_00589bc0(u32,u32);
extern void CM_CDECL cm_fn_005e8290(const char *,const char *,int);
extern u32 CM_CDECL cm_fn_00600a50(u32,u32,u8,u32,u32);
extern u32 CM_CDECL cm_fn_006b7cf0(u32,u32);
extern u32 CM_STDCALL cm_fn_007c3a40(u32,u32);
extern unsigned long long CM_CDECL cm_fn_0090cfc0(u32);
extern char * CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,void *,void *);
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile s16 *)(a))
#define B(a) (*(volatile s8 *)(a))
void CM_CDECL cm_fn_006b79a0(u32 club,u32 arg,u32 report,s8 mode,u32 match)
{
    u8 scratch[512];
    u32 i, p, q, slot, base, r;
    if (!club || !report || !arg) {
        u32 line;
        char *text;
        if (!club) {
            cm_fn_00944cff(0xa15708,0,0,scratch+256,scratch);
            line=0xc45;
            text=cm_fn_0090d130();
            cm_fn_00944c9f(0xae24d0,0x9870e8,text,scratch+256,scratch,line);
        } else {
            cm_fn_00944cff(0xa15708,0,0,scratch,scratch+256);
            line=!report ? 0xc4a : 0xc4f;
            text=cm_fn_0090d130();
            cm_fn_00944c9f(0xae24d0,0x9870e8,text,scratch,scratch+256,line);
        }
        cm_fn_005e8290((const char *)0x9870e0,(const char *)0xae24d0,0);
        CM_GLOBAL(u32,0xb67a34)=0;
        return;
    }
    if (!D(club+0xcf)) return;
    r=cm_fn_00600a50(club,0,1,0,0);
    if (!r && W(club+0x80)<0x128e) {
        r=(u32)cm_fn_0090cfc0(100);
        if (r) return;
    }
    r=cm_fn_006b7cf0(report,club);
    if ((u8)r && B(report+0x40) && B(report+0x41) &&
        !cm_fn_0053b900((u32 *)club)) {
        for (i=0;i<50;i++) {
            p=D(club+0xd7+i*4);
            if (!p) continue;
            q=cm_fn_004cf5a0(CM_GLOBAL(u32,0xae1050),p);
            if (!q || D(q+0x45)) continue;
            q=D(p+0x61);
            if (!q || W(q+0xb)<=0x109a || B(p+0x59)<=5 ||
                B(p+0x57)<=15 || B(p+0x58)<=10 || mode<=2) continue;
            r=(u32)cm_fn_0090cfc0((u32)(s32)mode);
            if (r) {
                cm_fn_005896d0((u32 *)p,(u32 *)club,(u32 *)arg,(u32)(s32)mode);
                return;
            }
        }
    }
    slot=match+0x1ce + (D(club)!=D(match) ? 0x625 : 0);
    for (i=0;i<20;i++,slot+=0x49) {
        r=D(slot-0x3e);
        if (r==0xffffffff || !B(slot)) continue;
        base=CM_GLOBAL(u32,0xae23bc);
        p=base+r*110;
        if (!D(p+0x61)) continue;
        if (p && (s32)D(p)>=(s32)CM_GLOBAL(u32,0xae2364)) continue;
        if (B(p+0x18)>20) continue;
        q=cm_fn_004cf5a0(CM_GLOBAL(u32,0xae1050),p);
        if (!q || D(q+0x45)) continue;
        if (!B(report+0x40) || !B(report+0x41)) continue;
        q=D(report+0x14);
        if (!q || q!=D(club+0x57)) continue;
        if (cm_fn_0053b900((u32 *)club)) continue;
        if (B(p+0x59)<=15) continue;
        r=(u32)cm_fn_0090cfc0(5);
        if (B(p+0x57)<=(s8)((u8)r+10)) continue;
        q=D(p+0x61);
        if (!q || W(q+0xb)>=0x109a) continue;
        if ((u16)cm_fn_007c3a40(p,club)) continue;
        cm_fn_00589bc0(p,club);
        if (!cm_fn_00547d60(p,club)) cm_fn_00547d90(p,club,1);
        return;
    }
}

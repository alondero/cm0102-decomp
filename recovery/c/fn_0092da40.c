/* Preferred-image VA 0092DA40, base00400000; pinned CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   All helpers remain original-code boundaries. Selected input observations are
   retained under scale-20261107ac/worldcup-exec; no constructor output claimed. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
extern u32 CM_CDECL cm_fn_004afd80(u32, u8);
extern u32 CM_CDECL cm_fn_0053b3d0(u32);
extern u8 CM_THISCALL cm_fn_005fa730(u8 *,s32,s16,s32 *,s32 *,s32);
extern void CM_CDECL cm_fn_005e8290(const char *,const char *,s32);
extern char * CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,void *,void *);
/* Small nonzero bounds at both sites never consume entry EAX/EDX. The
   register-prefix positions preserve the helper's physical stack interface. */
extern unsigned long long CM_CDECL __attribute__((regparm(2)))
    cm_fn_0090cfc0(u32,u32,u32);

#define NATION(g) do { id=D(g); base=D(0x00ae23a8); value=cm_fn_0053b3d0(base+id*290u); } while(0)
#define FIXED(g,off) do { NATION(g); table=D(self+0x14u); D(table+(off))=value; } while(0)
/* L+8 and L+108 are adjacent original 256-byte buffers. The first
   capacity failure reverses their order relative to every later site. */
#define ERROR_ROW(flip) do { \
    u8 *a=scratch+((flip)?0x108:8), *b=scratch+((flip)?8:0x108); \
    cm_fn_00944cff(0x00ad5888,0,0,a,b); \
    message=(u32)cm_fn_0090d130(); \
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,message,(u32)a,(u32)b,0x456); \
    cm_fn_005e8290((const char *)0x009870e0,(const char *)0x00ae24d0,0); \
    D(0x00b67a34)=0; \
} while(0)
/* cp records the original reload order after the byte type store. */
#define APPEND(g,cp,flip) do { \
    NATION(g); count=(s16)W(self+0x36u); capacity=(s16)W(self+0x38u); \
    if(count<capacity) { \
        table=D(self+0x14u); B(table+(u32)count*6u+4u)=6; \
        if(cp) { count=(s16)W(self+0x36u); table=D(self+0x14u); } \
        else { table=D(self+0x14u); count=(s16)W(self+0x36u); } \
        D(table+(u32)count*6u)=value; W(self+0x36u)=(u16)(W(self+0x36u)+1u); \
    } else { ERROR_ROW(flip); } \
} while(0)

u32 CM_THISCALL cm_fn_0092da40(u32 self)
{
    u32 table,value,id,base,left,right,message;
    s32 count,capacity;
    s8 pair_count;
    u8 scratch[0x208]; /* Pair outputs occupy L+0 and L+4; no initialization. */
    table=D(self+0x14u); /* Native0092DA4D precedes the count write. */
    W(self+0x36u)=0;
    B(table+4u)=2;
    id=D(0x009cf79c); base=D(0x00ae23d0);
    value=cm_fn_004afd80(base+id*107u,0);
    count=(s16)W(self+0x36u); table=D(self+0x14u);
    D(table+(u32)count*6u)=value;
    W(self+0x36u)=(u16)(W(self+0x36u)+1u);
    capacity=(s16)W(self+0x40u);
    id=D(0x009cf79c); base=D(0x00b5e134);
    pair_count=(s8)cm_fn_005fa730((u8 *)base,(s32)id,(s16)capacity,
                                (s32 *)(scratch+4),(s32 *)scratch,1);
    if(pair_count>0) {
        count=(s16)W(self+0x36u); table=D(self+0x14u);
        B(table+(u32)count*6u+4u)=1;
        id=*(u32 *)(scratch+4); base=D(0x00ae23a8);
        value=cm_fn_0053b3d0(base+id*290u);
        count=(s16)W(self+0x36u); table=D(self+0x14u);
        D(table+(u32)count*6u)=value;
        table=D(self+0x14u); W(self+0x36u)=(u16)(W(self+0x36u)+1u);
        count=(s16)W(self+0x36u); left=D(table); right=D(table+6u);
        if(left==right) W(self+0x36u)=(u16)(count-1);
        if(pair_count>1) {
            count=(s16)W(self+0x36u); B(table+(u32)count*6u+4u)=1;
            id=*(u32 *)scratch; base=D(0x00ae23a8);
            value=cm_fn_0053b3d0(base+id*290u);
            count=(s16)W(self+0x36u); table=D(self+0x14u);
            D(table+(u32)count*6u)=value;
            table=D(self+0x14u); W(self+0x36u)=(u16)(W(self+0x36u)+1u);
            count=(s16)W(self+0x36u); left=D(table); right=D(table+12u);
            if(left==right) W(self+0x36u)=(u16)(count-1);
        } else { FIXED(0x009cf298,12u); }
    } else {
        FIXED(0x009cf348,6u); FIXED(0x009cf344,12u); W(self+0x36u)=3;
    }
    FIXED(0x009cf360,0x12); FIXED(0x009cf488,0x18);
    FIXED(0x009cf218,0x1e); FIXED(0x009cf444,0x24);
    FIXED(0x009cf338,0x2a); FIXED(0x009cf400,0x30);
    FIXED(0x009cf480,0x36); FIXED(0x009cf27c,0x3c);
    FIXED(0x009cf4d8,0x42); FIXED(0x009cf3d8,0x48);
    FIXED(0x009cf3d0,0x4e); FIXED(0x009cf368,0x54);
    FIXED(0x009cf4f8,0x5a); FIXED(0x009cf2e4,0x60);
    FIXED(0x009cf45c,0x66); FIXED(0x009cf2c0,0x6c);
    FIXED(0x009cf268,0x72); FIXED(0x009cf51c,0x78);
    FIXED(0x009cf408,0x7e); FIXED(0x009cf244,0x84);
    FIXED(0x009cf2ac,0x8a); FIXED(0x009cf228,0x90);
    FIXED(0x009cf29c,0x96); FIXED(0x009cf420,0x9c);
    FIXED(0x009cf294,0xa2); FIXED(0x009cf484,0xa8);
    FIXED(0x009cf36c,0xae); FIXED(0x009cf458,0xb4);
    FIXED(0x009cf354,0xba);
    if(W(self+0x40u)!=2002) return 1;
    APPEND(0x009cf2a8,1,1);
    if((u32)cm_fn_0090cfc0(0,0,3)==0) {
        APPEND(0x009cf4f8,0,0); APPEND(0x009cf33c,1,0);
    } else if((u32)cm_fn_0090cfc0(0,0,2)==0) {
        APPEND(0x009cf4f8,0,0); APPEND(0x009cf3d0,1,0);
    } else {
        APPEND(0x009cf33c,0,0); APPEND(0x009cf3d0,1,0);
    }
    APPEND(0x009cf27c,0,0); APPEND(0x009cf400,1,0);
    APPEND(0x009cf460,0,0); APPEND(0x009cf4d8,1,0);
    APPEND(0x009cf480,0,0);
    return 1;
}

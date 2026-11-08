#include "cmre.h"
/* Preferred-image VA 007a09e0, base00400000; CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Original numeric callees remain boundaries. The qsort boundary takes the
   four actual pushed words; its recovered register-prefix tier is not composed.
   Scratch offsets are relative to L=ESP after SUB ESP,214h. */
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_0053b4f0(u32);
extern u8 CM_THISCALL cm_fn_005fa730(u8 *,s32,s16,s32 *,s32 *,s32);
extern void CM_CDECL cm_fn_00945333(u32,u32,u32,u32);
extern void CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,void *,void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern void CM_CDECL cm_fn_005e8290(const char *,const char *,u32);
typedef u32 word_7a09e0 __attribute__((aligned(1),may_alias));
typedef u16 half_7a09e0 __attribute__((aligned(1),may_alias));
#define D(a) (*(volatile word_7a09e0 *)(u32)(a))
#define H(a) (*(volatile half_7a09e0 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_THISCALL cm_fn_007a09e0(u8 *object)
{
    u8 scratch[0x214];
    u32 self=(u32)object,temporary,result,nation,team,category,id,value;
    u32 count=0,iteration=0,index,selected=0,first,second,line;
    s8 initial;
    u16 year=H(self+0x40u);
    D(self+0xf2u)=16;
    if (year==2000u) {
        result=cm_fn_00944e46(64);
        D(self+0xeeu)=result;
        if (!result) {
            first=(u32)scratch+0x114u;second=(u32)scratch+0x14u;line=0x337u;
            goto error;
        }
#define INITIAL(global,offset) \
        id=D(global);nation=D(0xae23a8u)+id*0x122u; \
        value=cm_fn_0053b4f0(nation);result=D(self+0xeeu);D(result+offset)=value
        INITIAL(0x9cf224u,0u); INITIAL(0x9cf360u,4u);
        INITIAL(0x9cf400u,8u); INITIAL(0x9cf33cu,12u);
        INITIAL(0x9cf4f8u,16u); INITIAL(0x9cf2b8u,20u);
        INITIAL(0x9cf27cu,24u); INITIAL(0x9cf380u,28u);
        INITIAL(0x9cf480u,32u); INITIAL(0x9cf36cu,36u);
        INITIAL(0x9cf260u,40u); INITIAL(0x9cf470u,44u);
        INITIAL(0x9cf294u,48u); INITIAL(0x9cf3d8u,52u);
        INITIAL(0x9cf484u,56u); INITIAL(0x9cf488u,60u);
#undef INITIAL
        return 1;
    }
    temporary=cm_fn_00944e46(D(0xae2350u)*4u);
    if (!temporary) {
        first=(u32)scratch+0x14u;second=(u32)scratch+0x114u;line=0x356u;
        goto error;
    }
    year=H(self+0x40u);
    value=D(0x9cf7a0u);result=D(0xb5e134u);
    initial=(s8)cm_fn_005fa730((u8 *)result,(s32)value,(s16)year,
        (s32 *)(scratch+0xcu),(s32 *)(scratch+0x10u),1);
    if (initial>0) {
        index=(u32)scratch+0xcu;
        do {
            id=D(index);nation=D(0xae23a8u)+id*0x122u;
            value=cm_fn_0053b4f0(nation);
            result=(u32)(s32)(s16)count;count++;
            index+=4u;D(temporary+result*4u)=value;
        } while ((s16)count<(s16)initial);
    }
    value=D(0xae2350u);nation=D(0xae23a8u);
    if ((s32)value>0) {
        do {
            id=D(nation);
            if (id==D(0x9cf45cu)) goto next_nation;
            if (id==D(0x9cf2e4u)) goto next_nation;
            if (id==D(0x9cf510u)) goto next_nation;
            if (id==D(0x9cf2dcu)) goto next_nation;
            if (id==D(0x9cf3e4u)) goto next_nation;
            if (B(nation+0x75u)==6) goto next_nation;
            if (id==D(0x9cf408u)) goto next_nation;
            if (id==D(0x9cf4a8u)) goto next_nation;
            result=0;
            if (initial>0) {
                index=temporary;
                for (value=(u32)(s32)initial;value;value--) {
                    team=D(index);
                    if (D(team+0x53u)==nation) result=1;
                    index+=4u;
                }
                if (result) goto next_nation;
            }
            value=cm_fn_0053b4f0(nation);
            result=(u32)(s32)(s16)count;count++;
            D(temporary+result*4u)=value;
next_nation:
            value=D(0xae2350u);
            iteration++;nation+=0x122u;
        } while ((s32)(s16)iteration<(s32)value);
    }
    cm_fn_00945333(temporary+4u,(u32)((s32)(s16)count-1),4,0x4b07d0u);
    result=cm_fn_00944e46(D(self+0xf2u)<<2);
    D(self+0xeeu)=result;
    if (!result) {
        first=(u32)scratch+0x14u;second=(u32)scratch+0x114u;line=0x38fu;
        goto error;
    }
#define SELECT(limit,global) \
    index=0; \
    if ((s16)count>0) { \
        do { \
            if ((s16)selected>=limit) break; \
            team=D(temporary+(u32)(s32)(s16)index*4u); \
            nation=D(team+0x53u); \
            if (nation) { \
                category=D(nation+0x71u); \
                if (category) { \
                    value=D(category); \
                    if (value==D(global)) { \
                        result=D(self+0xeeu);value=(u32)(s32)(s16)selected;selected++; \
                        D(result+value*4u)=team; \
                    } \
                } \
            } \
            index++; \
        } while ((s16)index<(s16)count); \
    }
    SELECT(5,0x9cfa10u); SELECT(8,0x9cfa08u); SELECT(10,0x9cfa14u);
    SELECT(12,0x9cfa1cu); SELECT(15,0x9cfa0cu); SELECT(16,0x9cfa18u);
#undef SELECT
    cm_fn_009452ca(temporary);
    result=cm_fn_00944e46(D(self+0xf2u)<<2);
    if (!result) {
        first=(u32)scratch+0x14u;second=(u32)scratch+0x114u;line=0x3ddu;
        goto error;
    }
#define COPY(dst,src) temporary=D(self+0xeeu);value=D(temporary+src);D(result+dst)=value
    COPY(0u,0u); COPY(4u,20u); COPY(8u,36u); COPY(12u,52u);
    COPY(16u,4u); COPY(20u,24u); COPY(24u,40u); COPY(28u,56u);
    COPY(32u,8u); COPY(36u,28u); COPY(40u,44u); COPY(44u,60u);
    COPY(48u,12u); COPY(52u,32u); COPY(56u,48u); COPY(60u,16u);
#undef COPY
    temporary=D(self+0xeeu);cm_fn_009452ca(temporary);
    D(self+0xeeu)=result;
    return 1;
error:
    cm_fn_00944cff(0xa6a360u,0,0,(void *)first,(void *)second);
    value=(u32)cm_fn_0090d130();
    cm_fn_00944c9f(0xae24d0u,0x9870e8u,value,first,second,line);
    cm_fn_005e8290((const char *)0x9870e0u,(const char *)0xae24d0u,0);
    D(0xb67a34u)=0;
    return 0;
}

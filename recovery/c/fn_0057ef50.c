/* WIP: case 2 virtual slot40 target0048CE10 RET0 leaves five arguments live;
   C thiscall assumes RET20. Two failed invocations used, three remain. */
#include "cmre.h"

/* Preferred-image VA0057EF50/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS0/RET0/full EAX: the direct caller returns it unchanged.
   L=afterSUB420. Scratch offsets retain the original alias relationships;
   L18/L1E are virtual-call outputs, not initialized by this routine.
   Native6826D0 leaves EDX=59*index. Native522E00 reads its year/count/
   round count as WORDs and its flag arguments as BYTEs. */
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
#define SXB(a) ((u32)(s32)(s8)B(a))
#define SXW(a) ((u32)(s32)(s16)W(a))
extern u32 CM_THISCALL cm_fn_006826d0(u32,u32);
extern u32 CM_THISCALL cm_fn_0086c1d0(u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00775000(u32,u32);
extern u32 CM_THISCALL cm_fn_0086bdd0(u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_CDECL cm_fn_0053b3d0(u32);
extern u32 CM_THISCALL cm_fn_00522e00(u32,u32,s16,u32,s16,u32,u32,
                                     u16,u8,u8,u16,u8,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0051c800(u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

typedef u32 (CM_THISCALL *slot5)(u32,u32,u32,u32,u32,u32);
typedef u32 (CM_THISCALL *slot4)(u32,u32,u32,u32,u32);

u32 CM_THISCALL cm_fn_0057ef50(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    volatile u8 scratch[0x420];
    u32 l=(u32)scratch, a,c,d,object,index,offset,array,table;
    u32 first,second,owner,target,allocation,flag,year,rounds,count;
    u32 filename,extension,line,version;
    u8 limit,probe;
    static const u32 nations[8]={0x009cf45c,0x009cf2e4,0x009cf35c,
        0x009cf2c0,0x009cf474,0x009cf4f0,0x009cf2dc,0x009cf4dc};

    a=CM_FS(volatile u32,0);
    frame.state=0xffffffffu;
    frame.handler=0x009588fcu;
    frame.prev=a;
    CM_FS(volatile u32,0)=(u32)&frame;
    if (B(self+0x100u)==0xffu) {
        a=(a&0xffffff00u)|B(self+0xffu);
        c=0xffffffffu;
        D(l+8u)=c;
        if ((s8)a < -1) goto done;
        do {
            if (c==0xffffffffu) object=self;
            else { a=D(self+0xcu); object=D(a+c*4u); }
            index=0;
            if ((s16)W(object+0x3eu)>0) {
                offset=0;
                do {
                    d=D(object+0xb1u);
                    a=d+offset;
                    probe=B(d+offset+0x37u);
                    if (probe!=7) {
                        c=D(self+4u);
                        d=D(a);
                        table=D(0x00dd7f00u);
                        cm_fn_0086c1d0(table,d,c,0x41c,0x64);
                        a=D(object+0xb1u);
                        c=D(a+offset);
                        d=D(c+0x53u);
                        c=D(0x00b74318u);
                        cm_fn_00775000(c,d);
                        c=D(l+8u);
                    }
                    a=SXW(object+0x3eu);
                    ++index;
                    offset+=0x3bu;
                } while ((s32)index<(s32)a);
            }
            d=SXB(self+0xffu);
            ++c;
            D(l+8u)=c;
        } while ((s32)c<=(s32)d);
        goto done;
    }

    probe=B(self+0xf6u);
    limit=B(self+0xffu);
    D(l+8u)=0;
    c=0xffffffffu;
    D(l+4u)=c;
    limit=(s8)limit < -1 ? 0 : 1;
    probe=probe==1 ? 3 : 1;
    B(l+3u)=probe;
    if (limit!=0) {
        do {
            if (c==0xffffffffu) object=self;
            else { d=D(self+0xcu); object=D(d+c*4u); }
            index=0;
            if ((s16)W(object+0x3eu)>0) {
                do {
                    limit=0;
                    if ((s8)probe>0) {
                        do {
                            c=SXB(self+0x100u);
                            d=D(self+0xcu);
                            c=D(d+c*4u);
                            a=cm_fn_006826d0(c,(u32)(s32)(s8)limit);
                            D(l+0x10u)=a;
                            a=cm_fn_006826d0(object,index);
                            c=D(l+0x10u);
                            a=D(a);
                            if (a==D(c)) {
                                a=cm_fn_006826d0(object,index);
                                B(a+0x37u)=7;
                                d=(index*59u&0xffffff00u)|B(self+0x100u);
                                table=D(self);
                                a=cm_fn_006826d0(object,index);
                                a=D(a);
                                target=D(table+0x40u);
                                ((slot5)target)(self,a,7,d,0,0);
                                a=D(l+8u);
                                B(object+0xbeu)=2;
                                ++a;
                                B(object+0xbfu)=0;
                                D(l+8u)=a;
                                break;
                            }
                            probe=B(l+3u);
                            ++limit;
                        } while ((s8)limit<(s8)probe);
                    }
                    probe=B(l+3u);
                    c=SXW(object+0x3eu);
                    ++index;
                } while ((s32)index<(s32)c);
                c=D(l+4u);
            }
            object=D(l+8u);
            d=(u32)(s32)(s8)probe;
            if (object==d) break;
            d=SXB(self+0xffu);
            ++c;
            D(l+4u)=c;
        } while ((s32)c<=(s32)d);
    }
    probe=B(self+0xf6u);
    a=probe==1 ? 6u : 8u;
    D(l+0xcu)=a;
    array=cm_fn_00944e46((u32)(s32)(s16)a*4u);
    if (array==0) {
        filename=l+0x220u; extension=l+0x20u; line=0x441;
        goto error;
    }
    d=SXB(self+0x100u);
    a=D(self+0xcu);
    c=SXW(l+0xcu);
    object=D(a+d*4u);
    a=SXW(object+0x3eu);
    index=a-c;
    if ((s32)index<(s32)a) {
        D(l+4u)=array;
        do {
            a=cm_fn_006826d0(object,index);
            c=D(a);
            a=D(l+4u);
            ++index;
            D(a)=c;
            a+=4u;
            d=SXW(object+0x3eu);
            D(l+4u)=a;
        } while ((s32)index<(s32)d);
    }
    probe=B(self+0xffu);
    a=0xffffffffu;
    D(l+8u)=a;
    if ((s8)probe>=-1) {
        do {
            if (a==0xffffffffu) object=self;
            else { c=D(self+0xcu); object=D(c+a*4u); }
            offset=0;
            D(l+0x10u)=0;
            if ((s16)W(object+0x3eu)>0) {
                do {
                    a=SXW(l+0xcu);
                    D(l+4u)=0;
                    if ((s32)a>0) {
                        d=D(object+0xb1u);
                        a=array;
                        c=offset+d;
                        do {
                            d=D(c);
                            if (D(a)==d) goto found;
                            d=D(l+4u);
                            a+=4u;
                            ++d;
                            D(l+4u)=d;
                            d=SXW(l+0xcu);
                        } while ((s32)D(l+4u)<(s32)d);
                    }
                    a=D(object+0xb1u)+offset;
                    if (B(a+0x37u)!=7) {
                        c=D(self+4u);
                        d=D(a);
                        table=D(0x00dd7f00u);
                        cm_fn_0086c1d0(table,d,c,0x41c,0x64);
                        a=D(object+0xb1u);
                        c=D(offset+a);
                        d=D(c+0x53u);
                        c=D(0x00b74318u);
                        cm_fn_00775000(c,d);
                    }
                    goto after_record;
found:
                    B(c+0x37u)=1;
                    c=D(object+0xb1u);
                    a=D(self+4u);
                    d=D(offset+c);
                    c=D(0x00dd7f00u);
                    cm_fn_0086bdd0(c,d,a,0,0xa0,0x1e);
after_record:
                    a=D(l+0x10u);
                    offset+=0x3bu;
                    c=SXW(object+0x3eu);
                    ++a;
                    D(l+0x10u)=a;
                } while ((s32)a<(s32)c);
                a=D(l+8u);
            }
            d=SXB(self+0xffu);
            ++a;
            D(l+8u)=a;
        } while ((s32)a<=(s32)d);
    }
    c=SXW(object+0x3eu);
    a=SXW(l+0xcu);
    c-=a;
    index=0;
    if ((s32)c>0) {
        D(l+4u)=0;
        do {
            a=D(object+0xb1u);
            c=D(l+4u);
            table=D(self);
            a=D(c+a);
            target=D(table+0x68u);
            ((slot5)target)(self,a,0xffffffffu,1,1,1);
            a=D(l+4u);
            ++index;
            c=SXW(object+0x3eu);
            a+=0x3bu;
            D(l+4u)=a;
            a=SXW(l+0xcu);
            c-=a;
        } while ((s32)index<(s32)c);
    }
    table=D(self);
    a=l+0x1eu;
    second=a;
    a=(a&0xffffff00u)|B(self+0x101u);
    first=l+0x18u;
    target=D(table+0x3cu);
    object=((slot4)target)(self,a,first,second,0);
    c=D(0x00dd6eccu);
    year=W(0x00ae2c92u);
    a=D(0x00dd6ec8u);
    D(l+0x10u)=a;
    D(l+0x14u)=c;
    c=W(l+0x12u);
    flag=0;
    if (c>year) flag=1;
    else if (c==year) flag=(s16)a>(s16)W(0x00ae2c90u);
    if (flag!=0) {
        for (index=0;index<8;++index) {
            a=D(nations[index]);
            d=D(0x00ae23a8u);
            a=cm_fn_0053b3d0(d+a*290u);
            D(array+index*4u)=a;
        }
        B(object+0xeu)=(u8)(B(object+0xeu)|2u);
    }
    allocation=cm_fn_00944cf1(0xb2);
    D(l+0x10u)=allocation;
    frame.state=flag!=0 ? 0u : 1u;
    if (allocation!=0) {
        probe=B(self+0x101u);
        year=W(self+0x40u);
        owner=D(self+4u);
        rounds=W(l+0x18u);
        count=W(l+0xcu);
        a=cm_fn_00522e00(allocation,self,(s16)count,array,(s16)rounds,
                           owner,object,(u16)year,probe,4,0,0x14,flag,0,0,0);
    } else a=0;
    d=SXB(self+0x101u);
    c=D(self+0xcu);
    frame.state=0xffffffffu;
    D(c+d*4u)=a;
    cm_fn_009452ca(array);
    cm_fn_009452ca(object);
    a=SXB(self+0x101u);
    c=D(self+0xcu);
    c=D(c+a*4u);
    if (c==0) {
        filename=l+0x320u; extension=l+0x120u; line=0x4a3;
        goto error;
    }
    cm_fn_0051c800(c,0);
    a=SXB(self+0x101u);
    D(self+0x30u)=a;
    goto done;
error:
    cm_fn_00944cff(0x009c4674u,0,0,filename,extension);
    version=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u,0x009870e8u,version,filename,extension,line);
    a=cm_fn_005e8290(0x009870e0u,0x00ae24d0u,0);
    D(0x00b67a34u)=0;
done:
    CM_FS(volatile u32,0)=frame.prev;
    return a;
}

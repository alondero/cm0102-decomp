#include "cmre.h"
/* Preferred-image VA0085dfc0/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS0/full qsort/error EAX; scratch230 countBYTEL3/indexL4/IDsL8,
   Lc/diagnosticsL30,L130. The exclusion-ID scan does not exclude a row:
   every exit falls through to insertion. Keep stale counts if old buffer
   is null, paired-byte string reads, wrapping allocations and all reloads. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern u8 CM_THISCALL cm_fn_005fa7d0(u32,u32,u32,u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_00945667(u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);
extern u32 CM_CDECL __attribute__((regparm(1))) cm_fn_00945333(u32,u32,u32,u32,u32);

u32 CM_THISCALL cm_fn_0085dfc0(u32 self)
{
    u8 scratch[0x230];
    u32 l=(u32)scratch,eax,manager,key,offset,limit,stadium,pointer,club;
    u32 p,q,index,id,count,capacity,base,value,other,version,line;
    u8 total,found,a,b,flag;
#define REPORT(n) do { \
    cm_fn_00944cff(0x00a835b0,0,0,l+0x30,l+0x130); \
    version=cm_fn_0090d130(); \
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,l+0x30,l+0x130,n); \
    eax=cm_fn_005e8290(0x009870e0,0x00ae24d0,0);D(0x00b67a34)=0; \
} while(0)
    eax=D(self+0xc);B(l+3)=0;
    if(eax!=0){
        cm_fn_009452ca(eax);D(self+0xc)=0;D(self+0x14)=0;D(self+0x10)=0;
    }
    manager=D(0x00b5e134);
    if(manager==0){
        cm_fn_00944cff(0x00a835b0,0,0,l+0x130,l+0x30);
        version=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,l+0x130,l+0x30,0x858);
        cm_fn_005e8290(0x009870e0,0x00ae24d0,0);D(0x00b67a34)=0;
    }else{
        key=D(0x009cf79c);total=cm_fn_005fa7d0(manager,key,l+8,l+0xc);
        offset=(u32)(s32)(s8)total*4u;key=D(0x009cf7a4);manager=D(0x00b5e134);
        found=cm_fn_005fa7d0(manager,key,l+8+offset,l+0xc+offset);total=(u8)(total+found);
        offset=(u32)(s32)(s8)total*4u;key=D(0x009cf7ac);manager=D(0x00b5e134);
        found=cm_fn_005fa7d0(manager,key,l+8+offset,l+0xc+offset);total=(u8)(total+found);
        B(l+3)=total;
    }
    limit=D(0x00ae2358);stadium=D(0x00ae23b0);D(l+4)=0;
    if((s32)limit>0){
        do{
            pointer=D(stadium+0x38);
            if(pointer!=0){
                club=D(pointer+0x1f);
                if(club!=0 && D(club+0x71)!=0){
                    p=stadium+4;q=0x00a835ec;
                    for(;;){
                        a=B(p);b=B(q);
                        if(a!=b){eax=(a<b)?0xffffffffu:1;break;}
                        if(a==0){eax=0;break;}
                        a=B(p+1);b=B(q+1);
                        if(a!=b){eax=(a<b)?0xffffffffu:1;break;}
                        p+=2;q+=2;
                        if(a==0){eax=0;break;}
                    }
                    if(eax!=0){
                        flag=B(club+0x11c);
                        if(flag==0 || (s32)D(stadium+0x3c)>=5000){
                            total=B(l+3);index=0;
                            if((s8)total>0){
                                id=D(club);
                                do{
                                    if(id==D(l+8+(u32)(s32)(s8)(u8)index*4u) &&
                                       (s32)D(stadium+0x3c)<5000) break;
                                    index=(u8)(index+1u);
                                }while((s8)(u8)index<(s8)total);
                            }
                            eax=D(self+0xc);
                            if(eax==0){
                                eax=(u32)cm_fn_00944e46(0x1e0);D(self+0xc)=eax;
                                if(eax==0){line=0x89c;goto error;}
                                D(self+0x14)=30;
                            }else{
                                capacity=D(self+0x14);count=D(self+0x10);
                                if(count==capacity){
                                    eax=(u32)cm_fn_00945667(eax,(capacity+30u)*16u);
                                    D(self+0xc)=eax;
                                    if(eax==0){line=0x8ac;goto error;}
                                    D(self+0x14)=D(self+0x14)+30u;
                                }
                            }
                            count=D(self+0x10);base=D(self+0xc);id=D(club);
                            D(base+count*16u)=id;
                            pointer=D(club+0x71);count=D(self+0x10);base=D(self+0xc);value=D(pointer);
                            D(base+count*16u+4)=value;
                            count=D(self+0x10);base=D(self+0xc);D(base+count*16u+0xc)=stadium;
                            other=D(club+0x80);count=D(self+0x10);base=D(self+0xc);
                            D(base+count*16u+8)=(other==stadium);
                            count=D(self+0x10);D(self+0x10)=count+1u;
                        }
                    }
                }
            }
            index=D(l+4);limit=D(0x00ae2358);++index;stadium+=78u;D(l+4)=index;
        }while((s32)(s16)(u16)index<(s32)limit);
    }
    count=D(self+0x10);base=D(self+0xc);
    eax=cm_fn_00945333(count,base,count,16,0x0085e2f0);B(self+0x18)=1;
    return eax;
error:
    REPORT(line);
#undef REPORT
    return eax;
}

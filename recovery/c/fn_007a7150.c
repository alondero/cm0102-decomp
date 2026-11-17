#include "cmre.h"
#include "cmre_x87.h"
/* Original instruction VAs below refer to CM0102 3.9.68,
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   007a7168 and 007a72ff: RET 4, one stack argument. */
/* Each original call pushes one bound (007a71b3, 007a721b,
   007a72ba, 007a72cd, 007a72f1) and removes four bytes.
   Original 0090cfc2 reads that first stack argument. */
extern unsigned long long CM_CDECL cm_fn_0090cfc0(u32);
#define R32(p,o) (*(volatile u32 *)((u32)(p)+(o)))
#define R16(p,o) (*(volatile s16 *)((u32)(p)+(o)))
#define R8(p,o) (*(volatile s8 *)((u32)(p)+(o)))
u32 CM_STDCALL cm_fn_007a7150(volatile u32 home) {
 /* With entry ESP=S, 007a7150/53 and 007a71a7 leave ESP=S-16.
    007a71ff and 007a7244 write DWORD [ESP+14h]=S+4; after
    007a7248 PUSH EBX, 007a7261 writes DWORD [ESP+18h]=S+4.
    Preserve that caller-owned home, separately from ESI's cached pointer
    loaded at 007a7154. FILD reloads it at 007a7203/7249/7267.
    VAs and pinned binary SHA-256 are specified above. */
 u32 p=home;
 u32 a, d, q, t, v, b; s32 x,y; s16 z;
 if (R32(p,0x69)) return 0xffffd8f0u;
 a=R32(p,0); d=CM_GLOBAL(volatile u32,0xae2d0c);
 if (*(volatile u8 *)(d+a*79u+11u)&0x30) return 0xffffd8f0u;
 x=R8(p,0x18); if(x<31) return 0xffffd8f0u;
 a=(u32)((x-31)*150); q=(u32)cm_fn_0090cfc0(a);
 a=R32(p,0x39);
 if(a && (z=R16(a,0x80))!=0 && (a=R32(p,0x61))!=0) q+=(u32)((R16(a,11)/(s32)z)*250);
 a=R32(p,0x61);
 if(a) {
  x=R8(a,0x2f)/2;
  home=(u32)x; x=(s32)home;
  /* 007a7211 _CIpow, 007a7216 __ftol */
  { long long r=(long long)cm_x87_CIpow((long double)x,(long double)CM_GLOBAL(double,0x968930)); a=(u32)r; d=(u32)((unsigned long long)r>>32); }
  q+=(u32)cm_fn_0090cfc0(a);
  a=R32(p,0x61); if(R16(a,13)>7500) q+=100; if(R16(a,11)>7500) q+=50;
 }
 x=R8(p,0x5b);
 home=(u32)x; x=(s32)home;
 /* 007a7253 _CIpow, 007a7258 __ftol */
 { long long r=(long long)cm_x87_CIpow((long double)x,(long double)CM_GLOBAL(double,0x968950)); b=(u32)r; d=(u32)((unsigned long long)r>>32); }
 x=R8(p,0x57);
 home=(u32)x; x=(s32)home;
 /* 007a7271 _CIpow, 007a7276 __ftol */
 { long long r=(long long)cm_x87_CIpow((long double)x,(long double)CM_GLOBAL(double,0x968950)); a=(u32)r; d=(u32)((unsigned long long)r>>32); }
 b+=a; x=R8(p,0x5a); y=R8(p,0x5d); x=(x+y)*3; y=R8(p,0x58); x+=(R8(p,0x59)+y)*2; d=(u32)(s32)R8(p,0x56); x+=(s32)d; q+=(u32)x*5u+b;
 a=R32(p,0x39);
 if(a) { if(!R32(a,0xcf)) q+=75; }
 else { unsigned long long r; a=*(volatile u8 *)(p+0x22); d=a*a; q+=d; r=cm_fn_0090cfc0(10000); a=(u32)r; t=R32(p,0x61); d=(u32)(s32)R16(t,13); if((s32)d>(s32)a) q+=q; }
 v=(u32)cm_fn_0090cfc0(q); return v;
}

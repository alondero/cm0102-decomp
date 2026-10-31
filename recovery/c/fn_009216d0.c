/* Preferred-image VA009216D0, base00400000; target SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS0/RET0; conservative full EAX from the final original call.
   Every row keeps the original two volatile global loads in their original
   order. Original922350, including its unresolved scratch provenance, stays
   an explicit boundary; this source supplies no missing initialization. */
#include "cmre.h"
#define D(p) CM_GLOBAL(volatile u32,(u32)(p))
extern u32 CM_THISCALL cm_fn_00922350(u32,u32,u32,u32,u32,u32,u32,u32,u32);
typedef u32 (CM_THISCALL *slot_009216d0)(u32);
#define G218 0x9cf218u
#define G254 0x9cf254u
#define G260 0x9cf260u
#define G294 0x9cf294u
#define G29C 0x9cf29cu
#define G2D4 0x9cf2d4u
#define G420 0x9cf420u
#define G428 0x9cf428u
#define G4F4 0x9cf4f4u
#define G504 0x9cf504u
/* The second row global is loaded first at every native call site. */
#define R(p2,p3,p4,g5,p6,g7,p8) do { \
    a=D(g7); b=D(g5); \
    result=cm_fn_00922350(self,0xffffffffu,p2,p3,p4,b,p6,a,p8); \
} while(0)

u32 CM_THISCALL cm_fn_009216d0(u32 self)
{
    u32 table,target,a,b,result;
    table=D(self); target=D(table+0x5c);
    ((slot_009216d0)target)(self);
    D(self+0x45)=1;
    R(0x1c,2,0x7d0,G29C,0,G260,0);
    R(0x1d,2,0x7d0,G2D4,2,G504,0);
    R(0x1d,2,0x7d0,G4F4,1,G254,0);
    R(0x1d,2,0x7d0,G428,2,G420,0);
    R(0x1d,2,0x7d0,G218,4,G294,1);
    R(0x1a,3,0x7d0,G254,1,G29C,1);
    R(0x1a,3,0x7d0,G420,1,G4F4,0);
    R(0x1a,3,0x7d0,G504,0,G218,4);
    R(0x1a,3,0x7d0,G294,1,G428,1);
    R(0x1a,3,0x7d0,G260,3,G2D4,2);
    R(3,5,0x7d0,G4F4,2,G294,1);
    R(3,5,0x7d0,G420,3,G2D4,1);
    R(4,5,0x7d0,G218,1,G254,0);
    R(4,5,0x7d0,G428,0,G260,1);
    R(4,5,0x7d0,G29C,3,G504,0);
    R(0x1c,5,0x7d0,G504,4,G254,2);
    R(0x1c,5,0x7d0,G260,1,G4F4,1);
    R(0x1d,5,0x7d0,G2D4,2,G428,1);
    R(0x1d,5,0x7d0,G294,3,G420,1);
    R(0x1d,5,0x7d0,G29C,1,G218,3);
    R(0x12,6,0x7d0,G4F4,3,G504,1);
    R(0x12,6,0x7d0,G420,2,G260,1);
    R(0x13,6,0x7d0,G254,1,G294,0);
    R(0x13,6,0x7d0,G218,2,G2D4,0);
    R(0x13,6,0x7d0,G428,0,G29C,1);
    R(0x19,6,0x7d0,G2D4,0,G29C,0);
    R(0x19,6,0x7d0,G504,0,G294,2);
    R(0x1a,6,0x7d0,G4F4,0,G428,0);
    R(0x1a,6,0x7d0,G260,3,G218,1);
    R(0x1b,6,0x7d0,G254,0,G420,0);
    R(0xf,7,0x7d0,G29C,1,G4F4,0);
    R(0xf,7,0x7d0,G294,3,G260,0);
    R(0x10,7,0x7d0,G2D4,2,G254,0);
    R(0x10,7,0x7d0,G218,1,G420,1);
    R(0x10,7,0x7d0,G428,1,G504,0);
    R(2,8,0x7d0,G420,3,G504,0);
    R(2,8,0x7d0,G294,0,G29C,1);
    R(3,8,0x7d0,G4F4,4,G2D4,0);
    R(3,8,0x7d0,G428,1,G218,2);
    R(3,8,0x7d0,G260,5,G254,0);
    R(7,9,0x7d0,G29C,0,G420,2);
    R(8,9,0x7d0,G504,0,G260,6);
    R(8,9,0x7d0,G254,1,G428,0);
    R(8,9,0x7d0,G2D4,1,G294,0);
    R(8,9,0x7d0,G218,2,G4F4,1);
    R(0xf,10,0x7d0,G260,1,G29C,0);
    R(0xf,10,0x7d0,G254,0,G4F4,0);
    R(0xf,10,0x7d0,G504,1,G2D4,2);
    R(0xf,10,0x7d0,G420,5,G428,1);
    R(0xf,10,0x7d0,G294,0,G218,2);
    R(0x1b,2,0x7d1,G29C,2,G254,0);
    R(0x1b,2,0x7d1,G428,3,G294,1);
    R(0x1c,2,0x7d1,G2D4,1,G260,0);
    R(0x1c,2,0x7d1,G4F4,0,G420,1);
    R(0x1c,2,0x7d1,G218,5,G504,0);
    R(0x18,3,0x7d1,G2D4,2,G420,1);
    R(0x18,3,0x7d1,G504,2,G29C,2);
    R(0x18,3,0x7d1,G294,0,G4F4,1);
    R(0x19,3,0x7d1,G254,3,G218,3);
    R(0x19,3,0x7d1,G260,1,G428,1);
    R(2,5,0x7d1,G420,1,G294,0);
    R(2,5,0x7d1,G428,1,G2D4,2);
    R(3,5,0x7d1,G254,5,G504,0);
    R(3,5,0x7d1,G218,3,G29C,0);
    R(1,6,0x7d1,G4F4,1,G260,0);
    R(0xe,7,0x7d1,G2D4,0,G218,2);
    R(0xf,7,0x7d1,G504,2,G4F4,0);
    R(0xf,7,0x7d1,G294,2,G254,2);
    R(0x10,7,0x7d1,G260,2,G420,0);
    R(0x11,7,0x7d1,G29C,0,G428,1);
    R(5,8,0x7d1,G294,0,G504,2);
    R(5,8,0x7d1,G428,0,G4F4,2);
    R(6,8,0x7d1,G420,5,G254,1);
    R(6,8,0x7d1,G218,2,G260,1);
    R(6,8,0x7d1,G29C,0,G2D4,0);
    R(6,9,0x7d1,G4F4,0xffffffffu,G29C,0xffffffffu);
    R(6,9,0x7d1,G260,0xffffffffu,G294,0xffffffffu);
    R(6,9,0x7d1,G254,0xffffffffu,G2D4,0xffffffffu);
    R(6,9,0x7d1,G420,0xffffffffu,G218,0xffffffffu);
    R(6,9,0x7d1,G504,0xffffffffu,G428,0xffffffffu);
    R(7,10,0x7d1,G218,0xffffffffu,G428,0xffffffffu);
    R(7,10,0x7d1,G254,0xffffffffu,G260,0xffffffffu);
    R(7,10,0x7d1,G2D4,0xffffffffu,G4F4,0xffffffffu);
    R(7,10,0x7d1,G29C,0xffffffffu,G294,0xffffffffu);
    R(7,10,0x7d1,G504,0xffffffffu,G420,0xffffffffu);
    R(0xe,10,0x7d1,G4F4,0xffffffffu,G218,0xffffffffu);
    R(0xe,10,0x7d1,G260,0xffffffffu,G504,0xffffffffu);
    R(0xe,10,0x7d1,G420,0xffffffffu,G29C,0xffffffffu);
    R(0xe,10,0x7d1,G294,0xffffffffu,G2D4,0xffffffffu);
    R(0xe,10,0x7d1,G428,0xffffffffu,G254,0xffffffffu);
    return result;
}

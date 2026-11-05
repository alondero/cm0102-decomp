#include "cmre.h"
/* Preferred-image VA 008b2be0, base 00400000, CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Full EAX is observable at 008b38ab; see retained native return proof. */
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_008b4b20(u32, u32, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))
#define S(a) ((int)(signed char)B(a))
#define W(a) (*(volatile short *)(u32)(a))
#define CLAMP(v,lo,hi) ((v)<(lo)?(lo):((v)>(hi)?(hi):(v)))
#define UPDATE(off,delta,lo,hi) do { t=S(profile+(off))+(delta); B(profile+(off))=(u8)CLAMP(t,lo,hi); } while (0)

u32 CM_THISCALL cm_fn_008b2be0(u32 self, u32 player, u32 changes)
{
    /* L is ESP immediately following SUB ESP,20c. Both error buffers,
       saved self, counters and the output slot share their native offsets. */
    volatile u8 scratch[0x20c];
    u32 l=(u32)scratch, profile, id, limit, table, base, raw, index;
    u32 first, second, line, text, result, target;
    int old, delta, six, t, next, saved, key, grade, offset;
    u8 input, packed, again, count;
    D(l+4)=self;
    B(l+3)=0;
    B(l+2)=0;
    if (player==0) {
        first=l+0x10c; second=l+0xc; line=0x4b3;
        goto failure;
    }
    profile=D(player+0x61);
    if (profile==0) {
        first=l+0xc; second=l+0x10c; line=0x4ba;
        goto failure;
    }
    id=D(player);
    limit=D(0x00ae2364);
    if ((int)id >= (int)limit) return limit;
    id=D(profile);
    base=D(self+0x651d);
    table=base+id*6u;

    input=B(changes+3);
    if (input) {
        old=S(table+4)>>4;
        delta=(signed char)(((int)(signed char)input+old)/2-old/2);
        if (delta) {
            UPDATE(0x22,delta,1,20);
            six=delta*6;
            UPDATE(0x23,six,-125,125);
            UPDATE(0x26,six,-125,125);
            UPDATE(0x29,delta,1,20);
            UPDATE(0x2b,six,-125,125);
            UPDATE(0x37,six,-125,125);
            UPDATE(0x3e,six,-125,125);
            t=S(profile+0x40)+delta;
            saved=CLAMP(t,1,20);
            next=S(profile+0x41)+six;
            B(profile+0x40)=(u8)saved;
            B(profile+0x41)=(u8)CLAMP(next,-125,125);
        }
        packed=B(table+4); again=B(changes+3);
        B(table+4)=(u8)(((packed&0xf0)+(again<<4))^(packed&15));
    }
    input=B(changes);
    if (input) {
        packed=B(table+3);
        old=((signed char)(packed<<4))>>4;
        delta=(signed char)(((int)(signed char)input+old)/2-old/2);
        if (delta) {
            UPDATE(0x1b,delta,1,20);
            t=S(profile+0x1d)+delta; saved=CLAMP(t,1,20);
            next=S(profile+0x2e);
            B(profile+0x1d)=(u8)saved;
            t=next+delta; B(profile+0x2e)=(u8)CLAMP(t,1,20);
            t=S(profile+0x34)+delta; saved=CLAMP(t,1,20);
            next=S(profile+0x36);
            B(profile+0x34)=(u8)saved;
            t=next+delta; saved=CLAMP(t,1,20);
            next=S(profile+0x3a);
            B(profile+0x36)=(u8)saved;
            t=next+delta*6; B(profile+0x3a)=(u8)CLAMP(t,-125,125);
            t=S(profile+0x3c)+delta; saved=CLAMP(t,1,20);
            next=S(profile+0x3d)+delta;
            B(profile+0x3c)=(u8)saved;
            B(profile+0x3d)=(u8)CLAMP(next,1,20);
        }
        packed=B(table+3); again=B(changes);
        B(table+3)=(u8)((((packed+again)^packed)&15)^packed);
    }
    input=B(changes+2);
    if (input) {
        packed=B(table+4); old=((signed char)(packed<<4))>>4;
        delta=(signed char)(((int)(signed char)input+old)/2-old/2);
        if (delta) {
            t=S(profile+0x27); six=delta*6; t+=six;
            saved=CLAMP(t,-125,125); next=S(profile+0x31);
            B(profile+0x27)=(u8)saved;
            t=next+six; B(profile+0x31)=(u8)CLAMP(t,-125,125);
            UPDATE(0x38,six,-125,125);
        }
        packed=B(table+4); again=B(changes+2);
        B(table+4)=(u8)((((packed+again)^packed)&15)^packed);
    }
    input=B(changes+1);
    if (input) {
        old=S(table+3)>>4;
        delta=(signed char)(((int)(signed char)input+old)/2-old/2);
        if (delta) {
            six=delta*6;
            UPDATE(0x24,six,-125,125);
            UPDATE(0x32,six,-125,125);
            UPDATE(0x33,six,-125,125);
            t=S(profile+0x3f)+delta; saved=CLAMP(t,1,20);
            next=S(profile+0x39)+six;
            B(profile+0x3f)=(u8)saved;
            B(profile+0x39)=(u8)CLAMP(next,-125,125);
        }
        packed=B(table+3); again=B(changes+1);
        B(table+3)=(u8)(((packed&0xf0)+(again<<4))^(packed&15));
    }
    input=B(changes+4);
    if (input) {
        packed=B(table+5); old=((signed char)(packed<<4))>>4;
        delta=(signed char)(((int)(signed char)input+old)/2-old/2);
        if (delta) {
            t=S(profile+0x2a); six=delta*6; t+=six;
            B(profile+0x2a)=(u8)CLAMP(t,-125,125);
            UPDATE(0x35,six,-125,125);
        }
        packed=B(table+5); again=B(changes+4);
        B(table+5)=(u8)((((packed+again)^packed)&15)^packed);
    }
    raw=cm_fn_008b4b20(D(l+4),player,l+8);
    /* 008b31ac overwrites all earlier EAX writers. EDI retains raw;
       from here to RET only AL is written, including each loop backedge. */
    if ((int)raw>0) {
        count=(S(profile+0xf)>=15)?1:B(l+3);
        if (S(profile+0x10)>=15) ++count;
        if (S(profile+0x11)>=15) ++count;
        if (S(profile+0x12)>=15) ++count;
        if (S(profile+0x13)>=15) ++count;
        if (S(profile+0x14)>=15) ++count;
        if (S(profile+0x15)>=15) ++count;
        if (S(profile+0x18)>=15) B(l+2)=1;
        if (S(profile+0x19)>=15) ++B(l+2);
        if (S(profile+0x17)>=15) ++B(l+2);
    } else count=B(l+3);
    for (index=0; (int)index<(int)raw; ++index) {
        base=D(l+8);
        grade=S(base+index*8u+6);
        key=W(base+index*8u+4);
        offset=0;
        if (key<=32) {
            if (key==32) offset=0x14;
            else if ((u32)(key-1)<=15u) {
                if (grade>=15) {
                    target=B(0x008b33ccu+(u32)(key-1));
                    target=D(0x008b33b4u+target*4u);
                } else {
                    target=B(0x008b33f4u+(u32)(key-1));
                    target=D(0x008b33dcu+target*4u);
                }
                switch (target) {
                case 0x008b325a: case 0x008b32f8: offset=0xf; break;
                case 0x008b3263: case 0x008b330b: offset=0x10; break;
                case 0x008b326c: case 0x008b331e: offset=0x11; break;
                case 0x008b3275: case 0x008b332a: offset=0x12; break;
                case 0x008b327e: case 0x008b3336: offset=0x13; break;
                }
            }
        } else if (key==0x40) offset=0x15;
        else if (key==0x80) offset=0x18;
        else if (key==0x200) offset=0x19;
        else if (key==0x800) offset=0x17;
        if (offset) {
            if (grade>=15) B(profile+(u32)offset)=20;
            else if (offset<=0x15) {
                if ((signed char)count>1) { B(profile+(u32)offset)=1; --count; }
            } else if ((signed char)B(l+2)>1) {
                B(profile+(u32)offset)=1; --B(l+2);
            }
        }
    }
    return (raw&0xffffff00u)|count;
failure:
    cm_fn_00944cff(0x00a8ccd0,0,0,first,second);
    text=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,text,first,second,line);
    result=cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
    D(0x00b67a34)=0;
    return result;
}

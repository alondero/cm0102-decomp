/* CM0102 3.9.68; preferred-image VA 00693850, image base 00400000.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   S is ESP after the four saved registers; L=S+16 is the allocation base
   after the compiler's 00945610 stack probe. Every private address below
   is an S offset, corrected for the original live pushes. No SEH or x87
   instructions occur in this root. Native callees remain explicit boundaries. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
extern u32 CM_CDECL cm_fn_0053b900(u32 *input);
extern u32 CM_CDECL cm_fn_0053c590(u32 club,u8 mode,u32 filter);
extern u32 CM_THISCALL cm_fn_005a0590(u32 self,u32 club);
extern u8 CM_THISCALL cm_fn_005986b0(u32 self);
extern void CM_CDECL cm_fn_005480f0(u32 object,u32 array,u32 count);
extern void CM_CDECL cm_fn_00548090(u32 object,u32 array,u32 count);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self,u32 person);
extern u32 CM_THISCALL cm_fn_004cf5f0(u32 self,u32 person,u32 club);
extern u32 CM_THISCALL cm_fn_008cca10(u32 self,u32 person,u32 output);
extern u32 CM_CDECL cm_fn_00544520(u32 person,u32 club);
extern u32 CM_CDECL cm_fn_0053dba0(u32 person,u32 output,u32 capacity,u32 selector);
extern s32 CM_THISCALL cm_fn_0054cdb0(u32 date,u32 low,u32 high);
extern u32 CM_CDECL cm_fn_0066f4e0(u32 output,u32 format,...);

u8 CM_STDCALL cm_fn_00693850(u32 output,u32 club,u32 id_output)
{
    u8 scratch[0x12f8];
    u32 S=(u32)scratch-16u;
    u32 eax,ecx,edx,person,slot,mapped,index,used,format,target,first,second;
    s32 count_out;
    u8 bl;
    B(S+0x13)=0; B(S+0x15)=0; B(S+0x17)=0;
    B(S+0x14)=0; B(S+0x11)=0; B(S+0x12)=0;
    if (!output) goto empty;
    if (!club) goto empty;
    eax=D(club+0xbf);
    if (!eax) goto empty;
    ecx=D(eax+0x69);
    if (!ecx) goto empty;
    eax=cm_fn_0053b900((u32 *)club);
    if (eax) goto empty;
    eax=cm_fn_0053c590(club,(u8)eax,eax);
    bl=(u8)eax;
    if ((s8)bl<15) B(S+0x14)=1;
    else {
        u16 rating=W(club+0x80);
        if (((s16)rating<2000 && (s8)bl>30) ||
            ((s16)rating<3250 && (s8)bl>45)) B(S+0x11)=1;
    }
    ecx=D(0xae2a38);
    eax=cm_fn_005a0590(ecx,club);
    if (!eax) B(S+0x12)=0;
    else {
        u8 condition=cm_fn_005986b0(eax);
        if (condition==0xfe) {
            u8 prior=B(S+0x11);
            B(S+0x12)=1;
            if (!prior && (s16)W(club+0x80)<3500 && (s8)bl>40)
                B(S+0x11)=1;
        } else if (condition==0xff) B(S+0x12)=1;
        else B(S+0x11)=0;
    }
    edx=D(club+0xbf);
    cm_fn_005480f0(edx,S+0x2a0,S+0x16);
    eax=B(S+0x16); bl=0;
    if ((s8)eax>0) do {
        index=(u32)(s32)(s8)bl;
        slot=S+0x2a0+index*4u;
        person=D(slot);
        if (!person) goto outgoing_next;
        ecx=D(person+0x61);
        if (!ecx) goto outgoing_next;
        if (B(person+0x3d)!=11) goto outgoing_next;
        ecx=D(0xae1050);
        eax=cm_fn_004cf5a0(ecx,person);
        D(0xae1080)=eax;
        if (!eax) goto outgoing_next;
        eax=D(eax+4);
        edx=D(0xae23b4);
        mapped=edx+eax*0x245u;
        if (!mapped || mapped!=club) goto outgoing_next;
        person=D(slot); ecx=D(person+0x61);
        edx=W(ecx+0xb);
        if ((s16)edx>=(s16)W(club+0x80)) goto outgoing_next;
        ecx=D(0xde1f54);
        eax=cm_fn_008cca10(ecx,person,0);
        if (eax) goto outgoing_next;
        person=D(slot);
        eax=cm_fn_00544520(person,mapped);
        if (eax) goto outgoing_next;
        person=D(slot); ecx=D(0xae1050);
        eax=cm_fn_004cf5f0(ecx,person,mapped);
        if (!eax) goto outgoing_next;
        person=D(slot); ecx=D(0xae1050);
        eax=cm_fn_004cf5f0(ecx,person,mapped);
        if (B(eax+0x49)) goto outgoing_next;
        eax=B(S+0x17); edx=D(slot);
        D(S+0x18+(u32)(s32)(s8)eax*4u)=edx;
        B(S+0x17)=(u8)(eax+1u);
outgoing_next:
        eax=B(S+0x16); bl=(u8)(bl+1u);
    } while ((s8)bl<(s8)eax);
    edx=D(club+0xbf);
    cm_fn_00548090(edx,S+0x2a0,S+0x16);
    eax=B(S+0x16); bl=0;
    if ((s8)eax>0) do {
        index=(u32)(s32)(s8)bl;
        slot=S+0x2a0+index*4u;
        person=D(slot);
        if (!person) goto incoming_next;
        ecx=D(person+0x61);
        if (!ecx) goto incoming_next;
        if (B(person+0x3d)!=11) goto incoming_next;
        ecx=D(0xae1050);
        eax=cm_fn_004cf5a0(ecx,person);
        D(0xae1080)=eax;
        if (!eax) goto external;
        eax=D(eax+4); edx=D(0xae23b4);
        mapped=edx+eax*0x245u;
        if (!mapped || mapped!=club) goto external;
        person=D(slot);
        if ((s8)B(person+0x18)>=36) goto external;
        ecx=D(person+0x61);
        edx=(u32)(s32)(s16)W(ecx+0xb);
        ecx=(u32)(s32)(s16)W(club+0x80);
        if ((s32)edx<=(s32)(ecx-750u)) goto external;
        ecx=B(S+0x15);
        D(S+0x34+(u32)(s32)(s8)ecx*4u)=person;
        B(S+0x15)=(u8)(ecx+1u);
        goto incoming_next;
external:
        edx=D(slot); eax=D(edx+0x61); eax=W(eax+0xb);
        if ((s16)eax<=4500) goto incoming_next;
        ecx=(u32)(s32)(s16)W(club+0x80);
        eax=(u32)(s32)(s16)eax;
        if ((s32)eax<=(s32)(ecx-750u)) goto incoming_next;
        if ((s32)eax>=(s32)(ecx+750u)) goto incoming_next;
        if ((s8)B(edx+0x18)>=35) goto incoming_next;
        eax=B(S+0x13);
        D(S+0x28+(u32)(s32)(s8)eax*4u)=edx;
        B(S+0x13)=(u8)(eax+1u);
incoming_next:
        eax=B(S+0x16); bl=(u8)(bl+1u);
    } while ((s8)bl<(s8)eax);
    if (id_output) {
        ecx=(u32)(s32)(s8)B(S+0x13); used=0;
        if ((s32)ecx>0) {
            used=ecx; index=0;
            do { eax=D(S+0x28+index*4u); eax=D(eax);
                 D(id_output+index*4u)=eax; ++index; --ecx; } while (ecx);
        }
        edx=(u32)(s32)(s8)B(S+0x15);
        if ((s32)edx>0) {
            ecx=id_output+used*4u; eax=S+0x34; used+=edx;
            do { person=D(eax); eax+=4; ecx+=4; --edx;
                 person=D(person); D(ecx-4)=person; } while (edx);
        }
        bl=B(S+0x17); edx=(u32)(s32)(s8)bl;
        if ((s32)edx>0) {
            ecx=id_output+used*4u; eax=S+0x18;
            do { person=D(eax); eax+=4; ecx+=4; --edx;
                 person=D(person); D(ecx-4)=person; } while (edx);
        }
    } else bl=B(S+0x17);
    /* Three report buffers share the original scratch allocation with their
       person arrays and name buffers; no initialization beyond original stores. */
    eax=(u32)(s32)(s8)B(S+0x13); D(S+0x44)=eax;
    if (eax==1) {
        person=D(S+0x28); cm_fn_0053dba0(person,S+0x48,200,1);
        cm_fn_0066f4e0(S+0xb38,0xa02ee0,S+0x48);
    } else if (eax==2 || eax==3) {
        person=D(S+0x28); cm_fn_0053dba0(person,S+0x48,200,1);
        person=D(S+0x2c); cm_fn_0053dba0(person,S+0x110,200,1);
        if (eax==3) {
            person=D(S+0x30); cm_fn_0053dba0(person,S+0x1d8,200,1);
            cm_fn_0066f4e0(S+0xb38,0xa02fa0,S+0x48,S+0x110,S+0x1d8);
        } else cm_fn_0066f4e0(S+0xb38,0xa02f30,S+0x48,S+0x110);
    } else B(S+0xb38)=0;
    eax=(u32)(s32)(s8)B(S+0x15); D(S+0x40)=eax;
    if (eax==1) {
        person=D(S+0x34); cm_fn_0053dba0(person,S+0x48,200,1);
        edx=D(person+0x42); ecx=D(person+0x3e);
        count_out=cm_fn_0054cdb0(0xae2c90,ecx,edx);
        format=count_out<30 ? 0xa02d90 : 0xa02d44;
        cm_fn_0066f4e0(S+0x750,format,S+0x48);
    } else if (eax==2 || eax==3) {
        person=D(S+0x34); cm_fn_0053dba0(person,S+0x48,200,1);
        person=D(S+0x38); cm_fn_0053dba0(person,S+0x110,200,1);
        if (eax==3) {
            person=D(S+0x3c); cm_fn_0053dba0(person,S+0x1d8,200,1);
            cm_fn_0066f4e0(S+0x750,0xa02e50,S+0x48,S+0x110,S+0x1d8);
        } else cm_fn_0066f4e0(S+0x750,0xa02ddc,S+0x48,S+0x110);
    } else B(S+0x750)=0;
    count_out=(s8)bl;
    if (count_out==1) {
        person=D(S+0x18); cm_fn_0053dba0(person,S+0x48,200,1);
        if (B(S+0x11)) {
            format=B(S+0x12) ? 0xa02614 : 0xa02578;
            cm_fn_0066f4e0(S+0x368,format,S+0x48);
        }
        /* Original falls through and overwrites the previous formatted text. */
        format=B(S+0x14) ? 0xa024bc : 0xa02474;
        cm_fn_0066f4e0(S+0x368,format,S+0x48);
    } else if (count_out==2 || count_out==3) {
        person=D(S+0x18); cm_fn_0053dba0(person,S+0x48,200,1);
        person=D(S+0x1c); cm_fn_0053dba0(person,S+0x110,200,1);
        if (count_out==3) {
            person=D(S+0x20); cm_fn_0053dba0(person,S+0x1d8,200,1);
            if (B(S+0x11)) format=B(S+0x12) ? 0xa02c40 : 0xa02b60;
            else format=B(S+0x14) ? 0xa02a7c : 0xa029f0;
            cm_fn_0066f4e0(S+0x368,format,S+0x48,S+0x110,S+0x1d8);
        } else {
            if (B(S+0x11)) format=B(S+0x12) ? 0xa02904 : 0xa02838;
            else format=B(S+0x14) ? 0xa02758 : 0xa026e0;
            cm_fn_0066f4e0(S+0x368,format,S+0x48,S+0x110);
        }
    } else B(S+0x368)=0;
    ecx=B(S+0x13); eax=0; B(S+0xf20)=0;
    if (ecx) { first=S+0xb38; eax=1; D(S+0x18)=first; }
    else first=D(S+0x18);
    ecx=B(S+0x15);
    if (ecx) { index=(u32)(s32)(s8)eax; ++eax;
        D(S+0x18+index*4u)=S+0x750; first=D(S+0x18); }
    if (bl) { index=(u32)(s32)(s8)eax; ++eax;
        D(S+0x18+index*4u)=S+0x368; first=D(S+0x18); }
    ecx=B(S+0x12);
    if (ecx) {
        ecx=B(S+0x11);
        if (ecx && !bl) { mapped=0x694404; goto combine; }
    } else {
        ecx=B(S+0x14);
        if (ecx && !bl) { mapped=0x694414; goto combine; }
    }
    mapped=0x694424;
combine:
    index=(u32)(s32)(s8)eax-1u;
    if (index>3u) {
        if (mapped==0x694424) goto empty;
        format=mapped==0x694404 ? 0xa01fe4 : 0xa01be4;
        cm_fn_0066f4e0(output,format);
        goto result;
    }
    second=D(S+0x1c);
    target=D(mapped+index*4u);
    /* These literal original table reads precede the dispatch. */
    switch (target) {
    case 0x69414c: format=0xa02318; goto four;
    case 0x69423e: format=0xa01ea8; goto four;
    case 0x69431c: format=0xa01af4; goto four;
    case 0x69418b: format=0xa021dc; goto three;
    case 0x69427d: format=0xa01da0; goto three;
    case 0x69435b: format=0xa01a34; goto three;
    case 0x6941a7: format=0xa020fc; goto two;
    case 0x694299: format=0xa01cc8; goto two;
    case 0x694377: format=0xa019a8; goto two;
    case 0x6941d8: format=0xa02038; goto one;
    case 0x6942ca: format=0xa01c38; goto one;
    case 0x6943a8: format=0xa01960; goto one;
    default: __builtin_unreachable();
    }
four:
    edx=D(S+0x24); eax=D(S+0x20); ecx=D(S+0x1c);
    cm_fn_0066f4e0(output,format,first,ecx,eax,edx); goto result;
three:
    eax=D(S+0x20);
    cm_fn_0066f4e0(output,format,first,second,eax);
    format=mapped==0x694404 ? 0xa020fc : mapped==0x694414 ? 0xa01cc8 : 0xa019a8;
two:
    cm_fn_0066f4e0(output,format,first,second); goto result;
one:
    cm_fn_0066f4e0(output,format,first);
result:
    ecx=D(S+0x44); edx=D(S+0x40);
    return (u8)(ecx+edx+(u32)count_out);
empty:
    B(output)=0;
    return 0;
}

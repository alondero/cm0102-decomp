/* WIP: original self-interior call005E5DF1 hits frozen VM escape guard; case19. */
/* Preferred-image VA005E5DF0, base00400000; target SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL7/RET0; full EAX retained, including the final original draw result.
   Original graphics calls and indirect slots remain explicit boundaries. */
#include "cmre.h"
#define D(p) CM_GLOBAL(volatile u32,(u32)(p))
#define W(p) CM_GLOBAL(volatile u16,(u32)(p))
#define B(p) CM_GLOBAL(volatile u8,(u32)(p))
extern u32 CM_CDECL cm_fn_005e3680(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_005e3730(u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_005e3b50(u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_005e42e0(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_005e7840(u32,u32,u32,u32,u32,u32);
typedef u32 (CM_THISCALL *lock_005e5df0)(u32,u32,u32,u32,u32,u32);
typedef u32 (CM_THISCALL *unlock_005e5df0)(u32,u32,u32);

struct channels_005e5df0 {u32 r,g,b,green;};

static u32 pack_005e5df0(u32 r,u32 g,u32 b,u32 green)
{
    if((s32)b>=255) b=255;
    if((s32)g>=255) g=255;
    if((s32)r>=255) r=255;
    if(green==0x7e0)
        return ((r&0xf8)<<8)|((g&0xfc)<<3)|((u8)b>>3);
    return ((r&0xf8)<<7)|((g&0xf8)<<2)|((u8)b>>3);
}

static struct channels_005e5df0 decode_005e5df0(u32 color)
{
    struct channels_005e5df0 c;
    u32 mask,numerator,denominator;
    color&=0xffff;
    mask=D(0xae3168); numerator=(color&mask)<<8; denominator=mask+1;
    c.r=(u8)(numerator/denominator);
    mask=D(0xae316c); numerator=(color&mask)<<8; denominator=mask+1;
    c.g=(u8)(numerator/denominator); c.green=mask;
    mask=D(0xae3170); numerator=(color&mask)<<8; denominator=mask+1;
    c.b=(u8)(numerator/denominator);
    return c;
}

static struct channels_005e5df0 cached_005e5df0(u32 color,u32 red,u32 green,u32 blue)
{
    struct channels_005e5df0 c;
    color&=0xffff;
    c.r=(u8)(((color&red)<<8)/(red+1));
    c.g=(u8)(((color&green)<<8)/(green+1)); c.green=green;
    c.b=(u8)(((color&blue)<<8)/(blue+1));
    return c;
}

static u32 scale_005e5df0(struct channels_005e5df0 c,u8 factor)
{
    return pack_005e5df0(c.r*factor/100u,c.g*factor/100u,c.b*factor/100u,c.green);
}

static u32 half_005e5df0(u32 value)
{
    u32 sign=(u32)((s32)value>>31);
    return (u32)((s32)(value-sign)>>1);
}

u32 CM_CDECL cm_fn_005e5df0(u32 left,u32 top,u32 right,u32 bottom,
                           u32 flags,u32 color,u32 other_color)
{
    volatile u8 scratch[0xc4];
    u32 eax,object,table,target,p,a,b,red,green,blue,den_red,den_green,den_blue;
    u32 index,numerator,denominator,level,thickness,remaining,q;
    u32 inner_left,inner_right,inner_top,inner_bottom,dy,dx,bright,dark,t;
    u32 saved_right,mid;
    struct channels_005e5df0 channels;
    eax=D(0xaebe0c);
    if(eax!=0) return eax;
    if((flags&0x20000)!=0) {right-=2; bottom-=2;}
    if((flags&0x40000)!=0) {left+=2; top+=2;}
    if((flags&0x1000)!=0) {
        object=D(0xaebde4);
        if(object==0) color=0;
        else {
            eax=cm_fn_005e3680(left,top,left,top,(u32)scratch+0x48);
            if(eax==0) color=0;
            else {
                object=D(0xaebde4);
                D((u32)scratch+0x58)=0x6c;
                table=D(object); target=D(table+0x64);
                eax=((lock_005e5df0)target)(table,object,(u32)scratch+0x48,
                                           (u32)scratch+0x58,0x11,0);
                if(eax!=0) color=0;
                else {
                    p=D((u32)scratch+0x7c);
                    color=W(p);
                    object=D(0xaebde4); table=D(object); target=D(table+0x80);
                    ((unlock_005e5df0)target)(table,object,0);
                    /* Native BP only: its upper bits are dead at every
                       subsequent original color consumer (WORD or maskFFFF). */
                }
            }
        }
    }
    if((flags&2)!=0) cm_fn_005e42e0(left,top,right,bottom);
    else if((flags&0x10)!=0) {
        if((flags&0x10000)!=0) cm_fn_005e7840(left,top,right,bottom,4,color);
        else cm_fn_005e3b50(left,top,right,bottom,4,color);
    } else if((flags&4)!=0) {
        if((s32)left<=(s32)right) {
            numerator=0; denominator=right-left; index=left;
            do {
                q=(u32)((s32)numerator/(s32)denominator);
                eax=D(0xaebe0c);
                if(eax==0) {channels=decode_005e5df0(color); eax=scale_005e5df0(channels,(u8)(q+100));}
                else eax=0;
                cm_fn_005e3730(index,top,index,bottom,2,eax);
                index++; numerator-=100;
            } while((s32)index<=(s32)right);
        }
    } else if((flags&8)!=0) {
        if((s32)top<=(s32)bottom) {
            numerator=0; denominator=bottom-top; index=top;
            do {
                q=(u32)((s32)numerator/(s32)denominator);
                eax=D(0xaebe0c);
                if(eax==0) {channels=decode_005e5df0(color); eax=scale_005e5df0(channels,(u8)(q+100));}
                else eax=0;
                cm_fn_005e3730(left,index,right,index,2,eax);
                index++; numerator-=100;
            } while((s32)index<=(s32)bottom);
        }
    }
    if((flags&0x20)!=0) {
        if((flags&0x10000)!=0) {
            cm_fn_005e7840(left,top,right,bottom,(flags&0x2000)!=0?1:2,other_color);
        } else if((flags&0x200)!=0) {
            cm_fn_005e3b50(left,top,right,bottom,(flags&0x2000)!=0?1:2,color);
        } else {
            thickness=2;
            if((s32)(right-left)>=50 && (s32)(bottom-top)>=50 &&
               (s32)bottom<100 && (flags&0x10)!=0) thickness=4;
            inner_left=left+thickness; inner_right=right-thickness;
            inner_top=top+thickness; inner_bottom=bottom-thickness;
            index=inner_top-1; saved_right=inner_right+1;
            dx=inner_left-inner_top; dy=inner_bottom-inner_right;
            level=0x42; remaining=thickness;
            do {
                a=color;
                if((u16)a==0) a=D(0xaebe04);
                q=(u32)((s32)level/(s32)thickness);
                blue=D(0xae3170); green=D(0xae316c); red=D(0xae3168);
                b=D(0xaebe0c);
                if(b!=0) bright=0;
                else {channels=cached_005e5df0(a,red,green,blue); bright=scale_005e5df0(channels,(u8)(q+100));}
                b=D(0xaebe0c);
                if(b!=0) dark=0;
                else {channels=cached_005e5df0(color,red,green,blue); dark=scale_005e5df0(channels,(u8)(100-q));}
                if((flags&0x40)!=0) {t=bright; bright=dark; dark=t;}
                a=dx+index; b=dy+saved_right;
                cm_fn_005e3730(a,index,saved_right,index,2,bright);
                cm_fn_005e3730(a,index,a,b,2,bright);
                cm_fn_005e3730(saved_right,b,dx+index+1,b,2,dark);
                cm_fn_005e3730(saved_right,b,saved_right,index+1,2,dark);
                level+=0x42; index--; saved_right++; remaining--;
            } while(remaining!=0);
        }
    } else if((flags&0x80)!=0) {
        a=color; if((u16)a==0) a=D(0xaebe04);
        b=D(0xaebe0c);
        if(b!=0) eax=0;
        else {channels=decode_005e5df0(a); eax=scale_005e5df0(channels,90);}
        cm_fn_005e3730(left+2,bottom,right-2,bottom,1,eax);
    } else if((flags&0x400)!=0) {
        if((flags&0x4000)!=0) {
            if((flags&0x8000)!=0) a=top;
            else a=top+half_005e5df0(bottom-top);
            cm_fn_005e3730(left,a,right,a,(flags&0x2000)!=0?1:2,color);
        } else {
            cm_fn_005e3730(left,top,right,bottom,(flags&0x2000)!=0?1:2,color);
        }
    }
    if((flags&0x1000000)!=0) {
        a=color; if((u16)a==0) a=D(0xaebe04);
        b=D(0xaebe0c);
        if(b!=0) {bright=0; dark=0;}
        else {
            a&=0xffff;
            red=D(0xae3168); den_red=red+1;
            channels.r=(u8)(((a&red)<<8)/den_red);
            green=D(0xae316c); den_green=green+1;
            channels.g=(u8)(((a&green)<<8)/den_green);
            blue=D(0xae3170); den_blue=blue+1;
            channels.b=(u8)(((a&blue)<<8)/den_blue);
            green=D(0xae316c);
            bright=pack_005e5df0(channels.r*133u/100u,channels.g*133u/100u,channels.b*133u/100u,green);
            a=color&0xffff;
            channels.r=(u8)(((a&red)<<8)/den_red);
            green=D(0xae316c);
            channels.g=(u8)(((a&green)<<8)/den_green);
            blue=D(0xae3170);
            channels.b=(u8)(((a&blue)<<8)/den_blue);
            green=D(0xae316c);
            dark=pack_005e5df0(channels.r*66u/100u,channels.g*66u/100u,channels.b*66u/100u,green);
        }
        mid=top+half_005e5df0(bottom-top);
        cm_fn_005e3730(left+2,mid,right-2,mid,2,dark);
        cm_fn_005e3730(left+2,mid+1,right-2,mid+1,2,bright);
    }
    eax=flags;
    if((flags&0x800)!=0) {
        a=W(0xaebdec);
        eax=cm_fn_005e3b50(left-1,top-1,right+1,bottom+1,2,a);
    }
    return eax;
}

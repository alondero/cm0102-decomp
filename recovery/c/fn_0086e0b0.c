/* Preferred-image VA0086E0B0, base00400000, SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   STDCALL2/RET8. Native B=L-10 after SUB4E8/four saves. One allocation
   preserves B10..4F8 scratch followed by the linked SEH record at B4F8.
   Buffers B28/48/1D8/3CC are uninitialized; original callees remain boundaries. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32,(u32)(a))
#define W(a) CM_GLOBAL(volatile u16,(u32)(a))
#define B(a) CM_GLOBAL(volatile u8,(u32)(a))
#define SB(a) ((u32)(s32)(s8)B(a))
extern u32 CM_CDECL cm_fn_0066f4e0(u32,u32,...);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_THISCALL cm_fn_004ae660(u32,u32);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0053e140(u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0053e990(u32,u32,u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_0043f460(u32,u32);
extern u32 CM_CDECL cm_fn_00441c10(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_004bfa30(u32,u16,u16,u8,u32,u32);
extern u8 CM_CDECL cm_fn_004bff70(u32,u16,u16,u8);
extern u32 CM_CDECL cm_fn_0054d3b0(u8);
extern u32 CM_CDECL cm_fn_0090c310(u32,u8);
extern u32 CM_CDECL cm_fn_0053ad00(u8,u32,u32,u32);

u32 CM_STDCALL cm_fn_0086e0b0(u32 record,u32 output)
{
    volatile struct {
        u8 scratch[0x4e8];
        struct {u32 previous,handler,state;} frame;
    } local;
    u32 pool=(u32)local.scratch;
    u32 small=pool+0x18, text=pool+0x38, name=pool+0x1c8, other=pool+0x3bc;
    u32 a,c,d,offset,index,object,fmt,first,second,third,person,club,amount;
    u32 gender,gender2,result;
    u16 x,y;
    u8 kind,subtype,flag,qual;
    unsigned long long money;
    local.frame.previous=CM_FS(u32,0);
    local.frame.state=0xffffffffu;
    local.frame.handler=0x962df1;
    CM_FS(u32,0)=(u32)&local.frame;
    kind=B(record+0xc);
    result=(local.frame.previous&0xffffff00u)|kind;
    if(kind==0x96){
        subtype=B(record+0xd);index=D(record+8);
        a=D(0xae23b4)+index*581u;gender=SB(a+0x37);
        fmt=subtype==1?0xa8499c:subtype==0?0xa84960:0xa84924;
        cm_fn_0066f4e0(0xde1f64,fmt,gender,gender,a+4);
        cm_fn_004ae660(output,0xde1f64);
        index=D(record+8);a=D(0xae23b4)+index*581u;
        goto add_club_record;
    }
    if(kind==0x97){
        person=D(record+0x11);subtype=B(record+0xd);
        first=D(record+0x15);second=D(record+0x19);third=D(record+0x1d);
        B(pool+3)=subtype;
        cm_fn_0053e140(first,second,third,name,500,1,person);
        subtype=B(pool+3);fmt=subtype==1?0xa848ec:subtype==0?0xa848b0:0xa84874;
        cm_fn_0066f4e0(0xde1f64,fmt,name);cm_fn_004ae660(output,0xde1f64);
        offset=person*110u;
        result=cm_fn_0053e990(D(0xae23bc)+offset,first,second,third,0);
        if(result!=0)result=cm_fn_004ae8a0(output,name,0x7d7,D(0xae23bc)+offset);
        goto done;
    }
    if(kind==0 || kind==1 || kind==2 || kind==5){
        index=D(record+0xd);offset=index*107u;a=D(0xae23d0)+offset;gender=SB(a+0x37);
        fmt=kind==0?0xa84848:kind==1?0xa84818:kind==2?0xa847e8:0xa847b0;
        cm_fn_0066f4e0(0xde1f64,fmt,gender,gender,a+4);
        cm_fn_004ae660(output,0xde1f64);
        a=D(0xae23d0)+offset;result=cm_fn_004ae8a0(output,a+4,0x7d0,a);goto done;
    }
    if(kind==0x34){
        flag=B(record+0x15);subtype=B(record+0x19);index=D(record+0xd);x=W(record+0x11);
        B(pool+3)=subtype;
        if(flag!=0)cm_fn_00944c9f(small,0x988070,(u32)x-(u32)(s32)(s8)flag,(u32)x%100u);
        else cm_fn_00944c9f(small,0x98806c,(u32)x);
        subtype=B(pool+3);fmt=subtype==1?0xa84750:subtype==2?0xa846e4:0xa84678;
        offset=index*101u;a=D(0xae23e0)+offset;gender=SB(a+0x37);
        cm_fn_0066f4e0(0xde1f64,fmt,gender,gender,small,a+4);
        goto add_competition;
    }
    if(kind==0x32){
        flag=B(record+0x15);index=D(record+0xd);x=W(record+0x11);
        if(flag!=0){a=(u32)x+(u32)(s32)(s8)flag;d=(u32)((s32)a%100);cm_fn_00944c9f(small,0x988070,(u32)x,d);}
        else cm_fn_00944c9f(small,0x98806c,(u32)x);
        offset=index*101u;a=D(0xae23e0)+offset;gender=SB(a+0x37);
        cm_fn_0066f4e0(0xde1f64,0xa84610,gender,gender,small,a+4);
        goto add_competition;
    }
    if(kind==0x33){
        index=D(record+0xd);flag=B(record+0x11);B(pool+8)=flag;
        offset=index*101u;a=D(0xae23e0)+offset;gender=SB(a+0x37);object=a;
        c=cm_fn_0054d3b0(B(pool+8));
        cm_fn_0066f4e0(0xde1f64,0xa845b0,gender,gender,object+4,c);
        goto add_competition;
    }
    if(kind==0x35){
        a=D(record+0x11);x=W(record+0xd);
        if(a!=0)cm_fn_00944c9f(small,0x988070,(u32)x,((u32)x+1u)%100u);
        else cm_fn_00944c9f(small,0x98806c,(u32)x);
        cm_fn_0066f4e0(text,0xa84590);
        cm_fn_0066f4e0(0xde1f64,0xa84534,small,text);
        cm_fn_004ae660(output,0xde1f64);
        index=D(record+8);a=D(0xae23b4)+index*581u;
        result=cm_fn_004ae8a0(output,text,0x7e6,a);goto done;
    }
    if(kind==6){
        index=D(record+0xd);a=D(0xae23b4);offset=index*581u;
        d=SB(record+0x11);gender=SB(a+offset+0x52);
        cm_fn_0066f4e0(0xde1f64,0xa844d4,gender,gender,a+offset+0x38,d);
        cm_fn_004ae660(output,0xde1f64);a=D(0xae23b4)+offset;goto add_club_record;
    }
    if(kind==3 || kind==4 || kind==7){
        index=D(record+0xd);x=W(record+0x11);y=kind==7?0:W(record+0x15);
        object=0;if(index!=0xffffffffu)object=D(0xae23d0)+index*107u;
        cm_fn_004bfa30(object,x,y,2,text,400);
        gender=(u32)(s32)(s8)cm_fn_004bff70(object,x,y,2);
        fmt=kind==3?0xa8448c:kind==4?0xa84440:0xa843f0;
        cm_fn_0066f4e0(0xde1f64,fmt,gender,gender,text);
        result=cm_fn_004ae660(output,0xde1f64);
        if(object!=0)result=cm_fn_004ae8a0(output,text,0x7d0,object);
        goto done;
    }
    if(kind==8 || kind==9){
        index=D(record+0xd);object=0;
        if(index!=0xffffffffu)object=D(0xae23d0)+index*107u;
        gender=SB(object+0x37);a=object+4;
        fmt=kind==8?0xa843ac:0xa84368;
        cm_fn_0066f4e0(0xde1f64,fmt,gender,gender,a);
        result=cm_fn_004ae660(output,0xde1f64);
        if(object!=0)result=cm_fn_004ae8a0(output,a,0x7d0,object);
        goto done;
    }
    if(kind==0xa){
        x=W(record+0x11);flag=B(record+0x15);index=D(record+0xd);
        object=D(0xae23d0)+index*107u;
        cm_fn_004bfa30(object,x,0,1,text,200);
        gender=(u32)(s32)(s8)cm_fn_004bff70(object,x,0,1);
        c=cm_fn_0090c310((u32)(s32)(s8)flag,0xff);
        cm_fn_0066f4e0(0xde1f64,0xa84310,gender,gender,c,text);
        cm_fn_004ae660(output,0xde1f64);
        result=cm_fn_004ae8a0(output,text,0x7d0,object);goto done;
    }
    if(kind==0x64){
        third=D(record+0x19);person=D(record+0xd);D(pool+8)=third;
        flag=B(record+0x21);B(pool+3)=flag;
        second=D(record+0x15);qual=B(record+0x29);first=D(record+0x11);B(pool+0xc)=qual;
        amount=D(record+0x1d);club=D(record+0x25);
        D(pool+4)=first;D(pool+0x10)=second;
        cm_fn_0053e140(first,second,third,name,500,1,person);
        if((s32)amount>0){
            D(pool+0x14)=amount;local.frame.state=0;
            money=cm_fn_0043f460(pool+0x14,5);
            cm_fn_00441c10(text,100,(u32)money,(u32)(money>>32),5);
            qual=B(pool+0xc);local.frame.state=0xffffffffu;
            if(qual==0xb){fmt=B(pool+3)!=0?0xa84280:0xa84200;goto person_with_amount;}
        }
        if(B(pool+3)!=0){fmt=0xa84198;goto person_with_club;}
        if(B(pool+0xc)!=0xb){
            a=cm_fn_0053ad00(B(pool+0xc),0,0,pool+3);gender=SB(pool+3);
            cm_fn_0066f4e0(0xde1f64,0xa84144,gender,gender,name,a);goto finish_person;
        }
        if(club!=0xffffffffu){fmt=0xa840dc;goto person_with_club;}
        cm_fn_0066f4e0(0xde1f64,0xa8409c,name);goto finish_person;
    }
    if(kind==0x65){
        third=D(record+0x19);person=D(record+0xd);D(pool+0xc)=third;
        flag=B(record+0x21);B(pool+3)=flag;second=D(record+0x15);
        qual=B(record+0x29);B(pool+8)=qual;first=D(record+0x11);
        amount=D(record+0x1d);club=D(record+0x25);D(pool+0x14)=first;D(pool+0x10)=second;
        cm_fn_0053e140(first,second,third,name,500,1,person);
        if((s32)amount>0){
            D(pool+4)=amount;local.frame.state=1;money=cm_fn_0043f460(pool+4,5);
            cm_fn_00441c10(text,100,(u32)money,(u32)(money>>32),5);local.frame.state=0xffffffffu;
        }
        if(B(pool+8)!=0xb){
            a=cm_fn_0053ad00(B(pool+8),0,0,pool+3);
            c=D(0xae23b4)+club*581u;gender=SB(c+0x52);gender2=SB(pool+3);
            cm_fn_0066f4e0(0xde1f64,0xa84024,gender,gender,gender2,gender2,name,c+0x38,a);
            goto finish_person;
        }
        if((s32)amount>0){fmt=B(pool+3)!=0?0xa83f98:0xa83f1c;goto person_with_amount;}
        if(B(pool+3)!=0){fmt=0xa83eb8;goto person_with_club;}
        if(club!=0xffffffffu){fmt=0xa83e50;goto person_with_club;}
        cm_fn_0066f4e0(0xde1f64,0xa83e0c,name);goto finish_person;
    }
    if(kind==0x66){
        club=D(record+0xd);qual=B(record+0x19);amount=D(record+0x11);flag=B(record+0x15);
        B(pool+4)=qual;
        if(club==0xffffffffu){cm_fn_0066f4e0(0xde1f64,0xa83c5c);goto finish_club;}
        if((s32)amount>0 && qual==0xb){
            D(pool+4)=amount;local.frame.state=2;money=cm_fn_0043f460(pool+4,5);
            cm_fn_00441c10(text,100,(u32)money,(u32)(money>>32),5);
            a=D(0xae23b4)+club*581u;gender=SB(a+0x52);
            cm_fn_0066f4e0(0xde1f64,flag!=0?0xa83da4:0xa83d4c,gender,gender,a+0x38,text);
            local.frame.state=0xffffffffu;goto finish_club;
        }
        if(flag!=0){fmt=0xa83d0c;goto club_only;}
        if(qual!=0xb){
            a=cm_fn_0053ad00(B(pool+4),0,0,pool+3);
            c=D(0xae23b4)+club*581u;gender=SB(c+0x52);gender2=SB(pool+3);
            cm_fn_0066f4e0(0xde1f64,0xa83cb8,gender,gender,gender2,gender2,c+0x38,a);goto finish_club;
        }
        fmt=0xa83c78;
club_only:
        a=D(0xae23b4)+club*581u;gender=SB(a+0x52);
        cm_fn_0066f4e0(0xde1f64,fmt,gender,gender,a+0x38);
finish_club:
        result=cm_fn_004ae660(output,0xde1f64);
        if(club!=0xffffffffu){a=D(0xae23b4)+club*581u;goto add_club_record;}
        goto done;
    }
    if(kind==0x68){
        person=D(record+0xd);first=D(record+0x11);second=D(record+0x15);third=D(record+0x19);
        cm_fn_0053e140(first,second,third,text,300,1,person);
        cm_fn_0066f4e0(0xde1f64,0xa83e0c,text);cm_fn_004ae660(output,0xde1f64);
        offset=person*110u;result=cm_fn_0053e990(D(0xae23bc)+offset,first,second,third,0);
        if(result!=0)result=cm_fn_004ae8a0(output,text,0x7d7,D(0xae23bc)+offset);
        goto done;
    }
    if(kind==0x67){cm_fn_0066f4e0(0xde1f64,0xa83c5c);result=cm_fn_004ae660(output,0xde1f64);goto done;}
    if(kind==0x69){
        person=D(record+0xd);third=D(record+0x19);second=D(record+0x15);first=D(record+0x11);club=D(record+0x1d);
        D(pool+0x10)=second;D(pool+4)=third;
        cm_fn_0053e140(first,second,third,other,300,1,person);
        c=D(0xae23b4);offset=club*581u;gender=SB(offset+c+0x52);
        cm_fn_0066f4e0(0xde1f64,0xa83c04,gender,gender,other,offset+c+0x38);
        cm_fn_004ae660(output,0xde1f64);
        a=D(pool+4);d=D(pool+0x10);index=person*110u;
        result=cm_fn_0053e990(D(0xae23bc)+index,first,d,a,0);
        if(result!=0)result=cm_fn_004ae8a0(output,other,0x7d7,D(0xae23bc)+index);
        a=offset+D(0xae23b4);goto add_club_record;
    }
    if(kind==0x6a){
        index=D(record+0xd);a=D(0xae23b4);offset=index*581u;gender=SB(a+offset+0x52);
        cm_fn_0066f4e0(0xde1f64,0xa83bd0,gender,gender,a+offset+0x38);
        cm_fn_004ae660(output,0xde1f64);a=D(0xae23b4)+offset;goto add_club_record;
    }
    goto done;
person_with_amount:
    a=D(0xae23b4)+club*581u;gender=SB(a+0x52);
    cm_fn_0066f4e0(0xde1f64,fmt,gender,gender,name,a+0x38,text);goto finish_person;
person_with_club:
    a=D(0xae23b4)+club*581u;gender=SB(a+0x52);
    cm_fn_0066f4e0(0xde1f64,fmt,gender,gender,name,a+0x38);
finish_person:
    cm_fn_004ae660(output,0xde1f64);
    if(kind==0x64){d=D(pool+0x10);c=D(pool+4);a=D(pool+8);}
    else{a=D(pool+0xc);d=D(pool+0x10);c=D(pool+0x14);}
    offset=person*110u;result=cm_fn_0053e990(D(0xae23bc)+offset,c,d,a,0);
    if(result!=0)result=cm_fn_004ae8a0(output,name,0x7d7,D(0xae23bc)+offset);
    if(club==0xffffffffu)goto done;
    a=D(0xae23b4)+club*581u;
add_club_record:
    result=cm_fn_004ae8a0(output,a+0x38,0x7d5,a);goto done;
add_competition:
    cm_fn_004ae660(output,0xde1f64);a=D(0xae23e0)+offset;
    result=cm_fn_004ae8a0(output,a+4,0x7d4,a);
done:
    a=local.frame.previous;CM_FS(u32,0)=a;
    return result;
}

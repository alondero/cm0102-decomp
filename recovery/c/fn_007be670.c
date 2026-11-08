#include "cmre.h"

/* Preferred-image VA 007be670, base 00400000; primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The one direct caller pushes nine cdecl arguments and discards EAX.
   Original __chkstk(1b30) is compiler frame allocation. Scratch offsets below
   use the stable original ESP after its four saved-register pushes. Its +10
   is the first allocated byte. Keep one contiguous uninitialized allocation:
   the 26-byte filter records and their deliberately unaligned fields overlap
   later scratch reads exactly as in the original. Callees remain original. */
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32,u32,...);
extern u32 CM_CDECL cm_fn_0053dba0(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0053e140(u32,u32,u32,u32,u32,u32,u32);
extern u8 CM_CDECL cm_fn_007bfcd0(s8,u32,u32,u32);
extern u8 CM_CDECL cm_fn_007bfbd0(s8,u32,u32,u32);
extern u8 CM_CDECL cm_fn_007bfc50(s8,u32,u32,u32);

#define D(a) CM_GLOBAL(volatile u32,(a))
#define B(a) CM_GLOBAL(volatile u8,(a))
#define W(a) CM_GLOBAL(volatile u16,(a))
#define P(o) ((u32)scratch + (o) - 0x10u)
#define SD(o) D(P(o))
#define SB(o) B(P(o))
#define SW(o) W(P(o))
#define FORMAT(...) cm_fn_0066f4e0(__VA_ARGS__)

void CM_CDECL cm_fn_007be670(u32 destination, u32 first, u32 second,
    u32 mode, u32 flags, u32 count, u32 records, u32 indices, u32 copied)
{
    u8 scratch[0x1b30];
    u8 description = 0, last_count, selected_mode, value, other;
    u32 object, name, a, b, c, d, bits, format, reason;
    s32 gender, first_gender;
    SB(0x11) = 0;
    SB(0x10) = 0;
    if (destination == 0) {
        cm_fn_00944cff(0x00a701b0,0,0,P(0x155c),P(0xf78));
        a = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0,0x009870e8,a,P(0x155c),P(0xf78),0x24b6);
        cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
        D(0x00b67a34)=0;
        return;
    }
    if (first==0 || second==0 || records==0 || indices==0) {
        cm_fn_00944c9f(destination,0x00a759b4);
        return;
    }
    B(copied)=0;
    object=D(second+0xcf);
    if (object==0) {
        object=D(second+0xd3);
        if (object==0) goto no_object;
    }
    a=D(object+0x69);
    if (a==0) goto no_object;
    value=B(a+0x43);
    switch (value) {
    case 1: format=0x00a782c0; break;
    case 2: format=0x00a782b8; break;
    case 3: format=0x00a782b0; break;
    case 4: format=0x00a782a8; break;
    case 5: format=0x00a782a0; break;
    case 6: format=0x00a78298; break;
    case 7: format=0x00a7828c; break;
    case 8: format=0x00a78284; break;
    case 9: format=0x00a1594c; break;
    default:format=0x00a782b0; break;
    }
    FORMAT(P(0x268),format);
    a=D(object+0x69);
    value=B(a+0xe);
    if ((s8)value>=15) {
        if ((s8)B(a+0x13)>=15) { format=0x00a78258; goto describe; }
        if ((s8)value>=15 && (s8)B(a+0x13)<=5) { format=0x00a78220; goto describe; }
    }
    if ((s8)value<=5) {
        if ((s8)B(a+0x13)>=15) { format=0x00a781ec; goto describe; }
        if ((s8)value<=5 && (s8)B(a+0x13)<=5) { format=0x00a781b4; goto describe; }
    }
    if ((s8)value>=15) { format=0x00a78258; goto describe; }
    if ((s8)value<=5) { format=0x00a78188; goto describe; }
    value=B(a+0x13);
    if ((s8)value>=15) { format=0x00a78160; goto describe; }
    if ((s8)value<=5) { format=0x00a78138; goto describe; }
    goto object_text;
describe:
    FORMAT(P(0x1078),format,0x00ad9c64);
    description=1;
    goto object_text;
no_object:
    FORMAT(P(0x165c),0x00a78130);
object_text:
    if (object!=0) {
        cm_fn_0053dba0(object,P(0x88c),0x78,1);
        gender=(s8)B(second+0x52);
        name=second+0x38;
        if (description)
            FORMAT(P(0x165c),0x00a78084,0x64,gender,gender,P(0x88c),name,P(0x1078),P(0x268));
        else
            FORMAT(P(0x165c),0x00a77ff4,0x64,gender,gender,P(0x88c),name,P(0x268));
    } else {
        gender=(s8)B(second+0x52);
        name=second+0x38;
        FORMAT(P(0x165c),0x00a77fa4,gender,gender,name);
    }
    a=(u32)(s32)(s8)mode;
    if (a<=6) {
        (void)D(0x007bfbb0+a*4);
        switch (a) {
        case 6:
            value=B(flags+0xc);
            gender=(s8)B(second+0x52);
            format=(s8)value < -3 ? 0x00a77f30:0x00a77eec;
            break;
        case 5:
            value=B(flags+0xb);
            if (value!=0) {
                a=W(second+0x80);
                if ((s16)a<(s16)W(first+0x80)) { format=0x00a77e90; goto mode_gender; }
            }
            value=B(flags+0xc);
            gender=(s8)B(second+0x52);
            format=(s8)value < -3 ? 0x00a77e14:0x00a77dd0;
            break;
        case 4:
            value=B(flags+0xb);
            if (value!=0) {
                a=W(second+0x80);
                if ((s16)a<(s16)W(first+0x80)) { format=0x00a77d8c; goto mode_gender; }
            }
            value=B(flags+0xc);
            gender=(s8)B(second+0x52);
            format=(s8)value < -3 ? 0x00a77d10:0x00a77cd4;
            break;
        case 3:
            value=B(flags+0xb);
            gender=(s8)B(second+0x52);
            format=value?0x00a77c8c:0x00a77c48;
            break;
        case 1:
            if ((s8)B(flags+0xc)<-3) { format=0x00a77bbc; goto mode_gender; }
            value=B(flags+0xb);
            if (value==0) {
                a=W(second+0x80);
                if ((s16)a>(s16)W(first+0x80)) { format=0x00a77b70; goto mode_gender; }
            }
            format=0x00a77b2c; goto mode_gender;
        case 0:
            value=B(flags+0xb);
            if (value==0) {
                a=W(second+0x80);
                if ((s16)a>(s16)W(first+0x80)) { format=0x00a77adc; goto mode_gender; }
            }
            format=0x00a77a98; goto mode_gender;
        default:format=0x00a77a54;
mode_gender:
            gender=(s8)B(second+0x52);
            break;
        }
        FORMAT(P(0x904),format,gender,gender,name);
        SB(0x11)=1;
    }
    value=cm_fn_007bfcd0((s8)count,records,second,P(0x1e4));
    SB(0x13)=value;
    value=cm_fn_007bfbd0((s8)count,records,second,P(0x14));
    SB(0x12)=value;
    last_count=cm_fn_007bfc50((s8)count,records,second,P(0x160));
    selected_mode=(u8)mode;
    if ((s8)selected_mode<2) {
        value=B(flags+5);
        if (value!=0) {
            other=B(flags+0xb);
            if (other==0) {
                a=W(second+0x80);
                if ((s16)a>(s16)W(first+0x80)) {
                    FORMAT(P(0x3a8),0x00a77a1c); goto reason_done;
                }
            }
        }
        if (value!=0) { format=0x00a77998; goto first_reason; }
        value=B(flags+7);
        if (value!=0) { format=0x00a77924; goto first_reason; }
        value=B(flags);
        if (value!=0 && B(flags+2)!=0) { format=0x00a778ac; goto first_reason; }
        other=B(flags+1);
        if (other==0) goto combine;
        if (B(flags+2)==0 && value==0) {
            FORMAT(P(0x3a8),0x00a77858); goto reason_done;
        }
        if (other==0) goto combine;
        FORMAT(P(0x3a8),0x00a77828); goto reason_done;
first_reason:
        gender=(s8)B(first+0x52);
        FORMAT(P(0x3a8),format,gender,gender,first+0x38);
        goto reason_done;
    }
    if ((u8)count!=0 && (D(records+0x14)&0x40000)!=0) {
        a=D(records+4); c=D(records+0x10); d=D(records+0xc); b=D(records+8);
        cm_fn_0053e140(b,d,c,P(0x98),0xc8,1,a);
        bits=D(records+0x14);
        if (bits&0x200) format=0x00a777c8;
        else if (bits&0x400) format=0x00a77764;
        else if (bits&0x100) format=0x00a77704;
        else format=0x00a776a8;
        FORMAT(P(0x3a8),format,P(0x98));
        B(copied)=1; a=D(records+4); D(indices)=a;
        goto reason_done;
    }
    if (last_count!=0 && SB(0x12)!=0 && (SB(0x28)&1)!=0) {
        a=SD(0x18);d=SD(0x24);b=SD(0x20);c=SD(0x1c);
        cm_fn_0053e140(c,b,d,P(0x98),0xc8,1,a);
        a=SD(0x164);d=SD(0x170);b=SD(0x16c);c=SD(0x168);
        cm_fn_0053e140(c,b,d,P(0x2e0),0xc8,1,a);
        a=SD(0x18);d=SD(0x24);b=SD(0x20);c=SD(0x1c);
        cm_fn_0053e140(c,b,d,P(0xde8),0xc8,5,a);
        a=SD(0x164);d=SD(0x170);b=SD(0x16c);c=SD(0x168);
        cm_fn_0053e140(c,b,d,P(0xeb0),0xc8,5,a);
        bits=SD(0x28); a=SD(0x174);
        if ((bits&0x1000)!=0 && (a&0x4000)!=0) {
            gender=(s8)B(first+0x52);
            FORMAT(P(0x3a8),0x00a775e0,0x64,gender,gender,P(0x2e0),P(0x98),first+0x38);
        } else if (a&4) {
            gender=(s8)B(first+0x52);
            FORMAT(P(0x3a8),0x00a774a0,0x64,gender,gender,P(0x2e0),P(0x98),first+0x38,P(0xde8),P(0xeb0));
        } else FORMAT(P(0x3a8),0x00a77410,P(0x98),P(0x2e0));
        a=SD(0x18); B(copied)=2; b=SD(0x164); D(indices)=a; D(indices+4)=b;
        goto reason_done;
    }
    if ((s8)last_count>1 && (D(records+0x14)&0x100)!=0) {
        a=SD(0x164);d=SD(0x170);b=SD(0x16c);c=SD(0x168);
        cm_fn_0053e140(c,b,d,P(0x98),0xc8,1,a);
        a=SD(0x17e);d=SD(0x18a);b=SD(0x186);c=SD(0x182);
        cm_fn_0053e140(c,b,d,P(0x2e0),0xc8,1,a);
        FORMAT(P(0x3a8),0x00a7737c,P(0x98),P(0x2e0));
        a=SD(0x164);b=SD(0x17e);B(copied)=2;D(indices)=a;D(indices+4)=b;
        goto reason_done;
    }
    value=SB(0x12);
    if ((s8)value>1 && (D(records+0x14)&0x400)!=0) {
        a=SD(0x18);d=SD(0x24);b=SD(0x20);c=SD(0x1c);
        cm_fn_0053e140(c,b,d,P(0x98),0xc8,1,a);
        a=SD(0x32);d=SD(0x3e);b=SD(0x3a);c=SD(0x36);
        cm_fn_0053e140(c,b,d,P(0x2e0),0xc8,1,a);
        bits=SD(0x28); a=SD(0x42);
        if ((bits&0x1000)!=0 && (a&1)!=0) {
            if (bits&0x2000) FORMAT(P(0x3a8),0x00a772e0,P(0x2e0),P(0x98));
            else FORMAT(P(0x3a8),0x00a77244,P(0x2e0),P(0x98));
        } else if ((a&0x1000)!=0 && (bits&1)!=0) {
            if (a&0x2000) FORMAT(P(0x3a8),0x00a772e0,P(0x2e0),P(0x98));
            else FORMAT(P(0x3a8),0x00a77244,P(0x98),P(0x2e0));
        } else FORMAT(P(0x3a8),0x00a771c0,P(0x98),P(0x2e0));
        a=SD(0x18);B(copied)=2;b=SD(0x32);D(indices)=a;D(indices+4)=b;
        goto reason_done;
    }
    if ((s8)SB(0x13)>1) {
        bits=D(records+0x14);
        if (bits&0x200) {
            a=SD(0x1e8);d=SD(0x1f4);b=SD(0x1f0);c=SD(0x1ec);
            cm_fn_0053e140(c,b,d,P(0x98),0xc8,1,a);
            a=SD(0x202);d=SD(0x20e);b=SD(0x20a);c=SD(0x206);
            cm_fn_0053e140(c,b,d,P(0x2e0),0xc8,1,a);
            gender=(s8)B(second+0x52);
            FORMAT(P(0x3a8),0x00a7712c,gender,gender,P(0x98),P(0x2e0),name);
            a=SD(0x1e8);B(copied)=2;b=SD(0x202);D(indices)=a;D(indices+4)=b;
            goto reason_done;
        }
    }
    if (last_count!=0 && (D(records+0x14)&0x100)!=0) {
        a=SD(0x164);d=SD(0x170);b=SD(0x16c);c=SD(0x168);
        cm_fn_0053e140(c,b,d,P(0x98),0xc8,1,a);
        bits=SD(0x174);
        if (bits&0x10000) { format=0x00a770a4; goto row_reason; }
        if (bits&0x20000) { format=0x00a7700c; goto first_row_reason; }
        if (bits&0x8000) { format=0x00a76fb8; goto row_reason; }
        a=SW(0x178);
        if ((s16)a>160) {
            if ((bits&0x1000000)!=0 && (bits&0x200000)!=0) {
                gender=(s8)B(second+0x52);first_gender=(s8)B(first+0x52);
                FORMAT(P(0x3a8),0x00a76ea4,first_gender,first_gender,gender,gender,P(0x98),name,first+0x38);
                goto single_last;
            }
            if (bits&0x100000) { format=0x00a76dec; goto first_row_reason; }
            if (bits&4) { format=0x00a76d34; goto first_row_reason; }
            c=bits&0x20;
            if (c!=0 && (bits&2)!=0) { format=0x00a76c94; goto first_row_reason; }
            format=c?0x00a76bdc:0x00a76b48;goto first_row_reason;
        }
        if ((s16)a<100) {
            c=bits&0x80000;
            if (c!=0 && (bits&0x1000000)!=0 && (bits&0x200000)!=0) {
                format=0x00a76ae0;goto row_reason;
            }
            if (c!=0 && (bits&0x800000)!=0 && (bits&0x200000)!=0) {
                format=0x00a76a2c;goto first_row_reason;
            }
            if (c!=0 && (bits&0x800000)!=0) {format=0x00a7699c;goto first_row_reason;}
            if ((bits&0x400000)!=0 && (bits&0x1000000)!=0) {format=0x00a768ec;goto first_row_reason;}
            if (bits&4) {format=0x00a76854;goto first_row_reason;}
            c=bits&0x20;
            if (c!=0 && (bits&2)!=0) {format=0x00a767ac;goto first_row_reason;}
            if (c!=0) {
                gender=(s8)B(second+0x52);
                FORMAT(P(0x3a8),0x00a766d8,0x64,gender,gender,P(0x98),name);
                goto single_last;
            }
            format=0x00a7666c;goto row_reason;
        }
        if ((s16)a<125) {
            if (bits&0x80000) {format=0x00a765f8;goto row_reason;}
            c=bits&0x20;
            if (c!=0 && (bits&2)!=0) {format=0x00a76564;goto first_row_reason;}
            if (c!=0) {format=0x00a76494;goto first_row_reason;}
            format=0x00a7641c;goto row_reason;
        }
        if (bits&1) {format=0x00a763ac;goto row_reason;}
        c=bits&0x20;
        if (c!=0 && (bits&2)!=0) {format=0x00a76300;goto first_row_reason;}
        if (c!=0) {format=0x00a76230;goto first_row_reason;}
        format=0x00a761c0;goto row_reason;
first_row_reason:
        gender=(s8)B(first+0x52);
        FORMAT(P(0x3a8),format,gender,gender,P(0x98),first+0x38);
        goto single_last;
row_reason:
        FORMAT(P(0x3a8),format,P(0x98));
single_last:
        a=SD(0x164);B(copied)=1;D(indices)=a;
        goto reason_done;
    }
    value=SB(0x12);
    if (value!=0) {
        bits=SD(0x28);
        if ((bits&0x1000)!=0 && (D(records+0x14)&0x400)!=0) {
            a=SD(0x18);d=SD(0x24);b=SD(0x20);c=SD(0x1c);
            cm_fn_0053e140(c,b,d,P(0x98),0xc8,1,a);
            other=SB(0x42);
            FORMAT(P(0x3a8),(other&1)?0x00a76178:0x00a760f8,P(0x98));
            a=SD(0x18);B(copied)=1;D(indices)=a;
            goto reason_done;
        }
        a=D(records+0x14)&0x400;
        if (a!=0 && (bits&1)!=0) {
            a=SD(0x18);d=SD(0x24);b=SD(0x20);c=SD(0x1c);
            cm_fn_0053e140(c,b,d,P(0x98),0xc8,1,a);
            FORMAT(P(0x3a8),0x00a7607c,P(0x98));
            a=SD(0x18);B(copied)=1;D(indices)=a;
            goto reason_done;
        }
        if (a!=0 && (bits&2)!=0) {
            a=SD(0x18);d=SD(0x24);b=SD(0x20);c=SD(0x1c);
            cm_fn_0053e140(c,b,d,P(0x98),0xc8,1,a);
            FORMAT(P(0x3a8),0x00a75fe8,P(0x98));
            goto single_first;
        }
    }
    other=SB(0x13);
    if (other!=0) {
        a=SD(0x1e8);d=SD(0x1f4);b=SD(0x1f0);c=SD(0x1ec);
        cm_fn_0053e140(c,b,d,P(0x98),0xc8,1,a);
        bits=SD(0x1f8);
        if (bits&0x80) format=0x00a75f5c;
        else if (bits&0x80000) format=0x00a75edc;
        else format=0x00a75e70;
        gender=(s8)B(second+0x52);
        FORMAT(P(0x3a8),format,gender,gender,P(0x98),name);
        a=SD(0x1e8);B(copied)=1;D(indices)=a;
        goto reason_done;
    }
    if (value!=0) {
        a=SD(0x18);d=SD(0x24);b=SD(0x20);c=SD(0x1c);
        cm_fn_0053e140(c,b,d,P(0x98),0xc8,1,a);
        gender=(s8)B(second+0x52);
        FORMAT(P(0x3a8),0x00a75dec,gender,gender,P(0x98),name);
single_first:
        a=SD(0x18);B(copied)=1;D(indices)=a;
        goto reason_done;
    }
    value=B(flags+1);
    if (value!=0) {
        other=B(flags+2);
        if (other!=0 && B(flags)!=0) {format=0x00a75d48;goto fallback_mode;}
        if (value!=0) {
            other=B(flags+2);
            if (other!=0) goto fallback_pair;
            if (B(flags)==0) {format=0x00a75cd0;goto fallback_mode;}
        }
    }
    other=B(flags+2);
    if (other!=0) {
fallback_pair:
        if (B(flags)!=0) {
            if (value!=0) {format=0x00a75b88;goto fallback_mode;}
            gender=(s8)B(second+0x52);first_gender=(s8)B(first+0x52);
            FORMAT(P(0x904),0x00a75bfc,gender,gender,first_gender,first_gender,name,first+0x38);
            reason=P(0x904);goto append_reason;
        }
    }
    if (value!=0) {format=0x00a75b88;goto fallback_mode;}
    value=B(flags+4);
    if (value!=0) {format=0x00a75aec;goto fallback_mode;}
    value=B(flags+9);
    if (value==0) goto combine;
    format=0x00a75a74;
fallback_mode:
    gender=(s8)B(second+0x52);
    FORMAT(P(0x904),format,gender,gender,name);
    reason=P(0x904);goto append_reason;
reason_done:
    SB(0x10)=1;
combine:
    value=SB(0x11); other=SB(0x10);
    if (value!=0) {
        if (other!=0) {
            cm_fn_00944c9f(destination,0x00a6a4e0,P(0x165c),P(0x904),P(0x3a8));
            return;
        }
        reason=P(0x904);goto append_reason;
    }
    if (other!=0) {reason=P(0x3a8);goto append_reason;}
    cm_fn_00944c9f(destination,0x00991944,P(0x165c));
    return;
append_reason:
    cm_fn_00944c9f(destination,0x00991948,P(0x165c),reason);
}

/* Preferred-image VA00460590/base00400000; CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL2/RET0/full EAX. Native root VA00460590..00467190.
   Normal B=L-10, uninitialized scratch starts at B+10. The linked SEH
   frame is separate C storage; every native state write addresses it.
   Four actual return paths remain distinct. Callees stay original boundaries;
   all field/getter rows retain their individual read/store/carry order. */
#include "cmre.h"
#define D(a) (*(volatile u32 *)(a))
typedef u16 display_word __attribute__((may_alias));
#define W(a) (*(volatile display_word *)(a))
#define BYTE(a) (*(volatile u8 *)(a))
typedef float display_float __attribute__((may_alias));
#define F32(a) (*(volatile display_float *)(a))
extern u32 CM_THISCALL cm_fn_007ff010(u32,u32);
extern u32 CM_CDECL cm_fn_0053b900(u32);
extern u32 CM_CDECL cm_fn_00600c90(u32,u32,u32);
extern u32 CM_THISCALL cm_fn_005a0590(u32,u32);
extern u32 CM_CDECL cm_fn_005e4800(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32,u32,...);
extern u32 CM_CDECL cm_fn_00460450(u32,u32);
extern u32 CM_CDECL cm_fn_005e5cd0(u32);
extern u32 CM_CDECL cm_fn_005e5ad0(u32,u32);
extern u32 CM_CDECL cm_fn_00544c70(u32,u32,u32);
extern u32 CM_CDECL cm_fn_0053dba0(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0053ad00(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00945333(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_THISCALL cm_fn_00403320(u32,u32);
extern u32 CM_THISCALL cm_fn_007ff270(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00487840(u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_0043f460(u32,u32);
extern u32 CM_THISCALL cm_fn_0043f4c0(u32,u32,u32);
extern u32 CM_CDECL cm_fn_00441c10(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0053d7d0(u32);
extern u32 CM_CDECL cm_fn_0043f440(void);
extern unsigned long long CM_THISCALL cm_fn_00598800(u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_004671a0(u32,u32);
extern u32 CM_THISCALL cm_fn_004671b0(u32);
extern u32 CM_THISCALL cm_fn_004671c0(u32);
extern u32 CM_THISCALL cm_fn_004671d0(u32);
extern u32 CM_THISCALL cm_fn_004671e0(u32);
extern u32 CM_THISCALL cm_fn_00450aa0(u32,u32,u32,u32);
extern void CM_THISCALL cm_fn_005a53c0(u32);
extern u32 CM_CDECL cm_fn_0053b3d0(u32);
extern u32 CM_STDCALL cm_fn_00775790(u32);
extern u32 CM_THISCALL cm_fn_005986b0(u32);
extern u32 CM_THISCALL cm_fn_00804100(u32,
    u32,u32,u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_008040a0(u32,
    u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00804030(u32,
    u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_0055fba0(u32,
    u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_00803fd0(u32,
    u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32);
/* Real EAX/ECX inputs, eighteen stack arguments and RET48. Incoming EDX
   cannot affect EAX/memory/arguments; its entire returned EDX is
   dead at all fourteen direct root sites (display-direct-draw-edx.json).
   The zero below is a proved unused input, never a fabricated live value. */
extern u32 CM_STDCALL __attribute__((regparm(3))) cm_fn_0055f990(
    u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,
    u32,u32,u32,u32,u32,u32);

/* The native scans include NUL, then copy DWORDs followed by residual bytes.
   Do not use strlen/memcpy: retain access widths, order and fault boundaries. */
#define COPY_SCANNED(destination,source) do { \
    u32 scan_=(source),left_=0xffffffffu,length_,from_,to_,count_,value_; \
    u8 byte_; \
    while(left_!=0){byte_=BYTE(scan_);++scan_;--left_;if(byte_==0)break;} \
    length_=~left_;from_=scan_-length_;to_=(destination); \
    count_=length_>>2; \
    while(count_!=0){value_=D(from_);D(to_)=value_;from_+=4;to_+=4;--count_;} \
    count_=length_&3u; \
    while(count_!=0){byte_=BYTE(from_);BYTE(to_)=byte_;++from_;++to_;--count_;} \
} while(0)

/* Label and value columns have different native colors/fonts. The caller
   formats first; these macros retain the two subsequent WORD read orders. */
#define DRAW_LABEL(row) do { \
    active=W(0x00ae3184);shade=W(0x00aebdea); \
    draw=cm_fn_00804100(0x00b74450,tab,0,(row),1,shade,1,3,active,0x00de1f64); \
} while(0)
#define DRAW_VALUE(row,value) do { \
    pointer=(value);text=W(0x00aebdec);shade=W(0x00aebdea); \
    draw=cm_fn_00804100(0x00b74450,tab,1,(row),1,shade,1,2,text,pointer); \
} while(0)
#define STAT_LABEL(row) do { \
    active=W(0x00ae3184);shade=W(0x00aebdea); \
    draw=cm_fn_00804100(0x00b74450,tab,0,(row),1,shade,1,2,active,0x00de1f64); \
} while(0)
/* Original flag pointer is reloaded for each metric. Format precedes the
   packed DWORD read. Byte-only mode writes retain the other scratch bits. */
#define WAGE_METRIC(slot,annual,monthly,weekly,row) do { \
    pointer=D(0x00ae3138);flags=D(pointer+4u); \
    if((flags&0x200000u)!=0){ \
        cm_fn_0066f4e0(0x00de1f64,(annual)); \
        first=D(b+(slot));D(b+0x2c)=first;BYTE(b+0x14)=8; \
    }else if((flags&0x400000u)!=0){ \
        cm_fn_0066f4e0(0x00de1f64,(monthly)); \
        first=D(b+(slot));BYTE(b+0x14)=8;D(b+0x2c)=first*4u; \
    }else{ \
        cm_fn_0066f4e0(0x00de1f64,(weekly)); \
        first=D(b+(slot));BYTE(b+0x14)=4;D(b+0x2c)=first*52u; \
    } \
    STAT_LABEL(row);mode=D(b+0x14); \
    pair=cm_fn_0043f460(b+0x2c,mode); \
    cm_fn_00441c10(0x00de1f64,0x7d0,(u32)pair,(u32)(pair>>32),mode); \
    DRAW_VALUE(row,0x00de1f64); \
} while(0)
#define SCAN_LENGTH(source,length) do { \
    u32 scan_=(source),left_=0xffffffffu;u8 byte_; \
    while(left_!=0){byte_=BYTE(scan_);++scan_;--left_;if(byte_==0)break;} \
    (length)=~left_-1u; \
} while(0)
/* These two record metrics retain the last cached EBX mode from the third
   WAGE_METRIC. They do not write the mode byte again. Both native scans and
   the record-pointer reload after each original call remain explicit. */
#define WAGE_RECORD(slot,annual,monthly,weekly,row,record_slot) do { \
    pointer=D(0x00ae3138);flags=D(pointer+4u); \
    if((flags&0x200000u)!=0){ \
        cm_fn_0066f4e0(0x00de1f64,(annual)); \
        first=D(b+(slot));D(b+0x2c)=first; \
    }else if((flags&0x400000u)!=0){ \
        cm_fn_0066f4e0(0x00de1f64,(monthly)); \
        first=D(b+(slot));D(b+0x2c)=first*4u; \
    }else{ \
        cm_fn_0066f4e0(0x00de1f64,(weekly)); \
        first=D(b+(slot));D(b+0x2c)=first*52u; \
    } \
    STAT_LABEL(row);pointer=D(b+(record_slot)); \
    if(pointer!=0){ \
        pair=cm_fn_0043f460(b+0x2c,mode); \
        cm_fn_00441c10(0x00de1f64,0x7d0,(u32)pair,(u32)(pair>>32),mode); \
        SCAN_LENGTH(0x00de1f64,length); \
        cm_fn_0066f4e0(0x00de1f64u+length,0x0098fd10); \
        SCAN_LENGTH(0x00de1f64,length);pointer=D(b+(record_slot)); \
        cm_fn_0053dba0(pointer,0x00de1f64u+length,0x7d0,1); \
        pointer=D(b+(record_slot));text=W(0x00aebdec);shade=W(0x00aebdea); \
        draw=cm_fn_008040a0(0x00b74450,tab,1,(row),1,shade,1,2,text, \
            0x00de1f64,0x7d7,pointer); \
    }else{DRAW_VALUE(row,0x0098ba0c);} \
} while(0)
#define RECORD_NUMBER(slot,field,signed_byte,row) do { \
    pointer=D(b+(slot)); \
    if(pointer!=0){ \
        if(signed_byte)first=(u32)(s32)(s8)BYTE(pointer+(field)); \
        else first=BYTE(pointer+(field)); \
        cm_fn_00944c9f(0x00de1f64,0x0098fcbc,first); \
        pointer=D(b+(slot));SCAN_LENGTH(0x00de1f64,length); \
        cm_fn_0053dba0(pointer,0x00de1f64u+length,0x7d0,1); \
        pointer=D(b+(slot));text=W(0x00aebdec);shade=W(0x00aebdea); \
        draw=cm_fn_008040a0(0x00b74450,tab,1,(row),1,shade,1,2,text, \
            0x00de1f64,0x7d7,pointer); \
    }else{DRAW_VALUE(row,0x0098ba0c);} \
} while(0)
#define COUNT_STAT(slot,row) do { \
    first=(u32)(s32)(s8)BYTE(b+(slot)); \
    cm_fn_00944c9f(0x00de1f64,0x0098806c,first); \
    DRAW_VALUE(row,0x00de1f64); \
} while(0)
#define FIN_HEADER(column,style,font) do { \
    active=W(0x00aebdd4);shade=W(0x00aebdea); \
    draw=cm_fn_00804100(0x00b74450,tab,(column),0,1,shade,(style),(font),active,0x00de1f64); \
} while(0)
/* Interleaved load/add order of the independently matched nine-field sums. */
#define SUM9(o0,o1,o2,o3,o4,o5,o6,o7,o8) do { \
    total=D(finance+(o0));first=D(finance+(o1)); \
    second=D(finance+(o2));third=D(finance+(o3)); \
    total+=first;first=D(finance+(o4));total+=second; \
    second=D(finance+(o5));total+=third;third=D(finance+(o6)); \
    total+=first;first=D(finance+(o7));total+=second; \
    second=D(finance+(o8));total+=third;total+=first;total+=second; \
    D(b+0x10)=total; \
} while(0)
#define FIN_FORMAT_MODE(primary) do { \
    pair=cm_fn_0043f460(b+0x10,(primary)); \
    cm_fn_00441c10(0x00de1f64,0x7d0,(u32)pair,(u32)(pair>>32),(primary)); \
    SCAN_LENGTH(0x00de1f64,length); \
    if(length>0xdu){ \
        pair=cm_fn_0043f460(b+0x10,0x21); \
        cm_fn_00441c10(0x00de1f64,0x7d0,(u32)pair,(u32)(pair>>32),0x21); \
    } \
} while(0)
#define FIN_FORMAT() FIN_FORMAT_MODE(0x22)
#define INCOME_CAPTURE(low,high,column) do { \
    pair=cm_fn_0043f460(b+0x10,0x20);D(b+(high))=(u32)(pair>>32); \
    text=W(0x00aebdec);D(b+(low))=(u32)pair;shade=W(0x00aebdea); \
    draw=cm_fn_00804100(0x00b74450,tab,(column),2,1,shade,finance_mode,2,text,0x00de1f64); \
} while(0)
#define EXPENSE_CAPTURE(low,high,column) do { \
    pair=cm_fn_0043f460(b+0x10,0x20);text=W(0x00aebdec); \
    D(b+(high))=(u32)(pair>>32);shade=W(0x00aebdea);D(b+(low))=(u32)pair; \
    draw=cm_fn_00804100(0x00b74450,tab,(column),3,1,shade,finance_mode,2,text,0x00de1f64); \
} while(0)
#define FIN_NEGATIVE_FORMAT() do { \
    pair=cm_fn_0043f460(b+0x10,0x22); \
    cm_fn_00441c10(b+0xac,0xc8,(u32)pair,(u32)(pair>>32),0x22); \
    SCAN_LENGTH(b+0xac,length); \
    if(length>0xcu){ \
        pair=cm_fn_0043f460(b+0x10,0x21); \
        cm_fn_00441c10(b+0xac,0x7d0,(u32)pair,(u32)(pair>>32),0x21); \
    } \
    cm_fn_00944c9f(0x00de1f64,0x0098fb04,b+0xac); \
} while(0)
/* Native SUB/SBB then JS selects the negative branch. On the other path,
   JG or TEST-low/JB both reach positive: TEST clears CF, so that JB never
   fires. Preserve this bug, including high-word overflow behavior. */
#define FIN_DIFFERENCE(in_lo,out_lo,in_hi,out_hi,column) do { \
    first=D(b+(in_lo));second=D(b+(out_lo)); \
    third=D(b+(in_hi));fourth=D(b+(out_hi)); \
    low=first-second;high=third-fourth-(first<second); \
    if((high&0x80000000u)!=0){ \
        high=0u-high-(low!=0);low=0u-low; \
        cm_fn_0043f4c0(b+0x10,low,high);FIN_NEGATIVE_FORMAT(); \
        text=W(0x00ae31a8);shade=W(0x00aebdea); \
    }else{ \
        cm_fn_0043f4c0(b+0x10,low,high);FIN_FORMAT(); \
        text=W(0x00adab48);shade=W(0x00aebdea); \
    } \
    draw=cm_fn_00804100(0x00b74450,tab,(column),5,1,shade,finance_mode,2,text,0x00de1f64); \
} while(0)
/* The bit4 field rows share formatting, but retain seven distinct native
   arithmetic/read/store orders. CF is captured before partial MOVAX/DX/CX
   color reads; it is never reconstructed from the overwritten register. */
#define FIELD_VALUE(field) do { \
    first=D(finance+(field));D(b+0x10)=first;FIN_FORMAT(); \
    pair=cm_fn_0043f460(b+0x10,0x20); \
} while(0)
#define FIELD_DRAW(column,row) \
    draw=cm_fn_00804100(0x00b74450,tab,(column),(row),1,shade,finance_mode,2,text,0x00de1f64)
#define FIELD_LOAD(lo,hi) do {low=D(b+(lo));high=D(b+(hi));} while(0)
#define FIELD_ADD_LOW() do {old=low;low+=(u32)pair;carry=low<old;} while(0)
#define FIELD_ADD_HIGH() do {high+=(u32)(pair>>32)+carry;} while(0)
#define FIELD_BASE(field,lo,hi,column,row) do { \
    FIELD_VALUE(field);shade=W(0x00aebdea);D(b+(lo))=(u32)pair; \
    text=W(0x00aebdec);D(b+(hi))=(u32)(pair>>32);FIELD_DRAW(column,row); \
} while(0)
#define FIELD_ADD_A(field,lo,hi,column,row) do { \
    FIELD_VALUE(field);FIELD_LOAD(lo,hi);FIELD_ADD_LOW(); \
    shade=W(0x00aebdea);FIELD_ADD_HIGH();text=W(0x00aebdec); \
    D(b+(lo))=low;D(b+(hi))=high;FIELD_DRAW(column,row); \
} while(0)
#define FIELD_ADD_B(field,lo,hi,column,row) do { \
    FIELD_VALUE(field);FIELD_LOAD(lo,hi);FIELD_ADD_LOW();FIELD_ADD_HIGH(); \
    shade=W(0x00aebdea);D(b+(hi))=high;text=W(0x00aebdec); \
    D(b+(lo))=low;FIELD_DRAW(column,row); \
} while(0)
#define FIELD_ADD_C(field,lo,hi,column,row) do { \
    FIELD_VALUE(field);FIELD_LOAD(lo,hi);FIELD_ADD_LOW();text=W(0x00aebdec); \
    FIELD_ADD_HIGH();D(b+(hi))=high;shade=W(0x00aebdea); \
    D(b+(lo))=low;FIELD_DRAW(column,row); \
} while(0)
#define FIELD_ADD_D(field,lo,hi,column,row) do { \
    FIELD_VALUE(field);FIELD_LOAD(lo,hi);FIELD_ADD_LOW();text=W(0x00aebdec); \
    D(b+(lo))=low;shade=W(0x00aebdea);FIELD_ADD_HIGH(); \
    D(b+(hi))=high;FIELD_DRAW(column,row); \
} while(0)
#define FIELD_ADD_E(field,lo,hi,column,row) do { \
    FIELD_VALUE(field);FIELD_LOAD(lo,hi);FIELD_ADD_LOW();D(b+(lo))=low; \
    text=W(0x00aebdec);FIELD_ADD_HIGH();shade=W(0x00aebdea); \
    D(b+(hi))=high;FIELD_DRAW(column,row); \
} while(0)
#define FIELD_LAST(field,lo,hi,column,row) do { \
    FIELD_VALUE(field);FIELD_LOAD(lo,hi);text=W(0x00aebdec);FIELD_ADD_LOW(); \
    FIELD_ADD_HIGH();last_total=low;shade=W(0x00aebdea); \
    D(b+(hi))=high;FIELD_DRAW(column,row); \
} while(0)
#define FIN_TOTAL(column) do { \
    cm_fn_0043f4c0(b+0x10,low,high);FIN_FORMAT(); \
    text=W(0x00aebde8);shade=W(0x00aebdea); \
    draw=cm_fn_00804100(0x00b74450,tab,(column),12,1,shade,finance_mode,2,text,0x00de1f64); \
} while(0)
#define WRAPPED_FORMAT() do { \
    cm_fn_00450aa0(b+0x10,0x00de1f64,0x7d0,0x22); \
    SCAN_LENGTH(0x00de1f64,length); \
    if(length>0xdu)cm_fn_00450aa0(b+0x10,0x00de1f64,0x7d0,0x21); \
} while(0)
#define GET_VALUE(getter,field) do { \
    pointer=(getter)(finance);first=D(pointer+(field)); \
    cm_fn_004671a0(b+0x10,first);WRAPPED_FORMAT(); \
    pair=cm_fn_0043f460(b+0x10,0x20); \
} while(0)
#define GET_ADD_A(getter,field,lo,hi,column,row) do { \
    GET_VALUE(getter,field);FIELD_LOAD(lo,hi);FIELD_ADD_LOW(); \
    shade=W(0x00aebdea);FIELD_ADD_HIGH();text=W(0x00aebdec); \
    D(b+(lo))=low;D(b+(hi))=high;FIELD_DRAW(column,row); \
} while(0)
#define GET_ADD_B(getter,field,lo,hi,column,row) do { \
    GET_VALUE(getter,field);FIELD_LOAD(lo,hi);FIELD_ADD_LOW();FIELD_ADD_HIGH(); \
    shade=W(0x00aebdea);D(b+(hi))=high;text=W(0x00aebdec); \
    D(b+(lo))=low;FIELD_DRAW(column,row); \
} while(0)
#define GET_ADD_C(getter,field,lo,hi,column,row) do { \
    GET_VALUE(getter,field);FIELD_LOAD(lo,hi);FIELD_ADD_LOW();text=W(0x00aebdec); \
    FIELD_ADD_HIGH();D(b+(hi))=high;shade=W(0x00aebdea); \
    D(b+(lo))=low;FIELD_DRAW(column,row); \
} while(0)
#define GET_ADD_D(getter,field,lo,hi,column,row) do { \
    GET_VALUE(getter,field);FIELD_LOAD(lo,hi);FIELD_ADD_LOW();text=W(0x00aebdec); \
    D(b+(lo))=low;shade=W(0x00aebdea);FIELD_ADD_HIGH(); \
    D(b+(hi))=high;FIELD_DRAW(column,row); \
} while(0)
#define GET_ADD_E(getter,field,lo,hi,column,row) do { \
    GET_VALUE(getter,field);FIELD_LOAD(lo,hi);FIELD_ADD_LOW();D(b+(lo))=low; \
    text=W(0x00aebdec);FIELD_ADD_HIGH();shade=W(0x00aebdea); \
    D(b+(hi))=high;FIELD_DRAW(column,row); \
} while(0)
#define GET_LAST(getter,field,lo,hi,column,row) do { \
    GET_VALUE(getter,field);low=D(b+(lo));shade=W(0x00aebdea); \
    high=D(b+(hi));FIELD_ADD_LOW();text=W(0x00aebdec); \
    FIELD_ADD_HIGH();last_total=low;D(b+(hi))=high;FIELD_DRAW(column,row); \
} while(0)
#define WRAPPED_TOTAL(column) do { \
    cm_fn_0043f4c0(b+0x10,low,high);WRAPPED_FORMAT(); \
    text=W(0x00aebde8);shade=W(0x00aebdea); \
    draw=cm_fn_00804100(0x00b74450,tab,(column),12,1,shade,finance_mode,2,text,0x00de1f64); \
} while(0)

/* One of five native tab pairs. Original word-read order is shade, text,
   active, followed by signed page, then receiver. Only the selected branch
   copies the label. All projected color/font/index upper bits are proved
   unobserved through 55F990 -> 5EE150 and its font helpers. */
#define TAB_PAIR(mask) do { \
    shade=W(0x00aebdea);text=W(0x00aebdec);active=W(0x00ae31aa); \
    page=(u32)(s32)(s16)W(0x00b77c7a); \
    owner=D(0x00b74c4cu+page*768u); \
    cm_fn_0055f990(page,0,owner,selected?0x2002:2, \
        0,0,0,0,0,0,0x10,active,text,1,1,shade,0x00de1f64, \
        0,0x20,(mask),(u16)tab); \
    if(selected){COPY_SCANNED(b+0xac,0x00de1f64); \
        top=arg1;record=D(b+0x1c);} \
} while(0)

u32 CM_CDECL cm_fn_00460590(volatile u32 arg1,volatile u32 arg2)
{
    volatile struct {u32 prev,handler,state;} frame;
    volatile u8 scratch[0x2bc];
    u32 b=(u32)scratch-0x10u;
    u32 previous,initial,flags,record,finance,top,pointer,draw,tab;
    u32 shade,text,active,page,owner,selected,bottom,nation,kind,result;
    u32 stadium,candidate,bound,index,city,first,second,third,capacity,seats,format;
    u32 numerator,count,last,row,plain,classification,code,offset;
    u32 mode,length,finance_mode,total,fourth,low,high,old,carry,last_total;
    unsigned long long pair;
    float comparison,zero;
    double promoted;
    volatile s32 divisor;
    previous=CM_FS(volatile u32,0); /* 460590 before frame/global access. */
    frame.state=0xffffffffu;frame.handler=0x00955ab1;frame.prev=previous;
    initial=BYTE(0x00adab32);
    CM_FS(volatile u32,0)=(u32)&frame;
    if((initial&1u)==0){
        BYTE(0x00adab32)=(u8)(initial|1u);
        draw=cm_fn_005e4800(0,0xc0,0,0);
        W(0x00adab48)=(u16)draw;
    }
    BYTE(b+0x5a)=2;BYTE(b+0x5b)=5;BYTE(b+0x58)=2;
    BYTE(b+0x59)=3;BYTE(b+0x5c)=2;BYTE(b+0x5d)=1;
    BYTE(b+0x5e)=1;BYTE(b+0x5f)=1;BYTE(b+0x60)=1;
    D(b+0x10)=0;frame.state=0;D(b+0x2c)=0;
    BYTE((u32)&frame.state)=1;
    flags=cm_fn_007ff010(0x00b74450,0x1b);D(b+0x14)=flags;
    record=cm_fn_007ff010(0x00b74450,0);D(b+0x1c)=record;
    draw=cm_fn_007ff010(0x00b74450,1);D(b+0x30)=draw;
    if((flags&0x11u)==0){
        if(cm_fn_0053b900(record)!=0)D(b+0x14)=1;
        if(cm_fn_00600c90(record,0,1)==0)D(b+0x14)=1;
    }
    pointer=D(0x00ae2a38);finance=cm_fn_005a0590(pointer,record);
    cm_fn_0066f4e0(0x00de1f64,0x00988004);
    active=W(0x00ae3184);top=arg1;shade=W(0x00ae317e);
    draw=cm_fn_00804030(0x00b74450,0x6e,top,0xeb,top+0x14u,
        0x30,shade,0xc,1,active,0x00de1f64,0,0);
    text=W(0x00ae31aa);page=(u32)(s32)(s16)W(0x00b77c7a);
    owner=D(0x00b74c4cu+page*768u);
    tab=cm_fn_0055fba0(owner,0,0,0,0,0,0,0,0,0x130,text,(u16)draw);
    cm_fn_0066f4e0(0x00de1f64,0x00990598);
    /* Native 460731 writes the first selection before its tab call. */
    selected=D(b+0x14)&1u;D(b+0x24)=selected;
    TAB_PAIR(1);
    cm_fn_0066f4e0(0x00de1f64,0x0098ee70);
    selected=D(b+0x14)&0x10u;D(b+0x38)=selected;TAB_PAIR(0x10);
    if(cm_fn_00600c90(record,0,1)!=0){
        if(cm_fn_0053b900(record)==0){
            cm_fn_0066f4e0(0x00de1f64,0x0099058c);
            selected=BYTE(b+0x14)&2u;TAB_PAIR(2);
            cm_fn_0066f4e0(0x00de1f64,0x00990584);
            selected=BYTE(b+0x14)&4u;TAB_PAIR(4);
            cm_fn_0066f4e0(0x00de1f64,0x00990574);
            selected=BYTE(b+0x14)&8u;TAB_PAIR(8);
        }
    }
    text=W(0x00aebdec);top+=0x19u;
    draw=cm_fn_00803fd0(0x00b74450,0x6e,top,0x30c,top+0x23u,
        2,0,0xc,3,text,b+0xac,1);
    selected=D(b+0x24);top+=0x28u;arg1=top;
    if(selected==0)goto L4619d3;
    page=(u32)(s32)(s16)W(0x00b77c7a);bottom=top+0x8eu;
    owner=D(0x00b74c4cu+page*768u);
    draw=cm_fn_0055fba0(owner,0x6e,top,0x30c,bottom,2,b+0x5a,6,0,2,0,0xffff);
    tab=draw;
    record=D(b+0x1c);draw=cm_fn_00460450(record,b+0x1c8);
    if(draw!=0){
        page=(u32)(s32)(s16)W(0x00b77c7a);
        owner=D(0x00b74c4cu+page*768u);
        draw=cm_fn_0055fba0(owner,0x27e,top,0x30c,bottom,1,0,1,0,1,0,0xffff);
        pointer=draw;page=(u32)(s32)(s16)W(0x00b77c7a);
        owner=D(0x00b74c4cu+page*768u);
        draw=cm_fn_0055f990(page,0,owner,0x400,0,0,0,0,0,0,1,
            0,0,0xc,1,0,b+0x1c8,0,0,0,(u16)pointer);
    }
    record=D(b+0x1c);
    if(cm_fn_0053b900(record)!=0){
        cm_fn_0066f4e0(0x00de1f64,0x00990568);DRAW_LABEL(0);
        nation=D(record+0x53);pointer=D(nation+0x71);
        if(pointer!=0){DRAW_VALUE(0,pointer+4u);goto L460e4a;}
        cm_fn_0066f4e0(0x00de1f64,0x00990544,0x00ad9c64);
        DRAW_VALUE(0,0x00de1f64);
    }else{
        cm_fn_0066f4e0(0x00de1f64,0x00990538);DRAW_LABEL(0);
        kind=D(record);result=D(0x009d0430);
        if(kind==result || kind==D(0x009d0434) || kind==D(0x009d043c)){
            cm_fn_0066f4e0(0x00de1f64,0x009904f4,0x00ad9c64);
            DRAW_VALUE(0,0x00de1f64);
        }else{
            nation=D(record+0x53);
            if(nation!=0){
                result=cm_fn_0053b3d0(nation);
                text=W(0x00aebdec);shade=W(0x00aebdea);
                nation=D(record+0x53);
                draw=cm_fn_008040a0(0x00b74450,tab,1,0,1,shade,1,2,
                    text,nation+4u,0x7d5,result);
            }else{
                cm_fn_0066f4e0(0x00de1f64,0x00990518,0x00ad9c64);
                DRAW_VALUE(0,0x00de1f64);
            }
        }
    }
L460e4a:
    cm_fn_0066f4e0(0x00de1f64,0x009904c0,0x00ad9c64);DRAW_LABEL(1);
    pointer=D(0x00b74318); /* Original read; 775790 has no ECX input. */
    if(cm_fn_00775790(record)!=0){
        COPY_SCANNED(0x00de1f64,0x0098ba0c);record=D(b+0x1c);
    }else{
        kind=BYTE(record+0x64);
        if(kind==1)cm_fn_0066f4e0(0x00de1f64,0x009904b0);
        else if(kind==2)cm_fn_0066f4e0(0x00de1f64,0x0099049c);
        else if(kind==3)cm_fn_0066f4e0(0x00de1f64,0x00990494);
        else cm_fn_0066f4e0(0x00de1f64,0x00990460,0x00ad9c64);
    }
    DRAW_VALUE(1,0x00de1f64);
    cm_fn_0066f4e0(0x00de1f64,0x00990454);DRAW_LABEL(2);
    pointer=D(0x00b74318);
    if(cm_fn_00775790(record)!=0){
        COPY_SCANNED(0x00de1f64,0x0098ba0c);record=D(b+0x1c);
    }else{
        /* Each threshold query is a distinct original call. Last path can
           retain the preexisting formatted label; never add a blank default. */
        if((u8)cm_fn_005986b0(finance)==2)
            cm_fn_0066f4e0(0x00de1f64,0x0099044c);
        else if((u8)cm_fn_005986b0(finance)==1)
            cm_fn_0066f4e0(0x00de1f64,0x00990444);
        else if((u8)cm_fn_005986b0(finance)==0)
            cm_fn_0066f4e0(0x00de1f64,0x00990428,0x00ad9c64);
        else{
            if((u8)cm_fn_005986b0(finance)!=0xff){
                if(BYTE(finance+0x165)==1){
                    if((u8)cm_fn_005986b0(finance)==0xfe)
                        cm_fn_0066f4e0(0x00de1f64,0x0099041c);
                    goto L46106d;
                }
            }
            cm_fn_0066f4e0(0x00de1f64,0x009903f8,0x00ad9c64);
        }
    }
L46106d:
    DRAW_VALUE(2,0x00de1f64);
    cm_fn_0066f4e0(0x00de1f64,0x009903ec);DRAW_LABEL(3);
    kind=BYTE(record+0x6d);stadium=D(record+0x69);
    if(kind==0){
        /* Both table globals are cached once, before any original call. */
        bound=D(0x00ae235c);candidate=D(0x00ae23b4);index=0;
        if((s32)bound>0){
            do{
                if(candidate!=record){
                    if(D(candidate+0x69)==stadium){
                        if(BYTE(candidate+0x6d)!=0)goto L461147;
                    }
                }
                ++index;candidate+=0x245u;
            }while((s32)index<(s32)bound);
        }
        if(stadium==0)goto L4611d7;
        city=D(stadium+0x38);
        if(city!=0){
            first=(u32)(s32)(s8)BYTE(city+0x1e);
            second=(u32)(s32)(s8)BYTE(stadium+0x37);
            cm_fn_0066f4e0(0x00de1f64,0x009903b4,second,second,first,first,
                stadium+4u,city+4u);
        }else{
            first=(u32)(s32)(s8)BYTE(stadium+0x37);
            cm_fn_0066f4e0(0x00de1f64,0x009902c8,first,first,stadium+4u);
        }
        goto L46127a;
L461147:
        if(stadium==0)goto L4611d7;
        city=D(stadium+0x38);third=(u32)(s32)(s8)BYTE(candidate+0x52);
        if(city!=0){
            first=(u32)(s32)(s8)BYTE(city+0x1e);
            second=(u32)(s32)(s8)BYTE(stadium+0x37);
            cm_fn_0066f4e0(0x00de1f64,0x00990348,second,second,first,first,
                third,third,stadium+4u,city+4u,candidate+0x38u);
            record=D(b+0x1c);
        }else{
            first=(u32)(s32)(s8)BYTE(stadium+0x37);
            cm_fn_0066f4e0(0x00de1f64,0x009902ec,first,first,third,third,
                stadium+4u,candidate+0x38u);
        }
        goto L46127a;
    }
    if(stadium!=0){
        city=D(stadium+0x38);
        if(city!=0){
            first=(u32)(s32)(s8)BYTE(city+0x1e);
            second=(u32)(s32)(s8)BYTE(stadium+0x37);
            cm_fn_0066f4e0(0x00de1f64,0x009902a4,second,second,first,first,
                stadium+4u,city+4u);
        }else{
            COPY_SCANNED(0x00de1f64,stadium+4u);record=D(b+0x1c);
        }
        goto L46127a;
    }
L4611d7:
    if(cm_fn_0053b900(record)!=0)cm_fn_0066f4e0(0x00de1f64,0x0099029c);
    else cm_fn_0066f4e0(0x00de1f64,0x0099027c,0x00ad9c64);
L46127a:
    DRAW_VALUE(3,0x00de1f64);
    cm_fn_0066f4e0(0x00de1f64,0x0099026c);DRAW_LABEL(4);
    if(stadium==0)goto L461468;
    capacity=D(stadium+0x3c);
    if(capacity==0)goto L461468;
    seats=D(stadium+0x40);
    if((s32)seats>=(s32)capacity){
        first=BYTE(stadium+0x4c);
        if(first!=0){
            second=BYTE(stadium+0x4d);
            format=second?0x00990220:0x009901ec;
        }else{
            second=BYTE(stadium+0x4d);
            format=second?0x009901b8:0x00990198;
        }
        cm_fn_0066f4e0(0x00de1f64,format,capacity);
    }else{
        seats=D(stadium+0x40); /* Original second load on this branch. */
        if(seats!=0){
            first=BYTE(stadium+0x4c);
            if(first!=0){
                second=BYTE(stadium+0x4d);
                format=second?0x00990138:0x009900f0;
            }else{
                second=BYTE(stadium+0x4d);
                format=second?0x009900a4:0x00990070;
            }
            cm_fn_0066f4e0(0x00de1f64,format,capacity,seats);
        }else{
            first=BYTE(stadium+0x4c);
            if(first!=0){
                second=BYTE(stadium+0x4d);
                format=second?0x00990030:0x00990008;
            }else{
                second=BYTE(stadium+0x4d);
                format=second?0x0098ffdc:0x0098ffc8;
            }
            cm_fn_0066f4e0(0x00de1f64,format,capacity);
        }
    }
    goto L461491;
L461468:
    COPY_SCANNED(0x00de1f64,0x0098ba0c);record=D(b+0x1c);
L461491:
    DRAW_VALUE(4,0x00de1f64);
    if(cm_fn_0053b900(record)==0){
        cm_fn_0066f4e0(0x00de1f64,0x0098ffb4);DRAW_LABEL(5);
        kind=BYTE(record+0x7f);
        if((s8)kind>15)format=0x0098ff90;
        else{
            first=(u32)(s32)(s8)kind;if((s32)first<=1)first=1;
            capacity=(u32)(s32)(s16)W(record+0x80);
            /* Native repeats four signed divisions. Denominator is 1..15;
               the signed WORD numerator cannot overflow their quotient. */
            if((s32)capacity/(s32)first<=0x190)format=0x0098ff80;
            else if((s32)capacity/(s32)first<=0x258)format=0x0098ff6c;
            else if((s32)capacity/(s32)first<=0x320)format=0x0098ff54;
            else if((s32)capacity/(s32)first<=0x3e8)format=0x0098ff3c;
            else format=0x0098ff28;
        }
        cm_fn_0066f4e0(0x00de1f64,format);DRAW_VALUE(5,0x00de1f64);
    }
    top=arg1+0x93u;draw=cm_fn_005e5cd0(3);bottom=arg2;
    divisor=(s32)draw;numerator=bottom-top-0x10u;
    BYTE(b+0x24)=(u8)((s32)numerator/divisor);
    first=D(b+0x24);page=(u32)(s32)(s16)W(0x00b77c7a);
    owner=D(0x00b74c4cu+page*768u);
    tab=cm_fn_0055fba0(owner,0x6e,top,0x30c,bottom,2,b+0x58,first,0,2,0,0xffff);
    cm_fn_0066f4e0(0x00de1f64,0x0098ff14);
    result=cm_fn_005e5ad0(3,0x00de1f64);
    text=W(0x00aebdd4);page=(u32)(s32)(s16)W(0x00b77c7a);
    result+=0x73u;owner=D(0x00b74c4cu+page*768u);
    draw=cm_fn_0055f990(page,0,owner,1,0x73,0,result,0,0,0,1,
        0,0,1,3,text,0x00de1f64,0,0,0,(u16)tab);
    active=W(0x00aebdd4);page=(u32)(s32)(s16)W(0x00b77c7a);
    owner=D(0x00b74c4cu+page*768u);
    draw=cm_fn_0055f990(page,0,owner,1,0x73,0,result,0,0,1,0xc400,
        active,0,0,0,0,0,0,0,0,(u16)tab);
    row=1;record=D(b+0x1c);
    count=(u32)(s32)(s8)cm_fn_00544c70(record,b+0x174,0x15);
    if(count==0){
        cm_fn_0066f4e0(0x00de1f64,0x0098fee8,0x00ad9c64);DRAW_LABEL(2);
    }else{
        BYTE(0x00adab33)=1;
        if(cm_fn_0053b900(record)!=0)pointer=0x00487660;
        else pointer=0x004875a0;
        /* Negative signed BYTE counts still reach original qsort. */
        cm_fn_00945333(b+0x174,count,4,pointer);
        last=0;
        if((s32)count>0){
            D(b+0x24)=b+0x174;
            do{
                pointer=D(b+0x24);candidate=D(pointer);
                if(candidate!=last){
                    last=candidate;++row;
                    cm_fn_0053dba0(last,b+0x1c8,0xc8,1);
                    cm_fn_00944c9f(0x00de1f64,0x00987ff0,b+0x1c8);
                    kind=BYTE(last+0x3d);plain=0;
                    if(kind==1){
                        first=D(last);second=D(0x00ae2d0c);
                        if((BYTE(second+first*79u+0xbu)&0x40u)==0)plain=1;
                    }
                    record=D(b+0x1c);classification=cm_fn_0053b900(record);
                    if(plain){
                        DRAW_LABEL(row);
                        code=classification?BYTE(last+0x28):BYTE(last+0x3d);
                        pointer=cm_fn_0053ad00((u8)code,0,1,0);
                        if(classification){
                            text=W(0x00aebdec);shade=W(0x00aebdea);
                        }else{
                            shade=W(0x00aebdea);text=W(0x00aebdec);
                        }
                    }else{
                        active=W(0x00ae3184);shade=W(0x00aebdea);
                        draw=cm_fn_008040a0(0x00b74450,tab,0,row,1,shade,1,3,
                            active,0x00de1f64,classification?0x7dc:0x7d7,last);
                        code=classification?BYTE(last+0x28):BYTE(last+0x3d);
                        pointer=cm_fn_0053ad00((u8)code,0,1,0);
                        if(classification){
                            shade=W(0x00aebdea);text=W(0x00aebdec);
                        }else{
                            text=W(0x00aebdec);shade=W(0x00aebdea);
                        }
                    }
                    draw=cm_fn_00804100(0x00b74450,tab,1,row,1,shade,1,2,text,pointer);
                }
                pointer=D(b+0x24);D(b+0x24)=pointer+4u;--count;
            }while(count!=0);
        }
    }
    result=cm_fn_007ff010(0x00b74450,0x1c);
    if((u16)tab!=0xffffu){
        page=(u32)(s32)(s16)W(0x00b77c7a);
        offset=(u32)(s32)(s16)(u16)tab*3057u;
        owner=D(0x00b74c4cu+page*768u);
        cm_fn_00403320(owner+offset+4u,result);
    }
    draw=cm_fn_007ff270(0x00b74450,0x1d,(u32)(s32)(s16)(u16)tab,0);
    goto L467166;
L4619d3:
    selected=D(b+0x38);
    if(selected==0)goto L462a80;
    draw=cm_fn_005e5cd0(2);bottom=arg2;
    divisor=(s32)draw;numerator=bottom-top;
    BYTE(b+0x24)=(u8)((s32)numerator/divisor);
    page=(u32)(s32)(s16)W(0x00b77c7a);first=D(b+0x24);
    owner=D(0x00b74c4cu+page*768u);
    tab=cm_fn_0055fba0(owner,0x6e,top,0x30c,bottom,2,0,first,0,2,0,0xffff);
    result=D(b+0x30);cm_fn_00487840(record,result,b+0x64);
    cm_fn_0066f4e0(0x00de1f64,0x0098fed4);STAT_LABEL(0);
    first=(u32)(s32)(s8)BYTE(b+0x64);
    cm_fn_00944c9f(0x00de1f64,0x0098806c,first);DRAW_VALUE(0,0x00de1f64);
    cm_fn_0066f4e0(0x00de1f64,0x0098feb8);STAT_LABEL(1);
    first=(u32)(s32)(s8)BYTE(b+0x65);
    cm_fn_00944c9f(0x00de1f64,0x0098806c,first);DRAW_VALUE(1,0x00de1f64);
    cm_fn_0066f4e0(0x00de1f64,0x0098fe9c);STAT_LABEL(2);
    /* Native FCOMP C3 selects blank for equal or unordered values. Keep the
       first float read before the constant, then reload for promotion only
       on the ordered unequal path. All calls have root-relative FP depth0. */
    comparison=F32(b+0x66);zero=F32(0x00968970);
    if(comparison<zero || comparison>zero){
        promoted=(double)F32(b+0x66); /* Native SECOND float read. */
        cm_fn_00944c9f(0x00de1f64,0x0098fe94,promoted);
    }else cm_fn_00944c9f(0x00de1f64,0x0098ba0c);
    DRAW_VALUE(2,0x00de1f64);
    cm_fn_0066f4e0(0x00de1f64,0x0098fe7c);STAT_LABEL(3);
    comparison=F32(b+0x6a);zero=F32(0x00968970);
    if(comparison<zero || comparison>zero){
        promoted=(double)F32(b+0x6a);
        cm_fn_00944c9f(0x00de1f64,0x0098fe94,promoted);
    }else cm_fn_00944c9f(0x00de1f64,0x0098ba0c);
    DRAW_VALUE(3,0x00de1f64);
    record=D(b+0x1c);row=4;
    if(cm_fn_0053b900(record)!=0)goto L4622ae;
    WAGE_METRIC(0x72,0x0098fe54,0x0098fe2c,0x0098fe04,4);
    WAGE_METRIC(0x6e,0x0098fde4,0x0098fdc4,0x0098fda4,5);
    WAGE_METRIC(0x76,0x0098fd8c,0x0098fd74,0x0098fd5c,6);
    WAGE_RECORD(0x8e,0x0098fd44,0x0098fd2c,0x0098fd14,7,0x86);
    WAGE_RECORD(0x92,0x0098fcfc,0x0098fce8,0x0098fcd4,8,0x8a);
    row=9;
L4622ae:
    cm_fn_0066f4e0(0x00de1f64,0x0098fcc4);STAT_LABEL(row);
    RECORD_NUMBER(0x7e,0x18,1,row);
    ++row;cm_fn_0066f4e0(0x00de1f64,0x0098fca8);STAT_LABEL(row);
    RECORD_NUMBER(0x82,0x18,1,row);
    ++row;
    if(cm_fn_0053b900(record)!=0){
        cm_fn_0066f4e0(0x00de1f64,0x0098fc90);STAT_LABEL(row);
        first=(u32)(s32)(s16)W(b+0x96);
        cm_fn_00944c9f(0x00de1f64,0x0098806c,first);DRAW_VALUE(row,0x00de1f64);
        ++row;cm_fn_0066f4e0(0x00de1f64,0x0098fc78);STAT_LABEL(row);
        RECORD_NUMBER(0x98,0x22,0,row);
        ++row;cm_fn_0066f4e0(0x00de1f64,0x0098fc60);STAT_LABEL(row);
        RECORD_NUMBER(0x9c,0x22,0,row);
        ++row;
    }
    draw=cm_fn_0053b900(record);
    if(draw!=0)goto L467166; /* Full helper EAX=1 survives common exit. */
    cm_fn_0066f4e0(0x00de1f64,0x0098fc48);STAT_LABEL(row);
    pointer=D(b+0xa0);
    if(pointer!=0){
        first=D(pointer+0x52);D(b+0x2c)=first;
        pair=cm_fn_0043f460(b+0x2c,6);
        cm_fn_00441c10(0x00de1f64,0x7d0,(u32)pair,(u32)(pair>>32),6);
        pointer=D(b+0xa0);first=D(pointer+0x52);
        SCAN_LENGTH(0x00de1f64,length);
        cm_fn_00944c9f(0x00de1f64u+length,0x0098fd10,first);
        SCAN_LENGTH(0x00de1f64,length);pointer=D(b+0xa0);
        cm_fn_0053dba0(pointer,0x00de1f64u+length,0x7d0,1);
        pointer=D(b+0xa0);text=W(0x00aebdec);shade=W(0x00aebdea);
        draw=cm_fn_008040a0(0x00b74450,tab,1,row,1,shade,1,2,text,
            0x00de1f64,0x7d7,pointer);
    }else{DRAW_VALUE(row,0x0098ba0c);}
    flags=D(0x009b5984);++row;
    if(flags==0){COPY_SCANNED(0x00de1f64,0x0098fc20);record=D(b+0x1c);}
    else cm_fn_0066f4e0(0x00de1f64,0x0098fbf4);
    STAT_LABEL(row);COUNT_STAT(0xa4,row);
    flags=D(0x009b5984);++row;
    if(flags==0){COPY_SCANNED(0x00de1f64,0x0098fbd0);record=D(b+0x1c);}
    else cm_fn_0066f4e0(0x00de1f64,0x0098fba8);
    STAT_LABEL(row);COUNT_STAT(0xa5,row);
    ++row;cm_fn_0066f4e0(0x00de1f64,0x0098fb8c);STAT_LABEL(row);
    COUNT_STAT(0xa6,row);
    ++row;draw=cm_fn_0053d7d0(record);
    if(draw==0)goto L467166; /* Full helper EAX=0 survives common exit. */
    cm_fn_0066f4e0(0x00de1f64,0x0098fb70);STAT_LABEL(row);
    COUNT_STAT(0xa7,row);goto L467166;
L462a80:
    D(b+0x30)=0;BYTE((u32)&frame.state)=2;
    result=cm_fn_0043f440();BYTE((u32)&frame.state)=1;
    finance_mode=result==0?0x40u:1u;
    draw=cm_fn_005e5cd0(3);bottom=arg2;
    divisor=(s32)draw;numerator=bottom-top-0x10u;
    BYTE(b+0x24)=(u8)((s32)numerator/divisor);
    first=D(b+0x24);page=(u32)(s32)(s16)W(0x00b77c7a);
    owner=D(0x00b74c4cu+page*768u);
    cm_fn_0055fba0(owner,0x6e,top,0x30c,bottom,5,b+0x5c,first,0,2,0,0xffff);
    first=D(b+0x24);page=(u32)(s32)(s16)W(0x00b77c7a);
    owner=D(0x00b74c4cu+page*768u);
    tab=cm_fn_0055fba0(owner,0x6e,top,0x302,bottom,5,b+0x5c,first,0,1,0,0xffff);
    cm_fn_0066f4e0(0x00de1f64,0x0098fb68);FIN_HEADER(0,1,3);
    cm_fn_0066f4e0(0x00de1f64,0x0098fb5c);
    finance_mode=(u32)(s32)(s16)(u16)finance_mode;FIN_HEADER(1,finance_mode,2);
    cm_fn_0066f4e0(0x00de1f64,0x0098fb50);FIN_HEADER(2,finance_mode,2);
    cm_fn_0066f4e0(0x00de1f64,0x0098fb44);FIN_HEADER(3,finance_mode,2);
    cm_fn_0066f4e0(0x00de1f64,0x0098fb38);FIN_HEADER(4,finance_mode,2);
    active=W(0x00aebdd4);page=(u32)(s32)(s16)W(0x00b77c7a);
    owner=D(0x00b74c4cu+page*768u);
    draw=cm_fn_0055f990(page,0,owner,1,0x7d,0,0x2fd,0,0,1,0xc400,
        active,0,0,0,0,0,0,0,0,(u16)tab);
    kind=BYTE(b+0x14);
    if((kind&2u)==0)goto L463bd1;
    cm_fn_0066f4e0(0x00de1f64,0x0098fb2c);DRAW_LABEL(2);
    /* VA00462CEE, four income columns. */
    SUM9(0xbc,0xb8,0xb4,0xb0,0xac,0xa8,0xa4,0xa0,0x78);
    FIN_FORMAT();INCOME_CAPTURE(0x30,0x34,1);
    SUM9(0x6c,0x68,0x64,0x60,0x5c,0x58,0x54,0x50,0x28);
    FIN_FORMAT();INCOME_CAPTURE(0x38,0x3c,2);
    SUM9(0x15c,0x158,0x154,0x150,0x14c,0x148,0x144,0x140,0x118);
    FIN_FORMAT();INCOME_CAPTURE(0x24,0x28,3);
    SUM9(0x10c,0x108,0x104,0x100,0xfc,0xf8,0xf4,0xf0,0xc8);
    FIN_FORMAT();INCOME_CAPTURE(0x48,0x4c,4);
    cm_fn_0066f4e0(0x00de1f64,0x0098fb1c);DRAW_LABEL(3);
    SUM9(0x9c,0x98,0x94,0x90,0x8c,0x88,0x84,0x80,0x70);
    FIN_FORMAT();EXPENSE_CAPTURE(0x14,0x18,1);
    SUM9(0x4c,0x48,0x44,0x40,0x3c,0x38,0x34,0x30,0x20);
    FIN_FORMAT();EXPENSE_CAPTURE(0x1c,0x20,2);
    SUM9(0x13c,0x138,0x134,0x130,0x12c,0x128,0x124,0x120,0x110);
    FIN_FORMAT();EXPENSE_CAPTURE(0x40,0x44,3);
    SUM9(0xec,0xe8,0xe4,0xe0,0xdc,0xd8,0xd4,0xd0,0xc0);
    FIN_FORMAT();EXPENSE_CAPTURE(0x50,0x54,4);
    cm_fn_0066f4e0(0x00de1f64,0x0098fb0c);DRAW_LABEL(5);
    FIN_DIFFERENCE(0x30,0x14,0x34,0x18,1);
    FIN_DIFFERENCE(0x38,0x1c,0x3c,0x20,2);
    FIN_DIFFERENCE(0x24,0x40,0x28,0x44,3);
    FIN_DIFFERENCE(0x48,0x50,0x4c,0x54,4);
    cm_fn_0066f4e0(0x00de1f64,0x0098fadc,0x00ad9c64);DRAW_LABEL(7);
    high=D(finance+4u);low=D(finance);
    /* Native tests the high DWORD, but formats only the signed low DWORD.
       Negative branch NEGates only low; preserve this original truncation. */
    if((s32)high<0){
        D(b+0x10)=0u-low;FIN_NEGATIVE_FORMAT();
        text=W(0x00ae31a8);shade=W(0x00aebdea);
    }else{
        D(b+0x10)=low;FIN_FORMAT();
        text=W(0x00adab48);shade=W(0x00aebdea);
    }
    draw=cm_fn_00804100(0x00b74450,tab,1,7,1,shade,finance_mode,2,text,0x00de1f64);
    cm_fn_0066f4e0(0x00de1f64,0x0098fac8);DRAW_LABEL(8);
    pair=cm_fn_00598800(finance,0,0,0,1,0);
    if((s32)(u32)(pair>>32)<0){
        cm_fn_0066f4e0(0x00de1f64,0x0098fab8);
        text=W(0x00ae31a8);shade=W(0x00aebdea);
    }else{
        /* Native asks again; its second result need not equal the first. */
        pair=cm_fn_00598800(finance,0,0,0,1,0);D(b+0x10)=(u32)pair;
        FIN_FORMAT_MODE(6);text=W(0x00adab48);shade=W(0x00aebdea);
    }
    draw=cm_fn_00804100(0x00b74450,tab,1,8,1,shade,finance_mode,2,text,0x00de1f64);
    goto L467166;
L463bd1:
    if((kind&4u)==0)goto L4657a4;
    cm_fn_0066f4e0(0x00de1f64,0x0098faa8);DRAW_LABEL(2);
    FIELD_BASE(0xa0,0x30,0x34,1,2);
    FIELD_BASE(0x50,0x38,0x3c,2,2);
    FIELD_BASE(0x140,0x24,0x28,3,2);
    FIELD_BASE(0xf0,0x48,0x4c,4,2);
    cm_fn_0066f4e0(0x00de1f64,0x0098fa94);DRAW_LABEL(3);
    FIELD_ADD_A(0xac,0x30,0x34,1,3);
    FIELD_ADD_A(0x5c,0x38,0x3c,2,3);
    FIELD_ADD_A(0x14c,0x24,0x28,3,3);
    FIELD_ADD_A(0xfc,0x48,0x4c,4,3);
    cm_fn_0066f4e0(0x00de1f64,0x0098fa84);DRAW_LABEL(4);
    FIELD_ADD_B(0xb8,0x30,0x34,1,4);
    FIELD_ADD_B(0x68,0x38,0x3c,2,4);
    FIELD_ADD_B(0x158,0x24,0x28,3,4);
    FIELD_ADD_B(0x108,0x48,0x4c,4,4);
    cm_fn_0066f4e0(0x00de1f64,0x0098fa74);DRAW_LABEL(5);
    FIELD_ADD_C(0xbc,0x30,0x34,1,5);
    FIELD_ADD_C(0x6c,0x38,0x3c,2,5);
    FIELD_ADD_C(0x15c,0x24,0x28,3,5);
    FIELD_ADD_D(0x10c,0x48,0x4c,4,5);
    cm_fn_0066f4e0(0x00de1f64,0x0098fa64);DRAW_LABEL(6);
    FIELD_ADD_A(0x78,0x30,0x34,1,6);
    FIELD_ADD_A(0x28,0x38,0x3c,2,6);
    FIELD_ADD_A(0x118,0x24,0x28,3,6);
    FIELD_ADD_A(0xc8,0x48,0x4c,4,6);
    cm_fn_0066f4e0(0x00de1f64,0x0098fa54);DRAW_LABEL(7);
    FIELD_ADD_B(0xa8,0x30,0x34,1,7);
    FIELD_ADD_B(0x58,0x38,0x3c,2,7);
    FIELD_ADD_B(0x148,0x24,0x28,3,7);
    FIELD_ADD_B(0xf8,0x48,0x4c,4,7);
    cm_fn_0066f4e0(0x00de1f64,0x0098fa44);DRAW_LABEL(8);
    FIELD_ADD_C(0xb4,0x30,0x34,1,8);
    FIELD_ADD_C(0x64,0x38,0x3c,2,8);
    FIELD_ADD_C(0x154,0x24,0x28,3,8);
    FIELD_ADD_D(0x104,0x48,0x4c,4,8);
    cm_fn_0066f4e0(0x00de1f64,0x0098fa24,0x00ad9c64);DRAW_LABEL(9);
    FIELD_ADD_A(0xb0,0x30,0x34,1,9);
    FIELD_ADD_A(0x60,0x38,0x3c,2,9);
    FIELD_ADD_A(0x150,0x24,0x28,3,9);
    FIELD_ADD_A(0x100,0x48,0x4c,4,9);
    cm_fn_0066f4e0(0x00de1f64,0x0098fa04,0x00ad9c64);DRAW_LABEL(10);
    FIELD_ADD_B(0xa4,0x30,0x34,1,10);
    FIELD_ADD_B(0x54,0x38,0x3c,2,10);
    FIELD_ADD_E(0x144,0x24,0x28,3,10);
    FIELD_LAST(0xf4,0x48,0x4c,4,10);
    cm_fn_0066f4e0(0x00de1f64,0x0098f9e4,0x00ad9c64);
    active=W(0x00aebde8);shade=W(0x00aebdea);
    draw=cm_fn_00804100(0x00b74450,tab,0,12,1,shade,1,3,active,0x00de1f64);
    high=D(b+0x34);low=D(b+0x30);FIN_TOTAL(1);
    high=D(b+0x3c);low=D(b+0x38);FIN_TOTAL(2);
    high=D(b+0x28);low=D(b+0x24);FIN_TOTAL(3);
    high=D(b+0x4c);low=last_total;FIN_TOTAL(4);goto L467166;
L4657a4:
    cm_fn_0066f4e0(0x00de1f64,0x0098f9dc);DRAW_LABEL(2);
    first=D(finance+0x90);D(b+0x10)=first;FIN_FORMAT();
    INCOME_CAPTURE(0x14,0x18,1);
    first=D(finance+0x40);D(b+0x10)=first;FIN_FORMAT();
    INCOME_CAPTURE(0x1c,0x20,2);
    first=D(finance+0x130);D(b+0x10)=first;FIN_FORMAT();
    INCOME_CAPTURE(0x40,0x44,3);
    first=D(finance+0xe0);D(b+0x10)=first;
    pair=cm_fn_0043f460(b+0x10,0x22);
    cm_fn_00441c10(0x00de1f64,0x7d0,(u32)pair,(u32)(pair>>32),0x22);
    SCAN_LENGTH(0x00de1f64,length);
    if(length>0xdu)cm_fn_00450aa0(b+0x10,0x00de1f64,0x7d0,0x21);
    INCOME_CAPTURE(0x50,0x54,4);
    cm_fn_0066f4e0(0x00de1f64,0x0098f9d0);DRAW_LABEL(3);
    GET_ADD_B(cm_fn_004671c0,0x28,0x14,0x18,1,3);
    GET_ADD_B(cm_fn_004671b0,0x28,0x1c,0x20,2,3);
    GET_ADD_B(cm_fn_004671e0,0x28,0x40,0x44,3,3);
    GET_ADD_B(cm_fn_004671d0,0x28,0x50,0x54,4,3);
    cm_fn_0066f4e0(0x00de1f64,0x0098f9bc);DRAW_LABEL(4);
    GET_ADD_C(cm_fn_004671c0,0x2c,0x14,0x18,1,4);
    GET_ADD_C(cm_fn_004671b0,0x2c,0x1c,0x20,2,4);
    GET_ADD_C(cm_fn_004671e0,0x2c,0x40,0x44,3,4);
    GET_ADD_D(cm_fn_004671d0,0x2c,0x50,0x54,4,4);
    cm_fn_0066f4e0(0x00de1f64,0x0098f9b0);DRAW_LABEL(5);
    GET_ADD_A(cm_fn_004671c0,0x24,0x14,0x18,1,5);
    GET_ADD_A(cm_fn_004671b0,0x24,0x1c,0x20,2,5);
    GET_ADD_A(cm_fn_004671e0,0x24,0x40,0x44,3,5);
    GET_ADD_A(cm_fn_004671d0,0x24,0x50,0x54,4,5);
    cm_fn_0066f4e0(0x00de1f64,0x0098f99c);DRAW_LABEL(6);
    GET_ADD_B(cm_fn_004671c0,0,0x14,0x18,1,6);
    GET_ADD_B(cm_fn_004671b0,0,0x1c,0x20,2,6);
    GET_ADD_B(cm_fn_004671e0,0,0x40,0x44,3,6);
    GET_ADD_B(cm_fn_004671d0,0,0x50,0x54,4,6);
    cm_fn_0066f4e0(0x00de1f64,0x0098f984);DRAW_LABEL(7);
    GET_ADD_C(cm_fn_004671c0,0x1c,0x14,0x18,1,7);
    GET_ADD_C(cm_fn_004671b0,0x1c,0x1c,0x20,2,7);
    GET_ADD_C(cm_fn_004671e0,0x1c,0x40,0x44,3,7);
    GET_ADD_D(cm_fn_004671d0,0x1c,0x50,0x54,4,7);
    cm_fn_0066f4e0(0x00de1f64,0x0098f974);DRAW_LABEL(8);
    GET_ADD_A(cm_fn_004671c0,0x18,0x14,0x18,1,8);
    GET_ADD_A(cm_fn_004671b0,0x18,0x1c,0x20,2,8);
    GET_ADD_A(cm_fn_004671e0,0x18,0x40,0x44,3,8);
    GET_ADD_A(cm_fn_004671d0,0x18,0x50,0x54,4,8);
    cm_fn_0066f4e0(0x00de1f64,0x0098fa24,0x00ad9c64);DRAW_LABEL(9);
    GET_ADD_B(cm_fn_004671c0,0x14,0x14,0x18,1,9);
    GET_ADD_B(cm_fn_004671b0,0x14,0x1c,0x20,2,9);
    GET_ADD_B(cm_fn_004671e0,0x14,0x40,0x44,3,9);
    GET_ADD_E(cm_fn_004671d0,0x14,0x50,0x54,4,9);
    cm_fn_0066f4e0(0x00de1f64,0x0098f950,0x00ad9c64);DRAW_LABEL(10);
    GET_ADD_C(cm_fn_004671c0,0x10,0x14,0x18,1,10);
    GET_ADD_C(cm_fn_004671b0,0x10,0x1c,0x20,2,10);
    GET_ADD_C(cm_fn_004671e0,0x10,0x40,0x44,3,10);
    GET_LAST(cm_fn_004671d0,0x10,0x50,0x54,4,10);
    cm_fn_0066f4e0(0x00de1f64,0x0098f92c,0x00ad9c64);
    active=W(0x00aebde8);shade=W(0x00aebdea);
    draw=cm_fn_00804100(0x00b74450,tab,0,12,1,shade,1,3,active,0x00de1f64);
    high=D(b+0x18);low=D(b+0x14);WRAPPED_TOTAL(1);
    high=D(b+0x20);low=D(b+0x1c);WRAPPED_TOTAL(2);
    high=D(b+0x44);low=D(b+0x40);WRAPPED_TOTAL(3);
    high=D(b+0x54);low=last_total;WRAPPED_TOTAL(4);
L467166:
    /* Both original cleanup helpers are bare RET and preserve actual EAX.
       Carry the result from 7FF270, 53B900, 53D7D0 or final804100 explicitly. */
    cm_fn_005a53c0(b+0x2c);cm_fn_005a53c0(b+0x10);
    previous=frame.prev;CM_FS(volatile u32,0)=previous;
    return draw;
}

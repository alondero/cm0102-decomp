/* CM0102 3.9.68, preferred-image VA008cc030, base00400000.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS6/RET24; explicit EAX retained also on cached-date early exits.
   Original numeric callees remain boundaries, including file/heap scaffolding. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32,(u32)(a))
#define W(a) CM_GLOBAL(volatile u16,(u32)(a))
#define B(a) CM_GLOBAL(volatile u8,(u32)(a))
extern u32 CM_THISCALL cm_fn_0054a110(u32);
extern u32 CM_THISCALL cm_fn_00549f70(u32,u32,u32,u32,u32);
extern u8 CM_THISCALL cm_fn_0054cfb0(u32);
extern u32 CM_THISCALL cm_fn_00417c10(u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00945667(u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_0066f4e0(u32,u32,...);
extern void CM_CDECL cm_fn_005e8290(u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090bde0(u32,u32);
extern u32 CM_CDECL cm_fn_00945b00(u32,u32);
extern u32 CM_CDECL cm_fn_00945bfe(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00945a09(u32);

static void allocation_error(u32 first,u32 second,u32 line)
{
    u32 version;
    cm_fn_00944cff(0x00a8d514u,0,0,first,second);
    version=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u,0x009a8120u,version,first,second,line,0x00a8ecb8u);
    cm_fn_0066f4e0(0x00ae28bcu,0x009a8108u);
    cm_fn_0066f4e0(0x00ae26c4u,0x009a7ff8u,0x00ae24d0u);
    cm_fn_005e8290(0x00ae28bcu,0x00ae26c4u,0);
}

u32 CM_THISCALL cm_fn_008cc030(u32 self,u32 start_month,u32 start_year,
    u32 end_month,u32 end_year,u32 output_slot,u32 count_slot)
{
    u8 scratch[0x240];
    u32 S=(u32)scratch-16u;
    volatile struct {void *prev;u32 handler;u32 state;} frame;
    u32 written=0,stream=0,first,second,date,record,dest,value;
    u32 ordinal,source_offset,destination_offset,year,remaining,result;
    u16 stored_year;
    frame.prev=CM_FS(void *,0);
    frame.handler=0x00963e7bu;
    frame.state=0xffffffffu;
    CM_FS(void *,0)=(void *)&frame;
    cm_fn_0054a110(S+0x45u);
    frame.state=0;
    if(D(output_slot)==0) {
        D(count_slot)=0;
        value=D(output_slot);
        if(value==0)goto scan;
    }
    if((s32)D(count_slot)>0) {
        date=cm_fn_00549f70(S+0x20u,1,start_month,start_year,0xffffffffu);
        first=D(date);second=D(date+4u);
        value=D(output_slot);
        D(S+0x18u)=first;
        record=value+0x1du;
        D(S+0x1cu)=second;
        stored_year=W(record+2u);
        if((u16)(first>>16)>stored_year ||
            ((u16)(first>>16)==stored_year && (s16)first>(s16)W(record))) {
            date=cm_fn_00549f70(S+0x20u,28,end_month,end_year,0xffffffffu);
            first=D(date);second=D(date+4u);
            D(S+0x1cu)=second;D(S+0x18u)=first;
            value=D(count_slot);
            record=D(output_slot)+value*37u-8u;
            stored_year=W(record+2u);
            if((u16)(first>>16)<stored_year) {
                result=record;
                goto done;
            }
            if((u16)(first>>16)==stored_year && (s16)first<(s16)W(record)) {
                result=1;
                goto done;
            }
        }
    }
scan:
    value=D(self+0x86au);
    ordinal=0;
    if((s32)value>0) {
        destination_offset=0;source_offset=0;
        do {
            value=D(self+0x862u);
            record=value+source_offset;
            stored_year=W(record+0x1fu);
            if(stored_year>(u16)end_year)break;
            if(stored_year==(u16)end_year &&
                (s8)cm_fn_0054cfb0(record+0x1du)>(s8)end_month)break;
            value=D(self+0x862u);
            record=value+source_offset;
            stored_year=W(record+0x1fu);
            if(stored_year>(u16)start_year ||
                (stored_year==(u16)start_year &&
                 (s8)cm_fn_0054cfb0(record+0x1du)>=(s8)start_month)) {
                value=D(count_slot);
                if(written==value) {
                    value+=100u;
                    D(count_slot)=value;
                    first=D(output_slot);
                    value=first?cm_fn_00945667(first,value*37u):cm_fn_00944e46(value*37u);
                    D(output_slot)=value;
                    if(value==0)allocation_error(S+0x150u,S+0x50u,0x291fu);
                }
                value=D(self+0x862u);
                first=D(output_slot);
                record=value+source_offset+0x1du;
                dest=first+destination_offset;
                ++written;
                D(dest)=D(record-0x1du);
                D(dest+4u)=D(record-0x19u);
                D(dest+8u)=D(record-0x15u);
                D(dest+12u)=D(record-0x11u);
                D(dest+16u)=D(record-0xdu);
                D(dest+20u)=D(record-9u);
                D(dest+24u)=D(record-5u);
                second=B(record-1u);
                B(dest+28u)=(u8)second;
                destination_offset+=37u;
                cm_fn_00417c10(dest+29u,S+0x20u,record);
            }
            value=D(self+0x86au);
            ++ordinal;source_offset+=37u;
        }while((s32)ordinal<(s32)value);
    }
    if((u16)start_year>(u16)end_year) {
        D(count_slot)=written;
        result=start_year;
        goto done;
    }
    year=(u16)start_year;
    remaining=(u32)(u16)end_year-year+1u;
    do {
        if(stream)cm_fn_00945a09(stream);
        cm_fn_00944c9f(0x00de1f64u,0x00a8ea38u,year);
        value=cm_fn_0090bde0(0x00987da0u,0x00de1f64u);
        stream=cm_fn_00945b00(value,0x0098f928u);
        if(stream) {
            value=cm_fn_00945bfe(S+0x28u,37,1,stream);
            if(value==1) {
                destination_offset=written*37u;
                do {
                    stored_year=W(S+0x47u);
                    if(stored_year>(u16)end_year)break;
                    if(stored_year==(u16)end_year) {
                        if((s8)cm_fn_0054cfb0(S+0x45u)>(s8)end_month)break;
                        stored_year=W(S+0x47u);
                    }
                    if(stored_year>(u16)start_year ||
                       (stored_year==(u16)start_year &&
                        (s8)cm_fn_0054cfb0(S+0x45u)>=(s8)start_month)) {
                        value=D(count_slot);
                        if(written==value) {
                            value+=100u;
                            D(count_slot)=value;
                            first=D(output_slot);
                            value=first?cm_fn_00945667(first,value*37u):cm_fn_00944e46(value*37u);
                            D(output_slot)=value;
                            if(value==0)allocation_error(S+0x50u,S+0x150u,0x295fu);
                        }
                        dest=D(output_slot)+destination_offset;
                        ++written;destination_offset+=37u;
                        D(dest)=D(S+0x28u);D(dest+4u)=D(S+0x2cu);
                        D(dest+8u)=D(S+0x30u);D(dest+12u)=D(S+0x34u);
                        D(dest+16u)=D(S+0x38u);D(dest+20u)=D(S+0x3cu);
                        D(dest+24u)=D(S+0x40u);B(dest+28u)=B(S+0x44u);
                        cm_fn_00417c10(dest+29u,S+0x20u,S+0x45u);
                    }
                    value=cm_fn_00945bfe(S+0x28u,37,1,stream);
                }while(value==1);
            }
            cm_fn_00945a09(stream);
            stream=0;
        }
        ++year;--remaining;
    }while(remaining!=0);
    D(count_slot)=written;
    result=0;
done:
    CM_FS(void *,0)=frame.prev;
    return result;
}

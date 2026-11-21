#include "cmre.h"
    /* Preferred-image VA008c3eb0/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS0/fullEAX; SEH963c4b/scratch420: acceptedL0/cachedyearDWORDL4,
   indexL8/recordLc/yearcursorL10/selfL14/dateL18/diagnosticsL20..41f.
   Keep same-year/null-file success counting, file/diagnostic order, wrapping
   allocation/copy sizes and original forward DWORD/BYTE compaction.
   Return initial nonpositive count, zero accepted count, actual allocation
   error EAX, or remaining count reloaded after free, as the listing does.
    Case38 explain: original faults at 008c416b with EDI=1, ESI=0xddd518,
    ECX=0xeb008d9, EDX=0x3ac02366; the default recovered build matches
    fault (20,1,4). This alone does not establish the O0 budget.
    Stack homes below use constant union indices, not runtime address
    arithmetic. This retains the original scratch adjacency, volatile widths
    and reloads while avoiding needless address computations at O0.
    ABI evidence: 008c3ecf reads ECX; 008c41bd RET pops zero bytes.
    008c3ed7..008c3eed pass receiver AE2C90 and stack (date,34);
    008c3f83..008c3fa2 leave 9C79B0 pending for fopen, not 90bde0;
    008c401d..008c4034 leave diagnostic arguments pending across 90d130.
    All instruction VAs refer to the pinned image identified above. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern u32 CM_THISCALL cm_fn_0054cab0(u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_0090bde0(u32,u32);
extern u32 CM_CDECL cm_fn_00945b00(u32,u32);
extern u32 CM_CDECL cm_fn_00945a09(u32);
extern u32 CM_CDECL cm_fn_00945ce6(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32,u32,...);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);

u32 CM_THISCALL cm_fn_008c3eb0(u32 self)
{
    volatile struct {u32 prev,handler,state;} frame;
    volatile union {
        u8 bytes[0x420];
        u16 halves[0x210];
        u32 words[0x108];
    } scratch;
    u32 l=(u32)&scratch,eax,record,cursor,stream=0,total;
    u32 first,second,line,version,deleted,capacity,allocation,bytes;
    u32 source,destination,remaining,object;
    u16 year,currentyear,day;
    frame.state=0xffffffffu;frame.handler=0x00963c4b;
    frame.prev=CM_FS(volatile u32,0);
    CM_FS(volatile u32,0)=(u32)&frame;
    scratch.words[5]=self;scratch.words[0]=0;scratch.words[1]=0;
    cm_fn_0054cab0(0x00ae2c90,l+0x18,0x34);
    eax=D(self+0x86a);record=D(self+0x862);
    frame.state=0;
    scratch.words[2]=0;scratch.words[3]=record;
    if((s32)eax<=0) goto done;
    cursor=record+0x1f;scratch.words[4]=cursor;
record_loop:
    year=W(cursor);eax=(eax&0xffff0000u)|year;
    currentyear=scratch.halves[0x1a/2];
    if(currentyear<year) goto archive_done;
    if(currentyear==year){
        day=scratch.halves[0x18/2];
        if((s16)day<=(s16)W(cursor-2)) goto archive_done;
        record=scratch.words[3];
    }
    if(year!=scratch.halves[4/2]){
        scratch.words[1]=eax;
        if(stream!=0) cm_fn_00945a09(stream);
        eax=scratch.words[1]&0xffffu;
        cm_fn_00944c9f(0x00de1f64,0x00a8ea38,eax);
        eax=cm_fn_0090bde0(0x00987da0,0x00de1f64);
        stream=cm_fn_00945b00(eax,0x009c79b0);
        if(stream==0){
            first=l+0x220;second=l+0x320;line=0x1332;
            goto archive_error;
        }
    }else if(stream==0) goto accepted;
    eax=cm_fn_00945ce6(record,0x25,1,stream);
    if(eax!=1){
        first=l+0x20;second=l+0x120;line=0x133c;
        goto archive_error;
    }
accepted:
    scratch.words[0]=scratch.words[0]+1u;
    goto next_record;
archive_error:
    cm_fn_00944cff(0x00a8d514,0,0,first,second);
    version=cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0,0x009870e8,version,first,second,line);
    cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
    D(0x00b67a34)=0;
next_record:
    eax=scratch.words[2];cursor=scratch.words[4];total=D(self+0x86a);
    ++eax;record+=0x25;cursor+=0x25;
    scratch.words[2]=eax;scratch.words[3]=record;scratch.words[4]=cursor;
    if((s32)eax<(s32)total) goto record_loop;
archive_done:
    if(stream!=0) cm_fn_00945a09(stream);
    eax=scratch.words[0];
    if(eax==0) goto done;
    capacity=D(self+0x86a)-eax+100u;
    allocation=(u32)cm_fn_00944e46(capacity*37u);
    if(allocation==0){
        cm_fn_00944cff(0x00a8d514,0,0,l+0x120,l+0x20);
        version=cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0,0x009a8120,version,
                       l+0x120,l+0x20,0x135d,0x00a8e9dc);
        cm_fn_0066f4e0(0x00ae28bc,0x009a8108);
        cm_fn_0066f4e0(0x00ae26c4,0x009a7ff8,0x00ae24d0);
        eax=cm_fn_005e8290(0x00ae28bc,0x00ae26c4,0);
        goto done;
    }
    eax=D(self+0x86a);deleted=scratch.words[0];eax-=deleted;
    bytes=eax*37u;
    source=deleted*37u;
    source+=D(self+0x862);destination=allocation;
    remaining=bytes>>2;
    while(remaining!=0){
        D(destination)=D(source);source+=4;destination+=4;--remaining;
    }
    remaining=bytes&3u;
    while(remaining!=0){
        B(destination)=B(source);++source;++destination;--remaining;
    }
    object=scratch.words[5];
    eax=D(object+0x862);
    cm_fn_009452ca(eax);
    deleted=scratch.words[0];eax=D(object+0x86a);eax-=deleted;
    D(object+0x862)=allocation;
    D(object+0x86a)=eax;
    D(object+0x866)=capacity;
done:
    CM_FS(volatile u32,0)=frame.prev;
    return eax;
}

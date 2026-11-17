/* fn_004d9f80 -- Championship Manager 01/02 3.9.68, preferred-image VA 0x004d9f80
   (module e:\dev\cm3\cm3 00-01\cm3\code\contract_manager.cpp).
   Image base 0x00400000; pinned binary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.

   Frame (original stack, entry esp = E):
     E+0   return address, E+4 the single stack argument (ret 4 pops it)
     E-0xc SEH {prev, handler, state} linked into fs:[0]
     E-0xec scratch area, 0x2e0 bytes:
       E-0x2ec 0x2ec-0xe0  command object, receiver of cm_fn_0077bbe0/00785790
       E-0x20c 0x20c-0x100 buffer 1 (arg5 of cm_fn_00944cff)
       E-0x10c 0x10c-0x100 buffer 2 (arg4 of cm_fn_00944cff)
 */
#include "cmre.h"

#define DAT_00ae23b4 CM_GLOBAL(int, 0x00ae23b4)
#define DAT_00ae23bc CM_GLOBAL(int, 0x00ae23bc)
#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(int *, 0x00b67a34)
#define MGR_00de1f54 CM_GLOBAL(volatile u32, 0x00de1f54)
#define MGR_00b7431c CM_GLOBAL(volatile u32, 0x00b7431c)

extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self_arg, u32 index_arg, u32 value_arg);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *self, u32 arg1, u32 arg2);
extern u32 CM_THISCALL cm_fn_008c1870(u32 self, u32 target);
extern u32 CM_THISCALL cm_fn_008c5cf0(u32 self, u32 a1, u32 a2, u32 a3);
extern u32 CM_THISCALL cm_fn_008c5e40(u32 self, volatile u32 record_arg, volatile u32 output_arg);
/* 0x0090d130 is entered with three dwords pushed right to left at 0x004d9fd6
   (0x2d91, edx, eax) and its arguments stay on the stack until the single
   "add esp,0x24" at 0x004da002, so it is a cdecl three-argument routine. */
extern char *CM_CDECL cm_fn_0090d130(void *arg1, void *arg2, u32 arg3);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

/* The original ends with "ret 4" (0x004da18b): the callee pops its single
   stack argument, so the entry convention is stdcall, not plain cdecl. */
void CM_STDCALL cm_fn_004d9f80(int *param_1)
{
  u8 scratch[0x2e0];
  void *obj;   /* E-0x2ec, the command object */
  u8 *buf1;    /* E-0x20c */
  u8 *buf2;    /* E-0x10c */
  volatile struct { void *prev; void *handler; s32 state; } seh;
  u32 a, base, ecx, eax, edx, ebx_v, rec, mgr;

  obj = (void *)scratch;
  buf1 = scratch + 0xe0;
  buf2 = scratch + 0x1e0;

  /* 0x004d9f80..0x004d9f8e: SEH frame {prev, handler, state}. */
  seh.prev = CM_FS(void *, 0);
  seh.handler = (void *)0x00956fab;
  seh.state = -1;
  CM_FS(void *, 0) = (void *)&seh;

  if (param_1 == 0) {
    char *msg;

    /* 0x004d9fa9..0x004d9fc5 */
    cm_fn_00944cff((u32)0x009a8190, 0, 0, (void *)buf2, (void *)buf1);
    /* 0x004d9fc8..0x004d9fdd: args 0x2d91, edx(buf1), eax(buf2) right to left. */
    msg = cm_fn_0090d130((void *)buf2, (void *)buf1, 0x2d91);
    /* 0x004d9fe2..0x004d9fed: the three arguments left on the stack by
       cm_fn_0090d130 (buf2, buf1, 0x2d91) are the trailing variadic words
       of the format call, so they are passed here in the same order. */
    cm_fn_00944c9f((u32)&DAT_00ae24d0, (u32)0x009870e8, (u32)msg, (u32)buf2,
                   (u32)buf1, 0x2d91);
    /* 0x004d9ff2..0x004d9ffd: args 0 (edi), 0x00ae24d0, 0x009870e0. */
    cm_fn_005e8290((const char *)0x009870e0, (const char *)&DAT_00ae24d0, 0);
    /* 0x004da005 */
    DAT_00b67a34 = param_1;
  } else {
    u32 flag;
    u32 p;

    /* 0x004da010..0x004da025: ebx = DAT_00ae23b4 + param_1[1] + param_1[0]*0x244 */
    a = (u32)param_1[1];
    base = (u32)DAT_00ae23b4;
    base = base + a;
    ecx = a + a * 8u;
    ecx = ecx << 4;
    ecx = ecx + a;
    eax = (u32)param_1[0];
    ebx_v = base + ecx * 4u;
    /* 0x004da028..0x004da03c: esi = DAT_00ae23bc + param_1[0]*0x6e */
    ecx = eax + eax * 4u;
    eax = eax + ecx * 2u;
    mgr = MGR_00de1f54;
    edx = eax + eax * 4u;
    eax = (u32)DAT_00ae23bc;
    rec = eax + edx * 2u;

    /* 0x004da03f..0x004da047 */
    if (cm_fn_008c1870(mgr, rec) != 0)
      goto done;
    /* 0x004da04d..0x004da05d */
    mgr = MGR_00de1f54;
    if (cm_fn_008c5cf0(mgr, rec, 0, 0) != 0)
      goto done;
    /* 0x004da063..0x004da072 */
    mgr = MGR_00de1f54;
    if (cm_fn_008c5e40(mgr, rec, 0) != 0)
      goto done;

    /* 0x004da078..0x004da082: args 0 (eax), 0xbc2; receiver is the object. */
    cm_fn_0077bbe0(obj, 0xbc2, 0);
    /* 0x004da087: value for index 0, read before the state store. */
    ecx = *(volatile u32 *)ebx_v;
    /* 0x004da089: dword store of 0 into the SEH state word. */
    seh.state = 0;
    /* 0x004da094..0x004da09b: index 0 */
    cm_fn_00785790(obj, 0, ecx);
    /* 0x004da0a0..0x004da0a9: index 2 */
    ecx = *(volatile u32 *)rec;
    cm_fn_00785790(obj, 2, ecx);
    /* 0x004da0ae..0x004da0bb: index 3 */
    eax = *(volatile u32 *)(rec + 4);
    ecx = *(volatile u32 *)(eax + 0x33);
    cm_fn_00785790(obj, 3, ecx);
    /* 0x004da0c0..0x004da0cd: index 4 */
    edx = *(volatile u32 *)(rec + 8);
    eax = *(volatile u32 *)(edx + 0x33);
    cm_fn_00785790(obj, 4, eax);
    /* 0x004da0d2..0x004da0df: index 5 */
    ecx = *(volatile u32 *)(rec + 0xc);
    edx = *(volatile u32 *)(ecx + 0x33);
    cm_fn_00785790(obj, 5, edx);
    /* 0x004da0e4..0x004da0f4: index 6, high nibble of the byte, sign extended */
    {
      u8 b = *(volatile u8 *)((u32)param_1 + 0x4f);
      s32 sv = (s32)(s8)(b >> 4);
      cm_fn_00785790(obj, 6, (u32)sv);
    }
    /* 0x004da0f9..0x004da101: index 7 */
    cm_fn_00785790(obj, 7, 0);
    /* 0x004da106..0x004da111: index 8, sign extended word */
    {
      s32 sv = (s32)(s16)*(volatile u16 *)((u32)param_1 + 0x2d);
      cm_fn_00785790(obj, 8, (u32)sv);
    }
    /* 0x004da116..0x004da123: index 9, zero extended word in eax */
    {
      u32 w = (u32)*(volatile u16 *)((u32)param_1 + 0x2f);
      cm_fn_00785790(obj, 9, w);
    }
    /* 0x004da128..0x004da132: index 10 */
    cm_fn_00785790(obj, 10, *(volatile u32 *)((u32)param_1 + 0xc));

    /* 0x004da137..0x004da154 */
    flag = 0;
    if ((s32)(s8)*(volatile u8 *)(rec + 0x59) < 10) {
      if ((s32)(s8)*(volatile u8 *)(rec + 0x5b) < 5) {
        p = *(volatile u32 *)(rec + 0x61);
        if (p != 0) {
          if ((s32)(s16)*(volatile u16 *)(p + 0xd) > 0x128e)
            flag = 1;
        }
      }
    }
    /* 0x004da158..0x004da15e: index 11 */
    cm_fn_00785790(obj, 0xb, flag);
    /* 0x004da163..0x004da16f */
    cm_fn_007861e0((void *)MGR_00b7431c, (u32)obj, ebx_v);
  }

done:
  /* 0x004da174..0x004da17e: restore the saved SEH predecessor. */
  CM_FS(void *, 0) = seh.prev;
}

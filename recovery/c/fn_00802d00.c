/* fn_00802d00 -- network.h / scrman.cpp, preferred-image VA 0x00802d00.
 *
 * Translated from the original instruction listing only.  Callee boundaries
 * stay explicit.  Frame notes taken from the listing:
 *   - `sub esp,0x204` then three pushes (ebx,esi,edi): L = esp after the sub.
 *     Stack arguments sit at L+0x208 (arg1) and L+0x20C (arg2); the two
 *     scratch buffers at L+0x004 and L+0x104.
 *   - `ret 8` pops the two stack arguments; ECX is read before written, so
 *     this is a thiscall with the object in ECX.
 *   - the loop counter is stored over the saved EBX slot (L-0x004).
 *   - callee shapes come from the call sites: 0x564080 pops 0xc (three
 *     stack arguments, ECX unused), 0x77ac30 is a thiscall with three
 *     stack arguments, 0x90d130 / 0x5e8290 / 0x944c9f / 0x944cff are cdecl
 *     (the original clears their arguments with `add esp`).
 */
#include "cmre.h"

#ifndef NULL
#define NULL ((void *)0)
#endif

/* Original globals at their preferred-image addresses. */
#define STR_9870E0 ((const char *)0x009870e0)
#define STR_9870E8 ((const char *)0x009870e8)
#define STR_9C3734 ((const char *)0x009c3734)
#define STR_A7DDBC ((const char *)0x00a7ddbc)
#define DAT_AE24D0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_B67A34 CM_GLOBAL(volatile u32, 0x00b67a34)

/* Callee boundaries. */
extern short CM_THISCALL cm_fn_00564080(u32 self, u32 param_1, u32 param_2, u32 param_3);
extern u32 CM_CDECL cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern u32 CM_THISCALL cm_fn_0077ac30(u32 self, u32 param_2, u32 param_3, u32 param_4);
extern char * CM_CDECL cm_fn_0090d130(void *param_1, void *param_2, u32 param_3);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

void CM_THISCALL cm_fn_00802d00(u32 self, int param_1, int param_2)
{
  /* One allocation keeps the two scratch buffers adjacent, as in the
   * original frame (L+0x004 and L+0x104). */
  u8 scratch[512];
  u8 *buf_a = scratch;         /* original L+0x004 */
  u8 *buf_b = scratch + 256;   /* original L+0x104 */

  volatile u16 *field_index = (volatile u16 *)(self + 0x382a);
  volatile u32 *field_base = (volatile u32 *)(self + 0x37fe);
  volatile u32 *field_count = (volatile u32 *)(self + 0x3802);
  volatile u32 *field_limit = (volatile u32 *)(self + 0x3806);
  volatile u16 *field_rows = (volatile u16 *)(self + 0x380a);
  volatile u16 *field_slots = (volatile u16 *)(self + 0x7d4);
  volatile u32 *field_peer = (volatile u32 *)(self + 0x3840);

  u32 eax_value;   /* models the original EAX across partial writes */
  u32 index;
  u32 offset;
  u32 net;

  index = (u32)(s32)(s16)*field_index;      /* movsx eax, word ptr [esi+0x382a] */
  offset = index * 3u * 256u;                /* lea eax,[eax+eax*2] ; shl eax,8 */
  net = *(volatile u32 *)(self + 0x7fc + offset);
  if (*(volatile u32 *)(net + 0x12f53d) != 0u) {
    return;                                   /* jne 0x802f6b */
  }

  *field_count = 0u;                          /* mov dword ptr [esi+0x3802], ebx */
  {
    u32 used = *field_count;                  /* mov eax, dword ptr [edi+4] */
    u32 limit = *field_limit;                 /* mov ecx, dword ptr [edi+8] */
    if (used + 1u <= limit) {                 /* lea edx,[eax+1] ; cmp edx,ecx ; jbe */
      u32 base = *field_base;                 /* mov ecx, dword ptr [edi] */
      *(volatile u8 *)(base + used) = (u8)9;  /* mov byte ptr [eax+ecx], 9 */
      used = *field_count;                    /* mov eax, dword ptr [edi+4] */
      *field_count = used + 1u;               /* inc eax ; mov dword ptr [edi+4],eax */
      eax_value = used + 1u;                  /* EAX after the store */
    } else {
      char *text;
      cm_fn_00944cff((u32)STR_9C3734, 0u, 0u, buf_b, buf_a);
      text = cm_fn_0090d130(buf_b, buf_a, 0x85u);
      /* The original leaves the 0x90d130 arguments on the stack, so the
       * formatter reads version, file, extension and the id from them. */
      eax_value = cm_fn_00944c9f((u32)(void *)&DAT_AE24D0, (u32)STR_9870E8,
                                 (u32)text, (u32)(void *)buf_b, (u32)(void *)buf_a, 0x85u);
      eax_value = cm_fn_005e8290(STR_9870E0, (const char *)(void *)&DAT_AE24D0, 0);
      DAT_B67A34 = 0u;
    }
  }

  if (param_1 != 0) {
    /* 0x802dbe: the loop counter lives over the saved EBX slot. */
    u32 iters = 0x10u;
    volatile u16 *rows = field_rows;
    volatile u16 *slots = field_slots;
    for (;;) {
      if ((s16)*rows > 0) {                  /* cmp word ptr [ebp],0 ; jle 0x802e89 */
        u32 word = *slots;                   /* mov ax, word ptr [ebx] */
        if (word != 0u) {                    /* test ax,ax ; jne 0x802e0c */
          u32 peer = *field_peer;            /* mov ecx, dword ptr [esi+0x3840] */
          u32 count = *field_count;          /* mov ecx, dword ptr [esi+0x3802] */
          u32 base = *field_base;            /* mov edx, dword ptr [edi] */
          u32 arg3 = (eax_value & 0xffff0000u) | word;
          u32 res = cm_fn_0077ac30(peer, base, count, arg3);
          if (res == 0u) {                   /* test eax,eax ; jne 0x802e89 */
            char *text;
            cm_fn_00944cff((u32)STR_A7DDBC, 0u, 0u, buf_a, buf_b);
            text = cm_fn_0090d130(buf_a, buf_b, 0xc40u);
            eax_value = cm_fn_00944c9f((u32)(void *)&DAT_AE24D0, (u32)STR_9870E8,
                                       (u32)text, (u32)(void *)buf_a, (u32)(void *)buf_b, 0xc40u);
            eax_value = cm_fn_005e8290(STR_9870E0, (const char *)(void *)&DAT_AE24D0, 0);
            DAT_B67A34 = 0u;
          }
        } else if (param_2 == 0) {            /* mov eax,[esp+0x21c] ; test ; jne */
          u32 peer = *field_peer;            /* mov edx, dword ptr [esi+0x3840] */
          cm_fn_00564080(self + 0x3844, peer, (short)0, self + 0x37fe);
        }
      }
      /* 0x802e89: loop tail. */
      iters--;
      eax_value = iters;                     /* mov eax,[esp+0x10] ; dec eax */
      rows++;
      slots += 0x180u;                       /* add ebx,0x300 */
      if (iters == 0u) {
        break;
      }
    }
    return;
  }

  /* 0x802eae: arg1 == 0. */
  {
    u32 index2 = (u32)(s32)(s16)*field_index;   /* movsx eax, word ptr [esi+0x382a] */
    u32 offset2 = index2 * 3u * 256u;           /* lea ecx,[eax+eax*2] ; shl ecx,8 */
    u32 word = *(volatile u16 *)(self + 0x7d4 + offset2);  /* mov ax, word ptr [...] */
    if (word != 0u) {                           /* cmp ax,bx ; jne 0x802ef5 */
      u32 peer = *field_peer;                   /* mov ecx, dword ptr [esi+0x3840] */
      u32 count = *field_count;                 /* mov ecx, dword ptr [esi+0x3802] */
      u32 base = *field_base;                   /* mov edi, dword ptr [edi] */
      u32 arg3 = (index2 & 0xffff0000u) | word;
      u32 res = cm_fn_0077ac30(peer, base, count, arg3);
      if (res == 0u) {                          /* test eax,eax ; jne 0x802f6b */
        char *text;
        cm_fn_00944cff((u32)STR_A7DDBC, 0u, 0u, buf_a, buf_b);
        text = cm_fn_0090d130(buf_a, buf_b, 0xc54u);
        cm_fn_00944c9f((u32)(void *)&DAT_AE24D0, (u32)STR_9870E8,
                       (u32)text, (u32)(void *)buf_a, (u32)(void *)buf_b, 0xc54u);
        cm_fn_005e8290(STR_9870E0, (const char *)(void *)&DAT_AE24D0, 0);
        DAT_B67A34 = 0u;
      }
    } else if (param_2 == 0) {                  /* cmp dword ptr [esp+0x218],ebx ; jne */
      u32 peer = *field_peer;                   /* mov edx, dword ptr [esi+0x3840] */
      cm_fn_00564080(self + 0x3844, peer, (short)0, self + 0x37fe);
      return;
    }
  }
}

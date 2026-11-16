/* fn_009094c0 - module comp/leagues/usa_mls.cpp
   Translated from the original instructions at preferred-image VA 0x009094c0,
   image base 0x00400000, pinned CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1c627a22c445d14d489bd9bff86.

   Convention: ECX is read before it is written and the routine ends with a bare
   `ret` (no stack bytes popped), so the object is the thiscall receiver and the
   routine takes no stack argument.

   Frame map used below (L = ESP after `sub esp,0x460`):
     L-0x10..L-0x04 saved EDI/ESI/EBP/EBX, L+0x0..L+0x45f locals,
     L+0x460 prev, L+0x464 handler (0x0096541e), L+0x468 SEH state.
   Locals:
     L+0x00 u16-source home, L+0x04 loop counter, L+0x08 object home
     (this, later the 0xb2 allocation), L+0x0c first output pointer,
     L+0x10 club-base home, L+0x14 second output pointer,
     L+0x18 6-byte record array (3 rows, re-initialised as 9 rows),
     L+0x060 / L+0x260 first error-message buffers,
     L+0x160 / L+0x360 second error-message buffers.
   The SEH state field (L+0x468) is written -1 at entry, 0 at 0x009096ec and
   -1 again at 0x00909727; it is the linked record's third dword. */

#include "cmre.h"

#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)
#define LAB_0090a130 CM_GLOBAL(u8, 0x0090a130)

#define SRC_00aa1fb4 ((char *)0x00aa1fb4)
#define STR_009870e0 ((char *)0x009870e0)
#define STR_009870e8 ((char *)0x009870e8)

#define SEH_HANDLER_0096541e 0x0096541eu

typedef struct {
  volatile u32 prev;
  volatile u32 handler;
  volatile u32 state;
} cm_seh_record;

/* `call dword ptr [eax + 0x3c]` after `mov ecx, esi`: thiscall slot taking
   selector 2, two output pointers and a zero, cleaning 16 stack bytes (the
   listing keeps ESP unchanged across it, e.g. 0x009096d4 -> 0x0090970d). The
   output widths of the target are not proven by this caller; only the dwords
   stored into L+0x0c / L+0x14 are observable here. */
typedef u32 (CM_THISCALL *cm_slot_0090x3c_fn)(u32 self, u32 selector,
                                              u32 *out_a, u32 *out_b, u32 zero);

extern u32 CM_THISCALL cm_fn_0051c800(u32 self, volatile u32 round_arg);
extern u32 CM_THISCALL cm_fn_00522e00(u32 self, u32 owner, s16 count, u32 clubs,
                                      s16 rounds, u32 competition, u32 schedule,
                                      u16 year, u8 flag_a, u8 flag_b,
                                      u16 round_flag, u8 flag_c, u32 builder,
                                      u32 numbered, u32 value_38,
                                      u32 club_flags);
extern void cm_fn_005e8290(const char * param_1, const char * param_2,
                           int param_3);
extern char * CM_CDECL cm_fn_0090d130(u32 param_1, u32 param_2, u32 param_3);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4,
                                   void *arg5);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);
/* 0x00945333 is called at 0x009095b7 and 0x00909692 with exactly four pushed
   dwords (base, count, width, comparator) and `add esp,0x10` at 0x009095c0 /
   0x009096bb, so it is plain cdecl with four stack arguments. EAX/EDX at those
   call sites are incidental (0x009095a3 loads a club base into EAX only for the
   [eax+0x19] read; 0x0090968b's EAX is the pushed base), not arguments. */
extern unsigned long long CM_CDECL cm_fn_00945333(u32 base_arg, u32 count_arg,
                                                  u32 width_arg,
                                                  u32 compare_arg);

void CM_THISCALL cm_fn_009094c0(u32 self)
{
  cm_seh_record frame;
  u8 L[0x460];
  u8 *rows;
  u32 h8;
  u32 club_a;
  u32 club_b;
  u32 this_r;
  u32 rec;
  s8 al;
  u32 off;
  u32 cnt;

  frame.state = 0xffffffffu;              /* 0x009094c0 push -1 */
  frame.handler = SEH_HANDLER_0096541e;   /* 0x009094c2 push 0x96541e */
  frame.prev = CM_FS(u32, 0);             /* 0x009094c7 mov eax, fs:[0] */
  CM_FS(u32, 0) = (u32)(void *)&frame;    /* 0x009094ce mov fs:[0], esp */

  rows = L + 0x18;

  {
    u32 base = *(volatile u32 *)(self + 0xc);  /* 0x009094e0 */
    club_a = *(u32 *)(base + 0);               /* 0x009094ea */
    club_b = *(u32 *)(base + 4);               /* 0x009094ec */
  }
  h8 = self;                                   /* 0x009094e6 */
  rec = (u32)cm_fn_00944e46(0x20);             /* 0x009094ef call 0x944e46 */

  if (rec == 0) {
    /* 0x009094fd: first failure diagnostic (line 0x58d) */
    cm_fn_00944cff((u32)(void *)SRC_00aa1fb4, 0, 0,
                   (void *)(void *)(L + 0x260), (void *)(void *)(L + 0x60));
    {
      u32 msg = (u32)cm_fn_0090d130((u32)(void *)(L + 0x260),
                                    (u32)(void *)(L + 0x60), 0x58d);
      /* 0x0090d130 is cdecl and leaves its three arguments on the stack
         (0x0090952b -> 0x00909530 has no cleanup); the single `add esp,0x24` at
         0x00909550 covers exactly those 12 bytes, the 12 pushed here and the 12
         pushed for 0x005e8290. The `v%s_%s_%s_%d` template at 0x009870e8 thus
         consumes the still-live (buffer, buffer, 0x58d) arguments. */
      cm_fn_00944c9f((u32)(void *)&DAT_00ae24d0,
                     (u32)(void *)STR_009870e8, msg,
                     (u32)(void *)(L + 0x260), (u32)(void *)(L + 0x60), 0x58d);
      cm_fn_005e8290(STR_009870e0, (const char *)(void *)&DAT_00ae24d0, 0);
    }
    DAT_00b67a34 = 0;
  }
  else {
    u32 this_b1;
    u32 club_b1;
    u32 v;

    /* ---- qsort pass 1: three 6-byte rows (id, u16) at L+0x18 ---- */
    this_b1 = *(volatile u32 *)(self + 0xb1);        /* 0x0090955e */
    *(u32 *)(rows + 0) = *(u32 *)(this_b1 + 0);      /* 0x0090956d */
    *(u16 *)(rows + 4) = *(u16 *)(this_b1 + 0x19);   /* 0x00909573 */

    club_b1 = *(volatile u32 *)(club_a + 0xb1);      /* 0x0090957c */
    *(u32 *)(rows + 6) = *(u32 *)(club_b1 + 0);      /* 0x00909584 */
    club_b1 = *(volatile u32 *)(club_a + 0xb1);      /* 0x00909588 */
    *(u16 *)(rows + 10) = *(u16 *)(club_b1 + 0x19);  /* 0x0090958e */

    club_b1 = *(volatile u32 *)(club_b + 0xb1);      /* 0x00909597 */
    *(u32 *)(rows + 12) = *(u32 *)(club_b1 + 0);     /* 0x0090959f */
    club_b1 = *(volatile u32 *)(club_b + 0xb1);      /* 0x009095a3 */
    *(u16 *)(rows + 16) = *(u16 *)(club_b1 + 0x19);  /* 0x009095ad */

    /* base = L+0x18, count 3, width 6, comparator 0x0090a130 (0x009095b7) */
    cm_fn_00945333((u32)(void *)rows, 3, 6,
                   (u32)(void *)&LAB_0090a130);     /* 0x009095b7 */

    *(u32 *)(rec + 4) = *(u32 *)(rows + 0);         /* 0x009095bc/0x009095c3 */
    *(u32 *)(rec + 0xc) = *(u32 *)(rows + 6);       /* 0x009095c6 */
    *(u32 *)(rec + 0x14) = *(u32 *)(rows + 12);     /* 0x009095cd */

    /* ---- nine rows of 6 bytes built from three club bases ---- */
    v = *(volatile u32 *)(self + 0xb1);             /* 0x009095d4 */
    *(u32 *)(L + 0x10) = v;                         /* 0x009095dc */
    *(u32 *)(L + 0x00) = v + 0x54;                  /* 0x009095e8 */
    *(u32 *)(L + 0x04) = 3;                         /* 0x009095ec */
    al = 0;                                         /* 0x009095da */
    off = 0x3b;                                     /* 0x009095e3 */
    cnt = 3;
    do {
      u32 src;
      u32 idx;

      src = *(u32 *)(L + 0x10);                     /* 0x009095fa */
      v = *(u32 *)(src + off);                      /* 0x009095fd */
      off += 0x3b;                                  /* 0x00909600 */
      idx = (u32)(s32)al * 6;                       /* 0x00909603/0x00909606 */
      al = (s8)(al + 1);                            /* 0x00909608 */
      *(u32 *)(rows + idx) = v;                     /* 0x0090960a */
      *(u16 *)(rows + idx + 4) = *(u16 *)(*(u32 *)(L + 0x00));  /* 0x0090960e.. */

      src = *(volatile u32 *)(club_a + 0xb1);       /* 0x0090961a */
      idx = (u32)(s32)al * 6;                       /* 0x00909620/0x00909627 */
      v = *(u32 *)(src + (off - 0x3b));             /* 0x00909623 */
      al = (s8)(al + 1);                            /* 0x0090962c */
      *(u32 *)(rows + idx) = v;                     /* 0x0090962e */
      src = *(volatile u32 *)(club_a + 0xb1);       /* 0x00909632 */
      *(u16 *)(rows + idx + 4) = *(u16 *)(src + (off - 0x22));  /* 0x00909638 */

      src = *(volatile u32 *)(club_b + 0xb1);       /* 0x00909642 */
      idx = (u32)(s32)al * 6;                       /* 0x00909648 */
      v = *(u32 *)(src + (off - 0x3b));             /* 0x0090964b */
      al = (s8)(al + 1);                            /* 0x00909654 */
      *(u32 *)(rows + idx) = v;                     /* 0x00909656 */
      src = *(volatile u32 *)(club_b + 0xb1);       /* 0x0090965a */
      *(u16 *)(rows + idx + 4) = *(u16 *)(src + (off - 0x22));  /* 0x00909660 */

      *(u32 *)(L + 0x00) += 0x3b;                   /* 0x0090966a..0x00909676 */
      cnt = cnt - 1;                                /* 0x00909675 */
    } while (cnt != 0);                             /* 0x0090967e */

    /* ---- qsort pass 2: the same array now holds nine 6-byte rows ---- */
    cm_fn_00945333((u32)(void *)rows, 9, 6,
                   (u32)(void *)&LAB_0090a130);     /* 0x00909692 */

    *(u32 *)(rec + 0x1c) = *(u32 *)(rows + 0);      /* 0x00909697 */
    this_r = h8;                                    /* 0x0090969b (self reload) */
    *(u32 *)(rec + 0x18) = *(u32 *)(rows + 6);      /* 0x009096a2 */
    *(u32 *)(rec + 0x10) = *(u32 *)(rows + 12);     /* 0x009096a9 */
    *(u32 *)(rec + 8) = *(u32 *)(rows + 18);        /* 0x009096b0 */
    *(u32 *)(rec + 0) = *(u32 *)(rows + 24);        /* 0x009096b7 */

    {
      u32 out_a;
      u32 out_b;
      u32 vt;
      u32 sched;
      u32 created;
      u32 result;

      vt = *(volatile u32 *)(this_r + 0);           /* 0x009096c3 */
      sched = ((cm_slot_0090x3c_fn)(*(u32 *)(vt + 0x3c)))
                  (this_r, 2, &out_a, &out_b, 0);   /* 0x009096d4 */

      created = (u32)cm_fn_00944cf1(0xb2);          /* 0x009096de */
      h8 = created;                                 /* 0x009096e6 */
      frame.state = 0;                              /* 0x009096ec */
      if (created == 0) {
        result = 0;                                 /* 0x00909721 */
      }
      else {
        result = cm_fn_00522e00(created, this_r, 8, rec, (s16)out_a,
                                *(volatile u32 *)(this_r + 4), sched,
                                (u16)*(volatile u16 *)(this_r + 0x40),
                                2, 1, (u16)out_b, 0x12, 1, 0, 0, 0);
      }                                             /* 0x0090971a */

      {
        u32 holder;
        u32 stored;

        holder = *(volatile u32 *)(this_r + 0xc);   /* 0x00909723 */
        /* Keep state zero through the holder load, including a load fault. */
        frame.state = 0xffffffffu;                 /* 0x00909727 */
        *(u32 *)(holder + 8) = result;              /* 0x00909732 */
        cm_fn_009452ca(sched);                      /* 0x00909735 */
        holder = *(volatile u32 *)(this_r + 0xc);   /* 0x0090973a */
        stored = *(u32 *)(holder + 8);              /* 0x00909740 */
        if (stored != 0) {
          cm_fn_0051c800(stored, 0);                /* 0x009097ac */
        }
        else {
          /* 0x00909747: second failure diagnostic (line 0x5c4) */
          cm_fn_00944cff((u32)(void *)SRC_00aa1fb4, 0, 0,
                         (void *)(void *)(L + 0x360), (void *)(void *)(L + 0x160));
          {
            u32 msg = (u32)cm_fn_0090d130((u32)(void *)(L + 0x360),
                                          (u32)(void *)(L + 0x160), 0x5c4);
            /* Same reuse of the live 0x90d130 arguments (line 0x5c4). */
            cm_fn_00944c9f((u32)(void *)&DAT_00ae24d0,
                           (u32)(void *)STR_009870e8, msg,
                           (u32)(void *)(L + 0x360),
                           (u32)(void *)(L + 0x160), 0x5c4);
            cm_fn_005e8290(STR_009870e0, (const char *)(void *)&DAT_00ae24d0, 0);
          }
          DAT_00b67a34 = 0;
        }
      }
    }
  }

  CM_FS(u32, 0) = frame.prev;                     /* 0x009097bc mov fs:[0], ecx */
}

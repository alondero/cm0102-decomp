/* Recovered from CM0102 3.9.68 (SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86),
   preferred-image VA 0x005b91c0, image base 0x00400000. */
#include "cmre.h"

#define G_009cf5cc CM_GLOBAL(u32, 0x009cf5cc)
#define G_009cf5d0 CM_GLOBAL(u32, 0x009cf5d0)
#define G_009cf5d8 CM_GLOBAL(u32, 0x009cf5d8)
#define G_009cf764 CM_GLOBAL(u32, 0x009cf764)
#define G_00ae2350 CM_GLOBAL(u32, 0x00ae2350)
#define G_00ae235c CM_GLOBAL(u32, 0x00ae235c)
#define G_00ae23b4 CM_GLOBAL(u32, 0x00ae23b4)
#define G_00ae23d0 CM_GLOBAL(u32, 0x00ae23d0)
#define G_00b67a34 CM_GLOBAL(u32, 0x00b67a34)

extern u32 CM_CDECL cm_fn_004afb40(u32 array, u32 raw_count);
extern u32 CM_THISCALL cm_fn_005b9fa0(u32 self, u32 clubs);
extern u32 CM_THISCALL cm_fn_005ba0a0(void *self);
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern u32 CM_THISCALL cm_fn_00682200(void *self);
extern u32 CM_FASTCALL cm_fn_006835c0(int *param_1);
extern void CM_THISCALL cm_fn_00687970(u8 *self, u8 *buffer);
extern char *cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4,
                                   void *arg5);

typedef struct {
  u32 prev;
  u32 handler;
  u32 state;
} cm_eh_frame;

/* One scratch allocation for the original frame F (after `sub esp, 0x32c`,
   F = entry_esp - 0x338). Physical slots, from the listing's esp-relative
   accesses (esp = F - 0x0c after the three saved-register pushes):
     F+0x000  dword zeroed at 0x005b9216, low word compared signed with 0x22
              at 0x005b92e8 and incremented as a dword at 0x005b92fb/9304
     F+0x004  dword index zeroed at 0x005b9285, reloaded at 0x005b930a
     F+0x008  dword receiver copy; original unwind handler reads this slot
     F+0x00c  byte buffer A [0x100]   (lea [esp+0x18], esp = F-0x0c)
     F+0x10c  byte buffer B [0x100]   (lea [esp+0x118])
     F+0x20c  4 records x 0x48 bytes  (lea [esp+0x21c])
   `frame` models the chain fields, not the original unwind slot placement. */
u32 * CM_THISCALL cm_fn_005b91c0(u32 *self, u16 param_2)

{
  volatile cm_eh_frame frame;
  u32 scratch[0x32c / 4];
  u8 *const F = (u8 *)scratch;
  u32 eax;
  u32 edi;
  u32 v5d8;

  frame.state = 0xffffffffu;                   /* 0x005b91c0 */
  frame.handler = 0x009598abu;                 /* 0x005b91c2 */
  frame.prev = CM_FS(u32, 0);                  /* 0x005b91c7 */
  CM_FS(u32, 0) = (u32)&frame;                 /* 0x005b91ce */

  *(u32 *)(F + 0x008) = (u32)self;             /* 0x005b91e0 */
  cm_fn_00682200(self);                        /* 0x005b91e4, ecx = esi */
  {
    u32 ecx_;
    u32 edx_;

    *(volatile u32 *)self = 0x0096a7b0u;
    *(volatile u16 *)((u8 *)self + 0x40) = param_2;
    v5d8 = CM_GLOBAL(volatile u32, 0x009cf5d8);
    edx_ = CM_GLOBAL(volatile u32, 0x00ae23d0);
    edi = 0;                                   /* 0x005b9206 */
    ecx_ = v5d8 + v5d8 * 2u;                   /* 0x005b9208 */
    frame.state = 0;                           /* 0x005b920b */
    *(u32 *)(F + 0x000) = edi;                 /* 0x005b9216 */
    *(volatile u32 *)((u8 *)self + 0x2c) = edi;
    ecx_ = ecx_ + ecx_ * 8u;                   /* 0x005b921d */
    ecx_ = ecx_ << 2;                          /* 0x005b9220 */
    ecx_ = ecx_ - v5d8;                        /* 0x005b9223 */
    ecx_ = ecx_ + edx_;                        /* 0x005b9228 */
    *(volatile u8 *)((u8 *)self + 0x50) = 0xff;
    *(volatile u32 *)((u8 *)self + 0x04) = ecx_;
    *(volatile u8 *)((u8 *)self + 0x44) = 0xff;
    *(volatile u32 *)((u8 *)self + 0x30) = 0xffffffffu;
  }

  eax = cm_fn_005ba0a0(self);                  /* 0x005b9238, ecx = esi */
  if (eax == 0) {                              /* 0x005b923d */
    cm_fn_00944cff(0x009ccaf0u, 0, 0, (void *)(F + 0x00c),
                   (void *)(F + 0x10c));       /* 0x005b924c */
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, (u32)cm_fn_0090d130(),
                   (u32)(F + 0x00c), (u32)(F + 0x10c), 0x40u);
    cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
    G_00b67a34 = 0;                            /* 0x005b93ee */
  } else {
    u32 c2350 = G_00ae2350;                    /* 0x005b9270 */
    u32 c235c = G_00ae235c;                    /* 0x005b9276 */
    u32 rec = G_00ae23b4;                      /* 0x005b927c */
    u32 lim = c235c - (c2350 + c2350);         /* 0x005b9282/0x005b928d */

    *(u32 *)(F + 0x004) = 0;                   /* 0x005b9285 */
    if ((s32)lim > 0) {                        /* 0x005b928f */
      for (;;) {
        u32 pv = *(u32 *)(rec + 0x57);         /* 0x005b9297 */
        if (pv != 0) {
          u32 vt = *(volatile u32 *)pv;        /* 0x005b929e */
          if (vt == G_009cf5cc || vt == G_009cf5d0) {
            s32 idx = (s32)(s16)(u16)edi;      /* 0x005b92b2 movsx ecx, di */
            edi = edi + 1;                     /* 0x005b92b5 */
            ((u32 *)(F + 0x20c))[(u32)idx] = rec; /* 0x005b92ba */
            if ((u16)edi == 0x48) goto after_loop; /* 0x005b92be */
          } else {
            u32 vt2 = *(volatile u32 *)pv;     /* 0x005b92c9 reload */
            u32 q;
            if (vt2 != G_009cf764) goto next;  /* 0x005b92d1 */
            q = *(u32 *)(rec + 0x60);          /* 0x005b92d5 */
            if (q == 0) goto next;             /* 0x005b92d8 */
            if (*(volatile u32 *)q != G_009cf5d8) goto next; /* 0x005b92dc */
            if ((s16)(u16)(*(u32 *)(F + 0x000)) >= 0x22) goto next; /* 0x005b92e8 */
            {
              s32 idx2 = (s32)(s16)(u16)edi;   /* 0x005b92f0 */
              u32 c2;
              edi = edi + 1;                   /* 0x005b92f3 */
              ((u32 *)(F + 0x20c))[(u32)idx2] = rec; /* 0x005b92f4 */
              c2 = *(u32 *)(F + 0x000);        /* 0x005b92fb */
              c2 = c2 + 1;                     /* 0x005b92ff */
              *(u32 *)(F + 0x000) = c2;        /* 0x005b9304 */
              if ((u16)edi == 0x48) goto after_loop; /* 0x005b9308 */
            }
          }
        }
      next:
        rec = rec + 0x245;                     /* 0x005b930e */
        {
          u32 i = *(u32 *)(F + 0x004);         /* 0x005b930a */
          i = i + 1;                           /* 0x005b9313 */
          *(u32 *)(F + 0x004) = i;             /* 0x005b9316 */
          if (!((s32)i < (s32)lim)) break;     /* 0x005b931a */
        }
      }
    }
  after_loop:
    cm_fn_004afb40((u32)(F + 0x20c), edi);     /* 0x005b9320/0x005b9329 */

    edi = 0;                                   /* 0x005b9331 */
    {
      u32 el = (u32)(F + 0x20c);               /* 0x005b9333 */
      for (;;) {
        /* 0x005b933e: ecx = esi, argument ebx */
        if (cm_fn_005b9fa0((u32)self, el) == 0) { /* 0x005b933e */
          cm_fn_00944cff(0x009ccaf0u, 0, 0, (void *)(F + 0x10c),
                         (void *)(F + 0x00c));
          cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, (u32)cm_fn_0090d130(),
                         (u32)(F + 0x10c), (u32)(F + 0x00c), 0x64u);
          cm_fn_005e8290((const char *)0x009870e0u,
                         (const char *)0x00ae24d0u, 0);
          G_00b67a34 = 0;
          goto done;
        }
        /* 0x005b9349: ecx = esi (0x005b9347) */
        if (cm_fn_006835c0((int *)self) == 0) { /* 0x005b9349 */
          cm_fn_00944cff(0x009ccaf0u, 0, 0, (void *)(F + 0x10c),
                         (void *)(F + 0x00c)); /* 0x005b9398 */
          cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, (u32)cm_fn_0090d130(),
                         (u32)(F + 0x10c), (u32)(F + 0x00c), 0x6bu);
          cm_fn_005e8290((const char *)0x009870e0u,
                         (const char *)0x00ae24d0u, 0);
          G_00b67a34 = 0;
          goto done;
        }
        /* 0x005b9356: ecx = esi, pushed 0 */
        cm_fn_00687970((u8 *)self, (u8 *)0);
        edi = edi + 1;                         /* 0x005b935b */
        el = el + 0x48;                        /* 0x005b935c */
        if (!((s32)edi < 4)) break;            /* 0x005b9362 */
      }
    }
  }
done:
  CM_FS(u32, 0) = frame.prev;                  /* 0x005b9404 */
  return self;                                 /* 0x005b940b add esp; ret 4 */
}

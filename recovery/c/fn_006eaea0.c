/* fn_006eaea0 -- VA 0x006eaea0, preferred image base 0x00400000, CM0102 3.9.68.
   Reconstructed from the original instruction listing only.

   Frame: `sub esp,0x18` then push ebx/ebp/esi/edi.  With S = esp after the sub:
     S+3    byte flag  (set 0x006eb4fd, read 0x006eba98)
     S+4    dword      (result of 0x004cf5f0, stored 0x006eafa5)
     S+8    m64 "df"   (0x006eb59a / 0x006eb5fe / 0x006eb606)
     S+0x10 m64 "dp"   (0x006eb7ab), also a dword scratch at 0x006eb0cb
   The original also uses the caller-owned argument homes as int scratch slots:
   0x006eb5e8/0x006eb789/0x006eb7db -> arg1 home, 0x006eb7f1/0x006eb805 -> arg2
   home, 0x006eb8e8/0x006eb924/0x006eb91c -> arg3 home.
   The final buffers instead occupy the adjacent caller-owned arg4/arg5 homes.
   ABI evidence: CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86,
   preferred-image VAs: 006eaea0 reserves 0x18; saved-register pushes at
   006eaea7/006eaeac/006eaeb1/006eafac put ESP at entry-0x28.
   006ebcb9 pops EDI: ESP becomes entry-0x24. Thus 006ebcf8 [esp+0x34]
   is arg4, 006ebcfc [esp+0x38] is arg5, and 006ebd0f [esp+0x44]
   after four pushes reads arg4's low word. Full-width formals retain these
   caller homes (including untouched upper bytes), not narrow formal copies. */

#include "cmre.h"

/* ---- original callees: explicit boundaries, machine-level signatures ------ */
extern u32 CM_THISCALL cm_fn_0048f150(u32 self, u32 target_arg);
extern u32 CM_THISCALL cm_fn_004a4850(u32 self, u32 target);
extern u32 CM_CDECL cm_fn_004b0700(u32 v);
extern u32 CM_THISCALL cm_fn_004cf5f0(u32 self, u32 person, u32 club);
extern u32 CM_CDECL cm_fn_0053b640(u32 club, u8 kind, u32 first, u32 second);
extern u32 CM_CDECL cm_fn_0053b900(u32 *input);
extern u32 CM_CDECL cm_fn_005409b0(u32 *input);
extern long long CM_CDECL cm_fn_00544410(u32 record, u32 adjustment);
extern u32 CM_CDECL cm_fn_00544520(u32 person, u32 club);
extern u32 CM_CDECL cm_fn_00547770(u32 object, u32 second);
extern u32 CM_CDECL cm_fn_00547790(u32 first, u32 second);
extern u32 CM_CDECL cm_fn_005477d0(u32 first, u32 second);
extern u32 CM_CDECL cm_fn_00547840(u32 target, u32 object);
extern u32 CM_CDECL cm_fn_00600a50(u32 club, u32 skip_status, u32 flag, u32 person, u32 force);
extern u8  CM_THISCALL cm_fn_0062c400(u32 self, u32 person);
extern u32 CM_THISCALL cm_fn_0062c590(u32 self, u32 person, u32 optional);
extern u16 CM_THISCALL cm_fn_006c38c0(u32 self, u32 object);
extern u8  CM_THISCALL cm_fn_006d4280(u32 self);
extern u8  CM_STDCALL cm_fn_007a7eb0(s32 *param_1);
extern long double CM_THISCALL cm_fn_007c1e30(u32 self, u32 player, u32 category, u32 field, float incoming);
extern u32 CM_STDCALL cm_fn_007c3a40(u32 player, u32 club);
extern u32 CM_STDCALL cm_fn_007c3bb0(u32 player, u32 club);
extern u32 CM_STDCALL cm_fn_007c3d20(u32 player);
extern long double CM_THISCALL cm_fn_007c47b0(u32 self, u32 player, s8 kind);
extern void CM_THISCALL cm_fn_008b1750(u32 self, u32 a, u32 b);
extern u32 CM_CDECL cm_fn_0090cfc0(u32 bound);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);

/* BLOCKED boundary, not a recovered arithmetic operation.
   Original preferred-image VAs, CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86:
   006eb78d FILD, 006eb791 FMUL m64, 006eb797 FADD m64 leave ST0;
   006eb79d FLD m64 pushes a second operand;
   006eb7a3 CALL 00945ff0 has both operands in x87 registers;
   006eb7ab FSTP m64 consumes its result.
   The supplied contract has no plain-C transport for that boundary.
   Square-and-multiply is NOT instruction evidence for the helper, even for
   integral exponents, and substituting zero for other exponents invents
   behaviour. Both substitutions have been removed. Do not compile or accept
   the remainder until an evidence-reviewed transport/recovery contract covers
   helper spills, control word, x87 depth and observable state at O0 and O2. */
#include "cmre_x87.h"

/* 0x00945640 (MSVC __ftol) is `fistp qword` with the control word set to
   truncate: it truncates ST0 toward zero and stores the low 32 bits in EAX.
   Values that do not fit a 64-bit integer, and NaN, produce the x87 integer
   indefinite 0x8000000000000000, so EAX is 0 in those cases.  Spelled out here
   so the result does not depend on how the compiler lowers the cast. */
static long long cm_ftol_00945640(long double t)
{
  if (t >= -9223372036854775808.0L && t < 9223372036854775808.0L)
    return (long long)t;
  return -9223372036854775807LL - 1;              /* 0x8000000000000000 */
}

/* ---- original data ------------------------------------------------------ */
#define DAT_009cf36c  CM_GLOBAL(s32, 0x009cf36c)
#define DAT_009cf668  CM_GLOBAL(s32, 0x009cf668)
#define PTR_00adadfc  CM_GLOBAL(u32, 0x00adadfc)
#define DAT_00ae2364  CM_GLOBAL(s32, 0x00ae2364)
#define DAT_00ae236c  CM_GLOBAL(u32, 0x00ae236c)
#define DAT_00ae23c4  CM_GLOBAL(u32, 0x00ae23c4)
#define DAT_00ae23d0  CM_GLOBAL(u32, 0x00ae23d0)
#define DAT_00ae2c92  CM_GLOBAL(u16, 0x00ae2c92)
#define PTR_00ae2d14  CM_GLOBAL(u32, 0x00ae2d14)
#define PTR_00ae1050  CM_GLOBAL(u32, 0x00ae1050)
#define PTR_00b63d28  CM_GLOBAL(u32, 0x00b63d28)
#define PTR_00b7434c  CM_GLOBAL(u32, 0x00b7434c)
#define PTR_00b74350  CM_GLOBAL(u32, 0x00b74350)
#define PTR_00de1f44  CM_GLOBAL(u32, 0x00de1f44)

#define C_967890 CM_GLOBAL(volatile double, 0x00967890)
#define C_967888 CM_GLOBAL(volatile double, 0x00967888)
#define C_968930 CM_GLOBAL(volatile double, 0x00968930)
#define C_968e80 CM_GLOBAL(volatile double, 0x00968e80)
#define C_968968 CM_GLOBAL(volatile double, 0x00968968)
#define C_969040 CM_GLOBAL(volatile double, 0x00969040)
#define C_968ef0 CM_GLOBAL(volatile double, 0x00968ef0)
#define C_969000 CM_GLOBAL(volatile double, 0x00969000)
#define C_969028 CM_GLOBAL(volatile double, 0x00969028)
#define C_96d650 CM_GLOBAL(volatile double, 0x0096d650)
#define C_96d658 CM_GLOBAL(volatile double, 0x0096d658)

void CM_THISCALL
cm_fn_006eaea0(u8 *self, u32 arg1, int *arg2, s8 arg3,
              volatile u32 arg4, volatile u32 arg5)
{
  typedef u16 home_word __attribute__((may_alias));
  s8 arg5byte = *(volatile s8 *)&arg5; /* BL cached at 0x006eaea8 */
  struct frame18 {
    u8            pad0[3];
    volatile u8   flag;      /* S+3  */
    volatile u32  local4;    /* S+4  */
    volatile double df;      /* S+8  */
    volatile double dp;      /* S+0x10 */
  } L;

  u32 a1home;                /* [esp+0x2c] dword scratch (arg1 home) */
  u32 a2home;                /* [esp+0x30] dword scratch (arg2 home) */
  u32 a3home;                /* [esp+0x34] dword scratch (arg3 home) */
  u32 arg2v;
  u32 otherteam;             /* edi after 0x006eaff4 / 0x006eb01a */
  u32 w2;
  u32 blv;                   /* bl */
  u32 rnd1;
  s32 i, j;
  u32 u;
  long long q;
  long double td;

#define SP8(off)  (*(volatile u8  *)(self + (off)))
#define SP16(off) (*(volatile u16 *)(self + (off)))
#define SP32(off) (*(volatile u32 *)(self + (off)))

  arg2v = (u32)arg2;
  a1home = arg1;
  a2home = arg2v;
  a3home = (u32)(s32)arg3;

  /* 0x006eaeb4 */
  L.local4 = 0;
  L.flag = 0;
  SP32(0x19e) = arg1;
  SP32(0x69) = arg2v;
  SP8(0x00) = *(volatile u8 *)&arg4;
  SP8(0x2e) = (u8)arg5byte;

  /* 0x006eaed3 */
  if (*(volatile u32 *)(arg2v + 0x61) != 0) {
    SP32(0x6d) = *(volatile u32 *)(arg2v + 0x61);
  }
  else {
    u32 team;
    if ((u8)arg3 != 0)
      team = *(volatile u32 *)(*(volatile u32 *)(arg1 + 0x4792) + 0x20);
    else
      team = *(volatile u32 *)(*(volatile u32 *)(arg1 + 0x4792) + 0x1c);
    cm_fn_00944c9f(0x00de1f64u, 0x00a15ff4u,
                    *(volatile u32 *)(arg2v + 0),
                    *(volatile u32 *)(arg2v + 4),
                    *(volatile u32 *)(arg2v + 8),
                   team + 4);
    i = (s32)cm_fn_0090cfc0(DAT_00ae236c);
    /* lea [eax*8]; sub eax; lea [ecx+ecx*4]; lea [eax+edx*2] == 70*eax */
    SP32(0x6d) = DAT_00ae23c4 + 70u * (u32)i;
  }

  /* 0x006eaf46 */
  if (SP32(0x69) == 0 || *(volatile s32 *)(SP32(0x69)) < DAT_00ae2364)
    SP8(0x17) = *(volatile u8 *)(SP32(0x6d) + 0x45);
  else
    SP8(0x17) = 10;

  /* 0x006eaf67 */
  if (arg2v != 0 && *(volatile s32 *)arg2v >= DAT_00ae2364) {
    /* 0x006eafa9 */
  }
  else {
    u32 team;
    if (SP8(0x27) != 0)
      team = *(volatile u32 *)(*(volatile u32 *)(SP32(0x19e) + 0x4792) + 0x20);
    else
      team = *(volatile u32 *)(*(volatile u32 *)(SP32(0x19e) + 0x4792) + 0x1c);
    L.local4 = cm_fn_004cf5f0(PTR_00ae1050, arg2v, team);
  }

  /* 0x006eafa9 */
  SP16(0x47) = *(volatile u16 *)(SP32(0x6d) + 0xb);
  SP16(0x49) = *(volatile u16 *)(SP32(0x6d) + 9);
  SP16(0x4b) = *(volatile u16 *)(SP32(0x6d) + 0xd);

  /* 0x006eafc7 */
  if ((u8)arg3 != 0) {
    SP32(0xf9) = *(volatile u32 *)(*(volatile u32 *)(SP32(0x19e) + 0x4792) + 0x20);
    otherteam   = *(volatile u32 *)(*(volatile u32 *)(SP32(0x19e) + 0x4792) + 0x1c);
  }
  else {
    SP32(0xf9) = *(volatile u32 *)(*(volatile u32 *)(SP32(0x19e) + 0x4792) + 0x1c);
    otherteam   = *(volatile u32 *)(*(volatile u32 *)(SP32(0x19e) + 0x4792) + 0x20);
  }

  /* 0x006eb01d: 0x0062c590, the two float stores, then 0x008b1750.
     fild reads the arg1 home (2 pushes have moved esp), the int is the
     sign-extended low word of the callee result. */
  a1home = (u32)(s32)(s16)(u16)cm_fn_0062c590(PTR_00b63d28, arg2v, 0);
  *(volatile float *)(self + 0x4d) = (float)(s32)a1home;
  *(volatile float *)(self + 0x51) = (float)(s32)a1home;
  cm_fn_008b1750(PTR_00de1f44, arg2v, 0);

  SP32(0x59) = 0;

  /* 0x006eb051 */
  u = (u32)cm_fn_0062c400(PTR_00b63d28, arg2v);
  if ((u8)u == 2) {
    SP8(0x19) = 0xfe;
  }
  else if (arg5byte < 0 || arg5byte > 10) {
    SP8(0x19) = 0xff;
    SP16(0x35) = 0x1900;
    SP16(0x1bc) = 0;
  }
  else {
    SP8(0x19) = (u8)arg5byte;
    if (SP32(0x19e) == 0)
      SP16(0x1bc) = 0;
    else
      SP16(0x1bc) = cm_fn_006c38c0(SP32(0x19e), (u32)self);
  }

  /* 0x006eb0aa */
  SP8(0x1f) = 0xff;
  SP8(0x22) = 0xff;
  SP8(0x20) = 0xff;
  SP8(0x21) = 0xff;

  /* 0x006eb0bd: (0x19 - (s8)[rec+0x5d]) * 2 stored to S+0x10, fild, -> float */
  {
    s32 e = (s32)(s8)*(volatile u8 *)(SP32(0x69) + 0x5d);
    SP16(0x31) = 0;
    *(volatile float *)(self + 0x1a2) = (float)((0x19 - e) * 2);
  }

  if (cm_fn_00547790(SP32(0x69), otherteam) != 0)
    SP16(0x31) = (u16)(SP16(0x31) + 10);
  else if (cm_fn_005477d0(SP32(0x69), otherteam) != 0)
    SP16(0x31) = (u16)(SP16(0x31) + 0xffec);      /* add -0x14 */

  /* 0x006eb109 */
  {
    u32 f9v = SP32(0xf9);
    if (f9v == *(volatile u32 *)(otherteam + 0xb3) ||
        f9v == *(volatile u32 *)(otherteam + 0xb7) ||
        f9v == *(volatile u32 *)(otherteam + 0xbb))
      SP16(0x31) = (u16)(SP16(0x31) + 0xfff6);    /* add bx (-10) */

    /* 0x006eb132 (0x006eb137 stores 10 between the two pushes) */
    SP16(0x33) = 10;
    if (cm_fn_00547790(SP32(0x69), f9v) != 0)
      SP16(0x33) = (u16)(SP16(0x33) + 0x1e);
    else if (cm_fn_005477d0(SP32(0x69), f9v) != 0)
      SP16(0x33) = (u16)(SP16(0x33) + 0xfff6);

    /* 0x006eb16b */
    f9v = SP32(0xf9);
    if (f9v == *(volatile u32 *)(otherteam + 0xb3) ||
        f9v == *(volatile u32 *)(otherteam + 0xb7) ||
        f9v == *(volatile u32 *)(otherteam + 0xbb))
      SP16(0x33) = (u16)(SP16(0x33) + 5);
    i = (s32)f9v;                                  /* ecx kept for 0x006eb1fb */
  }

  /* 0x006eb190 */
  blv = (u8)arg3;
  SP8(0x1d) = 0;
  SP8(0x1e) = 0;
  SP16(0x39) = 0;
  SP8(0x2f) = 0;

  if (*(volatile u8 *)(SP32(0x19e) + 0x8ebb) == 0)
    goto L_b39f;

  w2 = *(volatile u32 *)(SP32(0x19e) + 0x4792);
  if ((u8)arg3 != 0) {
    if (*(volatile u8 *)(w2 + 0x41) == 0)
      goto L_b39f;
  }
  else {
    if (*(volatile u8 *)(w2 + 0x40) == 0)
      goto L_b39f;
  }

  /* 0x006eb1dc (fallthrough) */
  if (arg2v != 0 && *(volatile s32 *)arg2v >= DAT_00ae2364)
    goto L_b39f;
  if (*(volatile u8 *)(w2 + 0x3a) == 1)
    goto L_b39f;

  /* 0x006eb1fb: pushed ecx is this->0xf9 (loaded at 0x006eb16b) */
  if (cm_fn_005409b0((u32 *)(u32)i) != 0)
    goto L_b3fd;

  /* 0x006eb20c */
  if (cm_fn_0053b900((u32 *)SP32(0xf9)) != 0) {
    /* 0x006eb21f */
    SP8(0x30) = (u8)(*(volatile u8 *)(arg2v + 0x22) == 0);
    SP16(0x3d) = 0xfc18;
    SP16(0x3f) = (u16)*(volatile u8 *)(arg2v + 0x23);
    SP16(0x43) = 0xfc18;
    SP16(0x45) = 0xfc18;
    SP16(0x41) = 0xfc18;
    goto L_b41a;
  }

  /* 0x006eb24d */
  {
    u32 person = SP32(0xf9);
    u32 clubrec = *(volatile u32 *)(person + 0x53);
    if (*(volatile u8 *)(clubrec + 0x11c) == 0)
      goto L_b3fd;
    u = *(volatile u32 *)(person + 0x57);
    if (u == 0)
      goto L_b3fd;
    if (*(volatile u32 *)(PTR_00adadfc + *(volatile u32 *)u * 4) == 0)
      goto L_b3fd;
    if (*(volatile u32 *)clubrec == (u32)DAT_009cf36c) {
      if (DAT_00ae2c92 < 0x7d2)
        goto L_b3fd;
    }

    /* 0x006eb29c */
    SP16(0x3f) = 0xfc18;
    i = *(volatile s32 *)(*(volatile u32 *)(SP32(0x19e) + 0x4792));
    if (i > -1 && cm_fn_004b0700((u32)i) != 0) {
      /* 0x006eb2c7: 0x007c47b0 returns in ST0, 0x00945640 truncates it */
      q = cm_ftol_00945640(cm_fn_007c47b0(PTR_00b74350, SP32(0x6d), 2));
      SP16(0x41) = (u16)(u32)q;
      SP16(0x43) = (u16)cm_fn_007c3d20(arg2v);
    }
    else {
      SP16(0x41) = 0xfc18;
      SP16(0x43) = 0xfc18;
    }

    /* 0x006eb2fb */
    SP8(0x30) = (u8)((u16)cm_fn_007c3a40(arg2v, SP32(0xf9)) == 0);
    SP16(0x3d) = (u16)cm_fn_007c3bb0(arg2v, SP32(0xf9));
    SP16(0x45) = 0xfc18;

    /* 0x006eb332 */
    u = *(volatile u32 *)(arg2v + 0x39);
    if (u == 0)
      goto L_b41a;
    if (cm_fn_00547770(u, 0x00ae2c90u) == 0)
      goto L_b41a;
    td = cm_fn_007c1e30(PTR_00b74350, SP32(0x6d), 6, 1, 6.5f);
    if (!(td >= C_968930))         /* fcomp: skip on ST0 < m64 or unordered */
      goto L_b41a;
    td = cm_fn_007c1e30(PTR_00b74350, SP32(0x6d), 6, 2, 6.5f);
    q = cm_ftol_00945640(td);
    SP16(0x45) = (u16)(u32)q;
    goto L_b41a;
  }

L_b39f:
  /* 0x006eb39f: pushed ecx is this->0xf9 (loaded at 0x006eb16b) */
  if (cm_fn_0053b900((u32 *)(u32)i) == 0)
    goto L_b3fd;
  if (cm_fn_005409b0((u32 *)SP32(0xf9)) != 0)
    goto L_b3fd;
  if (*(volatile u8 *)(*(volatile u32 *)(SP32(0x19e) + 0x4792) + 0x40) == 0)
    goto L_b3fd;
  /* 0x006eb3d2 */
  SP8(0x30) = (u8)(*(volatile u8 *)(arg2v + 0x22) == 0);
  SP16(0x3d) = 0xfc18;
  SP16(0x3f) = (u16)*(volatile u8 *)(arg2v + 0x23);
  SP16(0x43) = 0xfc18;
  SP16(0x45) = 0xfc18;
  SP16(0x41) = 0xfc18;
  goto L_b41a;

L_b3fd:
  /* 0x006eb3fd */
  SP8(0x30) = 0;
  SP16(0x3d) = 0xfc18;
  SP16(0x3f) = 0xfc18;
  SP16(0x41) = 0xfc18;
  SP16(0x43) = 0xfc18;
  SP16(0x45) = 0xfc18;

L_b41a:
  /* 0x006eb41a */
  {
    u32 r = SP32(0x6d);
    s32 d30 = (s32)(s8)*(volatile u8 *)(r + 0x30);
    s32 d3b = (s32)(s8)*(volatile u8 *)(r + 0x3b);
    if (d30 > d3b) {
      SP8(0x2c) = 0;
      SP8(0x2d) = (u8)(0x14 - (s32)(s8)*(volatile u8 *)(r + 0x3b));
    }
    else {
      SP8(0x2c) = 1;
      SP8(0x2d) = (u8)(0x14 - (s32)(s8)*(volatile u8 *)(r + 0x30));
    }
  }

  /* 0x006eb445: the three optional record ids, one 0xffffffff cache */
  {
    u32 ones = 0xffffffffu;
    if (*(volatile u32 *)(arg2v + 4) != 0) {
      SP32(0x5d) = *(volatile u32 *)(*(volatile u32 *)(arg2v + 4) + 0x33);
      ones = 0xffffffffu;
    }
    else {
      ones = 0xffffffffu;
      SP32(0x5d) = ones;
    }
    if (*(volatile u32 *)(arg2v + 8) != 0)
      SP32(0x61) = *(volatile u32 *)(*(volatile u32 *)(arg2v + 8) + 0x33);
    else
      SP32(0x61) = ones;
    if (*(volatile u32 *)(arg2v + 0xc) != 0)
      SP32(0x65) = *(volatile u32 *)(*(volatile u32 *)(arg2v + 0xc) + 0x33);
    else
      SP32(0x65) = ones;
  }

  SP8(0x27) = (u8)blv;

  /* 0x006eb484 */
  {
    u32 f9v = SP32(0xf9);
    if (f9v != 0 && *(volatile u32 *)(f9v + 0x53) != 0 &&
        *(volatile s16 *)(f9v + 0x80) > 0x1676) {
      u32 world = SP32(0x19e);
      u32 off = (u32)((s32)(s8)blv * 208);
      if (off + world + 0x9032u == 0) {
        u32 team;
        if ((u8)arg3 != 0)
          team = *(volatile u32 *)(*(volatile u32 *)(world + 0x4792) + 0x20);
        else
          team = *(volatile u32 *)(*(volatile u32 *)(world + 0x4792) + 0x1c);
        if (cm_fn_00600a50(team, 0, 1, 0, 0) == 0 &&
            *(volatile u8 *)(*(volatile u32 *)(SP32(0xf9) + 0x53) + 0x11c) == 0)
          L.flag = 1;
      }
    }
  }

  /* 0x006eb502 */
  SP8(0x118) = (u8)cm_fn_0090cfc0(0x14u);
  {
    u32 r = SP32(0x6d);
    s32 v;
    if (*(volatile u8 *)(r + 0xf) == 0x14)
      v = 0x23 - (s32)(s8)*(volatile u8 *)(r + 0x21);
    else
      v = (0x1e - (s32)(s8)*(volatile u8 *)(r + 0x21)) * 2;
    SP8(0x119) = (u8)cm_fn_0090cfc0((u32)v);
  }

  /* 0x006eb54b */
  if (cm_fn_006d4280(SP32(0x19e)) != 0)
    goto L_b9e3;
  if (arg2v != 0 && *(volatile s32 *)arg2v >= DAT_00ae2364)
    goto L_b9e3;

  /* 0x006eb56c */
  {
    u32 team;
    if (SP8(0x27) != 0)
      team = *(volatile u32 *)(*(volatile u32 *)(SP32(0x19e) + 0x4792) + 0x20);
    else
      team = *(volatile u32 *)(*(volatile u32 *)(SP32(0x19e) + 0x4792) + 0x1c);

    if (cm_fn_0053b900((u32 *)team) != 0) {
      L.df = 5.0;                                  /* 0x4014000000000000 */
    }
    else if (L.local4 != 0) {
      /* 0x006eb5b4: pick a m64 by the high nibble of [record+0x4f], scale the
         signed byte [record+0x44] in x87 extended precision, one m64 spill. */
      u32 l4 = L.local4;
      u32 nib = (u32)(*(volatile u8 *)(l4 + 0x4f) >> 4);
      double k;
      if (nib == 1)
        k = C_968e80;
      else if (nib == 2)
        k = C_968968;
      else if (nib == 3)
        k = C_969040;
      else
        k = C_968930;
      td = ((long double)(s32)(s8)*(volatile u8 *)(l4 + 0x44) * C_968ef0) + C_967890;
      td = td * (long double)k;
      L.df = (double)td;
    }
    else {
      L.df = 1.0;                                  /* 0x3ff0000000000000 */
    }
  }

  /* 0x006eb616: bl for the power base */
  {
    u32 world = SP32(0x19e);
    u32 w = *(volatile u32 *)(world + 0x4792);
    u32 ebx;
    if (*(volatile u8 *)(w + 0x40) != 0) {
      u32 sel;
      s32 t;
      if (SP8(0x27) == 0)
        sel = *(volatile u32 *)(w + 0x20);
      else
        sel = *(volatile u32 *)(*(volatile u32 *)(world + 0x4792) + 0x1c);
      {
        s32 w80 = (s32)(s16)*(volatile u16 *)(sel + 0x80);
        s32 w69 = (s32)(s16)*(volatile u16 *)(*(volatile u32 *)(w + 0x14) + 0x69);
        s32 qq = (s32)(((long long)w80 * 0x10624dd3LL) >> 37);
        t = qq + w69;
        t += (qq < 0) ? 1 : 0;
        t += (t < 0) ? 1 : 0;
        ebx = (u32)(t >> 1);
      }
      blv = (u8)ebx;
    }
    else {
      blv = *(volatile u8 *)(*(volatile u32 *)(w + 0x14) + 0x69);
    }
  }

  /* 0x006eb670: +10 when this side's club is one of the other side's three */
  {
    s32 dl27 = (s32)(s8)SP8(0x27);
    u32 world = SP32(0x19e);
    u32 a, b;
    if (dl27 != 0) {
      a = *(volatile u32 *)(*(volatile u32 *)(world + 0x4792) + 0x20);
      b = *(volatile u32 *)(*(volatile u32 *)(world + 0x4792) + 0x1c);
    }
    else {
      a = *(volatile u32 *)(*(volatile u32 *)(world + 0x4792) + 0x1c);
      b = *(volatile u32 *)(*(volatile u32 *)(world + 0x4792) + 0x20);
    }
    if (*(volatile u32 *)(a + 0xb3) == b ||
        *(volatile u32 *)(a + 0xb7) == b ||
        *(volatile u32 *)(a + 0xbb) == b)
      blv = (u8)(blv + 10);
  }

  /* 0x006eb700 */
  {
    u32 w = *(volatile u32 *)(SP32(0x19e) + 0x4792);
    i = *(volatile s32 *)w;
    if (i > -1 && *(volatile u32 *)(PTR_00adadfc + (u32)i * 4) == 0) {
      *(volatile u32 *)w = (u32)DAT_009cf668;
      *(volatile u32 *)(w + 0x14) = DAT_00ae23d0 +
          107u * (u32)DAT_009cf668;               /* 3,27,108,107 chain */
    }
  }

  /* 0x006eb743 */
  {
    u32 world = SP32(0x19e);
    u32 w = *(volatile u32 *)(world + 0x4792);
    if (*(volatile s32 *)w > -1) {
      u32 sel;
      u32 self_club;
      if (SP8(0x27) != 0)
        sel = *(volatile u32 *)(w + 0x20);
      else
        sel = *(volatile u32 *)(*(volatile u32 *)(world + 0x4792) + 0x1c);
      self_club = *(volatile u32 *)(PTR_00adadfc + *(volatile u32 *)w * 4);
      if (cm_fn_0048f150(self_club, sel) == 0)
        blv = (u8)(blv + 0xf6);                   /* add bl, -10 */
    }
  }

  /* 0x006eb783: power base = ((s8)bl - 10) * m64 + m64 (extended) */
  i = (s32)(s8)blv - 10;
  a1home = (u32)i;
  {
    long double base = (long double)(s32)i * C_967888 + C_967890;
    double exponent = C_969000;                   /* fld qword [0x00969000] */
    L.dp = (double)cm_x87_CIpow(base, exponent); /* 0x006eb7a3 */
  }

  if (SP8(0x27) != 0)
    goto L_b81f;
  if (*(volatile u8 *)(SP32(0x19e) + 0x1ad) != 0)
    goto L_b81f;

  /* 0x006eb7c3 (branch A) */
  {
    u32 world = SP32(0x19e);
    u32 ecx = *(volatile u32 *)(world + 0x91d2);
    u32 obj = *(volatile u32 *)(*(volatile u32 *)(world + 0x4792) + 0x18);
    if (obj != 0) {
      s32 d = *(volatile s32 *)(obj + 0x3c);
      a1home = (u32)d;
      if (d > 0)
        goto L_b7e9;
    }
    a1home = 0x5dc;
L_b7e9:
    a2home = (u32)(s32)(s8)*(volatile u8 *)(arg2v + 0x5b);
    {
      s32 A = (s32)a2home;
      s32 B = (s32)(2000u * ecx);
      long double t;
      t = (long double)B * (long double)A;
      t = t * (long double)A;
      t = t * (long double)L.df;
      t = t * (long double)L.dp;
      t = t / (long double)(s32)a1home;
      q = cm_ftol_00945640(t);                     /* 0x00945640 */
    }
    goto L_b889;
  }

L_b81f:
  /* 0x006eb81f (branch B) */
  {
    u32 world = SP32(0x19e);
    u32 ecx = *(volatile u32 *)(world + 0x91d2);
    u32 obj = *(volatile u32 *)(*(volatile u32 *)(world + 0x4792) + 0x18);
    if (obj != 0) {
      s32 d = *(volatile s32 *)(obj + 0x3c);
      a1home = (u32)d;
      if (d > 0)
        goto L_b84b;
    }
    a1home = 0x5dc;
L_b84b:
    a2home = (u32)(s32)(s8)*(volatile u8 *)(arg2v + 0x5b);
    {
      s32 A = (s32)a2home;
      s32 s = (s32)(s8)*(volatile u8 *)(SP32(0x6d) + 0x2c) + 10;
      s32 B = (s32)((u32)s * ecx * 100u);
      long double t;
      t = (long double)B * (long double)A;
      t = t * (long double)A;
      t = t * (long double)L.df;
      t = t * (long double)L.dp;
      t = t / (long double)(s32)a1home;
      q = cm_ftol_00945640(t);                     /* 0x00945640 */
    }
  }

L_b889:
  /* 0x006eb889: 0x00945640 truncates ST0, EAX is the rand bound */
  rnd1 = cm_fn_0090cfc0((u32)q);
  j = (s32)cm_fn_0090cfc0(2000000u);
  if ((s32)rnd1 > j) {
    SP8(0x100) = (u8)cm_fn_0090cfc0(10u);
    goto L_b8bd;
  }
  j = (s32)cm_fn_0090cfc0(1000000u);
  if ((s32)rnd1 > j) {
    SP8(0x100) = (u8)cm_fn_0090cfc0(5u);
    goto L_b8bd;
  }
  if (SP32(0x19e) != 0 && cm_fn_006c38c0(SP32(0x19e), (u32)self) == 1) {
    SP8(0x100) = 0;
    goto L_b8bd;
  }
  j = (s32)cm_fn_0090cfc0(100000u);
  if ((s32)rnd1 < j) {
    SP8(0x100) = (u8)cm_fn_0090cfc0(0xfffffffbu);
    goto L_b8bd;
  }
  j = (s32)cm_fn_0090cfc0(10000u);
  if ((s32)rnd1 > j)
    SP8(0x100) = (u8)cm_fn_0090cfc0(0xfffffff6u);

L_b8bd:
  /* 0x006eb8bd */
  td = L.dp;
  if (td > C_967890)
    td = td * C_96d658 + C_96d650;
  goto L_b8de;

L_b9e3:
  /* 0x006eb9e3 */
  td = C_967890;
  SP8(0x100) = 0;
  goto L_b8de;

L_b8de:
  /* 0x006eb8de: ebp is reloaded from the arg2 home here */
  {
    u32 r = SP32(0x6d);
    long double t;
    blv = (u8)arg3;
    i = (s32)(s8)*(volatile u8 *)(r + 0x2c) - 10;
    a3home = (u32)i;
    t = (long double)i * td;
    q = cm_ftol_00945640(t);                       /* 0x00945640 */
    SP8(0x100) = (u8)(SP8(0x100) + (u8)cm_fn_0090cfc0((u32)q));
    i = (s32)(s8)SP8(0x100);
    a3home = (u32)(s32)(s16)*(volatile u16 *)(r + 5);
    t = (long double)(s32)a3home * C_969028;
    t = C_967890 - t;                              /* fsubr */
    a3home = (u32)i;
    t = t * (long double)i;
    q = cm_ftol_00945640(t);                       /* 0x00945640 */
    SP8(0x100) = (u8)(u32)q;
  }

  /* 0x006eb945 */
  if (*(volatile u8 *)(*(volatile u32 *)(SP32(0x19e) + 0x4792) + 0x40) != 0) {
    u32 t1;
    if (SP8(0x27) != 0)
      t1 = *(volatile u32 *)(*(volatile u32 *)(SP32(0x19e) + 0x4792) + 0x1c);
    else
      t1 = *(volatile u32 *)(*(volatile u32 *)(SP32(0x19e) + 0x4792) + 0x20);
    if (cm_fn_00547840(*(volatile u32 *)(t1 + 0xcf), SP32(0x69)) != 0)
      SP8(0x100) = (u8)(SP8(0x100) + 10);
  }

  /* 0x006eba20 */
  {
    u32 team;
    if (SP8(0x27) != 0)
      team = *(volatile u32 *)(*(volatile u32 *)(SP32(0x19e) + 0x4792) + 0x20);
    else
      team = *(volatile u32 *)(*(volatile u32 *)(SP32(0x19e) + 0x4792) + 0x1c);
    if (cm_fn_00600a50(team, 0, 1, 0, 0) == 0) {
      u32 world = SP32(0x19e);
      u32 off = (u32)((s32)(s8)SP8(0x27) * 208);
      u32 p = *(volatile u32 *)(off + world + 0x904du);
      if (p != 0) {
        u32 pp = *(volatile u32 *)(p + 0x69);
        if (pp != 0) {
          u32 adj = (pp & 0xffffff00u) | (u32)*(volatile u8 *)(pp + 0x1b);
          q = cm_fn_00544410(pp, adj);
          i = (s32)(s8)(u8)(u32)q - 10;
          u = cm_fn_0090cfc0((u32)i);
          goto L_baae;
        }
      }
    }
    u = cm_fn_0090cfc0(L.flag ? 8u : 6u);
  }
L_baae:
  SP8(0x100) = (u8)(SP8(0x100) + (u8)u);

  /* 0x006ebac4 */
  {
    u32 f9v = SP32(0xf9);
    u32 local4;
    if (f9v != 0 && *(volatile u32 *)(f9v + 0x57) != 0) {
      local4 = L.local4;
      if (local4 != 0 &&
          *(volatile u32 *)(*(volatile u32 *)(SP32(0x19e) + 0x4792) + 0x14)
            == *(volatile u32 *)(f9v + 0x57) &&
          *(volatile s8 *)(local4 + 0x3d) > 0x19) {
        u32 target = *(volatile u32 *)(f9v + 0x57);
        u32 arr = *(volatile u32 *)(PTR_00adadfc + *(volatile u32 *)target * 4);
        if (arr != 0 && *(volatile u32 *)(arr + 8) != 0) {
          i = (s32)cm_fn_004a4850(*(volatile u32 *)(arr + 8), f9v);
          if (i != 0) {
            /* 0x006ebb28 */
            s32 f19 = *(volatile s32 *)(i + 0x19);
            if ((f19 > 5 || *(volatile s32 *)(i + 0x41) > 9) &&
                (s8)*(volatile u8 *)(arg2v + 0x5b) <
                    (s8)(u8)(f19 + 10)) {
              SP8(0x100) = (u8)(SP8(0x100) - 1);
            }
            else if ((f19 > 3 || *(volatile s32 *)(i + 0x41) > 7) &&
                     (s8)*(volatile u8 *)(arg2v + 0x5b) <
                         (s8)(u8)(*(volatile u8 *)(i + 0x19) + 6)) {
              SP8(0x100) = (u8)(SP8(0x100) - 1);
            }
            /* 0x006ebb5f */
            if (*(volatile s32 *)(i + 0x19) > 3 ||
                *(volatile s32 *)(i + 0x41) > 5) {
              u32 r1 = cm_fn_0090cfc0((u32)(s32)(s8)*(volatile u8 *)(arg2v + 0x5b));
              if (r1 == 0) {
                SP8(0x100) = (u8)(SP8(0x100) - 1);
              }
              else {
                u32 r2 = cm_fn_0090cfc0((u32)(s32)(s8)*(volatile u8 *)(arg2v + 0x5a));
                if (r2 == 0)
                  SP8(0x100) = (u8)(SP8(0x100) - 1);
              }
            }
          }
          /* 0x006ebb93 */
          {
            u32 f9b = SP32(0xf9);
            u32 cf = *(volatile u32 *)(f9b + 0xcf);
            if (cf != 0) {
              u8 r = cm_fn_007a7eb0((s32 *)(u32)cf);
              if (r != 0) {
                u32 v1 = cm_fn_0090cfc0((u32)(s32)(s8)*(volatile u8 *)(arg2v + 0x5b));
                if (v1 == 0)
                  SP8(0x100) = (u8)(SP8(0x100) - 1);
                else {
                  u32 v2 = cm_fn_0090cfc0((u32)(s32)(s8)*(volatile u8 *)(arg2v + 0x59));
                  if (v2 == 0)
                    SP8(0x100) = (u8)(SP8(0x100) - 1);
                }
              }
            }
          }
        }
      }
    }
    else {
      local4 = L.local4;
    }

    /* 0x006ebbe1 */
    if (cm_fn_00600a50(SP32(0xf9), 0, 1, 0, 0) != 0 &&
        SP32(0x19e) != 0 &&
        cm_fn_006c38c0(SP32(0x19e), (u32)self) == 1) {
      SP8(0x100) = (u8)(SP8(0x100) + 1);
    }

    /* 0x006ebc18 */
    {
      u32 f9v2 = SP32(0xf9);
      if (f9v2 != 0 && local4 != 0 && PTR_00adadfc != 0 &&
          *(volatile u32 *)(f9v2 + 0x57) != 0 &&
          *(volatile u32 *)(PTR_00adadfc +
              *(volatile u32 *)(*(volatile u32 *)(f9v2 + 0x57)) * 4) != 0) {
        u32 a = cm_fn_00600a50(f9v2, 0, 1, 0, 0);
        if (a != 0 || (s8)SP8(0x100) < 5) {
          if (*(volatile u32 *)(local4 + 0x45) == 0 &&
              *(volatile s8 *)(local4 + 0x44) > 0x19 &&
              cm_fn_00544520(arg2v, 0) == 0 &&
              *(volatile u8 *)(PTR_00ae2d14 + 5 +
                  *(volatile u32 *)SP32(0xf9) * 6) == 0 &&
              (s8)*(volatile u8 *)(arg2v + 0x5b) > 0x0e &&
              cm_fn_0090cfc0((u32)(s32)(s8)*(volatile u8 *)(arg2v + 0x59)) != 0) {
            SP8(0x100) = (u8)(SP8(0x100) + 1);
          }
        }
      }
    }
  }

  /* 0x006ebcb6 */
  {
    u32 r = SP32(0x6d);
    s32 v;
    i = (s32)(s8)SP8(0x100);
    v = i / 2 + (s32)(s8)*(volatile u8 *)(r + 0x25) + 10;
    SP8(0xff) = (u8)(v / 4);
  }

  /* 0x006ebce0 */
  {
    u32 world = SP32(0x19e);
    u32 k;
    if (SP8(0x27) == 0)
      k = *(volatile u8 *)(world + 0x8d4f);
    else
      k = *(volatile u8 *)(world + 0x8d50);
    cm_fn_0053b640(SP32(0xf9), (u8)k, (u32)&arg5, (u32)&arg4);
    SP16(0x1ba) = *(volatile home_word *)&arg4;
  }

  if (blv != 0)
    SP16(0x19a) = 0xb4;
  else
    SP16(0x19a) = 0;

  SP8(0x2b) = 0;
  SP8(0x102) = 0;
  SP8(0x103) = 0;
  SP16(0x198) = 0x65;

#undef SP8
#undef SP16
#undef SP32
}

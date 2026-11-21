/*
 * fn_008db580 - plain C reconstruction of the original routine.
 *
 * Original: CM0102 3.9.68 (CM0102/Game/cm0102.exe), preferred-image VA
 * 0x008db580, image base 0x00400000, SHA-256
 * 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
 *
 * The original ends in `ret 0xc`: it pops three stack words, so the entry is
 * stdcall with three arguments (the object, the mode flag and the object used
 * as `this` for the 0x004ae660 / 0x004ae8a0 calls).
 */
#include "cmre.h"

/* ------------------------------------------------------------------ */
/* original callees (navigation only, never authority)                */
/* ------------------------------------------------------------------ */
extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);
extern unsigned long long CM_THISCALL cm_fn_0043f460(const int *self, u32 flags);
extern u32 CM_CDECL cm_fn_00441c10(u32 output, u32 size, u32 low, u32 high, u32 mode);
extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 arg1);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32 self, const char *name, u32 key_a, u32 key_b);
extern u32 CM_CDECL cm_fn_0053e140(u32 first, u32 second, u32 third, u32 output, u32 size, u32 kind, u32 person);
extern u32 CM_CDECL cm_fn_0053e990(u32 person, u32 first, u32 second, u32 third, u32 date);
/* legacy callee: the original pushes eight arguments on the zero branch and
   fifteen on the other one; every argument is caller-cleaned (cdecl). */
extern char * CM_CDECL cm_fn_0066f4e0(void *a1, ...);

/* ------------------------------------------------------------------ */
/* original globals                                                    */
/* ------------------------------------------------------------------ */
#define LAB_009644fb CM_GLOBAL(u8, 0x009644fb)
#define DAT_00a97290 CM_GLOBAL(u8, 0x00a97290)
#define DAT_00ae23b4 CM_GLOBAL(volatile u32, 0x00ae23b4)
#define DAT_00ae23bc CM_GLOBAL(volatile u32, 0x00ae23bc)
#define DAT_00de1f64 CM_GLOBAL(u32, 0x00de1f64)
#define STR_00a9721c (&CM_GLOBAL(char, 0x00a9721c))

/*
 * The SEH record the original pushes: state (-1), handler (0x009644fb) and
 * the previous chain head; `mov dword ptr fs:[0], esp` links fs:[0] to the
 * prev slot, so the record reads { prev, handler, state } in memory.
 */
typedef struct cm_seh_frame_s {
	void *prev;
	u32 handler;
	u32 state;
} cm_seh_frame;

/*
 * One scratch allocation holding the locals of the original frame, at the
 * original offsets from L (the address right after `sub esp, 0x1e0`):
 *   L+0x18  the dword passed as `this` to cm_fn_0043f460
 *   L+0x1c  0x32 bytes written by cm_fn_00441c10
 *   L+0x50  0x190 bytes written by cm_fn_0053e140
 * Bytes the original never initialises are left uninitialised here.
 */
typedef struct cm_scratch_s {
	u8 pad_000[0x18];
	u32 this_slot;
	u8 buf1[0x32];
	u8 pad_04e[2];
	u8 buf2[0x190];
} cm_scratch;

void CM_STDCALL cm_fn_008db580(u32 param_1, int param_2, u32 param_3)
{
	volatile cm_seh_frame seh;
	cm_scratch scratch;
	u32 v0, v1, v2, v3, v5;
	s8 b4;

	/* 0x008db580-0x008db58e: SEH prologue. */
	seh.state = 0xffffffffu;
	seh.handler = (u32)&LAB_009644fb;
	seh.prev = CM_FS(void *, 0);
	CM_FS(void *, 0) = (void *)&seh.prev;

	/* 0x008db5a6-0x008db5f0: the first five index lookups. */
	v0 = (u32)cm_fn_00785830((void *)param_1, 0);
	v1 = (u32)cm_fn_00785830((void *)param_1, 1);
	v2 = (u32)cm_fn_00785830((void *)param_1, 2);
	v3 = (u32)cm_fn_00785830((void *)param_1, 3);
	b4 = (s8)(u8)cm_fn_00785830((void *)param_1, 4);
	v5 = (u32)cm_fn_00785830((void *)param_1, 5);

	if (param_2 != 0) {
		/* 0x008db601-0x008db628: three more index lookups; the index 7
		   result is copied into the slot that becomes `this`. */
		u32 v6 = (u32)cm_fn_00785830((void *)param_1, 6);
		u32 v7 = (u32)cm_fn_00785830((void *)param_1, 7);
		u32 v9 = (u32)cm_fn_00785830((void *)param_1, 9);
		u32 base;
		u32 t9, t5, t6;
		s8 b5, c9, d6;
		u32 person;

		scratch.this_slot = v7;

		/* 0x008db634: the SEH install level is cleared. */
		seh.state = 0;

		/* 0x008db63f-0x008db64d: the pair of calls whose arguments are
		   the 64-bit result of cm_fn_0043f460. */
		{
			unsigned long long q = cm_fn_0043f460((const int *)&scratch.this_slot, 6);
			cm_fn_00441c10((u32)scratch.buf1, 0x32, (u32)q, (u32)(q >> 32), 6);
		}

		/* 0x008db66e: the record is produced with kind 1. */
		cm_fn_0053e140(v1, v2, v3, (u32)scratch.buf2, 0x190, 1, v0);

		/* 0x008db673-0x008db6ab: the three indices are scaled by 581. */
		base = (u32)DAT_00ae23b4;
		t9 = v9 * 581u;
		t5 = v5 * 581u;
		t6 = v6 * 581u;

		/* 0x008db699, 0x008db6b0, 0x008db6c4: the three signed bytes. */
		b5 = *(s8 *)(u32)(t5 + base + 0x52u);
		c9 = *(s8 *)(u32)(t9 + base + 0x52u);
		d6 = *(s8 *)(u32)(base + t6 + 0x52u);

		/* 0x008db6ee: the fifteen argument call. */
		cm_fn_0066f4e0((void *)&DAT_00de1f64,
			       (void *)&DAT_00a97290,
			       (u32)b5, (u32)b5,
			       (u32)d6, (u32)d6,
			       100u,
			       (u32)c9, (u32)c9,
			       t5 + base + 0x38u,
			       (void *)scratch.buf1,
			       (u32)b4,
			       (void *)scratch.buf2,
			       base + t6 + 0x38u,
			       t9 + base + 0x38u);

		/* 0x008db6f3-0x008db753: the object call and the three keyed
		   registrations. */
		cm_fn_004ae660((void *)param_3, (u32)&DAT_00de1f64);
		base = DAT_00ae23b4;
		cm_fn_004ae8a0(param_3, (const char *)(t5 + base + 0x38u), 0x7d5u, t5 + base);
		base = DAT_00ae23b4;
		cm_fn_004ae8a0(param_3, (const char *)(t6 + base + 0x38u), 0x7d5u, t6 + base);
		base = DAT_00ae23b4;
		cm_fn_004ae8a0(param_3, (const char *)(t9 + base + 0x38u), 0x7d5u, t9 + base);

		/* 0x008db758-0x008db871: the lookup and its registration. */
		person = v0 * 110u + (u32)DAT_00ae23bc;
		if (cm_fn_0053e990(person, v1, v2, v3, 0) != 0) {
			person = v0 * 110u + DAT_00ae23bc;
			cm_fn_004ae8a0(param_3, (const char *)scratch.buf2, 0x7d7u, person);
		}
	} else {
		u32 base;
		u32 t5;
		s8 b5;
		u32 person;

		/* 0x008db7bf: the record is produced with kind 5. */
		cm_fn_0053e140(v1, v2, v3, (u32)scratch.buf2, 0x190, 5, v0);

		/* 0x008db7c4-0x008db7d9: the index 5 result is scaled by 581. */
		base = (u32)DAT_00ae23b4;
		t5 = v5 * 581u;

		/* 0x008db7e3: the signed byte. */
		b5 = *(s8 *)(u32)(t5 + base + 0x52u);

		/* 0x008db7fb: the eight argument call. */
		cm_fn_0066f4e0((void *)&DAT_00de1f64,
			       (void *)STR_00a9721c,
			       (u32)b5, (u32)b5,
			       100u,
			       t5 + base + 0x38u,
			       (u32)b4,
			       (void *)scratch.buf2);

		/* 0x008db800-0x008db82a: the object call and one registration. */
		cm_fn_004ae660((void *)param_3, (u32)&DAT_00de1f64);
		base = DAT_00ae23b4;
		cm_fn_004ae8a0(param_3, (const char *)(t5 + base + 0x38u), 0x7d5u, t5 + base);

		/* 0x008db82f-0x008db871: the lookup and its registration. */
		person = v0 * 110u + (u32)DAT_00ae23bc;
		if (cm_fn_0053e990(person, v1, v2, v3, 0) != 0) {
			person = v0 * 110u + DAT_00ae23bc;
			cm_fn_004ae8a0(param_3, (const char *)scratch.buf2, 0x7d7u, person);
		}
	}

	/* 0x008db876-0x008db88e: the SEH chain is restored and the three
	   arguments are popped by the `ret 0xc`. */
	CM_FS(void *, 0) = seh.prev;
}

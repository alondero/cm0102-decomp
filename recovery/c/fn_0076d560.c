/*
 * fn_0076d560 - CM0102 3.9.68 primary executable.
 *   preferred-image VA 0x0076d560 (542 bytes), image base 0x00400000
 *   pinned binary SHA-256
 *     0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86
 *
 * Entry: `ret 8` at 0x0076d77b, so the caller pushes two DWORD arguments
 * (stdcall).  EAX is the result (0x0076d76c `mov eax, ebp`).
 *
 * ABI findings, all read from the original instructions (the callee
 * declarations below are navigation only, they are not authority):
 *
 *  - 0x0076d560..0x0076d56e build the SEH record {prev,handler,state}
 *    {+0,+4,+8} and link it with fs:[0]; 0x0076d771 unlinks it again from
 *    the value saved in the record (0x0076d767 reads S+0x28).
 *    The state word is at S+0x30 and is written by a dword store at
 *    0x0076d589 (ebp == 0) and 0x0076d75a (0xffffffff) and by byte stores
 *    at 0x0076d59b (1), 0x0076d5b3 (0), 0x0076d669 (2), 0x0076d67e (4) and
 *    0x0076d6b3 (5).  The byte stores only change the low byte of state.
 *
 *  - Locals (S = the base right after `sub esp,0x28` at 0x0076d575):
 *      S+0x00  dword: allocator result (0x0076d595), later the date object
 *              built by 0x00549ef0 (0x0076d657 `lea ecx,[esp+0x18]`, esp=S-24)
 *      S+0x08  first date output (0x0076d660 `lea ecx,[esp+0x18]`, esp=S-16)
 *              and receiver of the second 0x0054c5b0 (0x0076d67a)
 *      S+0x10  second date output (0x0076d673 `lea edx,[esp+0x20]`, esp=S-16)
 *      S+0x18  container object (0x0076d57c `lea ecx,[esp+0x28]`)
 *      S+0x28  SEH prev, S+0x2c handler, S+0x30 state
 *
 *  - The nine query words of 0x005acbe0 are pushed right to left, so the
 *    chronologically last push (0x0076d6b2) is argument 1 and the first push
 *    (0x0076d68e) is argument 9.  The physical order is therefore
 *    p1=[S+0x3c], p2=0xfffffffe, p3=0xffffffff, p4=S+0x18, p5=[ebx+0x00],
 *    p6=[S+0x12], p7=[S+0x10], p8=[S+0x0a], p9=[S+0x08] (p1..p9 are pushed
 *    in that order, 0x0076d68e..0x0076d6b2).
 *    ESP=S-0x10 before the pushes; at 0x0076d695 it is S-0x18,
 *    at 0x0076d6a1 S-0x24, and at 0x0076d6a6 S-0x28.
 *    Every pushed query word is a full DWORD: 0x0076d693 pushes -2,
 *    0x0076d68e pushes the DWORD loaded at 0x0076d688, and
 *    0x0076d6a5/0x0076d6aa push the DWORD loads at 0x0076d68f/0x0076d69c.
 *
 *  - 0x0068d0d0 is called with the fastcall pair ECX=container, EDX=edx_in.
 *    That EDX is the register value left by the preceding original call
 *    (0x005409b0 at 0x0076d615, or 0x0053b4f0 at 0x0076d5dc/0x0076d627) or
 *    an address loaded at 0x0076d5ff/0x0076d623.  It is observed, never
 *    invented: the two calls are declared with a 64-bit result so the
 *    EDX:EAX pair the original leaves behind is read, not guessed.
 *    The EDX:EAX pair of 0x0068d0d0 is then carried into the date call
 *    0x00549ef0; the partial writes at 0x0076d64d (`mov dx`) and 0x0076d651
 *    (`mov ax`) keep the upper halves of the transported registers.
 *
 *  - The caller owned home of the second argument (S+0x3c) is written with a
 *    dword store (0x0076d5e8/0x0076d62f) and then with a byte store
 *    (0x0076d5f9/0x0076d60f/0x0076d649) before it is read back as a dword
 *    at 0x0076d688; the home is kept as a byte addressable volatile value
 *    so the store widths stay as in the original. The scalar formal itself
 *    is the home, not a local copy; 0x0076d5e4/0x0076d633 expose its address.
 */

#include "cmre.h"

/* ---- original callees (hybrid boundary: bound to the original code) ---- */

/* EDX:EAX pairs are physically transported by the original code, so the
 * results of 0x005409b0 / 0x0053b4f0 are read as a 64 bit EDX:EAX pair. */
extern unsigned long long CM_CDECL cm_fn_0053b4f0(u32 nation);
extern unsigned long long CM_CDECL cm_fn_005409b0(u32 *input);
extern unsigned long long CM_THISCALL cm_fn_00549ef0(void *self, u32 first, u32 second);
extern void CM_THISCALL cm_fn_0054c5b0(short *param_1, u32 *param_2, int param_3);
extern u32 CM_THISCALL cm_fn_005acbe0(u32 self, volatile u32 first_day, volatile u32 first_year,
                                      volatile u32 last_day, volatile u32 last_year,
                                      volatile u32 club_arg, volatile u32 output_arg,
                                      volatile u32 key_arg, volatile u32 filter_arg,
                                      volatile u32 side_arg);
extern u32 CM_THISCALL cm_fn_0068d020(u32 self);
extern u32 CM_THISCALL cm_fn_0068d040(u32 self);
extern unsigned long long CM_FASTCALL cm_fn_0068d0d0(volatile u8 *self, u32 edx_in,
                                                      const u8 *src, s16 count);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32 self);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);

/* ---- original globals ---- */
#define DAT_009cf78c CM_GLOBAL(volatile int, 0x009cf78c)
#define DAT_009cf790 CM_GLOBAL(volatile int, 0x009cf790)
#define DAT_009cf79c CM_GLOBAL(volatile int, 0x009cf79c)
#define DAT_009cf7a0 CM_GLOBAL(volatile int, 0x009cf7a0)
#define DAT_009cf7a4 CM_GLOBAL(volatile int, 0x009cf7a4)
#define DAT_009cf7a8 CM_GLOBAL(volatile int, 0x009cf7a8)
#define DAT_009cf7ac CM_GLOBAL(volatile int, 0x009cf7ac)
#define DAT_009cf8a0 CM_GLOBAL(volatile int, 0x009cf8a0)
#define DAT_009cf964 CM_GLOBAL(volatile int, 0x009cf964)
#define DAT_00ae2a58 CM_GLOBAL(volatile u32, 0x00ae2a58)

#define CM_PTRU(p) ((u32)(unsigned long)(p))

#define LD32(a) (*(volatile u32 *)(a))
#define LD16(a) (*(volatile u16 *)(a))
#define LD8(a)  (*(volatile u8 *)(a))

/* The original frame: locals S+0x00..S+0x27 followed by the SEH record at
 * S+0x28..S+0x33.  One allocation keeps the original adjacency. */
typedef struct {
	u32 pad[2];                            /* +0x00: keeps S 8 mod 16 as in the original */
	u32 w[10];                             /* +0x08: S+0x00 .. S+0x27 */
	volatile struct {                      /* +0x30: S+0x28 */
		void *prev;                        /* S+0x28 */
		void *handler;                     /* S+0x2c */
		union { u32 w; u8 b[4]; } state;   /* S+0x30 */
	} seh;
} cm_frame_t;

/* 0x0076d77b: `ret 8` - the callee pops the two argument dwords, so the
 * entry convention is __stdcall. */
u32 CM_STDCALL cm_fn_0076d560(u32 *param_1, volatile u32 param_2)
{
	cm_frame_t fr __attribute__((aligned(16)));
	u8 *S = (u8 *)fr.w;                        /* S+0x00 */
	u32 ebx_v;                                 /* first argument */
	u32 esi_v;                                 /* second argument */
	u32 ebp_v;                                 /* the container of the allocated block */
	u32 alloc_ptr;
	u32 eax_v, edx_v;
	u32 copy_lo, copy_hi;
	u32 edx_in;
	const u8 *src;
	u8 flag_byte;
	u32 nation, v_1c, v_1c2, ecx_v, eax_cmp;
	int same;
	unsigned long long pair;
	u32 node;

	/* ---- prologue: SEH record (0x0076d560..0x0076d56e) ---- */
	fr.seh.state.w = 0xffffffffu;              /* push -1 */
	fr.seh.handler = (void *)0x0095ddbbu;      /* push 0x95ddbb */
	fr.seh.prev = CM_FS(void *, 0);            /* push dword ptr fs:[0] */
	CM_FS(void *, 0) = (void *)&fr.seh;        /* mov dword ptr fs:[0], esp */

	/* 0x0076d580: construct the stack container at S+0x18 */
	(void)cm_fn_0068d020(CM_PTRU(S + 0x18));

	/* 0x0076d585: xor ebp, ebp */
	ebp_v = 0;

	/* 0x0076d587/0x0076d589: push 0xe; mov dword ptr [S+0x30], ebp */
	fr.seh.state.w = 0u;

	/* 0x0076d58d: allocate 0xe bytes */
	pair = cm_fn_00944cf1(0xe);
	alloc_ptr = (u32)pair;

	/* 0x0076d595: mov dword ptr [S+0x00], eax */
	*(volatile u32 *)(void *)(S + 0x00) = alloc_ptr;

	/* 0x0076d599: cmp eax, ebp */
	/* 0x0076d59b: mov byte ptr [S+0x30], 1 */
	fr.seh.state.b[0] = 1;
	if (alloc_ptr != 0) {
		/* 0x0076d5a2/0x0076d5a4/0x0076d5a9 */
		ebp_v = cm_fn_0068d020(alloc_ptr);
	}

	/* 0x0076d5ab/0x0076d5af: capture incoming arguments after construction. */
	esi_v = param_2;
	ebx_v = (u32)(unsigned long)param_1;
	/* 0x0076d5b3: mov byte ptr [S+0x30], 0 */
	fr.seh.state.b[0] = 0;

	/* 0x0076d5b9..0x0076d5c2: the compared operands, read before the call */
	v_1c = LD32(esi_v + 0x1c);                 /* mov eax, dword ptr [esi+0x1c] */
	ecx_v = LD32(v_1c + 0x53);                 /* mov ecx, dword ptr [eax+0x53] */
	eax_cmp = LD32(ebx_v + 0x53);              /* mov eax, dword ptr [ebx+0x53] */
	same = (ecx_v == eax_cmp);                 /* 0x0076d5c5: cmp ecx, eax */

	/* 0x0076d5c9 (equal) / 0x0076d615 (unequal): the same single call */
	pair = cm_fn_005409b0((u32 *)(unsigned long)ebx_v);
	/* EDX left by 0x005409b0 is only used on the unequal/zero path, but it
	 * has to be captured at the call, so the pair is taken here. */
	u32 edx_after_5409 = (u32)(pair >> 32);

	if (same) {
		if ((u32)pair == 0) {
			/* 0x0076d5ff: lea edx, [esi+0x20] -> the address is the EDX input */
			edx_v = (u32)(unsigned long)(esi_v + 0x20);
			src = (const u8 *)(unsigned long)(esi_v + 0x20);
			edx_in = edx_v;
		} else {
			/* 0x0076d5d5..0x0076d5dc */
			edx_v = LD32(esi_v + 0x20);
			nation = LD32(edx_v + 0x53);
			pair = cm_fn_0053b4f0(nation);
			eax_v = (u32)pair;
			edx_in = (u32)(pair >> 32);       /* EDX left by 0x0053b4f0 */
			/* 0x0076d5e4/0x0076d5e8: the dword store into the home */
			param_2 = eax_v;
			src = (const u8 *)&param_2;
		}
		/* 0x0076d5f1: call 0x0068d0d0 */
		pair = cm_fn_0068d0d0((volatile u8 *)(unsigned long)ebp_v, edx_in, src, 4);
		copy_lo = (u32)pair;
		copy_hi = (u32)(pair >> 32);
		/* 0x0076d5f6: mov al, byte ptr [esi+0x40] */
		flag_byte = LD8(esi_v + 0x40);
		eax_v = (copy_lo & 0xffffff00u) | (u32)flag_byte;
		/* 0x0076d5f9: mov byte ptr [S+0x3c], al */
		*(volatile u8 *)&param_2 = flag_byte;
	} else {
		if ((u32)pair != 0) {
			/* 0x0076d621..0x0076d627 */
			v_1c2 = LD32(esi_v + 0x1c);        /* mov ecx, dword ptr [edi] */
			edx_v = LD32(v_1c2 + 0x53);
			pair = cm_fn_0053b4f0(edx_v);
			eax_v = (u32)pair;
			edx_in = (u32)(pair >> 32);       /* EDX left by 0x0053b4f0 */
			/* 0x0076d62f: the dword store into the home */
			param_2 = eax_v;
			src = (const u8 *)&param_2;
		} else {
			/* 0x0076d63c/0x0076d63e: the source is &esi[0x1c] */
			src = (const u8 *)(unsigned long)(esi_v + 0x1c);
			edx_in = edx_after_5409;          /* EDX left by 0x005409b0 */
		}
		/* 0x0076d641: call 0x0068d0d0 */
		pair = cm_fn_0068d0d0((volatile u8 *)(unsigned long)ebp_v, edx_in, src, 4);
		copy_lo = (u32)pair;
		copy_hi = (u32)(pair >> 32);
		/* 0x0076d646: mov cl, byte ptr [esi+0x40] */
		flag_byte = LD8(esi_v + 0x40);
		eax_v = (copy_lo & 0xffffff00u) | (u32)flag_byte;
		/* 0x0076d649: mov byte ptr [S+0x3c], cl */
		*(volatile u8 *)&param_2 = flag_byte;
	}

	/* 0x0076d64d: mov dx, word ptr [esi+0x28] */
	edx_v = (copy_hi & 0xffff0000u) | (u32)LD16(esi_v + 0x28);
	/* 0x0076d651: mov ax, word ptr [esi+0x2a] */
	eax_v = (eax_v & 0xffff0000u) | (u32)LD16(esi_v + 0x2a);

	/* 0x0076d657/0x0076d65b: the date object at S+0x00 is built here */
	pair = cm_fn_00549ef0((void *)(S + 0x00), eax_v, edx_v);

	/* 0x0076d660..0x0076d66e */
	fr.seh.state.b[0] = 2;
	cm_fn_0054c5b0((short *)(u32)pair, (u32 *)(void *)(S + 0x08), 1);

	/* 0x0076d673..0x0076d683 */
	fr.seh.state.b[0] = 4;
	cm_fn_0054c5b0((short *)(void *)(S + 0x08), (u32 *)(void *)(S + 0x10), 13);

	/* 0x0076d688..0x0076d6a6: the nine query words, read in the original
	 * order, then pushed right to left by the call. */
	{
		u32 q_p1 = param_2;                            /* 0x0076d688 */
		u32 q_p5 = LD32(ebx_v + 0x00);                 /* 0x0076d68c */
		u32 q_p6 = *(volatile u32 *)(void *)(S + 0x12); /* 0x0076d68f */
		u32 q_p4 = CM_PTRU(S + 0x18);                  /* 0x0076d695 (lea) */
		u32 q_p7 = *(volatile u32 *)(void *)(S + 0x10); /* 0x0076d69c */
		u32 q_p8 = *(volatile u32 *)(void *)(S + 0x0a); /* 0x0076d6a1 */
		u32 q_p9 = *(volatile u32 *)(void *)(S + 0x08); /* 0x0076d6a6 */
		u32 query_receiver = DAT_00ae2a58;              /* 0x0076d6ab */

		/* 0x0076d6b3: mov byte ptr [S+0x30], 5 */
		fr.seh.state.b[0] = 5;

		/* 0x0076d6b8: the arguments are pushed right to left, so p9 is the
		 * first argument and p1 the ninth. */
		(void)cm_fn_005acbe0(query_receiver,
		                     q_p9,      /* first_day  */
		                     q_p8,      /* first_year */
		                     q_p7,      /* last_day   */
		                     q_p6,      /* last_year  */
		                     q_p5,      /* club_arg   */
		                     q_p4,      /* output_arg */
		                     0xffffffffu, /* key_arg   */
		                     0xfffffffeu, /* filter_arg */
		                     q_p1);     /* side_arg   */
	}

	/* 0x0076d6bd/0x0076d6c1: the four bytes at S+0x1a are copied to S+0x22 */
	*(volatile u32 *)(void *)(S + 0x22) = *(volatile u32 *)(void *)(S + 0x1a);

	/* 0x0076d6c5..0x0076d756: the container is walked */
	node = cm_fn_0068d1b0(CM_PTRU(S + 0x18));
	while (node != 0) {
		u32 p14 = LD32(node + 0x14);
		int skip = 0;

		if (p14 != 0) {
			u32 v = LD32(p14);
			if (v == (u32)DAT_009cf79c) {
				skip = 1;
			} else if (v == (u32)DAT_009cf7a0) {
				skip = 1;
			} else if (v == (u32)DAT_009cf7a4) {
				skip = 1;
			} else if (v == (u32)DAT_009cf7a8) {
				skip = 1;
			} else if (v == (u32)DAT_009cf7ac) {
				skip = 1;
			} else if (v == (u32)DAT_009cf790) {
				skip = 1;
			} else if (v == (u32)DAT_009cf8a0) {
				skip = 1;
			} else if (v == (u32)DAT_009cf78c) {
				skip = 1;
			} else if (v == (u32)DAT_009cf964) {
				skip = 1;
			}
		}

		if (!skip) {
			/* 0x0076d729..0x0076d744 */
			u32 n1c = LD32(node + 0x1c);              /* mov edx, [eax+0x1c] */
			u32 ebx53 = LD32(ebx_v + 0x53);           /* mov esi, [ebx+0x53] */
			u32 edx3 = LD32(n1c + 0x53);              /* mov edx, [edx+0x53] */
			const u8 *node_src;

			if (edx3 == ebx53) {
				node_src = (const u8 *)(unsigned long)(node + 0x20);
			} else {
				node_src = (const u8 *)(unsigned long)(node + 0x1c);
			}
			(void)cm_fn_0068d0d0((volatile u8 *)(unsigned long)ebp_v, edx3,
			                     node_src, 4);
		}

		/* 0x0076d749..0x0076d754 */
		node = cm_fn_0068d1b0(CM_PTRU(S + 0x18));
	}

	/* 0x0076d756/0x0076d75a/0x0076d762: destroy the stack container */
	fr.seh.state.w = 0xffffffffu;
	(void)cm_fn_0068d040(CM_PTRU(S + 0x18));

	/* 0x0076d767/0x0076d771: unlink the SEH record again */
	CM_FS(void *, 0) = fr.seh.prev;

	/* 0x0076d76c: mov eax, ebp */
	return ebp_v;
}

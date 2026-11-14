/* fn_0051e870 - plain C reconstruction.
 *
 * Preferred-image VA 0x0051e870 (image base 0x00400000), 840 bytes.
 * Evidence: "ret 8" pops two stack argument slots; ECX is read before it is
 * written, so ECX is the receiver and the two stack slots are the arguments.
 *
 * Stack layout at entry (ESP0 = entry ESP):
 *   [ESP0+0] return address
 *   [ESP0+4] argument 1, read as a WORD  (mov di, word ptr [esp+0x1c])
 *   [ESP0+8] argument 2, read as a DWORD (mov eax, dword ptr [esp+8])
 * The routine also keeps scratch words in its own frame (sub esp,8 area) and
 * overwrites the caller's argument homes; the comment at each instruction
 * site names the original store.
 *
 * Every store of the swap blocks below re-reads the row pointer exactly where
 * the listing reloads it (mov eax,[esi+0xad] / mov eax,[eax+edi*4]); the row
 * indirection is part of the address, not the rows array base.
 */
#include "cmre.h"

/* call dword ptr [eax + 0x90] with ecx = receiver and two stack arguments. */
typedef u32 (CM_THISCALL *cm_slot36_fn)(void *self, u32 arg1, u32 arg2);

#define CM_OBJ_A3   0xa3u   /* table of 0x68-byte entries            */
#define CM_OBJ_A7   0xa7u   /* table of 6-byte entries                */
#define CM_OBJ_AD   0xadu   /* array of record-row pointers            */
#define CM_OBJ_24   0x24u   /* owner object                           */
#define CM_REC_SZ   53      /* 0x35: size of one record                */
#define CM_ROW_SZ   104     /* 0x68: size of one 0xa3 table entry      */
#define CM_NO_IDX   0xffffu

/* mov edx, dword ptr [esi + 0xad] ; mov eax, dword ptr [edx + edi*4] */
static u32 cm_row_ptr(u32 selfp, s32 idx)
{
  return *(volatile u32 *)(*(volatile u32 *)(selfp + CM_OBJ_AD) + (u32)(idx * 4));
}

void CM_THISCALL cm_fn_0051e870(void *self, s16 arg1, u32 arg2)
{
  u32 selfp = (u32)self;
  u32 ax;          /* EAX: low half compared against DX                 */
  u32 dx;          /* EDX: low half compared against AX                 */
  u16 flags;       /* CX: entry flags at [0xa3 table + 0xb]             */
  s32 start;       /* sign extended AX: first record index              */
  s32 count;       /* sign extended DX - sign extended AX               */
  s32 off;         /* byte offset of the current record (53 * index)    */

  /* 0051e870 mov eax, dword ptr [esp+8] ; 0051e877 cmp ax, 0xffff        */
  if ((u16)arg2 == CM_NO_IDX) {
    /* 0051e88d movsx ecx, di ; xor eax, eax ;
     * 0051e898 mov edx, [esi+0xa3] ; mov dx, word ptr [edx+ecx*8+0x1a]   */
    ax = 0u;
    dx = *(volatile u16 *)(*(volatile u32 *)(selfp + CM_OBJ_A3) +
                           (u32)((s32)arg1 * CM_ROW_SZ) + 0x1au);
  } else {
    /* 0051e888 lea edx, [eax + 1] */
    ax = (u32)(u16)arg2;
    dx = (u32)(u16)(arg2 + 1u);
  }

  /* 0051e8a3 movsx edi, di ; 0051e8a6 mov ebx, [esi+0xa3] ;
   * 0051e8b2 mov cx, word ptr [ebx + ecx*8 + 0xb]                        */
  flags = *(volatile u16 *)(*(volatile u32 *)(selfp + CM_OBJ_A3) +
                            (u32)((s32)arg1 * CM_ROW_SZ) + 0xbu);

  if (flags & 1u) {
    /* 0051e8c0 cmp ax, dx ; 0051e8c9 movsx eax, ax ; movsx ecx, dx      */
    start = (s16)(u16)ax;
    count = (s16)(u16)dx - start;
    if (count > 0) {
      /* 0051e8cf .. 0051e8d7: ebx = 53 * start                          */
      off = CM_REC_SZ * start;
      do {
        /* 0051e8df mov edx, [esi+0xad] ; mov eax, [edx + edi*4] ;
         * 0051e8e8 add eax, ebx                                            */
        u32 p = cm_row_ptr(selfp, (s32)arg1) + (u32)off;
        /* 0051e8ea mov cx, word ptr [eax] ; cmp cx, -1 */
        u16 f0 = *(volatile u16 *)p;
        if (f0 != CM_NO_IDX) {
          /* 0051e8f7 mov bp, word ptr [eax+2] ; cmp bp, -1 */
          u16 f1 = *(volatile u16 *)(p + 2u);
          if (f1 != CM_NO_IDX) {
            /* 0051e905 mov eax, [esi+0xa7] ;
             * 0051e911 mov edx, dword ptr [eax + ecx*2]  (6 * f0) ;
             * 0051e91a mov ebp, dword ptr [eax + ecx*2]  (6 * f1)         */
            u32 base2 = *(volatile u32 *)(selfp + CM_OBJ_A7);
            u32 a1 = *(volatile u32 *)(base2 + (u32)((s32)(s16)f0 * 6));
            u32 a2 = *(volatile u32 *)(base2 + (u32)((s32)(s16)f1 * 6));
            /* 0051e91d mov eax, [esi] ; 0051e91f push ebp ; push edx ;
             * 0051e921 mov ecx, esi ; 0051e923 call dword ptr [eax+0x90]  */
            u32 vt = *(volatile u32 *)selfp;
            u32 r = ((cm_slot36_fn)*(u32 *)(vt + 0x90u))(self, a1, a2);
            /* 0051e929 test eax, eax ; 0051e92d cmp eax, ebp */
            if (r != 0u && r == a2) {
              /* 0051e931 .. 0051e953: reload the row and its fields        */
              u32 q = cm_row_ptr(selfp, (s32)arg1) + (u32)off;
              u16 g0 = *(volatile u16 *)q;          /* cx = [row]           */
              u16 g5 = *(volatile u16 *)(q + 0xau); /* bp = [row+0xa]       */
              u16 g1 = *(volatile u16 *)(q + 2u);   /* bp = [row+2]         */
              u16 g7 = *(volatile u16 *)(q + 0xeu); /* dx = [row+0xe]       */
              /* 0051e953 mov word ptr [eax], bp   : f0 <- f1               */
              *(volatile u16 *)q = g1;
              /* 0051e961 mov bp, [row+0x10] ; 0051e965 [row+0xe] <- bp   */
              q = cm_row_ptr(selfp, (s32)arg1) + (u32)off;
              *(volatile u16 *)(q + 0xeu) = *(volatile u16 *)(q + 0x10u);
              /* 0051e974 mov bp, [row+0xc] ; 0051e978 [row+0xa] <- bp    */
              q = cm_row_ptr(selfp, (s32)arg1) + (u32)off;
              *(volatile u16 *)(q + 0xau) = *(volatile u16 *)(q + 0xcu);
              /* 0051e985 mov word ptr [row+2], cx : f1 <- old f0           */
              q = cm_row_ptr(selfp, (s32)arg1) + (u32)off;
              *(volatile u16 *)(q + 2u) = g0;
              /* 0051e993 mov word ptr [row+0x10], dx : f8 <- old f7       */
              q = cm_row_ptr(selfp, (s32)arg1) + (u32)off;
              *(volatile u16 *)(q + 0x10u) = g7;
              /* 0051e9a6 mov word ptr [row+0xc], ax : f6 <- old f5        */
              q = cm_row_ptr(selfp, (s32)arg1) + (u32)off;
              *(volatile u16 *)(q + 0xcu) = g5;
            }
          }
        }
        /* 0051e9af add ebx, 0x35 */
        off += CM_REC_SZ;
        count--;
      } while (count != 0);
    }
  } else if (flags & 8u) {
    /* 0051e9d0 cmp ax, dx ; 0051e9d9 movsx ecx, ax ; movsx edx, dx      */
    start = (s16)(u16)ax;
    count = (s16)(u16)dx - start;
    if (count > 0) {
      /* 0051e9df .. 0051e9e7: eax = 53 * start                          */
      off = CM_REC_SZ * start;
      do {
        /* 0051e9ef mov ecx, [esi+0xad] ; mov edx, [ecx + edi*4] ;
         * 0051e9f8 mov bx, word ptr [edx + eax]                           */
        u32 p = cm_row_ptr(selfp, (s32)arg1) + (u32)off;
        u16 f0 = *(volatile u16 *)p;
        if (f0 != CM_NO_IDX) {
          /* 0051ea0d mov dx, word ptr [ecx+2] ; cmp dx, -1 */
          u16 f1 = *(volatile u16 *)(p + 2u);
          if (f1 != CM_NO_IDX) {
            /* 0051ea1e mov ebp, [esi+0xa7] ;
             * 0051ea35 mov dl, byte ptr [ebp + edx*2 + 4]  (6 * f0 + 4) ;
             * 0051ea39 cmp dl, byte ptr [ebp + ebx*2 + 4]  (6 * f1 + 4)    */
            u32 base2 = *(volatile u32 *)(selfp + CM_OBJ_A7);
            s8 k0 = *(volatile s8 *)(base2 + (u32)((s32)(s16)f0 * 6) + 4u);
            s8 k1 = *(volatile s8 *)(base2 + (u32)((s32)(s16)f1 * 6) + 4u);
            if (k0 < k1) {
              /* 0051ea3f .. 0051ea4b                                    */
              u32 q = cm_row_ptr(selfp, (s32)arg1) + (u32)off;
              u16 g1 = *(volatile u16 *)(q + 2u);   /* dx = [row+2]         */
              u16 g7 = *(volatile u16 *)(q + 0xeu); /* bx = [row+0xe]       */
              u16 g5 = *(volatile u16 *)(q + 0xau); /* bp = [row+0xa]       */
              /* 0051ea4b mov word ptr [ecx], dx : f0 <- f1                */
              *(volatile u16 *)q = g1;
              /* 0051ea5a mov dx, [row+0x10] ; 0051ea5f [row+0xe] <- dx  */
              q = cm_row_ptr(selfp, (s32)arg1) + (u32)off;
              *(volatile u16 *)(q + 0xeu) = *(volatile u16 *)(q + 0x10u);
              /* 0051ea6f mov dx, [row+0xc] ; 0051ea74 [row+0xa] <- dx    */
              q = cm_row_ptr(selfp, (s32)arg1) + (u32)off;
              *(volatile u16 *)(q + 0xau) = *(volatile u16 *)(q + 0xcu);
              /* 0051ea81 mov cx, word ptr [esp+0x10] ; [row+2] <- cx      */
              q = cm_row_ptr(selfp, (s32)arg1) + (u32)off;
              *(volatile u16 *)(q + 2u) = f0;
              /* 0051ea94 mov word ptr [row+0x10], bx : f8 <- old f7       */
              q = cm_row_ptr(selfp, (s32)arg1) + (u32)off;
              *(volatile u16 *)(q + 0x10u) = g7;
              /* 0051ea9f mov cx, word ptr [esp+0x10] ;
               * 0051eaa2 mov word ptr [row+0xc], bp : f6 <- old f5        */
              q = cm_row_ptr(selfp, (s32)arg1) + (u32)off;
              *(volatile u16 *)(q + 0xcu) = g5;
            }
          }
        }
        /* 0051eaab add eax, 0x35 */
        off += CM_REC_SZ;
        count--;
      } while (count != 0);
    }
  } else {
    /* 0051eac3 cmp dword ptr [esi + 0x24], -1 ; je exit                    */
    if (*(volatile u32 *)(selfp + CM_OBJ_24) != 0xffffffffu) {
      /* 0051ead0 cmp ax, dx ; 0051ead6 movsx ecx, ax ; movsx edx, dx    */
      start = (s16)(u16)ax;
      count = (s16)(u16)dx - start;
      if (count > 0) {
        /* 0051eadc .. 0051eae4: eax = 53 * start                        */
        off = CM_REC_SZ * start;
        do {
          /* 0051eaec mov edx, [esi+0xad] ; 0051eaf2 mov ecx, [edx+edi*4] ;
           * 0051eaf5 mov dx, word ptr [ecx + eax]                         */
          u32 p = cm_row_ptr(selfp, (s32)arg1) + (u32)off;
          u16 f0 = *(volatile u16 *)p;
          if (f0 != CM_NO_IDX) {
            /* 0051eb09 mov bp, word ptr [ecx+2] ; cmp bp, -1 */
            u16 f1 = *(volatile u16 *)(p + 2u);
            if (f1 != CM_NO_IDX) {
              /* 0051eb17 mov ebx, [esi+0xa7] ;
               * 0051eb23 lea edx, [ebx + edx*2] ; 0051eb26 mov edx, [edx] */
              u32 base2 = *(volatile u32 *)(selfp + CM_OBJ_A7);
              u32 v = *(volatile u32 *)(base2 + (u32)((s32)(s16)f1 * 6));
              /* 0051eb28 test edx, edx ; je skip */
              if (v != 0u) {
                /* 0051eb2c mov edx, dword ptr [edx + 0x69] ; test edx, edx */
                u32 w = *(volatile u32 *)(v + 0x69u);
                if (w != 0u) {
                  /* 0051eb33 mov edx, dword ptr [edx] ;
                   * 0051eb35 mov ebx, dword ptr [esi+0x24] ; cmp edx, ebx */
                  u32 owner = *(volatile u32 *)w;
                  u32 target = *(volatile u32 *)(selfp + CM_OBJ_24);
                  if (owner == target) {
                    /* 0051eb3c .. 0051eb44                                */
                    u32 q = cm_row_ptr(selfp, (s32)arg1) + (u32)off;
                    u16 g7 = *(volatile u16 *)(q + 0xeu); /* dx = [row+0xe] */
                    u16 g5 = *(volatile u16 *)(q + 0xau); /* bx = [row+0xa] */
                    /* 0051eb44 mov word ptr [ecx], bp : f0 <- f1          */
                    *(volatile u16 *)q = f1;
                    /* 0051eb52 mov bp, [row+0x10] ; 0051eb56 [row+0xe]<-bp*/
                    q = cm_row_ptr(selfp, (s32)arg1) + (u32)off;
                    *(volatile u16 *)(q + 0xeu) = *(volatile u16 *)(q + 0x10u);
                    /* 0051eb65 mov bp, [row+0xc] ; 0051eb69 [row+0xa]<-bp */
                    q = cm_row_ptr(selfp, (s32)arg1) + (u32)off;
                    *(volatile u16 *)(q + 0xau) = *(volatile u16 *)(q + 0xcu);
                    /* 0051eb73 mov bp, word ptr [esp+0x20] ;
                     * 0051eb7b mov word ptr [row+2], bp : f1 <- old f0     */
                    q = cm_row_ptr(selfp, (s32)arg1) + (u32)off;
                    *(volatile u16 *)(q + 2u) = f0;
                    /* 0051eb89 mov word ptr [row+0x10], dx : f8 <- old f7  */
                    q = cm_row_ptr(selfp, (s32)arg1) + (u32)off;
                    *(volatile u16 *)(q + 0x10u) = g7;
                    /* 0051eb97 mov word ptr [row+0xc], bx : f6 <- old f5   */
                    q = cm_row_ptr(selfp, (s32)arg1) + (u32)off;
                    *(volatile u16 *)(q + 0xcu) = g5;
                  }
                }
              }
            }
          }
          /* 0051eba0 add eax, 0x35 */
          off += CM_REC_SZ;
          count--;
        } while (count != 0);
      }
    }
  }
}

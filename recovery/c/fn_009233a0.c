/* fn_009233a0 - preferred-image VA 0x009233a0, image base 0x00400000.
 *
 * Two stack arguments, callee cleanup ("ret 8"):
 *   arg1 (u32)    destination buffer
 *   arg2 (char *) points at three bytes: wind code, precipitation code, number
 *
 * Original frame (L = frame base right after "sub esp, 0x5dc"):
 *   L + 0x000  precipitation text buffer (500 bytes)
 *   L + 0x1f4  wind text buffer          (500 bytes)
 *   L + 0x3e8  third text buffer         (500 bytes)
 * The three buffers live in one scratch array so their relative offsets match
 * the original frame exactly; they are left uninitialised, as in the original.
 *
 * Direction-flag obligation (why this function was held):
 *   The original copies the C string at 0x00ad9c64 with "repne scasb" plus
 *   "rep movsd" plus "rep movsb".  The REP forms take their direction from
 *   the incoming DF.  This function never clears or sets DF, so the copy
 *   direction was decided by its caller.  This C boundary uses the x86 ABI
 *   entry condition DF = 0, also used by the current differential fixtures.
 *   That condition is not proof that every original caller clears DF.
 *   cm_copy_cstring below is the forward form and it writes the same
 *   bytes as the original sequence: strlen(src) + 1 bytes (the terminator
 *   included), whole dwords first and the remaining bytes afterwards.
 *   DF = 1 entry behavior is not represented by this plain-C boundary and
 *   remains an explicit obligation for callers outside that ABI condition;
 *   no caller-wide or native direction-flag equivalence is claimed here.
 */
#include "cmre.h"

/* Original globals (preferred-image VAs). */
#define DAT_00ad4e94 CM_GLOBAL(u8, 0x00ad4e94)
#define DAT_00ad4ed8 CM_GLOBAL(u8, 0x00ad4ed8)
#define DAT_00ad4f38 CM_GLOBAL(u8, 0x00ad4f38)
#define DAT_00ad9c64 CM_GLOBAL(u8, 0x00ad9c64)
#define PTR_LAB_00746556_1_00ad4f1c CM_GLOBAL(void *, 0x00ad4f1c)
#define PTR_LAB_00ad4f20 CM_GLOBAL(void *, 0x00ad4f20)
#define s_Breezy_00ad4f54 ((char *)0x00ad4f54)
#define s_Downpour_00ad4f08 ((char *)0x00ad4f08)
#define s_Drizzle_00ad4f14 ((char *)0x00ad4f14)
#define s_Gusty_00ad4f4c ((char *)0x00ad4f4c)
#define s_Heavy_Snow_00ad4ee8 ((char *)0x00ad4ee8)
#define s_Invalid_WeatherSeasonPrecipitati_00ad4eb0 ((char *)0x00ad4eb0)
#define s_Invalid_Wind___d_00ad4f24 ((char *)0x00ad4f24)
#define s_Light_Snow_00ad4ef4 ((char *)0x00ad4ef4)
#define s_Shower_00ad4f00 ((char *)0x00ad4f00)
#define s_Sleet_00ad4ee0 ((char *)0x00ad4ee0)
#define s_Strong_Wind_00ad4f40 ((char *)0x00ad4f40)
#define s__s___s_0098f6ac ((char *)0x0098f6ac)
#define s__s___s___s_00ad4e88 ((char *)0x00ad4e88)

/* 0x0066f4e0 is called with two arguments at most sites and with three at
   0x0092355e ("add esp, 0xc" after the call), so it is a printf-style helper
   with a variable argument list; the legacy two-parameter map cannot express
   that call site. */
extern char * CM_CDECL cm_fn_0066f4e0(int param_1, char *param_2, ...);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);

/* "repne scasb" to measure, "rep movsd" for the whole dwords, "rep movsb" for
   the remaining bytes; see the direction-flag note in the header comment. */
static void cm_copy_cstring(char *dst, const char *src)
{
  u32 len = 1;                     /* bytes copied, terminator included */
  const char *s = src;
  u32 dwords;
  u32 i;

  while (*s != '\0') {             /* "repne scasb" */
    len++;
    s++;
  }

  dwords = len >> 2;               /* "shr ecx, 2" */
  for (i = 0; i < dwords; i++) {
    __builtin_memcpy(dst + 4 * i, src + 4 * i, 4);   /* "rep movsd" */
  }
  for (i = dwords * 4; i < len; i++) {
    dst[i] = src[i];               /* "rep movsb" */
  }
}

/* "ret 8" pops the two stack arguments, so the callee cleans up. */
void CM_STDCALL cm_fn_009233a0(u32 param_1, char *param_2)
{
  char scratch[0x5dc];
  char *const precip = scratch;              /* original L + 0x000 */
  char *const wind = scratch + 0x1f4;        /* original L + 0x1f4 */
  char *const third = scratch + 0x3e8;       /* original L + 0x3e8 */
  u32 wind_code;
  u32 precip_code;

  wind_code = (u32)(s32)(s8)param_2[0];      /* "movsx eax, byte ptr [ebx]" */

  /* "cmp eax, 4; ja" - unsigned compare of the sign extended byte. */
  if (wind_code > 4u) {
    /* 0x00923436: the default arm writes the "invalid wind" text into the
       third buffer, not into the wind buffer (0x3f8 is measured from the
       pushed argument, so the destination is L + 0x3e8). */
    cm_fn_00944c9f((u32)third, (u32)s_Invalid_Wind___d_00ad4f24, wind_code);
  } else {
    /* jump table at 0x009235ec: 0 -> 0x009233bf, 1 -> 0x009233ea,
       2 -> 0x009233f1, 3 -> 0x00923408, 4 -> 0x0092341f. */
    switch (wind_code) {
    case 0:
      cm_copy_cstring(wind, (const char *)&DAT_00ad9c64);
      break;
    case 1:
      cm_fn_0066f4e0((int)wind, s_Breezy_00ad4f54);
      break;
    case 2:
      cm_fn_0066f4e0((int)wind, s_Gusty_00ad4f4c);
      break;
    case 3:
      cm_fn_0066f4e0((int)wind, s_Strong_Wind_00ad4f40);
      break;
    case 4:
      cm_fn_0066f4e0((int)wind, (char *)&DAT_00ad4f38);
      break;
    default:
      break;
    }
  }

  precip_code = (u32)(s32)(s8)param_2[1];   /* "movsx eax, byte ptr [ebx + 1]" */

  /* "cmp eax, 9; ja" - unsigned compare of the sign extended byte. */
  if (precip_code > 9u) {
    /* 0x00923539: the default arm writes into the precipitation buffer
       (L + 0x0c measured after the pushed argument). */
    cm_fn_00944c9f((u32)precip, (u32)s_Invalid_WeatherSeasonPrecipitati_00ad4eb0,
                   precip_code);
  } else {
    /* jump table at 0x00923600: 0 -> 0x00923460, 1 -> 0x009234ad,
       2 -> 0x009234c4, 3 -> 0x009234df, 4 -> 0x00923539, 5 -> 0x009234d8,
       6 -> 0x00923511, 7 -> 0x00923525, 8 -> 0x009234f3, 9 -> 0x00923507. */
    switch (precip_code) {
    case 0:
      if (wind[0] != '\0') {                /* "mov al, byte ptr [esp + 0x200]" */
        cm_copy_cstring(precip, (const char *)&DAT_00ad9c64);
      } else {
        cm_fn_0066f4e0((int)precip, (char *)&PTR_LAB_00ad4f20);
      }
      break;
    case 1:
      cm_fn_0066f4e0((int)precip, (char *)&PTR_LAB_00746556_1_00ad4f1c);
      break;
    case 2:
      cm_fn_0066f4e0((int)precip, s_Drizzle_00ad4f14);
      break;
    case 3:
      cm_fn_0066f4e0((int)precip, s_Shower_00ad4f00);
      break;
    case 4:
      /* table entry 4 points at the same arm as the out-of-range default. */
      cm_fn_00944c9f((u32)precip, (u32)s_Invalid_WeatherSeasonPrecipitati_00ad4eb0,
                     precip_code);
      break;
    case 5:
      cm_fn_0066f4e0((int)precip, s_Downpour_00ad4f08);
      break;
    case 6:
      cm_fn_0066f4e0((int)precip, s_Sleet_00ad4ee0);
      break;
    case 7:
      cm_fn_0066f4e0((int)precip, (char *)&DAT_00ad4ed8);
      break;
    case 8:
      cm_fn_0066f4e0((int)precip, s_Light_Snow_00ad4ef4);
      break;
    case 9:
      cm_fn_0066f4e0((int)precip, s_Heavy_Snow_00ad4ee8);
      break;
    default:
      break;
    }
  }

  /* 0x0092354c: the number is formatted into the third buffer. */
  cm_fn_0066f4e0((int)third, (char *)&DAT_00ad4e94,
                 (int)(u32)(s32)(s8)param_2[2]);

  if (wind[0] == '\0') {                     /* "mov al, byte ptr [esp + 0x20c]" */
    /* 0x009235c0: precipitation and third text. */
    cm_fn_00944c9f(param_1, (u32)s__s___s_0098f6ac, (u32)precip, (u32)third);
    return;
  }
  if (precip[0] == '\0') {                   /* "mov al, byte ptr [esp]" */
    /* 0x009235af: wind and third text. */
    cm_fn_00944c9f(param_1, (u32)s__s___s_0098f6ac, (u32)wind, (u32)third);
    return;
  }
  /* 0x0092357c: wind, precipitation and third text. */
  cm_fn_00944c9f(param_1, (u32)s__s___s___s_00ad4e88, (u32)wind, (u32)precip,
                 (u32)third);
}

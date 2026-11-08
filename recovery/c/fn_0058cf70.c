/* CM0102 3.9.68, preferred-image VA0058cf70, base00400000.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   ECX receiver, RET4; direct caller76b7f6 overwrites EAX before reuse.
   Original helpers remain explicit boundaries. No SEH frame in this body.
   The x87 operations use extended intermediates with the original single
   precision spills. The two __ftol calls at58d40c/58d425 narrow EAX to a WORD. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
#define W(a) CM_GLOBAL(volatile s16, (a))
#define F(a) CM_GLOBAL(volatile float, (a))
#define Q(a) CM_GLOBAL(volatile double, (a))
extern u8 CM_THISCALL cm_fn_0058e190(u32 self, u32 record);
extern void CM_THISCALL cm_fn_0058dea0(u32 self, u32 record, u16 value, u32 owner);
extern u8 CM_CDECL cm_fn_0048dd70(u32 object, u32 zero);
extern u32 CM_STDCALL cm_fn_0076d940(u32 object);
extern u32 CM_THISCALL cm_fn_005c6710(u32 self, u32 object);
extern u32 CM_CDECL cm_fn_00944cff(u32 file, u32 zero1, u32 zero2, u32 first, u32 second);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 text, u32 zero);

__attribute__((target("no-sse3")))
void CM_THISCALL cm_fn_0058cf70(u32 self, u32 owner)
{
    u8 scratch[0x200];
    volatile union { double value; u32 words[2]; } multiplier =
        { .words = { 0, 0x3ff00000u } };
    u32 home = D(owner + 0x1cu), away = D(owner + 0x20u);
    u32 record = D(home + 0x53u), common, other, competition, id;
    s8 rank_home, rank_away, difference, home_score, away_score;
    u8 score_selector;
    long double weight, away_base, home_value;
    volatile float home_base = 0.0f, away_value = 0.0f, rounded_weight;
    s16 home_level, away_level;
    if (!record) return;
    common = D(record + 0x71u);
    if (!common) return;
    other = D(away + 0x53u);
    if (!other) return;
    other = D(other + 0x71u);
    if (!other) return;
    competition = D(owner + 0x14u);
    if (!competition) return;
    if (common == other) {
        /* Native two DWORD reads at58cfec/58cff2, rather than an unaligned
           compiler-selected wider load. */
        multiplier.words[0] = D(common + 0xbeu);
        multiplier.words[1] = D(common + 0xc2u);
    }
    rank_home = (s8)cm_fn_0058e190(self, record);
    record = D(away + 0x53u);
    rank_away = (s8)cm_fn_0058e190(self, record);
    if ((s32)rank_home - 1 > (s32)rank_away) difference = 2;
    else if ((s32)rank_away - 1 > (s32)rank_home) difference = -2;
    else if (rank_away < rank_home) difference = 1;
    else difference = rank_away <= rank_home ? 0 : -1;
    score_selector = B(owner + 0x47u);
    home_score = score_selector == 255 ? (s8)B(owner + 0x43u) : (s8)score_selector;
    away_score = (s8)B(owner + (score_selector == 255 ? 0x44u : 0x48u));
    competition = D(owner + 0x14u);
    other = D(0x009cf79c);
    id = D(competition);
    if (id == other) weight = Q(0x00968940);
    else {
        if (!cm_fn_0048dd70(competition, 0)) weight = Q(0x00968928);
        else {
            competition = D(owner + 0x14u);
            other = D(competition + 0x59u);
            if (other) {
                (void)D(0x00b74318u); /* original 0058d0a4 */
                other = cm_fn_0076d940(competition);
                if (other) {
                    competition = D(owner + 0x14u);
                    other = D(0x009cf79c);
                    weight = Q(0x00968928);
                    id = D(competition);
                    if (id != other && id != D(0x009cf7a4)) weight = Q(0x009689c0);
                    goto weighted;
                }
            }
            competition = D(owner + 0x14u);
            if (cm_fn_0048dd70(competition, 0) == 1) weight = Q(0x009689c0);
            else {
                competition = D(owner + 0x14u);
                other = D(competition + 0x59u);
                if (!other && ((void)D(0x00b74318u), /* original 0058d12c */
                               cm_fn_0076d940(competition))) weight = Q(0x009689c0);
                else {
                    competition = D(owner + 0x14u);
                    other = D(0x00ae2c74);
                    if (cm_fn_005c6710(other, competition)) weight = Q(0x00967890);
                    else {
                        u32 text;
                        cm_fn_00944cff(0x009c74f4,0,0,(u32)scratch+0x100u,(u32)scratch);
                        text = cm_fn_0090d130();
                        cm_fn_00944c9f(0x00ae24d0,0x009870e8,text,(u32)scratch+0x100u,(u32)scratch,0xde);
                        cm_fn_005e8290(0x009870e0,0x00ae24d0,0);
                        D(0x00b67a34)=0;
                        return;
                    }
                }
            }
        }
    }
weighted:
    /* Preserve the native DWORD dispatch-table read and its ordered fault. */
    other = (u32)((s32)difference + 2);
    if (other > 4u) away_base = away_value;
    else switch (D(0x0058d444u + other*4u)) {
    case 0x0058d0f5:
        if (home_score > away_score) { away_base=F(0x00968e70); home_base=1.0f; }
        else if (home_score == away_score) away_base=F(0x009689a8);
        else { away_base=F(0x00969584); home_base=-1.0f; }
        break;
    case 0x0058d1e7:
        if (home_score > away_score) { away_base=F(0x0096a340); home_base=0x1.e66666p0f; }
        else if (home_score == away_score) { away_base=F(0x0096a33c); home_base=0x1.ccccccp-1f; }
        else { away_base=F(0x0096a338); home_base=-0x1.99999ap-4f; }
        break;
    case 0x0058d223:
        if (home_score > away_score) { home_base=2.0f; away_base=away_value; }
        else if (home_score == away_score) { away_base=F(0x00968e70); home_base=1.0f; }
        else away_base=F(0x009689a8);
        break;
    case 0x0058d239:
        if (home_score < away_score) { away_base=F(0x00968e70); home_base=1.0f; }
        else if (home_score == away_score) { home_base=2.0f; away_base=away_value; }
        else { away_base=F(0x0096a334); home_base=3.0f; }
        break;
    case 0x0058d25f:
        if (home_score < away_score) { away_base=F(0x0096a330); home_base=0x1.99999ap-4f; }
        else if (home_score == away_score) { away_base=F(0x0096a32c); home_base=0x1.19999ap0f; }
        else { away_base=F(0x0096a328); home_base=0x1.0cccccp1f; }
        break;
    default: __builtin_unreachable();
    }
    if (home_score > 5) home_score=5;
    if (away_score > 5) away_score=5;
    other = D(owner + 0x18u);
    {
        long double value = (s32)home_score;
        value *= Q(0x00968918);
        value *= home_base;
        value *= Q(0x00968e80);
        value += home_base;
        home_base = (float)value;
        value = (s32)away_score;
        value *= Q(0x00968918);
        value *= away_base;
        value *= Q(0x00968e80);
        value += away_base;
        away_value = (float)value;
    }
    if (other) {
        other = D(other + 0x38u);
        if (other) {
            other = D(other + 0x1fu);
            record = D(home + 0x53u);
            if (other == record) away_value=(float)((long double)away_value+F(0x0096a324));
        }
    }
    weight *= multiplier.value;
    rounded_weight = (float)weight;
    home_value = weight * home_base;
    away_value = (float)((long double)rounded_weight * away_value);
    if (!(home_value >= F(0x00968970))) home_value=F(0x00968970);
    if (!(away_value >= F(0x00968970))) away_value=0.0f;
    record = D(home + 0x53u);
    home_level = W(record + 0x8eu);
    if (home_level < 3500 && home_value > F(0x00969584)) home_value=F(0x00969584);
    other = D(away + 0x53u);
    away_level = W(other + 0x8eu);
    if (away_level < 3500) {
        if (away_value > F(0x00969584)) away_value=3.0f;
        if (away_level < 3500 && !(home_value > Q(0x00968e18))) home_value=Q(0x00968e18);
    }
    if (home_level < 3500) {
        if (!(away_value > Q(0x00968e18))) away_value=(float)Q(0x00968e18);
        else away_value=away_value;
    }
    home_value *= Q(0x00968908);
    cm_fn_0058dea0(self, record, (u16)(long long)home_value, owner);
    {
        long double value=away_value;
        value *= Q(0x00968908);
        volatile u32 converted = (u32)(long long)value;
        other = D(away + 0x53u);
        cm_fn_0058dea0(self, other, (u16)converted, owner);
    }
}


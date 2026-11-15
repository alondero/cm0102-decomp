/*
 * Authored, asset-free i386 debugger fixture. No CRT or original-game code.
 * The declarations below are fixture plumbing, NOT recovered API semantics.
 * Build/observation contract: tests/fixtures/native_debuggee.md.
 */
typedef unsigned int U32;
typedef void *HANDLE;
typedef U32 (__attribute__((stdcall)) *THREAD)(void *);
#define API __attribute__((stdcall, dllimport))
API HANDLE GetStdHandle(U32);
API int ReadFile(HANDLE, void *, U32, U32 *, void *);
API int WriteFile(HANDLE, const void *, U32, U32 *, void *);
API void ExitProcess(U32) __attribute__((noreturn));
API void ExitThread(U32) __attribute__((noreturn));
API HANDLE CreateThread(void *, U32, THREAD, void *, U32, U32 *);
API U32 WaitForSingleObject(HANDLE, U32);
API int CloseHandle(HANDLE);
API HANDLE CreateEventA(void *, int, int, const char *);
API int SetEvent(HANDLE);
API void *VirtualAlloc(void *, U32, U32, U32);
API int VirtualFree(void *, U32, U32);
API U32 SetErrorMode(U32);
_Static_assert(sizeof(void *) == 4, "fixture must be i386");

extern U32 fixture_call(U32 mode, U32 depth, U32 *out, U32 token);
extern char fixture_entry, fixture_return, fixture_done, fixture_probe;
extern void done_marker(void), probe(void);
static HANDLE gate_a, gate_b;
static U32 worker_exit_code;
#ifdef FIXTURE_EXTERNAL_BREAKPOINTS
#define OBSERVATION_MARKER "nop\n"
#else
#define OBSERVATION_MARKER "int3\n"
#endif

/* cdecl frame is unchanged at both markers: [caller, mode, depth, out, token].
 * Every call uses the SAME return marker, including recursive/threaded calls.
 */
__asm__(
    ".text\n"
    ".globl _fixture_call, _fixture_entry, _fixture_return\n"
    "_fixture_call:\n_fixture_entry:\n"
    OBSERVATION_MARKER
    "pushl 16(%esp)\npushl 16(%esp)\npushl 16(%esp)\npushl 16(%esp)\n"
    "call _fixture_body\naddl $16, %esp\n"
    "movl $0x2468ace0, %edx\n"
    "_fixture_return:\n" OBSERVATION_MARKER "ret\n"
    ".globl _done_marker, _fixture_done\n"
    "_done_marker:\n_fixture_done:\nint3\nret\n"
    ".globl _probe, _fixture_probe\n"
    "_probe:\n_fixture_probe:\nnop\nret\n"
);

static void text(const char *s)
{
    U32 n = 0, wrote;
    while (s[n]) ++n;
    if (!WriteFile(GetStdHandle((U32)-11), s, n, &wrote, 0) || wrote != n)
        ExitProcess(70);
}

static void number(U32 n)
{
    char buf[11];
    U32 i = 10;
    buf[i] = 0;
    do { buf[--i] = '0' + n % 10; n /= 10; } while (n);
    text(buf + i);
}

static U32 command(void)
{
    char c;
    U32 got;
    if (!ReadFile(GetStdHandle((U32)-10), &c, 1, &got, 0) || got != 1)
        ExitProcess(71);
    return (U32)c;
}

U32 __attribute__((noinline)) fixture_body(U32 mode, U32 depth, U32 *out, U32 token)
{
    if (mode == 't' && depth == 1) {
        /* Both threads must enter before either can return. */
        if (!SetEvent(token == 201 ? gate_a : gate_b) ||
            WaitForSingleObject(token == 201 ? gate_b : gate_a, 5000) != 0)
            ExitProcess(78);
    }
    if (mode == 'f') {
        *(volatile U32 *)0 = token;  /* intentional access violation */
        return 0;
    }
    if (mode == 'x') ExitThread(23); /* deliberately leaves an open call */
    if (mode == 'u') {
        if (!VirtualFree(out, 0, 0x8000)) ExitProcess(72);
    } else {
        if (depth) fixture_call(mode, depth - 1, out, token + 1);
        *out = token ^ 0x13579bdf;
    }
    return token + depth + 17;
}

static U32 __attribute__((stdcall)) worker(void *arg)
{
    U32 out;
    fixture_call('t', 1, &out, (U32)arg);
    return worker_exit_code;
}

static U32 __attribute__((stdcall)) exiting_worker(void *unused)
{
    U32 out;
    (void)unused;
    fixture_call('x', 0, &out, 90);
    return 99; /* must not execute */
}

void start(void)
{
    U32 mode, out = 0, i, tid, wave;
    HANDLE a, b;
    U32 *page;
    SetErrorMode(0x0001 | 0x0002); /* no interactive fault UI */
    text("{\"version\":1,\"entry\":"); number((U32)&fixture_entry);
    text(",\"return\":"); number((U32)&fixture_return);
    text(",\"done\":"); number((U32)&fixture_done);
    text(",\"probe\":"); number((U32)&fixture_probe);
    text("}\n");
    mode = command(); /* fixture is inert until the debugger says go */
    if (mode == 't' || mode == 'r') {
        /* Deliberate debugger edge: 259 is also a possible ACTUAL exit value.
         * The fixture joins handles, never treats that value as liveness proof.
         */
        worker_exit_code = mode == 'r' ? 259 : 0;
        for (wave = 0; wave < (mode == 'r' ? 12u : 1u); ++wave) {
            gate_a = CreateEventA(0, 1, 0, 0);
            gate_b = CreateEventA(0, 1, 0, 0);
            if (!gate_a || !gate_b) ExitProcess(79);
            a = CreateThread(0, 0, worker, (void *)201, 0, &tid);
            b = CreateThread(0, 0, worker, (void *)202, 0, &tid);
            if (!a || !b) ExitProcess(73);
            if (WaitForSingleObject(a, 5000) != 0 || WaitForSingleObject(b, 5000) != 0)
                ExitProcess(74);
            CloseHandle(a); CloseHandle(b);
            CloseHandle(gate_a); CloseHandle(gate_b);
        }
    } else if (mode == 'x') {
        a = CreateThread(0, 0, exiting_worker, 0, 0, &tid);
        if (!a || WaitForSingleObject(a, 5000) != 0) ExitProcess(75);
        CloseHandle(a);
    } else if (mode == 's') {
        for (i = 0; i < 3; ++i) fixture_call(mode, 0, &out, 100 + i);
    } else {
        page = &out;
        if (mode == 'u') {
            page = VirtualAlloc(0, 4096, 0x3000, 0x04);
            if (!page) ExitProcess(76);
        }
        fixture_call(mode, mode == 'n' ? 2 : 0, page, 100);
    }
    done_marker();
    command(); /* debugger detaches before allowing the probe to execute */
    if (*(volatile unsigned char *)&fixture_probe != 0x90) ExitProcess(77);
    probe();
    text("{\"cleanup_ok\":true}\n");
    ExitProcess(0);
}

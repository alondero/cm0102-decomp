/* Authored freestanding i386 caller. REAL OS imports, no surrogate callees. */
typedef unsigned int U32;
typedef void *HANDLE;
#define API __attribute__((stdcall, dllimport))
API HANDLE GetStdHandle(U32);
API int ReadFile(HANDLE, void *, U32, U32 *, void *);
API int WriteFile(HANDLE, const void *, U32, U32 *, void *);
API HANDLE CreateFileA(const char *, U32, U32, void *, U32, U32, HANDLE);
API HANDLE GetProcessHeap(void);
API void *HeapAlloc(HANDLE, U32, U32);
API int HeapFree(HANDLE, U32, void *);
API int CloseHandle(HANDLE);
API U32 SetErrorMode(U32);
API void ExitProcess(U32) __attribute__((noreturn));
_Static_assert(sizeof(void *) == 4, "i386 only");
_Static_assert(sizeof(U32) == 4, "u32 stack words only");

/* Explicit import SLOT symbols, not the address of an authored shim. */
extern U32 heap_iat __asm__("__imp__HeapAlloc@12");
extern U32 read_iat __asm__("__imp__ReadFile@20");
extern char heap_pushes, heap_call, heap_cont, read_pushes, read_call, read_cont;
extern char fixture_done, fixture_probe;
extern void native_calls(void), done_marker(void), probe(void);
HANDLE heap, file;
U32 heap_result, read_result, read_count = 0xdeadbeef, read_request = 24;
unsigned char read_buffer[128];

/* Each continuation's FIRST instruction stores the untouched returned EAX.
 * There is NO caller ADD ESP after either WINAPI call. The live observer must
 * separately check the documented stdcall endpoint SP delta (16 / 24 bytes).
 */
__asm__(
    ".text\n.globl _native_calls\n_native_calls:\n"
    ".globl _heap_pushes, _heap_call, _heap_cont\n"
    "_heap_pushes:\npushl $32\npushl $8\npushl _heap\n"
    "_heap_call:\ncall *__imp__HeapAlloc@12\n"
    "_heap_cont:\nmovl %eax, _heap_result\n"
    ".globl _read_pushes, _read_call, _read_cont\n"
    "_read_pushes:\npushl $0\npushl $_read_count\npushl _read_request\n"
    "pushl $_read_buffer\npushl _file\n"
    "_read_call:\ncall *__imp__ReadFile@20\n"
    "_read_cont:\nmovl %eax, _read_result\nret\n"
    ".globl _done_marker, _fixture_done\n"
    "_done_marker:\n_fixture_done:\nint3\nret\n"
    ".globl _probe, _fixture_probe\n_probe:\n_fixture_probe:\nnop\nret\n"
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
    char b[11];
    U32 i = 10;
    b[i] = 0;
    do { b[--i] = '0' + n % 10; n /= 10; } while (n);
    text(b + i);
}

static U32 command(void)
{
    char c;
    U32 got;
    if (!ReadFile(GetStdHandle((U32)-10), &c, 1, &got, 0) || got != 1)
        ExitProcess(71);
    return (U32)c;
}

static void hex(const unsigned char *p, U32 size)
{
    const char *digits = "0123456789abcdef";
    char pair[3];
    U32 i;
    pair[2] = 0;
    for (i = 0; i < size; ++i) {
        pair[0] = digits[p[i] >> 4]; pair[1] = digits[p[i] & 15];
        text(pair);
    }
}

void start(void)
{
    U32 i, mode;
    SetErrorMode(3);
    heap = GetProcessHeap();
    file = CreateFileA("authored-input.bin", 0x80000000, 1, 0, 3, 0, 0);
    if (!heap || file == (HANDLE)-1) ExitProcess(72);
    for (i = 0; i < sizeof(read_buffer); ++i) read_buffer[i] = 0xa5;
    text("{\"version\":2,\"done\":"); number((U32)&fixture_done);
    text(",\"probe\":"); number((U32)&fixture_probe);
    text(",\"HeapAlloc\":{\"iat\":"); number((U32)&heap_iat);
    text(",\"pushes\":"); number((U32)&heap_pushes);
    text(",\"call\":"); number((U32)&heap_call);
    text(",\"continuation\":"); number((U32)&heap_cont);
    text("},\"ReadFile\":{\"iat\":"); number((U32)&read_iat);
    text(",\"pushes\":"); number((U32)&read_pushes);
    text(",\"call\":"); number((U32)&read_call);
    text(",\"continuation\":"); number((U32)&read_cont); text("}}\n");
    mode = command(); /* setup completed; only now perform the two selected calls */
    if (mode == 'l') read_request = 128;
    if (mode == 'e') {
        if (!CloseHandle(file)) ExitProcess(73);
        file = (HANDLE)-1; /* deliberate REAL ReadFile error, no replacement result */
    }
    native_calls();
    done_marker();
    command(); /* backend restores and detaches before release */
    if (*(volatile unsigned char *)&fixture_probe != 0x90) ExitProcess(74);
    probe();
    text("{\"heap_pointer\":"); number(heap_result);
    text(",\"heap_hex\":\"");
    if (heap_result) hex((unsigned char *)heap_result, 32);
    text("\",\"read_eax\":"); number(read_result);
    text(",\"read_count\":"); number(read_count);
    text(",\"read_hex\":\""); hex(read_buffer, read_request); text("\"}\n");
    if (file != (HANDLE)-1) CloseHandle(file);
    if (heap_result) HeapFree(heap, 0, (void *)heap_result);
    ExitProcess(0);
}

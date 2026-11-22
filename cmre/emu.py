"""Unicorn machine holding the complete original image plus an optional recovered image.

Every case starts from a pristine image (dirty pages are restored), a fixed
stack/arena/TEB layout and trapped imports, so results depend only on the case
specification. Nothing here models game or Windows behaviour: an import call
simply ends the case as an observable boundary.
"""
from __future__ import annotations

import bisect
import ctypes
import struct
from dataclasses import dataclass, field

import capstone
import unicorn
from capstone import x86_const as X
from unicorn import x86_const as R
from unicorn.unicorn_py3.unicorn import uclib

from . import image, imports, jump_transport, return_transport, x87_transport

PAGE = 0x1000
STACK, STACK_SIZE = 0x02000000, 0x40000
STACK_TOP = STACK + STACK_SIZE - 0x1000  # initial ESP; one page of caller frame above
ARENA, ARENA_SIZE = 0x03000000, 0x40000
MAX_ARENA_SIZE = 0x260000  # opt-in authored input capacity, about 2.4 MiB


def arena_capacity(size: int) -> int:
    if type(size) is not int or not ARENA_SIZE <= size <= MAX_ARENA_SIZE or size % PAGE:
        raise ValueError("arena_size must be page aligned, between 0x40000 and 0x260000")
    return size


TRAP, TRAP_SIZE = 0x7FF00000, 0x1000
STOP = 0x7FE00000
TEB = 0x7FFDF000
GDT = 0x7FFD0000
REGS = (R.UC_X86_REG_EAX, R.UC_X86_REG_ECX, R.UC_X86_REG_EDX, R.UC_X86_REG_EBX,
        R.UC_X86_REG_ESP, R.UC_X86_REG_EBP, R.UC_X86_REG_ESI, R.UC_X86_REG_EDI, R.UC_X86_REG_EFLAGS)
REG_NAMES = ("eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi", "eflags")


def _perms(section) -> int:
    perms = unicorn.UC_PROT_READ
    if section.writable:
        perms |= unicorn.UC_PROT_WRITE
    if section.executable:
        perms |= unicorn.UC_PROT_EXEC
    return perms


def _span(lo: int, hi: int) -> tuple[int, int]:
    return lo & ~(PAGE - 1), (hi + PAGE - 1) & ~(PAGE - 1)


def _align(lo: int, hi: int) -> tuple[int, int]:
    lo &= ~(PAGE - 1)
    hi = (hi + PAGE - 1) & ~(PAGE - 1)
    return lo, hi - lo


@dataclass
class Case:
    regs: dict[str, int]
    stack: list[int]  # dwords at ESP+4.. (ESP holds the STOP return address)
    arena: bytes
    globals: dict[int, int] = field(default_factory=dict)  # dword writes into the image
    import_seed: int = 0  # seeds the import stubs' return values
    fpcw: int | None = None  # optional incoming 16-bit x87 control word
    import_transcript: list | None = None
    # Captured input pages (ADR 0009): page VA -> 4 KiB of bytes observed in the
    # running original process at this function's entry. Mapped for this case only.
    pages: dict[int, bytes] | None = None


@dataclass
class Result:
    outcome: str  # return|fault|import|budget|escape|error
    detail: tuple
    regs: tuple
    events: list
    memory: dict  # page -> bytes, for pages whose content changed
    covered: set
    pristine: dict = field(default_factory=dict, repr=False)  # page -> initial bytes
    read_sites: dict = field(default_factory=dict)  # read event index -> EIP, when attribution is on
    fault_mapping: dict | None = None  # proof for a candidate extra-PE environment relation
    # (read event index, address, EIP) for each stack read wholly below the live ESP, when tracking is on
    dead_stack_reads: tuple = ()


class Machine:
    def __init__(self, original: image.Image, recovered: image.Image | None = None,
                 watch: tuple[int, int] | None = None, escape: bool = False,
                 arena_size: int = ARENA_SIZE):
        self.arena_size = arena_capacity(arena_size)
        self.uc = uc = unicorn.Uc(unicorn.UC_ARCH_X86, unicorn.UC_MODE_32)
        self.pristine: dict[int, bytes] = {}
        self._original_pages: set[int] = set()
        self._common_pages: set[int] = set()
        self._recovered_pages: dict[int, dict] = {}
        # Real PE protections: a store into .text/.rdata faults, as it would in the process.
        self._map(original.base, original.headers, perms=unicorn.UC_PROT_READ, origin='original')
        for s in original.sections:
            self._map(s.va, s.raw, s.virtual_size, _perms(s), origin='original')
        self.recovered = recovered
        self._executable = tuple((s.va, s.end) for s in original.sections if s.executable)
        # Page spans of writable original sections: the only already-mapped memory
        # a captured input page may overwrite.
        self._writable_image = tuple(_span(s.va, s.va + s.virtual_size)
                                     for s in original.sections if s.writable)
        self._case_pages: list[int] = []
        self._iat: dict[int, bytes] = {}
        # Recorder only (cmre/capture.py): supplies an absent page from the live
        # original process on an unmapped data access. Never set by a differential test.
        self.pager = None
        self.paged: dict[int, bytes] = {}
        self._indirect: dict = {}
        if recovered:
            for s in recovered.sections:
                self._map(s.va, s.raw, s.virtual_size, _perms(s),
                          origin='recovered-pe', section_name=s.name)
        for base, size in ((STACK, STACK_SIZE), (ARENA, self.arena_size), (TEB, PAGE), (GDT, PAGE)):
            self._map(base, bytes(size), size, unicorn.UC_PROT_READ | unicorn.UC_PROT_WRITE)
        for base, size in ((TRAP, TRAP_SIZE), (STOP, PAGE)):
            self._map(base, bytes(size), size, unicorn.UC_PROT_ALL)
        # Imports: each IAT slot points at its own stub (see imports.py; test scaffolding).
        self.trap_names = {}
        for i, (slot, name) in enumerate(sorted(original.imports.items())):
            trap = TRAP + 4 * i
            self.trap_names[trap] = name
            self._iat[slot] = struct.pack("<I", trap)
            self._poke(slot, struct.pack("<I", trap))
            self._poke(trap, imports.stub_code(name))
        # TEB: end-of-chain SEH, stack limits, self pointer; FS via a GDT descriptor.
        teb = struct.pack("<IIII", 0xFFFFFFFF, STACK + STACK_SIZE, STACK, 0) + bytes(8) + struct.pack("<I", TEB)
        self._poke(TEB, teb)
        gdt = bytes(8) + self._desc(0, 0xFFFFF, 0x9A, 0xC) + self._desc(0, 0xFFFFF, 0x92, 0xC) + self._desc(TEB, 0xFFF, 0x92, 0x4)
        self._poke(GDT, gdt)
        uc.reg_write(R.UC_X86_REG_GDTR, (0, GDT, len(gdt) - 1, 0))
        # Once a GDT is loaded every selector must name a flat ring-0 descriptor,
        # otherwise stack and data accesses through null selectors fault.
        uc.reg_write(R.UC_X86_REG_CS, 1 << 3)
        for seg in (R.UC_X86_REG_SS, R.UC_X86_REG_DS, R.UC_X86_REG_ES, R.UC_X86_REG_GS):
            uc.reg_write(seg, 2 << 3)
        uc.reg_write(R.UC_X86_REG_FS, 3 << 3)
        self.dirty: set[int] = set()
        self.written: set[int] = set()
        self.initial: dict[int, bytes] = {}
        self.case_arena = b""
        self.events: list = []
        self.fault = None
        self.stop_reason = None
        self.covered: set[int] = set()
        self.pending: int | None = None
        # Record which instruction made each read (input discovery only; it costs a register read).
        self.attribute_reads = False
        self.read_sites: dict[int, int] = {}
        # Record stack reads that lie wholly below the live ESP (the original side of a
        # C comparison only; it costs a register read per stack read).
        self.track_dead_stack_reads = False
        self.dead_stack_reads: list[tuple[int, int, int]] = []
        # The public binding allocates both a ctypes buffer and a bytearray per
        # read. Reuse only host scratch; each hook still reads guest memory at
        # the same point and appends its own immutable integer observation.
        self._read_buffers = {}
        for size, word in ((1, ctypes.c_uint8), (2, ctypes.c_uint16),
                           (4, ctypes.c_uint32), (8, ctypes.c_uint64)):
            buffer = ctypes.create_string_buffer(size)
            self._read_buffers[size] = buffer, word.__ctype_le__.from_buffer(buffer)
        uc.hook_add(unicorn.UC_HOOK_MEM_READ | unicorn.UC_HOOK_MEM_WRITE, self._access)
        uc.hook_add(unicorn.UC_HOOK_MEM_INVALID, self._invalid)
        self.return_runtime = None
        self.jump_runtime = None
        self.x87_runtime = None
        if recovered and recovered.return_bindings:
            self.return_runtime = return_transport.Runtime(uc, recovered.return_bindings)
        # Existing C-only machines retain Unicorn's native arrival budget. A
        # lifted CALL executes at its original address, so its VM redirects
        # must not consume the original-instruction budget.
        self.logical_budget = bool(recovered and any(
            binding.kind == 'lifted-direct' for binding in recovered.return_bindings))
        self._budget_limit = self._budget_steps = 0
        self._budget_exhausted = False
        if recovered and recovered.jump_bindings:
            self.jump_runtime = jump_transport.Runtime(uc, recovered.jump_bindings)
            # A pending indexed JMP must be checked before import scaffolding.
            uc.hook_add(unicorn.UC_HOOK_CODE, self._transport)
        if recovered and recovered.x87_bindings:
            self.x87_runtime = x87_transport.Runtime(uc, recovered.x87_bindings)
            if not self.jump_runtime:
                uc.hook_add(unicorn.UC_HOOK_CODE, self._transport)
        if self.logical_budget:
            if not self.jump_runtime and not self.x87_runtime:
                uc.hook_add(unicorn.UC_HOOK_CODE, self._transport)
            uc.hook_add(unicorn.UC_HOOK_CODE, self._charge_instruction)
        uc.hook_add(unicorn.UC_HOOK_CODE, self._trap, begin=TRAP, end=TRAP + TRAP_SIZE - 1)
        if self.return_runtime and not self.jump_runtime and not self.x87_runtime and not self.logical_budget:
            # Preserve the existing no-indexed-binding hook ordering.
            uc.hook_add(unicorn.UC_HOOK_CODE, self._transport)
        if watch:
            uc.hook_add(unicorn.UC_HOOK_CODE, self._escape if escape else self._cover, begin=watch[0], end=watch[1] - 1)
        # GPR writes alone leave Unicorn's hidden exception state behind.
        # Restore the initialized CPU (including segments) before every case.
        # Unicorn's reset can leave its internal x87 precision inconsistent
        # with the visible control word. Reapply that exact word through the
        # public setter before saving it; do not choose a different default.
        uc.reg_write(R.UC_X86_REG_FPCW, uc.reg_read(R.UC_X86_REG_FPCW))
        self._pristine_cpu = uc.context_save()

    def stop_indirect(self, sites: dict, entries: frozenset[int], local: tuple[tuple[int, int], ...],
                      words: int) -> None:
        """Stop before an indirect CALL/JMP at one of `sites` (va -> listing shape or None) when its
        target is not a function entry in `entries`, an import, or code in a `local` range.

        Semantic profile only (ADR 0002): an interior original target runs on registers the
        compiler chose, and a target that is not code ends in a fetch fault that hides the
        arguments. The stop records the target, the site, ECX and `words` argument dwords."""
        md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        md.detail = True
        self._entries, self._local, self._words = entries, local, words
        for site, shape in sorted(sites.items()):
            ins = next(md.disasm(bytes(self.uc.mem_read(site, 16)), site), None)
            if ins is not None and site not in self._indirect:
                self._indirect[site] = (ins, shape)
                self.uc.hook_add(unicorn.UC_HOOK_CODE, self._indirect_call, begin=site, end=site)

    def _slot_target(self, uc, ins) -> int | None:
        """The transfer target, or None when reading it faults (the instruction then faults itself)."""
        def reg(r):
            return uc.reg_read(getattr(R, f"UC_X86_REG_{ins.reg_name(r).upper()}"))
        op = ins.operands[0]
        if op.type == X.X86_OP_REG:
            return reg(op.reg)
        if op.mem.segment != X.X86_REG_INVALID:
            return None
        address = op.mem.disp + (reg(op.mem.base) if op.mem.base else 0)
        address += reg(op.mem.index) * op.mem.scale if op.mem.index else 0
        try:
            return struct.unpack("<I", uc.mem_read(address & 0xFFFFFFFF, 4))[0]
        except unicorn.UcError:
            return None

    def _indirect_call(self, uc, address, size, _):
        if self.stop_reason is not None or address not in self._indirect:
            return
        ins, shape = self._indirect[address]
        target = self._slot_target(uc, ins)
        if (target is None or target in self._entries or TRAP <= target < TRAP + TRAP_SIZE
                or any(lo <= target < hi for lo, hi in self._local)):
            return
        # A JMP leaves the caller's return address on top; its arguments sit above it.
        esp = uc.reg_read(R.UC_X86_REG_ESP) + (4 if ins.mnemonic == "jmp" else 0)
        words = []
        for i in range(self._words):
            try:
                words.append(struct.unpack("<I", uc.mem_read(esp + 4 * i, 4))[0])
            except unicorn.UcError:
                words.append(None)
        outcome = "interior" if any(lo <= target < hi for lo, hi in self._executable) else "bad-target"
        self.stop_reason = (outcome, target, address, uc.reg_read(R.UC_X86_REG_ECX), tuple(words), shape)
        uc.emu_stop()

    @staticmethod
    def _desc(base, limit, access, flags):
        return struct.pack("<HHBBBB", limit & 0xFFFF, base & 0xFFFF, (base >> 16) & 0xFF, access,
                           ((limit >> 16) & 0xF) | (flags << 4), (base >> 24) & 0xFF)

    def _map(self, va: int, data: bytes, size: int | None = None, perms: int = unicorn.UC_PROT_ALL,
             *, origin: str = 'common', section_name: str | None = None) -> None:
        lo, length = _align(va, va + (size if size is not None else len(data)))
        self.uc.mem_map(lo, length, perms)
        self.uc.mem_write(va, data)
        for p in range(lo, lo + length, PAGE):
            self.pristine[p] = None  # filled lazily after all pokes
            if origin == 'original':
                self._original_pages.add(p)
            elif origin == 'recovered-pe':
                self._recovered_pages[p] = {
                    'page': p, 'origin': origin, 'mapped_start': lo, 'mapped_end': lo + length,
                    'section_start': va, 'section_end': va + (size if size is not None else len(data)),
                    'initial_perms': perms, 'recovered_sections': [{
                        'name': section_name, 'va': va,
                        'end': va + (size if size is not None else len(data)),
                        'writable': bool(perms & unicorn.UC_PROT_WRITE),
                    }],
                }
            else:
                self._common_pages.add(p)
        self._pristine_ready = False

    def _fault_mapping_proof(self, access: int, address: int, size: int) -> dict | None:
        """Prove a candidate relation for added read-only PE mapping pages.

        This does not repair execution. The native write still faults, with its
        original registers, events, bytes and access class. A paired comparison
        must separately establish whether its raw data address was identical
        on the original; a truly relocated code pointer must remain protected.
        """
        if (access != unicorn.UC_MEM_WRITE_PROT or type(address) is not int
                or type(size) is not int or not 0 <= address < 1 << 32
                or size <= 0 or address + size > 1 << 32):
            return None
        regions = tuple(self.uc.mem_regions())
        pages = []
        first_page = address & ~(PAGE - 1)
        last_page = (address + size - 1) & ~(PAGE - 1)
        for page in range(first_page, last_page + PAGE, PAGE):
            source = self._recovered_pages.get(page)
            if (source is None or page in self._original_pages or page in self._common_pages
                    or not source['initial_perms'] & unicorn.UC_PROT_READ
                    or source['initial_perms'] & unicorn.UC_PROT_WRITE):
                return None
            current = next((perms for lo, hi, perms in regions
                            if lo <= page and page + PAGE - 1 <= hi), None)
            if (current is None or not current & unicorn.UC_PROT_READ
                    or current & unicorn.UC_PROT_WRITE):
                return None
            pages.append({**source, 'current_perms': current,
                          'original_mapped': False, 'common_mapped': False,
                          'recovered_sections': [dict(section) for section in source['recovered_sections']]})
        return {'schema': 'cmre-fault-mapping/1', 'raw_access': int(access),
                'projected_access': unicorn.UC_MEM_WRITE_UNMAPPED,
                'relation': 'candidate-original-unmapped-write', 'applied': False,
                'address': address, 'size': size, 'original_mapping': 'unmapped', 'pages': pages}

    def _poke(self, va: int, data: bytes) -> None:
        self.uc.mem_write(va, data)

    def _snapshot(self) -> None:
        for p in self.pristine:
            self.pristine[p] = bytes(self.uc.mem_read(p, PAGE))
        self._pristine_ready = True

    # hooks -----------------------------------------------------------------
    def _read_value(self, address: int, size: int) -> int:
        storage = self._read_buffers.get(size)
        if storage is None:
            return int.from_bytes(self.uc.mem_read(address, size), "little")
        buffer, word = storage
        # Same native call and UcError as Unicorn 2.x's public mem_read. Do not
        # use the buffer after an error, since a read may have filled only part.
        status = uclib.uc_mem_read(self.uc._uch, address, buffer, size)
        if status != unicorn.UC_ERR_OK:
            raise unicorn.UcError(status, address, size)
        return word.value

    def _access(self, uc, access, address, size, value, _):
        if access == unicorn.UC_MEM_WRITE:
            # The hook also fires for writes that then fault; only mapped pages are dirty.
            for page in {address & ~(PAGE - 1), (address + size - 1) & ~(PAGE - 1)}:
                if page in self.pristine:
                    self.dirty.add(page)
                    self.written.add(page)
            self.events.append(("w", address, size, value & ((1 << (8 * size)) - 1)))
        else:
            observed = self._read_value(address, size)
            if self.attribute_reads:
                self.read_sites[len(self.events)] = uc.reg_read(R.UC_X86_REG_EIP)
            if (self.track_dead_stack_reads and STACK <= address < STACK + STACK_SIZE
                    and address + size <= uc.reg_read(R.UC_X86_REG_ESP)):
                self.dead_stack_reads.append((len(self.events), address, uc.reg_read(R.UC_X86_REG_EIP)))
            self.events.append(("r", address, size, observed))
            if self.jump_runtime:
                self.jump_runtime.observe_read(address, size, observed)

    def _invalid(self, uc, access, address, size, value, _):
        if self.pager is not None and access in (unicorn.UC_MEM_READ_UNMAPPED, unicorn.UC_MEM_WRITE_UNMAPPED):
            wanted = {}
            for page in range(address & ~(PAGE - 1), ((address + max(size, 1) - 1) & ~(PAGE - 1)) + PAGE, PAGE):
                if page in self.pristine:
                    continue
                blob = self.pager(page)
                if blob is None:
                    wanted = None
                    break
                wanted[page] = bytes(blob)
            if wanted:
                for page, blob in wanted.items():
                    self._case_page(page, blob)
                    self.paged[page] = blob
                return True
        self.fault = (int(access), address, size)
        return False

    def _case_page(self, page: int, blob: bytes) -> None:
        """Map one captured input page for the current case; reset() removes it.

        A page may lie outside every mapping or inside a writable original
        section. Import slots keep their test stubs. Anything else is refused:
        captured bytes never replace code, read-only data or harness memory.
        """
        if type(page) is not int or page % PAGE or not 0 <= page < 1 << 32 or len(blob) != PAGE:
            raise ValueError("case pages are whole 4 KiB pages at page-aligned addresses")
        if page in self.pristine:
            if page in self._case_pages or not any(lo <= page < hi for lo, hi in self._writable_image):
                raise ValueError(f"case page {page:#010x} overlaps harness, read-only or repeated memory")
            self.uc.mem_write(page, bytes(blob))
            for slot, stub in self._iat.items():
                if page <= slot < page + PAGE:
                    self.uc.mem_write(slot, stub)
            self.dirty.add(page)
            return
        self.uc.mem_map(page, PAGE, unicorn.UC_PROT_READ | unicorn.UC_PROT_WRITE)
        self.uc.mem_write(page, bytes(blob))
        self.pristine[page] = bytes(blob)
        self._case_pages.append(page)

    def _trap(self, uc, address, size, _):
        if self.x87_runtime and (self.x87_runtime.rejection
                                 or (self.return_runtime and self.return_runtime.rejection)):
            return
        if self.jump_runtime and (self.jump_runtime.rejection
                                  or (self.return_runtime and self.return_runtime.rejection)):
            return
        name = self.trap_names.get(address)
        if name is None or name in imports.TERMINAL:
            if self.transcript is not None:
                self.stop_reason = ('error', 'import transcript',
                                    'unexpected terminal or unknown import: ' + (name or hex(address)),
                                    uc.reg_read(R.UC_X86_REG_EIP))
            else:
                self.stop_reason = ("import", name or hex(address))
            uc.emu_stop()
            return
        n = imports.STDCALL_ARGS[name]
        esp = uc.reg_read(R.UC_X86_REG_ESP)
        args = struct.unpack(f"<{n}I", uc.mem_read(esp + 4, 4 * n)) if n else ()
        self.events.append(("c", name, n, args))
        if self.transcript is None:
            value = imports.result(self.import_seed, self.import_count, ARENA, ARENA_SIZE)
            self._stub_outputs(name, args, value)
        else:
            try:
                value = self.transcript.call(name, args, self._transcript_write)
            except ValueError as exc:
                self.stop_reason = ('error', 'import transcript', str(exc), uc.reg_read(R.UC_X86_REG_EIP))
                uc.emu_stop()
                return
        uc.reg_write(R.UC_X86_REG_EAX, value)
        self.import_count += 1

    def _stub_outputs(self, name, args, value):
        """Write the documented out-parameters of a successful file-search/file-time stub."""
        def read(address, size):
            try:
                return bytes(self.uc.mem_read(address, size))
            except unicorn.UcError:
                return None
        for address, blob in imports.outputs(name, args, value, self.import_seed, self.import_count, read):
            try:
                self._transcript_write(address, blob)
            except unicorn.UcError:
                pass

    def _transcript_write(self, address, blob):
        # Host writes do not trigger Unicorn memory hooks. Retain final-memory
        # comparison and the same ordered write observations as guest writes.
        self.uc.mem_write(address, blob)
        for offset, value in enumerate(blob):
            self._access(self.uc, unicorn.UC_MEM_WRITE, address + offset, 1, value, None)

    def _cover(self, uc, address, size, _):
        # An instruction counts as covered once the next watched one starts, or the
        # run ends without faulting on it (see run()).
        if self.pending is not None:
            self.covered.add(self.pending)
        self.pending = address

    def _escape(self, uc, address, size, _):
        if self.x87_runtime and (self.x87_runtime.redirected(address)
                                 or self.x87_runtime.admitted(address)):
            return
        if self.jump_runtime and self.jump_runtime.redirected(address):
            return
        if self.return_runtime and (self.return_runtime.redirected(address)
                                    or self.return_runtime.admitted(address)):
            return
        self.stop_reason = ("escape", address)
        uc.emu_stop()

    def _transport(self, uc, address, size, _):
        if self.logical_budget and self._budget_steps >= self._budget_limit:
            # Stop at the native successor before a continuation dispatch,
            # escape/import hook or other protocol can observe a new arrival.
            self._budget_exhausted = True
            uc.emu_stop()
            return
        # A pending original instruction owns exactly its arrival and successor.
        # An idle CALL/JMP runtime is compatible; overlapping pending transfers
        # are rejected before either protocol can consume the other's arrival.
        if self.x87_runtime and self.x87_runtime.pending:
            if ((self.jump_runtime and self.jump_runtime.pending)
                    or (self.return_runtime and self.return_runtime.pending)):
                self.x87_runtime.reject('overlapping pending CALL/JMP and x87 transfers')
                self.stop_reason = ('escape', address)
                uc.emu_stop()
                return
            if not self.x87_runtime.step(address, size):
                self.stop_reason = ('escape', address)
                uc.emu_stop()
            return
        if self.jump_runtime:
            if not self.jump_runtime.step(address, size):
                self.stop_reason = ('escape', address)
                uc.emu_stop()
                return
            if self.jump_runtime.redirected(address):
                return
        if self.return_runtime and not self.return_runtime.step(address, size):
            self.stop_reason = ('escape', address)
            uc.emu_stop()
            return
        if self.x87_runtime:
            if self.return_runtime and (self.return_runtime.redirected(address)
                                        or self.return_runtime.admitted(address)):
                return
            if (address in self.x87_runtime.compiled
                    and ((self.jump_runtime and self.jump_runtime.pending)
                         or (self.return_runtime and self.return_runtime.pending))):
                self.x87_runtime.reject('overlapping pending CALL/JMP and x87 transfers')
                self.stop_reason = ('escape', address)
                uc.emu_stop()
                return
            if not self.x87_runtime.step(address, size):
                self.stop_reason = ('escape', address)
                uc.emu_stop()

    def _charge_instruction(self, uc, address, size, _):
        if self._budget_exhausted or self.stop_reason is not None:
            return
        if any(runtime and runtime.redirected(address) for runtime in (
                self.return_runtime, self.jump_runtime, self.x87_runtime)):
            return
        self._budget_steps += 1

    def _initial_page(self, p: int) -> bytes:
        if ARENA <= p < ARENA + self.arena_size:
            return self.case_arena[p - ARENA : p - ARENA + PAGE]
        return self.initial.get(p, self.pristine[p])

    # execution -------------------------------------------------------------
    def reset(self) -> None:
        self.uc.context_restore(self._pristine_cpu)
        if not self._pristine_ready:
            self._snapshot()
        for p in self._case_pages:
            self.uc.mem_unmap(p, PAGE)
            del self.pristine[p]
            self.dirty.discard(p)
        self._case_pages = []
        self.paged = {}
        for p in self.dirty:
            self.uc.mem_write(p, self.pristine[p])
        self.dirty = set()

    def run(self, entry: int, case: Case, budget: int) -> Result:
        if self.x87_runtime:
            self.x87_runtime.reset()
        if self.jump_runtime:
            self.jump_runtime.reset()
        if self.return_runtime:
            self.return_runtime.reset()
        if len(case.arena) != self.arena_size:
            raise ValueError("case arena length differs from machine arena capacity")
        self.transcript = (imports.Transcript(case.import_transcript, ARENA, self.arena_size)
                           if case.import_transcript is not None else None)
        self.reset()
        uc = self.uc
        for page, blob in sorted((case.pages or {}).items()):
            self._case_page(page, blob)
        for va, value in sorted(case.globals.items()):
            uc.mem_write(va, struct.pack("<I", value))
            self.dirty.update({va & ~(PAGE - 1), (va + 3) & ~(PAGE - 1)})
        uc.mem_write(ARENA, case.arena)
        frame = struct.pack("<I", STOP) + b"".join(struct.pack("<I", v & 0xFFFFFFFF) for v in case.stack)
        uc.mem_write(STACK_TOP, frame)
        self.dirty.update({ARENA + o for o in range(0, self.arena_size, PAGE)})
        # Case-seeded content of non-arena input pages (arena pages come from case.arena).
        self.initial = {p: bytes(uc.mem_read(p, PAGE)) for p in self.dirty if not ARENA <= p < ARENA + self.arena_size}
        self.case_arena = case.arena
        self.written = set()
        self.dirty.update({STACK_TOP & ~(PAGE - 1), (STACK_TOP + len(frame)) & ~(PAGE - 1)})
        for name, reg in zip(REG_NAMES, REGS):
            uc.reg_write(reg, case.regs.get(name, 0) & 0xFFFFFFFF)
        uc.reg_write(R.UC_X86_REG_ESP, STACK_TOP)
        if case.fpcw is not None:
            uc.reg_write(R.UC_X86_REG_FPCW, case.fpcw)
        self.events, self.fault, self.stop_reason, self.covered, self.pending = [], None, None, set(), None
        self.read_sites = {}
        self.dead_stack_reads = []
        self.import_seed, self.import_count = case.import_seed, 0
        self._budget_limit, self._budget_steps, self._budget_exhausted = budget, 0, False
        error = None
        try:
            # At most three owned redirect arrivals can surround one executed
            # instruction. Keep a finite native limit as a separate fail-closed
            # guard against a malformed zero-instruction redirect cycle.
            native_limit = max(32, 4 * budget + 16) if self.logical_budget else budget
            uc.emu_start(entry, STOP, count=native_limit)
        except unicorn.UcError as exc:
            error = exc.errno
        eip = uc.reg_read(R.UC_X86_REG_EIP)
        if self.pending is not None and not ((self.fault is not None or error is not None) and eip == self.pending):
            self.covered.add(self.pending)
        regs = tuple(uc.reg_read(r) for r in REGS)
        fault_mapping = None
        if self.fault is not None:
            fault_mapping = self._fault_mapping_proof(*self.fault)
            outcome, detail = "fault", (self.fault, eip)
        elif self.stop_reason is not None:
            outcome, detail = self.stop_reason[0], tuple(self.stop_reason[1:])
        elif error is not None:
            outcome, detail = "error", (error, eip)
        elif eip == STOP:
            if self.transcript is not None and self.transcript.index != len(self.transcript.steps):
                outcome, detail = 'error', ('import transcript not consumed', eip)
            else:
                outcome, detail = "return", ()
        elif self.logical_budget and not self._budget_exhausted:
            outcome, detail = "error", ('native-step guard exhausted', eip)
        else:
            outcome, detail = "budget", (eip,)
        # STOP is a top-level return only after every owned CALL has unwound.
        # Keep native faults and other stop reasons unchanged.
        if (outcome == 'return' and eip == STOP and self.return_runtime
                and (self.return_runtime.frames or self.return_runtime.pending)):
            outcome, detail = 'escape', (eip,)
        if outcome == 'return' and self.jump_runtime and not self.jump_runtime.completed():
            outcome, detail = 'escape', (eip,)
        if outcome == 'return' and self.x87_runtime and not self.x87_runtime.completed():
            outcome, detail = 'escape', (eip,)
        # Pages whose content changed during the run, relative to the case's initial
        # memory (a faulted/protected store marks a page but changes nothing).
        memory = {}
        for p in sorted(self.written):
            blob = bytes(uc.mem_read(p, PAGE))
            if blob != self._initial_page(p):
                memory[p] = blob
        return Result(outcome, detail, regs, self.events, memory, self.covered,
                      self.pristine, self.read_sites, fault_mapping, tuple(self.dead_stack_reads))


class AddressMap:
    """Translate recovered-image addresses/values to original VAs via v_<va> labels."""

    def __init__(self, symbols: dict[str, int], call_returns: dict[int, int], lo: int, hi: int, *, verified_fault_locations=None):
        pairs = sorted((addr, int(n[2:], 16)) for n, addr in symbols.items() if n.startswith("v_"))
        self.addrs = [a for a, _ in pairs]
        self.vas = [v for _, v in pairs]
        self.exact = dict(pairs)
        self.call_returns = call_returns  # compiled return address -> original return VA
        self.lo, self.hi = lo, hi  # recovered code section bounds
        self.verified_fault_locations = verified_fault_locations

    def location(self, addr: int) -> int:
        if not (self.lo <= addr < self.hi):
            return addr
        i = bisect.bisect_right(self.addrs, addr) - 1
        return self.vas[i] + (addr - self.addrs[i]) if i >= 0 else addr

    def fault_location(self, addr: int) -> int:
        if self.verified_fault_locations is None:
            return self.location(addr)
        return self.verified_fault_locations.get(addr, addr)

    def value(self, v: int) -> int:
        if v in self.call_returns:
            return self.call_returns[v]
        return self.exact.get(v, v)

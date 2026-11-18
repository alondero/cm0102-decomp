"""Publication boundary for the fresh GitHub history (issue #22).

The supplied game, original instruction streams, embedded executable bytes and
local analysis stay in the authoritative checkout. Before files are committed or
a commit is pushed, this scanner checks them against the local supplied files:

- original-bytes: THRESHOLD or more consecutive non-text bytes copied from a
  supplied PE image, raw or written as hex text. Other supplied files are
  sampled every COARSE_STRIDE bytes, so their excerpts are reported from
  COARSE_MINIMUM bytes; whole supplied files (asset-copy) are found by hash at
  any size. Runs with fewer than MIN_DISTINCT byte values (padding, small
  integers) are not distinctive.
- original-text: the game's own text. Matches under TEXT_MINIMUM characters are
  coincidental English and identifiers (API, DLL and symbol names) are not
  content; a file may quote briefly, but one quote of TEXT_QUOTE_LIMIT or a
  total of TEXT_FILE_LIMIT characters (a pasted string table) is blocked.
- instruction-listing: LISTING_LINES or more lines pairing an original address
  with an x86 instruction.
- credential, asset-path (supplied, generated or build paths), binary content,
  and private-history (ancestry shared with the prior local history).
- accepted-mismatch: recovery/c source in the published tree (or staged) whose
  bytes differ from what the local ledger accepted.

`scan --commit` covers every blob reachable from the commit and records a receipt
for its PUBLIC/PRIVATE push destination. A path/hash-specific authored-source
review can resolve only the listing heuristic; payload checks remain active.
The index is built from local files on each run; receipts record its identity.
"""
from __future__ import annotations

import hashlib
import json
import re
import subprocess
from dataclasses import asdict, dataclass
from datetime import datetime, timezone
from itertools import groupby
from pathlib import Path, PurePosixPath

from . import env
from .guard import PUBLICATION_SCHEMA, PUBLICATION_VISIBILITIES, receipt_allows_push, remote_state

THRESHOLD = 16
STRIDE = 8  # every run of THRESHOLD bytes contains a block starting at a multiple of STRIDE
COARSE_STRIDE = 64
COARSE_MINIMUM = COARSE_STRIDE + STRIDE - 1
MIN_DISTINCT = 4
# Game text: shorter matches are coincidental English; a file may quote a little.
TEXT_MINIMUM = 24
TEXT_QUOTE_LIMIT = 64
TEXT_FILE_LIMIT = 256
LISTING_LINES = 8
RECEIPT_SCHEMA = PUBLICATION_SCHEMA
RECEIPTS = env.ANALYSIS / "cmre/publication"
SUPPLIED = tuple(env.ROOT / "CM0102" / name for name in ("Game", "Original-3.9.60", "InstallMedia"))
# Generated baseline manifests quote public hashes; only whole copies matter.
COPY_ONLY = (env.ROOT / "CM0102/Inspection",)
PRIVATE_REFS = ("refs/private/cm0102-pre-publication",)  # preserved private history; never repoint
# Archives hold compressed copies; their extracted payloads are indexed instead.
CONTAINERS = {".iso", ".zip", ".cab", ".7z"}

ASSET_ROOTS = {"CM0102", "tools"}
LOCAL_RECORDS = ("recovery/ledger/", "recovery/lifted/", "recovery/blocked/", "recovery/hints/",
                 "recovery/c/tasks/", "local/fixtures/")
ASSET_SUFFIXES = {".exe", ".dll", ".pdb", ".obj", ".o", ".lib", ".iso", ".cab", ".zip", ".7z", ".lng",
                  ".dat", ".hsr", ".his", ".wav", ".pct", ".rgn", ".fnt", ".t2k", ".mbr", ".bin", ".stackdump"}

HEX_TOKEN = re.compile(rb"(?:(?<=0x)|(?<=0X)|(?<=\\x)|(?<![0-9A-Za-z_]))([0-9A-Fa-f]{2,})(?![0-9A-Za-z_])")
HEX_SEPARATOR = re.compile(rb"(?:\s|[,;:'\"+()\[\]{}]|\\[nrt]|0[xX]|\\x)*")
IDENTIFIER = re.compile(rb"[A-Za-z_?@.$][\w.@?$]*")
PRINTABLE = re.compile(rb"[\x20-\x7e\t\r\n]+")
MNEMONIC = (rb"mov[a-z]*|lea|push[a-z]*|pop[a-z]*|call|jmp|j[a-z]{1,3}|ret[nf]?|cmp[a-z]*|test|x?add|sub|adc|sbb"
            rb"|xor|and|or|not|neg|inc|dec|i?mul|i?div|sh[lr]d?|sa[lr]|ro[lr]|rc[lr]|cdq|cwde|leave|nop|int3?"
            rb"|xchg|set[a-z]{1,3}|cmov[a-z]{1,3}|bt[a-z]?|f[a-z]{1,6}|stos[bwd]?|lods[bwd]?|scas[bwd]?|rep[a-z]*")
OPERAND = rb"(?:%|\[|(?:byte|word|dword|qword|tbyte)\b|e?[a-d]x\b|[a-d][lh]\b|e?[sd]i\b|e?[bs]p\b|st\b|st\(\d\))"
# A label may stand on its own line above its instruction, as in lifted assembly.
LISTING = re.compile(rb"(?im)^[^\n]*?(?<![\w])(?:v_|loc_|sub_|0x)?(?:00)?[4-9a-f][0-9a-f]{5}h?(?![0-9a-z_])[ \t]*"
                     rb"(?::[ \t]*\r?\n?)?[ \t]*"
                     rb"(?:[0-9a-f]{2}[ \t]+)*(?:(?:lock|rep[a-z]*)[ \t]+)?(?:" + MNEMONIC + rb")"
                     rb"(?:[ \t]+" + OPERAND + rb"|[ \t]*(?:$|\\n|\"))")
CREDENTIAL = re.compile(rb"gh[pousr]_[A-Za-z0-9]{36}|github_pat_[A-Za-z0-9_]{22,}|-----BEGIN [A-Z ]*PRIVATE KEY-----"
                        rb"|\bAKIA[0-9A-Z]{16}\b|sk-ant-[A-Za-z0-9_-]{20,}|\bxox[abprs]-[A-Za-z0-9-]{10,}"
                        rb"|AIza[0-9A-Za-z_-]{35}")


@dataclass(frozen=True, order=True)
class Finding:
    path: str
    kind: str
    detail: str


def _sha256(path: Path) -> str:
    h = hashlib.sha256()
    with path.open("rb") as handle:
        while chunk := handle.read(1 << 20):
            h.update(chunk)
    return h.hexdigest()


def _label(path: Path) -> str:
    return env.rel(path)


class OriginalIndex:
    """Sorted 8-byte blocks of the supplied files, extended to full runs on a hit."""

    def __init__(self, files, values, owners, offsets, copies):
        self.files = files  # [(path, sha256, stride)]
        self.values, self.owners, self.offsets = values, owners, offsets
        self.copies = copies  # size -> [(sha256, or None for unhashed archives, path)]
        self._data: dict[int, bytes] = {}

    @classmethod
    def build(cls, roots=SUPPLIED, copy_only=COPY_ONLY) -> "OriginalIndex":
        import numpy as np

        listed = lambda tree: sorted(p for root in tree if Path(root).is_dir() for p in Path(root).rglob("*") if p.is_file())
        copies: dict[int, list[tuple[str | None, Path]]] = {}
        unique: dict[str, Path] = {}
        for path in listed(copy_only):
            copies.setdefault(path.stat().st_size, []).append((_sha256(path), path))
        for path in listed(roots):
            if path.suffix.lower() in CONTAINERS:
                copies.setdefault(path.stat().st_size, []).append((None, path))  # hashed only on a size match
                continue
            sha = _sha256(path)
            copies.setdefault(path.stat().st_size, []).append((sha, path))
            unique.setdefault(sha, path)
        files, values, owners, offsets = [], [], [], []
        uniform = np.uint64(0x0101010101010101)
        for sha, path in sorted(unique.items(), key=lambda item: str(item[1])):
            data = path.read_bytes()
            stride = STRIDE if data[:2] == b"MZ" else COARSE_STRIDE
            count = len(data) // stride
            if count == 0:
                continue
            blocks = np.frombuffer(data, np.uint8, count * stride).reshape(count, stride)[:, :STRIDE]
            block_values = np.ascontiguousarray(blocks).view("<u8").ravel()
            keep = block_values != (block_values & np.uint64(0xFF)) * uniform
            values.append(block_values[keep])
            offsets.append(np.nonzero(keep)[0].astype(np.int64) * stride)
            owners.append(np.full(int(keep.sum()), len(files), np.int32))
            files.append((path, sha, stride))
        if files:
            values, owners, offsets = (np.concatenate(x) for x in (values, owners, offsets))
            order = np.argsort(values, kind="stable")
            values, owners, offsets = values[order], owners[order], offsets[order]
        else:
            values = np.zeros(0, np.uint64)
            owners, offsets = np.zeros(0, np.int32), np.zeros(0, np.int64)
        return cls(files, values, owners, offsets, copies)

    def identity(self) -> str:
        files = [[_label(path), sha, stride] for path, sha, stride in self.files]
        payload = {"threshold": THRESHOLD, "stride": STRIDE, "coarse_stride": COARSE_STRIDE,
                   "min_distinct": MIN_DISTINCT, "text": [TEXT_MINIMUM, TEXT_QUOTE_LIMIT, TEXT_FILE_LIMIT],
                   "files": files}
        return hashlib.sha256(json.dumps(payload, sort_keys=True).encode()).hexdigest()

    def _source(self, owner: int) -> bytes:
        if owner not in self._data:
            self._data[owner] = self.files[owner][0].read_bytes()
        return self._data[owner]

    def copy_of(self, data: bytes) -> Path | None:
        candidates = self.copies.get(len(data))
        if not candidates:
            return None
        sha = hashlib.sha256(data).hexdigest()
        for known, path in candidates:
            if known == sha or (known is None and _sha256(path) == sha):
                return path
        return None

    def runs(self, data: bytes) -> list[tuple[int, int, int, int]]:
        """Distinctive copied runs as (start, length, owner, source offset), in data order."""
        import numpy as np

        if len(data) < STRIDE or not len(self.values):
            return []
        array = np.frombuffer(data, np.uint8)
        hits = []
        for phase in range(STRIDE):
            count = (len(array) - phase) // STRIDE
            if count <= 0:
                break
            windows = array[phase:phase + count * STRIDE].view("<u8")
            slots = np.minimum(np.searchsorted(self.values, windows), len(self.values) - 1)
            hits.extend(int(i) * STRIDE + phase for i in np.nonzero(self.values[slots] == windows)[0])
        found, covered = [], 0
        for start in sorted(hits):
            if start < covered:
                continue
            value = np.frombuffer(data, "<u8", 1, start)[0]
            lo, hi = np.searchsorted(self.values, value, "left"), np.searchsorted(self.values, value, "right")
            best = None
            for slot in range(lo, hi):
                owner, offset = int(self.owners[slot]), int(self.offsets[slot])
                source = self._source(owner)
                back = 0
                while back < start and back < offset and data[start - back - 1] == source[offset - back - 1]:
                    back += 1
                ahead = STRIDE
                while start + ahead < len(data) and offset + ahead < len(source) and \
                        data[start + ahead] == source[offset + ahead]:
                    ahead += 1
                minimum = THRESHOLD if self.files[owner][2] == STRIDE else COARSE_MINIMUM
                run = (start - back, back + ahead, owner, offset - back)
                if run[1] >= minimum and len(set(data[run[0]:run[0] + run[1]])) >= MIN_DISTINCT and (
                        best is None or run[1] > best[1]):
                    best = run
            if best:
                found.append(best)
                covered = best[0] + best[1]
        return found


def hex_sequences(text: bytes):
    """Byte strings written as hex in text: as written, and as little-endian 0x words."""
    tokens: list[re.Match] = []

    def flush():
        written, run = [], []
        for token in tokens + [None]:
            if token is None or len(token[1]) % 2:
                if run:
                    written.append((run[0].start(), bytes.fromhex(b"".join(t[1] for t in run).decode())))
                run = []
            else:
                run.append(token)
        width = lambda t: len(t[1]) if text[t.start() - 2:t.start()].lower() == b"0x" and len(t[1]) in (4, 8, 16) else 0
        for size, group in groupby(tokens, key=width):
            group = list(group)
            if size and len(group) > 1:
                written.append((group[0].start(),
                                b"".join(int(t[1], 16).to_bytes(size // 2, "little") for t in group)))
        tokens.clear()
        return [(start, data) for start, data in written if len(data) >= THRESHOLD]

    for token in HEX_TOKEN.finditer(text):
        if tokens and not HEX_SEPARATOR.fullmatch(text, tokens[-1].end(), token.start()):
            yield from flush()
        tokens.append(token)
    yield from flush()


def _line(text: bytes, offset: int) -> int:
    return text.count(b"\n", 0, offset) + 1


def path_findings(path: str) -> list[Finding]:
    posix = PurePosixPath(path)
    if posix.parts and posix.parts[0] in ASSET_ROOTS:
        return [Finding(path, "asset-path", f"{posix.parts[0]}/ is local-only")]
    if path.startswith(LOCAL_RECORDS):
        return [Finding(path, "asset-path", "generated recovery records are local-only")]
    if posix.name.upper() == "NUL" or posix.suffix.lower() in ASSET_SUFFIXES:
        return [Finding(path, "asset-path", f"{posix.suffix or posix.name} files are not published")]
    return []


def load_source_reviews(path: Path | None) -> dict:
    """Local analyst dispositions for exact authored-source path/content pairs."""
    if path is None:
        return {}
    try:
        document = json.loads(Path(path).read_text(encoding="utf-8"))
    except (OSError, ValueError) as exc:
        raise SystemExit(f"cannot read source reviews: {exc}") from exc
    if (not isinstance(document, dict) or document.get("schema") != "cmre-source-reviews/1"
            or not isinstance(document.get("reviews"), list)):
        raise SystemExit("invalid source reviews schema")
    reviews = {}
    for entry in document["reviews"]:
        if not isinstance(entry, dict) or not all(isinstance(entry.get(k), str) and entry[k].strip()
                                                for k in ("path", "sha256", "reviewer", "reason")):
            raise SystemExit("each source review needs path, sha256, reviewer and reason")
        name, sha = entry["path"], entry["sha256"]
        source = PurePosixPath(name)
        if (source.is_absolute() or source.as_posix() != name or ".." in source.parts
                or "\\" in name or ":" in name
                or source.suffix.lower() not in {".c", ".cc", ".cpp", ".cxx", ".h", ".hpp", ".s", ".asm"}
                or not re.fullmatch(r"[0-9a-f]{64}", sha) or (name, sha) in reviews):
            raise SystemExit(f"invalid or duplicate source review: {name}")
        reviews[name, sha] = {"reviewer": entry["reviewer"], "reason": entry["reason"]}
    return reviews


def content_findings(path: str, data: bytes, index: OriginalIndex, source_reviews=None) -> list[Finding]:
    found = []
    copy = index.copy_of(data)
    if copy:
        found.append(Finding(path, "asset-copy", f"identical to {_label(copy)}"))
    try:
        data.decode("utf-8")
        binary = b"\0" in data
    except UnicodeDecodeError:
        binary = True
    if binary:
        found.append(Finding(path, "binary", "binary content needs an explicit publication decision"))
    seen, text = set(), []
    candidates = [(None, data)] + list(hex_sequences(data))
    for number, (where, candidate) in enumerate(candidates):
        for start, length, owner, offset in index.runs(candidate):
            run = candidate[start:start + length]
            line = _line(data, start if where is None else where)
            source = _label(index.files[owner][0])
            if PRINTABLE.fullmatch(run):
                if length >= TEXT_MINIMUM and not IDENTIFIER.fullmatch(run):
                    text.append((number, start, start + length, line, source))
            elif (line, source) not in seen:
                seen.add((line, source))
                found.append(Finding(path, "original-bytes", f"line {line}: {length} bytes match {source} "
                                                             f"file offset 0x{offset:x}"))
    quotes = []  # one quotation reached through several index blocks is one interval
    for number, start, end, line, source in sorted(text):
        if quotes and quotes[-1][0] == number and start <= quotes[-1][2]:
            quotes[-1][2] = max(quotes[-1][2], end)
        else:
            quotes.append([number, start, end, line, source])
    total = sum(end - start for _, start, end, _, _ in quotes)
    if quotes:
        number, start, end, line, source = max(quotes, key=lambda q: q[2] - q[1])
        if end - start >= TEXT_QUOTE_LIMIT or total >= TEXT_FILE_LIMIT:
            sample = candidates[number][1][start:min(end, start + 60)].decode()
            found.append(Finding(path, "original-text",
                                 f"{len(quotes)} quotes, {total} characters of game text; longest {end - start} "
                                 f"at line {line} {sample!r} matches {source}"))
    found.extend(instruction_listing_findings(path, data, source_reviews))
    for match in CREDENTIAL.finditer(data):
        found.append(Finding(path, "credential", f"line {_line(data, match.start())}: {match[0][:12].decode(errors='replace')}..."))
    return sorted(set(found))


def instruction_listing_findings(path: str, data: bytes, source_reviews=None) -> list[Finding]:
    """Advisory provenance findings for authored candidate bytes."""
    listing = list(LISTING.finditer(data))
    if len(listing) < LISTING_LINES or (path, env.digest_bytes(data)) in (source_reviews or {}):
        return []
    first = _line(data, listing[0].start())
    return [Finding(path, "instruction-listing", f"{len(listing)} address-keyed instruction lines from line {first}")]


def candidate_listing_findings(root: Path, va: int, data: bytes) -> list[Finding]:
    """Use the same hash-specific dispositions as publication, without supplied assets."""
    reviews = root / "CM0102/Analysis/cmre/publication/source-reviews.json"
    path = f"recovery/c/fn_{va:08x}.c"
    findings = instruction_listing_findings(path, data)
    if not findings or not reviews.is_file():
        return findings
    try:
        dispositions = load_source_reviews(reviews)
    except (OSError, ValueError, SystemExit):
        return findings  # Unreadable review evidence never suppresses an advisory finding.
    return [] if (path, env.digest_bytes(data)) in dispositions else findings


C_SOURCE = re.compile(r"recovery/c/fn_([0-9a-f]{8})\.c")


def accepted_mismatch(path: str, data: bytes) -> list[Finding]:
    """Published C for an accepted function must be the bytes its ledger accepted."""
    match = C_SOURCE.fullmatch(path)
    record = env.LEDGER_DIR / f"fn_{match[1]}.json" if match else None
    if not record or not record.is_file():
        return []
    expected = (json.loads(record.read_text(encoding="utf-8")).get("c") or {}).get("source_sha256")
    if not expected or env.digest_bytes(data) == expected:
        return []
    return [Finding(path, "accepted-mismatch", f"bytes differ from the accepted source {expected[:12]}; "
                                               "Git line-ending normalisation or an unaccepted revision")]


def scan_blob(path: str, data: bytes, index: OriginalIndex, source_reviews=None) -> list[Finding]:
    return sorted(set(path_findings(path) + content_findings(path, data, index, source_reviews)))


def _git(repo: Path, *args: str, stdin: bytes | None = None) -> bytes:
    return subprocess.run(["git", *args], cwd=repo, input=stdin, capture_output=True, check=True).stdout


def _blobs(repo: Path, specs: list[str]):
    """Contents for object names or `:path` index entries, via one cat-file process."""
    if not specs:
        return
    out = _git(repo, "cat-file", "--batch", stdin=("\n".join(specs) + "\n").encode())
    position = 0
    for spec in specs:
        end = out.index(b"\n", position)
        header = out[position:end].split()
        if header[-1] == b"missing":
            raise SystemExit(f"git object missing: {spec}")
        size = int(header[2])
        yield spec, out[end + 1:end + 1 + size]
        position = end + 1 + size + 1


def scan_commit(repo: Path, ref: str, index: OriginalIndex,
                private_refs=PRIVATE_REFS, source_reviews=None) -> tuple[str, list[Finding]]:
    """Every path and blob reachable from `ref`, and its ancestry against the private history."""
    repo = Path(repo)
    commit = _git(repo, "rev-parse", "--verify", ref + "^{commit}").decode().strip()
    found = []
    for private in private_refs:
        base = subprocess.run(["git", "merge-base", commit, private], cwd=repo, capture_output=True, text=True)
        if base.returncode == 0:
            found.append(Finding(private, "private-history", f"shares commit {base.stdout.strip()} with {ref}"))
        elif base.returncode != 1:
            raise SystemExit(f"cannot compare {ref} with private ref {private}: {base.stderr.strip()}")
    paths = {line for line in _git(repo, "log", "--format=", "--name-only", "--no-renames", "-z", commit)
             .decode().replace("\n", "\0").split("\0") if line}
    for path in sorted(paths):
        found += path_findings(path)
    named = {}
    for line in _git(repo, "rev-list", "--objects", commit).decode().splitlines():
        sha, _, path = line.partition(" ")
        if path:
            named.setdefault(sha, set()).add(path)
    # rev-list records only one path per object. Recover every historical path so
    # a reviewed blob at one path cannot authorize an identical dump at another.
    raw = _git(repo, "log", "-m", "--format=", "--raw", "--no-abbrev", "--no-renames", "--root", "-z", commit).split(b"\0")
    for number, record in enumerate(raw[:-1]):
        fields = record.strip().split()
        if len(fields) == 5 and fields[0].startswith(b":"):
            path = raw[number + 1].decode()
            for sha in fields[2:4]:
                if sha.decode() in named:
                    named[sha.decode()].add(path)
    # Git 2.30 has no type filter for rev-list; trees also carry path names.
    kinds = _git(repo, "cat-file", "--batch-check=%(objectname) %(objecttype)",
                 stdin=("\n".join(named) + "\n").encode()).decode().split()
    named = {sha: named[sha] for sha, kind in zip(kinds[::2], kinds[1::2]) if kind == "blob"}
    for sha, data in _blobs(repo, sorted(named)):
        for path in sorted(named[sha]):
            found += path_findings(path)
            found += content_findings(path, data, index, source_reviews)
    # Provenance is a property of the tree being published, not of superseded blobs.
    tree = [line.split("\t", 1) for line in _git(repo, "ls-tree", "-r", commit, "--", "recovery/c").decode().splitlines()]
    current = {}
    for meta, path in tree:
        if C_SOURCE.fullmatch(path):
            current.setdefault(meta.split()[2], []).append(path)
    for sha, data in _blobs(repo, sorted(current)):
        for path in current[sha]:
            found += accepted_mismatch(path, data)
    return commit, sorted(set(found))


def scan_staged(repo: Path, index: OriginalIndex, source_reviews=None) -> list[Finding]:
    names = [n for n in _git(repo, "diff", "--cached", "--name-only", "-z", "--diff-filter=ACMR").decode().split("\0") if n]
    found = []
    for spec, data in _blobs(repo, [":" + name for name in names]):
        found += scan_blob(spec[1:], data, index, source_reviews)
    sources = _git(repo, "ls-files", "--cached", "-z", "--", "recovery/c").decode().split("\0")
    staged = sorted({path for path in sources if C_SOURCE.fullmatch(path)})
    for spec, data in _blobs(repo, [":" + path for path in staged]):
        found += accepted_mismatch(spec[1:], data)
    return sorted(set(found))


def scan_paths(paths, index: OriginalIndex, source_reviews=None) -> list[Finding]:
    found = []
    for given in paths:
        given = Path(given)
        files = [given] if given.is_file() else sorted(
            p for p in given.rglob("*") if p.is_file() and "__pycache__" not in p.parts)
        for path in files:
            found += scan_blob(env.rel(path), path.read_bytes(), index, source_reviews)
    return sorted(set(found))


def write_receipt(directory: Path, commit: str, findings, index: OriginalIndex, remote: dict,
                  source_reviews=None) -> Path:
    passed = not findings and remote.get("visibility") in PUBLICATION_VISIBILITIES and bool(remote.get("url"))
    receipt = {"schema": RECEIPT_SCHEMA, "commit": commit, "result": "pass" if passed else "fail",
               "created": datetime.now(timezone.utc).isoformat(), "target_sha256": env.TARGET_SHA256,
               "threshold_bytes": THRESHOLD, "coarse_minimum_bytes": COARSE_MINIMUM,
               "text_limits": {"minimum": TEXT_MINIMUM, "quote": TEXT_QUOTE_LIMIT, "file": TEXT_FILE_LIMIT},
               "index": {"sha256": index.identity(), "files": len(index.files)},
               "remote": remote, "findings": [asdict(f) for f in findings],
               "source_reviews": [{"path": name, "sha256": sha, **review}
                                  for (name, sha), review in sorted((source_reviews or {}).items())]}
    directory.mkdir(parents=True, exist_ok=True)
    path = directory / f"{commit}.json"
    path.write_text(json.dumps(receipt, indent=2) + "\n", encoding="utf-8", newline="\n")
    return path


MANIFEST = "STATUS.tsv"


def manifest_rows(c_dir: Path, ledger_dir: Path, index: OriginalIndex, source_reviews=None) -> list[tuple[str, str, str]]:
    """(function, status, sha256) for each recovered C file that may be published.

    Accepted means the local ledger accepted exactly these bytes; wip means the
    file is not accepted (held, work in progress or unrecorded). Files with
    unresolved instruction-listing findings or original bytes, and accepted files whose bytes differ
    from the accepted hash, are left out and stay local. The ledger's names and
    module paths are never written to the manifest.
    """
    rows = []
    for source in sorted(Path(c_dir).glob("fn_*.c")):
        data = source.read_bytes()
        if scan_blob(f"recovery/c/{source.name}", data, index, source_reviews):
            continue
        function, digest = source.stem, hashlib.sha256(data).hexdigest()
        ledger = Path(ledger_dir) / f"{function}.json"
        accepted = json.loads(ledger.read_text(encoding="utf-8")).get("c") if ledger.exists() else None
        if accepted is None:
            rows.append((function, "wip", digest))
        elif accepted.get("source_sha256") == digest:
            rows.append((function, "accepted", digest))
    return rows


def write_manifest(path: Path, rows) -> None:
    lines = ["function\tstatus\tsha256"] + [f"{function}\t{status}\t{digest}" for function, status, digest in rows]
    Path(path).write_text("\n".join(lines) + "\n", encoding="utf-8", newline="\n")


def supplied_index() -> OriginalIndex:
    """A clone without the pinned comparison executable cannot pass a scan."""
    if not env.TARGET.is_file() or env.digest(env.TARGET) != env.TARGET_SHA256:
        raise SystemExit("publication check unavailable: supply the pinned official 3.9.68 executable locally; "
                         "see docs/BUILDING.md")
    return OriginalIndex.build()


def manifest_command(a) -> int:
    source_reviews = load_source_reviews(getattr(a, 'source_reviews', None))
    index = supplied_index()
    source = Path(a.source) if a.source else env.ROOT
    c_dir = source / "recovery/c"
    rows = manifest_rows(c_dir, source / "recovery/ledger", index, source_reviews)
    out = Path(a.out) if a.out else c_dir / MANIFEST
    write_manifest(out, rows)
    published = {status: sum(1 for _, s, _ in rows if s == status) for status in ("accepted", "wip")}
    total = len(list(c_dir.glob("fn_*.c")))
    print(f"wrote {env.rel(out)}: {published['accepted']} accepted, {published['wip']} wip; "
          f"{total - len(rows)} function file(s) kept local")
    return 0


def command(a) -> int:
    source_reviews = load_source_reviews(getattr(a, 'source_reviews', None))
    index = supplied_index()
    print(f"index: {len(index.files)} supplied files, {len(index.values)} blocks, identity {index.identity()[:12]}")
    if a.commit:
        commit, findings = scan_commit(env.ROOT, a.commit, index, tuple(a.private_ref or PRIVATE_REFS), source_reviews)
        remote = remote_state(env.ROOT, a.remote)
        receipt = write_receipt(RECEIPTS, commit, findings, index, remote, source_reviews)
        print(f"remote {remote['name']} visibility {remote['visibility']}")
    elif a.staged:
        findings, receipt = scan_staged(env.ROOT, index, source_reviews), None
    else:
        findings, receipt = scan_paths(a.paths or ["."], index, source_reviews), None
    for finding in findings[:a.limit]:
        print(f"{finding.kind:20} {finding.path}: {finding.detail}")
    if len(findings) > a.limit:
        print(f"... {len(findings) - a.limit} more")
    print(f"{len(findings)} finding(s) in {len({f.path for f in findings})} path(s)")
    if receipt:
        allowed = receipt_allows_push(receipt, commit, a.remote)
        print(f"receipt {env.rel(receipt)}: {'pass' if allowed else 'fail'}")
        return 0 if allowed else 1
    return 1 if findings else 0


def verify_remote(a) -> int:
    """After a push: destination identity and main still match the scanned commit."""
    commit = _git(env.ROOT, "rev-parse", "--verify", a.ref + "^{commit}").decode().strip()
    remote = remote_state(env.ROOT, a.remote)
    heads = _git(env.ROOT, "ls-remote", remote["url"], "refs/heads/main").decode().split() if remote["url"] else []
    ok = heads[:1] == [commit] and receipt_allows_push(
        RECEIPTS / f"{commit}.json", commit, a.remote, remote_url=remote["url"], visibility=remote["visibility"])
    print(f"{a.remote} main {heads[0] if heads else 'missing'}; local {commit}; visibility {remote['visibility']}")
    return 0 if ok else 1


def add_commands(sub) -> None:
    p = sub.add_parser("publication", help="publication boundary: scan paths, staged files or a commit's history")
    actions = p.add_subparsers(dest="action", required=True)
    s = actions.add_parser("scan", help="report original bytes, listings, assets and credentials")
    s.add_argument("paths", nargs="*", help="working files or folders (default: whole checkout)")
    mode = s.add_mutually_exclusive_group()
    mode.add_argument("--staged", action="store_true", help="the files staged for the next commit")
    mode.add_argument("--commit", help="every blob reachable from this ref; writes the push receipt")
    s.add_argument("--private-ref", action="append", help=f"history that must not be shared (default {PRIVATE_REFS[0]})")
    s.add_argument("--remote", default="origin")
    s.add_argument("--source-reviews", type=Path, help="local path/hash-specific authored-source review JSON")
    s.add_argument("--limit", type=int, default=200, help="findings to print")
    s.set_defaults(func=command)
    s = actions.add_parser("verify-remote", help="after a push, check the remote branch and scanned destination")
    s.add_argument("ref")
    s.add_argument("--remote", default="origin")
    s.set_defaults(func=verify_remote)
    s = actions.add_parser("manifest", help="write recovery/c/STATUS.tsv: accepted and wip functions that may be published")
    s.add_argument("--source", help="checkout whose recovery/c and ledger are read (default: this checkout)")
    s.add_argument("--out", help="manifest file to write (default: recovery/c/STATUS.tsv beside the sources)")
    s.add_argument("--source-reviews", type=Path, help="local path/hash-specific authored-source review JSON")
    s.set_defaults(func=manifest_command)

"""Pinned paths, hashes and toolchain environment.

Everything that can vary between machines or shells is resolved here so that the
rest of the harness is deterministic. Subprocesses always receive a PATH with the
pinned MinGW toolchain first: Git Bash otherwise resolves devkitPro's `as`, which
makes gcc fail silently.
"""
from __future__ import annotations

import hashlib
import os
import subprocess
import sys
from functools import cache
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
TARGET = ROOT / "CM0102/Game/cm0102.exe"
TARGET_SHA256 = "0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86"
IMAGE_BASE = 0x00400000
ADDRESS_CONVENTION = "preferred-image VA, image base 0x00400000"

ANALYSIS = ROOT / "CM0102/Analysis"
RUNS = ANALYSIS / "cmre/runs"
FUNCTIONS_TSV = ANALYSIS / "baseline-c/exports/functions.tsv"
CALLS_TSV = ANALYSIS / "recovery-map-b/calls.tsv"
MODULES_JSON = ANALYSIS / "recovery-map-b/module-candidates.json"
# Accepted Ghidra exports; pinned so the work queue cannot silently change.
INVENTORY_SHA256 = {
    FUNCTIONS_TSV: "ecfe6c762c99ffd2058ea02698bfc9997f666937ff3a12171157da2676eca03a",
    CALLS_TSV: "020ce9259cf4cad2148be3394d20eb38066ee68ea69d690fccd7d54b33f86cd6",
    MODULES_JSON: "762212097945260d663742e9c8fd76d1d9e0b2cfe664500ed19a99d708a59d46",
}

RECOVERY = ROOT / "recovery"
LEDGER_DIR = RECOVERY / "ledger"
LIFTED_DIR = RECOVERY / "lifted"
C_DIR = RECOVERY / "c"
BLOCKED_DIR = RECOVERY / "blocked"
HINTS_DIR = RECOVERY / "hints"
REGISTRY = RECOVERY / "registry.json"

TOOLCHAIN = Path(os.environ.get("CMRE_TOOLCHAIN", "C:/ProgramData/mingw64/mingw64/bin"))
PYTHON_REQUIRED = (3, 13)

# Recovered images are linked here. It must not overlap the original image
# (0x00400000..0x00DE7000) or the emulator's stack/arena/trap regions, and its
# high bytes must not arise from small arena values (a random pointer into the
# recovered image would read code on one side and fault on the other).
LINK_BASE = 0x6E5D0000


def digest(path: Path) -> str:
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def digest_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def rel(path: Path) -> str:
    """Repository-relative POSIX path for records (absolute if outside the repository)."""
    path = Path(path).resolve()
    return path.relative_to(ROOT).as_posix() if path.is_relative_to(ROOT) else path.as_posix()


def tool_env() -> dict:
    env = dict(os.environ)
    env["PATH"] = str(TOOLCHAIN) + os.pathsep + env.get("PATH", "")
    return env


def tool(name: str) -> str:
    return str(TOOLCHAIN / (name + ".exe" if os.name == "nt" else name))


def run_tool(args: list[str], **kwargs) -> subprocess.CompletedProcess:
    return subprocess.run(args, env=tool_env(), capture_output=True, text=True, **kwargs)


@cache
def toolchain_identity() -> dict:
    out = {}
    for name in ("gcc", "as", "ld", "nm"):
        path = Path(tool(name))
        version = run_tool([str(path), "--version"]).stdout.splitlines()[0]
        out[name] = {"version": version, "sha256": digest(path)}
    return out


@cache
def target_bytes() -> bytes:
    data = TARGET.read_bytes()
    if digest_bytes(data) != TARGET_SHA256:
        raise SystemExit(f"Pinned target hash mismatch: {TARGET}")
    return data


def check_python() -> None:
    if sys.version_info[:2] != PYTHON_REQUIRED:
        raise SystemExit(
            f"cmre requires Python {PYTHON_REQUIRED[0]}.{PYTHON_REQUIRED[1]}; "
            f"run it as `py -3.13 -m cmre` (got {sys.version.split()[0]})"
        )

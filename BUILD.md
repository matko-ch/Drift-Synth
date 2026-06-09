# Drift — Build & Validation Guide

## Requirements

| Tool | Version | Notes |
|------|---------|-------|
| CMake | 3.22+ | tested with 4.1.1 |
| MSVC | 19.44+ (VS 2022 Build Tools) | `winget install Microsoft.VisualStudio.2022.BuildTools --override "--quiet --add Microsoft.VisualStudio.Workload.VCTools --includeRecommended --wait"` |
| Internet | required on first build | CMake fetches JUCE 8.0.4 and Catch2 v3.6.0 via FetchContent |

macOS / Linux: change the generator to Ninja or Makefile; AU format available on macOS only.

---

## Build (Windows / MSVC)

```bat
cmake -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release --parallel
```

### Artifact paths

| Artifact | Path |
|----------|------|
| VST3 plugin | `build\Drift_artefacts\Release\VST3\Drift.vst3` |
| Standalone exe | `build\Drift_artefacts\Release\Standalone\Drift.exe` |
| Unit test runner | `build\tests\Release\DriftTests.exe` |

### VST3 install location (Windows)

Copy the `.vst3` bundle to:

```
C:\Program Files\Common Files\VST3\Drift.vst3
```

---

## Unit Tests (Phase 2)

```bat
rem Direct runner (most informative):
build\tests\Release\DriftTests.exe --reporter console

rem Via CTest (generates one CTest entry per test):
ctest --test-dir build --build-config Release --output-on-failure
```

Expected: **36/36 pass**.

---

## pluginval (Phase 3)

Download the Windows binary from https://github.com/Tracktion/pluginval/releases/latest.

```bat
pluginval.exe --strictness-level 10 --validate-in-process ^
    build\Drift_artefacts\Release\VST3\Drift.vst3
```

Expected: **SUCCESS, exit 0**.

---

## Validation Status (as of commit ea38015)

| Phase | Result |
|-------|--------|
| 1 — MSVC build | PASS — 0 errors, 0 warnings |
| 2 — Catch2 tests | PASS — 36/36 |
| 3 — pluginval lvl10 | PASS — SUCCESS, exit 0 |
| 4 — RT safety audit | PASS — no alloc/lock/IO on audio thread; NaN/Inf clean at 44.1/48/96/192 kHz × {1,16} samples |
| 5 — Commit | PASS — ea38015 |

---

## DSP Fixes Applied During Validation

| Bug | Root Cause | Fix |
|-----|-----------|-----|
| Triangle amplitude 1.95 (expected ≤1.2) | Integrator cold-started at 0; DC-blocker needed 70+ cycles to settle | Initialize `mTriAccum`, `mTriDCx`, `mTriDCy` to the steady-state triangle value at reset time |
| Filter LP/HP gain 0.78 at drive=0 (expected ≈1.0) | `fastTanh(x*1)/1` compresses unity-amplitude signals even with no drive | Blend linearly from bypass (drive=0) to tanh saturation (drive=1) |

## MSVC Portability Fixes

| Issue | Fix |
|-------|-----|
| `[[gnu::hot]]` (C5030) | `DRIFT_HOT` macro expands to `[[gnu::hot]]` on GCC/Clang, empty on MSVC |
| `juce::Colour` constexpr (C2131) | Changed `inline constexpr auto` to `inline const auto` |
| `juce::String + int` ambiguity (C2666) | Changed `str + i` to `str + juce::String(i)` |
| `std::atomic<float>` copy-deleted (C2280) | Used `.load(std::memory_order_relaxed)` in `syncPatchFromParams` lambda |
| `const Rectangle::removeFromTop` (C2662) | Changed `const auto full` to `auto full` in `resized()` |

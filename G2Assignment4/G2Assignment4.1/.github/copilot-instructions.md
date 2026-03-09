# Copilot / AI assistant instructions for this repo

Purpose
- Help AI coding agents make concise, correct edits in this small C++ repository.

Quick summary
- This is a single-file C++ project: `main.cpp` is the canonical entry point.
- There is no top-level build system or CI config in the repo root to inspect.

What to look for first
- Open `main.cpp` to discover program inputs, feature flags, and any project-specific macros.
- If new files are added, check for a `CMakeLists.txt`, `Makefile`, or `.github/workflows` to adapt build/run steps.

Build & run (discoverable patterns)
- Common quick builds to try locally (adjust based on environment):
  - MinGW / g++: `g++ main.cpp -O2 -std=c++17 -o main.exe` then run `./main.exe` on Windows PowerShell.
  - MSVC: `cl /EHsc main.cpp` then run the produced executable.
- If compilation fails due to missing includes or libraries, inspect `#include` lines in `main.cpp` and add necessary packages or toolchain flags.

Code change constraints and style
- Keep edits minimal and focused: prefer small, self-contained patches that modify `main.cpp` only unless a new file is required.
- Preserve existing indentation and naming conventions used in `main.cpp`.
- Avoid introducing new third-party dependencies without adding a build manifest (CMake/Makefile) and update guidance.

Testing and debugging
- There are no tests in the repo. For runtime checks, compile and run the binary with representative inputs.
- Use logging or short `std::cerr` messages for quick debugging; avoid large refactors to add test scaffolding.

When adding files or expanding the project
- Add a `CMakeLists.txt` or `Makefile` at the repo root and include simple build instructions in `README.md`.
- Update this copilot instructions file when you change the build flow or add CI.

Examples from this repo
- Entry point: `main.cpp` — inspect top-level `main()` for argument parsing, I/O expectations, and global state.

What not to do
- Don't assume a complex multi-module build or hidden CI tools unless files for those exist in the repo.
- Don't add large dependency graphs; prefer pure standard-library solutions unless explicitly requested.

If uncertain
- Ask the repo maintainer before introducing new build systems or dependencies.

Next steps for a human reviewer
- Verify that `main.cpp` builds on your target toolchain and confirm preferred flags/standards (MSVC vs g++).
- Tell the AI which build toolchain to prefer and whether to add a `CMakeLists.txt` if you'd like that change.

— end —

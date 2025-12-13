# Development Log – mfind

This document is used as an internal development log for the `mfind` project.
It tracks progress, open tasks, known issues, and priorities.

---

## 📅 2025-12-13

### ✅ Done
- Repository structure created (`src/`, `include/`, `tests/`)
- Makefile added and verified (`make`, `make clean`)
- WSL + GCC development environment set up
- GitHub SSH authentication configured
- Initial README.md written
- Build artifacts excluded via `.gitignore`

### 🛠 In Progress
- Designing `config_t` structure for argument parsing
- Planning command-line option handling (`-name`, `-type`, depth options)

### ⏭ Next
- Implement argument parsing (`args.c / args.h`)
- Validate command-line options
- Add basic debug output for parsed config

### ⚠ Known Issues / Notes
- No actual directory traversal implemented yet
- No tests available at this stage

---

## 📅 2025-12-XX (planned)

### ⏭ Next Milestones
- Recursive directory traversal without filters
- Implement filter logic (`filters.c`)
- Combine traversal + filters
- Add basic test scripts

---

## 🧠 Design Decisions

- Start with single-threaded traversal before adding parallelism
- Keep filter logic separate from traversal logic
- Follow best-effort error handling (log to stderr, continue if possible)

---

## 📌 Open Questions

- How should symbolic links be handled? (ignore vs follow)
- Should multiple start directories be fully supported in MVP?
- When to introduce threading: before or after full filter support?

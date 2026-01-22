# mfind

`mfind` is a simplified reimplementation of the Unix `find` command, written in C.  
It recursively traverses directories, applies filters, and prints matching paths
to standard output. The output is line-based and pipeline-friendly.

This project was created as part of a university assignment.

---

## Features

- Recursive directory traversal
- Filter by name using shell-style patterns (`fnmatch`)
- Filter by file type (`f` = regular file, `d` = directory)
- Filter by file size (`-size`)
- Filter empty files and directories (`-empty`)
- Limit recursion depth (`-maxdepth`, `-mindepth`)
- Robust error handling
- No memory leaks (verified with Valgrind)
- Automated test suite (`make test`)

---

## Build

### Requirements

- GCC
- make
- Linux environment  
  (tested on Ubuntu via WSL)

### Build the project

```bash
make
```
### Clean build artifacts
```bash
make clean
```

---

### Usage
```bash
./mfind [OPTIONS] [STARTDIR]
```
- If no start directory is given, the current directory (.) is used.

--- 

### Supported Options

`-name <pattern>`
Filter by file name using shell-style patterns.
```bash 
./mfind . -name "*.c"
```
---

`-type f|d`
Filter by file type:
- `f` → regular files
- `d` → directories
```bash
./mfind . -type d
```

---

`-mindepth <n>`
Only print results at depth ≥ n.
```bash
./mfind . -mindepth 2
```

---

`-maxdepth <n>`
Do not traverse deeper than depth n.
```bash
./mfind . -maxdepth 1
```

---

`-size <N>[c|k|M]`
Filter files by size.
- `c` → bytes
- `k` → kilobytes (1024 bytes)
- `M` → megabytes (1024² bytes)
Optional prefix:
- `+N` → larger than N
- `-N` → smaller than N
- `N` → exactly N

Examples:
```bash
./mfind . -type f -size 0c       # empty files
./mfind . -type f -size 3c       # exactly 3 bytes
./mfind . -type f -size +1024c   # larger than 1024 bytes
./mfind . -type f -size -4c      # smaller than 4 bytes
```

---

`-empty`
Match empty files and empty directories.
```bash
./mfind . -empty
```

---

### Examples
```bash
./mfind . -name "*.c" -type f -maxdepth 2
```
```bash
./mfind tests/tmp -empty -type d
```

---

### Testing
Run automated tests
```bash
make test
```
The test suite checks:
- Basic execution
- `-name`
- `-type`
- `-empty`
- `-mindepth`
- `-maxdepth`
- `-size`
- Invalid argument handling

All tests must pass for a successful build.

---

### Memory Safety

The program is fully memory-safe.
Valgrind verification:
```bash
valgrind --leak-check=full --track-origins=yes ./mfind .
```

Result:
```text
All heap blocks were freed -- no leaks are possible
```

---

### Project Structure
```text
.
├── src/            # C source files
├── include/        # Header files
├── tests/
│   ├── run_tests.sh
│   └── tmp/        # Test fixtures
├── Makefile
├── README.md
└── DEVLOG.md
```

---

### Notes / Limitations
-Symbolic links are not followed
-No parallel traversal
-Best-effort error handling:
errors are printed to `stderr`, traversal continues where possible

---

### Coding Assistants
- ChatGPT was used for explanations, architectural guidance, and debugging help
- All code was written, reviewed, and understood by the authors

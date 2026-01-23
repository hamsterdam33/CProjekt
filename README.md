# mfind

`mfind` is a simplified reimplementation of the Unix `find` command, written in C.  
It recursively traverses directories, applies filters, and prints matching paths
to standard output. The output is line-based and pipeline-friendly.

This project was created as part of a university assignment.

---

## Features

- Recursive directory traversal
- Filter by name using shell-style patterns (`fnmatch`) via `-name`
- Filter by file type (`-type f|d`)
- Filter empty files and directories (`-empty`)
- Filter by file size (`-size`)
- Limit recursion depth (`-maxdepth`, `-mindepth`)
- Optional parallel traversal of **start directories** (`-j N`)
- Robust error handling (best effort, continues traversal where possible)
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
- If no start directory is given, the current directory (`.`) is used.
- You can provide multiple start directories.
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

`-empty`
Match empty files and empty directories
```bash
./mfind . -empty
```
`-j <N>`
Run traversal in parallel across start directories.
This means each start directory can be processed by a separate worker thread.

Example 
```bash
./mfind dir1 dir2 dir3 -j 4 -type f
```
Note:
- Output order may be nondeterministic with `-j > 1` (threads).
- The result set should be identical compared to `-j 1`.

---
### Examples 
```bash
./mfind . -name "*.c" -type f -maxdepth 2
```

```bash
./mfind tests/tmp -empty -type d
```

```bash
./mfind tests/tmp tests/tmp_size -type f -j 2
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
- `-j` (parallel startdirs)
- Invalid argument handling

> **Note (WSL/Windows mounts):**  
> Permission tests may be skipped if the filesystem ignores `chmod`.

All tests must pass for a successful build.

---

### Memory Safety

The program is fully memory-safe.
Valgrind verification:
```bash
valgrind --leak-check=full --track-origins=yes ./mfind .
```

Expected result:
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
- Symbolic links are not followed
- Best-effort error handling:
errors are printed to `stderr`, traversal continues where possible
- Parallelism (`-j`) is implemented only across start directories (no global work queue).

---

### Coding Assistants
- ChatGPT was used for explanations, architectural guidance, and debugging help
- All code was written, reviewed, and understood by the authors

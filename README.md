# mfind

`mfind` is a simplified reimplementation of the Unix `find` command, written in C.  
It recursively traverses directories, applies filters, and prints matching paths
to standard output. The output is line-based and pipeline-friendly.

This project was created as part of a university assignment.

---

## Features (MVP)

- Recursive directory traversal
- Filter by name using shell-style patterns (`fnmatch`)
- Filter by file type (regular file or directory)
- Limit recursion depth (`-maxdepth`, `-mindepth`)
- Output to stdout (one result per line)

---

## Build

### Requirements

- GCC
- make
- Linux environment (tested with WSL / Ubuntu)

### Build the project

```bash
make
```

### Clean build artifacts

```bash
make clean
```
---

## Usage 
```bash
./mfind [OPTIONS] [STARTDIR]
```
If no start directory is given, the current directory (.) is used.

## Supported options
```C
- name <pattern> //Filter by file name (e.g. "*.c")
- type f|d  //f for regular files, d for directories
- maxdepth <n> //Maximum recursion depth
- mindepth <n> //Minimum depth for output
```
---

## Example

```C
./mfind . -name "*.c" -type f -maxdepth 2
```

--- 

## Project Structure
```text
.
├── src/        # C source files
├── include/    # Header files
├── tests/      # Test scripts and fixtures
├── Makefile
└── README.md
```
---

## Notes/ Limitations
 - Symbolic links are not followed.
 - Error handling follows a best-effort approach: errors are printed to stderr, traversal continues where possible.
 - Parallel traversal is not implemented yet.

---

## Coding Assistants
- ChatGPT was used for explanations, project structuring, and conceptual guidance.
- No code was generated directly without understanding and manual adaptation.

#!/usr/bin/env bash
set -euo pipefail

PASS=0
FAIL=0

fail() {
    echo "❌ FAIL: $1"
    FAIL=$((FAIL + 1))
}

pass() {
    echo "✅ PASS: $1"
    PASS=$((PASS + 1))
}

# Build
make clean >/dev/null 2>&1 || true
make >/dev/null 

# ---  Fixtures for -size tests ---
rm -rf tests/tmp_size
mkdir -p tests/tmp_size

: > tests/tmp_size/empty                       # 0 bytes
printf "abc" > tests/tmp_size/three            # 3 bytes
dd if=/dev/zero of=tests/tmp_size/two_k bs=1 count=2048 status=none  # 2048 bytes

cleanup_tmp() {
  rm -rf tests/tmp_size
}
trap cleanup_tmp EXIT


# Helper: run command and expect exit code
expect_exit() {
    local expected="$1"
    shift
    set +e 
    "$@" >/tmp/mfind_out.txt 2>/tmp/mfind_err.txt
    local rc=$?
    set -e
    if [ "$rc" -eq "$expected" ]; then
        return 0
    fi 
        return 1
}


# Helper: run command, expect stdout contains pattern
expect_stdout_grep() {
    local pattern="$1"
    shift
    "$@" >/tmp/mfind_out.txt 2>/tmp/mfind_err.txt; 
    grep -qE "$pattern" /tmp/mfind_out.txt; 
}

expect_stdout_not_grep() {
    local pattern="$1"
    shift
    "$@" >/tmp/mfind_out.txt 2>/tmp/mfind_err.txt
    if grep -qE "$pattern" /tmp/mfind_out.txt; then
        return 1
    fi
    return 0
}


# Helper: run command, expect stderr contains pattern
expect_stderr_grep() {
    local pattern="$1"
    shift
    "$@" >/tmp/mfind_out.txt 2>/tmp/mfind_err.txt;
    set -e
    grep -qE "$pattern" /tmp/mfind_err.txt;
}




    ### Tests ###

    # 1) Smoke runs 
    if expect_exit 0 ./mfind .; then pass "runs (./mfind .)"; else fail "runs (./mfind .)"; fi

    # 2) -name "*.c" contains known files
    if expect_stdout_grep "src/args\.c" ./mfind -name "args.c" .; then pass "-name finds args.c"; else fail "-name finds args.c"; fi
    if expect_stdout_grep "src/main\.c" ./mfind -name "main.c" .; then pass "-name finds main.c"; else fail "-name finds main.c"; fi

    # 3) -tyoe d must include ./src
    if expect_stdout_grep "^\./src$" ./mfind -type d .; then pass "-type d finds ./src"; else fail "-type d finds ./src"; fi

    # 4) -type f must include ./mfind.c
    set +e
    ./mfind -type f . | grep -qx "./src"
    rc=$?
    set -e
    if [ $rc -ne 0 ]; then pass "-type f does not find ./src"; else fail "-type f excludes ./src"; fi

    # 5) -empty
    if expect_stdout_grep "tests/tmp/emptydir" ./mfind tests/tmp -empty; then pass "-empty finds emptydir"; else fail "-empty finds emptydir"; fi
    if expect_stdout_grep "tests/tmp/emptyfile.txt" ./mfind tests/tmp -empty; then pass "-empty finds emptyfile.txt"; else fail "-empty finds emptyfile.txt"; fi

    # 6) Error -name missing argument
    if expect_exit 1 ./mfind -name; then pass "error on missing arg"; else fail "error on missing arg"; fi

    # 7) Error: invalid -type 
    if expect_exit 1 ./mfind -type x .; then pass "error on invalid -type"; else fail "error on invalid -type"; fi

    # 8) -maxdepth 1 should not include depth-2 file
    if expect_stdout_not_grep "tests/tmp/notemptydir/file.txt" ./mfind tests/tmp -maxdepth 1; then pass "-maxdepth 1 excludes depth-2 file"; else fail "-maxdepth 1 excludes depth-2 file"; fi

    # 9)-mindepth 2 with -type f should include depth-2 file
    if expect_stdout_grep "tests/tmp/notemptydir/file.txt" ./mfind tests/tmp -mindepth 2 -type f; then pass "-mindepth 2 includes depth-2 file"; else fail "-mindepth 2 includes depth-2 file"; fi 

    # 10) -size 0c finds empty file#
    if expect_stdout_grep "tests/tmp_size/empty" ./mfind tests/tmp_size -type f -size 0c; then pass "-size 0c finds empty file"; else fail "-size 0c finds empty file"; fi

    # 11) -size 3c finds 3-byte file
    if expect_stdout_grep "tests/tmp_size/three" ./mfind tests/tmp_size -type f -size 3c; then pass "-size 3c finds three-byte file"; else fail "-size 3c finds three-byte file"; fi

    # 12) -size +1024c finds 2048-byte file
    if expect_stdout_grep "tests/tmp_size/two_k" ./mfind tests/tmp_size -type f -size +1024c; then pass "-size +1024c finds two_k file"; else fail "-size +1024c finds two_k file"; fi

    # 13) -size -4c finds 0-byte and 3-byte files (both are <4c)
    if expect_stdout_grep "tests/tmp_size/empty" ./mfind tests/tmp_size -type f -size -4c && expect_stdout_grep "tests/tmp_size/three" ./mfind tests/tmp_size -type f -size -4c; then pass "-size -4c finds empty and three-byte files"; else fail "-size -4c finds empty and three-byte files"; fi

    # Summary
    echo 
    echo "Pass: $PASS"
    echo "Fail: $FAIL"
    if [ $FAIL -eq 0 ]; then
        echo "All tests passed!"
        exit 0
    else
        echo "Some tests failed."
        exit 1
    fi

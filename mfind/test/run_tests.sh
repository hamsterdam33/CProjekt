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
    if expect_stdout_grep "tests/temp/emptydir" ./mfind tests/temp -empty; then pass "-empty finds emptydir"; else fail "-empty finds emptydir"; fi
    if expect_stdout_grep "tests/temp/emptyfile.txt" ./mfind tests/temp -empty; then pass "-empty finds emptyfile.txt"; else fail "-empty finds emptyfile.txt"; fi

    # 6) Error -name missing argument
    if expect_exit 1 ./mfind -name; then pass "error on missing arg"; else fail "error on missing arg"; fi

    # 7) Error: invalid -type 
    if expect_exit 1 ./mfind -type x .; then pass "error on invalid -type"; else fail "error on invalid -type"; fi

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

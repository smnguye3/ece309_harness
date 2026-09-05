#!/usr/bin/env bash
set -u

PROGRAM="./harness"

if [ ! -x "$PROGRAM" ]; then
    echo "ERROR: harness executable not found. Compile first with:"
    echo "gcc -Wall -Wextra -std=c11 harness.c -o harness"
    exit 1
fi

failures=0

run_test() {
    name="$1"
    input="$2"
    expected="$3"

    output=$(printf "%b" "$input" | "$PROGRAM")

    if printf "%s" "$output" | grep -Fq "$expected"; then
        echo "PASS: $name"
    else
        echo "FAIL: $name"
        echo "Expected to find: $expected"
        echo "Actual output:"
        echo "$output"
        failures=$((failures + 1))
    fi
}

run_test "hello greeting" "hello\nexit\n" "Hello! I am the mock model."
run_test "echo fallback" "random text\nexit\n" "Echo: random text"
run_test "calculator addition" "calc 8 + 4\nexit\n" "Tool result: 12.00"
run_test "calculator division by zero" "calc 8 / 0\nexit\n" "Tool error: division by zero is not allowed."

history_output=$(printf "one\ntwo\nthree\nfour\nfive\nsix\nhistory\nexit\n" | "$PROGRAM")

if printf "%s" "$history_output" | grep -Fq "User: one"; then
    echo "FAIL: history should discard turns older than the last 5"
    failures=$((failures + 1))
else
    echo "PASS: oldest turn discarded"
fi

if printf "%s" "$history_output" | grep -Fq "User: six" && \
   printf "%s" "$history_output" | grep -Fq "User: history"; then
    echo "PASS: recent history retained"
else
    echo "FAIL: recent history was not retained correctly"
    failures=$((failures + 1))
fi


if [ "$failures" -eq 0 ]; then
    echo "All automated tests passed."
    exit 0
else
    echo "$failures test(s) failed."
    exit 1
fi

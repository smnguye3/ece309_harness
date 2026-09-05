# ECE 309 Project 1 - Vibe Coding Log
# Scarlett Nguyen - smnguye3

## Purpose

This log documents the specification-driven development process used to generate and verify the mini LLM harness.

## Initial specification

I defined these rules:
1. The project must be written in standard C and compile with GCC in a POSIX environment.
2. The main program must use a terminal-based loop and read input with `fgets`.
3. Typing `exit` must safely stop the loop and terminate the program.
4. The real LLM must be replaced by a deterministic mock-model function.
5. Input containing the standalone word `hello` must be responded with a hard-coded greeting.
6. Other normal text should be echoed by the mock model.
7. The harness must store the last five user/model lines.
8. On the sixth turn, the oldest stored turn must be removed.
9. The harness must support at least one external-style tool function. I chose a calculator tool.
10. Calculator syntax must be `calc <number> <operator> <number>`.
11. Calculator operators must be `+`, `-`, `*`, and `/`.
12. Division by zero must be accounted for.
13. The program must not require an LLM API or network access.
14. The implementation should stay beginner-friendly and avoid unnecessary external libraries.
15. The test script must look at normal behavior, state management, and memory safety.

## Prompt 1 - Generate the main program

I gave the AI assistant this prompt:

> I need to write a simple terminal-based LLM agent harness in standard C for an ECE 309 project. I am a beginner, so keep the design simple and readable. Use only standard C libraries and make it compile with GCC in a POSIX environment. Use `fgets` for user input. Run a loop until the user types `exit`. Create a mock-model function instead of calling a real LLM API. If the input contains the standalone word `hello`, return a hard-coded greeting. Otherwise echo normal input. Store only the last five user/model conversation turns in a fixed-size context history. When the sixth turn is added, discard the oldest one. Add a calculator tool that recognizes input in the form `calc <number> <operator> <number>` and supports +, -, *, and /. Handle division by zero safely. Add a `history` command that lets me verify the stored context. Please keep the code in one file named `harness.c` and make it easy for a beginner to understand.

## AI Response - Program code 

The AI proposed:

- A 'Turn' structure containing one user message and one assistant message,
- A fixed array of five 'Turn' structures,
- Helper functions for newline removal and lowercase matching,
- A 'mock_model' function,
- A calculator parser using 'sscanf',
- An 'add_turn' function that shifts history when the array is full,
- A 'print_history' function,
- A 'while' loop in 'main' that reads input with 'fgets' and exits on 'exit'.

## Prompt 2 - Generate automated tests

I then gave the AI this prompt:

> I have a compiled C program named `harness`. Write a simple Bash script named `test.sh` for Linux, macOS, or WSL. It should pipe predefined input into the program and check its output. Test the `hello` greeting, normal echo behavior, calculator addition, division by zero, and the last-five-turn history behavior. 

## AI Response - Testing program

The AI generated a Bash script that:

- Runs deterministic tests with piped terminal input,
- Checks expected output using `grep`,
- cCounts failures,
- Sends more than five turns to test the context-window behavior,
- Verifies that an old turn is removed,
- Verifies that newer turns remain,
- Returns a nonzero exit status when a test fails.

## Final Verification Commands
My test commands:
```bash
wsl
sudo apt install gcc
cd /mnt/c/Users/nguye/ece309_harness
gcc harness.c -o harness
./harness
bash test.sh
```

My test inputs:
    hello
    blah blah
    calc 8 + 4
    calc 8 / 0
    history
    exit

## Result

The final project satisfies the project specifications: terminal loop, mock model, limited conversation context, tool execution, documented AI-assisted specification process, and automated testing. 
The program does not need a real LLM API and can be compiled and tested locally in a POSIX environment.
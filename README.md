# ECE 309 Project 1 - Mini LLM Harness in C
# Scarlett Nguyen - smnguye3

## Overview

This project's objective is to build a minimal LLM agent harness in C which will act as a bridge between LLM and the operating system, managing inputs, context boundaries, and tool execution. 
Vibe coding (steering an AI assistant) will be used to complete this project.

The program implements four functions:
- Terminal-based input/output loop with simulated responses,
- Mock model function (which will mimic an LLM),
- Conversation-context memory management that is stored for the last five turns,
- Simple calculator tool for mathematical operations.

Additionally in the repository, it will also include:
- An automated Bash test script
- Vibe coding log documenting the specification-driven AI workflow

## Files

- `README.md` - Project documentation
- `harness.c` - Main program code
- `test.sh` - Automated functional and memory tests
- `vibe_coding_log.md` - AI prompts, architectural rules, iterations, and verification notes


## Compile
(1) Initialize Windows Subsystem for Linux by running:
```bash
wsl
```

(2) Install GCC if you haven't by using:
```bash
sudo apt install gcc
```
OR
```bash
sudo apt update
```

(3) In the terminal, cd to where the file is located, ex:
```bash
cd /mnt/c/Users/nguye/ece309_harness
```

(4) Open the program:
```bash 
gcc harness.c -o harness
./harness
```

## Actions

Commands and behaviors:
- 'exit': Ends the program. (Standalone)
- 'history': Prints the stored conversation history. (Standalone)
- 'calc # <operator> #': Calculator tool. (Standalone)
    - Operators are '+', '-', '*', and '/'.
- Any sentence containing 'hello': Hard-coded mock-model greeting.
- Other normal text is echoed back.

Example:
    You: hello
    Model: Hello! I am the mock model.
    You: calc 10 / 2
    Model: Tool result: 5.00
    You: exit
    Goodbye!

## Context Management

The harness stores a maximum of five user/model conversation turns. 
On the sixth turn, the oldest turn is discarded. 

## Automated Tests

Run:

```bash
bash test.sh
```

The script checks:
- Greeting behavior,
- Echo behavior,
- Calculator behavior,
- Division-by-zero handling,
- Five-turn state/history behavior.

# UnixShell

## Status : Still in progress.....

This is a minimal UNIX shell that is implemented from scratch in C and C++ using POSIX system calls.

The goal of this project is for me to understand the how shells work at real OS level

## How it is working :

***Inside a REPL loop***

1.Reads user input from standard input 

2.Parses the input into command and arguments

3.Checks whether the command is a builtin

4.Check if its executable if not builtin (Searches in the PATH)

***Then uses Unix OS API to execute those executables***

5.Prints the next prompt

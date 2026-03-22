Resources Used :

The main resource that helped me was man page of linux.
Some other resources were these YT channels, this book tho i didnt use it much.

To execute executables :
To actually execute the executables i used fork and exec posix system calls 
exec does not:
start a new process
create a child
Instead it:
replaces the current process image
overwrites:
code segment
stack
heap
global variables
so we create a new process for exec to consume

s.data() returns pointer to the first element of any vector s of any type

argv is a array of each elements begin char pointers

While implementing pwd builtin, i used getcwd() syscall
When we pass NULL in getcwd()
What actually happens
getcwd:
calculates out required size
allocates memory using malloc
writes path into it
returns pointer

res → heap memory → "/home/sanjai"
so in this case we must free the used memory
Else we can pass a buffer, getcwd returns a pointer to the buffer after writing the current workin dir to the buffer 

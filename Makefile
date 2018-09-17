filename = ipc
objects = ipc.o

ipc: $(objects)
	clang -o ${filename} -Wall $(objects)

ipc.o: ipc.c
	clang -c -o ipc.o -Wall ipc.c

.PHONY: clean run debug
clean:
	-rm $(filename) $(objects)

run: $(filename)
	./$(filename)

debug:
	clang -c -o ipc.o -Wall -g ipc.c
	clang -o $(filename) -Wall -g ipc.o

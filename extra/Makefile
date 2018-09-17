filename = ipc_extra
objects = ipc_extra.o

ipc_extra: $(objects)
	clang -o ${filename} -Wall $(objects)

ipc_extra.o: ipc_extra.c
	clang -c -o ipc_extra.o -Wall ipc_extra.c

.PHONY: clean run debug
clean:
	-rm $(filename) $(objects)

run: $(filename)
	./$(filename)

debug:
	clang -c -o ipc_extra.o -Wall -g ipc_extra.c
	clang -o $(filename) -Wall -g ipc_extra.o

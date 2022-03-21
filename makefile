all:  shell server
shell: shell.o client.a client.h
	gcc -Wall -g -o shell shell.o client.a
client:client.a

client.a: client.o 
	ar -rcs client.a client.o
server:
	gcc server.c -o server
client.o: client.c client.h
	gcc -Wall -g -c client.c
shell.o: shell.c client.h
	gcc -Wall -g -c shell.c 

.PHONY: clean#.PHONY means that clean is not a file.
clean:
	rm -f *.o *.a  shell server
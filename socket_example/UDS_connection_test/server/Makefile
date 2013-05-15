OBJECT = server.o

server : $(OBJECT) 
	gcc -o server -lpthread $(OBJECT)
server.o : server.c
	gcc -c server.c

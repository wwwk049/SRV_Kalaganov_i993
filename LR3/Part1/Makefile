all: client server

client: client.c
	gcc -o client client.c
server: server.c
	gcc -o server server.c
server_debug: server.c 
	gcc -DDEBUG=1 -o server server.c
clean:
	rm -f server client

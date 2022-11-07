all:
	mkdir -p build
	gcc -c client.c -o build/client.o
	gcc -c server.c -o build/server.o
	gcc -c error.c -o build/error.o
	gcc -o client build/error.o build/client.o
	gcc -o server build/error.o build/server.o
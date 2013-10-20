all: cliente server

cliente:
	gcc -c cliente/main.c
	gcc cliente/main.o -o cliente
	
executa_cliente:
	./cliente
	
server:
	gcc -c servidor/main.c
	gcc servidor/main.o -o servidor
	
executa_servidor:
	./servidor


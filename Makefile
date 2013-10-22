all: Servidor Cliente

Servidor:
	cd server && make
	mv server/servidor servidor

executa_servidor:
	./servidor

Cliente:
	cd client && make
	mv client/cliente cliente
	
executa_cliente:
	./cliente

comita:
	git add .
	git cia "$(TXTCOMIT)"
	git push origin master
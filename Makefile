all: server client

server:
	cd servidor && make

executa_servidor:
	cd servidor && ./servidor

client:
	cd cliente && make
	
executa_client:
	cd cliente && ./cliente
comita:
	git add .
	git cia "$(TXTCOMIT)"
	git push origin master
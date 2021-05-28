all: encrypt

encrypt: encrypt.c
	gcc encrypt.c -L/usr/lib -lssl -lcrypto -ldl -lpthread -o encrypt
clean:
	rm encrypt
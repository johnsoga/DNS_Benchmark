CC=gcc

default: prog

get-deps:
	sudo apt-get update
	sudo apt-get install -y build-essential

.DELETE_ON_ERROR:
dns.o: dns.c
	gcc -c dns.c

prog: dns.o
	gcc -o dns dns.o
	./dns ipv4_public.txt

clean:
	rm *.o

cleanall: clean
	rm dns

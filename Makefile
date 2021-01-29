CC=gcc

default: prog

get-deps:
	sudo apt-get update
	sudo apt-get install -y build-essential

.DELETE_ON_ERROR:
dns.o: dns.c data_struct.c
	gcc -Wvisibility -c dns.c data_struct.c

prog: dns.o data_struct.o
	gcc -Wvisibility -o dns dns.o data_struct.o
	./dns ipv4_public.txt

clean:
	rm *.o

cleanall: clean
	rm dns

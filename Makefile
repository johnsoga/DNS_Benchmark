CC=gcc

default: prog

get-deps:
	sudo apt-get update
	sudo apt-get install -y build-essential

.DELETE_ON_ERROR:
dns.o: dns.c ds.c
	gcc -c dns.c ds.c

prog: dns.o ds.o
	gcc -o main dns.o ds.o
	./main ipv4_public.txt

clean:
	rm *.o

cleanall: clean
	rm main

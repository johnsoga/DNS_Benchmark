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

check_ds.o: ./tests/check_ds.c ds.c
	gcc -c ./tests/check_ds.c ds.c

test: ds.o check_ds.o
	gcc ds.o check_ds.o -lcheck -lm -lpthread -lrt -lsubunit -o check_ds_tests
	./check_ds_tests

clean:
	rm *.o

cleanall: clean
	rm main

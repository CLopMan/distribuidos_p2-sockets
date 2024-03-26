
OBJ=cliente.o servidor.o imp_clave.o libclaves.so
FLAGS=-g -Wall -lrt
LIB=-L. -Wl,-rpath=.
SRC=src/
EXTRA_CLIENTS=gcc $(FLAGS) $(LIB) -o clientes $(SRC)cliente_set.c libclaves.so gcc $(FLAGS) $(LIB) -o clienteg $(SRC)cliente_get.c libclaves.so common.o
PORT=4500
IP=localhost

.PHONY: all clean DonLimpio testing

all: $(OBJ)
	@printf "\n\033[;33m\033[1mCOMPILING: GENERATING 2 FILES...\033[0m\n"
	gcc $(FLAGS) $(LIB) -o cliente cliente.o libclaves.so
	gcc $(FLAGS) -o servidor servidor.o imp_clave.o common.o
	@printf "\n\033[;32m\033[1mSUCCESS\033[0m\n"
	@printf "USAGE\n-----\n\t1. First run ./servidor\n\t2. Second, run every client you need (./cliente is given as an example)\nruning an init() it is recommended if first time.\n"

cliente.o: $(SRC)cliente.c 
	@echo "compiling client..."
	gcc -c $<

servidor.o: $(SRC)servidor.c common.o
	@echo "compiling server..."
	gcc -c $(SRC)servidor.c

imp_clave.o: $(SRC)imp_clave.c
	@echo "compiling services..."
	gcc -c $<

common.o: $(SRC)common.c
	@echo "compiling common..."
	gcc -c -fPIC $<

libclaves.so: $(SRC)claves.c common.o
	@echo "compiling tuple..."
	gcc -c -fPIC $(SRC)claves.c
	gcc -shared -o libclaves.so claves.o common.o

testing: imp_clave.o tests/tests_imp.c tests/custom_client.c libclaves.so tests/traductor.c
	gcc -o tests_imp tests/tests_imp.c imp_clave.o
	gcc $(FLAGS) -o tests_concurrency tests/tests_concurrency.c 
	gcc $(FLAGS) $(LIB) -o custom_client tests/custom_client.c libclaves.so
	gcc $(FLAGS) -o traductor tests/traductor.c
	@bash tests/test_imp.sh
	@env IP_TUPLAS=localhost PORT_TUPLAS=4500 ./tests_concurrency
	@killall servidor
clean:
	rm *.o

DonLimpio:
	rm *.o cliente servidor *.so tests_imp traductor tests_concurrency *.txt custom_client



OBJ=cliente.o servidor.o imp_clave.o libclaves.so
FLAGS=-g -Wall -lrt
LIB=-L. -Wl,-rpath=.
SRC=src/
EXTRA_CLIENTS=gcc $(FLAGS) $(LIB) -o clientes $(SRC)cliente_set.c libclaves.so gcc $(FLAGS) $(LIB) -o clienteg $(SRC)cliente_get.c libclaves.so

.PHONY: all clean DonLimpio testing

all: $(OBJ)
	@echo -e "\n\0033[;33m\033[1mCOMPILING: GENERATING 2 FILES...\033[0m\n"
	gcc $(FLAGS) $(LIB) -o cliente cliente.o libclaves.so
	gcc $(FLAGS) -o servidor servidor.o imp_clave.o
	@echo -e "\n\033[;32m\033[1mSUCCESS\033[0m\n"
	@echo -e "USAGE\n-----\n\t1. First run ./servidor\n\t2. Second, run every client you need (./cliente is given as an example)\nruning an init() it is recommended if first time."

cliente.o: $(SRC)cliente.c 
	@echo "compiling client..."
	gcc -c $<

servidor.o: $(SRC)servidor.c 
	@echo "compiling server..."
	gcc -c $<

imp_clave.o: $(SRC)imp_clave.c
	@echo "compiling services..."
	gcc -c $<

libclaves.so: $(SRC)claves.c
	@echo "compiling tuple..."
	gcc -c -fPIC -shared -o libclaves.so $<

testing: imp_clave.o tests/tests_imp.c tests/custom_client.c libclaves.so tests/traductor.c
	gcc -o tests_imp tests/tests_imp.c imp_clave.o
	gcc $(FLAGS) -o tests_concurrency tests/tests_concurrency.c 
	gcc $(FLAGS) $(LIB) -o custom_client tests/custom_client.c libclaves.so
	gcc $(FLAGS) -o traductor tests/traductor.c
	@bash tests/test_imp.sh
	@./tests_concurrency

clean:
	rm *.o

DonLimpio:
	rm *.o cliente servidor *.so tests_imp traductor tests_concurrency *.txt custom_client


CC   = gcc
LIBS = -lpthread
DEPS = includes.h ../Shared\ Libraries/communication.h
EXEC = ./server_linux
SRC  = $(wildcard *.c)


$(EXEC) : $(SRC)
	$(CC) -o $(EXEC) $(SRC) $(DEPS) $(LIBS)

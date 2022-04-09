CC=clang
INC=
LIB=
CFLAGS=-Wall

BIN=stax
SRC=$(wildcard src/*.c)
HDR=$(wildcard src.*.h)

all: $(BIN)

clean:
	rm *.o

# one-shot build rule, will rebuild all for any change
$(BIN): $(SRC) $(HDR)
	$(CC) $(SRC) -o $(BIN) $(INC) $(LIB) $(CFLAGS)

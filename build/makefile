IDIR = ../include
CC = gcc
CFLAGS = -I$(IDIR) -g -lm -lcjson

ODIR = obj
SDIR = ../src
CJSON_DIR = ../cJSON

_DEPS = emulator.h
DEPS = $(patsubst %, $(IDIR)/%, $(_DEPS))

_OBJ = main.o emulator.o cJSON.o
OBJ = $(patsubst %, $(ODIR)/%, $(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	@mkdir -p $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o main

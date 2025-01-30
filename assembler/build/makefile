IDIR = ../include
CC = gcc
CFLAGS = -I$(IDIR) -g -lm

ODIR = obj
SDIR = ../src

_DEPS = assembler.h
DEPS = $(patsubst %, $(IDIR)/%, $(_DEPS))

_OBJ = main.o assembler.o
OBJ = $(patsubst %, $(ODIR)/%, $(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	@mkdir -p $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS)

asp: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o main

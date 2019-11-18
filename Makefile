cc = gcc
cflags = -Wall -std=c11 -pedantic -ggdb
prog1 = serverb
prog2 = clientb
source1 = server.c
source2 = client.c

#compile
.PHONY: all
all: $(prog1) $(prog2)

$(prog1) : $(source1)
	$(cc) $(cflags) $(source1) -o $(prog1)

$(prog2) : $(source2)
	$(cc) $(cflags) $(source2) -o $(prog2) 
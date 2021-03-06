CC = gcc
FLAGS := -g -pthread -lrt -O2 -Wpedantic -Wall  -std=c11
OUT = prog
OUT2 = prog2

all: $(OUT) $(OUT2) first second

$(OUT): main1.c
	$(CC) $(FLAGS) -o $(OUT) $<

$(OUT2): main2.c
	$(CC) $(FLAGS) -o $(OUT2) $<

clean:
	rm -fv prog* 

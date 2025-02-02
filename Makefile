.PHONY: all clean

all: pass-gen
clean:
	rm -f pass-gen

pass-gen: main.c eff_word_list.h
	gcc $< -o $@ -O3 -Wall -Wextra -Wpedantic -std=c99

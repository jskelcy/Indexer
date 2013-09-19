all: tokenizer2.c
	gcc -g -o tokenizer tokenizer2.c
clean:
	rm tokenizer

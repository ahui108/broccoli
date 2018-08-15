all: pthread.bin
	@echo "Done"

pthread.bin: pthread.c
	gcc -o $@ $? -lpthread

clean:
	rm -f pthread.bin

main.bin: main.o cHeader.o
	g++ -o main.bin main.o cHeader.o

cHeader.o: cHeader.c
	gcc -c -o cHeader.o cHeader.c

main.o: main.cpp
	g++ -c -o main.o main.cpp

pohny: clean

clean:
	rm -f main.bin main.o cHeader.o

all: fifo_writer fifo_reader
	echo "Done"

fifo_reader: fifo_reader.cpp
	g++ -o fifo_reader fifo_reader.cpp

fifo_writer: fifo_writer.cpp
	g++ -o fifo_writer fifo_writer.cpp

clean:
	rm -f fifo_reader fifo_reader.o fifo_writer fifo_writer.o

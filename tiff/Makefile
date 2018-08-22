all: tif_format.bin
	echo "Done"

tif_format.bin: tif_format.cpp
	g++ -g -o tif_format.bin tif_format.cpp

clean:
	rm -rf tif_format.bin tif_format.o

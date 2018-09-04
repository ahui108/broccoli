all: tiff_templ.bin
	echo "Done"

tif_format.bin: tif_format.cpp
	g++ -g -o tif_format.bin tif_format.cpp

tiff_templ.bin: tiff_templ.cpp
	g++ -g -o tiff_templ.bin tiff_templ.cpp

clean:
	rm -rf tif_format.bin tif_format.o tiff_templ.o tiff_templ.bin

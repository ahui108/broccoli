all: print-wch.bin print-ch.bin charset_conv.bin code_convert.bin
	echo "make done!"

code_convert.bin: code_convert.cpp
	g++ -o code_convert.bin code_convert.cpp

print-wch.bin: print-wch.cpp
	g++ -o print-wch.bin print-wch.cpp

print-ch.bin: print-ch.cpp
	g++ -o print-ch.bin print-ch.cpp

charset_conv.bin: charset_conv.cpp
	g++ -o charset_conv.bin charset_conv.cpp

.phony: clean

clean:
	rm -f print-wch.o print-ch.o charset_conv.o print-wch.bin print-ch.bin charset_conv.bin code_convert.o code_convert.bin

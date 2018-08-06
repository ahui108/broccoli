all: udp_server.bin udp_client.bin
	echo "Done"

udp_server.bin: udp_server.cpp
	g++ -o udp_server.bin udp_server.cpp

udp_client.bin: udp_client.cpp
	g++ -o udp_client.bin udp_client.cpp

clean:
	rm -f udp_server.o udp_server.bin udp_client.bin udp_client.o

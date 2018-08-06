all: udp_client.bin udp_server.bin
	echo "Done"

udp_client.bin: udp_client.cpp
	g++ -o udp_client.bin udp_client.cpp

udp_server.bin: udp_server.cpp
	g++ -o udp_server.bin udp_server.cpp

clean:
	rm -f udp_server.bin udp_server.o udp_client.bin udp_client.o

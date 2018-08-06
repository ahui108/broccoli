all: shm_cli shm_srv
	echo "done"

shm_cli: shm_cli.cpp
	g++ -o shm_cli shm_cli.cpp

shm_srv: shm_srv.cpp
	g++ -o shm_srv shm_srv.cpp

clean:
	rm -f shm_cli shm_cli.o shm_srv shm_srv.o

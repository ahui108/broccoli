all: libLowerLayer.so libUpperLayer.so main.cpp
	g++ main.cpp -o main -lUpperLayer -L . -Wl,-rpath .
	@echo "done!"

libUpperLayer.so: upperLayer/upperLayer.cpp
	g++ upperLayer/upperLayer.cpp -o $@ -fPIC --shared -lLowerLayer -L . -Wl,-rpath .

libLowerLayer.so: lowerLayer/lowerLayer.cpp
	g++ lowerLayer/lowerLayer.cpp -o $@ -fPIC --shared

.PHONY:clean
clean:
	-rm *.so main





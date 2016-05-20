#include<cstdlib>
#include<iostream>
#include<exception>

int main(int argc, char ** argv){

	int a = 10;
	int b = 0;
	try{
		int c = a/b;
	}catch(std::exception e){
		std::cout << e.what() << std::endl;
	}catch(...){
		std::cout << "exception caught!" << std::endl;
	}

	std::cout << "Exiting ..." << std::endl;
	std::exit(0);
}

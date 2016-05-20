#include<iostream>
#include<cstdio>
#include<cstring>

char A[] = {'s', 't', 'r', 'i', 'n', 'g', 'A'};//7 elements

int main(int argc, char ** argv){
	std::cout << "strlen(A) = " << strlen(A) << std::endl; //7, string itslef, not including the \0
	printf("A=\"%s\"\n", A);
	printf("sizeof(A)=%d\n\n", sizeof(A));//7*1

	char * B = "stringB";
	std::cout << "strlen(B) = " << strlen(B) << std::endl;	
	printf("B=\"%s\"\n", B);
	printf("sizeof(B)=%d\n\n", sizeof(B));

	char C[] = "stringC";
	std::cout << "strlen(C) = " << strlen(C) << std::endl; //strlen doesn't include the last null char
	printf("C=\"%s\"\n", C);
	printf("sizeof(C)=%d\n\n", sizeof(C));
}



#include<iostream>

//void bubblesort(int (&arrInt)[8]){
void bubblesort(int * arrInt, int len){
	/*int len = sizeof(arrInt)/sizeof(int);
	std::cout << "len = " << len << std::endl;
	if (len <=0 ){
		return;
	}
	*/
	
	if ((len <= 0) || (arrInt == NULL)) {
		return;
	}

	int iTemp = 0;
	for( int i = 0; i < len; i ++ ){
		for( int j = len - 1; j > i; j -- ){
			if (arrInt[j] > arrInt[j-1]){
				iTemp = arrInt[j];
				arrInt[j] = arrInt[j-1];
				arrInt[j-1] = iTemp; 
			}//end if
		}//end for
	}//end for
}//end 

int main(int argc, char ** argv){
	int arrInt[] = {4, 2, 7, 10, 5, 1, 3, 11};
	std::cout << "Before bubble sort --" << std::endl;
	for ( int i = 0; i < sizeof(arrInt)/sizeof(int); i ++ ){
		std::cout << arrInt[i] << ", ";
	}
	std::cout << std::endl;

	//bubblesort(arrInt);	
	bubblesort(arrInt, 8);	
	std::cout << "After bubble sort --" << std::endl;
	for ( int i = 0; i < sizeof(arrInt)/sizeof(int); i ++ ){
		std::cout << arrInt[i] << ", ";
	}
	std::cout << std::endl;
}

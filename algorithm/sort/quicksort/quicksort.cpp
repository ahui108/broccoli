#include<iostream>

int partion(int * A, int p, int r);
void exchange(int & a, int & b);
void print(int * A, int len);

void quicksort(int * A, int p, int r){
	if (p < r){
		int q = partion(A, p, r);
		quicksort(A, p, q -1);
		quicksort(A, q + 1, r);
	}	
}

int partion(int * A, int p, int r)
{
	int x = A[r];
	int i = p - 1;//'i' save the item which is less than 'x'. 'j' save the item which is greater than 'x'
	for (int j = p; j <= r - 1; j ++){
		if (A[j] <= x){//exchange 'i' and 'j'
			i++;
			exchange(A[i], A[j]);	
		}
	}
	
	exchange(A[i+1], A[r]);

	return i + 1;
}


void print(int * A, int len){
	if ((A != NULL) && (len > 0)){
		std::cout << A[0];
		for (int i = 1; i < len; i ++){
               		std::cout << "," << A[i];
        	}
		std::cout << std::endl;
	}
}

void exchange(int & a, int & b){
	int temp = a;
	a = b;
	b = temp;
}

int main(int argc, char ** argv){
	int A[] = {2, 8, 7, 1, 3, 5, 6, 4};
	std::cout << "**Before sort>>" << std::endl;
	print(A, 8);
	quicksort(A, 0, 7);	
	std::cout << "**After sort>>" << std::endl;
	print(A, 8);
}

#include<cstring>
#include<cstdio>

int bisearch(char * arr, int b, int e, char v){
	int minIndex = b, maxIndex = e, midIndex = -1;
	int vIndex = -1;
	while(minIndex < maxIndex){
		midIndex = (minIndex + maxIndex) / 2;
		char cMid = arr[midIndex];
		if (minIndex == midIndex){
			break;
		}
		if (cMid == v){
			vIndex = midIndex;
			minIndex = midIndex;
		} else if (cMid < v) {
			minIndex = midIndex;
		} else {
			maxIndex = midIndex;
		}
	}

	return vIndex;
}

int main(int argc, char ** argv){
	char arr[] = {'1', '2', '4', '6', '7', '8', '8', '9'};
	printf("The char arr:");
	for(int i = 0; i < 8; i++){
		printf("%c,", arr[i]);
	}
	int iIndex = bisearch(arr, 0, 7, '8');
	printf("the index of max item:%d, and the item is %c", iIndex, arr[iIndex]);
	
	return 0;
}

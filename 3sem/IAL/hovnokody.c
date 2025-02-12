#include <stdio.h>

void bubbleInsert(int arr[], int len) {

	for (int i = 1; i < len; i++) {

		int tmp = arr[i];
		j = i -1;

		while (j >= 0 && tmp < arr[j]) {

			arr[j+1] = arr[j];
			j -= 1;

		}
		arr[j+1] = tmp;

	}


}

void binaryInsert(int arr[], int len) {

	for (int i = 1; i < len; i++) {

		int tmp = arr[i];
		int left = 0;
		int right = i - 1;

		while (left <= right) {
			int m = (left+right) / 2;
			if (tmp < arr[m]) {

				right = m-1;
			} else {
				left = m+1;
			}

		}
		for (int j = i-1; j < left; j++) {
			arr[j+1] = arr[j];
		}

		arr[left] = tmp;


	}


}

void shellSort (int arr[], int len) {
	int step = len / 2;
	while (step > 0) {
	
		for (int i = step; i < len; i++) {
		
			int j = i-step;
			while (j >= 0 && arr[j] > arr[j+step]) {
				
				swap(arr[j], arr[j+step]);
				j -= step;
			
			}
		}
		step = step / 2;
	
	}
}


void mergeSort (int arr[], int left, int right) {
	
	if (left < right) {
		int q = (left+right) / 2;
		mergeSort(arr, left, q);
		mergeSort(arr, q+1, right);
		Merge(arr, left, q, right);

	}

} 

void recQuickSort(int arr[], int left, int right) {

}



int, int partition1 (int arr[], int left, int right) {
	

}



int main () {

	int arr[7] = {2,0,6,1,8,7,3};
	shellSort(arr, 7);
}

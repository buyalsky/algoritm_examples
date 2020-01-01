#include<stdio.h>
#include<stdlib.h>
#include <math.h>
#include<string.h>
typedef struct node N;
typedef struct data D;
struct data {
	int index1, index2;
	double distance;
};

struct node{
	int x, y;//x axis and y axis
};
N nodes[100];//array that stores location of points

//eucledian distance between given two points
double eucledian_distance(N node1, N node2) {
	return sqrt((node1.x - node2.x) * (node1.x - node2.x) + (node1.y - node2.y) * (node1.y - node2.y));
}

D find_min_interval(N arr[], int arr2[], int n, double median,double min_distance) {
	D min_node;
	if (n < 2) {
		min_node.index1 = 0;
		min_node.index2 = 0;
		min_node.distance = 999999,0;
		return min_node;
	}
	int i,j;
	if (n <= 3) {
	min_node.index1 = arr2[0];
	min_node.index2 = arr2[1];
	min_node.distance = eucledian_distance(arr[min_node.index1], arr[min_node.index2]);
		for (i = 0; i < n - 1; i++)
			for (j = i + 1;j < n; j++) {
				if (eucledian_distance(arr[arr2[i]], arr[arr2[j]]) < min_node.distance) {
					min_node.index1 = arr2[i];
					min_node.index2 = arr2[j];
				}
		}
		return min_node;
	}
	min_node.index1 = 0;
	min_node.index2 = 0;
	min_node.distance = min_distance;
	for(i=0;i<n-1;i++)
		for (j = i + 1; j < n ; j++) {
			if (fabs(arr[arr2[j]].y - arr[arr2[i]].y) < min_distance &&
				((arr[arr2[i]].x <= median && arr[arr2[j]].x >= median) ||
				(arr[arr2[j]].x <= median && arr[arr2[i]].x >= median)) && 
				eucledian_distance(arr[arr2[i]], arr[arr2[j]]) < min_node.distance) {
				min_node.index1 = arr2[i];
				min_node.index2 = arr2[j];
				min_node.distance = eucledian_distance(arr[arr2[i]], arr[arr2[j]]);
			}
		}
	return min_node;
}

D divide_and_conquer(N arr[], int l, int r) {
	D min_node;
	min_node.index1 = 0;
	min_node.index2 = 0;
	min_node.distance = 0,0;
	double median;
	//calculate median
	if ((l + r) % 2==0)
		median = (double)nodes[(l + r) / 2].x;
	else
		median = ((double)nodes[(l + r) / 2].x + nodes[(int)ceil((double)(l + r) / 2)].x) / 2;
	
	//check whether the # of points above 3
	if (r - l > 2){
		//if # of points is odd
		if ((r - l) % 2 == 0) {
			printf("dividing into two %d-%d and %d-%d (left right respectively)\n", l, ((l + r) / 2) - 1, 1 + (l + r) / 2, r);
			D min_left = divide_and_conquer(arr, l, ((l + r) / 2) - 1);
			printf("closest pair between %d and %d: %d and %d distance: %f\n", l, ((l + r) / 2) - 1,min_left.index1,min_left.index2,min_left.distance);
			D min_right = divide_and_conquer(arr, 1 + (l + r) / 2, r);
			printf("closest pair between %d and %d: %d and %d distance: %f\n", 1 + (l + r) / 2, r, min_right.index1, min_right.index2, min_right.distance);
			if (min_left.distance > min_right.distance)
				min_node = min_right;
			else

				min_node = min_left;
		}
		//çift sayýda eleman varsa
		else {
			printf("dividing into two %d-%d and %d-%d (left right respectively)\n", l, (l + r) / 2, 1 + (l + r) / 2, r);
			D min_left = divide_and_conquer(arr, l, (l + r) / 2);//closest pair in left side
			printf("closest pair between %d and %d: %d and %d distance: %f\n", l, ((l + r) / 2), min_left.index1, min_left.index2, min_left.distance);
			D min_right = divide_and_conquer(arr, 1 + (l + r) / 2, r);//closest pair in right side
			printf("closest pair between %d and %d: %d and %d distance: %f\n", 1 + (l + r) / 2, r, min_right.index1, min_right.index2, min_right.distance);
			if (min_left.distance > min_right.distance)
				min_node = min_right;
			else
				min_node = min_left;
		}
		int j=0,i;	
		int arr2[50];

		//find points around median line
		for (i = l; i <= r;i++) {
			if (arr[i].x >=median - min_node.distance 
				&& arr[i].x <=median + min_node.distance) {
				arr2[j] = i; j++;
			}
		}
		printf("between %d %d, %d poins exist, median= %f\n", l, r, j,median);
		
		D min_node_interval=find_min_interval(arr,arr2,j,median,min_node.distance);
		if (min_node.distance > min_node_interval.distance)
			min_node = min_node_interval;
	}
	
	else if (r-l==1){
		min_node.index1 = r;
		min_node.index2 = l;
		min_node.distance=eucledian_distance(arr[min_node.index1], arr[min_node.index2]);
	}
	//if 3 points exists we use brute-force approach
	else if (r - l == 2) {
		min_node.index1 = l; min_node.index2 = l + 1;
		min_node.distance = eucledian_distance(arr[min_node.index1], arr[min_node.index2]);
		int i, j;
		for (i = l; i < r; i++)
			for (j = i + 1; j <= r; j++) {
				if (eucledian_distance(arr[i], arr[j]) < min_node.distance) {
					min_node.distance = eucledian_distance(arr[i], arr[j]);
					min_node.index1 = i; min_node.index2 = j;
				}
			}
	}
	return min_node;
	}
	

void merge(N* arr, int l, int m, int r){
	int i, j, k;
	int n1 = m - l + 1;
	int n2 = r - m;
	N L[100], R[100];

	for (i = 0; i < n1; i++)
		L[i] = arr[l + i];
	for (j = 0; j < n2; j++)
		R[j] = arr[m + 1 + j];
	i = 0; 
	j = 0; 
	k = l; 
	while (i < n1 && j < n2){
		if (L[i].x <= R[j].x){
			arr[k] = L[i];
			i++;
		}
		else{
			arr[k] = R[j];
			j++;
		}
		k++;
	}
	while (i < n1){
		arr[k] = L[i];
		i++;
		k++;
	}
	while (j < n2){
		arr[k] = R[j];
		j++;
		k++;
	}
}

void mergeSort(N* arr, int l, int r){
	if (l < r){
		int m = l + (r - l) / 2;
		mergeSort(arr, l, m);
		mergeSort(arr, m + 1, r);
		merge(arr, l, m, r);
	}
}

int main(void) {
	int i;
	int size = 0;
	FILE* fp=fopen("input.txt", "r");
	char buff[20], *number;
	if (fp == NULL) {
		printf("Could not open file input.txt");
		return 1;
	}
	//reading the file input.txt
	while (fgets(buff, sizeof(buff), fp) != NULL && strlen(buff)>1) {
		number = strtok(buff, " ");
		nodes[size].x = atoi(number);
		number = strtok(NULL, "\n");
		nodes[size].y = atoi(number);
		size++;
	}
	fclose(fp);

	//sort points according to their x coordinates
	mergeSort(nodes,0, size-1);
	printf("number of points %d\n",size);
	
	for (i = 0; i < size; i++) {
		printf("  %d  %d  %d\n",i, nodes[i].x, nodes[i].y);
	}
	D min_node;
	min_node = divide_and_conquer(nodes, 0, size-1);
	printf("Closest pair (%d,%d) and (%d,%d) distance: %f", nodes[min_node.index1].x, nodes[min_node.index1].y,
		nodes[min_node.index2].x,nodes[min_node.index2].y,min_node.distance);
	return 0;
}

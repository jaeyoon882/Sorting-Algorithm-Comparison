#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_HEAP_ELEMENTS 1000000

typedef struct {
	char* str;
	int id;
} Element;

typedef struct {
    char alg_name[30];
    double time;
    long long compare;
    long long swap;
    int stable;
} SortResult;

long long comp_count,swap_count;

char *generate_string();
void copy_data(Element *temp, Element *list,int size);

int check_stability(Element *list, int size);

void run_sort(char *name, Element *list,int size, void (*fp)(Element*,int),SortResult *result);

void bubble_sort(Element *list,int size);
void insertion_sort(Element *list,int size);

void quick_sort_interface(Element* list,int size);
int partition(Element* list, int left,int right);
void quick_sort(Element *list,int left,int right);


void merge_sort_interface(Element* list, int size);
void merge(Element *list, int left, int mid, int right);
void merge_sort(Element *list,int left, int right);

void heap_sort(Element *list,int size);

void quick_sort_after_interface(Element *list, int size); 
int partition_after(Element* list, int left, int right);
void quick_sort_after(Element *list, int left, int right);



void heap_sort_after(Element *list, int size);

int str_cmp(const char* str1,const char* str2);
int cmp_after(Element a,Element b);

typedef struct {
	Element heap[MAX_HEAP_ELEMENTS];
	int heap_size;
}HeapType;

HeapType* create();
void init(HeapType* h);
void insert_max_heap(HeapType *h, Element item);
Element delete_max_heap(HeapType *h);

void insert_max_heap_after(HeapType *h, Element item);
Element delete_max_heap_after(HeapType *h);

int main() {
	srand(time(NULL));
	int n = 10000;
	SortResult results[7];
	printf("Number of instances: %d\n",n);	
	Element *list = (Element *)malloc(sizeof(Element)*n);
	for(int i=0;i<n;i++) {
		list[i].str = generate_string();
		list[i].id = i;
	}
	run_sort("bubble_sort",list,n,bubble_sort,results);
	run_sort("insertion_sort",list,n,insertion_sort,results+1);
	run_sort("merge_sort",list,n,merge_sort_interface,results+2);
	run_sort("quick_sort",list,n,quick_sort_interface,results+3);
	run_sort("heap_sort",list,n,heap_sort,results+4);
	run_sort("quick_sort_after",list,n,quick_sort_after_interface,results+5);
	run_sort("heap_sort_after",list,n,heap_sort_after,results+6);


	printf("==== Sorting Result Summary ===\n");
    	printf("Algorithm            | Time(s) | Compare   | Swap      | Stable \n");
    	printf("-------------------------------------------------------------------\n");

	

    	for(int i=0; i<5; i++) {
        	printf("%-20s | %-7.3f | %-9lld | %-9lld | %s \n", 
            	results[i].alg_name, 
            	results[i].time, 
            	results[i].compare, 
            	results[i].swap, 
            	results[i].stable ? "YES" : "NO");
    	}
	printf("-------------------------------------------------------------------\n");

	for(int i=5;i<7;i++) {	
        	printf("%-20s | %-7.3f | %-9lld | %-9lld | %s \n", 
            	results[i].alg_name, 
            	results[i].time, 
            	results[i].compare, 
            	results[i].swap, 
            	results[i].stable ? "YES" : "NO");
	}

 
   	 printf("-------------------------------------------------------------------\n");
	
	for(int i=0;i<n;i++) free(list[i].str);
	free(list);
	return 0;
}

char* generate_string() {
    const char charset[] = "abcdefghijklmnopqrstuvwxyz";
    int charset_size = strlen(charset);

    int length = (rand()%20+1);
    char* random_string = (char *)malloc(sizeof(char)*(length+1));
    for (int i = 0; i < length; i++) {
        random_string[i] = charset[rand() % charset_size];
    }
    random_string[length]='\0';
    return random_string;
}

void copy_data(Element *temp, Element *list,int size) {
	for(int i=0;i<size;i++) {
		temp[i] = list[i];
	}
}

int check_stability(Element *list,int size) {
	for(int i=0;i<size-1;i++) {
		if(str_cmp(list[i].str,list[i+1].str)==0) {
			if(list[i].id >list[i+1].id)
				return 0;
		}
	}
	return 1;
}

void run_sort(char* name,Element* list, int size, void (*fp)(Element*, int),SortResult *result) {
	printf("=> Starting -%s\n",name);
	Element *temp = malloc(sizeof(Element)*size);
	copy_data(temp,list,size);
	
	clock_t start, end;
	double duration;
	comp_count = 0, swap_count = 0;
	int is_stable = 0;
	
	start = clock();
	(*fp)(temp,size);
	end = clock();
	printf("=> Finished\n");
	duration = (double)(end - start)/CLOCKS_PER_SEC;
	
	is_stable = check_stability(temp,size);
	
	char filename[50];
	sprintf(filename,"%s.out",name);
	FILE *output = fopen(filename,"w");
	for(int i=0;i<size;i++) {
		fprintf(output,"%s\n",temp[i].str);
	}
	printf("=>Created - %s\n",filename);
	strcpy(result->alg_name,name);
	result->time = duration;
	result->compare = comp_count;
	result->swap = swap_count;
	result->stable = is_stable;

	fclose(output);
	free(temp);
}

int str_cmp(const char* str1,const char* str2) {
	comp_count++;
	while( (*str1 == *str2) && *str1 != '\0') {
		str1++;
		str2++;
	}
	
	if(*str1>*str2) return 1;
	if(*str1<*str2) return -1;

	return 0;//equal
}


void bubble_sort(Element* list,int size) {
	int i,j;
	Element temp;
	for(int i=size-1;i>0;i--) {
		for(j=0;j<i;j++) {
			if(str_cmp(list[j].str,list[j+1].str)==1) {
				temp = list[j];
				list[j] = list[j+1];
				list[j+1] = temp;
				swap_count++;		
			}
		}
	}
}

void insertion_sort(Element *list, int size) {
	int i,j;
	Element key;
	for(int i=0;i<size;i++) {
		key = list[i];
		comp_count++;
		for(j=i-1;j>=0 && str_cmp(list[j].str,key.str)==1;j--) {
			list[j+1] = list[j];
			swap_count++;
		}
		list[j+1] = key;
		swap_count++;
	}
}

void merge(Element* list, int left, int mid, int right) {
	int i,j,k,l;
	i=left; j=mid+1; k=left;
	Element *temp = (Element *)malloc(sizeof(Element)*(right+1));
	while(i<=mid && j<=right) {
		if(str_cmp(list[i].str,list[j].str)==1)
			temp[k++] = list[j++];
		else
			temp[k++] = list[i++];
		swap_count++;
	}
	if(i>mid) {
		for(l=j;l<=right;l++){
			temp[k++] = list[l];
			swap_count++;
		}
	}
	else {
		for(l=i;l<=mid;l++) {
			temp[k++] = list[l];
			swap_count++;
		}
	}
	
	for(l=left;l<=right;l++) {
		list[l] = temp[l];
		swap_count++;
	}
	free(temp);
}

void merge_sort(Element *list,int left, int right) {
	int mid;
	if(left<right) {
		mid = (left+right)/2;
		merge_sort(list,left,mid);
		merge_sort(list,mid+1,right);
		merge(list,left,mid,right);
	}

}

void merge_sort_interface(Element* list, int size) {
	merge_sort(list,0,size-1);
}


void quick_sort_interface(Element *list, int size) {
	quick_sort(list,0,size-1);
}

int partition(Element* list, int left, int right) {
	Element pivot;
	int low, high;
	Element temp;

	low = left;
	high = right+1;
	pivot = list[left];
	do {
		do {
			low++;
		}while(low<=right && str_cmp(list[low].str,pivot.str)==-1);

		do {
			high--;
		}while(high>=left && str_cmp(list[high].str,pivot.str)==1);
		if(low<high) {
			temp = list[low];
			list[low] = list[high];
			list[high] = temp;
			swap_count++;
		}
	}while(low<high);

	temp = list[left];
	list[left] = list[high];
	list[high] = temp;
	swap_count++;
	return high;
	
}

void quick_sort(Element *list, int left, int right) {
	if(left<right) {
		int q = partition(list,left,right);
		quick_sort(list,left,q-1);
		quick_sort(list,q+1,right);
	}	
}

HeapType* create() {
	return (HeapType*)malloc(sizeof(HeapType));
}

void init(HeapType *h) {
	h->heap_size=0;
}

void insert_max_heap(HeapType* h, Element item) {
	int i;
	i = ++(h->heap_size);

	while(i != 1 && (str_cmp(item.str,h->heap[i/2].str)==1)) {
		h->heap[i]=h->heap[i/2];
		swap_count++;
		i /= 2;
	}
	h->heap[i] = item;
	swap_count++;
}

Element delete_max_heap(HeapType* h) {
	int parent, child;
	Element item, temp;

	item = h->heap[1];
	temp = h->heap[(h->heap_size)--];
	parent = 1;
	child = 2;
	while(child <= h->heap_size) {
		if((child<h->heap_size)&&(str_cmp(h->heap[child+1].str,h->heap[child].str)==1)) {
			comp_count++;
			child++;
		}

		if(str_cmp(temp.str,h->heap[child].str)>=0)
			break;
		h->heap[parent] = h->heap[child];
		swap_count++;
		parent = child;
		child *= 2;
	}
	h->heap[parent] = temp;
	swap_count++;
	return item;
}

void heap_sort(Element* list,int size) {
	int i;
	HeapType *h;

	h = create();
	init(h);

	for(i=0;i<size;i++) {
		insert_max_heap(h,list[i]);
	}

	for(i = (size-1);i>=0;i--) { 
		list[i] = delete_max_heap(h);
	}
	free(h);
}

int cmp_after(Element a, Element b) {
	int str_result = str_cmp(a.str,b.str);
	
	if(str_result==0) {
		comp_count++;
		if(a.id > b.id) return 1;
		else return -1;
	}	

	return str_result;
}

void insert_max_heap_after(HeapType* h, Element item) {
	int i;
	i = ++(h->heap_size);

	while(i != 1 && (cmp_after(item,h->heap[i/2])==1)) {
		h->heap[i]=h->heap[i/2];
		swap_count++;
		i /= 2;
	}
	h->heap[i] = item;
	swap_count++;
}

Element delete_max_heap_after(HeapType* h) {
	int parent, child;
	Element item, temp;

	item = h->heap[1];
	temp = h->heap[(h->heap_size)--];
	parent = 1;
	child = 2;
	while(child <= h->heap_size) {
		if((child<h->heap_size)&&(cmp_after(h->heap[child+1],h->heap[child])==1)) {
			comp_count++;
			child++;
		}

		if(cmp_after(temp,h->heap[child])>=0)
			break;
		h->heap[parent] = h->heap[child];
		swap_count++;
		parent = child;
		child *= 2;
	}
	h->heap[parent] = temp;
	swap_count++;
	return item;
}


void heap_sort_after(Element* list, int size) {
	int i;
	HeapType *h;

	h = create();
	init(h);

	for(i=0;i<size;i++) {
		insert_max_heap_after(h,list[i]);
	}

	for(i = (size-1);i>=0;i--) { 
		list[i] = delete_max_heap_after(h);
	}
	free(h);
}


void quick_sort_after_interface(Element *list, int size) {
	quick_sort_after(list, 0, size-1);
}

int partition_after(Element* list, int left, int right) {
	Element pivot;
	int low, high;
	Element temp;

	low = left;
	high = right+1;
	pivot = list[left];
	do {
		do {
			low++;
		}while(low<=right && cmp_after(list[low],pivot)==-1);

		do {
			high--;
		}while(high>=left && cmp_after(list[high],pivot)==1);
		if(low<high) {
			temp = list[low];
			list[low] = list[high];
			list[high] = temp;
			swap_count++;
		}
	}while(low<high);

	temp = list[left];
	list[left] = list[high];
	list[high] = temp;
	swap_count++;
	return high;
	
}

void quick_sort_after(Element* list, int left, int right) {
	if(left<right) {
		int q = partition_after(list,left,right);
		quick_sort_after(list,left,q-1);
		quick_sort_after(list,q+1,right);
	}	
}

#include<stdio.h>
#include<math.h>

void swap(int *a,int *b){
	int temp=*a;
	*b=*a;
	*a=temp;
}

// int partition(int arr[],int lo,int hi){
// 	int pivot=arr[hi];
// 	int i=lo-1;
// 	for(int j=lo;j<=hi-1;j++){
// 		if(arr[j]<pivot){
// 			i++;
// 			swap(&arr[i],&arr[j]);
	
// 		}
// 	}
// 	swap(&arr[i+1],&arr[hi]);
// 	//int tempp=arr[i+1];
// 	//arr[i+1]=arr[hi];
// 	//arr[hi]=tempp;
// 	return (i+1);
	
	

// }
int partition(int arr[],int low,int high){
    int pivot=arr[high];
    int i=low-1;
    for(int j=low;j<=high-1;j++){
        if(arr[j]<pivot){
            i++;
            swap(&arr[i],&arr[j]);
        }
    }
    swap(&arr[i+1],&arr[high]);
    return (i+1);
}


// void quickSort(int arr[], int low, int high)
// {
// 	if(low<high){
// 	int pi=partition(arr,low,high);
// 	quickSort(arr,low,pi-1);
// 	quickSort(arr,pi+1,high);
// 	}
// }

void quickSort(int arr[],int low,int high){
    if(low<high){
        int pi=partition(arr,low,high);
        quickSort(arr,low,pi-1);
        quickSort(arr,pi+1,high);
    }
}

int main(){
    int arr[]={10,7,8,9,1,5};
    int n=sizeof(arr)/sizeof(arr[0]);
    quickSort(arr,0,n-1);
    for(int i=0;i<n;i++){
        printf("%d ",arr[i]);
    }
    return 0;
}

//void bubbleSort(int arr[],int n){
//
//	for (int i=0;i<n;i++){
//		for(int j=0;j<n-i-1;j++){
//			if (arr[j]>arr[j+1])
//			{
//			swap(&arr[j],&arr[j+1]);
//			}
//		}
//	}
//}



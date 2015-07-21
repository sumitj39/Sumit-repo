#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 2
#define NUM_ELE 3000 /* # of thread */

int num_iter=0;
int a[NUM_ELE];

typedef struct Arr {
    int low;
    int high;
} ArrayIndex;

void merge(int left, int right)

{
  int mid=(left+right)/2;
  int L[mid-left+1];
  int R[right-mid];
  int i,j,k;
  num_iter++;
  for(i=0;i<(mid-left+1);i++)
    L[i]=a[i+left];
  for(i=0;i<(right-mid);i++)
    R[i]=a[mid+1+i];
  i=0;j=0;k=left;
 //printf("Entered into mergesort\n");
  while (i<(mid-left+1) && j<(right-mid))
    {
	if(L[i]<=R[j])
	  a[k++]=L[i++];
	else
	  a[k++]=R[j++];
    }
  while (i<(mid-left+1))
    a[k++]=L[i++];
  while (j<(right-mid))
    a[k++]=R[j++];    
}

void * mergesort(void *a)
{
        ArrayIndex *pa = (ArrayIndex *)a;
        int mid = (pa->low + pa->high)/2;

        ArrayIndex aIndex[N];
        pthread_t thread[N];

        aIndex[0].low = pa->low;
        aIndex[0].high = mid;

        aIndex[1].low = mid+1;
        aIndex[1].high = pa->high;

        if (pa->low >= pa->high) return;

        int i;
        for(i = 0; i < N; i++) pthread_create(&thread[i], NULL, mergesort, &aIndex[i]);
        for(i = 0; i < N; i++) pthread_join(thread[i], NULL);

        merge(pa->low, pa->high);

        pthread_exit(NULL);
}
void randgen()
{
long i;
srand(time(NULL));
for(i=0;i<NUM_ELE;i++)
 a[i]=rand()%(1000010) +1 ;
}

int main()
{	
	int ver=0;
        ArrayIndex ai;
        ai.low = 0;
        ai.high = NUM_ELE;
	int no_of_ele=ai.high;
        pthread_t thread;
	randgen();
        pthread_create(&thread, NULL, mergesort, &ai);
        pthread_join(thread, NULL);

        int i;
        for (i = 1; i < no_of_ele+1; i++) {ver++;printf ("%d ", a[i]);}
	printf("\n%d\n",ver);

        return 0;
}

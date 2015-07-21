#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>
#define NUM_ELE 1000
int a[NUM_ELE];
int num_iter;
struct Array{
	long leftone;
	long rightone;
	};
//int a[NUM_ELE];
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
void* mergesort(void *threadarg)
{
 int left,right;
 int mid;
 int err;
 pthread_t thread[3];
 pthread_mutex_t lock;
 struct Array thd0,thd1,thd2;
 struct Array *mainthd;
 mainthd=(struct Array *)threadarg;
 left = mainthd->leftone;
 right=mainthd->rightone;  
 mid= (left+right)/2;
 if(pthread_mutex_init(&lock,NULL)){printf("Mutex init failed"); exit(0);}


 
 if(left<right){
	 num_iter++;
	  pthread_mutex_lock(&lock);
		thd0.leftone=left;
		thd0.rightone=mid;
		err=pthread_create(&(thread[0]),NULL,mergesort,(void *)&thd0);
		if(err) {printf("Error initializing thread"); exit(0);} 
	  pthread_mutex_unlock(&lock);
	  pthread_mutex_lock(&lock);
		thd1.leftone=mid+1;
		thd1.rightone=right;
		err=pthread_create(&(thread[1]),NULL,mergesort,(void *)&thd1);
		if(err) {printf("Error initializing thread"); exit(0);} 
	  pthread_mutex_unlock(&lock);
	  pthread_join(thread[0],NULL);
	  pthread_join(thread[1],NULL);
	  /*pthread_mutex_lock(&lock);
		thd2.leftone=left;
		thd2.rightone=right;
		err=pthread_create(&(thread[2]),NULL,mergesort,(void *)&thd2);
		if(err) {printf("Error initializing thread"); exit(0);} */		
		merge(left,right);
	  //pthread_mutex_unlock(&lock);
	pthread_exit(NULL);
	
}
}
void randgen()
{
long i;
srand(time(NULL));
for(i=0;i<NUM_ELE;i++)
 a[i]=rand()%(100000) +1 ;
}

void main()
{

long i;
clock_t start,end;
randgen();
struct Array mainone;
 mainone.leftone=0;
mainone.rightone=NUM_ELE;
/*for(i=0;i<NUM_ELE;i++)
printf("%d   ",a[i]);
printf("\n\n");*/
 start=time(NULL);
  mergesort((void *) &mainone);
 end=time(NULL);
 num_iter++;

 /* for(i=1;i<NUM_ELE+1;i++)
	printf("%d   ",a[i]);*/
 printf("\nNUmber_iter=%d\n",num_iter);
 printf("random number for check=%d\n",rand());
 printf("Time reqd= %f\n",difftime(end,start));
}

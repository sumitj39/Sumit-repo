#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>
#include<sys/time.h>
#include<time.h>

int *thearray;
int *tmparray;

struct sortrange {
	int start,size;
	int mdepth;
};

typedef int (*cmptype)(const void *,const void *);
typedef void * (*thrfunctype)(void *);

int compare(int *a, int *b) {
	return *a-*b;
}

void fillrandom(int a[],int n)
{
	int i;
	srand(time(NULL));
	for (i=0;i<n;i++) {
		a[i]=rand()%10000000;
	}
}

void mergesort(struct sortrange *s) {
	struct sortrange first,second;
	pthread_t fthr,sthr;
	int fi,si,i;

	//printf("{");
	if (s->mdepth<=0) {	/* normal sort */
		qsort(thearray+s->start,s->size,sizeof(int),(cmptype) compare);
	} else {
		first.start=s->start;
		first.size=s->size/2;
		first.mdepth=s->mdepth-1;
		second.start=s->start+first.size;
		second.size=s->size-first.size;
		second.mdepth=s->mdepth-1;
		 if (pthread_create(&fthr, NULL, (thrfunctype) mergesort, 
		 			(void *) &(first)))
		           perror("Create thread failed");
		/*
		mergesort(&first);
		*/
		 if (pthread_create(&sthr, NULL, (thrfunctype) mergesort, 
		 			(void *) &(second)))
		           perror("Create thread failed");
		pthread_join(fthr,NULL);
		pthread_join(sthr,NULL);
		/*
		mergesort(&second);
		*/
		fi=first.start;
		si=second.start;
		for (i=s->start;i < s->start+s->size;i++) {
			if (fi>=first.start+first.size)
				tmparray[i]=thearray[si++];
			else if (si>=second.start+second.size)
				tmparray[i]=thearray[fi++];
			else if (thearray[fi]<=thearray[si])
				tmparray[i]=thearray[fi++];
			else 
				tmparray[i]=thearray[si++];
		}
		memcpy(thearray+s->start,tmparray+s->start,sizeof(int)*s->size);
	}
	printf("}");
}


int main(int argc,char *argv[]) 
{
	int i;
	struct sortrange s;
	int N, D;
	struct timeval tv1,tv2;
	struct timezone tz;

	if (argc != 3) {
		fprintf(stderr,"Usage: %s size thrdepth\n",argv[0]);
		return 1;
	}

	D=atoi(argv[2]);
	if (D < 0 || D > 20) {
		fprintf(stderr,"0<=thrdepth<7 !\n");
		return 2;
	}
	N=atoi(argv[1]);
	if (N <= 2 || N > 200000000) {
		fprintf(stderr,"2<size<200 0000 000 !\n");
		return 3;
	}


	thearray=malloc(sizeof(int)*N);
	tmparray=malloc(sizeof(int)*N);
	fillrandom(thearray,N);
	s.start=0;
	s.size=N;
	s.mdepth=D;
	fprintf(stderr,"STARTED:\n");
	gettimeofday(&tv1,&tz);
	mergesort(&s);
	gettimeofday(&tv2,&tz);
	fprintf(stderr,"FINISHED:\n");
	for (i=0;i<N;i++) fprintf(stdout,"%d ",thearray[i]);
	fprintf(stdout,"\n");
	fprintf(stderr,"%.6f\n",tv2.tv_sec+tv2.tv_usec/1000000.0 -
				tv1.tv_sec-tv1.tv_usec/1000000.0);
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>

int rs, ps ,yo;
int *re, *ss;
int **al, **mr , **ne;
int np = 0;

pthread_mutex_t lr;
pthread_cond_t co;

bool SafeSequence() 
{
    int a[rs];
    for(int i=0; i<rs; i++) 
		a[i] = re[i];
	bool f[ps];
    for(int i=0; i<ps; i++) 
		f[i] = false;
    int fd=0;
    while(fd < ps) 
	{
        bool s = false;
		for(int i=0; i<ps; i++) 
		{
            if(!f[i]) 
			{
                bool bl = true;
				for(int j=0; j<rs; j++)
                if(ne[i][j] > a[j])
				{
                    bl = false;
                    break;
                }
				if(bl) 
				{
		for(int j=0; j<rs; j++)
	a[j] += al[i][j];
	ss[fd] = i;
	f[i] = true;
		++fd;
		s = true;
	}
            }
        }
		if(!s) 
		{
            for(int k=0; k<ps; k++) ss[k] = -1;
            return false;
        }
    }
    return true;
}


void* MainResult(void *arg) 
{
    int p = *((int *) arg);
	pthread_mutex_lock(&lr);
	while(p != ss[np])
    pthread_cond_wait(&co, &lr);
	printf("\nProcess %d", p+1);
    printf("\n\tAllocated : ");
    for(int i=0; i<rs; i++)
    printf("%3d", al[p][i]);
	printf("\n\tNeeded    : ");
    for(int i=0; i<rs; i++)
    printf("%3d", ne[p][i]);
	printf("\n\tAvailable : ");
    for(int i=0; i<rs; i++)
    printf("%3d", re[i]);
	printf("\n");
	printf("\tResource Allocated");
	for(int i=0; i<rs; i++)
    re[i] += al[p][i];
	printf("\n\tNow Available : ");
    for(int i=0; i<rs; i++)
    printf("%3d", re[i]);
    printf("\n\n");
	np++;
    pthread_cond_broadcast(&co);
    pthread_mutex_unlock(&lr);
	pthread_exit(NULL);
}

int main(int argc, char** argv) 
{
	srand(time(NULL));
	printf("                                                      WELCOME");
	do
	{
    printf("\nNumber of processes : ");
    scanf("%d", &ps);
	printf("\nNumber of resources : ");
    scanf("%d", &rs);
	re = (int *)malloc(rs * sizeof(*re));
    printf("\nCurrently Available resources : \n");
    for(int i=0; i<rs; i++)
    {
        printf("%d : ",i+1);
        scanf("%d", &re[i]);
	}
    al = (int **)malloc(ps * sizeof(*al));
    for(int i=0; i<ps; i++)
    al[i] = (int *)malloc(rs * sizeof(**al));
	mr = (int **)malloc(ps * sizeof(*mr));
    for(int i=0; i<ps; i++)
    mr[i] = (int *)malloc(rs * sizeof(**mr));
    printf("\n");
    for(int i=0; i<ps; i++) 
	{
	printf("\nResource allocated to process %d : \n", i+1);
	for(int j=0; j<rs; j++)
		{
	printf("%d : ", j+1);
	scanf("%d", &al[i][j]);
	}
    }
    printf("\n");
	for(int i=0; i<ps; i++) 
	{
	printf("\nMaximum resource required by process %d : \n", i+1);
	for(int j=0; j<rs; j++)
        {
            printf("%d : ", j+1);
            scanf("%d", &mr[i][j]);
        }
	}
    printf("\n");
	ne = (int **)malloc(ps * sizeof(*ne));
    for(int i=0; i<ps; i++)
    ne[i] = (int *)malloc(rs * sizeof(**ne));
	for(int i=0; i<ps; i++)
	for(int j=0; j<rs; j++)
    ne[i][j] = mr[i][j] - al[i][j];
	ss = (int *)malloc(ps * sizeof(*ss));
    for(int i=0; i<ps; i++) ss[i] = -1;
	if(!SafeSequence()) 
	{
	printf("\nSystem is in unsafe state.\n\n");
	exit(-1);
    }
	printf("\n\nSafe Sequence Found : ");
    for(int i=0; i<ps; i++)
	{
        printf("%-3d", ss[i]+1);
    }
	pthread_t pr[ps];
	pthread_attr_t at;
    pthread_attr_init(&at);
	int pn[ps];
	for(int i=0; i<ps; i++) pn[i] = i;
	for(int i=0; i<ps; i++)
    pthread_create(&pr[i], &at, MainResult, (void *)(&pn[i]));
	for(int i=0; i<ps; i++)
    pthread_join(pr[i], NULL);
	printf("\nAll Processes Finished\n");	
	free(re);
    for(int i=0; i<ps; i++) 
	{
	free(al[i]);
	free(mr[i]);
		free(ne[i]);
    }
    free(al);
    free(mr);
	free(ne);
    free(ss);
    printf("You want to continue then you press '0' otherwise press '1'");
    printf("\n0 = 'Yes'");
    printf("\n1 = 'No'\n");
    scanf("%d",&yo);
	}while(yo != 1);
}


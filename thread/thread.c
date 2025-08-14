#include <stdio.h>
#include <pthread.h>

void* compute(void* a)
{
    int *sum = (int *)a;  // cast void* to int*
    for (int i = 0; i < 10000000; i++)
    {
        *sum += 1;
    }

    printf("%d\n", *sum);

    return NULL; // pthread requires a return value
}

int main()
{
    pthread_t thread1;
    pthread_t thread2;

    int value1 = 1;
    int value2 = 2;

    //pthread_create(&thread1, NULL, compute, &value1);

    //pthread_create(&thread2, NULL, compute, &value2);

    //pthread_join(thread1, NULL);
    //pthread_join(thread2, NULL);

    compute(&value1);
    
    compute(&value2);
    
    return 0;
}

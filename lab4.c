#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX 1000;

int main(){

    //three arrays
    int one[MAX];
    int two[MAX];
    int three[MAX];
    int sum = 0;

    srand(time(NULL));
    for (int i = 0; i <= MAX; i++){
      one[i] =  rand() % 1000;
      two[i] =  rand() % 1000;
    }
    //
    for (int i = 0; i <= MAX; i++){
      three[i] = one[i] + two[i];
      sum += three[i];
    }
    //print it
    printf("The sum of all random numbers is: %d\n", sum);
}

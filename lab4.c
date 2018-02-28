#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int main(){

    //three arrays
    int one[1000];
    int two[1000];
    int three[1000];
    int sum = 0;

    srand(time(NULL));
    for (int i = 0; i <= 1000; i++){
      one[i] =  rand() % 1000;
      two[i] =  rand() % 1000;
      three[i] = one[i] + two[i];
      sum += three[i];
    }
    //print it
    printf("The sum of all random numbers is: %d\n", sum);
}

/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

void swap(int* a, int* b)
{
    int *t = (int*)malloc(sizeof(int));
    *t = *a;
    *a = *b;
    *b = *t;
    free(t);
}
//
// void swap(int* a, int* b)
// {
//     // int *t = (int*)malloc(sizeof(int));
//     int t = *a;
//     *a = *b;
//     *b = t;
//     // free(t);
// }

int partition (int arr[], int low, int high)
{
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high- 1; j++)
    {
        if (arr[j] <= pivot)
        {
            i++;swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quicksort(int arr[], int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high);
 	quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}


void app_main()
{
    printf("Hello world!\n");

    // char sentence[1000];
    // FILE *fptr;
    // fptr = fopen("program.txt", "w+");
    // if(fptr == NULL)
    // {
    //    printf("Error!");
    //    exit(1);
    // }
   // for(int i=0;i<20;i++)
    // fprintf(fptr,"%d ", rand()/10000000);
    // fclose(fptr);

 int a[630],index=0;
 // int temp;
   // FILE * q=fopen("program.txt","r");
   // do
   for(int i=0;i<630;i++)
     {
         // fscanf(q,"%d", &temp);
 	a[index++]=rand()/1000000;
    }
     // while(!feof(q));
  for(int i=0;i<630;i++)
 	printf("%d \n",a[i]);
 	quicksort(a,0,629);

 printf("\n\nAFTER SORTING\n\n");

  for(int i=0;i<629;i++)
 	if(a[i]>a[i+1])
 		printf("fail\n");


  for(int i=0;i<630;i++)
 	printf("%d \n",a[i]);


    fflush(stdout);
    // esp_restart();
}

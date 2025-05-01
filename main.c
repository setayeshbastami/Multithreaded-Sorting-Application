#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_SIZE 100

// global arrays
int original_array[MAX_SIZE];
int sorted_array[MAX_SIZE];
int array_size = 0;

// struct for data
typedef struct {
    int start;
    int end;
} thread_data;

// Bubble Sort
void* sort_thread(void* arg) {
    thread_data* data = (thread_data*)arg;
    int start = data->start;
    int end = data->end;
    int n = end - start;

    for (int i = 0; i < n-1; i++) {
      for (int j = start; j < end-i-1; j++) {
        if (original_array[j] > original_array[j+1]) {
          int temp = original_array[j];
          original_array[j] = original_array[j+1];
          original_array[j+1] = temp;
        }
    }
  }

    pthread_exit(NULL);

}

// merge two sorted thread
void* merge_thread(void* arg) {
    thread_data* data = (thread_data*)arg;
    int i = data->start;
    int j = data->end;
    int middle = j;
    int k = 0;

    while (i < middle && j < array_size) {
        if (original_array[i] < original_array[j]) {
          sorted_array[k++] = original_array[i++];
        } else {
          sorted_array[k++] = original_array[j++];
        }
   }

    while (i < middle) {
      sorted_array[k++] = original_array[i++];
    }

    while (j < array_size) {
      sorted_array[k++] = original_array[j++];
    }

    pthread_exit(NULL);

}

int main() {

    pthread_t threads[3];
    thread_data data[2];

    printf("(enter size of array , max: %d): ", MAX_SIZE);
    scanf("%d", &array_size);

    if (array_size <= 0 || array_size > MAX_SIZE) {
        printf("invalid size!\n");
        return 1;
    }

    printf("enter your array : \n");
    for (int i = 0; i < array_size; i++) {
        scanf("%d", &original_array[i]);
    }

    // divide original array
    int mid = array_size / 2;

    // create sorting threads
    data[0].start = 0;
    data[0].end = mid;
    pthread_create(&threads[0], NULL, sort_thread, &data[0]);

    data[1].start = mid;
    data[1].end = array_size;
    pthread_create(&threads[1], NULL, sort_thread, &data[1]);

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    // create merge thread
    thread_data merge_data;
    merge_data.start = 0;
    merge_data.end = mid;
    pthread_create(&threads[2], NULL, merge_thread, &merge_data);

    pthread_join(threads[2], NULL);

    // print sorted array
    printf("sorted array :\n");
    for (int i = 0; i < array_size; i++) {
        printf("%d ", sorted_array[i]);
    }
    printf("\n");

    return 0;
}


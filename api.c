#include <stdio.h>
#include "api.h" 

int global_array[100] = {-1};
int* ptr;
volatile int *p = 0;
void FunctionThatEscapesLocalObject() {
  int local[100];
  ptr = &local[0];
}

int check(const char *data, int len) 
{ 
  if(len < 4)return 1;

  int cnt = 0;
  //异常的业务逻辑
  if (data[0] == 'x') { cnt++; }

  if (data[1] == 'y') { cnt++; }

  if (data[2] == 'z') { cnt++; }

  if (cnt >= 1) {
    switch (data[3]) {
      case '0': {
        // segv
        int *p = NULL; *p = 123;
        break;
      }
      case '1': {
        // stack-buffer-underflow
        int* p = &cnt - 32; for (int i = 0; i < 32; i++) { *(p + i) = 0; }
        break;
      }
      case '2': {
        // stack-buffer-overflow 
        int* p = &cnt + 32; for (int i = 0; i < 32; i++) { *(p - i) = 0; }
        break;
      }
      case '3': {
        // bad-free
        int *p = &cnt; free(p);
        break;
      }
      case '4': {
        // double-free
        int* p = malloc(sizeof(int)); free(p); free(p);
        break;
      }
      case '5': {
        // heap-use-after-free
        int* p = malloc(sizeof(int)); free(p); *p = 123;
        break;
      }
      case '6': {
        // heap-buffer-overflow
        int* p = malloc(8 * sizeof(int)); for (int i = 0; i < 32; i++) { *(p + i) = 0; }
        break;
      }
      case '7': {
        // fpe
        int x = 0; int y = 123 / x;
        break;
      }
      case '8': {
        //memory leak
        int* p = malloc(7);
        p = 0; // The memory is leaked here.
        break;
      }
      case '9': {
        //global_buffer_over_flow
        printf(global_array[101]);  // BOOM 
      }
      case 'a': {
        //stack-use-after-free
        int a[5]={1,2,3,4,5};
        int* array = a;
        free(array);
        return array[1];  // BOOM
      }
      case 'b': {
        //use after return 
        FunctionThatEscapesLocalObject();
        return ptr[1];
      }
      case 'c': {
        //use after scope
         {
            int x = 0;
            p = &x;
          }
        *p = 5;
        return 0;
      }
      
    }
  }                        
  return 0; 
}

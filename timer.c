#include "timer.h"
#include <stdio.h>
#include <unistd.h>

void backout(int n) {
  for (int i = n; i > 0; i--) {
    printf("Wait for %d seconds\n", i);
    sleep(1);
  }
}

#include <stdio.h>
#include <unistd.h>

int main(void) {
  int i;
  for (i = 0; i < 10; i++) {
    fprintf(stdout, "tick\n");;
    fflush(stdout);
    sleep(1);
  }
  fprintf(stdout, "BOOM!\n");;
  return 0;
}

#include <stdio.h>
#include <assert.h>

#include "../libuv/include/uv.h"

#define PATHMAX 1024

#define S(str) #str
#define RUN(name) header(S(name)); (name)()

void header(char* name) {
  printf("\n\n%s\n", name);
  printf("===========================\n\n");
}

void exepath() {
  char buffer[PATHMAX];
  size_t size;
  int r;

  size = sizeof(buffer) / sizeof(buffer[0]);

  r = uv_exepath(buffer, &size);
  assert(!r);


  printf("current executable: %s", buffer);
}

void loadavg() {
  double avg[3];

  uv_loadavg(avg);
  assert(avg != NULL);

  printf("load averages: ");

  for (int i = 0; i < 3; i++) {
    printf("%f, ", avg[i]);
  }
}

int main() {
  RUN(exepath);
  RUN(loadavg);
  return 0;
}

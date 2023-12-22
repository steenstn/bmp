#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

  FILE *file;
  file = fopen(argv[1], "rb");
  char header[14];

  fread(header, 14, 1, file);
  int s = header[10];
  fclose(file);

  printf("s = %d\n", s);

  char *message = argv[2];

  file = fopen(argv[1], "r+b");
  fseek(file, s, SEEK_SET);
  fputs(message, file);

  //fwrite(message, 1, sizeof(message), file);
  fclose(file);
  return 0;
}

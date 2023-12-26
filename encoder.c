#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ENCODE 1
#define DECODE 2

void decode(char* filename);
void printInstructions() {
  printf("Usage: \n");
  printf("Encode a file: ./encoder -e filename.bmp message\n");
  printf("Decode a file: ./encoder -d filename.bmp\n");
}

int main(int argc, char *argv[]) {

  if(argc == 1) {
    printInstructions();
    return 0;
  }
  FILE *file;
  file = fopen(argv[2], "rb");
  unsigned char header[14];

  fread(header, 14, 1, file);
  // TODO can be 4 bytes
  //
  unsigned char imageStart = header[10];
  printf("Bits offset: %d\n", imageStart);
  fclose(file);

  char *alg = argv[1];

  int algorithm = 0;
  if(alg[1] == 'd') {
    algorithm = DECODE;
  } else if(alg[1] == 'e') {
    algorithm = ENCODE;
  }

  if(algorithm == DECODE) {
    decode(argv[2]);
    return 0;
  }


  char *message = argv[3];

  printf("Size of message: %lu\n", strlen(message));

  file = fopen(argv[2], "r+b");

  fseek(file, imageStart, SEEK_SET);

  printf("Message to encode: %s\n", message);

  for(int i = 0; i < (int)strlen(message); i++) {

    unsigned char block[8];
    fread(block, 1, 8, file);
    fseek(file, -8, SEEK_CUR);


    unsigned char target[8] =
      {
        (message[i] & 0b1),
        (message[i] & 0b10) >> 1,
        (message[i] & 0b100) >> 2,
        (message[i] & 0b1000) >> 3,
        (message[i] & 0b10000) >> 4,
        (message[i] & 0b100000) >> 5,
        (message[i] & 0b1000000) >> 6,
        (message[i] & 0b10000000) >> 7
      };

    for(int j = 0; j < 8; j++) {
      if(target[j] > (block[j] & 0b1)) {
        block[j]++;
      } else if (target[j] <( block[j] & 0b1)) {
        block[j]--;
      }
    }


    fwrite(block, 1, 8, file);
  }

  fclose(file);
  return 0;
}


void decode(char* filename) {
  FILE *file;
  file = fopen(filename, "rb");
  unsigned char header[14];

  fread(header, 14, 1, file);
  unsigned char imageStart = header[10];


  fseek(file, imageStart, SEEK_SET);

  for(int i = 0; i < 50; i++) {
    unsigned char block[8];
    fread(block, 1, 8, file);
    unsigned char result =
      (block[0] & 0b1) +
      ((block[1] & 0b1) << 1) +
      ((block[2] & 0b1) << 2) +
      ((block[3] & 0b1) << 3) +
      ((block[4] & 0b1) << 4) +
      ((block[5] & 0b1) << 5) +
      ((block[6] & 0b1) << 6) +
      ((block[7] & 0b1) << 7);

    printf("%c", result);
  }
  printf("\n");
  fclose(file);
}

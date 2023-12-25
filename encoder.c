#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ENCODE 1
#define DECODE 2

void decode(char* filename);

int main(int argc, char *argv[]) {

  FILE *file;
  file = fopen(argv[1], "rb");
  char header[14];

  fread(header, 14, 1, file);
  int imageStart = header[10];
  fclose(file);

  char *alg = argv[2];

  int algorithm = 0;
  if(alg[1] == 'd') {
    algorithm = DECODE;
  } else if(alg[1] == 'e') {
    algorithm = ENCODE;
  }

  if(algorithm == DECODE) {
    decode(argv[1]);
    return 0;
  }


  char *message = argv[3];

  printf("size of message: %lu\n", strlen(message));

  file = fopen(argv[1], "r+b");

  fseek(file, imageStart, SEEK_SET);

 // printf("bit shift: %d\n", 12 >> 2);

  int image = 220;
  char toEncode = 'A'; // 1000001
  int toWrite = toEncode & 0b11000000;
  int result = image | toWrite >> 6;
  //printf("result %d\n", result);


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
     // printf("%d: target %d  block %d, & %d\n", j, target[j], block[j],(block[j] & 0b1));
      if(target[j] == 0 && (block[j] & 0b1) == 1) {
      //  printf("minus\n");
        block[j]--;
      } else if (target[j] == 1 &&( block[j] | 0b0) == 0) {
     //   printf("plus\n");
        block[j]++;
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
  char header[14];

  fread(header, 14, 1, file);
  int imageStart = header[10];


  fseek(file, imageStart, SEEK_SET);

  for(int i = 0; i < 50; i++) {
    unsigned char block[8];
    fread(block, 1, 8, file);
   // printf("Block %d %d %d %d %d %d %d %d\n", block[0], block[1], block[2], block[3], block[4], block[5], block[6], block[7]);
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
//    printf("Result:%d -  %c\n\n", result, result);
  }
  printf("\n");
  fclose(file);
}
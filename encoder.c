#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ENCODE 1
#define DECODE 2

void encodeMessage(const char* message, const char* filename);
void encode(const char charToEncode, FILE* file); 
void decodeMessage(char* filename);

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
  
  char *alg = argv[1];

  int algorithm = 0;
  if(alg[1] == 'd') {
    algorithm = DECODE;
  } else if(alg[1] == 'e') {
    algorithm = ENCODE;
  }

  if(algorithm == DECODE) {
    decodeMessage(argv[2]);
  } else if(algorithm == ENCODE) {
    encodeMessage(argv[3], argv[2]);
  }

  return 0;
}

void encodeMessage(const char* message, const char* filename) {
  FILE *file;
  file = fopen(filename, "r+b");
  unsigned char header[14];

  fread(header, 14, 1, file);
  // TODO can be 4 bytes
  //
  unsigned char imageStart = header[10];
  printf("Bits offset: %d\n", imageStart);

  fseek(file, imageStart, SEEK_SET);

  printf("Size of message: %lu\n", strlen(message));
  unsigned int messageSize = strlen(message);
  printf("Message to encode: %s\n", message);
  
  encode((char)messageSize, file);
  
  for(int i = 0; i < (int)strlen(message); i++) {
    encode(message[i], file);
  }

  fclose(file);

}

void encode(const char charToEncode, FILE* file) {
  
  unsigned char block[8];
  fread(block, 1, 8, file);
  fseek(file, -8, SEEK_CUR); 
  
  unsigned char target[8] =
      {
        (charToEncode & 0b1),
        (charToEncode & 0b10) >> 1,
        (charToEncode & 0b100) >> 2,
        (charToEncode & 0b1000) >> 3,
        (charToEncode & 0b10000) >> 4,
        (charToEncode & 0b100000) >> 5,
        (charToEncode & 0b1000000) >> 6,
        (charToEncode & 0b10000000) >> 7
      };

 for(int j = 0; j < 8; j++) {
      if(target[j] > (block[j] & 0b1)) {
        block[j]++;
      } else if (target[j] < (block[j] & 0b1)) {
        block[j]--;
      }
    }


    fwrite(block, 1, 8, file); 
}


void decodeMessage(char* filename) {
  FILE *file;
  file = fopen(filename, "rb");
  unsigned char header[14];

  fread(header, 14, 1, file);
  unsigned char imageStart = header[10];


  fseek(file, imageStart, SEEK_SET);
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

  unsigned messageSize = (int)result;
  printf("Message size: %d\n", messageSize);
  for(int i = 0; i < messageSize; i++) {
    fread(block, 1, 8, file);
    result =
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

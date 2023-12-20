#include <stdio.h>
#include <stdlib.h>


int main (int argc, char *argv[]) {

  if(argc != 2) {
    printf("Please provide filename");
    return 0;
  }

  // TODO: Size of bmp file in bytes
  // TODO: Offset of the pixel array

  int bitmapWidth = 2;
  int bitmapHeight = 1;
  int bitsPerPixel = 24;
  unsigned char dibHeader[] = {40, 0, 0, 0, bitmapWidth, 0, 0, 0, bitmapHeight, 0, 0, 0, 1, 0, bitsPerPixel, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0 ,0 ,0, 10, 0, 0, 0, 0,0,0,0,0,0,0,0};


  int pixelmapOffset = 14 + sizeof(dibHeader);
  unsigned char header[] = {'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, pixelmapOffset, 0, 0, 0 };
  unsigned char image[] = {255, 0, 0, 0,255,0, 0, 0};




  int s = sizeof(header);

  printf("Creating %s\n", argv[1]);
  FILE *file;
  file = fopen(argv[1], "wb");
  fwrite(header, 1, sizeof(header), file);
  fwrite(dibHeader, 1, sizeof(dibHeader), file);
  fwrite(image, 1, sizeof(image), file);
  fclose(file);
  return 0;
}

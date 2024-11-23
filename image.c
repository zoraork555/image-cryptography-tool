// Ashton Johnson 11797181
// This program takes in an image in the PGM format and can either encode a message
// into the PGM or it can decode an already encoded picture.
// Three helper functions are included in this program.
// binaryCh and binaryNum take in a char and a num, respectively, and return a pointer
// to a list that includes the binary code of the input.
// Unbinary takes in that list once it's been encoded and reverts the binary into a
// base-10 number to be reinserted into the PGM or to be printed as the message's characters

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image.h"

// Given a filename of a pgm image, read in the image and
// store it in the ImagePGM structure. Return the address of
// the ImagePGM structure if the file can be opened or
// NULL otherwise.
ImagePGM *readPGM(char *filename) {
  FILE *fp=fopen(filename, "r");
	if (fp==NULL) return NULL;

  ImagePGM *ptr;
  ptr=malloc(sizeof(ImagePGM));

  fscanf(fp, "%s%d%d%d", ptr->magic, &ptr->width, &ptr->height, &ptr->max_value);

  //malloc space for matrix
  ptr->pixels = malloc((ptr->height)*sizeof(int*));
  for (int i=0; i<(ptr->height); i++) {
    ptr->pixels[i] = malloc((ptr->width)*sizeof(int));
  }

  //read in matrix
  for (int i=0; i<(ptr->height); ++i) {
    for (int j=0; j<(ptr->width); ++j) {
      fscanf(fp, "%d", &(ptr->pixels)[i][j]);
    }
  }

  fclose(fp);
  return ptr;
}

// Write out a pgm image stored in a ImagePGM structure into
// the specified file. Return 1 if the file can be opened or
// 0 otherwise.
int writePGM(ImagePGM *pImagePGM, char *filename) {
  FILE *fp=fopen(filename, "w");
  if (fp==NULL) {
    return 0;
  }

  ImagePGM *ptr = pImagePGM;

  fprintf(fp, "%s\n%d %d %d\n", ptr->magic, ptr->width, ptr->height, ptr->max_value);
  for (int i=0; i<(ptr->height); ++i) {
    for (int j=0; j<(ptr->width); ++j) {
      fprintf(fp, "%4d", (ptr->pixels)[i][j]);
    }
    fprintf(fp, "\n");
  }
  fprintf(fp, "\n");

  fclose(fp);
  return 1;
}

// Convert a character into binary and reverse it for entering into PGM
int *binaryCh(char ch) {
  int *bin = malloc(sizeof(int)*9);
  for (int j=0; j<8; ++j) {
    bin[j] = ch%2;
    ch/=2;
  }

  for (int i=0; i<4; ++i) {
    int temp = bin[i];
    bin[i] = bin[7-i];
    bin[7-i] = temp;
  }

  return bin;
}

// same as binaryCh but with an int input
int *binaryNum(int num) {
  int *bin = malloc(sizeof(int)*9);
  for (int j=0; j<8; ++j) {
    bin[j] = num%2;
    num/=2;
  }

  for (int i=0; i<4; ++i) {
    int temp = bin[i];
    bin[i] = bin[7-i];
    bin[7-i] = temp;
  }

  return bin;
}

// reverts binary into 10-base
int unbinary(int *bin) {
  int num=0, b[9] = {1, 2, 4, 8, 16, 32, 64, 128};
  for (int i=0; i<4; ++i) {
    int temp = bin[i];
    bin[i] = bin[7-i];
    bin[7-i] = temp;
  }

  for (int j=0; j<8; ++j) {
    if (bin[j]==1) num+=b[j];
  }

  return num;
}

// Encode/embed a message into a PGM image.
// Return 0 if the image is too small or
// 1 otherwise.
int encode(ImagePGM *pImagePGM, char *msg) {
  ImagePGM *ptr = pImagePGM;
  int countR = (ptr->height)-1, countC = (ptr->width)-1,
      cells = (ptr->height) * (ptr->width), len=strlen(msg);
  char ch;
  if (((len+1)*8)>cells) return 0;
  for (int i=0; i<len; ++i) {
    ch = msg[i];
    int *bin = binaryCh(ch);
    for (int j=0; j<8; j++) {
      int *pix = binaryNum((ptr->pixels)[countR][countC]);
      pix[7] = bin[j];
      ptr->pixels[countR][countC] = unbinary(pix);
      if (countC==0) {
        countC = (ptr->width)-1;
        --countR;
      }
      else --countC;
      free(pix);
    }
    free(bin);
  }

  for (int j=0; j<8; j++) { // null character on end
    int *pix = binaryNum((ptr->pixels)[countR][countC]);
    pix[7] = 0;
    ptr->pixels[countR][countC] = unbinary(pix);
    if (countC==0) {
      countC = (ptr->width)-1;
      --countR;
    }
    else --countC;
    free(pix);
  }

  return 1;
}

// Decode/extract and return the message embedded in a PGM image
char *decode(ImagePGM *pImagePGM) {
  ImagePGM *ptr = pImagePGM;
  int countR = (ptr->height)-1, countC = (ptr->width)-1, other=0,
      count = (ptr->height) * (ptr->width), *ch = malloc(sizeof(int)*9);
  char *p = malloc(sizeof(char)*(count/8)), letter;
  while (count>7) {
    for (int i=0; i<8; ++i) {
      int *pix = binaryNum((ptr->pixels)[countR][countC]);
      ch[i] = pix[7];
      free(pix);
      --count;
      if (countC==0) {
        countC = (ptr->width)-1;
        --countR;
      }
      else --countC;
    }
    letter = unbinary(ch);
    if (letter==0) break;
    p[other] = letter;
    other++;
  }
  p[other] = '\0';
  free(ch);
  return p;
}

// Free the space used by a pgm image.
void freePGM(ImagePGM *pImagePGM) {
  ImagePGM *ptr = pImagePGM;

  for (int i=0; i<ptr->height; i++) {
    free(ptr->pixels[i]);
  }
  free(ptr->pixels);
}

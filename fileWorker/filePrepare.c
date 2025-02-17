#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "filePrepare.h"
#include "../utils/printColors.h"
#include <stdbool.h>

// Step One

void getFileSize(FileInfo* fileInfo);
void getFileContent(FileInfo* fileInfo);
void fillSymbolsCountArr(FileInfo* fileInfo);
void sortSymbolsCountArr(FileInfo* fileInfo);

void init(FileInfo* fileInfo) {
  memset(fileInfo->symbolsCountArr, 0, sizeof(fileInfo->symbolsCountArr));
  memset(fileInfo->content, 0, sizeof(fileInfo->content));
  getFileSize(fileInfo);
  getFileContent(fileInfo);
  fillSymbolsCountArr(fileInfo);
  sortSymbolsCountArr(fileInfo);
}

void getFileSize(FileInfo* fileInfo) {
  fileInfo->size = 0;
  fileInfo->file = fopen("../testData/text_20byte.txt", "rb");
  if (fileInfo->file == 0) {
    perror("Open error");
    exit(1);
  }
  fseek(fileInfo->file, 0, SEEK_END);
  fileInfo->size = ftell(fileInfo->file);
  fseek(fileInfo->file, 0, SEEK_SET);
  fclose(fileInfo->file);
}

void getFileContent(FileInfo* fileInfo) {
  fileInfo->file = fopen("../testData/text_20byte.txt", "rb");
  unsigned long long readIndex = 0;
  unsigned char buffer = getc(fileInfo->file);
  while (readIndex < fileInfo->size) {
//    printf("readIndex -> %llu\tvalue -> %d\n", readIndex, buffer);
    fileInfo->content[readIndex] = buffer; // pls no write (char)buffer this is break all. we should use int
    buffer = fgetc(fileInfo->file);
    readIndex++;
  }
  fclose(fileInfo->file);
  fileInfo->content[readIndex] = 0;
}

void printFileSize(FileInfo* fileInfo) {
  printf("Size of file: %llu bytes.\n", fileInfo->size);
}

void printFileAsHex(FileInfo* fileInfo) {
  for (unsigned long long i = 0; i < fileInfo->size; i++) {
    if (i % 5 == 0 && i != 0) {
      printf("\n");
    }
    printf("0x%x -> %c\t", fileInfo->content[i], fileInfo->content[i]);
  }
  printf("\n");
}

void printFileAsText(FileInfo* fileInfo) {
  for (int i = 0; (int)fileInfo->content[i] != 0 ; ++i) {
    printf("%c", (char)fileInfo->content[i]);
  }
  printf("\n");
}

void fillSymbolsCountArr(FileInfo* fileInfo) {
  unsigned long long bufferArr[FILE_COUNT_ARR_LEN] = {0};
  for (unsigned long long i = 0; i < fileInfo->size; ++i) {
    bufferArr[fileInfo->content[i]]++;
  }
  for (int i = 0, j = 0; i < FILE_COUNT_ARR_LEN; ++i) {
    if (bufferArr[i] != 0) {
      fileInfo->symbolsCountArr[j][0] = i;
      fileInfo->symbolsCountArr[j][1] = bufferArr[i];
      j++;
    }
  }
}

void printSymbolsCountArr(FileInfo* fileInfo) {
  for (int i = 0; i < FILE_COUNT_ARR_LEN; ++i) {
    if (!(i%8)) {
      printf("\n");
    }
    printf("0x%x -> %llu\t", (int)fileInfo->symbolsCountArr[i][0], fileInfo->symbolsCountArr[i][1]);
  }
  printf("\n");
}

void sortSymbolsCountArr(FileInfo* fileInfo) {
  unsigned long long buffer[2] = {0};
  for (int i = 0; i < FILE_COUNT_ARR_LEN-1;) {
    if (fileInfo->symbolsCountArr[i][1] > fileInfo->symbolsCountArr[i+1][1] && fileInfo->symbolsCountArr[i+1][1] != 0) {
      buffer[0] = fileInfo->symbolsCountArr[i][0]; buffer[1] = fileInfo->symbolsCountArr[i][1];
      fileInfo->symbolsCountArr[i][0] = fileInfo->symbolsCountArr[i+1][0]; fileInfo->symbolsCountArr[i][1] = fileInfo->symbolsCountArr[i+1][1];
      fileInfo->symbolsCountArr[i+1][0] = buffer[0]; fileInfo->symbolsCountArr[i+1][1] = buffer[1];
      i = 0;
    } else {
      i++;
    }
  }
}

int getSymbolsCountArrLen(FileInfo* fileInfo) {
  int len = 0;
  while (fileInfo->symbolsCountArr[len][1]) {len++;}
  return len;
}

void checkFileSize(char* filenameInput, char* filenameOutput) {
  long fileSizes[2] = {0};
  FILE* fileInput = fopen(filenameInput, "rb");
  FILE* fileOutput = fopen(filenameOutput, "rb");
  if (fileInput == 0 || fileOutput == 0) {
    perror("Open error");
    exit(3);
  }
  fseek(fileInput, 0, SEEK_END);
  fileSizes[0] = ftell(fileInput);
  fseek(fileInput, 0, SEEK_SET);
  fclose(fileInput);
  fseek(fileOutput, 0, SEEK_END);
  fileSizes[1] = ftell(fileOutput);
  fseek(fileOutput, 0, SEEK_SET);
  fclose(fileOutput);
  printf("file input: %ld bytes,  file output: %ld bytes\n", fileSizes[0], fileSizes[1]);
  fileSizes[0] > fileSizes[1] ?
  printf(ANSI_COLOR_GREEN"save %ld bytes -> %.2f%%"ANSI_COLOR_RESET"\n", fileSizes[0] - fileSizes[1], 100-((float)fileSizes[1]/(float)fileSizes[0])*100) :
  printf(ANSI_COLOR_RED"get %ld bytes -> %.2f%%"ANSI_COLOR_RESET"\n", fileSizes[1] - fileSizes[0], ((float)fileSizes[1]/(float)fileSizes[0])*100);
}

void checkFileHash(const char* filenameInput, char* filenameOutput) {
  char cmd1[200] = "shasum -a 256 ", cmd2[100] = "shasum -a 256 ", outputFileName[100] = {0};
  bool filesSame = true;
  strncat(outputFileName, filenameOutput, 18);
  strncat(outputFileName, &filenameInput[17], 100);
  strncat(cmd1, filenameInput, 100);
  strncat(cmd2, outputFileName, 100);

  FILE* fileOriginal = popen(cmd1, "r");
  FILE* fileUnzip = popen(cmd2, "r");
  char buffer1[65], buffer2[65];
  if (fscanf(fileOriginal, "%64s", buffer1)==1 && fscanf(fileUnzip, "%64s", buffer2)==1) {
//    buffer2[3] = 'f';
    printf(ANSI_COLOR_GREEN);
    for (int i = 0; i < 65; ++i) {
      if (buffer1[i] != buffer2[i]) {
        printf(ANSI_COLOR_RED);
        filesSame = false;
        break;
      }
    }
    filesSame ? printf("Good\n") : printf("Archive / Decode error\n");
    printf("%.64s -> %s\n", buffer1, filenameInput);
    printf("%.64s -> %s\n", buffer2, outputFileName);
  } else {
    printf("Error: can't get checksum :(\n");
  }
  pclose(fileOriginal);
  pclose(fileUnzip);

}
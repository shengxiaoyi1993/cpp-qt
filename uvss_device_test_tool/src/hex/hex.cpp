#include "hex.h"
#include <stdlib.h>
#include <string.h>

std::string hexencoder::EncodeToHexString(std::string &input) {
  std::string result = "";

  int i;
  char b[3];
  unsigned char bu;
  const unsigned char* p = (const unsigned char*)(input.data());
  int len = input.length();

  char bMap[17] = "0123456789abcdef";
  std::string tmp = "";
  b[2] = 0;

  for(i = 0;i<len;i++) {
      bu = *(p + i);
      b[0] = bMap[bu>>4];
      b[1] = bMap[bu & 0xF];
      tmp = b;
      result += std::string(b,b+2);
  }

  return result;
}


std::string hexencoder::DecodeHexString(std::string &input) {
  char *p = const_cast<char*>(input.data());
  char b[3];
  int len = input.length();
  int i = 0;
  unsigned char bu;

  unsigned char *result = (unsigned char*)malloc(len/2+2);
  memset(result,0, len/2+2);

  b[2] = 0;

  for(i = 0;i<len;i+=2) {
      b[0] = *(p+i);
      b[1] = *(p+i+1);
      bu = (unsigned char)strtol(b,NULL,16);
      result[i>>1] = bu;
  }

  std::string output = std::string(result,result + len/2);

  free(result);
  return output;
}

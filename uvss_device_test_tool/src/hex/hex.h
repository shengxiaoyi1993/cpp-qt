#ifndef HEX_H
#define HEX_H

#include <iostream>

namespace hexencoder {
  std::string EncodeToHexString(std::string &input);
  std::string DecodeHexString(std::string &input);
}

#endif // HEX_H

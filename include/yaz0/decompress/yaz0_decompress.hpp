#pragma once
#include "yaz0/yaz0Format.hpp"
#include <cstdint>
#pragma once
#include <vector>

// compressionFlag is 8byte
// 2byte is
// lenth: 4, offset: 12.
// 3byte is
// null: 4, offset: 12, lenth: 8.

namespace yaz0 {

class Yaz0Decompress : public Yaz0Format {
private:
protected:
  PackState collectPack();

public:
  Yaz0Decompress() = default;
  Yaz0Decompress(Yaz0Decompress const &) = default;
  Yaz0Decompress(Yaz0Decompress &&) = default;

  std::vector<uint8_t> decompress() const;
};
} // namespace yaz0
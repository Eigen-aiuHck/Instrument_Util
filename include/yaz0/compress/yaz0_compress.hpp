#pragma once
#include "yaz0/yaz0Format.hpp"
#include <cstdint>
#include <memory>
#include <vector>

// compressionFlag is 8byte
// 2byte is
// lenth: 4, offset: 12.
// 3byte is
// null: 4, offset: 12, lenth: 8.

namespace yaz0 {
enum compress_level { noCompress = 0, normal = 1, maximum = 2 };

class Yaz0Compress : public Yaz0Format {
private:
  mutable PackState packStateBuf;
  mutable bool isCanUseBuffer = false;

  // argment is now place
  PackState calclateCompressLenth(compress_level const optimizeCompressionLevel,
                                  std::vector<uint8_t> const &src,
                                  int32_t const srcPlace) const;
  PackState calclateCompressCore(std::vector<uint8_t> const &src,
                                 int32_t const srcPlace) const;

  std::vector<uint8_t> calclateElement(PackState const &);

protected:
public:
  Yaz0Compress() = default;
  Yaz0Compress(Yaz0Compress const &) = default;
  Yaz0Compress(Yaz0Compress &&) = default;
  std::vector<uint8_t> compress(compress_level const optimizeCompressionLevel,
                                std::vector<uint8_t> const &src);
};
} // namespace yaz0
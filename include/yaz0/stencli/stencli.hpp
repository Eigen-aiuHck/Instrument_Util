#include <cstdint>
namespace yaz0 {
namespace stencil {
enum class header {
  magic_begin = 0x00,
  uncompressSize_begin = 0x04,
  data_begin = 0x10,

  header_size = 0x10,
  magic_size = 0x04,
  uncompressSize_size = 0x04,
};

enum class pack {
  byte2_compressLenth_minSize = 0x02,
  byte3_compressLenth_minSize = 0x10 + byte2_compressLenth_minSize,
  compressLenth_maxSize = 0xff + byte3_compressLenth_minSize,
  referenceOffset_maxSize = 0x0fff,
  compressLenth_end = compressLenth_maxSize + 0x01,
  referenceOffset_end = referenceOffset_maxSize + 0x01,
  container_size = 0x08, // container = pack[container_size]
};
} // namespace stencil
} // namespace yaz0
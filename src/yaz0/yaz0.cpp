#include "yaz0/yaz0.hpp"

#include "yaz0/compress/yaz0_compress.hpp"
#include "yaz0/decompress/yaz0_decompress.hpp"
#include "yaz0/yaz0Format.hpp"

namespace yaz0 {
void Yaz0::m_pData_swap(base_type *src) noexcept {
  delete m_pData;
  m_pData = src;
}

std::vector<uint8_t> Yaz0::exportData() const { return m_pData->exportData(); }
bool Yaz0::import(std::vector<uint8_t> const &yaz0Data) {
  m_pData->import(yaz0Data);
}

std::vector<uint8_t>
Yaz0::compress(compress_level const optimizeCompressionLevel,
               std::vector<uint8_t> const &src) {
  Yaz0Compress *buffer = new Yaz0Compress;
  buffer->compress(optimizeCompressionLevel, src);
  m_pData_swap(buffer);
}

std::vector<uint8_t> Yaz0::decompress() const { return m_pData->decompress(); }
} // namespace yaz0
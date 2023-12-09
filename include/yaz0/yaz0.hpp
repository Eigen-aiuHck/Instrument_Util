#include "yaz0/compress/yaz0_compress.hpp"
#include "yaz0/decompress/yaz0_decompress.hpp"
#include "yaz0/yaz0Format.hpp"

namespace yaz0 {
class Yaz0 {
private:
  using base_type = Yaz0Decompress;

  base_type *m_pData;

protected:
  void m_pData_swap(base_type *) noexcept;

public:
  Yaz0() : m_pData(nullptr) {}
  Yaz0(Yaz0 const &) = default;
  Yaz0(Yaz0 &&) = default;

  virtual ~Yaz0() { delete m_pData; }
  std::vector<uint8_t> exportData() const;
  bool import(std::vector<uint8_t> const &yaz0Data);

  std::vector<uint8_t> compress(compress_level const optimizeCompressionLevel,
                                std::vector<uint8_t> const &src);
  std::vector<uint8_t> decompress() const;
};
} // namespace yaz0
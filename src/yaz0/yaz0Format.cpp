#include "yaz0/yaz0Format.hpp"
#include <cstdint>
#include <memory>
#include <vector>
namespace yaz0 {

bool Yaz0Format::isMatchMagic(std::vector<uint8_t> const &data) {
  if (data.size() < sizeof(magic)) {
    return false;
  }
  for (auto count = 0; auto const i : magic) {
    if (data[count] != i) {
      return false;
    }
    count++;
  }
  return true;
}

std::vector<uint8_t> Yaz0Format::exportData() const { return m_data; }

bool Yaz0Format::import(std::vector<uint8_t> const &yaz0Data) {
  if (isMatchMagic(yaz0Data)) {
    m_data = yaz0Data;
    return false;
  }
  return true;
}

bool Yaz0Format::import(std::vector<uint8_t> &&yaz0Data) {
  if (isMatchMagic(yaz0Data)) {
    m_data.swap(yaz0Data);
    return false;
  }
  return true;
}
} // namespace yaz0
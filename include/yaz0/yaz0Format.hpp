#pragma once
#include "array_tools/extention_defines/ext_if"
#include "yaz0/stencli/stencli.hpp"

#include <cstdint>
#include <memory>
#include <vector>

namespace yaz0 {
class Yaz0Format {
private:
protected:
  static inline constexpr char magic[5] = "Yaz0";
  std::vector<uint8_t> m_data;

  struct PackState {
    int32_t compressLenth = 0;
    int32_t referenceBeginPlace = 0;
  };

public:
  static bool isMatchMagic(std::vector<uint8_t> const &data);

  Yaz0Format() = default;
  Yaz0Format(Yaz0Format const &) = default;
  Yaz0Format(Yaz0Format &&) = default;

  virtual std::vector<uint8_t> exportData() const;
  virtual bool import(std::vector<uint8_t> const &yaz0Data);
  virtual bool import(std::vector<uint8_t> &&yaz0Data);
};
} // namespace yaz0
#pragma once
#include "rarc/rarcFormat.hpp"

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace rarc {
class RARC {
private:
  std::optional<std::vector<uint8_t>> m_data;
  std::optional<RARC_format> m_format;

protected:
public:
  bool set_data(std::vector<uint8_t> const &data,
                std::endian endian = std::endian::big);
  bool set_data_swap(std::vector<uint8_t> &data,
                     std::endian endian = std::endian::big);

  std::vector<std::vector<std::string>> get_tree_list();
};

/*
--------------------------------------------------------------------------------
*/

bool inline RARC::set_data(std::vector<uint8_t> const &data,
                           std::endian endian = std::endian::big) {
  m_data = data;
  m_format.emplace(RARC_format());
  if (m_format->set_data(*m_data, endian)) {
    return true;
  }
  m_data = std::nullopt;
  return false;
}
bool inline RARC::set_data_swap(std::vector<uint8_t> &data,
                                std::endian endian = std::endian::big) {
  RARC_format buf_format;
  if (buf_format.set_data(data, endian)) {
    m_format.emplace(buf_format);
    m_data.emplace(std::vector<uint8_t>());
    m_data->swap(data);
    return true;
  }
  return false;
}

} // namespace rarc
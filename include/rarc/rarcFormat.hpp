#pragma once
#include "array_tools/endian_convertion.hpp"
#include "rarc/stencil/stencil.hpp"

#include <bit>
#include <cstdint>
#include <functional>
#include <vector>

namespace rarc {
class RARC_format {
private:
protected:
public:
  using RARC_header_const_pointer = typename stencil::RARC_header const *;
  using header_const_pointer = typename stencil::tree::Header const *;
  using directory_node_const_pointer =
      typename stencil::tree::Directory_node const *;
  using node_const_pointer = typename stencil::tree::Node const *;

  static inline constexpr char magic[5] = "RARC";
  std::endian m_endian;

  union {
    RARC_header_const_pointer m_RARC_header;
    uint8_t const *m_RARC_header_byte;
  };
  union {
    header_const_pointer m_header;
    uint8_t const *m_header_byte;
  };
  union {
    directory_node_const_pointer m_directory_node;
    uint8_t const *m_directory_node_byte;
  };
  union {
    node_const_pointer m_node;
    uint8_t const *m_node_byte;
  };

  uint8_t const *m_name_list;
  uint8_t const *m_data_list;

  template <class T> T endian_convirsion(T const &val) {
    return binary_tools::conv_endian::dynamic_conv_endian(m_endian, val);
  }

  RARC_format()
      : m_RARC_header(nullptr), m_header(nullptr), m_directory_node(nullptr),
        m_node(nullptr), m_name_list(nullptr), m_data_list(nullptr) {}
  RARC_format(RARC_format const &) = default;
  RARC_format(RARC_format &&) = default;
  bool set_data(std::vector<uint8_t> const &target, std::endian endian) {
    for (auto i = 0; i < 4; i++) {
      if (target[i] != magic[i]) {
        return false;
      }
    }

    m_endian = endian;
    // base set
    m_RARC_header_byte = target.data();

    // header
    if (uint8_t const offset = endian_convirsion(m_RARC_header->header_offset);
        offset < target.size()) {
      m_header_byte = target.data() + offset;
    } else {
      init_all_nullptr();
      return false;
    }

    // director node
    if (uint8_t const offset =
            endian_convirsion(m_header->directory_node.place_offset);
        offset < target.size()) {
      m_directory_node_byte = m_header_byte + offset;
    } else {
      init_all_nullptr();
      return false;
    }

    // node
    if (uint8_t const offset = endian_convirsion(m_header->node.place_offset);
        offset < target.size()) {
      m_node_byte = m_header_byte + offset;
    } else {
      init_all_nullptr();
      return false;
    }

    // name list
    if (uint8_t const offset =
            endian_convirsion(m_header->name_list.place_offset);
        offset < target.size()) {
      m_name_list = m_header_byte + offset;
    } else {
      init_all_nullptr();
      return false;
    }

    // data list
    if (uint8_t const offset = endian_convirsion(m_RARC_header->data_list.size);
        offset < target.size()) {
      m_data_list = m_header_byte + offset;
    }

    return true;
  }

  void init_all_nullptr() {
    m_RARC_header = nullptr;
    m_header = nullptr;
    m_directory_node = nullptr;
    m_node = nullptr;
    m_name_list = nullptr;
    m_data_list = nullptr;
    return;
  }
};
} // namespace rarc
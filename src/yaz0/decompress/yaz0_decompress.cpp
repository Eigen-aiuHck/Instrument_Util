#include "yaz0/decompress/yaz0_decompress.hpp"
#include "array_tools/endian_convertion.hpp"
#include "array_tools/vector_u8_reinterpret_cast.hpp"

#include <bit>
#include <iostream>
#include <vector>

namespace yaz0 {
std::vector<uint8_t> Yaz0Decompress::decompress() const {
  if (!isMatchMagic(m_data)) {
    std::cout << "not match magic!\n";
    return std::vector<uint8_t>();
  }
  // init destination container and place.
  std::vector<uint8_t> dst = {};
  int32_t const dst_targetSize =
      binary_tools::conv_typename::from_vector_u8<int32_t>(
          binary_tools::conv_endian::dynamic_conv_endian(
              std::endian::big, m_data,
              static_cast<int32_t>(stencil::header::uncompressSize_begin),
              static_cast<int32_t>(stencil::header::uncompressSize_size)));
  dst.reserve(dst_targetSize);

  // pack decode variables
  int32_t m_data_place = static_cast<int32_t>(stencil::header::data_begin);
  int32_t pack_eachContainerPlace =
      static_cast<int32_t>(stencil::pack::container_size);
  int32_t noPackFlagPlace = 0;
  for (; dst.size() < dst_targetSize; pack_eachContainerPlace++) {

    // pack decode.
    if (pack_eachContainerPlace >=
        static_cast<int32_t>(stencil::pack::container_size)) {
      pack_eachContainerPlace = 0;
      noPackFlagPlace = m_data_place;
      m_data_place++;
    }

    // main branch
    // 0x00 is compression. 0x01 is raw.
    if ((m_data[noPackFlagPlace] & (0x80 >> pack_eachContainerPlace)) == 0x00) {
      PackState packState = {};
      if ((m_data[m_data_place] & 0xf0) != 0x00) {
        // 2 length "pack"
        packState.compressLenth =
            (((m_data[m_data_place] & 0xf0) >> 0x04) +
             static_cast<int32_t>(stencil::pack::byte2_compressLenth_minSize));
        packState.referenceBeginPlace =
            binary_tools::conv_typename::from_vector_u8<uint16_t>(
                binary_tools::conv_endian::dynamic_conv_endian(
                    std::endian::big, m_data, m_data_place, sizeof(uint16_t))) &
            0x0fff;
        m_data_place += 2;
      } else {
        // 3 length "pack"
        packState.referenceBeginPlace =
            binary_tools::conv_typename::from_vector_u8<uint16_t>(
                binary_tools::conv_endian::dynamic_conv_endian(
                    std::endian::big, m_data, m_data_place, sizeof(uint16_t))) &
            0x0fff;
        m_data_place += 2;
        packState.compressLenth =
            m_data[m_data_place++] +
            static_cast<int32_t>(stencil::pack::byte3_compressLenth_minSize);
      }

      // writing
      {
        int32_t const referencePlace =
            (dst.size() - 1) - packState.referenceBeginPlace;
        for (auto compressLenthNow = 0;
             compressLenthNow < packState.compressLenth; compressLenthNow++) {
          dst.push_back(dst[referencePlace + compressLenthNow]);
        }
      }
    } else {
      dst.push_back(m_data[m_data_place++]);
    }
  }
  dst.resize(dst_targetSize);

  return dst;
}
} // namespace yaz0
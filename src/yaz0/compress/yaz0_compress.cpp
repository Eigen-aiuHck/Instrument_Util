#include "yaz0/compress/yaz0_compress.hpp"
#include "array_tools/endian_convertion.hpp"
#include "array_tools/vector_u8_reinterpret_cast.hpp"
#include <cstdint>
#include <cstdio>
#include <future>
#include <list>
#include <memory>
#include <stdexcept>
#include <thread>
#include <vector>
namespace yaz0 {
// member: bit, ...
// 2byte is
// lenth: 4, offset: 12.
// 3byte is
// null: 4, offset: 12, lenth: 8.

std::vector<uint8_t>
Yaz0Compress::compress(compress_level const optimizeCompressionFlag,
                       std::vector<uint8_t> const &src) {
  int32_t srcPlace = 0;

  m_data.clear();
  m_data.reserve(0x10 + (src.size() / 0x08) + src.size());

  // header
  for (auto const i : magic) {
    m_data.push_back(i);
  }

  // init dinamic status
  uint8_t compressionFlag = 0xff;
  int32_t compressionFlagPlace =
      static_cast<int32_t>(stencil::header::data_begin);
  m_data.resize(static_cast<int32_t>(stencil::header::data_begin) + 0x01);

  // compression lambda
  auto writeCompressFlag = [&]() {
    m_data[compressionFlagPlace] = compressionFlag; // compress status write.
    compressionFlagPlace =
        m_data.size();      // Specify where to write compression flags.
    m_data.push_back(0x00); // element reservation.
    compressionFlag = 0x00; // init flag.
  };

  // const compless
  for (auto i = 0; i < static_cast<int32_t>(stencil::pack::container_size);
       i++) {
    m_data.push_back(src[srcPlace++]);
  }
  writeCompressFlag();

  int32_t packCount = 0;
  for (; srcPlace < src.size(); packCount++) {
    if (packCount >= static_cast<int32_t>(stencil::pack::container_size)) {
      packCount = 0; // init count.
      writeCompressFlag();
    }

    if (auto compressVal =
            calclateCompressLenth(optimizeCompressionFlag, src, srcPlace);
        compressVal.compressLenth < 0x03) {
      //  not compress.
      compressionFlag |= 0x80 >> packCount;
      m_data.push_back(src[srcPlace++]);
    } else {
      // compress to 2 or 3.
      for (auto buf = calclateElement(compressVal);
           auto const &buf_elem : buf) {
        m_data.push_back(buf_elem);
      }
      srcPlace += compressVal.compressLenth;
    }
  }
  m_data.shrink_to_fit();

  writeCompressFlag();
  m_data[compressionFlagPlace] |= 0xff & (0xff >> packCount);

  // writing uncompress Size.
  // auto const srcSizeDecompos_big = convertEndian(
  //     std::endian::big,
  //     arrayDecomposition(static_cast<uint32_t>(src.size())));

  // elementRelocation(m_data, uncompressSizeBegin, srcSizeDecompos_big,
  //                   uncompressSize_size);
  // auto const buf_uncompress_size =
  // binary_tools::conv_endian::dynamic_conv_endian(
  // std::endian::big, binary_tools::conv_typename::to_vector_u8(
  // static_cast<uint32_t>(src.size())));
  {
    using namespace binary_tools;
    auto const buf_uncompress_size = conv_endian::dynamic_conv_endian(
        std::endian::big,
        conv_typename::to_vector_u8(static_cast<uint32_t>(src.size())));

    for (auto i = 0;
         i < static_cast<int32_t>(stencil::header::uncompressSize_size); i++) {
      m_data[static_cast<int32_t>(stencil::header::uncompressSize_size) + i] =
          buf_uncompress_size[i];
    }
  }

  return m_data;
}

Yaz0Compress::PackState Yaz0Compress::calclateCompressLenth(
    compress_level const optimizeCompressionFlag,
    std::vector<uint8_t> const &src, int32_t const srcPlace) const {
  PackState compVal1 = {};
  if (optimizeCompressionFlag != compress_level::noCompress) {
    if (optimizeCompressionFlag == compress_level::maximum) {
      if (!isCanUseBuffer) {
        packStateBuf = calclateCompressCore(src, srcPlace);
        isCanUseBuffer = true;
      }

      if (packStateBuf.compressLenth >
          static_cast<int32_t>(stencil::pack::compressLenth_maxSize)) {
        throw std::runtime_error("Over Value!! : yaz0Enc");
      }

      auto compVal2 = calclateCompressCore(src, srcPlace + 1);
      int32_t ifCompByte3 = 0;
      // if difference size.
      if ((packStateBuf.compressLenth <
           static_cast<int32_t>(stencil::pack::byte3_compressLenth_minSize)) &&
          !(compVal2.compressLenth <
            static_cast<int32_t>(stencil::pack::byte3_compressLenth_minSize))) {
        ifCompByte3 = 1;
      }

      if (compVal2.compressLenth > (packStateBuf.compressLenth + ifCompByte3)) {
        // Skip compress.
        compVal1 = {};
        packStateBuf = compVal2;
      } else {
        compVal1 = packStateBuf;
        isCanUseBuffer = false;
      }

    } else {
      compVal1 = calclateCompressCore(src, srcPlace);
    }
  }

  return compVal1;
}

Yaz0Compress::PackState
Yaz0Compress::calclateCompressCore(std::vector<uint8_t> const &src,
                                   int32_t const srcPlace) const {
  // select maximum value
  PackState provisionalMaximum = {};
  for (int32_t refPlace = 0;
       refPlace < static_cast<int32_t>(stencil::pack::referenceOffset_end);
       refPlace++) {
    int32_t lenth = 0;
    {
      if (int32_t referencePlace =
              srcPlace - (refPlace + 1); // srcPlace - minimumreferenceOffset
          -1 < referencePlace) {
        while (
            (lenth < static_cast<int32_t>(stencil::pack::compressLenth_end)) &&
            ((srcPlace + lenth) < src.size())) {
          if (!(src[referencePlace + lenth] == src[srcPlace + lenth])) {
            // last matched element.
            lenth++;
            break;
          }
          lenth++;
        }
        lenth--;
      }
    }
    if (lenth > provisionalMaximum.compressLenth) {
      provisionalMaximum.compressLenth = lenth;
      provisionalMaximum.referenceBeginPlace = refPlace;
    }
  }

  return provisionalMaximum;
}

std::vector<uint8_t>
Yaz0Compress::calclateElement(PackState const &compressVal) {
  auto getByte2Lenth = [&compressVal]() {
    return static_cast<uint8_t>(
        ((compressVal.compressLenth -
          static_cast<int32_t>(stencil::pack::byte2_compressLenth_minSize))
         << 4) &
        0xf0);
  };
  auto getOffset0 = [&compressVal]() {
    return static_cast<uint8_t>((compressVal.referenceBeginPlace >> 8) & 0x0f);
  };
  auto getOffset1 = [&compressVal]() {
    return static_cast<uint8_t>(compressVal.referenceBeginPlace & 0xff);
  };
  auto getByte3Lenth = [&compressVal]() {
    return static_cast<uint8_t>(
        (compressVal.compressLenth -
         static_cast<int32_t>(stencil::pack::byte3_compressLenth_minSize)) &
        0xff);
  };

  std::vector<uint8_t> buffer;
  if (compressVal.compressLenth <
      static_cast<int32_t>(stencil::pack::byte3_compressLenth_minSize)) {
    // 2 byte format(byte2)
    buffer.resize(2, 0x00);
    buffer[0] = getByte2Lenth() | getOffset0();
    buffer[1] = getOffset1();
  } else {
    // 3 byte format(byte3)
    buffer.resize(3, 0x00);
    buffer[0] = getOffset0();
    buffer[1] = getOffset1();
    buffer[2] = getByte3Lenth();
  }

  return buffer;
}
} // namespace yaz0
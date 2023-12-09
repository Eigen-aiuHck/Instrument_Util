#include "array_tools/endian_convertion.hpp"
#include "array_tools/fstream_vector.hpp"

#include "yaz0/compress/yaz0_compress.hpp"
#include "yaz0/decompress/yaz0_decompress.hpp"
#include <bit>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <vector>

bool test_Yaz0() {
  // std::vector<uint8_t> arc_file;
  // if (!binary_tools::ifstream_vector("./testResource/test.arc", arc_file)) {
  //   std::cout << "Target not found!!\n./testResource/test.arc" << std::endl;
  //   return false;
  // }
  std::vector<uint8_t> const arc_file = []() {
    std::vector<uint8_t> buf;
    binary_tools::ifstream_vector("./testResource/test.arc", buf);
    return buf;
  }();

  // time type setup
  std::chrono::system_clock::time_point start, end;

  float time_result;

  yaz0::Yaz0Compress yaz0Comp;

  {
    std::cout << "compress Yaz0 normal.\n";

    start = std::chrono::system_clock::now();
    yaz0Comp.compress(yaz0::compress_level::normal, arc_file);
    end = std::chrono::system_clock::now();

    std::cout << "output result to file.\ntime : "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                       start)
                     .count()
              << "ms\n";
    binary_tools::ofstream_vector("./testResource/test_normal.yaz0",
                                  yaz0Comp.exportData());
  }
  {
    std::cout << "compress Yaz0 no compress.\n";

    start = std::chrono::system_clock::now();
    yaz0Comp.compress(yaz0::compress_level::noCompress, arc_file);
    end = std::chrono::system_clock::now();

    std::cout << "output result to file.\ntime : "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                       start)
                     .count()
              << "ms\n";
    binary_tools::ofstream_vector("./testResource/test_noCompress.yaz0",
                                  yaz0Comp.exportData());
  }
  {
    std::cout << "compress Yaz0 maximum.\n";

    start = std::chrono::system_clock::now();
    yaz0Comp.compress(yaz0::compress_level::maximum, arc_file);
    end = std::chrono::system_clock::now();

    std::cout << "output result to file.\ntime : "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                       start)
                     .count()
              << "ms\n";
    binary_tools::ofstream_vector("./testResource/test_maximum.yaz0",
                                  yaz0Comp.exportData());
  }

  // decompress
  {
    yaz0::Yaz0Decompress yaz0Decomp;
    yaz0Decomp.import(yaz0Comp.exportData());
    std::cout << "decompress Yaz0.\n";

    start = std::chrono::system_clock::now();
    auto const resultData = yaz0Decomp.decompress();
    end = std::chrono::system_clock::now();

    std::cout << "output result to file.\ntime : "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                       start)
                     .count()
              << "ms\n";
    binary_tools::ofstream_vector("./testResource/test_deCompress.yaz0",
                                  resultData);
  }
  return true;
}

int main() {
  test_Yaz0();
  return 0;
}
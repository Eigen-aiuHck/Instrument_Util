#pragma once
#include "./formatBlock/data.hpp"
#include "./formatBlock/field.hpp"
#include "./formatBlock/header.hpp"

#include <vector>

// remine::
// format series is like the stencils.

class BcsvFormat {
private:
  void initHeader();
  void initField();
  void initDataAndStringSection();

protected:
  std::vector<uint8_t> data;

  Header_static header;
  std::vector<EachFieldPlace_dynamic> field;
  std::vector<EachEntryPlace_dynamic> entry; // it's dataSection
  uint32_t stringSectionBegin;

public:
  BcsvFormat();
  BcsvFormat(std::vector<uint8_t> const &);
};

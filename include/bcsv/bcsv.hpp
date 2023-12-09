#include "./bcsvFormat.hpp"
#include <vector>

class bcsv : protected BcsvFormat {
private:
  /* data */
public:
  bcsv();
  bcsv(std::vector<uint8_t> const &);
  bcsv(std::vector<uint8_t> &&);
};
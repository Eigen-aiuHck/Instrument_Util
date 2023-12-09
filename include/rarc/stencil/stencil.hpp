#include <cstdint>

namespace rarc {
namespace stencil {
// offset is byte.
// index is it for array.

// 0x20
struct RARC_header {
  uint8_t magic[4];
  uint32_t size;
  uint32_t header_offset; // 0x20.
  struct Data_list {      // all offset is (x + 0x20).
    uint32_t place_offset;
    uint32_t size;
    uint32_t MRAM_size;
    uint32_t ARAM_size;
    uint32_t DVD_size;
  } data_list;
};

namespace tree {
// 0x20
struct Header {
  // 0x08
  struct section_status {
    uint32_t size;
    uint32_t place_offset;
  };
  section_status directory_node;
  section_status node;
  section_status name_list;
  uint16_t next_usable_file_index; // uk
  uint8_t sync_fileID_flag;
  uint8_t padding[5];
};

// 0x10
struct Directory_node {
  uint8_t head_name[4];
  uint32_t name_list_at_offset;
  uint16_t hash;
  uint16_t entry_size;
  uint32_t data_list_at_offset;
};

// 0x10
struct Node {
  uint16_t nodeIndex;
  uint16_t hash;
  uint8_t node_attributes_flag;
  uint8_t padding0;
  uint16_t name_list_at_offset;
  union {
    uint32_t data_list_in_offset;
    uint32_t directory_node_index;
  };
  union {
    uint32_t data_list_in_size;
    uint32_t directory_node_size;
  };
  uint8_t padding1[4];
};

} // namespace tree
} // namespace stencil
} // namespace rarc
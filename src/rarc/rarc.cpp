#include "rarc/rarc.hpp"

#include <cstdint>
#include <memory>
#include <optional>
#include <stack>
#include <string>
#include <vector>

namespace rarc {
struct callback_argment {
  uint32_t node_size;
  RARC_format reference;
  std::vector<std::string> current_path;
  std::vector<std::vector<std::string>> result;
};
void callback_path(callback_argment &arg, uint32_t &current_loop_count) {
  if (arg.node_size <= current_loop_count) {
    // terminator.
    return;
  }
  
}

std::vector<std::vector<std::string>> RARC::get_tree_list() {
  if (!m_data.has_value() || !m_format.has_value()) {
    return {{{}}};
  }
  uint32_t const node_size =
      m_format->endian_convirsion(m_format->m_header->node.size);
}
} // namespace rarc
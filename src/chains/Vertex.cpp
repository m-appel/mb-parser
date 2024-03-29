#include "Vertex.h"

void Vertex::print() {
  std::cout << frame_ << "\n";
  for (const auto &vertex : successors_) {
    std::cout << "  ->" << vertex->getFrame() << "\n";
  }
}

uint32_t Vertex::walk(uint32_t max_weight_influence, uint32_t mb_threshold, uint32_t depth) {
  if (depth > max_depth_) {
    max_depth_ = depth;
  }
  uint32_t ret = 0;
  for (size_t i = 0; i < successors_.size(); i++) {
    uint32_t new_weight_influence = (weights_[i] < max_weight_influence) ? weights_[i] : max_weight_influence;
    ret += new_weight_influence;
    if (new_weight_influence < mb_threshold) {
      continue;
    }
    auto &weight_map = successor_chain_weights_[i];
    auto find_it = weight_map.find(new_weight_influence);
    if (find_it != weight_map.end()) {
      // The recursion has been calculated with this weight influence before.
      ret += find_it->second;
      continue;
    }
    uint32_t new_weight = successors_[i]->walk(new_weight_influence, mb_threshold, depth + 1);
    weight_map.emplace(new_weight_influence, new_weight);
    ret += new_weight;
  }
  return ret;
}

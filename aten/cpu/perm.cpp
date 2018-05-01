#ifndef PERM_CPP
#define PERM_CPP

#include <torch/torch.h>

inline std::tuple<at::Tensor, at::Tensor>
randperm(at::Tensor row, at::Tensor col, int num_nodes) {
  // Randomly reorder row and column indices.
  auto perm = at::randperm(torch::CPU(at::kLong), row.size(0));
  row = row.index_select(0, perm);
  col = col.index_select(0, perm);

  // Randomly swap row values.
  auto node_rid = at::randperm(torch::CPU(at::kLong), num_nodes);
  row = node_rid.index_select(0, row);

  // Sort row and column indices row-wise.
  std::tie(row, perm) = row.sort();
  col = col.index_select(0, perm);

  // Revert row value swaps.
  row = std::get<1>(node_rid.sort()).index_select(0, row);

  return {row, col};
}

#endif // PERM_CPP

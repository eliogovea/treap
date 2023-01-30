#pragma once

#include <memory>

namespace ImplicitKeyTreap
{
/// @brief Implicit keys treap node
/// @tparam LocalValue
/// @tparam SubTreeValue
/// @tparam LazyTagValue
template <typename LocalValue, typename SubTreeValue, typename LazyTagValue>
struct Node {
    struct LocalValueWrapper {
        LocalValue value;
        size_t     priority;
    };

    struct SubTreeValueWrapper {
        SubTreeValue value;
        size_t       size;
    };

    LocalValueWrapper   local;
    SubTreeValueWrapper sub_tree;
    LazyTagValue        lazy_tag;

    std::unique_ptr<Node> child_l;
    std::unique_ptr<Node> child_r;
};

}  // namespace ImplicitKeyTreap

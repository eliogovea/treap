#pragma once

#include <concepts>

#include "node.hpp"

namespace ImplicitKeyTreap
{
/// @brief example of operations that don't modify anything,
/// but define all functions required to make the treap work.
/// TODO: add a concept
/// @tparam LocalValue
/// @tparam SubTreeValue
/// @tparam LazyTagValue
template <typename LocalValue, typename SubTreeValue, typename LazyTagValue>
struct DoNothing {
    static void Update(SubTreeValue& lhs, const LocalValue& rhs)
    {
        // NOTHING
    }

    static void Update(SubTreeValue& lhs, const SubTreeValue& rhs)
    {
        // NOTHING
    }

    // Lazy Operations

    static void Update(LocalValue& lhs, const LazyTagValue& rhs)
    {
        // NOTHING
    }

    static void Update(SubTreeValue& lhs, const LazyTagValue& rhs)
    {
        // NOTHING
    }

    static void Update(LazyTagValue& lhs, const LazyTagValue& rhs)
    {
        // NOTHING
    }

    /// @brief Modify the tree structure
    /// @param child_l
    /// @param child_r
    /// @param lazy_tag
    static void Update(
      std::unique_ptr<Node<LocalValue, SubTreeValue, LazyTagValue>>& child_l,
      std::unique_ptr<Node<LocalValue, SubTreeValue, LazyTagValue>>& child_r,
      const LazyTagValue&                                            lazy_tag)
    {
        // NOTHING
    }
};

}  // namespace ImplicitKeyTreap
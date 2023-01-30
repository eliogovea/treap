#pragma once

#include <functional>
#include <memory>
#include <tuple>

#include "node.hpp"

namespace ImplicitKeyTreap
{
template <typename LocalValue,
          typename SubTreeValue,
          typename LazyTagValue,
          typename TreeOperations>
struct ImplicitKeyTreap {
    using NodeType = Node<LocalValue, SubTreeValue, LazyTagValue>;

    /// @brief Split the range represented by root
    /// [begin, end) -> {[begin, index), [index, end)}
    /// @param root Root of the implicit key treap
    /// @param index Split index
    /// @return roots of each implicit key treap
    static auto Split(std::unique_ptr<NodeType> root, size_t index)
      -> std::tuple<std::unique_ptr<NodeType>, std::unique_ptr<NodeType>>
    {
        PushLazyTag(root);

        if (index == 0) {
            return {nullptr, std::move(root)};
        }

        auto size_l =
          root->child_l != nullptr ? root->child_l->sub_tree.size : 0;

        std::unique_ptr<NodeType> lhs;
        std::unique_ptr<NodeType> rhs;

        if (index <= size_l) {
            rhs = std::move(root);

            std::tie(lhs, rhs->child_l) =  //
              Split(std::move(rhs->child_l), index);

            PullChildrenValues(*rhs);
        } else {
            lhs = std::move(root);

            std::tie(lhs->child_r, rhs) =
              Split(std::move(lhs->child_r), index - 1 - size_l);

            PullChildrenValues(*lhs);
        }

        return {std::move(lhs), std::move(rhs)};
    }

    /// @brief Merge two treaps into one
    /// @param lhs root of left treap
    /// @param rhs root of right treap
    /// @return root of the merged treap
    static auto Merge(std::unique_ptr<NodeType> lhs,
                      std::unique_ptr<NodeType> rhs)
      -> std::unique_ptr<NodeType>
    {
        if (lhs == nullptr && rhs == nullptr) {
            return nullptr;
        }

        if (lhs != nullptr && rhs == nullptr) {
            PushLazyTag(*lhs);
            return lhs;
        }

        if (lhs == nullptr && rhs != nullptr) {
            PushLazyTag(*rhs);
            return rhs;
        }

        std::unique_ptr<NodeType> root;

        if (lhs->local.priority > rhs->local.priority) {
            root = std::move(lhs);
            PushLazyTag(*root);
            root->child_r = Merge(std::move(root->child_r), std::move(rhs));
        } else {
            root = std::move(rhs);
            PushLazyTag(*root);
            root->child_l = Merge(std::move(lhs), std::move(root->child_l));
        }

        PullChildrenValues(*root);

        return root;
    }

    /// @brief Get the node at index `index`
    /// @param root
    /// @param index
    /// @return Reference to the requested node
    static auto GetUnsafe(const std::unique_ptr<NodeType>& root, size_t index)
      -> NodeType&
    {
        // TODO: change to iterative version ?

        auto size_l =
          root->child_l != nullptr ? root->child_l->sub_tree.size : 0;

        if (index == size_l) {
            return *root;
        }

        return index < size_l ? GetUnsafe(root->child_l, index)
                              : GetUnsafe(root->child_r, index - size_l - 1);
    }

    /// @brief Copy a tree
    /// @param root
    /// @return
    static auto Copy(const std::unique_ptr<NodeType>& root)
      -> std::unique_ptr<NodeType>
    {
        if (root == nullptr) {
            return nullptr;
        }

        return std::make_unique(NodeType{root->local,
                                         root->sub_tree,
                                         root->lazy_tag,
                                         Copy(root->child_l),
                                         Copy(root->child_r)});
    }

    /// @brief Update local value using children values
    /// @param root
    static void PullChildrenValues(NodeType& root)
    {
        root.sub_tree.value = SubTreeValue{};
        TreeOperations::Update(root.sub_tree.value, root.local.value);

        root.sub_tree.size = 1;

        if (root.child_l != nullptr) {
            TreeOperations::Update(root.sub_tree.value,
                                   root.child_l->sub_tree.value);
            root.sub_tree.size += root.child_l->sub_tree.size;
        }

        if (root.child_r != nullptr) {
            TreeOperations::Update(root.sub_tree.value,
                                   root.child_r->sub_tree.value);
            root.sub_tree.size += root.child_r->sub_tree.size;
        }
    }

    /// @brief Update local value using children values
    /// @param root
    static void PullChildrenValues(const std::unique_ptr<NodeType>& root)
    {
        if (root != nullptr) {
            PullChildrenValues(*root);
        }
    }

    /// @brief Push lazy tag
    /// @param root
    static void PushLazyTag(NodeType& root)
    {
        TreeOperations::Update(root.local.value, root.lazy_tag);
        TreeOperations::Update(root.sub_tree.value, root.lazy_tag);

        TreeOperations::Update(root.child_l, root.child_r, root.lazy_tag);

        if (root.child_l != nullptr) {
            TreeOperations::Update(root.child_l->lazy_tag, root.lazy_tag);
        }

        if (root.child_r != nullptr) {
            TreeOperations::Update(root.child_r->lazy_tag, root.lazy_tag);
        }

        root.lazy_tag = LazyTagValue{};
    }

    /// @brief Push lazy tag
    /// @param root
    static void PushLazyTag(std::unique_ptr<NodeType>& root)
    {
        if (root != nullptr) {
            PushLazyTag(*root);
        }
    }

    template <typename Callback>
    static void InOrderTraversal(std::unique_ptr<NodeType>& root,
                                 Callback&&                 callback)
    {
        if (root == nullptr) {
            return;
        }

        PushLazyTag(root);

        InOrderTraversal(root->child_l, std::forward<Callback>(callback));

        std::invoke(callback, root);

        InOrderTraversal(root->child_r, std::forward<Callback>(callback));
    }

    template <typename Callback>
    static void ExposeRange(std::unique_ptr<NodeType>& root,
                            size_t                     begin,
                            size_t                     end,
                            Callback&&                 callback)
    {
        std::unique_ptr<NodeType> lhs;
        std::unique_ptr<NodeType> rhs;

        std::tie(root, rhs) = Split(std::move(root), end);
        std::tie(lhs, root) = Split(std::move(root), begin);

        std::invoke(callback, root);

        root = Merge(std::move(lhs), std::move(root));
        root = Merge(std::move(root), std::move(rhs));
    }
};

}  // namespace ImplicitKeyTreap

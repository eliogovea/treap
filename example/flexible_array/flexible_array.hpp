#pragma once

#include <memory>

#include "implicit_key_treap/tree.hpp"
#include "implicit_key_treap/tree_operations.hpp"
#include "implicit_key_treap/utilities.hpp"

namespace FlexibleArray
{

namespace Detail
{

struct Empty {
};

struct DoNothing {
    template <typename LValue, typename RValue>
    static void Update(LValue&, const RValue&)
    {
    }

    template <typename NodePointer, typename TagValue>
    static void Update(NodePointer&, NodePointer&, const TagValue&)
    {
    }
};

}  // namespace Detail

template <typename Value,
          typename RandomNumberGenerator =
            Utilities::DefaultRandomNumberGenerator<size_t>>
class FlexibleArray
{
    using TreapType =  //
      ImplicitKeyTreap::ImplicitKeyTreap<Value,
                                         Detail::Empty,
                                         Detail::Empty,
                                         Detail::DoNothing>;

    using NodeType = typename TreapType::NodeType;

   public:
    FlexibleArray() = default;

    explicit FlexibleArray(RandomNumberGenerator rng)
        : m_root(nullptr)  //
        , m_rng(std::move(rng))
    {
    }

    FlexibleArray(const FlexibleArray& other)
        : m_root(TreapType::Copy(other.m_root))  //
        , m_rng(other.m_rng)
    {
    }

    FlexibleArray& operator=(const FlexibleArray& other)
    {
        m_root = TreapType::Copy(other.m_root);
        m_rng  = other.m_rng;
        return *this;
    }

    FlexibleArray(FlexibleArray&& other)   //
        : m_root(std::move(other.m_root))  //
        , m_rng(std::move(other.m_rng))
    {
    }

    FlexibleArray& operator=(FlexibleArray&& other)
    {
        m_root = std::move(other.m_root);
        m_rng  = std::move(other.m_rng);
        return *this;
    }

    size_t Size() const
    {
        return m_root != nullptr ? m_root->sub_tree.size : 0;
    }

    auto operator[](size_t index) -> Value&
    {
        return TreapType::GetUnsafe(m_root, index).local.value;
    }

    auto operator[](size_t index) const -> const Value&
    {
        return TreapType::GetUnsafe(m_root, index).local.value;
    }

    void Insert(Value value, size_t index)
    {
        Insert(MakeNode(value), index);
    }

    void Insert(const FlexibleArray& other, size_t index)
    {
        Insert(TreapType::Copy(other.m_root), index);
    }

    // TODO: check
    void Insert(FlexibleArray&& other, size_t index)
    {
        Insert(std::move(other.m_root), index);
    }

    void PushBack(Value value)
    {
        PushBack(MakeNode(std::move(value)));
    }

    void Remove(size_t begin, size_t end)
    {
        TreapType::ExposeRange(
          m_root,  //
          begin,
          end,
          [](std::unique_ptr<NodeType>& root) { root = nullptr; });
    }

    void Remove(size_t index)
    {
        Remove(index, index + 1);
    }

    template <typename Output>
    void To(Output&& output)
    {
        TreapType::InOrderTraversal(  //
          m_root,
          [&output](std::unique_ptr<NodeType>& root) {
              *output = root->local.value;
              ++output;
          });
    }

   private:
    auto MakeNode(Value value) -> std::unique_ptr<NodeType>
    {
        auto result = std::make_unique<NodeType>();

        result->local.value    = std::move(value);
        result->local.priority = m_rng();
        result->sub_tree.size  = 1;
        result->child_l        = nullptr;
        result->child_r        = nullptr;

        return result;
    }

    void Insert(std::unique_ptr<NodeType> root, size_t index)
    {
        if (index == 0) {
            PushFront(std::move(root));
            return;
        }

        if (Size() < index) {
            // TODO: throw exception ?
            return;
        }

        if (Size() == index) {
            PushBack(std::move(root));
            return;
        }

        std::unique_ptr<NodeType> rhs = nullptr;
        std::tie(m_root, rhs) = TreapType::Split(std::move(m_root), index);

        m_root = TreapType::Merge(std::move(m_root), std::move(root));
        m_root = TreapType::Merge(std::move(m_root), std::move(rhs));
    }

    void PushFront(std::unique_ptr<NodeType> root)
    {
        m_root = TreapType::Merge(std::move(root), std::move(m_root));
    }

    void PushBack(std::unique_ptr<NodeType> root)
    {
        m_root = TreapType::Merge(std::move(m_root), std::move(root));
    }

    std::unique_ptr<NodeType> m_root = nullptr;
    RandomNumberGenerator     m_rng  = {};
};

}  // namespace FlexibleArray
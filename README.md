# treap

This is an attempt to implement a generic and reusable [implicit-treap](https://en.wikipedia.org/wiki/Treap#Implicit_treap) using (modern) C++

## Build
```sh
mkdir build
cmake -DTREAP_BUILD_EXAMPLES:BOOL=ON -S . -B ./build
cmake --build ./build --parallel $(($(nproc)*2))
```

NOTE: requires installing [googletest](https://github.com/google/googletest)


## Test
```sh
ctest --test-dir ./build/ --parallel $(($(nproc)*2))
```


## Usage

### Node

Each `Node` instance provices information about a node in the tree

```cpp
/// @brief Implicit keys treap node
/// @tparam LocalValue
/// @tparam SubTreeValue
/// @tparam LazyTagValue
template <typename LocalValue, typename SubTreeValue, typename LazyTagValue>
struct Node;
```

`LocalValue` is the value you need to store in each node
`SubTreeValue` is a combination of `LocalValue` for all nodes in a subtree
`LazyTag` is a tag for a lazy operation


### ImplicitKeyTreap

`ImplicitKeyTreap` defines the basic operations supported by a `treap`

```cpp
template <typename LocalValue,
          typename SubTreeValue,
          typename LazyTagValue,
          typename TreeOperations>
struct ImplicitKeyTreap;
```

### TreeOperations

`TreeOperations` type defines the operations to modify the tree values implementing a set of `Update` function as in the following example:

```cpp
template <typename LocalValue, typename SubTreeValue, typename LazyTagValue>
struct TreeOperationsExample {
    static void Update(SubTreeValue& lhs, const LocalValue& rhs);
    static void Update(SubTreeValue& lhs, const SubTreeValue& rhs);
    static void Update(LocalValue& lhs, const LazyTagValue& rhs);
    static void Update(SubTreeValue& lhs, const LazyTagValue& rhs);
    static void Update(LazyTagValue& lhs, const LazyTagValue& rhs);

    // you can use `lazy_tag` to represent changes to the tree structure
    // for example: swapping nodes to reverse an interval
    static void Update(
      std::unique_ptr<Node<LocalValue, SubTreeValue, LazyTagValue>>& child_l,
      std::unique_ptr<Node<LocalValue, SubTreeValue, LazyTagValue>>& child_r,
      const LazyTagValue&                                            lazy_tag);
};
```
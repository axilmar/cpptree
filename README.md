# CPPTREE

version 1.0

A simple c++17 N-ary tree node class.

Since a tree is useful in many projects, and the c++ stl library does not even provide a basic one, I created this repo just to share a simple `node<T>` class that allows the creation of N-ary trees.

It's a source code only library.

The `node<T>` class' interface is very simple:

```cpp
get_parent()
get_prev_sibling()
get_next_sibling()
get_first_child()
get_last_child()
get_root()
empty()
not_empty()
virtual insert_child(child, next = nullptr)
virtual prepend_child(child)
virtual append_child(child)
virtual insert_all_children(parent, next = nullptr)
virtual prepend_all_children(parent)
virtual append_all_children(parent)
virtual remove_child(child)
virtual remove_all_children()
remove_from_parent()
virtual clone()
```

Pointers to nodes are of type `std::shared_ptr<T>`, because an app might want to share a node between different parts of the code.

Modifier methods are virtual in case a subclass may add additional code to all these cases.

The `clone()` function simply throws an exception. Subclasses can implement proper cloning, if desired.

Example:

```cpp
#include "cpptree.hpp"

class my_node : public cpptree::node<my_class> {
};

int main() {
	std::shared_ptr<my_node> root = std::make_shared<my_node>();
    root->insert_child(std::make_shared<my_node>());
    root->insert_child(std::make_shared<my_node>());
    root->remove_child(root->get_first_child());
    root->remove_child(root->get_last_child());
    root->remove_all_children();
}
```

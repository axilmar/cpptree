#ifndef CPPTREE_HPP
#define CPPTREE_HPP


#include <memory>
#include <stdexcept>


namespace cpptree {


    /**
     * A tree node class.
     * @param T class that extends the node class.
     */ 
    template <class T> class node : public std::enable_shared_from_this<T> {
    public:
        /**
         * Type of node pointer.
         */ 
        using node_ptr = std::shared_ptr<T>;

        /**
         * Returns a pointer to the parent.
         * @return a pointer to the parent.
         */ 
        node_ptr get_parent() const {
            return m_parent ? m_parent->shared_from_this() : nullptr;
        }

        /**
         * Returns a pointer to the previous sibling.
         * @return a pointer to the previous sibling.
         */ 
        const node_ptr& get_prev_sibling() const {
            return m_prev_sibling;
        }

        /**
         * Returns a pointer to the next sibling.
         * @return a pointer to the next sibling.
         */ 
        const node_ptr& get_next_sibling() const {
            return m_next_sibling;
        }

        /**
         * Returns a pointer to the first child.
         * @return a pointer to the first child.
         */ 
        const node_ptr& get_first_child() const {
            return m_first_child;
        }

        /**
         * Returns a pointer to the last child.
         * @return a pointer to the last child.
         */ 
        const node_ptr& get_last_child() const {
            return m_last_child;
        }

        /**
         * Returns a pointer to the root node of this tree.
         * @return a pointer to the root node of this tree.
         */ 
        node_ptr get_root() const {
            T* node = const_cast<T*>(static_cast<const T*>(this));
            T* root;
            do {
                root = node;
                node = node->m_parent;
            } while (node);
            return root->shared_from_this();
        }

        /**
         * Checks if the node has children.
         * @return true if the node does not have any children, false otherwise.
         */ 
        bool empty() const {
            return (bool)m_first_child == false;
        }

        /**
         * Checks if the node has children.
         * @return true if the node has children, false otherwise.
         */ 
        bool not_empty() const {
            return !empty();
        }

        /**
         * Interface for cloning the tree.
         * The default implementation simply throws an exception.
         * @exception std::logic_error thrown by default to indicate an unimplemented function.
         */ 
        virtual node_ptr clone() const {
            throw std::logic_error("cpptree::node::clone(): not implemented.");
        }

        /**
         * Inserts a child node before another child node.
         * @param child child to insert.
         * @param next the next child; if null, the new child is added as the last child.
         * @exception std::invalid_argument thrown if the child pointer is null, or if the child is not an orphan,
         *  or if next is not a child of this.
         */ 
        virtual void insert_child(const node_ptr& child, const node_ptr& next = nullptr) {
            //child must be non-null
            if (!child) {
                throw std::invalid_argument("cpptree::node::insert_child(child, next): null child pointer.");
            }
            
            //child must be orphan 
            if (child->m_parent) {
                throw std::invalid_argument("cpptree::node::insert_child(child, next): child not an orphan.");
            }
            
            //next must be a child of this
            if (next && next->m_parent != this) {
                throw std::invalid_argument("cpptree::node::insert_child(child, next): next not a child.");
            }

            //set the pointer
            child->m_parent = static_cast<T*>(this);

            //prev is either the next node's previous node or the last child
            const node_ptr& prev = next ? next->m_prev_sibling : m_last_child;

            //set previous/next of child
            child->m_prev_sibling = prev;
            child->m_next_sibling = next;

            //connect previous with new child
            if (prev) {
                prev->m_next_sibling = child;
            }
            else {
                m_first_child = child;
            }

            //connect next with new child
            if (next) {
                next->m_prev_sibling = child;
            }
            else {
                m_last_child = child;
            }
        }

        /**
         * Adds a child as a first child.
         * Same as `insert_child(child, get_first_child())`.
         * @param child the child to add.
         */ 
        virtual void prepend_child(const node_ptr& child) {
            insert_child(child, m_first_child);
        }

        /**
         * Adds a child as a last child.
         * Same as `insert_child(child, nullptr)`.
         * @param child the child to add.
         */ 
        virtual void append_child(const node_ptr& child) {
            insert_child(child, nullptr);
        }

        /**
         * Removes the children of a node and inserts them to this, before the given next node.
         * @param parent the parent node to tranfer nodes from.
         * @param next the next child; if null, the children are added at the end of the child list.
         * @exception std::invalid_argument thrown if parent is null, if parent is this, or if next is not a child of this.
         */ 
        virtual void insert_all_children(const node_ptr& parent, const node_ptr& next = nullptr) {
            //parent must not be null
            if (!parent) {
                throw std::invalid_argument("cpptree::node::insert_all_children(parent, next): null parent pointer.");
            }
            
            //parent must not be this
            if (parent.get() == static_cast<T*>(this)) {
                throw std::invalid_argument("cpptree::node::insert_all_children(parent, next): parent same as this.");
            }

            //next must be a child of this
            if (next && next->m_parent != this) {
                throw std::invalid_argument("cpptree::node::insert_all_children(parent, next): next not a child.");
            }

            //parent is empty, no further action required
            if (parent->empty()) {
                return;
            }

            //set the parent pointer of the given child nodes
            for (T* childp = parent->m_first_child.get(); childp; childp = childp->m_next_sibling.get()) {
                childp->m_parent = static_cast<T*>(this);
            }

            //prev is either the next node's previous node or the last child
            const node_ptr& prev = next ? next->m_prev_sibling : m_last_child;

            //connect the first and last children with the parent's list
            parent->m_first_child->m_prev_sibling = prev;
            parent->m_last_child->m_next_sibling = next;

            //connect the previous with the parent's list
            if (prev) {
                prev->m_next_sibling = parent->m_first_child;
            }
            else {
                m_first_child = parent->m_first_child;
            }

            //connect the next with the parent's list
            if (next) {
                next->m_prev_sibling = parent->m_last_child;
            }
            else {
                m_last_child = parent->m_last_child;
            }

            //reset the list of the parent
            parent->m_first_child.reset();
            parent->m_last_child.reset();
        }

        /**
         * Moves the children of the given parent to this parent, at the start of this parent's child list.
         * Same as `insert_all_children(parent, get_first_child())`.
         * @param parent the parent to transfer the children from.
         */ 
        virtual void prepend_all_children(const node_ptr& parent) {
            insert_all_children(parent, m_first_child);
        }

        /**
         * Moves the children of the given parent to this parent, at the end of the this parent's child list.
         * Same as `insert_all_children(parent, nullptr)`.
         * @param parent the parent to transfer the children from.
         */ 
        virtual void append_all_children(const node_ptr& parent) {
            insert_all_children(parent, nullptr);
        }

        /**
         * Removes a child.
         * @param child the child to remove.
         * @exception std::invalid_argument thrown if child is null, or child belongs to another parent.
         */ 
        virtual void remove_child(const node_ptr& child) {
            //child is null
            if (!child) {
                throw std::invalid_argument("cpptree::node::remove_child(child): null child pointer.");
            }
            
            //child belongs to another parent
            if (child->m_parent != static_cast<T*>(this)) {
                throw std::invalid_argument("cpptree::node::remove_child(child): child belongs to a different parent.");
            }

            //reset the parent pointer
            child->m_parent = nullptr;

            //connect the child's previous node to the child's next node
            if (child->m_prev_sibling) {
                child->m_prev_sibling->m_next_sibling = child->m_next_sibling;
            }
            else {
                m_first_child = child->m_next_sibling;
            }

            //connect the child's next node to the child's previous node
            if (child->m_next_sibling) {
                child->m_next_sibling->m_prev_sibling = child->m_prev_sibling;
            }
            else {
                m_last_child = child->m_prev_sibling;
            }

            //reset the child's pointers
            child->m_prev_sibling.reset();
            child->m_next_sibling.reset();
        }

        /**
         * Removes all children.
         */ 
        virtual void remove_all_children() {
            //if empty, do nothing
            if (empty()) {
                return;
            }

            //reset each child's pointers
            for (node_ptr childp = m_first_child; childp; ) {
                node_ptr next = childp->m_next_sibling;
                childp->m_parent = nullptr;
                childp->m_prev_sibling.reset();
                childp->m_next_sibling.reset();
                childp = next;
            }

            //reset the child list
            m_first_child.reset();
            m_last_child.reset();
        }

        /**
         * Removes this node from its parent, if it has one.
         */ 
        void remove_from_parent() {
            if (m_parent) {
                m_parent->remove_child(this->shared_from_this());
            }
        }

    private:
        T* m_parent{ nullptr };
        node_ptr m_prev_sibling;
        node_ptr m_next_sibling;
        node_ptr m_first_child;
        node_ptr m_last_child;
    };


} //namespace cpptree


#endif //CPPTREE_HPP

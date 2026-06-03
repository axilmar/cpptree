#include <string>
#include <cassert>
#include "cpptree.hpp"
using namespace cpptree;


class test_node : public node<test_node> {
public:
    const std::string tag;

    test_node(const std::string& tag = {}) : tag(tag) {
    }
};


static void check_node(
    const std::shared_ptr<test_node>& node, 
    const std::string& tag, 
    const std::shared_ptr<test_node>& parent, 
    const std::shared_ptr<test_node>& prev_sibling, 
    const std::shared_ptr<test_node>& next_sibling, 
    const std::shared_ptr<test_node>& first_child, 
    const std::shared_ptr<test_node>& last_child)
{
    assert(node);
    assert(node->tag == tag);
    assert(node->get_parent() == parent);
    assert(node->get_prev_sibling() == prev_sibling);
    assert(node->get_next_sibling() == next_sibling);
    assert(node->get_first_child() == first_child);
    assert(node->get_last_child() == last_child);
    assert(node->empty() == (node->get_first_child().get() == nullptr));
    assert(node->not_empty() == (node->get_first_child().get() != nullptr));
}


static void test_create_node() {
    std::shared_ptr<test_node> root = std::make_shared<test_node>("root");
    check_node(root, "root", nullptr, nullptr, nullptr, nullptr, nullptr);
}


static void test_insert_child() {
    //insert last
    {
        std::shared_ptr<test_node> root = std::make_shared<test_node>("root");

        std::shared_ptr<test_node> child1 = std::make_shared<test_node>("child1");
        std::shared_ptr<test_node> child2 = std::make_shared<test_node>("child2");
        std::shared_ptr<test_node> child3 = std::make_shared<test_node>("child3");

        root->insert_child(child1);
        check_node(root, "root", nullptr, nullptr, nullptr, child1, child1);
        check_node(child1, "child1", root, nullptr, nullptr, nullptr, nullptr);

        root->insert_child(child2);
        check_node(root, "root", nullptr, nullptr, nullptr, child1, child2);
        check_node(child1, "child1", root, nullptr, child2, nullptr, nullptr);
        check_node(child2, "child2", root, child1, nullptr, nullptr, nullptr);

        root->insert_child(child3);
        check_node(root, "root", nullptr, nullptr, nullptr, child1, child3);
        check_node(child1, "child1", root, nullptr, child2, nullptr, nullptr);
        check_node(child2, "child2", root, child1, child3, nullptr, nullptr);
        check_node(child3, "child3", root, child2, nullptr, nullptr, nullptr);
    }

    //insert first
    {
        std::shared_ptr<test_node> root = std::make_shared<test_node>("root");

        std::shared_ptr<test_node> child1 = std::make_shared<test_node>("child1");
        std::shared_ptr<test_node> child2 = std::make_shared<test_node>("child2");
        std::shared_ptr<test_node> child3 = std::make_shared<test_node>("child3");

        root->insert_child(child1);
        check_node(root, "root", nullptr, nullptr, nullptr, child1, child1);
        check_node(child1, "child1", root, nullptr, nullptr, nullptr, nullptr);

        root->insert_child(child2, child1);
        check_node(root, "root", nullptr, nullptr, nullptr, child2, child1);
        check_node(child1, "child1", root, child2, nullptr, nullptr, nullptr);
        check_node(child2, "child2", root, nullptr, child1, nullptr, nullptr);

        root->insert_child(child3, child2);
        check_node(root, "root", nullptr, nullptr, nullptr, child3, child1);
        check_node(child1, "child1", root, child2, nullptr, nullptr, nullptr);
        check_node(child2, "child2", root, child3, child1, nullptr, nullptr);
        check_node(child3, "child3", root, nullptr, child2, nullptr, nullptr);
    }

    //insert middle
    {
        std::shared_ptr<test_node> root = std::make_shared<test_node>("root");

        std::shared_ptr<test_node> child1 = std::make_shared<test_node>("child1");
        std::shared_ptr<test_node> child2 = std::make_shared<test_node>("child2");
        std::shared_ptr<test_node> child3 = std::make_shared<test_node>("child3");

        root->insert_child(child1);
        check_node(root, "root", nullptr, nullptr, nullptr, child1, child1);
        check_node(child1, "child1", root, nullptr, nullptr, nullptr, nullptr);

        root->insert_child(child2);
        check_node(root, "root", nullptr, nullptr, nullptr, child1, child2);
        check_node(child1, "child1", root, nullptr, child2, nullptr, nullptr);
        check_node(child2, "child2", root, child1, nullptr, nullptr, nullptr);

        root->insert_child(child3, child2);
        check_node(root, "root", nullptr, nullptr, nullptr, child1, child2);
        check_node(child1, "child1", root, nullptr, child3, nullptr, nullptr);
        check_node(child2, "child2", root, child3, nullptr, nullptr, nullptr);
        check_node(child3, "child3", root, child1, child2, nullptr, nullptr);
    }
}


static void test_insert_all_children() {
    //insert last
    {
        std::shared_ptr<test_node> root1 = std::make_shared<test_node>("root1");
        std::shared_ptr<test_node> child11 = std::make_shared<test_node>("child11");
        std::shared_ptr<test_node> child12 = std::make_shared<test_node>("child12");
        std::shared_ptr<test_node> child13 = std::make_shared<test_node>("child13");
        root1->insert_child(child11);
        root1->insert_child(child12);
        root1->insert_child(child13);

        std::shared_ptr<test_node> root2 = std::make_shared<test_node>("root2");
        std::shared_ptr<test_node> child21 = std::make_shared<test_node>("child21");
        std::shared_ptr<test_node> child22 = std::make_shared<test_node>("child22");
        std::shared_ptr<test_node> child23 = std::make_shared<test_node>("child23");
        root2->insert_child(child21);
        root2->insert_child(child22);
        root2->insert_child(child23);

        root1->insert_all_children(root2);

        check_node(root1, "root1", nullptr, nullptr, nullptr, child11, child23);
        check_node(root2, "root2", nullptr, nullptr, nullptr, nullptr, nullptr);
        check_node(child11, "child11", root1, nullptr, child12, nullptr, nullptr);
        check_node(child12, "child12", root1, child11, child13, nullptr, nullptr);
        check_node(child13, "child13", root1, child12, child21, nullptr, nullptr);
        check_node(child21, "child21", root1, child13, child22, nullptr, nullptr);
        check_node(child22, "child22", root1, child21, child23, nullptr, nullptr);
        check_node(child23, "child23", root1, child22, nullptr, nullptr, nullptr);
    }

    //insert first
    {
        std::shared_ptr<test_node> root1 = std::make_shared<test_node>("root1");
        std::shared_ptr<test_node> child11 = std::make_shared<test_node>("child11");
        std::shared_ptr<test_node> child12 = std::make_shared<test_node>("child12");
        std::shared_ptr<test_node> child13 = std::make_shared<test_node>("child13");
        root1->insert_child(child11);
        root1->insert_child(child12);
        root1->insert_child(child13);

        std::shared_ptr<test_node> root2 = std::make_shared<test_node>("root2");
        std::shared_ptr<test_node> child21 = std::make_shared<test_node>("child21");
        std::shared_ptr<test_node> child22 = std::make_shared<test_node>("child22");
        std::shared_ptr<test_node> child23 = std::make_shared<test_node>("child23");
        root2->insert_child(child21);
        root2->insert_child(child22);
        root2->insert_child(child23);

        root1->insert_all_children(root2, child11);

        check_node(root1, "root1", nullptr, nullptr, nullptr, child21, child13);
        check_node(root2, "root2", nullptr, nullptr, nullptr, nullptr, nullptr);
        check_node(child11, "child11", root1, child23, child12, nullptr, nullptr);
        check_node(child12, "child12", root1, child11, child13, nullptr, nullptr);
        check_node(child13, "child13", root1, child12, nullptr, nullptr, nullptr);
        check_node(child21, "child21", root1, nullptr, child22, nullptr, nullptr);
        check_node(child22, "child22", root1, child21, child23, nullptr, nullptr);
        check_node(child23, "child23", root1, child22, child11, nullptr, nullptr);
    }

    //insert middle
    {
        std::shared_ptr<test_node> root1 = std::make_shared<test_node>("root1");
        std::shared_ptr<test_node> child11 = std::make_shared<test_node>("child11");
        std::shared_ptr<test_node> child12 = std::make_shared<test_node>("child12");
        std::shared_ptr<test_node> child13 = std::make_shared<test_node>("child13");
        root1->insert_child(child11);
        root1->insert_child(child12);
        root1->insert_child(child13);

        std::shared_ptr<test_node> root2 = std::make_shared<test_node>("root2");
        std::shared_ptr<test_node> child21 = std::make_shared<test_node>("child21");
        std::shared_ptr<test_node> child22 = std::make_shared<test_node>("child22");
        std::shared_ptr<test_node> child23 = std::make_shared<test_node>("child23");
        root2->insert_child(child21);
        root2->insert_child(child22);
        root2->insert_child(child23);

        root1->insert_all_children(root2, child13);

        check_node(root1, "root1", nullptr, nullptr, nullptr, child11, child13);
        check_node(root2, "root2", nullptr, nullptr, nullptr, nullptr, nullptr);
        check_node(child11, "child11", root1, nullptr, child12, nullptr, nullptr);
        check_node(child12, "child12", root1, child11, child21, nullptr, nullptr);
        check_node(child13, "child13", root1, child23, nullptr, nullptr, nullptr);
        check_node(child21, "child21", root1, child12, child22, nullptr, nullptr);
        check_node(child22, "child22", root1, child21, child23, nullptr, nullptr);
        check_node(child23, "child23", root1, child22, child13, nullptr, nullptr);
    }
}


static void test_remove_child() {
    //remove first
    {
        std::shared_ptr<test_node> root = std::make_shared<test_node>("root");
        std::shared_ptr<test_node> child1 = std::make_shared<test_node>("child1");
        std::shared_ptr<test_node> child2 = std::make_shared<test_node>("child2");
        std::shared_ptr<test_node> child3 = std::make_shared<test_node>("child3");
        root->insert_child(child1);
        root->insert_child(child2);
        root->insert_child(child3);

        child1->remove_from_parent();
        check_node(root, "root", nullptr, nullptr, nullptr, child2, child3);
        check_node(child1, "child1", nullptr, nullptr, nullptr, nullptr, nullptr);
        check_node(child2, "child2", root, nullptr, child3, nullptr, nullptr);
        check_node(child3, "child3", root, child2, nullptr, nullptr, nullptr);
    }

    //remove middle
    {
        std::shared_ptr<test_node> root = std::make_shared<test_node>("root");
        std::shared_ptr<test_node> child1 = std::make_shared<test_node>("child1");
        std::shared_ptr<test_node> child2 = std::make_shared<test_node>("child2");
        std::shared_ptr<test_node> child3 = std::make_shared<test_node>("child3");
        root->insert_child(child1);
        root->insert_child(child2);
        root->insert_child(child3);

        child2->remove_from_parent();
        check_node(root, "root", nullptr, nullptr, nullptr, child1, child3);
        check_node(child1, "child1", root, nullptr, child3, nullptr, nullptr);
        check_node(child2, "child2", nullptr, nullptr, nullptr, nullptr, nullptr);
        check_node(child3, "child3", root, child1, nullptr, nullptr, nullptr);
    }

    //remove last
    {
        std::shared_ptr<test_node> root = std::make_shared<test_node>("root");
        std::shared_ptr<test_node> child1 = std::make_shared<test_node>("child1");
        std::shared_ptr<test_node> child2 = std::make_shared<test_node>("child2");
        std::shared_ptr<test_node> child3 = std::make_shared<test_node>("child3");
        root->insert_child(child1);
        root->insert_child(child2);
        root->insert_child(child3);

        child3->remove_from_parent();
        check_node(root, "root", nullptr, nullptr, nullptr, child1, child2);
        check_node(child1, "child1", root, nullptr, child2, nullptr, nullptr);
        check_node(child2, "child2", root, child1, nullptr, nullptr, nullptr);
        check_node(child3, "child3", nullptr, nullptr, nullptr, nullptr, nullptr);
    }
}


static void test_remove_all_children() {
    std::shared_ptr<test_node> root = std::make_shared<test_node>("root");
    std::shared_ptr<test_node> child1 = std::make_shared<test_node>("child1");
    std::shared_ptr<test_node> child2 = std::make_shared<test_node>("child2");
    std::shared_ptr<test_node> child3 = std::make_shared<test_node>("child3");
    root->insert_child(child1);
    root->insert_child(child2);
    root->insert_child(child3);

    root->remove_all_children();

    check_node(root, "root", nullptr, nullptr, nullptr, nullptr, nullptr);
    check_node(child1, "child1", nullptr, nullptr, nullptr, nullptr, nullptr);
    check_node(child2, "child2", nullptr, nullptr, nullptr, nullptr, nullptr);
    check_node(child3, "child3", nullptr, nullptr, nullptr, nullptr, nullptr);
}


int main() {
    test_create_node();
    test_insert_child();
    test_insert_all_children();
    test_remove_child();
    test_remove_all_children();
    return 0;
}

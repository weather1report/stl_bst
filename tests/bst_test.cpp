#include <bst_in.cpp>
#include <bst_pre.cpp>
#include <bst_post.cpp>
#include <gtest/gtest.h>
#include <vector>

TEST(bstTestSuite, IntTest1) {
    bst_in<int> a;
    a.insert(3);
    a.insert(1);
    a.insert(2);

    ASSERT_TRUE(a.contains(3));
    ASSERT_EQ(*a.begin(), 1);
}

TEST(bstTestSuite, IntTest2) {
    bst_pre<int> a;
    a.insert(3);
    a.insert(1);
    a.insert(2);

    ASSERT_TRUE(a.contains(3));
    ASSERT_EQ(*a.begin(), 3);
}

TEST(bstTestSuite, IntTest3) {
    bst_in<int> a;
    a.insert(3);
    a.insert(1);
    a.insert(2);
    a.insert(5);

    a.insert(3);
    a.erase(3);
    std::vector<int> b = {1, 2, 5};
    std::vector<int> c;
    for (auto it = a.begin(); it != a.end(); ++it) {
        c.push_back(*it);
    }
    ASSERT_EQ(b, c);
}


TEST(bstTestSuite, IntTest4) {
    bst_in<int> a;
    a.insert(3);
    a.insert(1);
    a.insert(2);
    a.insert(5);

    ASSERT_EQ(a.find(3), std::find(a.begin(), a.end(), 3));
}

TEST(bstTestSuite, IntTest5) {
    bst_in<int> a;
    a.insert(3);
    a.insert(1);
    a.insert(2);
    a.insert(5);
    a.insert(3);

    std::vector<int> b = {5, 3, 2, 1};
    std::vector<int> c;
    for (auto it = a.rbegin(); it != a.rend(); ++it) {
        c.push_back(*it);
    }

    ASSERT_EQ(c, b);
}

TEST(bstTestSuite, IntTest6) {
    bst_in<int> a;
    a.insert(1);
    a.insert(1);
    a.insert(1);
    a.insert(1);

    ASSERT_TRUE(a.size() == 1);
    a.erase(a.begin());
    ASSERT_TRUE(a.empty());

    a.insert(1);
    a.clear();
    ASSERT_TRUE(a.empty());
}

TEST(bstTestSuite, IntTest7) {
    bst_in<int> a;
    a.insert(2);
    a.insert(5);
    a.insert(7);
    a.insert(8);

    ASSERT_TRUE(a.lower_bound(2) == a.find(2));
    ASSERT_TRUE(a.upper_bound(2) == a.find(5));
    ASSERT_TRUE(a.upper_bound(8) == a.end());
}

TEST(bstTestSuite, StringTest) {
    bst_post<std::string> a;
    a.insert("12");
    a.insert("1");
    a.insert("123");
    a.insert("1234");

    std::vector<std::string> b = {"1", "1234", "123", "12"}, c;

    for (auto it = a.begin(); it != a.end(); ++it) {
        c.push_back(*it);
    }

    ASSERT_EQ(c, b);
}
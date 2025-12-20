#include <gtest/gtest.h>
#include <vector>
#include "Array.h"
#include <string>


TEST(ArrayBasicTest, BasicInsert) {
    Array<int> a;
    for (int i = 0; i < 10; ++i) {
        int idx = a.insert(i + 1);
        EXPECT_EQ(idx, i);
        EXPECT_EQ(a.size(), i + 1);
    }

    for (int i = 0; i < a.size(); ++i) {
        EXPECT_EQ(a[i], i + 1);
    }
}

TEST(ArrayBasicTest, ConstructorWithCapacity) {
    Array<int> a(32);

    EXPECT_EQ(a.size(), 0);

    a.insert(1);
    a.insert(2);
    a.insert(3);

    ASSERT_EQ(a.size(), 3);
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[1], 2);
    EXPECT_EQ(a[2], 3);
}


TEST(ArrayBasicTest, InsertAtPosition) {
    Array<int> a;
    a.insert(1);
    a.insert(2);
    a.insert(3);

    int idx = a.insert(2, 10);
    EXPECT_EQ(idx, 2);
    ASSERT_EQ(a.size(), 4);
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[1], 2);
    EXPECT_EQ(a[2], 10);
    EXPECT_EQ(a[3], 3);

    idx = a.insert(0, 7);
    EXPECT_EQ(idx, 0);
    ASSERT_EQ(a.size(), 5);
    EXPECT_EQ(a[0], 7);
    EXPECT_EQ(a[1], 1);
    EXPECT_EQ(a[2], 2);
    EXPECT_EQ(a[3], 10);
    EXPECT_EQ(a[4], 3);
}


TEST(ArrayBasicTest, RemoveElements) {
    Array<int> a;
    for (int i = 0; i < 5; ++i) {
        a.insert(i + 1);
    }

    a.remove(2);
    ASSERT_EQ(a.size(), 4);
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[1], 2);
    EXPECT_EQ(a[2], 4);
    EXPECT_EQ(a[3], 5);

    a.remove(0);
    ASSERT_EQ(a.size(), 3);
    EXPECT_EQ(a[0], 2);
    EXPECT_EQ(a[1], 4);
    EXPECT_EQ(a[2], 5);

    a.remove(a.size() - 1);
    ASSERT_EQ(a.size(), 2);
    EXPECT_EQ(a[0], 2);
    EXPECT_EQ(a[1], 4);
}

TEST(ArrayIteratorTest, Iterator) {
    Array<int> a;
    for (int i = 0; i < 5; ++i) {
        a.insert(i + 1);
    }

    std::vector<int> collected;
    for (auto it = a.iterator(); it.hasNext(); it.next()) {
        collected.push_back(it.get());
    }

    ASSERT_EQ(collected.size(), 5);
    EXPECT_EQ(collected[0], 1);
    EXPECT_EQ(collected[1], 2);
    EXPECT_EQ(collected[2], 3);
    EXPECT_EQ(collected[3], 4);
    EXPECT_EQ(collected[4], 5);
}


TEST(ArrayIteratorTest, ReverseIterator) {
    Array<int> a;
    for (int i = 0; i < 5; ++i) {
        a.insert(i + 1);
    }

    std::vector<int> collected;
    for (auto it = a.reverseIterator(); it.hasNext(); it.next()) {
        collected.push_back(it.get());
    }

    ASSERT_EQ(collected.size(), 5);
    EXPECT_EQ(collected[0], 5);
    EXPECT_EQ(collected[1], 4);
    EXPECT_EQ(collected[2], 3);
    EXPECT_EQ(collected[3], 2);
    EXPECT_EQ(collected[4], 1);
}

TEST(ArrayIteratorTest, ConstIterator) {
    Array<int> a;
    for (int i = 0; i < 5; ++i) {
        a.insert(i + 1);
    }

    const Array<int>& ca = a;


    std::vector<int> collected;
    for (auto it = ca.iterator(); it.hasNext(); it.next()) {
        collected.push_back(it.get());
    }

    ASSERT_EQ(collected.size(), 5);
    EXPECT_EQ(collected[0], 1);
    EXPECT_EQ(collected[1], 2);
    EXPECT_EQ(collected[2], 3);
    EXPECT_EQ(collected[3], 4);
    EXPECT_EQ(collected[4], 5);
}


TEST(ArrayIteratorTest, ConstReverseIterator) {
    Array<int> a;
    for (int i = 0; i < 5; ++i) {
        a.insert(i + 1);
    }

    const Array<int>& ca = a;

    std::vector<int> collected;
    for (auto it = ca.reverseIterator(); it.hasNext(); it.next()) {
        collected.push_back(it.get());
    }

    ASSERT_EQ(collected.size(), 5);
    EXPECT_EQ(collected[0], 5);
    EXPECT_EQ(collected[1], 4);
    EXPECT_EQ(collected[2], 3);
    EXPECT_EQ(collected[3], 2);
    EXPECT_EQ(collected[4], 1);
}

TEST(ArrayIteratorTest, IteratorsOnEmptyArray) {
    Array<int> a;

    auto it = a.iterator();
    EXPECT_FALSE(it.hasNext());

    auto rit = a.reverseIterator();
    EXPECT_FALSE(rit.hasNext());

    const Array<int>& ca = a;

    auto cit = ca.iterator();
    EXPECT_FALSE(cit.hasNext());

    auto crit = ca.reverseIterator();
    EXPECT_FALSE(crit.hasNext());
}


TEST(ArrayCopyMoveTest, CopyConstructor) {
    Array<int> a;
    a.insert(1);
    a.insert(2);
    a.insert(3);

    Array<int> b(a);
    ASSERT_EQ(b.size(), a.size());
    EXPECT_EQ(b[0], 1);
    EXPECT_EQ(b[1], 2);
    EXPECT_EQ(b[2], 3);
}

TEST(ArrayCopyMoveTest, CopyAssignment) {
    Array<int> a;
    a.insert(10);
    a.insert(20);

    Array<int> b;
    b.insert(1);
    b.insert(2);
    b.insert(3);

    b = a;
    ASSERT_EQ(b.size(), a.size());
    EXPECT_EQ(b[0], 10);
    EXPECT_EQ(b[1], 20);
}


TEST(ArrayCopyMoveTest, MoveConstructorTransfers) {
    Array<int> a;
    for (int i = 0; i < 5; ++i) {
        a.insert(i + 1);
    }

    Array<int> b(std::move(a));

    EXPECT_EQ(a.size(), 0);

    ASSERT_EQ(b.size(), 5);
    EXPECT_EQ(b[0], 1);
    EXPECT_EQ(b[1], 2);
    EXPECT_EQ(b[2], 3);
    EXPECT_EQ(b[3], 4);
    EXPECT_EQ(b[4], 5);
}

TEST(ArrayCopyMoveTest, MoveAssignmentTransfers) {
    Array<int> a;
    a.insert(10);
    a.insert(20);
    a.insert(30);

    Array<int> b;
    b.insert(1);
    b.insert(2);

    b = std::move(a);

    EXPECT_EQ(a.size(), 0);

    ASSERT_EQ(b.size(), 3);
    EXPECT_EQ(b[0], 10);
    EXPECT_EQ(b[1], 20);
    EXPECT_EQ(b[2], 30);
}

class ComplexType {
public:
    std::string name;
    std::vector<int> values;

    ComplexType() = default;

    ComplexType(std::string n, std::vector<int> v)
        : name(std::move(n))
        , values(std::move(v)) {
    }
};

TEST(ArrayComplexTypeTest, InsertAndRemoveComplex) {
    Array<ComplexType> a;

    a.insert(ComplexType{ "1",   {1, 2} });
    a.insert(ComplexType{ "2",   {3} });
    a.insert(ComplexType{ "3", {4, 5, 6} });

    ASSERT_EQ(a.size(), 3);

    EXPECT_EQ(a[0].name, "1");
    ASSERT_EQ(a[0].values.size(), 2);

    std::cout << a[0].values[0];
    EXPECT_EQ(a[0].values[0], 1);
    EXPECT_EQ(a[0].values[1], 2);

    EXPECT_EQ(a[1].name, "2");
    ASSERT_EQ(a[1].values.size(), 1);
    EXPECT_EQ(a[1].values[0], 3);

    EXPECT_EQ(a[2].name, "3");
    ASSERT_EQ(a[2].values.size(), 3);
    EXPECT_EQ(a[2].values[0], 4);
    EXPECT_EQ(a[2].values[1], 5);
    EXPECT_EQ(a[2].values[2], 6);

    a.remove(1);

    ASSERT_EQ(a.size(), 2);
    EXPECT_EQ(a[0].name, "1");
    ASSERT_EQ(a[0].values.size(), 2);
    EXPECT_EQ(a[0].values[0], 1);
    EXPECT_EQ(a[0].values[1], 2);


    EXPECT_EQ(a[1].name, "3");
    ASSERT_EQ(a[1].values.size(), 3);
    EXPECT_EQ(a[1].values[0], 4);
    EXPECT_EQ(a[1].values[1], 5);
    EXPECT_EQ(a[1].values[2], 6);
}

TEST(ArrayComplexTypeTest, CopyAndMoveComplex) {
    Array<ComplexType> a;
    a.insert(ComplexType{ "1", {1, 2} });
    a.insert(ComplexType{ "2",  {3, 4, 5} });

    Array<ComplexType> b(a);

    ASSERT_EQ(b.size(), a.size());
    EXPECT_EQ(b[0].name, "1");
    EXPECT_EQ(b[1].name, "2");
    ASSERT_EQ(b[1].values.size(), 3);
    EXPECT_EQ(b[1].values[0], 3);
    EXPECT_EQ(b[1].values[1], 4);
    EXPECT_EQ(b[1].values[2], 5);

    a[0].name = "1_updated";
    a[0].values.push_back(42);

    EXPECT_EQ(b[0].name, "1");
    ASSERT_EQ(b[0].values.size(), 2);
    EXPECT_EQ(b[0].values[0], 1);
    EXPECT_EQ(b[0].values[1], 2);

    Array<ComplexType> c(std::move(a));
    EXPECT_EQ(a.size(), 0);

    ASSERT_EQ(c.size(), 2);
    EXPECT_EQ(c[0].name, "1_updated");
    ASSERT_EQ(c[0].values.size(), 3u);
    EXPECT_EQ(c[0].values[0], 1);
    EXPECT_EQ(c[0].values[1], 2);
    EXPECT_EQ(c[0].values[2], 42);

    Array<ComplexType> d;
    d.insert(ComplexType{ "dummy", {0} });

    d = std::move(c);

    EXPECT_EQ(c.size(), 0);

    ASSERT_EQ(d.size(), 2);
    EXPECT_EQ(d[0].name, "1_updated");
    EXPECT_EQ(d[1].name, "2");
    ASSERT_EQ(d[1].values.size(), 3);
    EXPECT_EQ(d[1].values[0], 3);
    EXPECT_EQ(d[1].values[1], 4);
    EXPECT_EQ(d[1].values[2], 5);
}
TEST(ArrayComplexTypeTest, IteratorSetComplex) {
    Array<ComplexType> a;

    a.insert(ComplexType{"1", {1, 2}});
    a.insert(ComplexType{"2", {3}});
    a.insert(ComplexType{"3", {4, 5}});


    int idx = 0;
    for (auto it = a.iterator(); it.hasNext(); it.next(), ++idx) {
        ComplexType updated = it.get();
        updated.name += "_updated";
        updated.values.push_back(100 + idx);
        it.set(updated);
    }

    ASSERT_EQ(a.size(), 3);

    EXPECT_EQ(a[0].name, "1_updated");
    EXPECT_EQ(a[1].name, "2_updated");
    EXPECT_EQ(a[2].name, "3_updated");

    ASSERT_EQ(a[0].values.size(), 3);
    EXPECT_EQ(a[0].values[0], 1);
    EXPECT_EQ(a[0].values[1], 2);
    EXPECT_EQ(a[0].values[2], 100);

    ASSERT_EQ(a[1].values.size(), 2);
    EXPECT_EQ(a[1].values[0], 3);
    EXPECT_EQ(a[1].values[1], 101);

    ASSERT_EQ(a[2].values.size(), 3);
    EXPECT_EQ(a[2].values[0], 4);
    EXPECT_EQ(a[2].values[1], 5);
    EXPECT_EQ(a[2].values[2], 102);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
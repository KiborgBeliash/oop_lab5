#include <gtest/gtest.h>
#include "memory_pool.h"
#include "queue.h"

struct complex_type {
    int id;
    double value;
    
    complex_type(int i = 0, double v = 0.0) : id(i), value(v) {}
    
    bool operator==(const complex_type& other) const {
        return id == other.id && value == other.value;
    }
};

TEST(FixedBlockMemoryResourceTest, BasicAllocation) {
    fixed_block_memory_resource mr(64);
    
    void* ptr1 = mr.allocate(50);
    void* ptr2 = mr.allocate(30);
    
    EXPECT_NE(ptr1, nullptr);
    EXPECT_NE(ptr2, nullptr);
    EXPECT_EQ(mr.get_allocated_count(), 2);
    
    mr.deallocate(ptr1, 50);
    mr.deallocate(ptr2, 30);
}

TEST(FixedBlockMemoryResourceTest, ReuseMemory) {
    fixed_block_memory_resource mr(64);
    
    void* ptr1 = mr.allocate(50);
    mr.deallocate(ptr1, 50);
    
    void* ptr2 = mr.allocate(30);
    
    EXPECT_EQ(mr.get_allocated_count(), 1);
    mr.deallocate(ptr2, 30);
}

TEST(DoublyLinkedListTest, BasicOperations) {
    fixed_block_memory_resource mr(256);
    doubly_linked_list<int> list(&mr);
    
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.size(), 0);
    
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    
    EXPECT_FALSE(list.empty());
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.front(), 1);
    EXPECT_EQ(list.back(), 3);
}

TEST(DoublyLinkedListTest, Iteration) {
    fixed_block_memory_resource mr(256);
    doubly_linked_list<int> list(&mr);
    
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    
    int sum = 0;
    for (auto it = list.begin(); it != list.end(); ++it) {
        sum += *it;
    }
    
    EXPECT_EQ(sum, 6);
}

TEST(DoublyLinkedListTest, ComplexType) {
    fixed_block_memory_resource mr(256);
    doubly_linked_list<complex_type> list(&mr);
    
    list.emplace_back(1, 1.1);
    list.emplace_back(2, 2.2);
    list.emplace_back(3, 3.3);
    
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.front(), complex_type(1, 1.1));
    EXPECT_EQ(list.back(), complex_type(3, 3.3));
}

TEST(DoublyLinkedListTest, Modification) {
    fixed_block_memory_resource mr(256);
    doubly_linked_list<int> list(&mr);
    
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    
    list.pop_front();
    EXPECT_EQ(list.front(), 2);
    
    list.pop_back();
    EXPECT_EQ(list.back(), 2);
    
    list.push_front(0);
    EXPECT_EQ(list.front(), 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
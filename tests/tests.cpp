#include <gtest/gtest.h>
#include <string>
#include "memory_pool.h"
#include "queue.h"

// Тесты для FixedBlockMemoryResource
TEST(FixedBlockMemoryResourceTest, BasicAllocation) {
    FixedBlockMemoryResource mr(256);
    
    void* ptr1 = mr.allocate(100);
    void* ptr2 = mr.allocate(200);
    
    EXPECT_NE(ptr1, nullptr);
    EXPECT_NE(ptr2, nullptr);
    EXPECT_EQ(mr.get_allocated_count(), 2);
    
    mr.deallocate(ptr1, 100);
    mr.deallocate(ptr2, 200);
    
    EXPECT_EQ(mr.get_allocated_count(), 0);
}

TEST(FixedBlockMemoryResourceTest, FixedBlockSize) {
    FixedBlockMemoryResource mr(128);
    
    // Даже если запрашиваем меньше, выделяется полный блок
    void* ptr = mr.allocate(50);
    EXPECT_NE(ptr, nullptr);
    EXPECT_EQ(mr.get_allocated_count(), 1);
    
    mr.deallocate(ptr, 50);
}

// Тесты для DoublyLinkedList с int
TEST(DoublyLinkedListTest, BasicOperations) {
    FixedBlockMemoryResource mr(256);
    DoublyLinkedList<int> list(&mr);
    
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
    FixedBlockMemoryResource mr(256);
    DoublyLinkedList<int> list(&mr);
    
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    
    int sum = 0;
    for (const auto& item : list) {
        sum += item;
    }
    
    EXPECT_EQ(sum, 6);
}

TEST(DoublyLinkedListTest, PushPopOperations) {
    FixedBlockMemoryResource mr(256);
    DoublyLinkedList<int> list(&mr);
    
    list.push_front(2);
    list.push_front(1);
    list.push_back(3);
    list.push_back(4);
    
    EXPECT_EQ(list.front(), 1);
    EXPECT_EQ(list.back(), 4);
    
    list.pop_front();
    EXPECT_EQ(list.front(), 2);
    
    list.pop_back();
    EXPECT_EQ(list.back(), 3);
}

// Тесты для ComplexType
TEST(DoublyLinkedListTest, ComplexType) {
    FixedBlockMemoryResource mr(256);
    DoublyLinkedList<std::string> list(&mr);
    
    list.push_back("hello");
    list.push_back("world");
    list.push_back("test");
    
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.front(), "hello");
    EXPECT_EQ(list.back(), "test");
    
    std::string result;
    for (const auto& item : list) {
        result += item + " ";
    }
    
    EXPECT_TRUE(result.find("hello") != std::string::npos);
    EXPECT_TRUE(result.find("world") != std::string::npos);
}

TEST(DoublyLinkedListTest, EmplaceOperations) {
    FixedBlockMemoryResource mr(256);
    DoublyLinkedList<std::pair<int, std::string>> list(&mr);
    
    list.emplace_back(1, "one");
    list.emplace_front(0, "zero");
    list.emplace_back(2, "two");
    
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.front().first, 0);
    EXPECT_EQ(list.front().second, "zero");
    EXPECT_EQ(list.back().first, 2);
    EXPECT_EQ(list.back().second, "two");
}

// Структура для тестирования сложных типов
struct TestStruct {
    int id;
    double value;
    std::string name;
    
    TestStruct(int i = 0, double v = 0.0, const std::string& n = "")
        : id(i), value(v), name(n) {}
    
    bool operator==(const TestStruct& other) const {
        return id == other.id && value == other.value && name == other.name;
    }
};

TEST(DoublyLinkedListTest, CustomStruct) {
    FixedBlockMemoryResource mr(256);
    DoublyLinkedList<TestStruct> list(&mr);
    
    list.emplace_back(1, 3.14, "first");
    list.emplace_back(2, 2.71, "second");
    
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.front().id, 1);
    EXPECT_EQ(list.back().name, "second");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
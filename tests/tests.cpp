#include <gtest/gtest.h>
#include <string>
#include "memory_pool.h"
#include "list_2.h"

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
    EXPECT_EQ(mr.get_allocated_count(), 0);
}

TEST(FixedBlockMemoryResourceTest, ReuseMemory) {
    FixedBlockMemoryResource mr(256);
    
    void* ptr1 = mr.allocate(100);
    EXPECT_EQ(mr.get_allocated_count(), 1);
    
    mr.deallocate(ptr1, 100);
    EXPECT_EQ(mr.get_allocated_count(), 0);
    
    // Память должна переиспользоваться
    void* ptr2 = mr.allocate(150);
    EXPECT_NE(ptr2, nullptr);
    EXPECT_EQ(mr.get_allocated_count(), 1);
    
    mr.deallocate(ptr2, 150);
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
    int count = 0;
    for (const auto& item : list) {
        sum += item;
        count++;
    }
    
    EXPECT_EQ(sum, 6);
    EXPECT_EQ(count, 3);
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
    EXPECT_EQ(list.size(), 4);
    
    list.pop_front();
    EXPECT_EQ(list.front(), 2);
    EXPECT_EQ(list.size(), 3);
    
    list.pop_back();
    EXPECT_EQ(list.back(), 3);
    EXPECT_EQ(list.size(), 2);
}

TEST(DoublyLinkedListTest, Clear) {
    FixedBlockMemoryResource mr(256);
    DoublyLinkedList<int> list(&mr);
    
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    
    EXPECT_EQ(list.size(), 3);
    list.clear();
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.size(), 0);
}

// Тесты для string
TEST(DoublyLinkedListTest, StringType) {
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

TEST(DoublyLinkedListTest, MoveSemantics) {
    FixedBlockMemoryResource mr(256);
    DoublyLinkedList<std::string> list(&mr);
    
    std::string str = "test";
    list.push_back(std::move(str));
    
    EXPECT_TRUE(str.empty()); // строка должна быть перемещена
    EXPECT_EQ(list.front(), "test");
}

// Структура для тестирования сложных типов
struct ComplexType {
    int id;
    double value;
    std::string name;
    
    ComplexType(int i = 0, double v = 0.0, const std::string& n = "")
        : id(i), value(v), name(n) {}
    
    bool operator==(const ComplexType& other) const {
        return id == other.id && value == other.value && name == other.name;
    }
};

TEST(DoublyLinkedListTest, ComplexType) {
    FixedBlockMemoryResource mr(256);
    DoublyLinkedList<ComplexType> list(&mr);
    
    ComplexType ct1(1, 3.14, "first");
    ComplexType ct2(2, 2.71, "second");
    
    list.push_back(ct1);
    list.push_back(ct2);
    
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.front().id, 1);
    EXPECT_EQ(list.front().value, 3.14);
    EXPECT_EQ(list.front().name, "first");
    EXPECT_EQ(list.back().id, 2);
    EXPECT_EQ(list.back().name, "second");
}

TEST(DoublyLinkedListTest, IteratorOperations) {
    FixedBlockMemoryResource mr(256);
    DoublyLinkedList<int> list(&mr);
    
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);
    
    auto it = list.begin();
    EXPECT_EQ(*it, 10);
    ++it;
    EXPECT_EQ(*it, 20);
    it++;
    EXPECT_EQ(*it, 30);
    ++it;
    EXPECT_EQ(it, list.end());
}

// Тест на использование разных memory_resource
TEST(DoublyLinkedListTest, DifferentMemoryResources) {
    FixedBlockMemoryResource mr1(128);
    FixedBlockMemoryResource mr2(256);
    
    DoublyLinkedList<int> list1(&mr1);
    DoublyLinkedList<int> list2(&mr2);
    
    list1.push_back(1);
    list2.push_back(2);
    
    EXPECT_EQ(list1.front(), 1);
    EXPECT_EQ(list2.front(), 2);
    EXPECT_NE(&mr1, &mr2);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
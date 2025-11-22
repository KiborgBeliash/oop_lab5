#include <iostream>
#include <string>
#include "memory_pool.h"
#include "list_2.h"

struct ComplexType {
    int id;
    double value;
    std::string name;
    
    ComplexType(int i = 0, double v = 0.0, const std::string& n = "")
        : id(i), value(v), name(n) {}
    
    friend std::ostream& operator<<(std::ostream& os, const ComplexType& ct) {
        os << "ComplexType{id=" << ct.id << ", value=" << ct.value << ", name=\"" << ct.name << "\"}";
        return os;
    }
};

void demonstrate_memory_reuse() {
    std::cout << "=== Memory Reuse Demonstration ===\n";
    
    FixedBlockMemoryResource fixed_mr(256);
    
    std::cout << "Initial state - Allocated: " << fixed_mr.get_allocated_count() 
              << ", Free: " << fixed_mr.get_free_blocks_count() << "\n";
    
    {
        DoublyLinkedList<int> list(&fixed_mr);
        
        for (int i = 1; i <= 3; ++i) {
            list.push_back(i * 10);
        }
        
        std::cout << "After adding 3 elements - Allocated: " << fixed_mr.get_allocated_count() 
                  << ", Free: " << fixed_mr.get_free_blocks_count() << "\n";
        
        list.pop_back();
        list.pop_back();
        
        std::cout << "After removing 2 elements - Allocated: " << fixed_mr.get_allocated_count() 
                  << ", Free: " << fixed_mr.get_free_blocks_count() << "\n";
        
        list.push_back(40);
        list.push_back(50);
        
        std::cout << "After adding 2 new elements - Allocated: " << fixed_mr.get_allocated_count() 
                  << ", Free: " << fixed_mr.get_free_blocks_count() << "\n";
    }
    
    std::cout << "After list destruction - Allocated: " << fixed_mr.get_allocated_count() 
              << ", Free: " << fixed_mr.get_free_blocks_count() << "\n\n";
}

int main() {
    demonstrate_memory_reuse();
    
    FixedBlockMemoryResource fixed_mr(256);
    
    std::cout << "=== Demonstration with int ===\n";
    
    DoublyLinkedList<int> int_list(&fixed_mr);
    
    for (int i = 1; i <= 5; ++i) {
        int_list.push_back(i * 10);
    }
    
    std::cout << "List: ";
    for (auto it = int_list.begin(); it != int_list.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";
    
    std::cout << "\n=== Demonstration with ComplexType ===\n";
    
    DoublyLinkedList<ComplexType> complex_list(&fixed_mr);
    
    complex_list.emplace_back(1, 3.14, "first");
    complex_list.emplace_back(2, 2.71, "second"); 
    complex_list.emplace_back(3, 1.41, "third");
    
    std::cout << "ComplexType list:\n";
    for (const auto& item : complex_list) {
        std::cout << "  " << item << "\n";
    }
    
    std::cout << "\n=== Demonstration of modifications ===\n";
    
    complex_list.pop_front();
    complex_list.emplace_front(0, 0.0, "new_first");
    
    std::cout << "After modifications:\n";
    for (const auto& item : complex_list) {
        std::cout << "  " << item << "\n";
    }
    
    std::cout << "\n=== Memory resource information ===\n";
    std::cout << "Block size: " << fixed_mr.get_block_size() << " bytes\n";
    std::cout << "Allocated blocks: " << fixed_mr.get_allocated_count() << "\n";
    std::cout << "Free blocks available for reuse: " << fixed_mr.get_free_blocks_count() << "\n";
    
    return 0;
}
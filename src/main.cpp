#include <iostream>
#include <string>
#include "memory_pool.h"
#include "queue.h"

struct complex_type {
    int id;
    double value;
    std::string name;
    
    complex_type(int i = 0, double v = 0.0, const std::string& n = "")
        : id(i), value(v), name(n) {}
    
    friend std::ostream& operator<<(std::ostream& os, const complex_type& ct) {
        os << "complex_type{id=" << ct.id << ", value=" << ct.value << ", name=\"" << ct.name << "\"}";
        return os;
    }
};

int main() {
    fixed_block_memory_resource fixed_mr(256);
    
    std::cout << "=== Демонстрация с int ===\n";
    
    doubly_linked_list<int> int_list(&fixed_mr);
    
    for (int i = 1; i <= 5; ++i) {
        int_list.push_back(i * 10);
    }
    
    std::cout << "Список: ";
    for (auto it = int_list.begin(); it != int_list.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";
    
    std::cout << "\n=== Демонстрация с complex_type ===\n";
    
    doubly_linked_list<complex_type> complex_list(&fixed_mr);
    
    complex_list.emplace_back(1, 3.14, "first");
    complex_list.emplace_back(2, 2.71, "second");
    complex_list.emplace_back(3, 1.41, "third");
    
    std::cout << "Список complex_type:\n";
    for (const auto& item : complex_list) {
        std::cout << "  " << item << "\n";
    }
    
    std::cout << "\n=== Демонстрация модификаций ===\n";
    
    complex_list.pop_front();
    complex_list.emplace_front(0, 0.0, "new_first");
    
    std::cout << "После модификаций:\n";
    for (const auto& item : complex_list) {
        std::cout << "  " << item << "\n";
    }
    
    std::cout << "\n=== Информация о memory_resource ===\n";
    std::cout << "Размер блока: " << fixed_mr.get_block_size() << " байт\n";
    std::cout << "Выделено блоков: " << fixed_mr.get_allocated_count() << "\n";
    
    return 0;
}
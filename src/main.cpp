#include <iostream>
#include <string>
#include "memory_pool.h"
#include "list_2.h"

// Сложная структура для демонстрации
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

int main() {
    // Создаем memory_resource с размером блока 256 байт
    FixedBlockMemoryResource fixed_mr(256);
    
    std::cout << "=== Demonstration with int ===\n";
    
    // Создаем список с использованием нашего memory_resource
    DoublyLinkedList<int> int_list(&fixed_mr);
    
    // Добавляем элементы
    for (int i = 1; i <= 5; ++i) {
        int_list.push_back(i * 10);
    }
    
    // Выводим с помощью итератора
    std::cout << "List: ";
    for (auto it = int_list.begin(); it != int_list.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";
    
    // Демонстрация с сложным типом
    std::cout << "\n=== Demonstration with ComplexType ===\n";
    
    DoublyLinkedList<ComplexType> complex_list(&fixed_mr);
    
    // Создаем объекты явно, затем добавляем
    ComplexType ct1(3, 3.14, "first");
    ComplexType ct2(4, 2.71, "second"); 
    ComplexType ct3(5, 1.41, "third");
    
    complex_list.push_back(ct1);
    complex_list.push_back(ct2);
    complex_list.push_back(ct3);
    
    std::cout << "ComplexType list:\n";
    for (const auto& item : complex_list) {
        std::cout << "  " << item << "\n";
    }
    
    // Демонстрация модификации
    std::cout << "\n=== Demonstration of modifications ===\n";
    
    //1 замена
    complex_list.pop_front();
    ComplexType new_front(2, 0.0, "new_trird");
    complex_list.push_front(new_front);

    std::cout << "After 1 modifications:\n";
    for (const auto& item : complex_list) {
        std::cout << "  " << item << "\n";
    }
    
    //2 замена
    /*
    complex_list.pop_back();
    ComplexType new_front(1, 0.0, "new_second");
    complex_list.push_front(new_front);

    std::cout << "After 2 modifications:\n";
    for (const auto& item : complex_list) {
        std::cout << "  " << item << "\n";
    }*/

    //3 замена
    /*
    complex_list.pop_back();
    ComplexType new_front(0, 0.0, "new_first");
    complex_list.push_front(new_front);

    std::cout << "After 3 modifications:\n";
    for (const auto& item : complex_list) {
        std::cout << "  " << item << "\n";
    }
    */
    
    // Информация о memory_resource
    std::cout << "\n=== Memory resource information ===\n";
    std::cout << "Block size: " << fixed_mr.get_block_size() << " bytes\n";
    std::cout << "Allocated blocks: " << fixed_mr.get_allocated_count() << "\n";
    
    return 0;
}
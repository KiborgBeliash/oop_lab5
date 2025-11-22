#pragma once

#include <memory_resource>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <utility>

template<typename T>
class DoublyLinkedList {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;
        
        // Простой конструктор для создания данных
        template<typename... Args>
        Node(Args&&... args) 
            : data(std::forward<Args>(args)...), prev(nullptr), next(nullptr) {}
            
        // Конструктор с указателями
        Node(Node* p, Node* n) : data(), prev(p), next(n) {}
    };
    
    using allocator_type = std::pmr::polymorphic_allocator<Node>;
    allocator_type alloc_;
    Node* head_;
    Node* tail_;
    std::size_t size_;
    
public:
    // Итератор
    class Iterator {
    private:
        Node* current_;
        
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;
        
        explicit Iterator(Node* current = nullptr) : current_(current) {}
        
        reference operator*() const { return current_->data; }
        pointer operator->() const { return &current_->data; }
        
        Iterator& operator++() {
            if (current_) {
                current_ = current_->next;
            }
            return *this;
        }
        
        Iterator operator++(int) {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }
        
        bool operator==(const Iterator& other) const { return current_ == other.current_; }
        bool operator!=(const Iterator& other) const { return current_ != other.current_; }
    };
    
    // Конструкторы
    explicit DoublyLinkedList(std::pmr::memory_resource* mr = std::pmr::get_default_resource())
        : alloc_(mr), head_(nullptr), tail_(nullptr), size_(0) {}
    
    ~DoublyLinkedList() {
        clear();
    }
    
    // Итераторы
    Iterator begin() { return Iterator(head_); }
    Iterator end() { return Iterator(nullptr); }
    Iterator begin() const { return Iterator(head_); }
    Iterator end() const { return Iterator(nullptr); }
    
    // Модификаторы
    template<typename... Args>
    void emplace_back(Args&&... args) {
        Node* new_node = alloc_.allocate(1);
        try {
            // Сначала создаем узел с данными
            alloc_.construct(new_node, std::forward<Args>(args)...);
        } catch (...) {
            alloc_.deallocate(new_node, 1);
            throw;
        }
        
        // Затем настраиваем связи
        new_node->prev = tail_;
        new_node->next = nullptr;
        
        if (!head_) {
            head_ = tail_ = new_node;
        } else {
            tail_->next = new_node;
            tail_ = new_node;
        }
        ++size_;
    }
    
    void push_back(const T& value) {
        emplace_back(value);
    }
    
    void push_back(T&& value) {
        emplace_back(std::move(value));
    }
    
    template<typename... Args>
    void emplace_front(Args&&... args) {
        Node* new_node = alloc_.allocate(1);
        try {
            // Сначала создаем узел с данными
            alloc_.construct(new_node, std::forward<Args>(args)...);
        } catch (...) {
            alloc_.deallocate(new_node, 1);
            throw;
        }
        
        // Затем настраиваем связи
        new_node->prev = nullptr;
        new_node->next = head_;
        
        if (!head_) {
            head_ = tail_ = new_node;
        } else {
            head_->prev = new_node;
            head_ = new_node;
        }
        ++size_;
    }
    
    void push_front(const T& value) {
        emplace_front(value);
    }
    
    void push_front(T&& value) {
        emplace_front(std::move(value));
    }
    
    void pop_back() {
        if (!tail_) return;
        
        Node* to_delete = tail_;
        tail_ = tail_->prev;
        
        if (tail_) {
            tail_->next = nullptr;
        } else {
            head_ = nullptr;
        }
        
        alloc_.destroy(to_delete);
        alloc_.deallocate(to_delete, 1);
        --size_;
    }
    
    void pop_front() {
        if (!head_) return;
        
        Node* to_delete = head_;
        head_ = head_->next;
        
        if (head_) {
            head_->prev = nullptr;
        } else {
            tail_ = nullptr;
        }
        
        alloc_.destroy(to_delete);
        alloc_.deallocate(to_delete, 1);
        --size_;
    }
    
    void clear() {
        while (head_) {
            pop_front();
        }
    }
    
    // Доступ к элементам
    T& front() { 
        if (!head_) throw std::runtime_error("List is empty");
        return head_->data; 
    }
    
    const T& front() const { 
        if (!head_) throw std::runtime_error("List is empty");
        return head_->data; 
    }
    
    T& back() { 
        if (!tail_) throw std::runtime_error("List is empty");
        return tail_->data; 
    }
    
    const T& back() const { 
        if (!tail_) throw std::runtime_error("List is empty");
        return tail_->data; 
    }
    
    // Размер
    std::size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }
};
#pragma once

#include <memory_resource>
#include <iterator>
#include <cstddef>

template<typename T>
class doubly_linked_list {
private:
    struct node {
        T data;
        node* prev;
        node* next;
        
        template<typename... Args>
        node(Args&&... args, node* p = nullptr, node* n = nullptr)
            : data(std::forward<Args>(args)...), prev(p), next(n) {}
    };
    
    using allocator_type = std::pmr::polymorphic_allocator<node>;
    allocator_type alloc_;
    node* head_;
    node* tail_;
    std::size_t size_;
    
public:
    class iterator {
    private:
        node* current_;
        
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;
        
        explicit iterator(node* current = nullptr) : current_(current) {}
        
        reference operator*() const { return current_->data; }
        pointer operator->() const { return &current_->data; }
        
        iterator& operator++() {
            current_ = current_->next;
            return *this;
        }
        
        iterator operator++(int) {
            iterator temp = *this;
            ++(*this);
            return temp;
        }
        
        bool operator==(const iterator& other) const { return current_ == other.current_; }
        bool operator!=(const iterator& other) const { return current_ != other.current_; }
        
        node* get_node() const { return current_; }
    };
    
    explicit doubly_linked_list(std::pmr::memory_resource* mr = std::pmr::get_default_resource())
        : alloc_(mr), head_(nullptr), tail_(nullptr), size_(0) {}
    
    ~doubly_linked_list() {
        clear();
    }
    
    doubly_linked_list(const doubly_linked_list& other) 
        : alloc_(other.alloc_), head_(nullptr), tail_(nullptr), size_(0) {
        for (const auto& item : other) {
            push_back(item);
        }
    }
    
    doubly_linked_list& operator=(const doubly_linked_list& other) {
        if (this != &other) {
            clear();
            for (const auto& item : other) {
                push_back(item);
            }
        }
        return *this;
    }
    
    iterator begin() { return iterator(head_); }
    iterator end() { return iterator(nullptr); }
    
    template<typename... Args>
    void emplace_back(Args&&... args) {
        node* new_node = alloc_.new_object<node>(std::forward<Args>(args)...);
        
        if (!head_) {
            head_ = tail_ = new_node;
        } else {
            tail_->next = new_node;
            new_node->prev = tail_;
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
        node* new_node = alloc_.new_object<node>(std::forward<Args>(args)...);
        
        if (!head_) {
            head_ = tail_ = new_node;
        } else {
            new_node->next = head_;
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
        
        node* to_delete = tail_;
        tail_ = tail_->prev;
        
        if (tail_) {
            tail_->next = nullptr;
        } else {
            head_ = nullptr;
        }
        
        alloc_.delete_object(to_delete);
        --size_;
    }
    
    void pop_front() {
        if (!head_) return;
        
        node* to_delete = head_;
        head_ = head_->next;
        
        if (head_) {
            head_->prev = nullptr;
        } else {
            tail_ = nullptr;
        }
        
        alloc_.delete_object(to_delete);
        --size_;
    }
    
    void clear() {
        while (head_) {
            pop_front();
        }
    }
    
    T& front() { return head_->data; }
    const T& front() const { return head_->data; }
    T& back() { return tail_->data; }
    const T& back() const { return tail_->data; }
    
    std::size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }
};
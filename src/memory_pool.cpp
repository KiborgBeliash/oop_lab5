#include "memory_pool.h"
#include <algorithm>

FixedBlockMemoryResource::FixedBlockMemoryResource(std::size_t block_size, 
                                                 std::pmr::memory_resource* upstream)
    : block_size_(block_size), upstream_(upstream) {}

FixedBlockMemoryResource::~FixedBlockMemoryResource() {
    // Освобождаем всю память при уничтожении
    for (const auto& block : allocated_blocks_) {
        upstream_->deallocate(block.ptr, block.size);
    }
}

void* FixedBlockMemoryResource::do_allocate(std::size_t bytes, std::size_t alignment) {
    // Всегда выделяем фиксированный размер блока
    void* ptr = upstream_->allocate(block_size_, alignment);
    allocated_blocks_.push_back({ptr, block_size_});
    return ptr;
}

void FixedBlockMemoryResource::do_deallocate(void* ptr, std::size_t bytes, std::size_t alignment) {
    // Находим блок и освобождаем его
    auto it = std::find_if(allocated_blocks_.begin(), allocated_blocks_.end(),
                          [ptr](const BlockInfo& info) { return info.ptr == ptr; });
    
    if (it != allocated_blocks_.end()) {
        upstream_->deallocate(ptr, it->size, alignment);
        allocated_blocks_.erase(it);
    }
}

bool FixedBlockMemoryResource::do_is_equal(const std::pmr::memory_resource& other) const noexcept {
    return this == &other;
}
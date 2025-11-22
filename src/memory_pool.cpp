#include "memory_pool.h"
#include <algorithm>

FixedBlockMemoryResource::FixedBlockMemoryResource(std::size_t block_size, 
                                                 std::pmr::memory_resource* upstream)
    : block_size_(block_size), upstream_(upstream) {}

FixedBlockMemoryResource::~FixedBlockMemoryResource() {
    for (const auto& block : allocated_blocks_) {
        upstream_->deallocate(block.ptr, block.size);
    }
}

void* FixedBlockMemoryResource::do_allocate(std::size_t bytes, std::size_t alignment) {
    if (!free_blocks_.empty()) {
        void* ptr = free_blocks_.back();
        free_blocks_.pop_back();
        return ptr;
    }
    
    void* ptr = upstream_->allocate(block_size_, alignment);
    allocated_blocks_.push_back({ptr, block_size_});
    return ptr;
}

void FixedBlockMemoryResource::do_deallocate(void* ptr, std::size_t bytes, std::size_t alignment) {
    free_blocks_.push_back(ptr);
}

bool FixedBlockMemoryResource::do_is_equal(const std::pmr::memory_resource& other) const noexcept {
    return this == &other;
}
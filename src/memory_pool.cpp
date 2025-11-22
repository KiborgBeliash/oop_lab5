#include "memory_pool.h"
#include <algorithm>

fixed_block_memory_resource::fixed_block_memory_resource(std::size_t block_size, 
                                                       std::pmr::memory_resource* upstream)
    : block_size_(block_size), upstream_(upstream) {}

fixed_block_memory_resource::~fixed_block_memory_resource() {
    for (const auto& block : allocated_blocks_) {
        upstream_->deallocate(block.ptr, block.size);
    }
}

void* fixed_block_memory_resource::do_allocate(std::size_t bytes, std::size_t alignment) {
    void* ptr = upstream_->allocate(block_size_, alignment);
    allocated_blocks_.push_back({ptr, block_size_});
    return ptr;
}

void fixed_block_memory_resource::do_deallocate(void* ptr, std::size_t bytes, std::size_t alignment) {
    auto it = std::find_if(allocated_blocks_.begin(), allocated_blocks_.end(),
                          [ptr](const block_info& info) { return info.ptr == ptr; });
    
    if (it != allocated_blocks_.end()) {
        upstream_->deallocate(ptr, it->size, alignment);
        allocated_blocks_.erase(it);
    }
}

bool fixed_block_memory_resource::do_is_equal(const std::pmr::memory_resource& other) const noexcept {
    return this == &other;
}
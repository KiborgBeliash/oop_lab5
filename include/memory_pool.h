#pragma once

#include <memory_resource>
#include <list>
#include <cstddef>

class FixedBlockMemoryResource : public std::pmr::memory_resource {
private:
    struct BlockInfo {
        void* ptr;
        std::size_t size;
    };
    
    std::size_t block_size_;
    std::list<BlockInfo> allocated_blocks_;
    std::pmr::memory_resource* upstream_;
    
public:
    explicit FixedBlockMemoryResource(std::size_t block_size, 
                                     std::pmr::memory_resource* upstream = std::pmr::get_default_resource());
    
    FixedBlockMemoryResource(const FixedBlockMemoryResource&) = delete;
    FixedBlockMemoryResource& operator=(const FixedBlockMemoryResource&) = delete;
    
    ~FixedBlockMemoryResource() override;
    
    std::size_t get_block_size() const { return block_size_; }
    std::size_t get_allocated_count() const { return allocated_blocks_.size(); }

protected:
    void* do_allocate(std::size_t bytes, std::size_t alignment) override;
    void do_deallocate(void* ptr, std::size_t bytes, std::size_t alignment) override;
    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override;
};
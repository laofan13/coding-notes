#include <unistd.h>

#include <stddef.h>
#include <cstdlib>
#include <cstdio>

#include <iostream>
#include <mutex>
#include <vector>
#include <atomic>


class MemoryAllocator {
private:
    static constexpr int kBlockSize = 4096;
    static constexpr size_t align = 8;
public:
    MemoryAllocator(): head(new header_t(0)), tail(nullptr) 
    {
        memory_usage_ = 0;
    }
    
    char * Allocate(size_t iSize) {
        size_t align_size = (iSize + align - 1) & ~(align - 1);
        header_t * cur = head->next;
        while(cur) {
            if(cur->free && cur->size >= align_size) 
                break;
            cur = cur->next;
        }
        if(cur) {
            cur->free = false;
            return  cur->data;
        }
        size_t total_size = sizeof(header_t) + align_size - header_pending_size;
        void *mem = allocateMem(total_size);
        if(mem == nullptr) 
            return nullptr;
        
        cur = new (mem) header_t(align_size);

        fprintf(stdout, "alloc a new block: %p, size: %d\n", cur, cur->size);

        if (tail)
            tail->next = cur;
        tail = cur;
        return cur->data;
    }

    void free(char* p) {
        header_t *cur = (header_t *)(p - sizeof(header_t) + header_pending_size);
        cur->free = true;

        // merge memory block
        header_t *head = cur->next;
        while(head && head->free) {
            cur->size += sizeof(header_t) - header_pending_size + head->size;
            head = head->next;
        }

        cur->next = head;
        if(head == nullptr)
            tail = cur;

        fprintf(stdout, "free a new block: %p, size: %d\n", cur, cur->size);
    }
private:
    char * allocateMem(size_t bytes) {
        void* result = sbrk(bytes);
        memory_usage_.fetch_add(bytes, std::memory_order_relaxed);

        fprintf(stdout, "memory_usage: %d\n", memory_usage_.load(std::memory_order_acquire));
        return reinterpret_cast<char *>(result);
    }

    char * AllocateFallback(size_t bytes) {
    }


private:
    struct alignas(8) header_t {
        uint32_t size;
        bool free;
		header_t *next;
        char data[1];

        header_t(uint32_t s)
            :size(s),
            free(false),
            next(nullptr)
        {

        }
    };
    static constexpr size_t header_pending_size = sizeof(header_t)  - offsetof(header_t, data);

    header_t *head, *tail;

    char* alloc_ptr_;
    size_t alloc_bytes_remaining_;
    std::atomic<size_t> memory_usage_;
};


int main() {
    MemoryAllocator allocator;
    char * p1 = allocator.Allocate(7);
    char * p2 = allocator.Allocate(32);
    std::cout << p1 << "\n";
    std::cout << p2 << "\n";
    allocator.free(p2);
    allocator.free(p1);
    
    return 0;
}
#include <cstdio>
#include <unistd.h>

#include <stddef.h>
#include <cstdlib>

#include <iostream>
#include <mutex>
#include <vector>

#define ERR_LOG(str, ...) {                                  \
    fprintf(stderr, "%s:%d. [ERROR]: ", __FILE__, __LINE__); \
    fprintf(stderr, str, ##__VA_ARGS__);                     \
    fprintf(stderr, "\n");                                   \
}

#define INFO_LOG(str, ...) {                                 \
    fprintf(stdout, "%s:%d. [INFO]: ", __FILE__, __LINE__);  \
    fprintf(stdout, str, ##__VA_ARGS__);                     \
    fprintf(stdout, "\n");                                   \
}

class MemoryAllocator {
private:
    constexpr static size_t align = 8;
public:
    MemoryAllocator(): head(new header_t(0)), tail(nullptr) {}
    
    void * Allocate(size_t iSize) {
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
        
        size_t total_size = sizeof(header_t) + align_size - 4;
        void *mem = sbrk(total_size);
        if(mem == nullptr) 
            return nullptr;
        
        cur = new (mem) header_t(align_size);

        fprintf(stdout, "alloc a new block: %p, size: %d\n", cur, cur->size);

        if (tail)
            tail->next = cur;
        tail = cur;
        return cur->data;
    }

    void free(void* p) {
        header_t *cur = (header_t *)((char*)p - sizeof(header_t) + 4);
        cur->free = true;

        // merge memory block
        header_t *head = cur->next;
        while(head && head->free) {
            cur->size += sizeof(header_t) - 4 + head->size;
            head = head->next;
        }

        cur->next = head;
        if(head == nullptr)
            tail = cur;

        fprintf(stdout, "free a new block: %p, size: %d\n", cur, cur->size);
    }

public:
    #pragma pack(4)
    struct header_t {
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

    header_t *head, *tail;

    char* alloc_ptr_;
    std::atomic<size_t> memory_usage_;
};


int main() {
    MemoryAllocator allocator;
    void * p1 = allocator.Allocate(7);
    void * p2 = allocator.Allocate(32);
    std::cout << p1 << "\n";
    std::cout << p2 << "\n";
    allocator.free(p2);
    allocator.free(p1);
    
    return 0;
}
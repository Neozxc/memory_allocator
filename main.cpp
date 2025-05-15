#include <iostream>
#include <cstddef>
#include <cassert>

struct BlockHeader
{
    size_t size;
    bool free;
    BlockHeader *next;
};

constexpr size_t POOL_SIZE = 1024 * 1024;
char memory_pool[POOL_SIZE];

BlockHeader *free_list = nullptr;

void initialize_memory()
{
    if (!free_list)
    {
        free_list = reinterpret_cast<BlockHeader *>(memory_pool);
        free_list->size = POOL_SIZE - sizeof(BlockHeader);
        free_list->free = true;
        free_list->next = nullptr;
    }
}

size_t align(size_t size)
{
    return (size + 7) & ~7;
}

void *my_malloc(size_t size)
{
    initialize_memory();
    size = align(size);

    BlockHeader *current = free_list;
    BlockHeader *prev = nullptr;

    while (current)
    {
        if (current->free && current->size >= size)
        {
            if (current->size >= size + sizeof(BlockHeader) + 8)
            {
                BlockHeader *new_block = reinterpret_cast<BlockHeader *>(
                    reinterpret_cast<char *>(current) + sizeof(BlockHeader) + size);
                new_block->size = current->size - size - sizeof(BlockHeader);
                new_block->free = true;
                new_block->next = current->next;

                current->size = size;
                current->next = new_block;
            }

            current->free = false;
            return reinterpret_cast<char *>(current) + sizeof(BlockHeader);
        }

        prev = current;
        current = current->next;
    }

    return nullptr;
}

void my_free(void *ptr)
{
    if (!ptr)
        return;

    BlockHeader *block = reinterpret_cast<BlockHeader *>(
        reinterpret_cast<char *>(ptr) - sizeof(BlockHeader));

    block->free = true;

    if (block->next && block->next->free)
    {
        block->size += sizeof(BlockHeader) + block->next->size;
        block->next = block->next->next;
    }
}

int main()
{
    void *p1 = my_malloc(100);
    void *p2 = my_malloc(200);

    std::cout << "Allocated p1 and p2" << std::endl;

    my_free(p1);
    my_free(p2);

    void *p3 = my_malloc(150);
    std::cout << "Allocated p3" << std::endl;

    return 0;
}

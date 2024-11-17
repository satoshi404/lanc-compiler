#include <stdio.h>
#include <stdlib.h>
#include <lanc_lexer.h>


int main() {
    char* path[] = {"int number = 4;"};
    ArrayMemoryAllocator* allocator = lanc_lexing(path);

    for (size_t i = 0; i < allocator->counter; ++i) {
        printf("Token: %s, Value: %d\n", allocator->items[i].tkstr, allocator->items[i].value);
    }

    free(allocator->items);  // Free the memory allocated for the tokens.
    return 0;
}
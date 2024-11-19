#include <stdio.h>
#include <stdlib.h>
#include <lanc_parser.h>


int main() {
    char* path[] = {"int number = 88;"};
    ArrayMemoryAllocator* allocator = lanc_lexing(path);

    for (size_t i = 0; i < allocator->counter; ++i) {
        printf("Token: %s, Value: %d\n", allocator->items[i].tkstr, allocator->items[i].value);
    }

    Node* ast = lanc_parser_parse(allocator->items);

    free(allocator->items);  // Free the memory allocated for the tokens.
    return 0;
}
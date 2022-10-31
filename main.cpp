//please define POISON and FREE (not equal)
const int POISON = 2281337;
const int FREE   = 228228228;
typedef int Type_t;
#define LOG
#include "list/list.h"

int main() {
    List list = {0};
    list_init(list);

    Node *node1 = list_push(&list, list.zero_element, 0);
    
    Node *node2 = list_push(&list, list.zero_element, 1);

    Node *node3 = list_push(&list, list.zero_element, 0);

    list_pop(&list, node1);

    list_pop(&list, node3);
}
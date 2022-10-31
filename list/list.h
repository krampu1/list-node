#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

const char *log_file_path = "log/log.html";

int first_img_file_num = 0;

const size_t MAX_COMMAND_SIZE = 30;

enum List_errors {
    INDEX_IS_TOO_LARGE    = 1,
    PUSH_AFTER_FREE_INDEX = 2,
    POP_ZERO_ELEMENT      = 4,
};

struct Node {
    Type_t data;

    Node *next;
    Node *prev;
};

struct List {
    Node *zero_element;

    size_t size;
};



#define MIN(a, b) (((a) < (b) )? (a):(b))

#define MAX(a, b) (((a) > (b) )? (a):(b))

#define list_init(list) _list_init((&(list)))

#define list_dec(list) _list_dec((&(list)))



#ifdef LOG

#define check_list(list) _check_list(list, __PRETTY_FUNCTION__, __FILE__, __LINE__)

#define warning(list, message) _log_warning(list, message, __PRETTY_FUNCTION__, __FILE__, __LINE__)

#define info(list, message)    _log_info(list, message, __PRETTY_FUNCTION__, __FILE__, __LINE__)

#define error(list, message)   _log_error(list, message, __PRETTY_FUNCTION__, __FILE__, __LINE__)

void dump(List *list); //only for Type_t == int

void _check_list(List *list, const char *func, const char *file, const size_t line);

#else 

#define check_list(list)    assert(list != nullptr);       \
                            assert(list->data != nullptr); \
                            assert(list->left != nullptr); \
                            assert(list->right != nullptr);

#define warning(message)

#define info(message)

#define error(message)

#endif

void _list_init(List *list);

void _list_dec(List *list);

Node * list_push(List *list, Node *node, Type_t a);

Node * list_num_to_index(List *list, size_t num);

int list_pop(List *list, Node *node, size_t num);

#ifdef LOG

void _log_info(List *list, const char *message, const char *func, const char *file, const size_t line);

void _log_error(List *list, const char *message, const char *func, const char *file, const size_t line);

void _log_warning(List *list, const char *message, const char *func, const char *file, const size_t line);

void _log_warning(List *list, const char *message, const char *func, const char *file, const size_t line) {
    assert(message != nullptr);

    assert(log_file_path != nullptr);
    FILE *file_ptr = fopen(log_file_path, "a");
    assert(file_ptr != nullptr);

    fprintf(file_ptr, "<font color=\"orange\">warning:\"%s\" in file:%s in func:%s in line:%zu</font>\n\n", message, file, func, line);

    fclose(file_ptr);

    dump(list);
}

void _log_info(List *list, const char *message, const char *func, const char *file, const size_t line) {
    assert(message != nullptr);

    assert(log_file_path != nullptr);
    FILE *file_ptr = fopen(log_file_path, "a");
    assert(file_ptr != nullptr);

    fprintf(file_ptr, "<font color=\"green\">info:\"%s\" in file:%s in func:%s in line:%zu</font>\n\n", message, file, func, line);

    fclose(file_ptr);

    dump(list);
}

void _log_error(List *list, const char *message, const char *func, const char *file, const size_t line) {
    assert(message != nullptr);

    assert(log_file_path != nullptr);
    FILE *file_ptr = fopen(log_file_path, "a");
    assert(file_ptr != nullptr);

    fprintf(file_ptr, "<font color=\"red\">info:\"%s\" in file:%s in func:%s in line:%zu</font>\n\n", message, file, func, line);

    fclose(file_ptr);

    dump(list);
}

#endif


int list_pop(List *list, Node *node) {
    check_list(list);
    info(list, "start pop index");

    if (list->zero_element != node);

    node->next->prev = node->prev;

    node->prev->next = node->next;

    free(list);

    list->size--;

    return 0;
}

Node * list_push(List *list, Node *node, Type_t a) {
    check_list(list);
    info(list, "start push index");

    Node *new_node = (Node *)calloc(sizeof(Node), 1);

    new_node->next = node->next;
    new_node->prev = node;
    new_node->data = a;

    node->next->prev = new_node;

    node->next = new_node;

    list->size++;

    check_list(list);
    info(list, "end pop index");

    return new_node;
}

void _list_init(List *list) {
    assert(list != nullptr);
#ifdef LOG
    FILE *file = fopen(log_file_path, "w");
    assert(file != nullptr);
    fprintf(file, "<pre>\n\n");
    fclose(file);
#endif
    list->zero_element = (Node *)calloc(sizeof(Node), 1);

    list->zero_element->next = list->zero_element;
    list->zero_element->prev = list->zero_element;
    list->zero_element->data = POISON;

    list->size = 0;

    check_list(list);
}

void _list_dec(List *list) {
    check_list(list);

    for (Node *i = list->zero_element->next->next; i != list->zero_element; i = i->next) {
        free(i->prev);
    }
    
    if (list->zero_element->prev != list->zero_element) {
        free(list->zero_element->prev);
    }

    free(list->zero_element);

    list->size = -1;
}

Node * list_num_to_index(List *list, size_t num) {
    check_list(list);

    Node *node = list->zero_element->next;
    for (size_t i = 0; i < num; i++) {
        node = node->next;
    }

    return node;
}

#ifdef LOG

#define GEN_SVC

#define ADD_SVC_TO_LOG_HTML

void _check_list(List *list, const char *func, const char *file, const size_t line) {
    if (list == nullptr) {
        _log_error(list, "list ptr = nullptr", func, file, line);
    }

    if (list->zero_element == nullptr) {
        _log_error(list, "list.zero_element ptr = nullptr", func, file, line);
    }

    Node *node = list->zero_element->next;

    for (size_t i = 0; i < list->size; i++) {
        if (node->next == nullptr) {
            _log_error(list, "list.next ptr = nullptr", func, file, line);
        }
        if (node->prev == nullptr) {
            _log_error(list, "list.prev ptr = nullptr", func, file, line);
        }
        if (node == list->zero_element) {
            _log_error(list, "list.size > real size", func, file, line);
        }
        node = node->next;
    }

    if (node != list->zero_element) {
        _log_error(list, "list.size > real size", func, file, line);
    }
}

void dump(List *list) {
    assert(list        != nullptr);

    assert(list->zero_element  != nullptr);

    GEN_SVC {
        FILE *file = fopen("log/input.dot", "w");

        fprintf(file, "digraph G {rankdir=LR;style=filled;graph [splines = headport splines=ortho];\n");

        Node *node = list->zero_element;
        for (size_t i = 0; i <= list->size; i++) {
            if (node != nullptr) {
                if (node->data == FREE) {
                    fprintf(file, "VERTEX%zu[label=\"%zu | free | l = %zu | r = %zu\", shape=\"Mrecord\", style = filled, fillcolor = \"#c0ffee\"]\n",\
                                        node,          node,        node->prev, node->next);
                }
                else if (node->data == POISON) {
                    fprintf(file, "VERTEX%zu[label=\"%zu | poison | l = %zu | r = %zu\", shape=\"Mrecord\"]\n",\
                                        node,          node,        node->prev, node->next);
                }
                else{
                    fprintf(file, "VERTEX%zu[label=\"%zu | data = %d | l = %zu | r = %zu\", shape=\"Mrecord\", style = filled, fillcolor = \"#decade\"]\n",\
                                        node,          node, node->data, node->prev, node->next);
                }
                node = node->next;
            }
        }
        node = list->zero_element;
        for (size_t i = 0; i <= list->size; i++) {
            if (node != nullptr) {
                fprintf(file, "VERTEX%zu->VERTEX%zu[color=\"green\"]\n", node, node->next);
                node = node->next;
            }
        }
        node= list->zero_element;
        for (size_t i = 0; i <= list->size; i++ ) {
            if (node != nullptr) {
                fprintf(file, "VERTEX%zu->VERTEX%zu[color=\"red\"]\n", node, node->next);
                node = node->next;
            }
        }
        fprintf(file, "}");

        fclose(file);
    }

    ADD_SVC_TO_LOG_HTML {
        char *command = (char *)calloc(sizeof(char), MAX_COMMAND_SIZE);
        assert(command != nullptr);

        sprintf(command, "dot -Tsvg log/input.dot > log/img%d.svg", first_img_file_num);

        system(command);

        free(command);

        FILE *file = fopen(log_file_path, "a");
        assert(file != nullptr);

        fprintf(file, "<img src=\"img%d.svg\" height=\"300px\" width=\"400px\"/>\n\n", first_img_file_num);

        fclose(file);

        first_img_file_num++;
    }
}

#undef GEN_SVC

#undef ADD_SVC_TO_LOG_HTML

#endif
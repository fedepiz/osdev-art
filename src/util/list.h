#ifndef UTIL_LIST_H
#define UTIL_LIST_H

#include <stddef.h>
#include <util/text.h>

namespace util {
    template <class T> class list {
        private:
        struct node_t {
            T value;
            node_t* next;
        };
        node_t* head_ptr;
        node_t* get_node(int index) {
            node_t* ptr = this->head_ptr;
            for(int i = 0; i < index; i++) {
                if(ptr == nullptr) {
                    util::panicf("Index %d too long for list\n", index);
                }
                 ptr = ptr->next;
             }
            return ptr;
        }
        node_t* last_node() {
            if(this->head_ptr == nullptr) {
                util::panicf("Cannot find last node of empty list\n");
            }
            node_t* ptr = this->head_ptr;
            while(ptr->next != nullptr) {
                ptr = ptr->next;
            }
            return ptr;
        }
        public:
        list();
        ~list();
        bool is_empty();
        size_t length();
        void prepend(T value);
        void append(T value);
        T behead();
        void remove_last();
        T get(size_t index);
        T& operator[](size_t index);
    };

    template <class T> list<T>::list() {
        this->head_ptr = nullptr;
    }

    template <class T> list<T>::~list() {
        while(this->head_ptr != nullptr) {
            node_t* toKill = this->head_ptr;
            this->head_ptr = this->head_ptr->next;
                logf("Freeing node\n");
            delete toKill;
        }
    }

    template <class T> bool list<T>::is_empty() {
        return this->head_ptr != nullptr;
    }

    template <class T> size_t list<T>::length() {
        node_t* ptr = this->head_ptr;
        size_t count = 0;;
        while(ptr != nullptr) {
            count++;
            ptr = ptr->next;
        }
        return count;
    }

    template <class T> T& list<T>::operator[](size_t index) {
        node_t* node = this->get_node(index);
        return node->value;
    }
    
    template <class T> T list<T>::get(size_t index) {
        return this[index];
    }

    template <class T> void list<T>::prepend(T value) {
        logf("Allocating new node\n");
        node_t* newNode = new node_t;
        newNode->value = value;
        newNode->next = this->head_ptr;
        this->head_ptr = newNode;
    }

    template <class T> void list<T>::append(T value) {
        logf("Allocating new node\n");
        node_t* newNode = new node_t;
        newNode->value = value;
        newNode->next = nullptr;
        if(this->head_ptr == nullptr) {
            this->head_ptr = newNode; 
        } else {
            node_t* lastNode = this->last_node();
            lastNode->next = newNode;
        }
    }

    template <class T> T list<T>::behead() {
        if(this->head_ptr == nullptr) {
            util::panicf("Cannot behead an empty list\n");
        }
        T value = this->head_ptr->value;
        node_t* toKill = this->head_ptr;
        this->head_ptr = this->head_ptr->next;
        logf("Freeing node\n");
        delete toKill;
        return value;
    }
}

#endif
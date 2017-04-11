#ifndef UTIL_LIST_H
#define UTIL_LIST_H

#include <stddef.h>
#include <util/text.h>
#include <memory/debug.h>

namespace util {
    using util::panicf;

    template <class T> class list {
        private:
        struct node_t {
            T value;
            node_t* next;
        };
        node_t* head_ptr;
        node_t* get_node(int index);
        node_t* last_node();
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
        node_t* ptr = this->head_ptr;
        while(ptr != nullptr) {
            node_t* toKill = ptr;
            ptr = ptr->next;
            delete toKill;
        }
    }

    template <class T> typename list<T>::node_t* list<T>::get_node(int index) {
        node_t* ptr = this->head_ptr;
        for(int i = 0; i < index;i++) {
            if(ptr == nullptr) {
                panicf("Cannot access list item, list too short!\n");
            }
            ptr = ptr->head_ptr;
        }
        return ptr;
    }

    template <class T> typename list<T>::node_t* list<T>::last_node() {
        if(this->head_ptr == nullptr) return nullptr;
        node_t* ptr = this->head_ptr;
        while(ptr->next != nullptr) {
            ptr = ptr->next;
        }
        return ptr;
    }

    template <class T> bool list<T>::is_empty() {
        return this->head_ptr == nullptr;
    }

    template <class T> size_t list<T>::length() {
        size_t count = 0;
        node_t* ptr = this->head_ptr;
        while(ptr->next != nullptr) {
            ptr = ptr->next;
            count++;
        }
        return count;
    }

    template <class T> T& list<T>::operator[](size_t index) {
        node_t* ptr = this->get_node(index);
        return ptr->value;
    }
    
    template <class T> T list<T>::get(size_t index) {
        return this[index];
    }

    template <class T> void list<T>::prepend(T value) {
        node_t* newNode = new node_t;
        newNode->value = value;
        newNode->next = this->head_ptr;
        this->head_ptr = newNode;
    }

    template <class T> void list<T>::append(T value) {
        node_t* newNode = new node_t;
        //logf("New node build at address %x\n", newNode);
        newNode->value = value;
        newNode->next = nullptr;
        if(this->head_ptr == nullptr) {
            this->head_ptr = newNode;
        } else {
            node_t* last = this->last_node();
            last->next = newNode;
        }
    }

    template <class T> T list<T>::behead() {
        if(this->head_ptr == nullptr) {
            panicf("Cannot behead empty an empty list\n");
        }
        T value = this->head_ptr->value;
        node_t* toKill = this->head_ptr;
        this->head_ptr = this->head_ptr->next;
        delete toKill;
        return value;
    }
}

#endif
#ifndef KSTD_VECTOR_H
#define KSTD_VECTOR_H

#include <kstdio.h>
#include <kstdlib.h>
#include <stddef.h>
#include <util/text.h>

namespace util {
    const size_t VECTOR_INIITAL_SIZE = 50;

    template <class T> class vector {
        private:
        T* array;
        size_t arr_size;
        size_t item_count;
        private:
        void grow();
        public:
        vector();
        ~vector();
        void push_back(const T& val);
        T& operator[](int index);
        T get(int index) const;
        vector<T>& operator=(const vector& other);
        size_t size() const;
        void erase(int position);
        void clear();
    };

    template <class T> vector<T>::vector() {       
        logf("Constructing vector of element size %d\n", sizeof(T));
        this->array = new T[VECTOR_INIITAL_SIZE];
        this->arr_size = VECTOR_INIITAL_SIZE;
        this->item_count = 0;
    }

    template <class T> vector<T>::~vector() {
        logf("Destroying vector of size %d\n", this->arr_size * sizeof(T));
        delete this->array;
    }

    template <class T> void vector<T>::grow() {
            size_t newSize = 2*this->arr_size;
            T* newArray = new T[newSize];
            for(size_t i = 0; i < this->arr_size;i++) {
                newArray[i] = array[i];
            }
            delete array;
            this->array = newArray;
            this->arr_size = newSize;
    }

    template <class T> void vector<T>::push_back(const T& val) {
        if(this->item_count == arr_size) {
            grow();
        }
        this->array[item_count] = val;
        item_count++;
    }

    template <class T> T vector<T>::get(int index) const {
        if(index < 0) {
            panic("Negative index in vector get");
        }
        if((size_t)index >= this->item_count) {
            panic("Vector indexed out of bounds");
        }
        return array[index];
    }

    template <class T> T& vector<T>::operator[] (int index) {
        if(index < 0) {
            panic("Negative index in vector []");
        }
        if((size_t)index >= this->item_count) {
            panic("Vector indexed out of bounds");
        }
        return array[index];
    }

    template <class T> size_t vector<T>::size() const {
        return item_count;
    }

    template <class T> void vector<T>::erase(int index) {
        if(index < 0) {
            panic("Negative index in vector::erase");
        }
        if((size_t)index >= this->item_count) {
            panic("Vector indexed out of bounds");
        }
        //Move all elements one closer
        for(size_t i = index+1; i < this->item_count;i++) {
            array[i - 1] = array[i];
        }
        //set last to zero
        kstd::memset(&array[item_count-1],0,sizeof(T));
        //shrink
        this->item_count--;
    }

    template <class T> void vector<T>::clear() {
        kstd::memset(this->array, 0, sizeof(T)*arr_size);
        this->item_count = 0;
    }

    template <class T> vector<T>& vector<T>::operator=(const vector& other) {
        this->arr_size = other->arr_size;
        this->item_count = other->item_count;
        this->array = new T[arr_size];
        kstd::memcpy(this->array, other->array, this->item_count*sizeof(T));
        return *this;
    }

};
#endif
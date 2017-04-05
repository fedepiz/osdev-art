#ifndef UTIL_VECTOR_H
#define UTIL_VECTOR_H
#include <kernel/kstd.h>
#include <stddef.h>
namespace kstd {
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
        size_t size() const;
        void erase(int position);
        void clear();
    };

    template <class T> vector<T>::vector() {
        this->array = new T[VECTOR_INIITAL_SIZE];
        this->arr_size = VECTOR_INIITAL_SIZE;
        this->item_count = 0;
    }

    template <class T> vector<T>::~vector() {
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
        memset(&array[item_count-1],0,sizeof(T));
        //shrink
        this->item_count--;
    }

    template <class T> void vector<T>::clear() {
        memset(this->array, 0, sizeof(T)*arr_size);
        this->item_count = 0;
    }
};
#endif
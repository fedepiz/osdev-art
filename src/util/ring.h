#ifndef UTIL_RING_H
#define UTIL_RING_H
#include <kstdio.h>
#include <kstdlib.h>
#include <stddef.h>

namespace util {
    template <class T> class ring {
        private:
        T* array;
        size_t arr_size;
        size_t head_index;
        size_t tail_index;
        //helpers
        void inc_index(size_t& index);
        void dec_index(size_t& index);
        public:
        size_t count();
        size_t available();
        ring(size_t maxSize);
        ~ring();
        void push_back(T entry);
        T pop_front();
        T pop_back();
        T& operator[](unsigned int pos);
    };
    template <class T> ring<T>::ring(size_t maxSize) {
        this->arr_size = maxSize+1;
        this->array = new T[maxSize+1];
        this->head_index = 0;
        this->tail_index = 0;
    }

    template <class T> ring<T>::~ring() {
        delete this->array;
    }

    template <class T> void ring<T>::inc_index(size_t& index) {
        index = (index + 1) % arr_size;
    }

    template <class T> void ring<T>::dec_index(size_t& index) {
        if(index == 0) {
            index = this->arr_size - 1;
        } else {
            index--;
        }
    }


    template <class T> size_t ring<T>::count() {
        //check if we have wrapped around yet
        if(this->head_index >= this->tail_index) {
            return this->head_index - this->tail_index;
        } else {
            //Wrapped-around, we must add size in the calculation
            return this->head_index + arr_size - this->tail_index;
        }
    }

    template <class T> size_t ring<T>::available() {
        return this->arr_size - this->count() - 1;
    }

    template <class T> void ring<T>::push_back(T entry) {
        if(this->available() == 0) {
           //throw panic
           panic("No more space availalbe in ring"); 
        }
        this->array[this->head_index] = entry;
        this->inc_index(this->head_index);
    }

     template <class T> T ring<T>::pop_front() {
         //If empty...
         if(this-> count() == 0) {
             panic("Ring is empty");
         }
         T value = this->array[this->head_index];
         this->dec_index(this->head_index);
         return value;
     }
     template <class T> T ring<T>::pop_back() {
         //If not empty
         if(this-> count() == 0) {
             panic("Ring is empty");
         }
         T value = this->array[this->tail_index];
         this->inc_index(this->tail_index);
         return value;
     }

     template <class T> T& ring<T>::operator[](unsigned int index) {
         if(this->count() <= index) {
             panic("Not enough elements in ring");
         }
         size_t pos = this->head_index - 1 - index;
         if(pos < 0) pos += this->arr_size;
         return this->array[pos];
     }
};
#endif
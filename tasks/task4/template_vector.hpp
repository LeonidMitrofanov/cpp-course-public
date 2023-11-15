#pragma once
#include "template_iterator.hpp"

template <class T>
class Vector
{
public:
    static const size_t reserve_multiply_ = 2;
    static const size_t empty_vector_size_ = 8;

public:
    using iterator = Iterator<T>;
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using size_type = size_t;

    Vector() : data_(nullptr), size_(0), capacity_(empty_vector_size_)
    {
        data_ = new T[capacity_];
    }

    ~Vector()
    {
        delete[] data_;
    }

    reference operator[] (size_type n)
    {
        if (n >= size_) {
            throw std::out_of_range("out of range");
        }
        return data_[n];
    }

    const reference operator[] (size_type n) const
    {
        if (n >= size_) {
            throw std::out_of_range("out of range");
        }
        return data_[n];
    }

    void push_back(const T& value)
    {
        if (size_ >= capacity_) {
            reserve(capacity_ * reserve_multiply_);
        }
        data_[size_] = value;
        size_++;
    }

    void push_back(T&& value)
    {
        if (size_ >= capacity_) {
            reserve(capacity_ * reserve_multiply_);
        }
        data_[size_] = std::move(value);
        size_++;
    }

    void pop_back()
    {
        if (size_ > 0) {
            data_[size_-1] = 0;
            size_--;
        }
        else {
            throw std::out_of_range("empty vector");
        }
    }

    void clear()
    {
        size_ = 0;
    }

    void reserve(size_type new_size)
    {
        if (new_size > capacity_) {
            pointer new_data = new T[new_size];
            for (size_type i = 0; i < size_; ++i) {
                new_data[i] = std::move(data_[i]);
            }
            delete[] data_;
            data_ = new_data;
            capacity_ = new_size;
        }
        else {
            for (size_type i = new_size; i < size_; i++){
                data_[i] = T();
            }
        }
    }

    void resize(size_type new_size)
    {
        reserve(new_size);
        size_ = new_size;
    }

    iterator begin() const
    {
        return iterator(data_);
    }

    iterator end() const
    {
        return iterator(data_ + size_);
    }

    size_type size() const
    {
        return size_;
    }

    size_type capacity() const
    {
        return capacity_;
    }

    bool empty() const
    {
        return size_ == 0;
    }

private:
    pointer data_;
    size_type size_;
    size_type capacity_;
};

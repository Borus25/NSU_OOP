#include "bit_array.h"
#include <stdexcept>
#include <algorithm>

constexpr size_t BITS_PER_LONG = sizeof(unsigned long) * 8;

size_t num_longs(size_t num_bits) {
    return (num_bits + BITS_PER_LONG - 1) / BITS_PER_LONG;
}

BitArray::BitArray() : data_(nullptr), size_(0), capacity_(0) {}

BitArray::~BitArray() {
    delete[] data_;
}

BitArray::BitArray(int num_bits, unsigned long value)
    : data_(nullptr), size_(num_bits), capacity_(num_bits) {
    if (num_bits > 0) {
        size_t num_elements = num_longs(num_bits);
        data_ = new unsigned long[num_elements]();
        if (value != 0) {
            data_[0] = value;
            // Обнуляем биты, выходящие за пределы size_
            if (size_ < BITS_PER_LONG) {
                unsigned long mask = (1UL << size_) - 1;
                data_[0] &= mask;
            }
        }
    }
}

BitArray::BitArray(const BitArray& b)
    : data_(nullptr), size_(b.size_), capacity_(b.capacity_) {
    if (capacity_ > 0) {
        size_t num_elements = num_longs(capacity_);
        data_ = new unsigned long[num_elements];
        std::copy(b.data_, b.data_ + num_elements, data_);
    }
}

void BitArray::swap(BitArray& b) noexcept {
    std::swap(data_, b.data_);
    std::swap(size_, b.size_);
    std::swap(capacity_, b.capacity_);
}

BitArray& BitArray::operator=(const BitArray& b) {
    if (this != &b) {
        BitArray temp(b);
        swap(temp);
    }
    return *this;
}

void BitArray::resize(size_t num_bits, bool value) {
    size_t old_size = size_;
    size_t new_capacity = num_bits;

    if (new_capacity > capacity_) {
        size_t num_elements = num_longs(new_capacity);
        unsigned long* new_data = new unsigned long[num_elements]();

        if (data_) {
            size_t old_num_elements = num_longs(capacity_);
            std::copy(data_, data_ + old_num_elements, new_data);
            delete[] data_;
        }

        data_ = new_data;
        capacity_ = new_capacity;
    }

    size_ = num_bits;

    if (num_bits > old_size) {
        for (size_t i = old_size; i < num_bits; ++i) {
            this->set(i, value);
        }
    }
}

void BitArray::clear() {
    delete[] data_;
    data_ = nullptr;
    size_ = 0;
    capacity_ = 0;
}

void BitArray::push_back(bool bit) {
    if (size_ >= capacity_) {
        size_t new_capacity = capacity_ == 0 ? BITS_PER_LONG : capacity_ * 2;
        resize(size_, false);
        capacity_ = new_capacity;

        size_t num_elements = num_longs(capacity_);
        unsigned long* new_data = new unsigned long[num_elements]();

        if (data_) {
            size_t old_num_elements = num_longs(size_);
            std::copy(data_, data_ + old_num_elements, new_data);
            delete[] data_;
        }

        data_ = new_data;
    }

    size_++;
    set(size_ - 1, bit);
}

BitArray& BitArray::operator&=(const BitArray& b) {
    if (size_ != b.size_) {
        throw std::invalid_argument("BitArray sizes must match");
    }

    size_t num_elements = num_longs(size_);
    for (size_t i = 0; i < num_elements; ++i) {
        data_[i] &= b.data_[i];
    }
    return *this;
}

BitArray& BitArray::operator|=(const BitArray& b) {
    if (size_ != b.size_) {
        throw std::invalid_argument("BitArray sizes must match");
    }

    size_t num_elements = num_longs(size_);
    for (size_t i = 0; i < num_elements; ++i) {
        data_[i] |= b.data_[i];
    }
    return *this;
}

BitArray& BitArray::operator^=(const BitArray& b) {
    if (size_ != b.size_) {
        throw std::invalid_argument("BitArray sizes must match");
    }

    size_t num_elements = num_longs(size_);
    for (size_t i = 0; i < num_elements; ++i) {
        data_[i] ^= b.data_[i];
    }
    return *this;
}

BitArray& BitArray::operator<<=(int n) {
    if (n < 0 || size_ == 0) return *this;
    if (n >= static_cast<int>(size_)) {
        reset();
        return *this;
    }

    for (int i = size_ - 1; i >= n; --i) {
        set(i, (*this)[i - n]);
    }
    for (int i = 0; i < n; ++i) {
        reset(i);
    }
    return *this;
}

BitArray& BitArray::operator>>=(int n) {
    if (n < 0 || size_ == 0) return *this;
    if (n >= static_cast<int>(size_)) {
        reset();
        return *this;
    }

    for (size_t i = 0; i < size_ - n; ++i) {
        set(i, (*this)[i + n]);
    }
    for (size_t i = size_ - n; i < size_; ++i) {
        reset(i);
    }
    return *this;
}

BitArray BitArray::operator<<(int n) const {
    BitArray result(*this);
    result <<= n;
    return result;
}

BitArray BitArray::operator>>(int n) const {
    BitArray result(*this);
    result >>= n;
    return result;
}

BitArray& BitArray::set(int n, bool val) {
    if (n < 0 || n >= static_cast<int>(size_)) {
        throw std::out_of_range("Index out of range");
    }

    size_t long_index = n / BITS_PER_LONG;
    size_t bit_index = n % BITS_PER_LONG;

    if (val) {
        data_[long_index] |= (1UL << bit_index);
    } else {
        data_[long_index] &= ~(1UL << bit_index);
    }
    return *this;
}

BitArray& BitArray::set() {
    size_t num_elements = num_longs(size_);
    for (size_t i = 0; i < num_elements; ++i) {
        data_[i] = ~0UL;
    }

    if (size_ % BITS_PER_LONG != 0) {
        size_t last_bits = size_ % BITS_PER_LONG;
        unsigned long mask = (1UL << last_bits) - 1;
        data_[num_elements - 1] &= mask;
    }
    return *this;
}

BitArray& BitArray::reset(int n) {
    return set(n, false);
}

BitArray& BitArray::reset() {
    size_t num_elements = num_longs(size_);
    for (size_t i = 0; i < num_elements; ++i) {
        data_[i] = 0;
    }
    return *this;
}

bool BitArray::any() const {
    size_t num_elements = num_longs(size_);
    for (size_t i = 0; i < num_elements; ++i) {
        if (data_[i] != 0) {
            return true;
        }
    }
    return false;
}

bool BitArray::none() const {
    return !any();
}

BitArray BitArray::operator~() const {
    if (data_ == nullptr) {
        return {};
    }

    BitArray result(*this);
    size_t num_elements = num_longs(size_);
    for (size_t i = 0; i < num_elements; ++i) {
        result.data_[i] = ~result.data_[i];
    }

    if (size_ % BITS_PER_LONG != 0) {
        size_t last_bits = size_ % BITS_PER_LONG;
        unsigned long mask = (1UL << last_bits) - 1;
        result.data_[num_elements - 1] &= mask;
    }

    return result;
}

int BitArray::count() const {
    int counter = 0;
    size_t num_elements = num_longs(size_);
    for (size_t i = 0; i < num_elements; ++i) {
        unsigned long val = data_[i];
        while (val) {
            counter += val & 1;
            val >>= 1;
        }
    }
    return counter;
}

bool BitArray::operator[](int i) const {
    if (i < 0 || i >= static_cast<int>(size_)) {
        throw std::out_of_range("Index out of range");
    }

    size_t long_index = i / BITS_PER_LONG;
    size_t bit_index = i % BITS_PER_LONG;

    return (data_[long_index] & (1UL << bit_index)) != 0;
}

int BitArray::size() const {
    return static_cast<int>(size_);
}

bool BitArray::empty() const {
    return size_ == 0;
}

std::string BitArray::to_string() const {
    if (size_ == 0) {
        return "";
    }

    std::string result;
    result.reserve(size_);

    size_t num_elements = num_longs(size_);
    size_t full_blocks = size_ / BITS_PER_LONG;
    size_t remaining_bits = size_ % BITS_PER_LONG;

    if (remaining_bits > 0) {
        size_t last_index = num_elements - 1;
        unsigned long last_block = data_[last_index];

        for (int i = remaining_bits - 1; i >= 0; --i) {
            result += (last_block & (1UL << i)) ? '1' : '0';
        }
    }

    for (int block_idx = full_blocks - 1; block_idx >= 0; --block_idx) {
        unsigned long block = data_[block_idx];

        for (int bit = BITS_PER_LONG - 1; bit >= 0; --bit) {
            result += (block & (1UL << bit)) ? '1' : '0';
        }
    }

    return result;
}

bool operator==(const BitArray& a, const BitArray& b) {
    if (a.size() != b.size()) return false;

    for (int i = 0; i < a.size(); ++i) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

bool operator!=(const BitArray& a, const BitArray& b) {
    return !(a == b);
}

BitArray operator&(const BitArray& b1, const BitArray& b2) {
    BitArray result(b1);
    result &= b2;
    return result;
}

BitArray operator|(const BitArray& b1, const BitArray& b2) {
    BitArray result(b1);
    result |= b2;
    return result;
}

BitArray operator^(const BitArray& b1, const BitArray& b2) {
    BitArray result(b1);
    result ^= b2;
    return result;
}

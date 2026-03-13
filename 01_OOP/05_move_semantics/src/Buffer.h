#pragma once
#include <iostream>
#include <string>
#include <cstring>

// =============================================================
// Klasa Buffer ilustruje move semantics (C++11)
// =============================================================
class Buffer {
public:
    std::string name_;
    size_t      size_;
    int*        data_;

    // --- Konstruktor ---
    Buffer(const std::string& name, size_t size)
        : name_(name), size_(size), data_(new int[size_]{}) {
        for (size_t i = 0; i < size_; ++i) data_[i] = static_cast<int>(i);
        std::cout << "[Buffer] Konstruktor: " << name_ << " @ " << data_ << "\n";
    }

    // --- Konstruktor kopiujący (głęboka kopia) ---
    Buffer(const Buffer& other)
        : name_(other.name_ + "_copy"), size_(other.size_),
          data_(new int[other.size_]) {
        std::memcpy(data_, other.data_, size_ * sizeof(int));
        std::cout << "[Buffer] Kopia: " << name_
                  << " (nowy blok @ " << data_ << ")\n";
    }

    // --- Konstruktor PRZENOSZĄCY (move constructor) ---
    // Przyjmuje r-value reference (&&) – tymczasowy lub std::move()
    Buffer(Buffer&& other) noexcept
        : name_(std::move(other.name_)),   // przenieś string
          size_(other.size_),
          data_(other.data_) {             // PRZEJMIJ wskaźnik
        other.data_ = nullptr;             // unieważnij źródło
        other.size_ = 0;
        std::cout << "[Buffer] MOVE ctor: " << name_
                  << " (przejęto @ " << data_ << ")\n";
    }

    // --- Operator przypisania kopiującego ---
    Buffer& operator=(const Buffer& other) {
        if (this == &other) return *this;
        delete[] data_;
        name_ = other.name_ + "_assigned";
        size_ = other.size_;
        data_ = new int[size_];
        std::memcpy(data_, other.data_, size_ * sizeof(int));
        std::cout << "[Buffer] operator= (copy)\n";
        return *this;
    }

    // --- Operator PRZYPISANIA PRZENOSZĄCEGO (move assignment) ---
    Buffer& operator=(Buffer&& other) noexcept {
        if (this == &other) return *this;
        delete[] data_;                    // zwolnij własne zasoby
        name_ = std::move(other.name_);
        size_ = other.size_;
        data_ = other.data_;               // przejmij wskaźnik
        other.data_ = nullptr;
        other.size_ = 0;
        std::cout << "[Buffer] operator= (MOVE)\n";
        return *this;
    }

    // --- Destruktor ---
    ~Buffer() {
        std::cout << "[Buffer] ~dtor: " << name_
                  << " @ " << data_ << "\n";
        delete[] data_;
    }

    void print() const {
        std::cout << "Buffer '" << name_ << "' [@ " << data_
                  << ", size=" << size_ << "]: ";
        if (data_) {
            for (size_t i = 0; i < size_; ++i)
                std::cout << data_[i] << " ";
        } else {
            std::cout << "(moved-from – pusty)";
        }
        std::cout << "\n";
    }

    bool valid() const { return data_ != nullptr; }
};

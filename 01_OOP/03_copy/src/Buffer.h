#pragma once
#include <iostream>
#include <string>
#include <cstring>

// =============================================================
// ShallowBuffer – kopia PŁYTKA (brak własnego konstruktora kopiującego)
// Oba obiekty wskazują na ten SAM obszar pamięci → błąd podwójnego free!
// =============================================================
class ShallowBuffer {
public:
    int size_;
    int* data_;   // wskaźnik na dynamiczną tablicę

    explicit ShallowBuffer(int size) : size_(size) {
        data_ = new int[size_];
        for (int i = 0; i < size_; ++i) data_[i] = i;
        std::cout << "[ShallowBuffer] Konstruktor, data @ " << data_ << "\n";
    }

    // Brak własnego konstruktora kopiującego!
    // Kompilator wygeneruje PŁYTKĄ kopię: data_ będzie skopiowany jako wskaźnik.

    ~ShallowBuffer() {
        std::cout << "[ShallowBuffer] Destruktor, data @ " << data_ << "\n";
        delete[] data_;   // UWAGA: oba obiekty próbują zwolnić ten sam blok!
    }

    void set(int index, int value) { data_[index] = value; }
    int  get(int index) const      { return data_[index];  }

    void print(const std::string& label) const {
        std::cout << label << " [@ " << data_ << "]: ";
        for (int i = 0; i < size_; ++i)
            std::cout << data_[i] << " ";
        std::cout << "\n";
    }
};

// =============================================================
// DeepBuffer – kopia GŁĘBOKA (własny konstruktor kopiujący)
// Każdy obiekt ma WŁASNĄ kopię danych → bezpieczne!
// =============================================================
class DeepBuffer {
public:
    int  size_;
    int* data_;

    explicit DeepBuffer(int size) : size_(size) {
        data_ = new int[size_];
        for (int i = 0; i < size_; ++i) data_[i] = i;
        std::cout << "[DeepBuffer] Konstruktor, data @ " << data_ << "\n";
    }

    // Konstruktor kopiujący – GŁĘBOKA kopia
    DeepBuffer(const DeepBuffer& other) : size_(other.size_) {
        data_ = new int[size_];                      // NOWY blok pamięci
        std::memcpy(data_, other.data_, size_ * sizeof(int)); // kopiuj dane
        std::cout << "[DeepBuffer] Kopia głęboka: " << other.data_
                  << " → " << data_ << "\n";
    }

    // Operator przypisania kopiującego (reguła trzech)
    DeepBuffer& operator=(const DeepBuffer& other) {
        if (this == &other) return *this;            // ochrona przed self-assignment
        delete[] data_;
        size_ = other.size_;
        data_ = new int[size_];
        std::memcpy(data_, other.data_, size_ * sizeof(int));
        std::cout << "[DeepBuffer] operator= (deep copy)\n";
        return *this;
    }

    ~DeepBuffer() {
        std::cout << "[DeepBuffer] Destruktor, data @ " << data_ << "\n";
        delete[] data_;
    }

    void set(int index, int value) { data_[index] = value; }
    int  get(int index) const      { return data_[index];  }

    void print(const std::string& label) const {
        std::cout << label << " [@ " << data_ << "]: ";
        for (int i = 0; i < size_; ++i)
            std::cout << data_[i] << " ";
        std::cout << "\n";
    }
};

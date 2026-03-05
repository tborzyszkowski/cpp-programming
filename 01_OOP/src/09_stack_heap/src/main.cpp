#include <iostream>
#include <vector>
#include <memory>
#include "MemoryDemo.h"

// ============================================================
// Demonstracja: stos i sterta – zarządzanie pamięcią w C++
// Kompilacja: g++ -std=c++17 -o memory src/main.cpp
// ============================================================

// ---- Funkcja ilustrująca stos ----
void stackDemo() {
    std::cout << "\n=== STOS (Stack) ===\n";
    // Obiekty na stosie – automatyczne zarządzanie pamięcią
    int a = 10;
    double b = 3.14;
    Point3D p1(1, 1.0, 2.0, 3.0);   // na stosie
    Point3D p2(2, 4.0, 5.0, 6.0);   // na stosie

    std::cout << "p1 @ " << &p1 << ": " << p1.toString() << "\n";
    std::cout << "p2 @ " << &p2 << ": " << p2.toString() << "\n";

    {
        Point3D p3(3, 7.0, 8.0, 9.0);   // wewnętrzny blok
        std::cout << "Wewnątrz bloku: p3 @ " << &p3 << "\n";
    }   // ← destruktor p3 wywołany AUTOMATYCZNIE tu

    std::cout << "p2 nadal żyje: " << p2.toString() << "\n";
}   // ← destruktory p1, p2 wywołane automatycznie

// ---- Funkcja ilustrująca stertę ----
void heapDemo() {
    std::cout << "\n=== STERTA (Heap) ===\n";

    // new – alokacja pojedynczego obiektu
    Point3D* p1 = new Point3D(10, 1.0, 2.0, 3.0);
    std::cout << "p1 (heap) @ " << p1 << ": " << p1->toString() << "\n";

    // Modyfikacja przez wskaźnik
    p1->x = 99.0;
    std::cout << "Po modyfikacji: " << p1->toString() << "\n";

    delete p1;        // ← destruktor wywołany RĘCZNIE
    p1 = nullptr;     // ← dobra praktyka: uникaj dangling pointer

    // new[] – alokacja tablicy
    std::cout << "\n--- Tablica na stercie ---\n";
    const int N = 3;
    Point3D** arr = new Point3D*[N];
    for (int i = 0; i < N; ++i)
        arr[i] = new Point3D(100 + i, i * 1.0, i * 2.0, i * 3.0);

    std::cout << "Tablica punktów:\n";
    for (int i = 0; i < N; ++i)
        std::cout << "  " << arr[i]->toString() << "\n";

    // Zwolnienie tablicy obiektów
    for (int i = 0; i < N; ++i) {
        delete arr[i];
        arr[i] = nullptr;
    }
    delete[] arr;
    arr = nullptr;
}

// ---- Wyciek pamięci (celowy przykład) ----
void memoryLeakDemo() {
    std::cout << "\n=== WYCIEK PAMIĘCI (brak delete!) ===\n";
    // ResourceLeakDemo nie ma destruktora!
    ResourceLeakDemo* leak = new ResourceLeakDemo("LEAK", 1000);
    std::cout << "Brak delete → pamięć @ " << leak->data_
              << " nigdy nie zostanie zwolniona!\n";
    // delete leak;  ← BEZ TEGO jest wyciek
    // W C++ brak GC – musisz sам zarządzać pamięcią!
}

// ---- Prawidłowe zwalnianie (RAII) ----
void raii_demo() {
    std::cout << "\n=== RAII – prawidłowe zarządzanie zasobem ===\n";
    {
        ResourceSafeDemo safe("SAFE", 1000);
        std::cout << "Obiekt RAII żyje: " << safe.label_ << "\n";
    }   // ← destruktor zwolni pamięć automatycznie przy wyjściu ze scope
    std::cout << "Po scope – pamięć zwolniona\n";
}

// ---- Smart pointers (C++11) jako alternatywa dla ręcznego delete ----
void smartPointerDemo() {
    std::cout << "\n=== SMART POINTERS (C++11) ===\n";

    // unique_ptr – jeden właściciel, automatyczne delete
    {
        auto p = std::make_unique<Point3D>(20, 5.0, 6.0, 7.0);
        std::cout << "unique_ptr: " << p->toString() << "\n";
    }   // ← delete wywołany automatycznie przez unique_ptr

    // shared_ptr – współdzielona własność, delete gdy refcount==0
    {
        auto sp1 = std::make_shared<Point3D>(21, 8.0, 9.0, 10.0);
        auto sp2 = sp1;    // shared ownership
        std::cout << "shared_ptr use_count: " << sp1.use_count() << "\n";
    }   // ← delete gdy sp1 i sp2 wyjdą ze scope

    std::cout << "Smart pointery zwolniły pamięć automatycznie\n";
}

int main() {
    std::cout << "=== Demonstracja Stosu i Sterty w C++ ===\n";
    std::cout << "C++ NIE MA garbage collectora!\n";
    std::cout << "Każdy 'new' musi mieć odpowiadający 'delete'.\n";

    stackDemo();
    heapDemo();
    // memoryLeakDemo();   // odkomentuj żeby zobaczyć wyciek
    raii_demo();
    smartPointerDemo();

    std::cout << "\n=== Koniec programu ===\n";
    return 0;
}

#include <iostream>
#include "TemperatureSensor.h"

// ============================================================
// Rozwiązanie Zadania 3: TemperatureSensor + DataExporter (friend class)
// Kompilacja: g++ -std=c++17 -Wall -Wextra -o ex3_sensor main.cpp
// ============================================================

int main() {
    TemperatureSensor sensorA("A1", 80.0);
    TemperatureSensor sensorB("B2", 40.0);

    // Symulacja odczytów
    for (double t : {21.3, 35.0, 55.8, 72.1, 87.5, 92.1})
        sensorA.addReading(t);

    for (double t : {18.0, 22.5, 38.0, 41.2, 39.8, 45.0, 28.3})
        sensorB.addReading(t);

    std::cout << "\nAktualny odczyt A1: " << sensorA.current() << " C\n";
    std::cout << "Aktualny odczyt B2: " << sensorB.current() << " C\n";

    DataExporter exporter;

    exporter.printStats(sensorA);
    exporter.printStats(sensorB);

    // Anomalie
    std::cout << "\nAnomalie A1: ";
    for (double a : exporter.anomalies(sensorA))
        std::cout << a << "  ";
    std::cout << "\n";

    std::cout << "Anomalie B2: ";
    for (double a : exporter.anomalies(sensorB))
        std::cout << a << "  ";
    std::cout << "\n";

    // Eksport CSV
    exporter.exportCSV(sensorA, "sensor_A1.csv");
    exporter.exportCSV(sensorB, "sensor_B2.csv");

    return 0;
}

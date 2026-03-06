#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <fstream>
#include <iomanip>

// Forward declaration
class DataExporter;

// =============================================================
// Klasa TemperatureSensor
// =============================================================
class TemperatureSensor {
private:
    std::string          sensorId_;
    std::vector<double>  readings_;
    double               threshold_;   // próg alarmu [°C]

public:
    TemperatureSensor(const std::string& id, double threshold)
        : sensorId_(id), threshold_(threshold) {}

    void addReading(double temp) {
        readings_.push_back(temp);
        if (temp > threshold_) {
            std::cout << "[ALARM] Czujnik " << sensorId_ << ": "
                      << std::fixed << std::setprecision(2) << temp
                      << " C przekracza prog " << threshold_ << " C!\n";
        }
    }

    double current() const {
        if (readings_.empty())
            return std::numeric_limits<double>::quiet_NaN();
        return readings_.back();
    }

    std::string id() const { return sensorId_; }

    // ---- zaprzyjaźnienie ----
    friend class DataExporter;
};

// =============================================================
// Klasa DataExporter – friend TemperatureSensor
// Ma dostęp do prywatnych: readings_, threshold_, sensorId_
// =============================================================
class DataExporter {
public:
    void printStats(const TemperatureSensor& sensor) const {
        const auto& r = sensor.readings_;   // prywatne!
        if (r.empty()) {
            std::cout << "Brak danych dla czujnika " << sensor.sensorId_ << "\n";
            return;
        }

        double minVal = *std::min_element(r.begin(), r.end());
        double maxVal = *std::max_element(r.begin(), r.end());
        double avg    = std::accumulate(r.begin(), r.end(), 0.0)
                        / static_cast<double>(r.size());
        long overCount = std::count_if(r.begin(), r.end(),
                                       [&](double t){ return t > sensor.threshold_; });

        std::cout << "\n=== Statystyki czujnika " << sensor.sensorId_ << " ===\n";
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Liczba odczytow : " << r.size() << "\n";
        std::cout << "Min / Max       : " << minVal << " / " << maxVal << " C\n";
        std::cout << "Srednia         : " << avg << " C\n";
        std::cout << "Przekroczenia   : " << overCount
                  << "  (prog: " << sensor.threshold_ << " C)\n";
    }

    std::vector<double> anomalies(const TemperatureSensor& sensor) const {
        std::vector<double> result;
        for (double t : sensor.readings_)          // prywatne!
            if (t > sensor.threshold_)             // prywatne!
                result.push_back(t);
        return result;
    }

    void exportCSV(const TemperatureSensor& sensor,
                   const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Blad: nie mozna otworzyc pliku " << filename << "\n";
            return;
        }
        file << "index,temperature_C\n";
        for (std::size_t i = 0; i < sensor.readings_.size(); ++i)
            file << i << "," << std::fixed << std::setprecision(2)
                 << sensor.readings_[i] << "\n";

        std::cout << "Eksport -> " << filename
                  << " (" << sensor.readings_.size() << " rekordow)\n";
    }
};

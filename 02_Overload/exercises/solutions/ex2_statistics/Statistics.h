#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <stdexcept>

class Statistics {
    std::vector<double> data_;
    std::string label_;

public:
    explicit Statistics(const std::string& label) : label_(label) {}

    // --- funktor: łańcuchowe dodawanie wartości ---
    Statistics& operator()(double val) {
        data_.push_back(val);
        return *this;
    }

    // --- konwersje ---
    explicit operator double() const { return mean(); }
    explicit operator bool() const { return !data_.empty(); }

    // --- statystyki ---
    size_t count() const { return data_.size(); }

    double mean() const {
        if (data_.empty()) throw std::logic_error("Statistics::mean(): brak danych");
        return std::accumulate(data_.begin(), data_.end(), 0.0) / data_.size();
    }

    double stddev() const {
        if (data_.empty()) throw std::logic_error("Statistics::stddev(): brak danych");
        double m = mean();
        double sq_sum = 0.0;
        for (double x : data_) sq_sum += (x - m) * (x - m);
        return std::sqrt(sq_sum / data_.size());
    }

    double min() const {
        if (data_.empty()) throw std::logic_error("Statistics::min(): brak danych");
        return *std::min_element(data_.begin(), data_.end());
    }

    double max() const {
        if (data_.empty()) throw std::logic_error("Statistics::max(): brak danych");
        return *std::max_element(data_.begin(), data_.end());
    }

    // --- scalanie ---
    Statistics operator+(const Statistics& rhs) const {
        Statistics result(label_ + " + " + rhs.label_);
        result.data_ = data_;
        result.data_.insert(result.data_.end(), rhs.data_.begin(), rhs.data_.end());
        return result;
    }

    // --- strumień ---
    friend std::ostream& operator<<(std::ostream& os, const Statistics& s) {
        os << std::fixed << std::setprecision(2);
        os << s.label_ << ": [n=" << s.count()
           << "] avg=" << s.mean()
           << " stddev=" << s.stddev()
           << " min=" << s.min()
           << " max=" << s.max();
        return os;
    }
};

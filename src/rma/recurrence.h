//
//                  Recurrence Header
//
//  ------------------------------------------------------------------------------------------------------------------
#pragma once
//  ------------------------------------------------------------------------------------------------------------------
#ifndef RMA_RECURRENCE_H
#define RMA_RECURRENCE_H
//  ------------------------------------------------------------------------------------------------------------------
//          Import necessary libraries:
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <memory>
namespace py = pybind11;
//  ------------------------------------------------------------------------------------------------------------------
//          Necessary library files:
#include "../utils/metric.h"
//  ------------------------------------------------------------------------------------------------------------------
//          Name of available recurrence functions.
enum class RecurrenceFunction { Standard, Corridor, JRP };
//  ------------------------------------------------------------------------------------------------------------------
///         Virtual class to recurrence functions.
class IRecurrence {
protected:
    const std::unique_ptr<IMetric> &metric;
public:
    virtual bool compute(const double* x_ptr, const double* y_ptr, ssize_t shape, unsigned long stride, ssize_t x_idx, ssize_t y_idx) const = 0;
    explicit IRecurrence(const std::unique_ptr<IMetric> &metric) : metric(metric) {}
    virtual ~IRecurrence() = default;
};
//  ------------------------------------------------------------------------------------------------------------------
//            Standard recurrence
class StandardRecurrence final : public IRecurrence {
    double threshold;
public:
    explicit StandardRecurrence(const std::unique_ptr<IMetric> &metric, py::object const &params) : IRecurrence(metric) {
        //      Verify if the param is a float.
        if (!py::isinstance<py::float_>(params)) throw std::invalid_argument("You are using a standard recurrence function, so params must be a float.");
        threshold = params.cast<double>();
    }

    [[nodiscard]] bool compute(
        const double* x_ptr,                  //    Input `x`
        const double* y_ptr,                  //    Input `y`
        const ssize_t shape,                   //    First dimension size
        const unsigned long stride,            //    Stride of the first dimension
        const ssize_t x_idx,                   //    Pre-computed offset of `x`
        const ssize_t y_idx                    //    Pre-computed offset of `y`
    ) const override {
        return metric->evaluate(x_ptr, y_ptr, shape, stride, x_idx, y_idx) <= threshold;
    }
};
//  ------------------------------------------------------------------------------------------------------------------
//            Corridor recurrence
class CorridorRecurrence final : public IRecurrence {
    double threshold_min;
    double threshold_max;
public:
    explicit CorridorRecurrence(const std::unique_ptr<IMetric> &metric, py::object const &params) : IRecurrence(metric) {
        if (!(py::isinstance<py::tuple>(params) || py::isinstance<py::list>(params))) {
            throw std::invalid_argument("You are using a corridor recurrence function, so params must be a list or tuple with 2 elements.");
        }

        const auto threshold = params.cast<std::vector<double>>();
        if (threshold.size() != 2) {
            throw std::invalid_argument("params must contain exactly 2 elements.");
        }

        threshold_min = threshold[0];
        threshold_max = threshold[1];
    }

    [[nodiscard]] bool compute(
        const double* x_ptr,                  //    Input `x`
        const double* y_ptr,                  //    Input `y`
        const ssize_t shape,                   //    First dimension size
        const unsigned long stride,            //    Stride of the first dimension
        const ssize_t x_idx,                   //    Pre-computed offset of `x`
        const ssize_t y_idx                    //    Pre-computed offset of `y`
    ) const override {
        const double distance = metric->evaluate(x_ptr, y_ptr, shape, stride, x_idx, y_idx);
        return distance <= threshold_max && distance >= threshold_min;
    }
};
//  ------------------------------------------------------------------------------------------------------------------
//            Standard recurrence
class JRP final : public IRecurrence {
    double threshold_x;
    double threshold_y;
public:
    explicit JRP(const std::unique_ptr<IMetric> &metric, py::object const &params) : IRecurrence(metric) {
        if (!(py::isinstance<py::tuple>(params) || py::isinstance<py::list>(params))) {
            if (!py::isinstance<py::float_>(params)) throw std::invalid_argument("You are using a JRP, so params must be a float, or a list or tuple with 2 elements.");
            threshold_x = params.cast<double>();
            threshold_y = threshold_x;
        }

        const auto threshold = params.cast<std::vector<double>>();
        if (threshold.size() != 2) {
            throw std::invalid_argument("params must contain exactly 2 elements.");
        }

        threshold_x = threshold[0];
        threshold_y = threshold[1];
    }

    [[nodiscard]] bool compute(
        const double* x_ptr,                  //    Input `x`
        const double* y_ptr,                  //    Input `y`
        const ssize_t shape,                   //    First dimension size
        const unsigned long stride,            //    Stride of the first dimension
        const ssize_t x_idx,                   //    Pre-computed offset of `x`
        const ssize_t y_idx                    //    Pre-computed offset of `y`
    ) const override {
        return (metric->evaluate(x_ptr, x_ptr, shape, stride, x_idx, y_idx) <= threshold_x) && (metric->evaluate(y_ptr, y_ptr, shape, stride, x_idx, y_idx) <= threshold_y);
    }
};
//  ------------------------------------------------------------------------------------------------------------------
#endif
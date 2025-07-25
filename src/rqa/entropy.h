//
//                  Recurrence Microstates Entropy (RME) Header
//
//  ------------------------------------------------------------------------------------------------------------------
#pragma once
//  ------------------------------------------------------------------------------------------------------------------
#ifndef RQA_ENTROPY_H
#define RQA_ENTROPY_H
//  ------------------------------------------------------------------------------------------------------------------
//          Import necessary libraries:
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <vector>
namespace py = pybind11;
//  ------------------------------------------------------------------------------------------------------------------
///         Compute the recurrence entropy from a distribution.
inline double entropy(const std::vector<double>& dist) {
    //          Prepare...
    double result = 0.0;

    for (const double p : dist) {
        if (p > 0.0) result += (-1) * p * std::log2(p);
    }

    //          Return our result =3
    return result;
}
//  ------------------------------------------------------------------------------------------------------------------
#endif
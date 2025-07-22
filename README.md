# RMAPy: A Python library for analyzing dynamical systems with recurrence microstates

### 📦 Dependencies

You will need the libraries `pybind11` and `setuptools` to compile `rmapy`. Moreover, when using `rmapy`,
the `numpy` library is required to provide input data and receive output, so it is also necessary.

To install the dependencies, please run:
```text
pip install pybind11 setuptools numpy
```

### ⚙️ Installation

With the dependencies installed on your system, clone the `rmapy` GitHub
repository into any directory and access the cloned repo folder:
```text
git clone https://github.com/DynamicsUFPR/recurrence-microstates-python.git
cd recurrence-microstates-python
```

Then, try installing it using:
```text
pip install .
```

There might be an issue with the `.so` file name. If this happens,
try renaming the folder `recurrence-microstates-python` to `recurrence_microstates`, and run `pip install .` again.

## Library usage guide

The library has been written with a structure similar to [Julia' version](https://github.com/DynamicsUFPR/RecurrenceMicrostatesAnalysis.jl),
so you can use its documentation as a reference. However, since `RMAPy` is not a native library like `RecurrenceMicrostatesAnalysis.jl`, there are
some variations in how *kwarg* settings are implemented.

```python
import numpy as np
import recurrence_microstates as rma

data = np.random.rand(3, 1000).astype(np.float64)
dist = rma.distribution(data, 0.2, 3)
s = rma.entropy(dist)

print(dist)
print(s)
```

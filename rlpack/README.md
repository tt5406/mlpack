# mlpack reinforcement learning subset

This directory provides a minimal CMake project that builds only the
reinforcement learning components from the mlpack source tree.  It reuses the
headers that live in `../src` and configures the third-party dependencies that
these components rely on (Armadillo, ensmallen, cereal, and Boost).

Two build targets are generated:

* `mlpack_rl_example`: a trivial executable that includes
  `mlpack/methods/reinforcement_learning/reinforcement_learning.hpp`.  This
  serves as a quick smoke test to ensure that all dependencies are correctly
  wired and that the headers compile.
* `mlpack_rl`: an interface library that downstream projects can link against to
  consume the reinforcement learning headers without the rest of mlpack.

To build the subset, run the following commands from the repository root:

```bash
cmake -S rlpack -B rlpack/build
cmake --build rlpack/build
```

The resulting interface target and install rules provide all of the headers
required by the reinforcement learning code, including the ANN module and the
core mlpack utilities that it depends on.

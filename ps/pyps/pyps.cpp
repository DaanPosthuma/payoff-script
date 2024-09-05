#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
//#include <ps/ps.h>

namespace py = pybind11;

namespace {

using namespace std::chrono_literals;

}  // namespace

PYBIND11_MODULE(pymd, m) {
  m.doc() = "payoff script";
}

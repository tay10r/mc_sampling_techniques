#include "samplers.hpp"

#include <memory>

#include <cmath>
#include <cstdio>

namespace {

double
func_1D(double X)
{
  return std::sin(X) + (0.2 * std::cos(16 * X)) + (0.1 * std::sin(128 * X)) +
         (0.4 * std::sqrt(X));
}

double
func_2D(double X, double Y)
{
  // 2x^2 + 3y^5 + 7xy + 11ysin(13x)
  return (2 * X * X) + (3 * std::pow(Y, 5)) + (7 * X * Y) +
         (11 * Y * std::sin(13 * X));
}

std::pair<size_t, double>
run_sampler_1D(sampler& Sampler)
{
  std::size_t Threshold = 4096;

  std::size_t Step = 1;

  double Expected = 1.375076308106107;

  double Bias = 0.000000000000001;

  double Result = 0;

  for (size_t I = 1; I < Threshold; I += Step) {

    Result = Sampler.sample_1D(func_1D, 4.0, 7.0, I);

    double Min = Result - Bias;
    double Max = Result + Bias;

    if ((Min <= Expected) && (Max > Expected))
      return { I, std::abs((Expected - Result) / Expected) };
  }

  return { Threshold, std::abs((Expected - Result) / Expected) };
}

std::pair<size_t, double>
run_sampler_2D(sampler& Sampler)
{
  std::size_t Threshold = 4096;

  std::size_t Step = 1;

  double Expected = 1546.093263048995;

  double Bias = 0.000000000001;

  double Result = 0;

  double XMin = 2.0;
  double XMax = 5.0;

  double YMin = 1.0;
  double YMax = 3.0;

  for (size_t I = 1; I < Threshold; I += Step) {

    Result = Sampler.sample_2D(func_2D, XMin, XMax, YMin, YMax, I);

    double Min = Result - Bias;
    double Max = Result + Bias;

    if ((Min <= Expected) && (Max > Expected))
      return { I, std::abs((Expected - Result) / Expected) };
  }

  return { Threshold, std::abs((Expected - Result) / Expected) };
}

void
run_sampler_test(const char* Name, sampler* SamplerPtr)
{
  std::unique_ptr<sampler> Sampler(SamplerPtr);

  using ulong = unsigned long int;

  auto Result1D = run_sampler_1D(*Sampler);
  auto Count1D = ulong(Result1D.first);
  auto Error1D = Result1D.second * 100;
  printf("%15s %14lu %10f%%  1D\n", Name, Count1D, Error1D);

  auto Result2D = run_sampler_2D(*Sampler);
  auto Count2D = ulong(Result2D.first);
  auto Error2D = Result2D.second * 100;
  printf("%15s %14lu %10f%%  2D\n", Name, Count2D, Error2D);
}

} // namespace

int
main()
{
  printf(
    "%15s %14s %10s  %s\n", "Sampler Name", "Samples Taken", "Error", "Dim");

  printf(
    "%15s %14s %10s  %s\n", "------------", "-------------", "-----", "---");

  run_sampler_test("Uniform", new uniform_sampler());

  run_sampler_test("Random", new random_sampler());

  run_sampler_test("Grid Stratified", new grid_stratified_sampler());

  return 0;
}

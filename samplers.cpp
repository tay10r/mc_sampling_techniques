#include "samplers.hpp"

#include <random>

double
uniform_sampler::sample_1D(const std::function<double(double)>& F,
                           double DomainMin,
                           double DomainMax,
                           size_t N) const
{
  double Result = 0.0;

  auto DX = (DomainMax - DomainMin) / N;

  for (size_t I = 0; I < N; I++)
    Result += F(DomainMin + (DX * I)) * DX;

  return Result;
}

double
uniform_sampler::sample_2D(const std::function<double(double, double)>& F,
                           double DomainXMin,
                           double DomainXMax,
                           double DomainYMin,
                           double DomainYMax,
                           size_t N) const
{
  auto XRange = DomainXMax - DomainXMin;
  auto YRange = DomainYMax - DomainYMin;

  size_t SampleCountX = 0;
  size_t SampleCountY = 0;

  if (YRange > XRange) {
    SampleCountX = std::sqrt(double(N));
    SampleCountY = N / SampleCountX;
  } else {
    SampleCountY = std::sqrt(double(N));
    SampleCountX = N / SampleCountY;
  }

  double Sum = 0;

  for (size_t X = 0; X < SampleCountX; X++) {

    for (size_t Y = 0; Y < SampleCountY; Y++) {

      double U = double(X) / SampleCountX;
      double V = double(Y) / SampleCountY;

      Sum += F(DomainXMin + (U * XRange), DomainYMin + (V * YRange));
    }
  }

  return Sum * ((XRange * YRange) / (SampleCountX * SampleCountY));
}

double
random_sampler::sample_1D(const std::function<double(double)>& F,
                          double DomainMin,
                          double DomainMax,
                          size_t N) const
{
  std::mt19937 Rng(N);

  std::uniform_real_distribution<double> DomainDist(DomainMin, DomainMax);

  double Result = 0.0;

  auto DX = (DomainMax - DomainMin) / N;

  for (size_t I = 0; I < N; I++)
    Result += F(DomainDist(Rng)) * DX;

  return Result;
}

double
random_sampler::sample_2D(const std::function<double(double, double)>& F,
                          double DomainXMin,
                          double DomainXMax,
                          double DomainYMin,
                          double DomainYMax,
                          size_t N) const
{
  std::mt19937 Rng(N);

  using dist_type = std::uniform_real_distribution<double>;

  dist_type XDist(DomainXMin, DomainXMax);
  dist_type YDist(DomainYMin, DomainYMax);

  auto DX = (DomainXMax - DomainXMin);
  auto DY = (DomainYMax - DomainYMin);

  double Weight = (DX * DY) / N;

  double Result = 0.0;

  for (size_t I = 0; I < N; I++)
    Result += F(XDist(Rng), YDist(Rng));

  return Result * Weight;
}

double
grid_stratified_sampler::sample_1D(const std::function<double(double)>& F,
                                   double DomainMin,
                                   double DomainMax,
                                   size_t N) const
{
  std::mt19937 Rng(N);

  double Result = 0.0;

  auto DX = (DomainMax - DomainMin) / N;

  using dist_type = std::uniform_real_distribution<double>;

  for (size_t I = 0; I < N; I++) {

    dist_type DomainDist(DomainMin + (DX * I), DomainMin + (DX * (I + 1)));

    Result += F(DomainDist(Rng)) * DX;
  }

  return Result;
}

double
grid_stratified_sampler::sample_2D(
  const std::function<double(double, double)>& F,
  double DomainXMin,
  double DomainXMax,
  double DomainYMin,
  double DomainYMax,
  size_t N) const
{
  std::mt19937 Rng(N);

  using dist_type = std::uniform_real_distribution<double>;

  auto XRange = DomainXMax - DomainXMin;
  auto YRange = DomainYMax - DomainYMin;

  size_t SampleCountX = 0;
  size_t SampleCountY = 0;

  if (YRange > XRange) {
    SampleCountX = std::sqrt(double(N));
    SampleCountY = N / SampleCountX;
  } else {
    SampleCountY = std::sqrt(double(N));
    SampleCountX = N / SampleCountY;
  }

  double Sum = 0;

  for (size_t X = 0; X < SampleCountX; X++) {

    for (size_t Y = 0; Y < SampleCountY; Y++) {

      double UMin = double(X) / SampleCountX;
      double VMin = double(Y) / SampleCountY;

      double UMax = double(X + 1) / SampleCountX;
      double VMax = double(Y + 1) / SampleCountY;

      dist_type XDist(DomainXMin + (UMin * XRange),
                      DomainXMin + (UMax * XRange));

      dist_type YDist(DomainYMin + (VMin * YRange),
                      DomainYMin + (VMax * YRange));

      Sum += F(XDist(Rng), YDist(Rng));
    }
  }

  return Sum * ((XRange * YRange) / (SampleCountX * SampleCountY));
}

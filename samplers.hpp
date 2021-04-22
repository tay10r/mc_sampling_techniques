#pragma once

#include <functional>

class sampler
{
public:
  virtual ~sampler() = default;

  virtual double sample_1D(const std::function<double(double)>& F,
                           double DomainMin,
                           double DomainMax,
                           size_t N) const = 0;

  virtual double sample_2D(const std::function<double(double, double)>& F,
                           double DomainXMin,
                           double DomainXMax,
                           double DomainYMin,
                           double DomainYMax,
                           size_t N) const = 0;
};

class uniform_sampler final : public sampler
{
public:
  double sample_1D(const std::function<double(double)>& F,
                   double DomainMin,
                   double DomainMax,
                   size_t N) const override;

  double sample_2D(const std::function<double(double, double)>& F,
                   double DomainXMin,
                   double DomainXMax,
                   double DomainYMin,
                   double DomainYMax,
                   size_t N) const override;
};

class random_sampler final : public sampler
{
public:
  double sample_1D(const std::function<double(double)>& F,
                   double DomainMin,
                   double DomainMax,
                   size_t N) const override;

  double sample_2D(const std::function<double(double, double)>& F,
                   double DomainXMin,
                   double DomainXMax,
                   double DomainYMin,
                   double DomainYMax,
                   size_t N) const override;
};

class grid_stratified_sampler final : public sampler
{
public:
  double sample_1D(const std::function<double(double)>& F,
                   double DomainMin,
                   double DomainMax,
                   size_t N) const override;

  double sample_2D(const std::function<double(double, double)>& F,
                   double DomainXMin,
                   double DomainXMax,
                   double DomainYMin,
                   double DomainYMax,
                   size_t N) const override;
};

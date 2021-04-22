cmake_minimum_required(VERSION 3.9.6)

project(mc_sampling_techniques)

add_executable(main
  main.cpp
  samplers.hpp
  samplers.cpp)

target_compile_features(main PRIVATE cxx_std_17)

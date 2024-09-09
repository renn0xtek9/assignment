// Copyright 2024 <Maxime Haselbauer>
#ifndef SOFTWARE_IN_THE_LOOP_PARAMETERS_H
#define SOFTWARE_IN_THE_LOOP_PARAMETERS_H
#include <chrono>
#include <iostream>
#include <string>
#include <version.hpp>

struct SoftwareInTheLoopRunParameters {
  std::chrono::milliseconds duration_ms{1000};
};

inline void PrintHelp() {
  std::cout
      << "Software in the loop " << getVersion() << std::endl
      << "Usage: " << std::endl
      << "\t--duration_ms <duration_ms> : duration in milliseconds for which the simulation shall run (default: 1s)"
      << std::endl
      << "\t--help | -h : print help message" << std::endl;
}

inline SoftwareInTheLoopRunParameters ParseArguments(int argc, char* argv[]) {
  if (argc < 1) {
    return SoftwareInTheLoopRunParameters{};
  } else {
    SoftwareInTheLoopRunParameters run_parameters{};
    for (int index = 1; index < argc; index++) {
      const std::string argument{argv[index]};
      if (argument == "-h" || argument == "--help") {
        PrintHelp();
        exit(0);
      } else if (argument == "--duration_ms") {
        if (index + 1 < argc) {
          try {
            run_parameters.duration_ms = std::chrono::milliseconds{std::stoi(argv[index + 1])};
          } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid argument for --duration_ms: " << argv[index + 1] << std::endl;
            PrintHelp();
            exit(1);
          }
          index++;
          continue;
        } else {
          std::cerr << "Missing argument for --duration_ms" << std::endl;
          PrintHelp();
          exit(1);
        }
      } else {
        std::cerr << "Unknown argument: " << argument << std::endl;
        PrintHelp();
        exit(1);
      }
    }
    return run_parameters;
  }
}

#endif  // SOFTWARE_IN_THE_LOOP_PARAMETERS_H

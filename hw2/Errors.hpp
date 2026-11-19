#pragma once

#include <stdexcept>
#include <string>

class FleetError : public std::runtime_error {
public:
  explicit FleetError(const std::string& msg) : std::runtime_error(msg) {}
};

class InspectionError : public FleetError {
public:
  InspectionError() : FleetError("inspection rejected") {}
};

class CommandError : public FleetError {
public:
  explicit CommandError(const std::string& msg) : FleetError(msg) {}
};

#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "Errors.hpp"
#include "Fleet.hpp"

const int cNoviceScore = 6;

struct ModelSpec {
  bool electric;
  int noviceScore;
};

using Models = std::map<std::string, ModelSpec>;

std::string ReadWord(std::istringstream& in) {
  std::string s;
  if (!(in >> s)) {
    throw CommandError("missing argument");
  }
  return s;
}

void RequireEnd(std::istringstream& in) {
  std::string s;
  if (in >> s) {
    throw CommandError("extra argument");
  }
}

template<typename T>
T ReadNumber(std::istringstream& in) {
  std::istringstream number(ReadWord(in));
  T x;
  char c;
  if (!(number >> x) || (number >> c)) {
    throw CommandError("invalid number");
  }
  return x;
}

void AddItem(std::istringstream& in, Fleet& fleet, const Models& models) {
  std::string type = ReadWord(in);
  std::string id = ReadWord(in);
  std::string name = ReadWord(in);
  Models::const_iterator it = models.find(type);

  if (it != models.end()) {
    if (it->second.electric) {
      double energy = ReadNumber<double>(in);
      int score = ReadNumber<int>(in);
      RequireEnd(in);
      fleet.Add(std::make_unique<ElectricTransport>(id, name, energy, score,
                                                    it->second.noviceScore));
    } else {
      int score = ReadNumber<int>(in);
      RequireEnd(in);
      fleet.Add(std::make_unique<Transport>(id, name, score,
                                            it->second.noviceScore));
    }
  } else if (type == "helmet") {
    RequireEnd(in);
    fleet.Add(std::make_unique<Item>(id, name));
  } else if (type == "dock") {
    double energy = ReadNumber<double>(in);
    RequireEnd(in);
    fleet.Add(std::make_unique<Dock>(id, name, energy));
  } else if (type == "powerbank") {
    int charge = ReadNumber<int>(in);
    std::vector<std::string> ports;
    std::string port;
    while (in >> port) {
      ports.push_back(port);
    }
    fleet.Add(std::make_unique<Powerbank>(id, name, charge, ports));
  } else {
    throw CommandError("unknown item type");
  }
}

bool RunCommand(std::istringstream& in, Fleet& fleet, const Models& models) {
  std::string cmd;
  if (!(in >> cmd)) {
    return true;
  }

  if (cmd == "add") {
    AddItem(in, fleet, models);
    return true;
  }
  if (cmd == "focus" || cmd == "remove" || cmd == "rent" || cmd == "return") {
    std::string id = ReadWord(in);
    RequireEnd(in);
    if (cmd == "focus") {
      fleet.SetFocus(id);
    } else if (cmd == "remove") {
      fleet.Remove(id);
    } else {
      fleet.SetOccupied(id, cmd == "rent");
    }
    return true;
  }

  RequireEnd(in);
  if (cmd == "list") {
    fleet.List(std::cout);
  } else if (cmd == "novice") {
    fleet.Novice(std::cout);
  } else if (cmd == "energy") {
    fleet.Energy(std::cout);
  } else if (cmd == "report") {
    fleet.Report(std::cout);
  } else if (cmd == "occupied") {
    fleet.Occupied(std::cout);
  } else if (cmd == "show_focus") {
    fleet.ShowFocus(std::cout);
  } else if (cmd == "quit") {
    return false;
  } else {
    throw CommandError("unknown command");
  }
  return true;
}

int main() {
  const Models models{
    {"scooter", {true, cNoviceScore}},
    {"bike", {false, cNoviceScore}},
    {"ebike", {true, cNoviceScore}}
  };
  Fleet fleet;
  std::string line;

  while (std::getline(std::cin, line)) {
    try {
      std::istringstream in(line);
      if (!RunCommand(in, fleet, models)) {
        break;
      }
    } catch (const FleetError& error) {
      std::cout << "error: " << error.what() << "\n";
    }
  }
  return 0;
}

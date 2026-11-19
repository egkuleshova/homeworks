#include "Inventory.hpp"

#include "Errors.hpp"

const int cMinScore = 1;
const int cMaxScore = 10;
const int cMaxCharge = 100;

void InventoryItem::SetOccupied(bool) {
  throw CommandError("not transport");
}

std::ostream& operator<<(std::ostream& out, const InventoryItem& item) {
  return out << item.InventoryId() << ' ' << item.Name();
}

bool operator<(const InventoryItem& a, const InventoryItem& b) {
  return a.InventoryId() < b.InventoryId();
}

Item::Item(const std::string& id, const std::string& name)
    : id_(id), name_(name) {
  if (id.empty() || name.empty()) {
    throw CommandError("empty id or name");
  }
}

void Item::Describe(std::ostream& out) const {
  out << *this;
}

Transport::Transport(const std::string& id, const std::string& name, int score,
                     int noviceScore)
    : Item(id, name), score_(score), noviceScore_(noviceScore) {
  if (score < cMinScore || score > cMaxScore) {
    throw CommandError("score must be from 1 to 10");
  }
}

void Transport::Inspect() const {
  const char lower[] = "broken";
  const char upper[] = "BROKEN";
  int pos = 0;
  for (char c : Name()) {
    if (c == lower[pos] || c == upper[pos]) {
      ++pos;
      if (lower[pos] == '\0') {
        throw InspectionError();
      }
    } else if (c == lower[0] || c == upper[0]) {
      pos = 1;
    } else {
      pos = 0;
    }
  }
}

void Transport::SetOccupied(bool value) {
  if (occupied_ == value) {
    if (value) {
      throw CommandError("already rented");
    }
    throw CommandError("not rented");
  }
  occupied_ = value;
}

static void CheckEnergy(double energy) {
  if (!(energy >= 0)) {
    throw CommandError("energy must be nonnegative");
  }
}

ElectricTransport::ElectricTransport(const std::string& id,
                                     const std::string& name, double energy,
                                     int score, int noviceScore)
    : Transport(id, name, score, noviceScore), energy_(energy) {
  CheckEnergy(energy);
}

Dock::Dock(const std::string& id, const std::string& name, double energy)
    : Item(id, name), energy_(energy) {
  CheckEnergy(energy);
}

Powerbank::Powerbank(const std::string& id, const std::string& name, int charge,
                     const std::vector<std::string>& ports)
    : Item(id, name), charge_(charge), ports_(ports) {
  if (charge < 0 || charge > cMaxCharge) {
    throw CommandError("charge must be from 0 to 100");
  }
  if (ports.empty()) {
    throw CommandError("missing port type");
  }
}

void Powerbank::Describe(std::ostream& out) const {
  out << *this << ' ' << charge_ << '%';
  for (std::vector<std::string>::size_type i = 0; i < ports_.size(); ++i) {
    out << ' ' << ports_[i];
  }
}

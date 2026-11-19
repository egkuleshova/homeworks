#pragma once

#include <iostream>
#include <string>
#include <vector>

class InventoryItem {
public:
  virtual ~InventoryItem() = default;
  virtual const std::string& InventoryId() const = 0;
  virtual const std::string& Name() const = 0;
  virtual void Describe(std::ostream& out) const = 0;
  virtual void Inspect() const {}
  virtual double DailyEnergy() const { return 0; }
  virtual bool Novice() const { return false; }
  virtual bool Occupied() const { return false; }
  virtual void SetOccupied(bool value);
};

std::ostream& operator<<(std::ostream& out, const InventoryItem& item);
bool operator<(const InventoryItem& a, const InventoryItem& b);

class EnergyConsumer {
public:
  virtual ~EnergyConsumer() = default;
  virtual double DailyEnergy() const = 0;
};

class Item : public InventoryItem {
  std::string id_;
  std::string name_;

public:
  Item(const std::string& id, const std::string& name);
  const std::string& InventoryId() const override { return id_; }
  const std::string& Name() const override { return name_; }
  void Describe(std::ostream& out) const override;
};

class Transport : public Item {
  int score_;
  int noviceScore_;
  bool occupied_ = false;

public:
  Transport(const std::string& id, const std::string& name, int score,
            int noviceScore);
  void Inspect() const override;
  bool Novice() const override { return score_ >= noviceScore_; }
  bool Occupied() const override { return occupied_; }
  void SetOccupied(bool value) override;
};

class ElectricTransport : public Transport, public EnergyConsumer {
  double energy_;

public:
  ElectricTransport(const std::string& id, const std::string& name,
                    double energy, int score, int noviceScore);
  double DailyEnergy() const override { return energy_; }
};

class Dock : public Item, public EnergyConsumer {
  double energy_;

public:
  Dock(const std::string& id, const std::string& name, double energy);
  double DailyEnergy() const override { return energy_; }
};

class Powerbank : public Item {
  int charge_;
  std::vector<std::string> ports_;

public:
  Powerbank(const std::string& id, const std::string& name, int charge,
            const std::vector<std::string>& ports);
  void Describe(std::ostream& out) const override;
};

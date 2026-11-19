#pragma once

#include <memory>
#include <vector>

#include "Inventory.hpp"

class Fleet {
  using Positions = std::vector<std::shared_ptr<InventoryItem>>;

  Positions items_;
  std::weak_ptr<InventoryItem> focus_;
  bool hasFocus_ = false;

  Positions::iterator Find(const std::string& id);
  Positions::iterator RequirePosition(const std::string& id);

public:
  void Add(std::unique_ptr<InventoryItem> item);
  void List(std::ostream& out) const;
  void Novice(std::ostream& out) const;
  void Energy(std::ostream& out) const;
  void Report(std::ostream& out) const;
  void Occupied(std::ostream& out) const;
  void SetFocus(const std::string& id);
  void ShowFocus(std::ostream& out) const;
  void Remove(const std::string& id);
  void SetOccupied(const std::string& id, bool value);
};

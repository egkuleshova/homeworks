#include "Fleet.hpp"

#include <algorithm>
#include <iomanip>
#include <utility>

#include "Errors.hpp"

const int cPrecision = 1;

template<typename T>
class Total {
  T ans_ = 0;

public:
  Total& operator+=(T x) {
    ans_ += x;
    return *this;
  }

  void Print(std::ostream& out) const { out << ans_; }
};

template<>
void Total<double>::Print(std::ostream& out) const {
  out << std::fixed << std::setprecision(cPrecision) << ans_;
}

Fleet::Positions::iterator Fleet::Find(const std::string& id) {
  for (Positions::iterator it = items_.begin(); it != items_.end(); ++it) {
    if ((*it)->InventoryId() == id) {
      return it;
    }
  }
  return items_.end();
}

Fleet::Positions::iterator Fleet::RequirePosition(const std::string& id) {
  Positions::iterator it = Find(id);
  if (it == items_.end()) {
    throw CommandError("unknown id");
  }
  return it;
}

void Fleet::Add(std::unique_ptr<InventoryItem> item) {
  item->Inspect();
  if (Find(item->InventoryId()) != items_.end()) {
    throw CommandError("duplicate id");
  }
  items_.emplace_back(std::move(item));
}

static bool Compare(const InventoryItem* a, const InventoryItem* b) {
  return *a < *b;
}

void Fleet::List(std::ostream& out) const {
  std::vector<const InventoryItem*> a;
  a.reserve(items_.size());
  for (Positions::size_type i = 0; i < items_.size(); ++i) {
    a.push_back(items_[i].get());
  }
  std::sort(a.begin(), a.end(), Compare);
  for (const InventoryItem* item : a) {
    out << *item << "\n";
  }
}

void Fleet::Novice(std::ostream& out) const {
  for (Positions::size_type i = 0; i < items_.size(); ++i) {
    if (items_[i]->Novice()) {
      out << *items_[i] << "\n";
    }
  }
}

void Fleet::Energy(std::ostream& out) const {
  Total<double> ans;
  for (Positions::size_type i = 0; i < items_.size(); ++i) {
    ans += items_[i]->DailyEnergy();
  }
  ans.Print(out);
  out << "\n";
}

void Fleet::Report(std::ostream& out) const {
  out << items_.size() << ' ';
  Energy(out);
}

void Fleet::Occupied(std::ostream& out) const {
  Total<long long> cnt;
  for (Positions::size_type i = 0; i < items_.size(); ++i) {
    cnt += items_[i]->Occupied();
  }
  cnt.Print(out);
  out << "\n";
}

void Fleet::SetFocus(const std::string& id) {
  focus_ = *RequirePosition(id);
  hasFocus_ = true;
}

void Fleet::ShowFocus(std::ostream& out) const {
  std::shared_ptr<InventoryItem> item = focus_.lock();
  if (item) {
    item->Describe(out);
    out << "\n";
  } else if (hasFocus_) {
    out << "removed\n";
  } else {
    out << "expired\n";
  }
}

void Fleet::Remove(const std::string& id) {
  items_.erase(RequirePosition(id));
}

void Fleet::SetOccupied(const std::string& id, bool value) {
  (*RequirePosition(id))->SetOccupied(value);
}

# ФизКат

Консольный учёт транспорта и вещей на балансе.

## Сборка и запуск

Команды выполняются из папки `Fleet`.

```sh
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp Inventory.cpp Fleet.cpp -o fizkat
./fizkat
```

## Команды

Каждая команда вводится с новой строки. Инвентарный номер уникален. Имя, номер
и каждый тип разъёма записываются без пробелов.

| Команда | Действие |
| --- | --- |
| `add scooter id name energy score` | Добавить электросамокат. |
| `add bike id name score` | Добавить обычный велосипед. |
| `add ebike id name energy score` | Добавить электровелосипед. |
| `add helmet id name` | Добавить шлем. |
| `add dock id name energy` | Добавить док-станцию. |
| `add powerbank id name charge port [port ...]` | Добавить повербанк с целым зарядом 0–100% и хотя бы одним типом разъёма. |
| `list` | Вывести позиции в формате `id name` по возрастанию номера. |
| `novice` | Вывести транспорт с простотой от 6 до 10 в порядке добавления. |
| `energy` | Вывести суммарный расход в кВт·ч/сутки с одним знаком после точки. |
| `report` | Вывести число позиций и суммарный расход в одной строке. |
| `rent id` / `return id` | Отметить выдачу или возврат транспорта. |
| `occupied` | Вывести количество выданных единиц транспорта, которые числятся на балансе. |
| `focus id` / `show_focus` | Выбрать позицию и показать её. Для повербанка выводятся заряд и разъёмы. До первого выбора выводится `expired`, после удаления выбранной позиции — `removed`. |
| `remove id` | Удалить позицию с баланса. |
| `quit` | Завершить программу. |

Транспорт при добавлении свободен. Выданный транспорт учитывается в `occupied`
до возврата или удаления с баланса. Вклад повербанка в суточный расход равен нулю.

## Схема наследования

```text
InventoryItem (абстрактный)
└── Item (шлем и общая реализация номера и имени)
    ├── Transport (обычный велосипед)
    │   └── ElectricTransport (самокат и электровелосипед)
    ├── Dock
    └── Powerbank

EnergyConsumer (абстрактный)
├── ElectricTransport
└── Dock
```

`ElectricTransport` и `Dock` используют множественное наследование.
`InventoryItem` и `EnergyConsumer` образуют независимые ветви. [Inventory.hpp](Inventory.hpp)

## Указатели, ошибки и шаблоны

- Новый объект создаётся через `make_unique`. В `Fleet::Add` объект принадлежит
  `unique_ptr<InventoryItem>` во время техосмотра и проверки номера. Если проверка
  бросает исключение, указатель уничтожает объект при выходе из функции и парк
  остаётся в прежнем состоянии. После проверок `emplace_back(std::move(item))`
  создаёт `shared_ptr<InventoryItem>` в векторе парка и передаёт ему владение.
  [Fleet.cpp](Fleet.cpp) · [Fleet.hpp](Fleet.hpp) · [main.cpp](main.cpp)
- Витрина хранит `weak_ptr<InventoryItem>` на выбранную позицию. `show_focus`
  временно получает указатель через `lock()`. После `remove` витрина выдаёт
  `removed`, потому что последний владелец позиции исчезает.
  [Fleet.cpp](Fleet.cpp) · [Fleet.hpp](Fleet.hpp)
- `InspectionError` возникает, если в имени транспорта есть `broken` в любом
  регистре. `CommandError` сообщает об ошибках команды, неверных аргументах,
  повторном номере и невозможной выдаче или возврате. Обе ошибки наследуют
  `FleetError`, который ловится в `main`. [Errors.hpp](Errors.hpp) · [main.cpp](main.cpp)
- `operator<<` печатает номер и имя, а `operator<` сравнивает номера для
  сортировки. Шаблон `Total<T>` накапливает значения через `operator+=`
  в отчётах `energy` и `occupied`. У `double` есть специализация вывода
  с одним знаком после точки. [Inventory.cpp](Inventory.cpp) · [Fleet.cpp](Fleet.cpp)
- Креативная часть включает класс `Powerbank` и отчёт `occupied` с командами
  выдачи и возврата. Использовала ИИ при создании README, подключении команд в main.cpp, обсуждала с ним как лучше все разделить на файлы, проверяла работу кода на его примерах.

## Пример

**Ввод**

```text
show_focus
add powerbank P-1 Pocket 75 USB-C USB-A
add bike B-1 City 6
add ebike E-1 bBRoKeNModel 3.0 7
add dock D-1 Hub 10.0
focus P-1
show_focus
rent B-1
occupied
return B-1
occupied
remove P-1
show_focus
report
quit
```

**Вывод**

```text
expired
error: inspection rejected
P-1 Pocket 75% USB-C USB-A
1
0
removed
2 10.0
```

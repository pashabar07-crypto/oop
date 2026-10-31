#include <iostream>
#include <vector>
#include <list>
#include <array>
#include <string>
#include <iomanip>
#include <fstream>

using namespace std;

int visLen(const string& s) {
    int count = 0;
    for (int i = 0; i < (int)s.length(); i++) {
        if ((s[i] & 0xC0) != 0x80) {
            count++;
        }
    }
    return count;
}

string pad(const string& s, int width) {
    int len = visLen(s);
    if (len >= width) {
        return s;
    }
    return s + string(width - len, ' ');
}

string formatFloat(float val, int decimals) {
    int mult = 1;
    for (int i = 0; i < decimals; i++) {
        mult *= 10;
    }
    int scaled = (int)(val * mult + (val >= 0 ? 0.5f : -0.5f));
    int intPart = scaled / mult;
    int fracPart = scaled % mult;
    if (fracPart < 0) {
        fracPart = -fracPart;
    }
    string fracStr = to_string(fracPart);
    while ((int)fracStr.length() < decimals) {
        fracStr = "0" + fracStr;
    }
    return to_string(intPart) + "." + fracStr;
}

class Person {
protected:
    string fullName;
    int age;
public:
    Person() {
        fullName = "Неизвестно";
        age = 0;
    }
    Person(string fullName, int age) {
        this->fullName = fullName;
        this->age = age;
    }

    void setFullName(string fullName) {
        this->fullName = fullName;
    }
    string getFullName() const {
        return fullName;
    }
    void setAge(int age) {
        this->age = age;
    }
    int getAge() const {
        return age;
    }
};

class Passenger : public Person {
private:
    string passport;
    int tripId;
public:
    Passenger() : Person() {
        passport = "Нет данных";
        tripId = 0;
    }
    Passenger(string fullName, int age, string passport, int tripId = 0)
        : Person(fullName, age) {
        this->passport = passport;
        this->tripId = tripId;
    }

    void setPassport(string passport) {
        this->passport = passport;
    }
    string getPassport() const {
        return passport;
    }
    void setTripId(int id) {
        this->tripId = id;
    }
    int getTripId() const {
        return tripId;
    }

    friend ostream& operator<<(ostream& stream, const Passenger& obj);
    friend bool operator<(const Passenger& o1, const Passenger& o2);
    friend bool operator>(const Passenger& o1, const Passenger& o2);
    friend bool operator==(const Passenger& o1, const Passenger& o2);
    friend bool operator!=(const Passenger& o1, const Passenger& o2);
};

ostream& operator<<(ostream& stream, const Passenger& obj) {
    string ticketStr = (obj.tripId == 0) ? "Нет билета" : ("Рейс №" + to_string(obj.tripId));
    stream << "| " << pad(obj.fullName, 24)
        << " | " << pad(to_string(obj.age), 8)
        << " | " << pad(obj.passport, 14)
        << " | " << pad(ticketStr, 14) << " |";
    return stream;
}

bool operator<(const Passenger& o1, const Passenger& o2) {
    return o1.fullName < o2.fullName;
}
bool operator>(const Passenger& o1, const Passenger& o2) {
    return o1.fullName > o2.fullName;
}
bool operator==(const Passenger& o1, const Passenger& o2) {
    return o1.passport == o2.passport;
}
bool operator!=(const Passenger& o1, const Passenger& o2) {
    return o1.passport != o2.passport;
}

class Route {
private:
    string startPoint;
    string endPoint;
    float distance;
public:
    Route() {
        startPoint = "Не задано";
        endPoint = "Не задано";
        distance = 0.0f;
    }
    Route(string startPoint, string endPoint, float distance) {
        this->startPoint = startPoint;
        this->endPoint = endPoint;
        this->distance = distance;
    }

    void setStartPoint(string startPoint) {
        this->startPoint = startPoint;
    }
    string getStartPoint() const {
        return startPoint;
    }
    void setEndPoint(string endPoint) {
        this->endPoint = endPoint;
    }
    string getEndPoint() const {
        return endPoint;
    }
    void setDistance(float distance) {
        this->distance = distance;
    }
    float getDistance() const {
        return distance;
    }
};


class Bus {
private:
    string plateNumber;
    string model;
    int capacity;
    bool isActive;
public:
    Bus() {
        plateNumber = "---";
        model = "Свободно";
        capacity = 0;
        isActive = false;
    }
    Bus(string plateNumber, string model, int capacity, bool isActive = true) {
        this->plateNumber = plateNumber;
        this->model = model;
        this->capacity = capacity;
        this->isActive = isActive;
    }

    void setPlateNumber(string plateNumber) {
        this->plateNumber = plateNumber;
    }
    string getPlateNumber() const {
        return plateNumber;
    }
    void setModel(string model) {
        this->model = model;
    }
    string getModel() const {
        return model;
    }
    void setCapacity(int capacity) {
        this->capacity = capacity;
    }
    int getCapacity() const {
        return capacity;
    }
    void setIsActive(bool active) {
        this->isActive = active;
    }
    bool getIsActive() const {
        return isActive;
    }

    friend ostream& operator<<(ostream& stream, const Bus& obj);
    friend bool operator<(const Bus& o1, const Bus& o2);
    friend bool operator>(const Bus& o1, const Bus& o2);
    friend bool operator==(const Bus& o1, const Bus& o2);
    friend bool operator!=(const Bus& o1, const Bus& o2);
};

ostream& operator<<(ostream& stream, const Bus& obj) {
    string statusStr = obj.isActive ? "Занят" : "Свободен";
    stream << "| " << pad(obj.plateNumber, 14)
        << " | " << pad(obj.model, 20)
        << " | " << pad(to_string(obj.capacity), 12)
        << " | " << pad(statusStr, 14) << " |";
    return stream;
}

bool operator<(const Bus& o1, const Bus& o2) {
    return o1.capacity < o2.capacity;
}
bool operator>(const Bus& o1, const Bus& o2) {
    return o1.capacity > o2.capacity;
}
bool operator==(const Bus& o1, const Bus& o2) {
    return o1.plateNumber == o2.plateNumber;
}
bool operator!=(const Bus& o1, const Bus& o2) {
    return o1.plateNumber != o2.plateNumber;
}


class Trip {
private:
    int tripId;
    Route route;
    Bus* bus;
    float ticketPrice;
    int soldTickets;
public:
    Trip() {
        tripId = 0;
        bus = nullptr;
        ticketPrice = 0.0f;
        soldTickets = 0;
    }
    Trip(int tripId, Route route, Bus* bus, float ticketPrice, int soldTickets = 0) {
        this->tripId = tripId;
        this->route = route;
        this->bus = bus;
        this->ticketPrice = ticketPrice;
        this->soldTickets = soldTickets;
    }

    void setTripId(int id) {
        this->tripId = id;
    }
    int getTripId() const {
        return tripId;
    }
    void setRoute(Route r) {
        this->route = r;
    }
    Route getRoute() const {
        return route;
    }
    void setBus(Bus* b) {
        this->bus = b;
    }
    Bus* getBus() const {
        return bus;
    }
    void setTicketPrice(float price) {
        this->ticketPrice = price;
    }
    float getTicketPrice() const {
        return ticketPrice;
    }
    void setSoldTickets(int sold) {
        this->soldTickets = sold;
    }
    int getSoldTickets() const {
        return soldTickets;
    }

    int getMaxSeats() const {
        if (bus != nullptr && bus->getIsActive()) {
            return bus->getCapacity();
        }
        return 0;
    }

    bool hasFreeSeats() const {
        return soldTickets < getMaxSeats();
    }

    float getRevenue() const {
        return soldTickets * ticketPrice;
    }

    friend ostream& operator<<(ostream& stream, const Trip& obj);
    friend bool operator<(const Trip& o1, const Trip& o2);
    friend bool operator>(const Trip& o1, const Trip& o2);
    friend bool operator==(const Trip& o1, const Trip& o2);
    friend bool operator!=(const Trip& o1, const Trip& o2);
};

ostream& operator<<(ostream& stream, const Trip& obj) {
    string busInfo = (obj.bus != nullptr && obj.bus->getIsActive())
        ? obj.bus->getPlateNumber()
        : "Не назначен";
    string seatsInfo = to_string(obj.soldTickets) + "/" + to_string(obj.getMaxSeats());

    stream << "| " << pad(to_string(obj.tripId), 6)
        << " | " << pad(obj.route.getStartPoint(), 14)
        << " | " << pad(obj.route.getEndPoint(), 14)
        << " | " << pad(formatFloat(obj.route.getDistance(), 1), 10)
        << " | " << pad(busInfo, 14)
        << " | " << pad(seatsInfo, 10)
        << " | " << pad(formatFloat(obj.ticketPrice, 2), 11) << " |";
    return stream;
}

bool operator<(const Trip& o1, const Trip& o2) {
    return o1.ticketPrice < o2.ticketPrice;
}
bool operator>(const Trip& o1, const Trip& o2) {
    return o1.ticketPrice > o2.ticketPrice;
}
bool operator==(const Trip& o1, const Trip& o2) {
    return o1.tripId == o2.tripId;
}
bool operator!=(const Trip& o1, const Trip& o2) {
    return o1.tripId != o2.tripId;
}


const string PASS_BORDER = "+--------------------------+----------+----------------+----------------+";
const string BUS_BORDER = "+----------------+----------------------+--------------+----------------+";
const string TRIP_BORDER = "+--------+----------------+----------------+------------+----------------+------------+-------------+";
const string FIN_BORDER = "+--------+-----------------------------+------------+--------------+--------------+---------------+";

void printPassengerHeader(ostream& out) {
    out << PASS_BORDER << "\n";
    out << "| " << pad("ФИО пассажира", 24)
        << " | " << pad("Возраст", 8)
        << " | " << pad("Паспорт", 14)
        << " | " << pad("Статус билета", 14) << " |\n";
    out << PASS_BORDER << "\n";
}

void printBusHeader(ostream& out) {
    out << BUS_BORDER << "\n";
    out << "| " << pad("Госномер", 14)
        << " | " << pad("Модель автобуса", 20)
        << " | " << pad("Вместимость", 12)
        << " | " << pad("Статус бокса", 14) << " |\n";
    out << BUS_BORDER << "\n";
}

void printTripHeader(ostream& out) {
    out << TRIP_BORDER << "\n";
    out << "| " << pad("Рейс", 6)
        << " | " << pad("Откуда", 14)
        << " | " << pad("Куда", 14)
        << " | " << pad("Дист. (км)", 10)
        << " | " << pad("Автобус", 14)
        << " | " << pad("Билеты", 10)
        << " | " << pad("Цена (BYN)", 11) << " |\n";
    out << TRIP_BORDER << "\n";
}

void printFinanceHeader(ostream& out) {
    out << FIN_BORDER << "\n";
    out << "| " << pad("Рейс", 6)
        << " | " << pad("Маршрут перевозки", 27)
        << " | " << pad("Продано", 10)
        << " | " << pad("Свободно мест", 12)
        << " | " << pad("Цена (BYN)", 12)
        << " | " << pad("Выручка (BYN)", 13) << " |\n";
    out << FIN_BORDER << "\n";
}

void syncAllTripTickets(vector<Passenger>& passengers, list<Trip>& trips) {
    for (list<Trip>::iterator t = trips.begin(); t != trips.end(); t++) {
        int count = 0;
        for (int i = 0; i < (int)passengers.size(); i++) {
            if (passengers[i].getTripId() == t->getTripId()) {
                count++;
            }
        }
        t->setSoldTickets(count);
    }
}

void menuVector(vector<Passenger>& passengers, list<Trip>& trips) {
    int choice = -1;
    while (choice != 0) {
        cout << "\n--- УПРАВЛЕНИЕ ПАССАЖИРАМИ (Контейнер std::vector) ---\n";
        cout << "1. Просмотр всех пассажиров (на экран)\n";
        cout << "2. Вывод таблицы пассажиров в файл (passengers.txt)\n";
        cout << "3. Добавить пассажира (push_back)\n";
        cout << "4. Редактировать пассажира\n";
        cout << "5. Удалить пассажира (erase + iterator)\n";
        cout << "6. Поиск пассажира по номеру паспорта\n";
        cout << "7. Фильтр пассажиров по минимальному возрасту\n";
        cout << "8. Сортировка пассажиров по ФИО\n";
        cout << "0. Назад в главное меню\n";
        while (true) {
            cout << "Ваш выбор: ";
            if (cin >> choice && choice >= 0 && choice <= 8) {
                cin.ignore(10000, '\n');
                break;
            }
            cout << "Ошибка! Введите пункт меню от 0 до 8.\n";
            cin.clear();
            cin.ignore(10000, '\n');
        }

        switch (choice) {
        case 1: {
            if (passengers.empty()) {
                cout << "Список пассажиров пуст.\n";
            }
            else {
                printPassengerHeader(cout);
                vector<Passenger>::iterator p = passengers.begin();
                while (p != passengers.end()) {
                    cout << *p << endl;
                    p++;
                }
                cout << PASS_BORDER << "\n";
            }
            break;
        }
        case 2: {
            ofstream fout("passengers.txt");
            if (!fout.is_open()) {
                cout << "Ошибка открытия файла passengers.txt для записи!\n";
                break;
            }
            printPassengerHeader(fout);
            for (int i = 0; i < (int)passengers.size(); i++) {
                fout << passengers[i] << endl;
            }
            fout << PASS_BORDER << "\n";
            fout.close();
            cout << "Таблица пассажиров успешно сохранена в файл passengers.txt!\n";
            break;
        }
        case 3: {
            string name, pass;
            int age;

            while (true) {
                cout << "Введите ФИО: ";
                getline(cin, name);
                if (!name.empty()) break;
                cout << "ФИО не может быть пустым!\n";
            }
            while (true) {
                cout << "Введите возраст (1 - 120): ";
                if (cin >> age && age >= 1 && age <= 120) {
                    cin.ignore(10000, '\n');
                    break;
                }
                cout << "Ошибка! Возраст должен быть числом от 1 до 120.\n";
                cin.clear();
                cin.ignore(10000, '\n');
            }
            while (true) {
                cout << "Введите серию и номер паспорта: ";
                getline(cin, pass);
                if (!pass.empty()) break;
                cout << "Паспорт не может быть пустым!\n";
            }

            passengers.push_back(Passenger(name, age, pass, 0));
            cout << "Пассажир успешно добавлен! Текущий размер вектора = " << passengers.size() << endl;
            break;
        }
        case 4: {
            if (passengers.empty()) {
                cout << "Вектор пассажиров пуст!\n";
                break;
            }
            int idx;
            while (true) {
                cout << "Введите индекс пассажира для редактирования (0 - " << (int)passengers.size() - 1 << "): ";
                if (cin >> idx && idx >= 0 && idx < (int)passengers.size()) {
                    cin.ignore(10000, '\n');
                    break;
                }
                cout << "Ошибка! Неверный индекс.\n";
                cin.clear();
                cin.ignore(10000, '\n');
            }

            string name, pass;
            int age;
            int currentTripId = passengers[idx].getTripId();

            while (true) {
                cout << "Новое ФИО: ";
                getline(cin, name);
                if (!name.empty()) break;
                cout << "ФИО не может быть пустым!\n";
            }
            while (true) {
                cout << "Новый возраст (1 - 120): ";
                if (cin >> age && age >= 1 && age <= 120) {
                    cin.ignore(10000, '\n');
                    break;
                }
                cout << "Ошибка! Возраст должен быть числом от 1 до 120.\n";
                cin.clear();
                cin.ignore(10000, '\n');
            }
            while (true) {
                cout << "Новый паспорт: ";
                getline(cin, pass);
                if (!pass.empty()) break;
                cout << "Паспорт не может быть пустым!\n";
            }

            passengers[idx] = Passenger(name, age, pass, currentTripId);
            cout << "Данные пассажира успешно обновлены!\n";
            break;
        }
        case 5: {
            if (passengers.empty()) {
                cout << "Вектор пассажиров пуст!\n";
                break;
            }
            int idx;
            while (true) {
                cout << "Введите индекс пассажира для удаления (0 - " << (int)passengers.size() - 1 << "): ";
                if (cin >> idx && idx >= 0 && idx < (int)passengers.size()) {
                    cin.ignore(10000, '\n');
                    break;
                }
                cout << "Ошибка! Неверный индекс.\n";
                cin.clear();
                cin.ignore(10000, '\n');
            }

            vector<Passenger>::iterator p = passengers.begin();
            p += idx;
            passengers.erase(p);
            syncAllTripTickets(passengers, trips);
            cout << "Пассажир удален (если у него был куплен билет, место на рейсе освобождено).\n";
            cout << "Текущий размер вектора = " << passengers.size() << endl;
            break;
        }
        case 6: {
            string searchPass;
            while (true) {
                cout << "Введите номер паспорта для поиска: ";
                getline(cin, searchPass);
                if (!searchPass.empty()) break;
                cout << "Номер паспорта не может быть пустым!\n";
            }

            bool found = false;
            printPassengerHeader(cout);
            for (int i = 0; i < (int)passengers.size(); i++) {
                if (passengers[i].getPassport() == searchPass) {
                    cout << passengers[i] << endl;
                    found = true;
                }
            }
            cout << PASS_BORDER << "\n";
            if (!found) {
                cout << "Пассажир с указанным паспортом не найден.\n";
            }
            break;
        }
        case 7: {
            int minAge;
            while (true) {
                cout << "Показать пассажиров старше или возраста (лет): ";
                if (cin >> minAge && minAge >= 0) {
                    cin.ignore(10000, '\n');
                    break;
                }
                cout << "Ошибка! Введите неотрицательное число.\n";
                cin.clear();
                cin.ignore(10000, '\n');
            }

            printPassengerHeader(cout);
            for (int i = 0; i < (int)passengers.size(); i++) {
                if (passengers[i].getAge() >= minAge) {
                    cout << passengers[i] << endl;
                }
            }
            cout << PASS_BORDER << "\n";
            break;
        }
        case 8: {
            for (int i = 0; i < (int)passengers.size(); i++) {
                for (int j = 0; j + 1 < (int)passengers.size() - i; j++) {
                    if (passengers[j] > passengers[j + 1]) {
                        Passenger temp = passengers[j];
                        passengers[j] = passengers[j + 1];
                        passengers[j + 1] = temp;
                    }
                }
            }
            cout << "Вектор пассажиров успешно отсортирован по алфавиту (ФИО)!\n";
            break;
        }
        case 0:
            break;
        }
    }
}

void menuArray(array<Bus, 5>& park) {
    int choice = -1;
    while (choice != 0) {
        cout << "\n--- УПРАВЛЕНИЕ АВТОПАРКОМ (Контейнер std::array<Bus, 5>) ---\n";
        cout << "1. Просмотр всех боксов автопарка (на экран)\n";
        cout << "2. Вывод автопарка в файл (park.txt)\n";
        cout << "3. Добавить автобус в свободный бокс\n";
        cout << "4. Редактировать данные автобуса по номеру бокса (at)\n";
        cout << "5. Освободить бокс (удалить автобус)\n";
        cout << "6. Поиск автобуса по госномеру\n";
        cout << "7. Фильтр автобусов по минимальной вместимости\n";
        cout << "8. Сортировка автопарка по возрастанию вместимости\n";
        cout << "0. Назад в главное меню\n";
        while (true) {
            cout << "Ваш выбор: ";
            if (cin >> choice && choice >= 0 && choice <= 8) {
                cin.ignore(10000, '\n');
                break;
            }
            cout << "Ошибка! Введите пункт меню от 0 до 8.\n";
            cin.clear();
            cin.ignore(10000, '\n');
        }

        switch (choice) {
        case 1: {
            printBusHeader(cout);
            for (int i = 0; i < (int)park.size(); i++) {
                cout << park[i] << endl;
            }
            cout << BUS_BORDER << "\n";
            break;
        }
        case 2: {
            ofstream fout("park.txt");
            if (!fout.is_open()) {
                cout << "Ошибка открытия файла park.txt для записи!\n";
                break;
            }
            printBusHeader(fout);
            for (int i = 0; i < (int)park.size(); i++) {
                fout << park[i] << endl;
            }
            fout << BUS_BORDER << "\n";
            fout.close();
            cout << "Данные автопарка успешно сохранены в файл park.txt!\n";
            break;
        }
        case 3: {
            int freeIndex = -1;
            for (int i = 0; i < (int)park.size(); i++) {
                if (!park[i].getIsActive()) {
                    freeIndex = i;
                    break;
                }
            }
            if (freeIndex == -1) {
                cout << "Все 5 боксов автопарка заняты! Сначала освободите один из боксов.\n";
            }
            else {
                string plate, model;
                int cap;

                while (true) {
                    cout << "Введите госномер: ";
                    getline(cin, plate);
                    if (!plate.empty()) break;
                    cout << "Госномер не может быть пустым!\n";
                }
                while (true) {
                    cout << "Введите модель автобуса: ";
                    getline(cin, model);
                    if (!model.empty()) break;
                    cout << "Модель автобуса не может быть пустой!\n";
                }
                while (true) {
                    cout << "Введите вместимость мест (1 - 200): ";
                    if (cin >> cap && cap >= 1 && cap <= 200) {
                        cin.ignore(10000, '\n');
                        break;
                    }
                    cout << "Ошибка! Вместимость должна быть числом от 1 до 200.\n";
                    cin.clear();
                    cin.ignore(10000, '\n');
                }

                park[freeIndex] = Bus(plate, model, cap, true);
                cout << "Автобус успешно помещен в бокс №" << freeIndex << "!\n";
            }
            break;
        }
        case 4: {
            int idx;
            while (true) {
                cout << "Введите индекс бокса (0 - 4): ";
                if (cin >> idx && idx >= 0 && idx <= 4) {
                    cin.ignore(10000, '\n');
                    break;
                }
                cout << "Ошибка! Номер бокса должен быть от 0 до 4.\n";
                cin.clear();
                cin.ignore(10000, '\n');
            }

            string plate, model;
            int cap;

            while (true) {
                cout << "Новый госномер: ";
                getline(cin, plate);
                if (!plate.empty()) break;
                cout << "Госномер не может быть пустым!\n";
            }
            while (true) {
                cout << "Новая модель автобуса: ";
                getline(cin, model);
                if (!model.empty()) break;
                cout << "Модель автобуса не может быть пустой!\n";
            }
            while (true) {
                cout << "Новая вместимость (1 - 200): ";
                if (cin >> cap && cap >= 1 && cap <= 200) {
                    cin.ignore(10000, '\n');
                    break;
                }
                cout << "Ошибка! Вместимость должна быть числом от 1 до 200.\n";
                cin.clear();
                cin.ignore(10000, '\n');
            }

            park.at(idx) = Bus(plate, model, cap, true);
            cout << "Бокс №" << idx << " успешно обновлен!\n";
            break;
        }
        case 5: {
            int idx;
            while (true) {
                cout << "Введите индекс бокса для освобождения (0 - 4): ";
                if (cin >> idx && idx >= 0 && idx <= 4) {
                    cin.ignore(10000, '\n');
                    break;
                }
                cout << "Ошибка! Индекс должен быть от 0 до 4.\n";
                cin.clear();
                cin.ignore(10000, '\n');
            }

            park[idx] = Bus();
            cout << "Бокс №" << idx << " освобожден (автобус удален).\n";
            break;
        }
        case 6: {
            string plate;
            while (true) {
                cout << "Введите госномер автобуса для поиска: ";
                getline(cin, plate);
                if (!plate.empty()) break;
                cout << "Госномер не может быть пустым!\n";
            }

            printBusHeader(cout);
            for (auto bus : park) {
                if (bus.getIsActive() && bus.getPlateNumber() == plate) {
                    cout << bus << endl;
                }
            }
            cout << BUS_BORDER << "\n";
            break;
        }
        case 7: {
            int minCap;
            while (true) {
                cout << "Показать автобусы с вместимостью не менее (мест): ";
                if (cin >> minCap && minCap >= 0) {
                    cin.ignore(10000, '\n');
                    break;
                }
                cout << "Ошибка! Введите положительное число.\n";
                cin.clear();
                cin.ignore(10000, '\n');
            }

            printBusHeader(cout);
            for (auto bus : park) {
                if (bus.getIsActive() && bus.getCapacity() >= minCap) {
                    cout << bus << endl;
                }
            }
            cout << BUS_BORDER << "\n";
            break;
        }
        case 8: {
            for (int i = 0; i < (int)park.size(); i++) {
                for (int j = 0; j + 1 < (int)park.size() - i; j++) {
                    if (park[j] > park[j + 1]) {
                        Bus temp = park[j];
                        park[j] = park[j + 1];
                        park[j + 1] = temp;
                    }
                }
            }
            cout << "Автопарк успешно отсортирован по возрастанию вместимости!\n";
            break;
        }
        case 0:
            break;
        }
    }
}

void menuList(list<Trip>& trips, array<Bus, 5>& park, vector<Passenger>& passengers) {
    int choice = -1;
    while (choice != 0) {
        cout << "\n--- УПРАВЛЕНИЕ РЕЙСАМИ (Контейнер std::list) ---\n";
        cout << "1. Просмотр всех рейсов (на экран)\n";
        cout << "2. Вывод расписания рейсов в файл (trips.txt)\n";
        cout << "3. Добавить рейс в конец списка (push_back)\n";
        cout << "4. Редактировать рейс по номеру (ID)\n";
        cout << "5. Удалить рейс по номеру (erase)\n";
        cout << "6. Поиск рейса по пункту назначения\n";
        cout << "7. Фильтр рейсов по максимальной цене билета\n";
        cout << "8. Сортировка рейсов по цене билета (list::sort)\n";
        cout << "0. Назад в главное меню\n";
        while (true) {
            cout << "Ваш выбор: ";
            if (cin >> choice && choice >= 0 && choice <= 8) {
                cin.ignore(10000, '\n');
                break;
            }
            cout << "Ошибка! Введите пункт меню от 0 до 8.\n";
            cin.clear();
            cin.ignore(10000, '\n');
        }

        switch (choice) {
        case 1: {
            if (trips.empty()) {
                cout << "Список рейсов пуст.\n";
            }
            else {
                printTripHeader(cout);
                list<Trip>::iterator p = trips.begin();
                while (p != trips.end()) {
                    cout << *p << endl;
                    p++;
                }
                cout << TRIP_BORDER << "\n";
            }
            break;
        }
        case 2: {
            ofstream fout("trips.txt");
            if (!fout.is_open()) {
                cout << "Ошибка открытия файла trips.txt для записи!\n";
                break;
            }
            printTripHeader(fout);
            list<Trip>::iterator p = trips.begin();
            while (p != trips.end()) {
                fout << *p << endl;
                p++;
            }
            fout << TRIP_BORDER << "\n";
            fout.close();
            cout << "Расписание рейсов успешно сохранено в файл trips.txt!\n";
            break;
        }
        case 3: {
            int id, busIdx;
            string startP, endP;
            float dist, price;

            while (true) {
                cout << "Введите уникальный ID рейса (> 0): ";
                if (cin >> id && id > 0) {
                    cin.ignore(10000, '\n');
                    bool exists = false;
                    for (list<Trip>::iterator it = trips.begin(); it != trips.end(); it++) {
                        if (it->getTripId() == id) {
                            exists = true;
                            break;
                        }
                    }
                    if (!exists) {
                        break;
                    }
                    cout << "Ошибка! Рейс с таким ID уже существует в расписании.\n";
                }
                else {
                    cout << "Ошибка! ID должен быть положительным числом.\n";
                    cin.clear();
                    cin.ignore(10000, '\n');
                }
            }
            while (true) {
                cout << "Пункт отправления: ";
                getline(cin, startP);
                if (!startP.empty()) break;
                cout << "Пункт отправления не может быть пустым!\n";
            }
            while (true) {
                cout << "Пункт назначения: ";
                getline(cin, endP);
                if (!endP.empty()) break;
                cout << "Пункт назначения не может быть пустым!\n";
            }
            while (true) {
                cout << "Расстояние (км): ";
                if (cin >> dist && dist > 0.0f) {
                    cin.ignore(10000, '\n');
                    break;
                }
                cout << "Ошибка! Расстояние должно быть больше 0.\n";
                cin.clear();
                cin.ignore(10000, '\n');
            }
            while (true) {
                cout << "Цена билета (BYN): ";
                if (cin >> price && price >= 0.0f) {
                    cin.ignore(10000, '\n');
                    break;
                }
                cout << "Ошибка! Цена не может быть отрицательной.\n";
                cin.clear();
                cin.ignore(10000, '\n');
            }
            while (true) {
                cout << "Выберите индекс бокса автопарка (0 - 4): ";
                if (cin >> busIdx && busIdx >= 0 && busIdx <= 4) {
                    cin.ignore(10000, '\n');
                    break;
                }
                cout << "Ошибка! Индекс бокса должен быть от 0 до 4.\n";
                cin.clear();
                cin.ignore(10000, '\n');
            }

            Bus* selectedBus = nullptr;
            if (park[busIdx].getIsActive()) {
                selectedBus = &park[busIdx];
            }
            else {
                cout << "Выбранный бокс пуст, автобус на рейс пока не назначен.\n";
            }

            trips.push_back(Trip(id, Route(startP, endP, dist), selectedBus, price, 0));
            cout << "Рейс успешно добавлен! Размер списка рейсов = " << trips.size() << endl;
            break;
        }
        case 4: {
            if (trips.empty()) {
                cout << "Список рейсов пуст!\n";
                break;
            }
            int id;
            while (true) {
                cout << "Введите ID рейса для редактирования: ";
                if (cin >> id && id > 0) {
                    cin.ignore(10000, '\n');
                    break;
                }
                cout << "Ошибка! Введите положительный ID.\n";
                cin.clear();
                cin.ignore(10000, '\n');
            }

            list<Trip>::iterator p = trips.begin();
            bool found = false;
            while (p != trips.end()) {
                if (p->getTripId() == id) {
                    string startP, endP;
                    float dist, price;

                    while (true) {
                        cout << "Новый пункт отправления: ";
                        getline(cin, startP);
                        if (!startP.empty()) break;
                        cout << "Пункт не может быть пустым!\n";
                    }
                    while (true) {
                        cout << "Новый пункт назначения: ";
                        getline(cin, endP);
                        if (!endP.empty()) break;
                        cout << "Пункт не может быть пустым!\n";
                    }
                    while (true) {
                        cout << "Новое расстояние (км): ";
                        if (cin >> dist && dist > 0.0f) {
                            cin.ignore(10000, '\n');
                            break;
                        }
                        cout << "Ошибка! Расстояние должно быть больше 0.\n";
                        cin.clear();
                        cin.ignore(10000, '\n');
                    }
                    while (true) {
                        cout << "Новая цена билета (BYN): ";
                        if (cin >> price && price >= 0.0f) {
                            cin.ignore(10000, '\n');
                            break;
                        }
                        cout << "Ошибка! Цена не может быть отрицательной.\n";
                        cin.clear();
                        cin.ignore(10000, '\n');
                    }

                    p->setRoute(Route(startP, endP, dist));
                    p->setTicketPrice(price);
                    cout << "Данные рейса успешно обновлены!\n";
                    found = true;
                    break;
                }
                p++;
            }
            if (!found) {
                cout << "Рейс с указанным ID не найден.\n";
            }
            break;
        }
        case 5: {
            if (trips.empty()) {
                cout << "Список рейсов пуст!\n";
                break;
            }
            int id;
            while (true) {
                cout << "Введите ID рейса для удаления: ";
                if (cin >> id && id > 0) {
                    cin.ignore(10000, '\n');
                    break;
                }
                cout << "Ошибка! Введите положительный ID.\n";
                cin.clear();
                cin.ignore(10000, '\n');
            }

            list<Trip>::iterator p = trips.begin();
            bool deleted = false;
            while (p != trips.end()) {
                if (p->getTripId() == id) {
                    for (int i = 0; i < (int)passengers.size(); i++) {
                        if (passengers[i].getTripId() == id) {
                            passengers[i].setTripId(0);
                        }
                    }
                    trips.erase(p);
                    cout << "Рейс удален из расписания! Билеты пассажиров этого рейса аннулированы.\n";
                    deleted = true;
                    break;
                }
                p++;
            }
            if (!deleted) {
                cout << "Рейс с указанным ID не найден.\n";
            }
            break;
        }
        case 6: {
            string target;
            while (true) {
                cout << "Введите пункт назначения: ";
                getline(cin, target);
                if (!target.empty()) break;
                cout << "Пункт назначения не может быть пустым!\n";
            }

            printTripHeader(cout);
            for (list<Trip>::iterator p = trips.begin(); p != trips.end(); p++) {
                if (p->getRoute().getEndPoint() == target) {
                    cout << *p << endl;
                }
            }
            cout << TRIP_BORDER << "\n";
            break;
        }
        case 7: {
            float maxPrice;
            while (true) {
                cout << "Введите максимальную цену билета: ";
                if (cin >> maxPrice && maxPrice >= 0.0f) {
                    cin.ignore(10000, '\n');
                    break;
                }
                cout << "Ошибка! Введите неотрицательное число.\n";
                cin.clear();
                cin.ignore(10000, '\n');
            }

            printTripHeader(cout);
            for (list<Trip>::iterator p = trips.begin(); p != trips.end(); p++) {
                if (p->getTicketPrice() <= maxPrice) {
                    cout << *p << endl;
                }
            }
            cout << TRIP_BORDER << "\n";
            break;
        }
        case 8: {
            trips.sort();
            cout << "Список рейсов отсортирован по возрастанию цены билета (list::sort)!\n";
            break;
        }
        case 0:
            break;
        }
    }
}


void menuTicketsAndReports(vector<Passenger>& passengers, list<Trip>& trips) {
    int choice = -1;
    while (choice != 0) {
        cout << "\n--- КАССА БИЛЕТОВ, ЗАВЕРШЕНИЕ РЕЙСОВ И ОТЧЕТНОСТЬ ---\n";
        cout << "1. Оформить покупку билета пассажиру на рейс\n";
        cout << "2. Оформить возврат (отказ от) билета\n";
        cout << "3. Подтвердить завершение рейса (отчет в файл + возобновление мест)\n";
        cout << "4. Показать посадочную ведомость конкретного рейса\n";
        cout << "5. Финансовая ведомость по рейсам (выручка и заполняемость)\n";
        cout << "0. Назад в главное меню\n";
        while (true) {
            cout << "Ваш выбор: ";
            if (cin >> choice && choice >= 0 && choice <= 5) {
                cin.ignore(10000, '\n');
                break;
            }
            cout << "Ошибка! Введите пункт меню от 0 до 5.\n";
            cin.clear();
            cin.ignore(10000, '\n');
        }

        switch (choice) {
        case 1: {
            if (passengers.empty() || trips.empty()) {
                cout << "Операция невозможна: отсутствует база пассажиров или расписание рейсов.\n";
                break;
            }

            cout << "\nСписок пассажиров:\n";
            printPassengerHeader(cout);
            for (int i = 0; i < (int)passengers.size(); i++) {
                cout << passengers[i] << endl;
            }
            cout << PASS_BORDER << "\n";

            string passNum;
            while (true) {
                cout << "Введите паспорт пассажира для покупки билета: ";
                getline(cin, passNum);
                if (!passNum.empty()) break;
                cout << "Паспорт не может быть пустым!\n";
            }

            int passIdx = -1;
            for (int i = 0; i < (int)passengers.size(); i++) {
                if (passengers[i].getPassport() == passNum) {
                    passIdx = i;
                    break;
                }
            }

            if (passIdx == -1) {
                cout << "Пассажир с указанным паспортом не найден!\n";
                break;
            }
            if (passengers[passIdx].getTripId() != 0) {
                cout << "У пассажира уже имеется билет на Рейс №" << passengers[passIdx].getTripId()
                    << ". Сначала оформите возврат!\n";
                break;
            }

            cout << "\nДоступные рейсы в расписании:\n";
            printTripHeader(cout);
            for (list<Trip>::iterator it = trips.begin(); it != trips.end(); it++) {
                cout << *it << endl;
            }
            cout << TRIP_BORDER << "\n";

            int targetTripId;
            while (true) {
                cout << "Введите номер рейса (ID) для посадки: ";
                if (cin >> targetTripId && targetTripId > 0) {
                    cin.ignore(10000, '\n');
                    break;
                }
                cout << "Ошибка! Введите корректный положительный ID рейса.\n";
                cin.clear();
                cin.ignore(10000, '\n');
            }

            bool tripFound = false;
            for (list<Trip>::iterator it = trips.begin(); it != trips.end(); it++) {
                if (it->getTripId() == targetTripId) {
                    tripFound = true;
                    if (it->getMaxSeats() == 0) {
                        cout << "Отказ: на данный рейс еще не назначен активный автобус!\n";
                    }
                    else if (!it->hasFreeSeats()) {
                        cout << "Отказ: на рейсе №" << targetTripId << " отсутствуют свободные места!\n";
                    }
                    else {
                        passengers[passIdx].setTripId(targetTripId);
                        it->setSoldTickets(it->getSoldTickets() + 1);
                        cout << "Билет успешно оформлен! Пассажир: " << passengers[passIdx].getFullName()
                            << " назначен на Рейс №" << targetTripId
                            << " (" << it->getRoute().getStartPoint() << " -> " << it->getRoute().getEndPoint() << ").\n";
                    }
                    break;
                }
            }
            if (!tripFound) {
                cout << "Рейс с указанным ID не найден в расписании!\n";
            }
            break;
        }
        case 2: {
            if (passengers.empty()) {
                cout << "Список пассажиров пуст.\n";
                break;
            }
            string passNum;
            while (true) {
                cout << "Введите паспорт пассажира для оформления возврата: ";
                getline(cin, passNum);
                if (!passNum.empty()) break;
                cout << "Паспорт не может быть пустым!\n";
            }

            int passIdx = -1;
            for (int i = 0; i < (int)passengers.size(); i++) {
                if (passengers[i].getPassport() == passNum) {
                    passIdx = i;
                    break;
                }
            }

            if (passIdx == -1) {
                cout << "Пассажир с указанным паспортом не найден!\n";
                break;
            }

            int oldTripId = passengers[passIdx].getTripId();
            if (oldTripId == 0) {
                cout << "У данного пассажира нет активных билетов!\n";
                break;
            }

            passengers[passIdx].setTripId(0);
            syncAllTripTickets(passengers, trips);
            cout << "Возврат билета на Рейс №" << oldTripId << " для пассажира "
                << passengers[passIdx].getFullName() << " успешно оформлен! Место освобождено.\n";
            break;
        }
        case 3: {
            syncAllTripTickets(passengers, trips);
            cout << "\nДействующие рейсы:\n";
            printTripHeader(cout);
            for (list<Trip>::iterator it = trips.begin(); it != trips.end(); it++) {
                cout << *it << endl;
            }
            cout << TRIP_BORDER << "\n";

            int tId;
            while (true) {
                cout << "Введите ID завершенного рейса: ";
                if (cin >> tId && tId > 0) {
                    cin.ignore(10000, '\n');
                    break;
                }
                cout << "Ошибка! Введите положительное число.\n";
                cin.clear();
                cin.ignore(10000, '\n');
            }

            bool found = false;
            for (list<Trip>::iterator it = trips.begin(); it != trips.end(); it++) {
                if (it->getTripId() == tId) {
                    found = true;
                    ofstream fout("completed_trips_report.txt", ios::app);
                    if (!fout.is_open()) {
                        cout << "Ошибка открытия архивного файла completed_trips_report.txt!\n";
                        break;
                    }

                    string routeName = it->getRoute().getStartPoint() + " -> " + it->getRoute().getEndPoint();
                    string busPlate = (it->getBus() && it->getBus()->getIsActive())
                        ? it->getBus()->getPlateNumber()
                        : "Не назначен";

                    cout << "\n====================================================================\n";
                    cout << "             ОТЧЕТ О ЗАВЕРШЕНИИ РЕЙСА №" << tId << "\n";
                    cout << "====================================================================\n";
                    fout << "====================================================================\n";
                    fout << "             ОТЧЕТ О ЗАВЕРШЕНИИ РЕЙСА №" << tId << "\n";
                    fout << "====================================================================\n";

                    string tripSummary = "Маршрут следования : " + routeName + "\n"
                        + "Дистанция пути     : " + formatFloat(it->getRoute().getDistance(), 1) + " км\n"
                        + "Автобус рейса      : " + busPlate + "\n"
                        + "Перевезено человек : " + to_string(it->getSoldTickets()) + "\n"
                        + "Полученная выручка : " + formatFloat(it->getRevenue(), 2) + " BYN\n";

                    cout << tripSummary;
                    fout << tripSummary;

                    cout << "\nПосадочный список пассажиров совершенного рейса:\n";
                    fout << "\nПосадочный список пассажиров совершенного рейса:\n";

                    printPassengerHeader(cout);
                    printPassengerHeader(fout);

                    for (int i = 0; i < (int)passengers.size(); i++) {
                        if (passengers[i].getTripId() == tId) {
                            cout << passengers[i] << endl;
                            fout << passengers[i] << endl;
                            passengers[i].setTripId(0);
                        }
                    }

                    cout << PASS_BORDER << "\n";
                    fout << PASS_BORDER << "\n\n";
                    fout.close();

                    it->setSoldTickets(0);

                    cout << "\n[Успешно]: Рейс №" << tId << " завершен. Отчет занесен в completed_trips_report.txt!\n";
                    break;
                }
            }
            if (!found) {
                cout << "Рейс с указанным ID не найден!\n";
            }
            break;
        }
        case 4: {
            int targetTripId;
            while (true) {
                cout << "Введите номер рейса (ID) для просмотра посадочной ведомости: ";
                if (cin >> targetTripId && targetTripId > 0) {
                    cin.ignore(10000, '\n');
                    break;
                }
                cout << "Ошибка! Введите положительное число.\n";
                cin.clear();
                cin.ignore(10000, '\n');
            }

            cout << "\nПосадочная ведомость пассажиров рейса №" << targetTripId << ":\n";
            printPassengerHeader(cout);
            int count = 0;
            for (int i = 0; i < (int)passengers.size(); i++) {
                if (passengers[i].getTripId() == targetTripId) {
                    cout << passengers[i] << endl;
                    count++;
                }
            }
            cout << PASS_BORDER << "\n";
            cout << "Всего пассажиров с купленными билетами: " << count << endl;
            break;
        }
        case 5: {
            syncAllTripTickets(passengers, trips);
            float totalCompanyRevenue = 0.0f;
            int totalSoldTickets = 0;

            cout << "\nФИНАНСОВАЯ ВЕДОМОСТЬ ПРЕДПРИЯТИЯ ПО ВСЕМ РЕЙСАМ:\n";
            printFinanceHeader(cout);
            for (list<Trip>::iterator it = trips.begin(); it != trips.end(); it++) {
                string routeName = it->getRoute().getStartPoint() + " -> " + it->getRoute().getEndPoint();
                int sold = it->getSoldTickets();
                int freeSeats = it->getMaxSeats() - sold;
                if (freeSeats < 0) freeSeats = 0;
                float rev = it->getRevenue();

                totalCompanyRevenue += rev;
                totalSoldTickets += sold;

                cout << "| " << pad(to_string(it->getTripId()), 6)
                    << " | " << pad(routeName, 27)
                    << " | " << pad(to_string(sold), 10)
                    << " | " << pad(to_string(freeSeats), 12)
                    << " | " << pad(formatFloat(it->getTicketPrice(), 2), 12)
                    << " | " << pad(formatFloat(rev, 2), 13) << " |\n";
            }
            cout << FIN_BORDER << "\n";
            cout << "Всего продано билетов: " << totalSoldTickets
                << " | Суммарная выручка предприятия: " << formatFloat(totalCompanyRevenue, 2) << " BYN\n";
            break;
        }
        case 0:
            break;
        }
    }
}


void lab3(){
    system("cls");

    array<Bus, 5> park{
        Bus("1245 AB-7", "MAZ-251", 44, true),
        Bus("8890 KE-7", "Mercedes Tourismo", 50, true),
        Bus("3311 OP-7", "MAN Lion's Coach", 48, true),
        Bus(),
        Bus()
    };

    vector<Passenger> passengers;
    passengers.push_back(Passenger("Иванов И.И.", 25, "MP1234567", 101));
    passengers.push_back(Passenger("Петров П.П.", 42, "MC7654321", 101));
    passengers.push_back(Passenger("Сидорова А.В.", 19, "HB1122334", 102));
    passengers.push_back(Passenger("Козлов Д.С.", 31, "MP9988776", 0));

    list<Trip> trips;
    trips.push_back(Trip(101, Route("Минск", "Брест", 348.0f), &park[0], 35.50f, 2));
    trips.push_back(Trip(102, Route("Минск", "Гродно", 278.0f), &park[1], 28.00f, 1));
    trips.push_back(Trip(103, Route("Минск", "Гомель", 312.0f), &park[2], 32.00f, 0));

    syncAllTripTickets(passengers, trips);

    int mainChoice = -1;
    while (mainChoice != 0) {
        cout << "\n==========================================================\n";
        cout << "   СИСТЕМА УЧЕТА ДАННЫХ ПО ПАССАЖИРСКИМ ПЕРЕВОЗКАМ\n";
        cout << "==========================================================\n";
        cout << "1. Работа с пассажирами (контейнер std::vector)\n";
        cout << "2. Работа с автопарком (контейнер std::array)\n";
        cout << "3. Работа с рейсами (контейнер std::list)\n";
        cout << "4. Касса, завершение рейсов и ведомости\n";
        cout << "5. Сводный табличный отчет всех данных (Экран + Файл)\n";
        cout << "0. Выход из программы\n";
        while (true) {
            cout << "Выберите пункт меню: ";
            if (cin >> mainChoice && mainChoice >= 0 && mainChoice <= 5) {
                cin.ignore(10000, '\n');
                break;
            }
            cout << "Ошибка! Введите пункт от 0 до 5.\n";
            cin.clear();
            cin.ignore(10000, '\n');
        }

        switch (mainChoice) {
        case 1:
            menuVector(passengers, trips);
            break;
        case 2:
            menuArray(park);
            break;
        case 3:
            menuList(trips, park, passengers);
            break;
        case 4:
            menuTicketsAndReports(passengers, trips);
            break;
        case 5: {
            syncAllTripTickets(passengers, trips);
            ofstream fout("full_report.txt");

            cout << "\n>>> 1. ТАБЛИЦА ПАССАЖИРОВ (vector) <<<\n";
            fout << ">>> 1. ТАБЛИЦА ПАССАЖИРОВ (vector) <<<\n";
            printPassengerHeader(cout);
            printPassengerHeader(fout);
            for (int i = 0; i < (int)passengers.size(); i++) {
                cout << passengers[i] << endl;
                fout << passengers[i] << endl;
            }
            cout << PASS_BORDER << "\n";
            fout << PASS_BORDER << "\n\n";

            cout << "\n>>> 2. ТАБЛИЦА АВТОПАРКА (array) <<<\n";
            fout << ">>> 2. ТАБЛИЦА АВТОПАРКА (array) <<<\n";
            printBusHeader(cout);
            printBusHeader(fout);
            for (int i = 0; i < (int)park.size(); i++) {
                cout << park[i] << endl;
                fout << park[i] << endl;
            }
            cout << BUS_BORDER << "\n";
            fout << BUS_BORDER << "\n\n";

            cout << "\n>>> 3. ТАБЛИЦА РЕЙСОВ (list) <<<\n";
            fout << ">>> 3. ТАБЛИЦА РЕЙСОВ (list) <<<\n";
            printTripHeader(cout);
            printTripHeader(fout);
            for (list<Trip>::iterator p = trips.begin(); p != trips.end(); p++) {
                cout << *p << endl;
                fout << *p << endl;
            }
            cout << TRIP_BORDER << "\n";
            fout << TRIP_BORDER << "\n\n";

            float totalRev = 0.0f;
            int totalSold = 0;
            cout << "\n>>> 4. ФИНАНСОВАЯ ВЕДОМОСТЬ ПРЕДПРИЯТИЯ <<<\n";
            fout << ">>> 4. ФИНАНСОВАЯ ВЕДОМОСТЬ ПРЕДПРИЯТИЯ <<<\n";
            printFinanceHeader(cout);
            printFinanceHeader(fout);
            for (list<Trip>::iterator it = trips.begin(); it != trips.end(); it++) {
                string routeName = it->getRoute().getStartPoint() + " -> " + it->getRoute().getEndPoint();
                int sold = it->getSoldTickets();
                int freeSeats = it->getMaxSeats() - sold;
                if (freeSeats < 0) freeSeats = 0;
                float rev = it->getRevenue();
                totalRev += rev;
                totalSold += sold;

                string row = "| " + pad(to_string(it->getTripId()), 6)
                    + " | " + pad(routeName, 27)
                    + " | " + pad(to_string(sold), 10)
                    + " | " + pad(to_string(freeSeats), 12)
                    + " | " + pad(formatFloat(it->getTicketPrice(), 2), 12)
                    + " | " + pad(formatFloat(rev, 2), 13) + " |";
                cout << row << endl;
                fout << row << endl;
            }
            cout << FIN_BORDER << "\n";
            fout << FIN_BORDER << "\n";

            string summary = "Всего продано билетов: " + to_string(totalSold)
                + " | Суммарная выручка: " + formatFloat(totalRev, 2) + " BYN\n";
            cout << summary;
            fout << summary;

            fout.close();
            cout << "\nПолный сводный отчет сохранен в файл full_report.txt!\n";
            break;
        }
        case 0:
            break;
        }
    }
}
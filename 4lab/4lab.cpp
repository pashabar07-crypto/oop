#include <iostream>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <iomanip>
#include <fstream>

using namespace std;

int getLength1(const string& s) {
    int count = 0;
    for (int i = 0; i < (int)s.length(); i++) {
        if ((s[i] & 0xC0) != 0x80) {
            count++;
        }
    }
    return count;
}

string setPadding1(const string& s, int width) {
    int len = getLength1(s);
    if (len >= width) {
        return s;
    }
    return s + string(width - len, ' ');
}

string formatNum1(float val, int decimals) {
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

class Person1 {
protected:
    string fullName;
    int age;
public:
    Person1() {
        fullName = "Неизвестно";
        age = 0;
    }
    Person1(string fullName, int age) {
        this->fullName = fullName;
        this->age = age;
    }
    void setFullName1(string fullName) { this->fullName = fullName; }
    string getFullName1() const { return fullName; }
    void setAge1(int age) { this->age = age; }
    int getAge1() const { return age; }
};

class Passenger1 : public Person1 {
private:
    string passport;
    int tripId;
public:
    Passenger1() : Person1() {
        passport = "Нет данных";
        tripId = 0;
    }
    Passenger1(string fullName, int age, string passport, int tripId = 0)
        : Person1(fullName, age) {
        this->passport = passport;
        this->tripId = tripId;
    }

    void setPassport1(string passport) { this->passport = passport; }
    string getPassport1() const { return passport; }
    void setTripId1(int id) { this->tripId = id; }
    int getTripId1() const { return tripId; }

    friend ostream& operator<<(ostream& stream, const Passenger1& obj) {
        string ticketStr = (obj.tripId == 0) ? "Нет билета" : ("Рейс №" + to_string(obj.tripId));
        stream << "| " << setPadding1(obj.fullName, 24)
            << " | " << setPadding1(to_string(obj.age), 8)
            << " | " << setPadding1(obj.passport, 14)
            << " | " << setPadding1(ticketStr, 14) << " |";
        return stream;
    }
    friend bool operator<(const Passenger1& a, const Passenger1& b) {
        return a.fullName < b.fullName;
    }
    friend bool operator==(const Passenger1& a, const Passenger1& b) {
        return a.passport == b.passport;
    }
};

class Station1 {
private:
    string cityName;
    string region;
public:
    Station1() {
        cityName = "Не задано";
        region = "Не задано";
    }
    Station1(string cityName, string region) {
        this->cityName = cityName;
        this->region = region;
    }
    string getCityName1() const { return cityName; }
    string getRegion1() const { return region; }
    void setRegion1(string r) { region = r; }

    friend ostream& operator<<(ostream& stream, const Station1& obj) {
        stream << "| " << setPadding1(obj.cityName, 20)
            << " | " << setPadding1(obj.region, 26) << " |";
        return stream;
    }
    friend bool operator<(const Station1& a, const Station1& b) {
        return a.cityName < b.cityName;
    }
    friend bool operator==(const Station1& a, const Station1& b) {
        return a.cityName == b.cityName;
    }
};

struct StationHash1 {
    size_t operator()(const Station1& s) const {
        return hash<string>()(s.getCityName1());
    }
};

class Bus1 {
private:
    string plateNumber;
    string model;
    int capacity;
public:
    Bus1() {
        plateNumber = "---";
        model = "Неизвестно";
        capacity = 0;
    }
    Bus1(string plateNumber, string model, int capacity) {
        this->plateNumber = plateNumber;
        this->model = model;
        this->capacity = capacity;
    }
    string getPlateNumber1() const { return plateNumber; }
    string getModel1() const { return model; }
    int getCapacity1() const { return capacity; }

    friend ostream& operator<<(ostream& stream, const Bus1& obj) {
        stream << "| " << setPadding1(obj.plateNumber, 16)
            << " | " << setPadding1(obj.model, 22)
            << " | " << setPadding1(to_string(obj.capacity), 14) << " |";
        return stream;
    }
    friend bool operator<(const Bus1& a, const Bus1& b) {
        if (a.capacity != b.capacity) {
            return a.capacity < b.capacity;
        }
        return a.plateNumber < b.plateNumber;
    }
    friend bool operator==(const Bus1& a, const Bus1& b) {
        return a.plateNumber == b.plateNumber;
    }
};

class Route1 {
private:
    const Station1* startStation;
    const Station1* endStation;
    float distance;
public:
    Route1() {
        startStation = nullptr;
        endStation = nullptr;
        distance = 0.0f;
    }
    Route1(const Station1* startStation, const Station1* endStation, float distance) {
        this->startStation = startStation;
        this->endStation = endStation;
        this->distance = distance;
    }
    string getStartPoint1() const {
        return (startStation != nullptr) ? startStation->getCityName1() : "Удалена";
    }
    string getEndPoint1() const {
        return (endStation != nullptr) ? endStation->getCityName1() : "Удалена";
    }
    const Station1* getStartStationPtr1() const { return startStation; }
    const Station1* getEndStationPtr1() const { return endStation; }
    void resetStation1(const string& city) {
        if (startStation != nullptr && startStation->getCityName1() == city) startStation = nullptr;
        if (endStation != nullptr && endStation->getCityName1() == city) endStation = nullptr;
    }
    float getDistance1() const { return distance; }
};

class Trip1 {
private:
    int tripId;
    Route1 route;
    const Bus1* bus;
    float ticketPrice;
    int soldTickets;
public:
    Trip1() {
        tripId = 0;
        bus = nullptr;
        ticketPrice = 0.0f;
        soldTickets = 0;
    }
    Trip1(int tripId, Route1 route, const Bus1* bus, float ticketPrice, int soldTickets = 0) {
        this->tripId = tripId;
        this->route = route;
        this->bus = bus;
        this->ticketPrice = ticketPrice;
        this->soldTickets = soldTickets;
    }

    int getTripId1() const { return tripId; }
    void setRoute1(Route1 r) { route = r; }
    Route1& getRouteRef1() { return route; }
    Route1 getRoute1() const { return route; }
    void setBus1(const Bus1* b) { bus = b; }
    const Bus1* getBus1() const { return bus; }
    string getBusPlate1() const {
        return (bus != nullptr) ? bus->getPlateNumber1() : "Не назначен";
    }
    int getMaxSeats1() const {
        return (bus != nullptr) ? bus->getCapacity1() : 0;
    }
    void setTicketPrice1(float price) { ticketPrice = price; }
    float getTicketPrice1() const { return ticketPrice; }
    void setSoldTickets1(int sold) { soldTickets = sold; }
    int getSoldTickets1() const { return soldTickets; }

    bool hasSeats1() const { return soldTickets < getMaxSeats1(); }
    float getRevenue1() const { return soldTickets * ticketPrice; }

    friend ostream& operator<<(ostream& stream, const Trip1& obj) {
        string seatsInfo = to_string(obj.soldTickets) + "/" + to_string(obj.getMaxSeats1());
        stream << "| " << setPadding1(to_string(obj.tripId), 6)
            << " | " << setPadding1(obj.route.getStartPoint1(), 14)
            << " | " << setPadding1(obj.route.getEndPoint1(), 14)
            << " | " << setPadding1(formatNum1(obj.route.getDistance1(), 1), 10)
            << " | " << setPadding1(obj.getBusPlate1(), 14)
            << " | " << setPadding1(seatsInfo, 10)
            << " | " << setPadding1(formatNum1(obj.ticketPrice, 2), 11) << " |";
        return stream;
    }
    friend bool operator<(const Trip1& a, const Trip1& b) {
        return a.ticketPrice < b.ticketPrice;
    }
};

class TicketRecord1 {
private:
    const Trip1* trip;
    const Passenger1* passenger;
    float priceSnapshot;
public:
    TicketRecord1() {
        trip = nullptr;
        passenger = nullptr;
        priceSnapshot = 0.0f;
    }
    TicketRecord1(const Trip1* trip, const Passenger1* passenger, float priceSnapshot) {
        this->trip = trip;
        this->passenger = passenger;
        this->priceSnapshot = priceSnapshot;
    }
    int getTripId1() const {
        return (trip != nullptr) ? trip->getTripId1() : 0;
    }
    string getPassengerPassport1() const {
        return (passenger != nullptr) ? passenger->getPassport1() : "Нет данных";
    }
    float getPrice1() const {
        return (trip != nullptr) ? trip->getTicketPrice1() : priceSnapshot;
    }

    friend ostream& operator<<(ostream& stream, const TicketRecord1& obj) {
        string pName = (obj.passenger != nullptr) ? obj.passenger->getFullName1() : "Неизвестно";
        stream << "| " << setPadding1(to_string(obj.getTripId1()), 8)
            << " | " << setPadding1(obj.getPassengerPassport1(), 14)
            << " | " << setPadding1(pName, 22)
            << " | " << setPadding1(formatNum1(obj.getPrice1(), 2), 12) << " |";
        return stream;
    }
    friend bool operator<(const TicketRecord1& a, const TicketRecord1& b) {
        return a.getPrice1() < b.getPrice1();
    }
};

class CompletedTripLog1 {
private:
    int tripId;
    string routeDesc;
    int passengersCount;
    float totalRevenue;
public:
    CompletedTripLog1() {
        tripId = 0;
        routeDesc = "Нет данных";
        passengersCount = 0;
        totalRevenue = 0.0f;
    }
    CompletedTripLog1(int tripId, string routeDesc, int passengersCount, float totalRevenue) {
        this->tripId = tripId;
        this->routeDesc = routeDesc;
        this->passengersCount = passengersCount;
        this->totalRevenue = totalRevenue;
    }
    int getTripId1() const { return tripId; }
    string getRouteDesc1() const { return routeDesc; }
    int getPassengersCount1() const { return passengersCount; }
    float getTotalRevenue1() const { return totalRevenue; }

    friend ostream& operator<<(ostream& stream, const CompletedTripLog1& obj) {
        stream << "| " << setPadding1(to_string(obj.tripId), 8)
            << " | " << setPadding1(obj.routeDesc, 26)
            << " | " << setPadding1(to_string(obj.passengersCount), 12)
            << " | " << setPadding1(formatNum1(obj.totalRevenue, 2), 14) << " |";
        return stream;
    }
    friend bool operator<(const CompletedTripLog1& a, const CompletedTripLog1& b) {
        return a.totalRevenue < b.totalRevenue;
    }
    friend bool operator==(const CompletedTripLog1& a, const CompletedTripLog1& b) {
        return a.tripId == b.tripId && a.routeDesc == b.routeDesc && a.passengersCount == b.passengersCount;
    }
};

struct ArchiveHash1 {
    size_t operator()(const CompletedTripLog1& log) const {
        return hash<int>()(log.getTripId1()) ^ hash<string>()(log.getRouteDesc1());
    }
};

const string PASS_BORDER = "+--------------------------+----------+----------------+----------------+";
const string BUS_BORDER = "+------------------+------------------------+----------------+";
const string TRIP_BORDER = "+--------+----------------+----------------+------------+----------------+------------+-------------+";
const string TICKET_BORDER = "+----------+----------------+------------------------+--------------+";
const string STATION_BORDER = "+----------------------+----------------------------+";
const string LOG_BORDER = "+----------+----------------------------+--------------+----------------+";

void printPassHeader1(ostream& out) {
    out << PASS_BORDER << "\n| " << setPadding1("ФИО пассажира", 24) << " | " << setPadding1("Возраст", 8)
        << " | " << setPadding1("Паспорт", 14) << " | " << setPadding1("Статус билета", 14) << " |\n" << PASS_BORDER << "\n";
}
void printBusHeader1(ostream& out) {
    out << BUS_BORDER << "\n| " << setPadding1("Госномер", 16) << " | " << setPadding1("Модель автобуса", 22)
        << " | " << setPadding1("Вместимость", 14) << " |\n" << BUS_BORDER << "\n";
}
void printTripHeader1(ostream& out) {
    out << TRIP_BORDER << "\n| " << setPadding1("ID", 6) << " | " << setPadding1("Откуда", 14) << " | " << setPadding1("Куда", 14)
        << " | " << setPadding1("Дист.", 10) << " | " << setPadding1("Автобус", 14) << " | " << setPadding1("Места", 10)
        << " | " << setPadding1("Цена", 11) << " |\n" << TRIP_BORDER << "\n";
}
void printTicketHeader1(ostream& out) {
    out << TICKET_BORDER << "\n| " << setPadding1("ID Рейса", 8) << " | " << setPadding1("Паспорт", 14)
        << " | " << setPadding1("ФИО пассажира", 22) << " | " << setPadding1("Цена", 12) << " |\n" << TICKET_BORDER << "\n";
}
void printStationHeader1(ostream& out) {
    out << STATION_BORDER << "\n| " << setPadding1("Город", 20) << " | " << setPadding1("Область / Регион", 26)
        << " |\n" << STATION_BORDER << "\n";
}
void printLogHeader1(ostream& out) {
    out << LOG_BORDER << "\n| " << setPadding1("ID Рейса", 8) << " | " << setPadding1("Маршрут", 26)
        << " | " << setPadding1("Пассажиров", 12) << " | " << setPadding1("Выручка", 14) << " |\n" << LOG_BORDER << "\n";
}

const Station1* findStation1(unordered_set<Station1, StationHash1>& stationsUSet, const string& cityName) {
    unordered_set<Station1, StationHash1>::iterator it = stationsUSet.find(Station1(cityName, ""));
    if (it != stationsUSet.end()) {
        return &(*it);
    }
    pair<unordered_set<Station1, StationHash1>::iterator, bool> res = stationsUSet.insert(Station1(cityName, "Региональный узел"));
    return &(*res.first);
}

const Bus1* findBus1(const set<Bus1>& busSet, const string& plate) {
    for (set<Bus1>::const_iterator it = busSet.begin(); it != busSet.end(); ++it) {
        if (it->getPlateNumber1() == plate) {
            return &(*it);
        }
    }
    return nullptr;
}

void updateData1(
    map<int, Trip1>& tripsMap,
    unordered_map<string, Passenger1>& passUMap,
    multimap<string, Trip1>& destMMap,
    unordered_multimap<int, Passenger1>& boardingUMMap,
    multiset<TicketRecord1>& ticketsMSet
) {
    destMMap.clear();
    boardingUMMap.clear();
    ticketsMSet.clear();

    for (map<int, Trip1>::iterator tIt = tripsMap.begin(); tIt != tripsMap.end(); ++tIt) {
        int count = 0;
        int currentTripId = tIt->first;
        float price = tIt->second.getTicketPrice1();

        for (unordered_map<string, Passenger1>::iterator pIt = passUMap.begin(); pIt != passUMap.end(); ++pIt) {
            if (pIt->second.getTripId1() == currentTripId) {
                count++;
                boardingUMMap.insert(make_pair(currentTripId, pIt->second));
                ticketsMSet.insert(TicketRecord1(&(tIt->second), &(pIt->second), price));
            }
        }
        tIt->second.setSoldTickets1(count);
        destMMap.insert(make_pair(tIt->second.getRoute1().getEndPoint1(), tIt->second));
    }
}

void menuTrips1(
    map<int, Trip1>& tripsMap,
    set<Bus1>& busSet,
    unordered_set<Station1, StationHash1>& stationsUSet,
    unordered_map<string, Passenger1>& passUMap,
    multimap<string, Trip1>& destMMap,
    unordered_multimap<int, Passenger1>& boardingUMMap,
    multiset<TicketRecord1>& ticketsMSet
) {
    int ch = -1;
    while (ch != 0) {
        cout << "\n--- 1. УПРАВЛЕНИЕ РЕЙСАМИ ---\n";
        cout << "1. Просмотр всех рейсов\n";
        cout << "2. Запись таблицы рейсов в файл\n";
        cout << "3. Добавить новый рейс\n";
        cout << "4. Update рейса по ID\n";
        cout << "5. Удалить рейс по ID\n";
        cout << "6. Search рейса по ID\n";
        cout << "7. Фильтр и сортировка рейсов по цене\n";
        cout << "0. Назад в главное меню\n";
        while (true) {
            cout << "Ваш выбор: ";
            if (cin >> ch && ch >= 0 && ch <= 7) { cin.ignore(10000, '\n'); break; }
            cout << "Ошибка! Введите 0 - 7.\n"; cin.clear(); cin.ignore(10000, '\n');
        }

        switch (ch) {
        case 1: {
            if (tripsMap.empty()) cout << "Список рейсов пуст.\n";
            else {
                printTripHeader1(cout);
                for (map<int, Trip1>::iterator it = tripsMap.begin(); it != tripsMap.end(); ++it) {
                    cout << it->second << endl;
                }
                cout << TRIP_BORDER << "\n";
            }
            break;
        }
        case 2: {
            ofstream fout("map_trips.txt");
            printTripHeader1(fout);
            for (map<int, Trip1>::iterator it = tripsMap.begin(); it != tripsMap.end(); ++it) {
                fout << it->second << endl;
            }
            fout << TRIP_BORDER << "\n";
            fout.close();
            cout << "Сохранено в файл map_trips.txt!\n";
            break;
        }
        case 3: {
            int id; string sP, eP, bPlate; float dist, price;
            while (true) {
                cout << "Введите уникальный ID рейса: ";
                if (cin >> id && id > 0 && tripsMap.find(id) == tripsMap.end()) {
                    cin.ignore(10000, '\n'); break;
                }
                cout << "Ошибка или такой ID уже существует!\n";
                cin.clear(); cin.ignore(10000, '\n');
            }
            while (true) { cout << "Пункт отправления: "; getline(cin, sP); if (!sP.empty()) break; }
            while (true) { cout << "Пункт назначения: "; getline(cin, eP); if (!eP.empty()) break; }
            while (true) {
                cout << "Дистанция: ";
                if (cin >> dist && dist > 0.0f) { cin.ignore(10000, '\n'); break; }
                cout << "Ошибка!\n"; cin.clear(); cin.ignore(10000, '\n');
            }
            while (true) {
                cout << "Цена билета: ";
                if (cin >> price && price >= 0.0f) { cin.ignore(10000, '\n'); break; }
                cout << "Ошибка!\n"; cin.clear(); cin.ignore(10000, '\n');
            }

            cout << "Доступные автобусы:\n";
            printBusHeader1(cout);
            for (set<Bus1>::iterator bIt = busSet.begin(); bIt != busSet.end(); ++bIt) cout << *bIt << endl;
            cout << BUS_BORDER << "\n";

            while (true) { cout << "Введите госномер автобуса: "; getline(cin, bPlate); if (!bPlate.empty()) break; }
            const Bus1* selectedBus = findBus1(busSet, bPlate);
            if (selectedBus == nullptr) {
                cout << "Автобус не найден, рейс создан без привязки к автобусу.\n";
            }

            const Station1* stStart = findStation1(stationsUSet, sP);
            const Station1* stEnd = findStation1(stationsUSet, eP);

            tripsMap.insert(pair<int, Trip1>(id, Trip1(id, Route1(stStart, stEnd, dist), selectedBus, price, 0)));
            updateData1(tripsMap, passUMap, destMMap, boardingUMMap, ticketsMSet);
            cout << "Рейс успешно добавлен!\n";
            break;
        }
        case 4: {
            int id;
            while (true) {
                cout << "Введите ID рейса для update: ";
                if (cin >> id && id > 0) { cin.ignore(10000, '\n'); break; }
                cout << "Ошибка!\n"; cin.clear(); cin.ignore(10000, '\n');
            }
            map<int, Trip1>::iterator mapPtr = tripsMap.find(id);
            if (mapPtr != tripsMap.end()) {
                string sP, eP; float dist, price;
                while (true) { cout << "Новый пункт отправления: "; getline(cin, sP); if (!sP.empty()) break; }
                while (true) { cout << "Новый пункт назначения: "; getline(cin, eP); if (!eP.empty()) break; }
                while (true) {
                    cout << "Новая дистанция: ";
                    if (cin >> dist && dist > 0.0f) { cin.ignore(10000, '\n'); break; }
                    cout << "Ошибка!\n"; cin.clear(); cin.ignore(10000, '\n');
                }
                while (true) {
                    cout << "Новая цена билета: ";
                    if (cin >> price && price >= 0.0f) { cin.ignore(10000, '\n'); break; }
                    cout << "Ошибка!\n"; cin.clear(); cin.ignore(10000, '\n');
                }
                const Station1* stStart = findStation1(stationsUSet, sP);
                const Station1* stEnd = findStation1(stationsUSet, eP);
                mapPtr->second.setRoute1(Route1(stStart, stEnd, dist));
                mapPtr->second.setTicketPrice1(price);
                updateData1(tripsMap, passUMap, destMMap, boardingUMMap, ticketsMSet);
                cout << "Рейс успешно обновлен!\n";
            }
            else {
                cout << "Рейс с таким ID не найден.\n";
            }
            break;
        }
        case 5: {
            int id;
            while (true) {
                cout << "Введите ID рейса для удаления: ";
                if (cin >> id && id > 0) { cin.ignore(10000, '\n'); break; }
                cout << "Ошибка!\n"; cin.clear(); cin.ignore(10000, '\n');
            }
            if (tripsMap.erase(id) > 0) {
                for (unordered_map<string, Passenger1>::iterator pIt = passUMap.begin(); pIt != passUMap.end(); ++pIt) {
                    if (pIt->second.getTripId1() == id) pIt->second.setTripId1(0);
                }
                updateData1(tripsMap, passUMap, destMMap, boardingUMMap, ticketsMSet);
                cout << "Рейс удален, билеты пассажиров аннулированы!\n";
            }
            else {
                cout << "ID не найден!\n";
            }
            break;
        }
        case 6: {
            int id;
            while (true) {
                cout << "Введите ID рейса для search: ";
                if (cin >> id && id > 0) { cin.ignore(10000, '\n'); break; }
                cout << "Ошибка!\n"; cin.clear(); cin.ignore(10000, '\n');
            }
            map<int, Trip1>::iterator mapPtr = tripsMap.find(id);
            if (mapPtr != tripsMap.end()) {
                printTripHeader1(cout);
                cout << mapPtr->second << endl;
                cout << TRIP_BORDER << "\n";
            }
            else {
                cout << "Рейс с ID " << id << " не найден.\n";
            }
            break;
        }
        case 7: {
            float maxP;
            while (true) {
                cout << "Введите максимальную цену для фильтрации: ";
                if (cin >> maxP && maxP >= 0.0f) { cin.ignore(10000, '\n'); break; }
                cout << "Ошибка!\n"; cin.clear(); cin.ignore(10000, '\n');
            }
            multimap<float, Trip1> sortedByPrice;
            for (map<int, Trip1>::iterator it = tripsMap.begin(); it != tripsMap.end(); ++it) {
                if (it->second.getTicketPrice1() <= maxP) {
                    sortedByPrice.insert(make_pair(it->second.getTicketPrice1(), it->second));
                }
            }
            cout << "Отфильтрованные рейсы по возрастанию цены:\n";
            printTripHeader1(cout);
            for (multimap<float, Trip1>::iterator it = sortedByPrice.begin(); it != sortedByPrice.end(); ++it) {
                cout << it->second << endl;
            }
            cout << TRIP_BORDER << "\n";
            break;
        }
        }
    }
}

void menuPassengers1(
    unordered_map<string, Passenger1>& passUMap,
    map<int, Trip1>& tripsMap,
    multimap<string, Trip1>& destMMap,
    unordered_multimap<int, Passenger1>& boardingUMMap,
    multiset<TicketRecord1>& ticketsMSet
) {
    int ch = -1;
    while (ch != 0) {
        cout << "\n--- 2. УПРАВЛЕНИЕ ПАССАЖИРАМИ ---\n";
        cout << "1. Просмотр всех пассажиров\n";
        cout << "2. Вывод таблицы пассажиров в файл\n";
        cout << "3. Добавить пассажира\n";
        cout << "4. Update пассажира по паспорту\n";
        cout << "5. Удалить пассажира по паспорту\n";
        cout << "6. Search пассажира по паспорту\n";
        cout << "7. Фильтр по минимальному возрасту\n";
        cout << "8. Сортировка пассажиров по ФИО\n";
        cout << "0. Назад в главное меню\n";
        while (true) {
            cout << "Ваш выбор: ";
            if (cin >> ch && ch >= 0 && ch <= 8) { cin.ignore(10000, '\n'); break; }
            cout << "Ошибка! Введите 0 - 8.\n"; cin.clear(); cin.ignore(10000, '\n');
        }

        switch (ch) {
        case 1: {
            printPassHeader1(cout);
            for (unordered_map<string, Passenger1>::iterator it = passUMap.begin(); it != passUMap.end(); ++it) {
                cout << it->second << endl;
            }
            cout << PASS_BORDER << "\n";
            break;
        }
        case 2: {
            ofstream fout("unordered_map_passengers.txt");
            printPassHeader1(fout);
            for (unordered_map<string, Passenger1>::iterator it = passUMap.begin(); it != passUMap.end(); ++it) {
                fout << it->second << endl;
            }
            fout << PASS_BORDER << "\n";
            fout.close();
            cout << "Сохранено в unordered_map_passengers.txt!\n";
            break;
        }
        case 3: {
            string pass, name; int age;
            while (true) {
                cout << "Введите серию и номер паспорта: ";
                getline(cin, pass);
                if (!pass.empty() && passUMap.find(pass) == passUMap.end()) break;
                cout << "Ошибка или паспорт уже есть в базе!\n";
            }
            while (true) { cout << "Введите ФИО: "; getline(cin, name); if (!name.empty()) break; }
            while (true) {
                cout << "Введите возраст: ";
                if (cin >> age && age >= 1 && age <= 120) { cin.ignore(10000, '\n'); break; }
                cout << "Ошибка!\n"; cin.clear(); cin.ignore(10000, '\n');
            }
            passUMap.insert(make_pair(pass, Passenger1(name, age, pass, 0)));
            cout << "Пассажир добавлен!\n";
            break;
        }
        case 4: {
            string pass;
            while (true) { cout << "Введите паспорт для update: "; getline(cin, pass); if (!pass.empty()) break; }
            unordered_map<string, Passenger1>::iterator it = passUMap.find(pass);
            if (it != passUMap.end()) {
                string name; int age;
                while (true) { cout << "Новое ФИО: "; getline(cin, name); if (!name.empty()) break; }
                while (true) {
                    cout << "Новый возраст: ";
                    if (cin >> age && age >= 1 && age <= 120) { cin.ignore(10000, '\n'); break; }
                    cout << "Ошибка!\n"; cin.clear(); cin.ignore(10000, '\n');
                }
                it->second.setFullName1(name);
                it->second.setAge1(age);
                updateData1(tripsMap, passUMap, destMMap, boardingUMMap, ticketsMSet);
                cout << "Данные пассажира обновлены!\n";
            }
            else {
                cout << "Пассажир с таким паспортом не найден.\n";
            }
            break;
        }
        case 5: {
            string pass;
            while (true) { cout << "Введите паспорт для удаления: "; getline(cin, pass); if (!pass.empty()) break; }
            if (passUMap.erase(pass) > 0) {
                updateData1(tripsMap, passUMap, destMMap, boardingUMMap, ticketsMSet);
                cout << "Пассажир удален!\n";
            }
            else {
                cout << "Пассажир не найден.\n";
            }
            break;
        }
        case 6: {
            string pass;
            while (true) { cout << "Введите паспорт для search: "; getline(cin, pass); if (!pass.empty()) break; }
            unordered_map<string, Passenger1>::iterator it = passUMap.find(pass);
            if (it != passUMap.end()) {
                printPassHeader1(cout);
                cout << it->second << endl;
                cout << PASS_BORDER << "\n";
            }
            else {
                cout << "Пассажир не найден.\n";
            }
            break;
        }
        case 7: {
            int minAge;
            while (true) {
                cout << "Минимальный возраст: ";
                if (cin >> minAge && minAge >= 0) { cin.ignore(10000, '\n'); break; }
                cout << "Ошибка!\n"; cin.clear(); cin.ignore(10000, '\n');
            }
            printPassHeader1(cout);
            for (unordered_map<string, Passenger1>::iterator it = passUMap.begin(); it != passUMap.end(); ++it) {
                if (it->second.getAge1() >= minAge) cout << it->second << endl;
            }
            cout << PASS_BORDER << "\n";
            break;
        }
        case 8: {
            multiset<Passenger1> sortedPass;
            for (unordered_map<string, Passenger1>::iterator it = passUMap.begin(); it != passUMap.end(); ++it) {
                sortedPass.insert(it->second);
            }
            printPassHeader1(cout);
            for (multiset<Passenger1>::iterator it = sortedPass.begin(); it != sortedPass.end(); ++it) {
                cout << *it << endl;
            }
            cout << PASS_BORDER << "\n";
            break;
        }
        }
    }
}

void menuFleet1(
    set<Bus1>& busSet,
    map<int, Trip1>& tripsMap,
    unordered_map<string, Passenger1>& passUMap,
    multimap<string, Trip1>& destMMap,
    unordered_multimap<int, Passenger1>& boardingUMMap,
    multiset<TicketRecord1>& ticketsMSet
) {
    int ch = -1;
    while (ch != 0) {
        cout << "\n--- 3. УПРАВЛЕНИЕ АВТОПАРКОМ ---\n";
        cout << "1. Просмотр автопарка по возрастанию вместимости\n";
        cout << "2. Запись автопарка в файл\n";
        cout << "3. Добавить автобус\n";
        cout << "4. Update автобуса\n";
        cout << "5. Удалить автобус по госномеру\n";
        cout << "6. Search автобуса по госномеру\n";
        cout << "7. Фильтр по минимальной вместимости\n";
        cout << "0. Назад в главное меню\n";
        while (true) {
            cout << "Ваш выбор: ";
            if (cin >> ch && ch >= 0 && ch <= 7) { cin.ignore(10000, '\n'); break; }
            cout << "Ошибка! Введите 0 - 7.\n"; cin.clear(); cin.ignore(10000, '\n');
        }

        switch (ch) {
        case 1: {
            printBusHeader1(cout);
            for (set<Bus1>::iterator it = busSet.begin(); it != busSet.end(); ++it) cout << *it << endl;
            cout << BUS_BORDER << "\n";
            break;
        }
        case 2: {
            ofstream fout("set_buses.txt");
            printBusHeader1(fout);
            for (set<Bus1>::iterator it = busSet.begin(); it != busSet.end(); ++it) fout << *it << endl;
            fout << BUS_BORDER << "\n";
            fout.close();
            cout << "Сохранено в set_buses.txt!\n";
            break;
        }
        case 3: {
            string pl, md; int cap;
            while (true) { cout << "Госномер: "; getline(cin, pl); if (!pl.empty()) break; }
            while (true) { cout << "Модель автобуса: "; getline(cin, md); if (!md.empty()) break; }
            while (true) {
                cout << "Вместимость: ";
                if (cin >> cap && cap >= 10 && cap <= 150) { cin.ignore(10000, '\n'); break; }
                cout << "Ошибка!\n"; cin.clear(); cin.ignore(10000, '\n');
            }
            busSet.insert(Bus1(pl, md, cap));
            cout << "Автобус добавлен!\n";
            break;
        }
        case 4: {
            string pl;
            while (true) { cout << "Введите госномер автобуса для update: "; getline(cin, pl); if (!pl.empty()) break; }
            set<Bus1>::iterator target = busSet.end();
            for (set<Bus1>::iterator it = busSet.begin(); it != busSet.end(); ++it) {
                if (it->getPlateNumber1() == pl) { target = it; break; }
            }
            if (target != busSet.end()) {
                const Bus1* oldBusPtr = &(*target);
                string newPl, newMd; int newCap;
                while (true) { cout << "Новый госномер: "; getline(cin, newPl); if (!newPl.empty()) break; }
                while (true) { cout << "Новая модель: "; getline(cin, newMd); if (!newMd.empty()) break; }
                while (true) {
                    cout << "Новая вместимость: ";
                    if (cin >> newCap && newCap >= 10 && newCap <= 150) { cin.ignore(10000, '\n'); break; }
                    cout << "Ошибка!\n"; cin.clear(); cin.ignore(10000, '\n');
                }
                busSet.erase(target);
                pair<set<Bus1>::iterator, bool> ins = busSet.insert(Bus1(newPl, newMd, newCap));
                const Bus1* newBusPtr = &(*ins.first);

                for (map<int, Trip1>::iterator tIt = tripsMap.begin(); tIt != tripsMap.end(); ++tIt) {
                    if (tIt->second.getBus1() == oldBusPtr) {
                        tIt->second.setBus1(newBusPtr);
                    }
                }
                updateData1(tripsMap, passUMap, destMMap, boardingUMMap, ticketsMSet);
                cout << "Данные автобуса обновлены!\n";
            }
            else {
                cout << "Автобус не найден.\n";
            }
            break;
        }
        case 5: {
            string pl;
            while (true) { cout << "Госномер для удаления: "; getline(cin, pl); if (!pl.empty()) break; }
            bool del = false;
            for (set<Bus1>::iterator it = busSet.begin(); it != busSet.end(); ++it) {
                if (it->getPlateNumber1() == pl) {
                    const Bus1* oldBusPtr = &(*it);
                    for (map<int, Trip1>::iterator tIt = tripsMap.begin(); tIt != tripsMap.end(); ++tIt) {
                        if (tIt->second.getBus1() == oldBusPtr) {
                            tIt->second.setBus1(nullptr);
                        }
                    }
                    busSet.erase(it);
                    del = true;
                    break;
                }
            }
            updateData1(tripsMap, passUMap, destMMap, boardingUMMap, ticketsMSet);
            cout << (del ? "Автобус удален!\n" : "Автобус не найден.\n");
            break;
        }
        case 6: {
            string pl;
            while (true) { cout << "Госномер для search: "; getline(cin, pl); if (!pl.empty()) break; }
            printBusHeader1(cout);
            for (set<Bus1>::iterator it = busSet.begin(); it != busSet.end(); ++it) {
                if (it->getPlateNumber1() == pl) cout << *it << endl;
            }
            cout << BUS_BORDER << "\n";
            break;
        }
        case 7: {
            int minCap;
            while (true) {
                cout << "Введите минимальную вместимость: ";
                if (cin >> minCap && minCap >= 0) { cin.ignore(10000, '\n'); break; }
                cout << "Ошибка!\n"; cin.clear(); cin.ignore(10000, '\n');
            }
            set<Bus1>::iterator lb = busSet.lower_bound(Bus1("", "", minCap));
            printBusHeader1(cout);
            while (lb != busSet.end()) {
                cout << *lb << endl;
                ++lb;
            }
            cout << BUS_BORDER << "\n";
            break;
        }
        }
    }
}

void menuReference1(
    map<int, Trip1>& tripsMap,
    multimap<string, Trip1>& destMMap,
    multiset<TicketRecord1>& ticketsMSet,
    unordered_set<Station1, StationHash1>& stationsUSet,
    unordered_multimap<int, Passenger1>& boardingUMMap,
    unordered_multiset<CompletedTripLog1, ArchiveHash1>& completedLogsUMSet
) {
    int ch = -1;
    while (ch != 0) {
        cout << "\n--- 4. МАРШРУТЫ, БИЛЕТЫ, СТАНЦИИ И АРХИВ ---\n";
        cout << "1. Search всех рейсов в город назначения\n";
        cout << "2. Архив проданных билетов и выборка по диапазону цен\n";
        cout << "3. Справочник станций\n";
        cout << "4. Посадочная ведомость по номеру рейса\n";
        cout << "5. Просмотр журнала завершенных рейсов\n";
        cout << "6. Сохранить отчет по завершенным рейсам в файл\n";
        cout << "0. Назад в главное меню\n";
        while (true) {
            cout << "Ваш выбор: ";
            if (cin >> ch && ch >= 0 && ch <= 6) { cin.ignore(10000, '\n'); break; }
            cout << "Ошибка! Введите 0 - 6.\n"; cin.clear(); cin.ignore(10000, '\n');
        }

        switch (ch) {
        case 1: {
            cout << "\nВсе маршруты по городу назначения:\n";
            printTripHeader1(cout);
            for (multimap<string, Trip1>::iterator it = destMMap.begin(); it != destMMap.end(); ++it) {
                cout << it->second << endl;
            }
            cout << TRIP_BORDER << "\n";

            string city;
            cout << "Введите город назначения для search: ";
            getline(cin, city);
            if (!city.empty()) {
                cout << "Количество рейсов в город " << city << ": " << destMMap.count(city) << endl;
                pair<multimap<string, Trip1>::iterator, multimap<string, Trip1>::iterator> range = destMMap.equal_range(city);
                printTripHeader1(cout);
                for (multimap<string, Trip1>::iterator it = range.first; it != range.second; ++it) {
                    cout << it->second << endl;
                }
                cout << TRIP_BORDER << "\n";
            }
            break;
        }
        case 2: {
            cout << "\nВсе проданные билеты:\n";
            printTicketHeader1(cout);
            for (multiset<TicketRecord1>::iterator it = ticketsMSet.begin(); it != ticketsMSet.end(); ++it) {
                cout << *it << endl;
            }
            cout << TICKET_BORDER << "\n";

            if (!ticketsMSet.empty()) {
                float pMin, pMax;
                while (true) {
                    cout << "Введите минимальную цену: ";
                    if (cin >> pMin && pMin >= 0.0f) { cin.ignore(10000, '\n'); break; }
                    cout << "Ошибка!\n"; cin.clear(); cin.ignore(10000, '\n');
                }
                while (true) {
                    cout << "Введите максимальную цену: ";
                    if (cin >> pMax && pMax >= pMin) { cin.ignore(10000, '\n'); break; }
                    cout << "Ошибка!\n"; cin.clear(); cin.ignore(10000, '\n');
                }
                multiset<TicketRecord1>::iterator low = ticketsMSet.lower_bound(TicketRecord1(nullptr, nullptr, pMin));
                multiset<TicketRecord1>::iterator up = ticketsMSet.upper_bound(TicketRecord1(nullptr, nullptr, pMax));
                cout << "Билеты в выбранном диапазоне цен:\n";
                printTicketHeader1(cout);
                for (multiset<TicketRecord1>::iterator it = low; it != up; ++it) {
                    cout << *it << endl;
                }
                cout << TICKET_BORDER << "\n";
            }
            break;
        }
        case 3: {
            int sub = -1;
            cout << "\n--- Справочник станций ---\n";
            cout << "1. Показать все станции\n2. Добавить станцию\n3. Update региона станции\n4. Удалить станцию\n5. Search станции\nВаш выбор: ";
            if (!(cin >> sub)) { cin.clear(); cin.ignore(10000, '\n'); break; }
            cin.ignore(10000, '\n');

            if (sub == 1) {
                printStationHeader1(cout);
                for (unordered_set<Station1, StationHash1>::iterator it = stationsUSet.begin(); it != stationsUSet.end(); ++it) {
                    cout << *it << endl;
                }
                cout << STATION_BORDER << "\n";
            }
            else if (sub == 2) {
                string c, r;
                while (true) { cout << "Название города: "; getline(cin, c); if (!c.empty()) break; }
                while (true) { cout << "Область: "; getline(cin, r); if (!r.empty()) break; }
                stationsUSet.insert(Station1(c, r));
                cout << "Станция добавлена!\n";
            }
            else if (sub == 3) {
                string c, r;
                while (true) { cout << "Город для update области: "; getline(cin, c); if (!c.empty()) break; }
                unordered_set<Station1, StationHash1>::iterator it = stationsUSet.find(Station1(c, ""));
                if (it != stationsUSet.end()) {
                    while (true) { cout << "Новая область: "; getline(cin, r); if (!r.empty()) break; }
                    stationsUSet.erase(it);
                    pair<unordered_set<Station1, StationHash1>::iterator, bool> ins = stationsUSet.insert(Station1(c, r));
                    const Station1* newPtr = &(*ins.first);
                    for (map<int, Trip1>::iterator tIt = tripsMap.begin(); tIt != tripsMap.end(); ++tIt) {
                        Route1 rt = tIt->second.getRoute1();
                        const Station1* sPtr = (rt.getStartPoint1() == c) ? newPtr : rt.getStartStationPtr1();
                        const Station1* ePtr = (rt.getEndPoint1() == c) ? newPtr : rt.getEndStationPtr1();
                        tIt->second.setRoute1(Route1(sPtr, ePtr, rt.getDistance1()));
                    }
                    cout << "Обновлено!\n";
                }
                else cout << "Город не найден!\n";
            }
            else if (sub == 4) {
                string c;
                while (true) { cout << "Город для удаления: "; getline(cin, c); if (!c.empty()) break; }
                for (map<int, Trip1>::iterator tIt = tripsMap.begin(); tIt != tripsMap.end(); ++tIt) {
                    tIt->second.getRouteRef1().resetStation1(c);
                }
                if (stationsUSet.erase(Station1(c, "")) > 0) cout << "Удалено!\n";
                else cout << "Город не найден!\n";
            }
            else if (sub == 5) {
                string c;
                while (true) { cout << "Город для search: "; getline(cin, c); if (!c.empty()) break; }
                unordered_set<Station1, StationHash1>::iterator it = stationsUSet.find(Station1(c, ""));
                if (it != stationsUSet.end()) {
                    printStationHeader1(cout); cout << *it << endl; cout << STATION_BORDER << "\n";
                }
                else cout << "Такой станции нет в сети.\n";
            }
            break;
        }
        case 4: {
            int tId;
            while (true) {
                cout << "Введите ID рейса для выборки пассажиров: ";
                if (cin >> tId && tId > 0) { cin.ignore(10000, '\n'); break; }
                cout << "Ошибка!\n"; cin.clear(); cin.ignore(10000, '\n');
            }
            cout << "Найдено пассажиров на рейсе №" << tId << ": " << boardingUMMap.count(tId) << endl;
            pair<unordered_multimap<int, Passenger1>::iterator, unordered_multimap<int, Passenger1>::iterator> range = boardingUMMap.equal_range(tId);
            printPassHeader1(cout);
            for (unordered_multimap<int, Passenger1>::iterator it = range.first; it != range.second; ++it) {
                cout << it->second << endl;
            }
            cout << PASS_BORDER << "\n";
            break;
        }
        case 5: {
            cout << "\nЖурнал завершенных поездок:\n";
            printLogHeader1(cout);
            for (unordered_multiset<CompletedTripLog1, ArchiveHash1>::iterator it = completedLogsUMSet.begin(); it != completedLogsUMSet.end(); ++it) {
                cout << *it << endl;
            }
            cout << LOG_BORDER << "\n";
            break;
        }
        case 6: {
            ofstream fout("archive_completed_trips.txt");
            fout << "ОТЧЕТ ПО ЗАВЕРШЕННЫМ РЕЙСАМ ПРЕДПРИЯТИЯ\n\n";
            printLogHeader1(fout);
            float total = 0.0f;
            for (unordered_multiset<CompletedTripLog1, ArchiveHash1>::iterator it = completedLogsUMSet.begin(); it != completedLogsUMSet.end(); ++it) {
                fout << *it << endl;
                total += it->getTotalRevenue1();
            }
            fout << LOG_BORDER << "\n";
            fout << "Итого историческая выручка по всем рейсам: " << formatNum1(total, 2) << " BYN\n";
            fout.close();
            cout << "Исторический отчет сформирован в файле archive_completed_trips.txt!\n";
            break;
        }
        }
    }
}

void menuCashier1(
    map<int, Trip1>& tripsMap,
    unordered_map<string, Passenger1>& passUMap,
    multimap<string, Trip1>& destMMap,
    unordered_multimap<int, Passenger1>& boardingUMMap,
    multiset<TicketRecord1>& ticketsMSet,
    unordered_multiset<CompletedTripLog1, ArchiveHash1>& completedLogsUMSet
) {
    int ch = -1;
    while (ch != 0) {
        cout << "\n--- 5. КАССА БИЛЕТОВ И ВЫПОЛНЕНИЕ РЕЙСОВ ---\n";
        cout << "1. Купить билет пассажиру на рейс\n";
        cout << "2. Оформить отказ от билета\n";
        cout << "3. Подтвердить завершение рейса\n";
        cout << "0. Назад в главное меню\n";
        while (true) {
            cout << "Ваш выбор: ";
            if (cin >> ch && ch >= 0 && ch <= 3) { cin.ignore(10000, '\n'); break; }
            cout << "Ошибка! Введите 0 - 3.\n"; cin.clear(); cin.ignore(10000, '\n');
        }

        switch (ch) {
        case 1: {
            printPassHeader1(cout);
            for (unordered_map<string, Passenger1>::iterator it = passUMap.begin(); it != passUMap.end(); ++it) cout << it->second << endl;
            cout << PASS_BORDER << "\n";

            string pass;
            while (true) { cout << "Введите паспорт пассажира: "; getline(cin, pass); if (!pass.empty()) break; }
            unordered_map<string, Passenger1>::iterator pIt = passUMap.find(pass);
            if (pIt == passUMap.end()) { cout << "Пассажир не найден!\n"; break; }
            if (pIt->second.getTripId1() != 0) {
                cout << "У пассажира уже куплен билет на Рейс №" << pIt->second.getTripId1() << "!\n";
                break;
            }

            printTripHeader1(cout);
            for (map<int, Trip1>::iterator it = tripsMap.begin(); it != tripsMap.end(); ++it) cout << it->second << endl;
            cout << TRIP_BORDER << "\n";

            int tId;
            while (true) {
                cout << "Введите ID рейса: ";
                if (cin >> tId && tId > 0) { cin.ignore(10000, '\n'); break; }
                cout << "Ошибка!\n"; cin.clear(); cin.ignore(10000, '\n');
            }
            map<int, Trip1>::iterator tIt = tripsMap.find(tId);
            if (tIt == tripsMap.end()) { cout << "Рейс не найден!\n"; break; }
            if (!tIt->second.hasSeats1()) {
                cout << "Отказ: на рейсе нет свободных мест или не назначен автобус!\n";
                break;
            }

            pIt->second.setTripId1(tId);
            updateData1(tripsMap, passUMap, destMMap, boardingUMMap, ticketsMSet);
            cout << "Билет успешно оформлен!\n";
            break;
        }
        case 2: {
            string pass;
            while (true) { cout << "Введите паспорт пассажира для возврата билета: "; getline(cin, pass); if (!pass.empty()) break; }
            unordered_map<string, Passenger1>::iterator pIt = passUMap.find(pass);
            if (pIt == passUMap.end() || pIt->second.getTripId1() == 0) {
                cout << "Пассажир не найден или у него нет активного билета!\n";
                break;
            }
            pIt->second.setTripId1(0);
            updateData1(tripsMap, passUMap, destMMap, boardingUMMap, ticketsMSet);
            cout << "Возврат оформлен! Место на рейсе освобождено.\n";
            break;
        }
        case 3: {
            printTripHeader1(cout);
            for (map<int, Trip1>::iterator it = tripsMap.begin(); it != tripsMap.end(); ++it) cout << it->second << endl;
            cout << TRIP_BORDER << "\n";

            int tId;
            while (true) {
                cout << "Введите ID завершившегося рейса: ";
                if (cin >> tId && tId > 0) { cin.ignore(10000, '\n'); break; }
                cout << "Ошибка!\n"; cin.clear(); cin.ignore(10000, '\n');
            }
            map<int, Trip1>::iterator tIt = tripsMap.find(tId);
            if (tIt == tripsMap.end()) { cout << "Рейс не найден!\n"; break; }

            string rDesc = tIt->second.getRoute1().getStartPoint1() + " -> " + tIt->second.getRoute1().getEndPoint1();
            int sold = tIt->second.getSoldTickets1();
            float rev = tIt->second.getRevenue1();

            CompletedTripLog1 logEntry(tId, rDesc, sold, rev);
            completedLogsUMSet.insert(logEntry);

            ofstream fout("completed_trips_report.txt", ios::app);
            fout << "=== ОТЧЕТ О ЗАВЕРШЕНИИ РЕЙСА №" << tId << " ===\n";
            fout << "Маршрут: " << rDesc << " | Перевезено: " << sold << " чел. | Выручка: " << formatNum1(rev, 2) << " BYN\n";
            printPassHeader1(fout);
            for (unordered_map<string, Passenger1>::iterator pIt = passUMap.begin(); pIt != passUMap.end(); ++pIt) {
                if (pIt->second.getTripId1() == tId) {
                    fout << pIt->second << endl;
                    pIt->second.setTripId1(0);
                }
            }
            fout << PASS_BORDER << "\n\n";
            fout.close();

            updateData1(tripsMap, passUMap, destMMap, boardingUMMap, ticketsMSet);
            cout << "Рейс №" << tId << " завершен! Запись добавлена в архив и файл completed_trips_report.txt.\n";
            cout << "Данные рейса и пассажиров возобновлены для новых перевозок.\n";
            break;
        }
        }
    }
}

void lab4() {
    system("cls");

    set<Bus1> busSet;
    busSet.insert(Bus1("1245 AB-7", "MAZ-251", 44));
    busSet.insert(Bus1("3311 OP-7", "MAN Lion's Coach", 48));
    busSet.insert(Bus1("8890 KE-7", "Mercedes Tourismo", 50));

    unordered_set<Station1, StationHash1> stationsUSet;
    const Station1* stMinsk = findStation1(stationsUSet, "Минск");
    const Station1* stBrest = findStation1(stationsUSet, "Брест");
    const Station1* stGrodno = findStation1(stationsUSet, "Гродно");
    findStation1(stationsUSet, "Гомель");

    const Bus1* b1 = findBus1(busSet, "1245 AB-7");
    const Bus1* b2 = findBus1(busSet, "8890 KE-7");
    const Bus1* b3 = findBus1(busSet, "3311 OP-7");

    map<int, Trip1> tripsMap;
    tripsMap.insert(make_pair(101, Trip1(101, Route1(stMinsk, stBrest, 348.0f), b1, 35.50f, 0)));
    tripsMap.insert(make_pair(102, Trip1(102, Route1(stMinsk, stGrodno, 278.0f), b2, 28.00f, 0)));
    tripsMap.insert(make_pair(103, Trip1(103, Route1(stMinsk, stBrest, 355.0f), b3, 32.00f, 0)));

    unordered_map<string, Passenger1> passUMap;
    passUMap.insert(make_pair("KB4455667", Passenger1("Кузнецов Д.М.", 29, "KB4455667", 101)));
    passUMap.insert(make_pair("AB8899001", Passenger1("Васильев О.С.", 36, "AB8899001", 101)));
    passUMap.insert(make_pair("EK3322114", Passenger1("Морозова Т.Н.", 22, "EK3322114", 102)));
    passUMap.insert(make_pair("MH7788992", Passenger1("Новиков В.П.", 48, "MH7788992", 0)));

    multimap<string, Trip1> destMMap;
    multiset<TicketRecord1> ticketsMSet;
    unordered_multimap<int, Passenger1> boardingUMMap;

    unordered_multiset<CompletedTripLog1, ArchiveHash1> completedLogsUMSet;
    completedLogsUMSet.insert(CompletedTripLog1(101, "Минск -> Брест", 40, 1420.0f));

    updateData1(tripsMap, passUMap, destMMap, boardingUMMap, ticketsMSet);

    int mainChoice = -1;
    while (mainChoice != 0) {
        cout << "\n======================================================\n";
        cout << "   СИСТЕМА УЧЕТА ДАННЫХ ПО ПАССАЖИРСКИМ ПЕРЕВОЗКАМ\n";
        cout << "======================================================\n";
        cout << "1. Расписание рейсов\n";
        cout << "2. База пассажиров\n";
        cout << "3. Реестр автопарка\n";
        cout << "4. Маршруты, билеты, станции и архив\n";
        cout << "5. Касса билетов и завершение рейсов\n";
        cout << "6. Полный сводный отчет всех данных\n";
        cout << "0. Выход из программы\n";
        while (true) {
            cout << "Выберите пункт меню: ";
            if (cin >> mainChoice && mainChoice >= 0 && mainChoice <= 6) {
                cin.ignore(10000, '\n');
                break;
            }
            cout << "Ошибка! Введите число от 0 до 6.\n";
            cin.clear();
            cin.ignore(10000, '\n');
        }

        switch (mainChoice) {
        case 1:
            menuTrips1(tripsMap, busSet, stationsUSet, passUMap, destMMap, boardingUMMap, ticketsMSet);
            break;
        case 2:
            menuPassengers1(passUMap, tripsMap, destMMap, boardingUMMap, ticketsMSet);
            break;
        case 3:
            menuFleet1(busSet, tripsMap, passUMap, destMMap, boardingUMMap, ticketsMSet);
            break;
        case 4:
            menuReference1(tripsMap, destMMap, ticketsMSet, stationsUSet, boardingUMMap, completedLogsUMSet);
            break;
        case 5:
            menuCashier1(tripsMap, passUMap, destMMap, boardingUMMap, ticketsMSet, completedLogsUMSet);
            break;
        case 6: {
            ofstream fout("full_associative_report.txt");

            cout << "\n>>> 1. РАСПИСАНИЕ РЕЙСОВ <<<\n";
            fout << ">>> 1. РАСПИСАНИЕ РЕЙСОВ <<<\n";
            printTripHeader1(cout); printTripHeader1(fout);
            for (map<int, Trip1>::iterator it = tripsMap.begin(); it != tripsMap.end(); ++it) {
                cout << it->second << endl; fout << it->second << endl;
            }
            cout << TRIP_BORDER << "\n"; fout << TRIP_BORDER << "\n\n";

            cout << "\n>>> 2. БАЗА ПАССАЖИРОВ <<<\n";
            fout << ">>> 2. БАЗА ПАССАЖИРОВ <<<\n";
            printPassHeader1(cout); printPassHeader1(fout);
            for (unordered_map<string, Passenger1>::iterator it = passUMap.begin(); it != passUMap.end(); ++it) {
                cout << it->second << endl; fout << it->second << endl;
            }
            cout << PASS_BORDER << "\n"; fout << PASS_BORDER << "\n\n";

            cout << "\n>>> 3. АВТОПАРК <<<\n";
            fout << ">>> 3. АВТОПАРК <<<\n";
            printBusHeader1(cout); printBusHeader1(fout);
            for (set<Bus1>::iterator it = busSet.begin(); it != busSet.end(); ++it) {
                cout << *it << endl; fout << *it << endl;
            }
            cout << BUS_BORDER << "\n"; fout << BUS_BORDER << "\n\n";

            cout << "\n>>> 4. РЕЙСЫ ПО ГОРОДАМ ПРИБЫТИЯ <<<\n";
            fout << ">>> 4. РЕЙСЫ ПО ГОРОДАМ ПРИБЫТИЯ <<<\n";
            printTripHeader1(cout); printTripHeader1(fout);
            for (multimap<string, Trip1>::iterator it = destMMap.begin(); it != destMMap.end(); ++it) {
                cout << it->second << endl; fout << it->second << endl;
            }
            cout << TRIP_BORDER << "\n"; fout << TRIP_BORDER << "\n\n";

            cout << "\n>>> 5. ПРОДАННЫЕ БИЛЕТЫ <<<\n";
            fout << ">>> 5. ПРОДАННЫЕ БИЛЕТЫ <<<\n";
            printTicketHeader1(cout); printTicketHeader1(fout);
            for (multiset<TicketRecord1>::iterator it = ticketsMSet.begin(); it != ticketsMSet.end(); ++it) {
                cout << *it << endl; fout << *it << endl;
            }
            cout << TICKET_BORDER << "\n"; fout << TICKET_BORDER << "\n\n";

            cout << "\n>>> 6. СТАНЦИИ МАРШРУТНОЙ СЕТИ <<<\n";
            fout << ">>> 6. СТАНЦИИ МАРШРУТНОЙ СЕТИ <<<\n";
            printStationHeader1(cout); printStationHeader1(fout);
            for (unordered_set<Station1, StationHash1>::iterator it = stationsUSet.begin(); it != stationsUSet.end(); ++it) {
                cout << *it << endl; fout << *it << endl;
            }
            cout << STATION_BORDER << "\n"; fout << STATION_BORDER << "\n\n";

            cout << "\n>>> 7. ПОСАДОЧНАЯ ВЕДОМОСТЬ <<<\n";
            fout << ">>> 7. ПОСАДОЧНАЯ ВЕДОМОСТЬ <<<\n";
            printPassHeader1(cout); printPassHeader1(fout);
            for (unordered_multimap<int, Passenger1>::iterator it = boardingUMMap.begin(); it != boardingUMMap.end(); ++it) {
                cout << it->second << endl; fout << it->second << endl;
            }
            cout << PASS_BORDER << "\n"; fout << PASS_BORDER << "\n\n";

            cout << "\n>>> 8. АРХИВ ЗАВЕРШЕННЫХ РЕЙСОВ <<<\n";
            fout << ">>> 8. АРХИВ ЗАВЕРШЕННЫХ РЕЙСОВ <<<\n";
            printLogHeader1(cout); printLogHeader1(fout);
            for (unordered_multiset<CompletedTripLog1, ArchiveHash1>::iterator it = completedLogsUMSet.begin(); it != completedLogsUMSet.end(); ++it) {
                cout << *it << endl; fout << *it << endl;
            }
            cout << LOG_BORDER << "\n"; fout << LOG_BORDER << "\n";

            fout.close();
            cout << "\nПолный сводный отчет сохранен в файл full_associative_report.txt!\n";
            break;
        }
        case 0:
            break;
        }
    }

}
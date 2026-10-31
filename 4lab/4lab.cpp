#include <iostream>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <iomanip>
#include <fstream>

using namespace std;

int visLen1(const string& s) {
    int count = 0;
    for (int i = 0; i < (int)s.length(); i++) {
        if ((s[i] & 0xC0) != 0x80) {
            count++;
        }
    }
    return count;
}

string pad1(const string& s, int width) {
    int len = visLen1(s);
    if (len >= width) {
        return s;
    }
    return s + string(width - len, ' ');
}

string formatFloat1(float val, int decimals) {
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
        stream << "| " << pad1(obj.fullName, 24)
            << " | " << pad1(to_string(obj.age), 8)
            << " | " << pad1(obj.passport, 14)
            << " | " << pad1(ticketStr, 14) << " |";
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
        stream << "| " << pad1(obj.cityName, 20)
            << " | " << pad1(obj.region, 26) << " |";
        return stream;
    }
    friend bool operator<(const Station1& a, const Station1& b) {
        return a.cityName < b.cityName;
    }
    friend bool operator==(const Station1& a, const Station1& b) {
        return a.cityName == b.cityName;
    }
};

struct StationHasher1 {
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
        stream << "| " << pad1(obj.plateNumber, 16)
            << " | " << pad1(obj.model, 22)
            << " | " << pad1(to_string(obj.capacity), 14) << " |";
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
    string startPoint;
    string endPoint;
    float distance;
public:
    Route1() {
        startPoint = "Не задано";
        endPoint = "Не задано";
        distance = 0.0f;
    }
    Route1(string startPoint, string endPoint, float distance) {
        this->startPoint = startPoint;
        this->endPoint = endPoint;
        this->distance = distance;
    }
    string getStartPoint1() const { return startPoint; }
    string getEndPoint1() const { return endPoint; }
    float getDistance1() const { return distance; }
};

class Trip1 {
private:
    int tripId;
    Route1 route;
    string busPlate;
    int maxSeats;
    float ticketPrice;
    int soldTickets;
public:
    Trip1() {
        tripId = 0;
        busPlate = "Не назначен";
        maxSeats = 0;
        ticketPrice = 0.0f;
        soldTickets = 0;
    }
    Trip1(int tripId, Route1 route, string busPlate, int maxSeats, float ticketPrice, int soldTickets = 0) {
        this->tripId = tripId;
        this->route = route;
        this->busPlate = busPlate;
        this->maxSeats = maxSeats;
        this->ticketPrice = ticketPrice;
        this->soldTickets = soldTickets;
    }

    int getTripId1() const { return tripId; }
    void setRoute1(Route1 r) { route = r; }
    Route1 getRoute1() const { return route; }
    void setBusInfo1(string plate, int seats) { busPlate = plate; maxSeats = seats; }
    string getBusPlate1() const { return busPlate; }
    int getMaxSeats1() const { return maxSeats; }
    void setTicketPrice1(float price) { ticketPrice = price; }
    float getTicketPrice1() const { return ticketPrice; }
    void setSoldTickets1(int sold) { soldTickets = sold; }
    int getSoldTickets1() const { return soldTickets; }

    bool hasFreeSeats1() const { return soldTickets < maxSeats; }
    float getRevenue1() const { return soldTickets * ticketPrice; }

    friend ostream& operator<<(ostream& stream, const Trip1& obj) {
        string seatsInfo = to_string(obj.soldTickets) + "/" + to_string(obj.maxSeats);
        stream << "| " << pad1(to_string(obj.tripId), 6)
            << " | " << pad1(obj.route.getStartPoint1(), 14)
            << " | " << pad1(obj.route.getEndPoint1(), 14)
            << " | " << pad1(formatFloat1(obj.route.getDistance1(), 1), 10)
            << " | " << pad1(obj.busPlate, 14)
            << " | " << pad1(seatsInfo, 10)
            << " | " << pad1(formatFloat1(obj.ticketPrice, 2), 11) << " |";
        return stream;
    }
    friend bool operator<(const Trip1& a, const Trip1& b) {
        return a.ticketPrice < b.ticketPrice;
    }
};

class TicketRecord1 {
private:
    int tripId;
    string passengerPassport;
    float price;
public:
    TicketRecord1() {
        tripId = 0;
        passengerPassport = "Нет";
        price = 0.0f;
    }
    TicketRecord1(int tripId, string passengerPassport, float price) {
        this->tripId = tripId;
        this->passengerPassport = passengerPassport;
        this->price = price;
    }
    int getTripId1() const { return tripId; }
    string getPassengerPassport1() const { return passengerPassport; }
    float getPrice1() const { return price; }

    friend ostream& operator<<(ostream& stream, const TicketRecord1& obj) {
        stream << "| " << pad1(to_string(obj.tripId), 10)
            << " | " << pad1(obj.passengerPassport, 18)
            << " | " << pad1(formatFloat1(obj.price, 2), 14) << " |";
        return stream;
    }
    friend bool operator<(const TicketRecord1& a, const TicketRecord1& b) {
        return a.price < b.price;
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
        routeDesc = "Нет";
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
        stream << "| " << pad1(to_string(obj.tripId), 8)
            << " | " << pad1(obj.routeDesc, 26)
            << " | " << pad1(to_string(obj.passengersCount), 12)
            << " | " << pad1(formatFloat1(obj.totalRevenue, 2), 14) << " |";
        return stream;
    }
    friend bool operator<(const CompletedTripLog1& a, const CompletedTripLog1& b) {
        return a.totalRevenue < b.totalRevenue;
    }
    friend bool operator==(const CompletedTripLog1& a, const CompletedTripLog1& b) {
        return a.tripId == b.tripId && a.routeDesc == b.routeDesc && a.passengersCount == b.passengersCount;
    }
};

struct CompletedLogHasher1 {
    size_t operator()(const CompletedTripLog1& log) const {
        return hash<int>()(log.getTripId1()) ^ hash<string>()(log.getRouteDesc1());
    }
};

const string PASS_BORDER = "+--------------------------+----------+----------------+----------------+";
const string BUS_BORDER = "+------------------+------------------------+----------------+";
const string TRIP_BORDER = "+--------+----------------+----------------+------------+----------------+------------+-------------+";
const string TICKET_BORDER = "+------------+--------------------+----------------+";
const string STATION_BORDER = "+----------------------+----------------------------+";
const string LOG_BORDER = "+----------+----------------------------+--------------+----------------+";

void printPassHeader1(ostream& out) {
    out << PASS_BORDER << "\n| " << pad1("ФИО пассажира", 24) << " | " << pad1("Возраст", 8)
        << " | " << pad1("Паспорт (Ключ)", 14) << " | " << pad1("Статус билета", 14) << " |\n" << PASS_BORDER << "\n";
}
void printBusHeader1(ostream& out) {
    out << BUS_BORDER << "\n| " << pad1("Госномер", 16) << " | " << pad1("Модель автобуса", 22)
        << " | " << pad1("Вместимость", 14) << " |\n" << BUS_BORDER << "\n";
}
void printTripHeader1(ostream& out) {
    out << TRIP_BORDER << "\n| " << pad1("ID", 6) << " | " << pad1("Откуда", 14) << " | " << pad1("Куда", 14)
        << " | " << pad1("Дист.(км)", 10) << " | " << pad1("Автобус", 14) << " | " << pad1("Места", 10)
        << " | " << pad1("Цена (BYN)", 11) << " |\n" << TRIP_BORDER << "\n";
}
void printTicketHeader1(ostream& out) {
    out << TICKET_BORDER << "\n| " << pad1("ID Рейса", 10) << " | " << pad1("Паспорт пассажира", 18)
        << " | " << pad1("Цена (BYN)", 14) << " |\n" << TICKET_BORDER << "\n";
}
void printStationHeader1(ostream& out) {
    out << STATION_BORDER << "\n| " << pad1("Город (Ключ)", 20) << " | " << pad1("Область / Регион", 26)
        << " |\n" << STATION_BORDER << "\n";
}
void printLogHeader1(ostream& out) {
    out << LOG_BORDER << "\n| " << pad1("ID Рейса", 8) << " | " << pad1("Маршрут", 26)
        << " | " << pad1("Пассажиров", 12) << " | " << pad1("Выручка (BYN)", 14) << " |\n" << LOG_BORDER << "\n";
}

void syncAssociativeData1(
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
                ticketsMSet.insert(TicketRecord1(currentTripId, pIt->first, price));
            }
        }
        tIt->second.setSoldTickets1(count);
        destMMap.insert(make_pair(tIt->second.getRoute1().getEndPoint1(), tIt->second));
    }
}

void menuMapTrips1(
    map<int, Trip1>& tripsMap,
    set<Bus1>& busSet,
    unordered_map<string, Passenger1>& passUMap,
    multimap<string, Trip1>& destMMap,
    unordered_multimap<int, Passenger1>& boardingUMMap,
    multiset<TicketRecord1>& ticketsMSet
) {
    int ch = -1;
    while (ch != 0) {
        cout << "\n--- 1. КОНТЕЙНЕР std::map<int, Trip> (Расписание рейсов по ID) ---\n";
        cout << "1. Просмотр всех рейсов (прямой итератор begin -> end)\n";
        cout << "2. Просмотр рейсов в обратном порядке (reverse_iterator rbegin -> rend)\n";
        cout << "3. Запись таблицы рейсов в файл (map_trips.txt)\n";
        cout << "4. Добавить новый рейс (insert + pair / make_pair)\n";
        cout << "5. Редактировать рейс по ключу ID\n";
        cout << "6. Удалить рейс по ключу ID (erase)\n";
        cout << "7. Поиск рейса по ключу ID (метод find)\n";
        cout << "8. Фильтр рейсов по максимальной цене и сортировка по цене\n";
        cout << "0. Назад в главное меню\n";
        while (true) {
            cout << "Ваш выбор: ";
            if (cin >> ch && ch >= 0 && ch <= 8) { cin.ignore(10000, '\n'); break; }
            cout << "Ошибка! Введите 0 - 8.\n"; cin.clear(); cin.ignore(10000, '\n');
        }

        switch (ch) {
        case 1: {
            if (tripsMap.empty()) cout << "Отображение map пусто.\n";
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
            if (tripsMap.empty()) cout << "Отображение map пусто.\n";
            else {
                printTripHeader1(cout);
                for (map<int, Trip1>::reverse_iterator rit = tripsMap.rbegin(); rit != tripsMap.rend(); ++rit) {
                    cout << rit->second << endl;
                }
                cout << TRIP_BORDER << "\n";
            }
            break;
        }
        case 3: {
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
        case 4: {
            int id; string sP, eP, bPlate; float dist, price;
            while (true) {
                cout << "Введите уникальный ключ (ID рейса > 0): ";
                if (cin >> id && id > 0 && tripsMap.find(id) == tripsMap.end()) {
                    cin.ignore(10000, '\n'); break;
                }
                cout << "Ошибка или такой ключ уже существует в map!\n";
                cin.clear(); cin.ignore(10000, '\n');
            }
            while (true) { cout << "Пункт отправления: "; getline(cin, sP); if (!sP.empty()) break; }
            while (true) { cout << "Пункт назначения: "; getline(cin, eP); if (!eP.empty()) break; }
            while (true) {
                cout << "Дистанция (км): ";
                if (cin >> dist && dist > 0.0f) { cin.ignore(10000, '\n'); break; }
                cout << "Ошибка!\n"; cin.clear(); cin.ignore(10000, '\n');
            }
            while (true) {
                cout << "Цена билета (BYN): ";
                if (cin >> price && price >= 0.0f) { cin.ignore(10000, '\n'); break; }
                cout << "Ошибка!\n"; cin.clear(); cin.ignore(10000, '\n');
            }

            cout << "Доступные автобусы в set<Bus>:\n";
            printBusHeader1(cout);
            for (set<Bus1>::iterator bIt = busSet.begin(); bIt != busSet.end(); ++bIt) cout << *bIt << endl;
            cout << BUS_BORDER << "\n";

            while (true) { cout << "Введите госномер автобуса: "; getline(cin, bPlate); if (!bPlate.empty()) break; }
            int cap = 0;
            for (set<Bus1>::iterator bIt = busSet.begin(); bIt != busSet.end(); ++bIt) {
                if (bIt->getPlateNumber1() == bPlate) { cap = bIt->getCapacity1(); break; }
            }
            if (cap == 0) { bPlate = "Не назначен"; cout << "Автобус не найден, создан без мест.\n"; }

            tripsMap.insert(pair<int, Trip1>(id, Trip1(id, Route1(sP, eP, dist), bPlate, cap, price, 0)));
            syncAssociativeData1(tripsMap, passUMap, destMMap, boardingUMMap, ticketsMSet);
            cout << "Рейс успешно добавлен в map!\n";
            break;
        }
        case 5: {
            int id;
            while (true) {
                cout << "Введите ключ (ID рейса) для редактирования: ";
                if (cin >> id && id > 0) { cin.ignore(10000, '\n'); break; }
                cout << "Ошибка!\n"; cin.clear(); cin.ignore(10000, '\n');
            }
            map<int, Trip1>::iterator mapPtr = tripsMap.find(id);
            if (mapPtr != tripsMap.end()) {
                string sP, eP; float dist, price;
                while (true) { cout << "Новый пункт отправления: "; getline(cin, sP); if (!sP.empty()) break; }
                while (true) { cout << "Новый пункт назначения: "; getline(cin, eP); if (!eP.empty()) break; }
                while (true) {
                    cout << "Новая дистанция (км): ";
                    if (cin >> dist && dist > 0.0f) { cin.ignore(10000, '\n'); break; }
                    cout << "Ошибка!\n"; cin.clear(); cin.ignore(10000, '\n');
                }
                while (true) {
                    cout << "Новая цена билета (BYN): ";
                    if (cin >> price && price >= 0.0f) { cin.ignore(10000, '\n'); break; }
                    cout << "Ошибка!\n"; cin.clear(); cin.ignore(10000, '\n');
                }
                mapPtr->second.setRoute1(Route1(sP, eP, dist));
                mapPtr->second.setTicketPrice1(price);
                syncAssociativeData1(tripsMap, passUMap, destMMap, boardingUMMap, ticketsMSet);
                cout << "Рейс успешно отредактирован!\n";
            }
            else {
                cout << "Рейс с таким ключом не найден.\n";
            }
            break;
        }
        case 6: {
            int id;
            while (true) {
                cout << "Введите ключ (ID рейса) для удаления: ";
                if (cin >> id && id > 0) { cin.ignore(10000, '\n'); break; }
                cout << "Ошибка!\n"; cin.clear(); cin.ignore(10000, '\n');
            }
            if (tripsMap.erase(id) > 0) {
                for (unordered_map<string, Passenger1>::iterator pIt = passUMap.begin(); pIt != passUMap.end(); ++pIt) {
                    if (pIt->second.getTripId1() == id) pIt->second.setTripId1(0);
                }
                syncAssociativeData1(tripsMap, passUMap, destMMap, boardingUMMap, ticketsMSet);
                cout << "Рейс удален из map, билеты пассажиров аннулированы!\n";
            }
            else {
                cout << "Ключ не найден!\n";
            }
            break;
        }
        case 7: {
            int id;
            while (true) {
                cout << "Введите ключ (ID рейса) для поиска: ";
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
                cout << "Рейс с ключом " << id << " не найден в отображении.\n";
            }
            break;
        }
        case 8: {
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
            cout << "Рейсы (отфильтрованы и автоматически упорядочены по возрастанию цены):\n";
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

void menuUnorderedMapPassengers1(
    unordered_map<string, Passenger1>& passUMap,
    map<int, Trip1>& tripsMap,
    multimap<string, Trip1>& destMMap,
    unordered_multimap<int, Passenger1>& boardingUMMap,
    multiset<TicketRecord1>& ticketsMSet
) {
    int ch = -1;
    while (ch != 0) {
        cout << "\n--- 2. КОНТЕЙНЕР std::unordered_map<string, Passenger> (Пассажиры по паспорту) ---\n";
        cout << "1. Просмотр всех пассажиров (на экран)\n";
        cout << "2. Вывод таблицы в файл (unordered_map_passengers.txt)\n";
        cout << "3. Добавить пассажира (insert + make_pair)\n";
        cout << "4. Редактировать пассажира по ключу-паспорту\n";
        cout << "5. Удалить пассажира по ключу-паспорту (erase)\n";
        cout << "6. Быстрый поиск по паспорту за O(1) (find)\n";
        cout << "7. Фильтр по минимальному возрасту\n";
        cout << "8. Вывод пассажиров в алфавитном порядке ФИО (через упорядоченный set)\n";
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
                cout << "Введите серию и номер паспорта (уникальный ключ): ";
                getline(cin, pass);
                if (!pass.empty() && passUMap.find(pass) == passUMap.end()) break;
                cout << "Ошибка или паспорт уже есть в базе!\n";
            }
            while (true) { cout << "Введите ФИО: "; getline(cin, name); if (!name.empty()) break; }
            while (true) {
                cout << "Введите возраст (1 - 120): ";
                if (cin >> age && age >= 1 && age <= 120) { cin.ignore(10000, '\n'); break; }
                cout << "Ошибка!\n"; cin.clear(); cin.ignore(10000, '\n');
            }
            passUMap.insert(make_pair(pass, Passenger1(name, age, pass, 0)));
            cout << "Пассажир добавлен в unordered_map!\n";
            break;
        }
        case 4: {
            string pass;
            while (true) { cout << "Введите паспорт для редактирования: "; getline(cin, pass); if (!pass.empty()) break; }
            unordered_map<string, Passenger1>::iterator it = passUMap.find(pass);
            if (it != passUMap.end()) {
                string name; int age;
                while (true) { cout << "Новое ФИО: "; getline(cin, name); if (!name.empty()) break; }
                while (true) {
                    cout << "Новый возраст (1 - 120): ";
                    if (cin >> age && age >= 1 && age <= 120) { cin.ignore(10000, '\n'); break; }
                    cout << "Ошибка!\n"; cin.clear(); cin.ignore(10000, '\n');
                }
                it->second.setFullName1(name);
                it->second.setAge1(age);
                syncAssociativeData1(tripsMap, passUMap, destMMap, boardingUMMap, ticketsMSet);
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
                syncAssociativeData1(tripsMap, passUMap, destMMap, boardingUMMap, ticketsMSet);
                cout << "Пассажир удален из хеш-таблицы!\n";
            }
            else {
                cout << "Пассажир не найден.\n";
            }
            break;
        }
        case 6: {
            string pass;
            while (true) { cout << "Введите паспорт для поиска: "; getline(cin, pass); if (!pass.empty()) break; }
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

void menuSetBuses1(set<Bus1>& busSet) {
    int ch = -1;
    while (ch != 0) {
        cout << "\n--- 3. КОНТЕЙНЕР std::set<Bus> (Уникальные автобусы, автосортировка по вместимости) ---\n";
        cout << "1. Просмотр автопарка (упорядочен по возрастанию вместимости)\n";
        cout << "2. Просмотр автопарка по убыванию вместимости (reverse_iterator)\n";
        cout << "3. Запись в файл (set_buses.txt)\n";
        cout << "4. Добавить автобус (insert)\n";
        cout << "5. Редактировать автобус (удаление старого ключа + вставка нового)\n";
        cout << "6. Удалить автобус по госномеру\n";
        cout << "7. Поиск автобуса по госномеру\n";
        cout << "8. Диапазонный фильтр по вместимости через lower_bound()\n";
        cout << "0. Назад в главное меню\n";
        while (true) {
            cout << "Ваш выбор: ";
            if (cin >> ch && ch >= 0 && ch <= 8) { cin.ignore(10000, '\n'); break; }
            cout << "Ошибка! Введите 0 - 8.\n"; cin.clear(); cin.ignore(10000, '\n');
        }

        switch (ch) {
        case 1: {
            printBusHeader1(cout);
            for (set<Bus1>::iterator it = busSet.begin(); it != busSet.end(); ++it) cout << *it << endl;
            cout << BUS_BORDER << "\n";
            break;
        }
        case 2: {
            printBusHeader1(cout);
            for (set<Bus1>::reverse_iterator rit = busSet.rbegin(); rit != busSet.rend(); ++rit) cout << *rit << endl;
            cout << BUS_BORDER << "\n";
            break;
        }
        case 3: {
            ofstream fout("set_buses.txt");
            printBusHeader1(fout);
            for (set<Bus1>::iterator it = busSet.begin(); it != busSet.end(); ++it) fout << *it << endl;
            fout << BUS_BORDER << "\n";
            fout.close();
            cout << "Сохранено в set_buses.txt!\n";
            break;
        }
        case 4: {
            string pl, md; int cap;
            while (true) { cout << "Госномер: "; getline(cin, pl); if (!pl.empty()) break; }
            while (true) { cout << "Модель автобуса: "; getline(cin, md); if (!md.empty()) break; }
            while (true) {
                cout << "Вместимость (10 - 150): ";
                if (cin >> cap && cap >= 10 && cap <= 150) { cin.ignore(10000, '\n'); break; }
                cout << "Ошибка!\n"; cin.clear(); cin.ignore(10000, '\n');
            }
            busSet.insert(Bus1(pl, md, cap));
            cout << "Автобус добавлен и автоматически отсортирован в set!\n";
            break;
        }
        case 5: {
            string pl;
            while (true) { cout << "Введите госномер автобуса для редактирования: "; getline(cin, pl); if (!pl.empty()) break; }
            set<Bus1>::iterator target = busSet.end();
            for (set<Bus1>::iterator it = busSet.begin(); it != busSet.end(); ++it) {
                if (it->getPlateNumber1() == pl) { target = it; break; }
            }
            if (target != busSet.end()) {
                string newPl, newMd; int newCap;
                while (true) { cout << "Новый госномер: "; getline(cin, newPl); if (!newPl.empty()) break; }
                while (true) { cout << "Новая модель: "; getline(cin, newMd); if (!newMd.empty()) break; }
                while (true) {
                    cout << "Новая вместимость (10 - 150): ";
                    if (cin >> newCap && newCap >= 10 && newCap <= 150) { cin.ignore(10000, '\n'); break; }
                    cout << "Ошибка!\n"; cin.clear(); cin.ignore(10000, '\n');
                }
                busSet.erase(target);
                busSet.insert(Bus1(newPl, newMd, newCap));
                cout << "Данные автобуса обновлены в set!\n";
            }
            else {
                cout << "Автобус не найден.\n";
            }
            break;
        }
        case 6: {
            string pl;
            while (true) { cout << "Госномер для удаления: "; getline(cin, pl); if (!pl.empty()) break; }
            bool del = false;
            for (set<Bus1>::iterator it = busSet.begin(); it != busSet.end(); ++it) {
                if (it->getPlateNumber1() == pl) { busSet.erase(it); del = true; break; }
            }
            cout << (del ? "Автобус удален из set!\n" : "Автобус не найден.\n");
            break;
        }
        case 7: {
            string pl;
            while (true) { cout << "Госномер для поиска: "; getline(cin, pl); if (!pl.empty()) break; }
            printBusHeader1(cout);
            for (set<Bus1>::iterator it = busSet.begin(); it != busSet.end(); ++it) {
                if (it->getPlateNumber1() == pl) cout << *it << endl;
            }
            cout << BUS_BORDER << "\n";
            break;
        }
        case 8: {
            int minCap;
            while (true) {
                cout << "Введите минимальную вместимость для lower_bound: ";
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

void menuOtherContainers1(
    multimap<string, Trip1>& destMMap,
    multiset<TicketRecord1>& ticketsMSet,
    unordered_set<Station1, StationHasher1>& stationsUSet,
    unordered_multimap<int, Passenger1>& boardingUMMap,
    unordered_multiset<CompletedTripLog1, CompletedLogHasher1>& completedLogsUMSet
) {
    int ch = -1;
    while (ch != 0) {
        cout << "\n--- 4. РАБОТА С MULTIMAP, MULTISET И НЕУПОРЯДОЧЕННЫМИ КОНТЕЙНЕРАМИ ---\n";
        cout << "1. [multimap<string, Trip>] Поиск всех рейсов в город назначения (equal_range)\n";
        cout << "2. [multiset<TicketRecord>] Архив билетов + границы цен (lower_bound / upper_bound)\n";
        cout << "3. [unordered_set<Station>] Справочник станций (Просмотр / Добавить / Удалить / Найти)\n";
        cout << "4. [unordered_multimap<int, Passenger>] Посадочная ведомость по номеру рейса (count + equal_range)\n";
        cout << "5. [unordered_multiset<CompletedTripLog>] Журнал завершенных рейсов (Просмотр / Поиск / В файл)\n";
        cout << "0. Назад в главное меню\n";
        while (true) {
            cout << "Ваш выбор: ";
            if (cin >> ch && ch >= 0 && ch <= 5) { cin.ignore(10000, '\n'); break; }
            cout << "Ошибка! Введите 0 - 5.\n"; cin.clear(); cin.ignore(10000, '\n');
        }

        switch (ch) {
        case 1: {
            cout << "\nВсе маршруты в multimap<string, Trip> (упорядочены по городу назначения):\n";
            printTripHeader1(cout);
            for (multimap<string, Trip1>::iterator it = destMMap.begin(); it != destMMap.end(); ++it) {
                cout << it->second << endl;
            }
            cout << TRIP_BORDER << "\n";

            string city;
            cout << "Введите город назначения для выборки через equal_range (или Enter для пропуска): ";
            getline(cin, city);
            if (!city.empty()) {
                cout << "Количество рейсов в город " << city << " (метод count): " << destMMap.count(city) << endl;
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
            cout << "\nВсе проданные билеты в multiset<TicketRecord> (автосортировка по цене):\n";
            printTicketHeader1(cout);
            for (multiset<TicketRecord1>::iterator it = ticketsMSet.begin(); it != ticketsMSet.end(); ++it) {
                cout << *it << endl;
            }
            cout << TICKET_BORDER << "\n";

            if (!ticketsMSet.empty()) {
                float pMin, pMax;
                while (true) {
                    cout << "Введите минимальную цену (для lower_bound): ";
                    if (cin >> pMin && pMin >= 0.0f) { cin.ignore(10000, '\n'); break; }
                    cout << "Ошибка!\n"; cin.clear(); cin.ignore(10000, '\n');
                }
                while (true) {
                    cout << "Введите максимальную цену (для upper_bound): ";
                    if (cin >> pMax && pMax >= pMin) { cin.ignore(10000, '\n'); break; }
                    cout << "Ошибка!\n"; cin.clear(); cin.ignore(10000, '\n');
                }
                multiset<TicketRecord1>::iterator low = ticketsMSet.lower_bound(TicketRecord1(0, "", pMin));
                multiset<TicketRecord1>::iterator up = ticketsMSet.upper_bound(TicketRecord1(0, "", pMax));
                cout << "Билеты в диапазоне цен [" << formatFloat1(pMin, 2) << " .. " << formatFloat1(pMax, 2) << "]:\n";
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
            cout << "\n--- Справочник станций (unordered_set<Station>) ---\n";
            cout << "1. Показать все станции\n2. Добавить станцию\n3. Редактировать регион станции\n4. Удалить станцию\n5. Проверить наличие станции (find)\nВаш выбор: ";
            if (!(cin >> sub)) { cin.clear(); cin.ignore(10000, '\n'); break; }
            cin.ignore(10000, '\n');

            if (sub == 1) {
                printStationHeader1(cout);
                for (unordered_set<Station1, StationHasher1>::iterator it = stationsUSet.begin(); it != stationsUSet.end(); ++it) {
                    cout << *it << endl;
                }
                cout << STATION_BORDER << "\n";
            }
            else if (sub == 2) {
                string c, r;
                while (true) { cout << "Название города: "; getline(cin, c); if (!c.empty()) break; }
                while (true) { cout << "Область: "; getline(cin, r); if (!r.empty()) break; }
                stationsUSet.insert(Station1(c, r));
                cout << "Станция добавлена в unordered_set!\n";
            }
            else if (sub == 3) {
                string c, r;
                while (true) { cout << "Город для изменения области: "; getline(cin, c); if (!c.empty()) break; }
                unordered_set<Station1, StationHasher1>::iterator it = stationsUSet.find(Station1(c, ""));
                if (it != stationsUSet.end()) {
                    while (true) { cout << "Новая область: "; getline(cin, r); if (!r.empty()) break; }
                    stationsUSet.erase(it);
                    stationsUSet.insert(Station1(c, r));
                    cout << "Обновлено!\n";
                }
                else cout << "Город не найден!\n";
            }
            else if (sub == 4) {
                string c;
                while (true) { cout << "Город для удаления: "; getline(cin, c); if (!c.empty()) break; }
                if (stationsUSet.erase(Station1(c, "")) > 0) cout << "Удалено!\n";
                else cout << "Город не найден!\n";
            }
            else if (sub == 5) {
                string c;
                while (true) { cout << "Город для поиска: "; getline(cin, c); if (!c.empty()) break; }
                unordered_set<Station1, StationHasher1>::iterator it = stationsUSet.find(Station1(c, ""));
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
                cout << "Введите ID рейса для выборки пассажиров из unordered_multimap: ";
                if (cin >> tId && tId > 0) { cin.ignore(10000, '\n'); break; }
                cout << "Ошибка!\n"; cin.clear(); cin.ignore(10000, '\n');
            }
            cout << "Найдено пассажиров на рейсе №" << tId << " (метод count): " << boardingUMMap.count(tId) << endl;
            auto range = boardingUMMap.equal_range(tId);
            printPassHeader1(cout);
            for (auto it = range.first; it != range.second; ++it) {
                cout << it->second << endl;
            }
            cout << PASS_BORDER << "\n";
            break;
        }
        case 5: {
            cout << "\nЖурнал завершенных поездок (unordered_multiset<CompletedTripLog>):\n";
            printLogHeader1(cout);
            for (auto it = completedLogsUMSet.begin(); it != completedLogsUMSet.end(); ++it) {
                cout << *it << endl;
            }
            cout << LOG_BORDER << "\n";
            break;
        }
        }
    }
}

void menuTicketOffice1(
    map<int, Trip1>& tripsMap,
    unordered_map<string, Passenger1>& passUMap,
    multimap<string, Trip1>& destMMap,
    unordered_multimap<int, Passenger1>& boardingUMMap,
    multiset<TicketRecord1>& ticketsMSet,
    unordered_multiset<CompletedTripLog1, CompletedLogHasher1>& completedLogsUMSet
) {
    int ch = -1;
    while (ch != 0) {
        cout << "\n--- 5. КАССА БИЛЕТОВ И ВЫПОЛНЕНИЕ РЕЙСОВ ---\n";
        cout << "1. Купить билет пассажиру на рейс\n";
        cout << "2. Оформить отказ от билета (возврат)\n";
        cout << "3. Подтвердить завершение рейса (Запись в отчет + возобновление мест)\n";
        cout << "0. Назад в главное меню\n";
        while (true) {
            cout << "Ваш выбор: ";
            if (cin >> ch && ch >= 0 && ch <= 3) { cin.ignore(10000, '\n'); break; }
            cout << "Ошибка! Введите 0 - 3.\n"; cin.clear(); cin.ignore(10000, '\n');
        }

        switch (ch) {
        case 1: {
            printPassHeader1(cout);
            for (auto it = passUMap.begin(); it != passUMap.end(); ++it) cout << it->second << endl;
            cout << PASS_BORDER << "\n";

            string pass;
            while (true) { cout << "Введите паспорт пассажира: "; getline(cin, pass); if (!pass.empty()) break; }
            auto pIt = passUMap.find(pass);
            if (pIt == passUMap.end()) { cout << "Пассажир не найден!\n"; break; }
            if (pIt->second.getTripId1() != 0) {
                cout << "У пассажира уже куплен билет на Рейс №" << pIt->second.getTripId1() << "!\n";
                break;
            }

            printTripHeader1(cout);
            for (auto it = tripsMap.begin(); it != tripsMap.end(); ++it) cout << it->second << endl;
            cout << TRIP_BORDER << "\n";

            int tId;
            while (true) {
                cout << "Введите ID рейса: ";
                if (cin >> tId && tId > 0) { cin.ignore(10000, '\n'); break; }
                cout << "Ошибка!\n"; cin.clear(); cin.ignore(10000, '\n');
            }
            auto tIt = tripsMap.find(tId);
            if (tIt == tripsMap.end()) { cout << "Рейс не найден!\n"; break; }
            if (!tIt->second.hasFreeSeats1()) {
                cout << "Отказ: на рейсе нет свободных мест!\n";
                break;
            }

            pIt->second.setTripId1(tId);
            syncAssociativeData1(tripsMap, passUMap, destMMap, boardingUMMap, ticketsMSet);
            cout << "Билет успешно оформлен! Все ассоциативные контейнеры синхронизированы.\n";
            break;
        }
        case 2: {
            string pass;
            while (true) { cout << "Введите паспорт пассажира для возврата билета: "; getline(cin, pass); if (!pass.empty()) break; }
            auto pIt = passUMap.find(pass);
            if (pIt == passUMap.end() || pIt->second.getTripId1() == 0) {
                cout << "Пассажир не найден или у него нет активного билета!\n";
                break;
            }
            pIt->second.setTripId1(0);
            syncAssociativeData1(tripsMap, passUMap, destMMap, boardingUMMap, ticketsMSet);
            cout << "Возврат оформлен! Место на рейсе освобождено.\n";
            break;
        }
        case 3: {
            printTripHeader1(cout);
            for (auto it = tripsMap.begin(); it != tripsMap.end(); ++it) cout << it->second << endl;
            cout << TRIP_BORDER << "\n";

            int tId;
            while (true) {
                cout << "Введите ID завершившегося рейса: ";
                if (cin >> tId && tId > 0) { cin.ignore(10000, '\n'); break; }
                cout << "Ошибка!\n"; cin.clear(); cin.ignore(10000, '\n');
            }
            auto tIt = tripsMap.find(tId);
            if (tIt == tripsMap.end()) { cout << "Рейс не найден!\n"; break; }

            string rDesc = tIt->second.getRoute1().getStartPoint1() + " -> " + tIt->second.getRoute1().getEndPoint1();
            int sold = tIt->second.getSoldTickets1();
            float rev = tIt->second.getRevenue1();

            CompletedTripLog1 logEntry(tId, rDesc, sold, rev);
            completedLogsUMSet.insert(logEntry);

            ofstream fout("completed_trips_report.txt", ios::app);
            fout << "=== ОТЧЕТ О ЗАВЕРШЕНИИ РЕЙСА №" << tId << " ===\n";
            fout << "Маршрут: " << rDesc << " | Перевезено: " << sold << " чел. | Выручка: " << formatFloat1(rev, 2) << " BYN\n";
            printPassHeader1(fout);
            for (auto pIt = passUMap.begin(); pIt != passUMap.end(); ++pIt) {
                if (pIt->second.getTripId1() == tId) {
                    fout << pIt->second << endl;
                    pIt->second.setTripId1(0);
                }
            }
            fout << PASS_BORDER << "\n\n";
            fout.close();

            syncAssociativeData1(tripsMap, passUMap, destMMap, boardingUMMap, ticketsMSet);
            cout << "Рейс №" << tId << " завершен! Запись добавлена в unordered_multiset и файл completed_trips_report.txt.\n";
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

    unordered_set<Station1, StationHasher1> stationsUSet;
    stationsUSet.insert(Station1("Минск", "Минская область"));
    stationsUSet.insert(Station1("Брест", "Брестская область"));
    stationsUSet.insert(Station1("Гродно", "Гродненская область"));
    stationsUSet.insert(Station1("Гомель", "Гомельская область"));

    map<int, Trip1> tripsMap;
    tripsMap.insert(make_pair(101, Trip1(101, Route1("Минск", "Брест", 348.0f), "1245 AB-7", 44, 35.50f, 0)));
    tripsMap.insert(make_pair(102, Trip1(102, Route1("Минск", "Гродно", 278.0f), "8890 KE-7", 50, 28.00f, 0)));
    tripsMap.insert(make_pair(103, Trip1(103, Route1("Минск", "Брест", 355.0f), "3311 OP-7", 48, 32.00f, 0)));

    unordered_map<string, Passenger1> passUMap;
    passUMap.insert(make_pair("KB4455667", Passenger1("Кузнецов Д.М.", 29, "KB4455667", 101)));
    passUMap.insert(make_pair("AB8899001", Passenger1("Васильев О.С.", 36, "AB8899001", 101)));
    passUMap.insert(make_pair("EK3322114", Passenger1("Морозова Т.Н.", 22, "EK3322114", 102)));
    passUMap.insert(make_pair("MH7788992", Passenger1("Новиков В.П.", 48, "MH7788992", 0)));

    multimap<string, Trip1> destMMap;
    multiset<TicketRecord1> ticketsMSet;
    unordered_multimap<int, Passenger1> boardingUMMap;

    unordered_multiset<CompletedTripLog1, CompletedLogHasher1> completedLogsUMSet;
    completedLogsUMSet.insert(CompletedTripLog1(101, "Минск -> Брест", 40, 1420.0f));

    syncAssociativeData1(tripsMap, passUMap, destMMap, boardingUMMap, ticketsMSet);

    int mainChoice = -1;
    while (mainChoice != 0) {
        cout << "\n====================================================================\n";
        cout << "   ЛР №4. УЧЕТ ПАССАЖИРСКИХ ПЕРЕВОЗОК (8 АССОЦИАТИВНЫХ КОНТЕЙНЕРОВ)\n";
        cout << "====================================================================\n";
        cout << "1. Расписание рейсов         [контейнер std::map]\n";
        cout << "2. База пассажиров           [контейнер std::unordered_map]\n";
        cout << "3. Реестр автопарка          [контейнер std::set]\n";
        cout << "4. Маршруты, билеты, станции [multimap, multiset, unordered_*]\n";
        cout << "5. Касса билетов и завершение рейсов (с возобновлением данных)\n";
        cout << "6. Полный сводный отчет по всем 8 контейнерам (Экран + Файл)\n";
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
            menuMapTrips1(tripsMap, busSet, passUMap, destMMap, boardingUMMap, ticketsMSet);
            break;
        case 2:
            menuUnorderedMapPassengers1(passUMap, tripsMap, destMMap, boardingUMMap, ticketsMSet);
            break;
        case 3:
            menuSetBuses1(busSet);
            break;
        case 4:
            menuOtherContainers1(destMMap, ticketsMSet, stationsUSet, boardingUMMap, completedLogsUMSet);
            break;
        case 5:
            menuTicketOffice1(tripsMap, passUMap, destMMap, boardingUMMap, ticketsMSet, completedLogsUMSet);
            break;
        case 6: {
            ofstream fout("full_associative_report.txt");

            cout << "\n>>> 1. std::map<int, Trip> (Расписание рейсов) <<<\n";
            fout << ">>> 1. std::map<int, Trip> (Расписание рейсов) <<<\n";
            printTripHeader1(cout); printTripHeader1(fout);
            for (auto it = tripsMap.begin(); it != tripsMap.end(); ++it) {
                cout << it->second << endl; fout << it->second << endl;
            }
            cout << TRIP_BORDER << "\n"; fout << TRIP_BORDER << "\n\n";

            cout << "\n>>> 2. std::unordered_map<string, Passenger> (Пассажиры) <<<\n";
            fout << ">>> 2. std::unordered_map<string, Passenger> (Пассажиры) <<<\n";
            printPassHeader1(cout); printPassHeader1(fout);
            for (auto it = passUMap.begin(); it != passUMap.end(); ++it) {
                cout << it->second << endl; fout << it->second << endl;
            }
            cout << PASS_BORDER << "\n"; fout << PASS_BORDER << "\n\n";

            cout << "\n>>> 3. std::set<Bus> (Автопарк) <<<\n";
            fout << ">>> 3. std::set<Bus> (Автопарк) <<<\n";
            printBusHeader1(cout); printBusHeader1(fout);
            for (auto it = busSet.begin(); it != busSet.end(); ++it) {
                cout << *it << endl; fout << *it << endl;
            }
            cout << BUS_BORDER << "\n"; fout << BUS_BORDER << "\n\n";

            cout << "\n>>> 4. std::multimap<string, Trip> (Рейсы по городам прибытия) <<<\n";
            fout << ">>> 4. std::multimap<string, Trip> (Рейсы по городам прибытия) <<<\n";
            printTripHeader1(cout); printTripHeader1(fout);
            for (auto it = destMMap.begin(); it != destMMap.end(); ++it) {
                cout << it->second << endl; fout << it->second << endl;
            }
            cout << TRIP_BORDER << "\n"; fout << TRIP_BORDER << "\n\n";

            cout << "\n>>> 5. std::multiset<TicketRecord> (Проданные билеты по цене) <<<\n";
            fout << ">>> 5. std::multiset<TicketRecord> (Проданные билеты по цене) <<<\n";
            printTicketHeader1(cout); printTicketHeader1(fout);
            for (auto it = ticketsMSet.begin(); it != ticketsMSet.end(); ++it) {
                cout << *it << endl; fout << *it << endl;
            }
            cout << TICKET_BORDER << "\n"; fout << TICKET_BORDER << "\n\n";

            cout << "\n>>> 6. std::unordered_set<Station> (Станции сети) <<<\n";
            fout << ">>> 6. std::unordered_set<Station> (Станции сети) <<<\n";
            printStationHeader1(cout); printStationHeader1(fout);
            for (auto it = stationsUSet.begin(); it != stationsUSet.end(); ++it) {
                cout << *it << endl; fout << *it << endl;
            }
            cout << STATION_BORDER << "\n"; fout << STATION_BORDER << "\n\n";

            cout << "\n>>> 7. std::unordered_multimap<int, Passenger> (Посадочная ведомость) <<<\n";
            fout << ">>> 7. std::unordered_multimap<int, Passenger> (Посадочная ведомость) <<<\n";
            printPassHeader1(cout); printPassHeader1(fout);
            for (auto it = boardingUMMap.begin(); it != boardingUMMap.end(); ++it) {
                cout << it->second << endl; fout << it->second << endl;
            }
            cout << PASS_BORDER << "\n"; fout << PASS_BORDER << "\n\n";

            cout << "\n>>> 8. std::unordered_multiset<CompletedTripLog> (Архив завершенных рейсов) <<<\n";
            fout << ">>> 8. std::unordered_multiset<CompletedTripLog> (Архив завершенных рейсов) <<<\n";
            printLogHeader1(cout); printLogHeader1(fout);
            for (auto it = completedLogsUMSet.begin(); it != completedLogsUMSet.end(); ++it) {
                cout << *it << endl; fout << *it << endl;
            }
            cout << LOG_BORDER << "\n"; fout << LOG_BORDER << "\n";

            fout.close();
            cout << "\nПолный отчет по всем 8 контейнерам сохранен в full_associative_report.txt!\n";
            break;
        }
        case 0:
            break;
        }
    }

}
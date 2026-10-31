#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include "2lab.h"

using namespace std;

void copyStrToChar2(char* dest, const string& src, int maxLen) {
    int i = 0;
    for (; i < (int)src.length() && i < maxLen - 1; ++i) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
}

int padW2(const string& s, int width) {
    int extraBytes = 0;
    for (int i = 0; i < (int)s.length(); ++i) {
        unsigned char c = s[i];
        if ((c & 0xC0) == 0x80) {
            extraBytes++;
        }
    }
    return width + extraBytes;
}

void clearInputStream2() {
    if (cin.fail() || cin.bad()) {
        cin.clear();
    }
    cin.ignore(10000, '\n');
}

void checkStreamStatus2(ios& stream) {
    if (stream.eof()) {
        cout << "[Статус потока]: Достигнут конец потока (eofbit = 1).\n";
    }
    if (stream.bad()) {
        cout << "[Ошибка потока]: Потеря целостности буфера (badbit = 1, код rdstate = " << stream.rdstate() << ")!\n";
    }
    if (stream.fail() && !stream.eof()) {
        cout << "[Ошибка потока]: Ошибка операции ввода/вывода (failbit = 1, код rdstate = " << stream.rdstate() << ").\n";
    }
}

struct OrderRecord2 {
    int catalogIndex;
    char productName[50];
    char brandName[30];
    char country[30];
    double price;
    int quantity;
};

class ProductInfo2 {
protected:
    string brandName;
    string country;
public:
    ProductInfo2() : brandName("Неизвестно"), country("Неизвестно") {}
    ProductInfo2(string b, string c) : brandName(b), country(c) {}
    virtual ~ProductInfo2() {}

    void setBrandName(string b) { brandName = b; }
    void setCountry(string c) { country = c; }
    string getBrandName() { return brandName; }
    string getCountry() { return country; }
};

class Product2 : public ProductInfo2 {
private:
    string productName;
    double price;
    int stockQuantity;
public:
    Product2() : ProductInfo2(), productName("Без названия"), price(0.0), stockQuantity(0) {}
    Product2(string pName, double pr, string brand, string cntr, int stock)
        : ProductInfo2(brand, cntr), productName(pName), price(pr), stockQuantity(stock) {}

    void setProductName(string pName) { productName = pName; }
    void setPrice(double pr) { price = pr; }
    void setStock(int s) { stockQuantity = s; }

    string getProductName() { return productName; }
    double getPrice() { return price; }
    int getStock() { return stockQuantity; }
};

class Customer2 {
private:
    string fullName;
    string phone;
public:
    Customer2() : fullName("Аноним"), phone("-") {}
    Customer2(string name, string ph) : fullName(name), phone(ph) {}

    void setFullName(string name) { fullName = name; }
    void setPhone(string ph) { phone = ph; }
    string getFullName() { return fullName; }
    string getPhone() { return phone; }

    friend istream& operator>>(istream& s, Customer2& c);
    friend ostream& operator<<(ostream& s, Customer2& c);
};

istream& operator>>(istream& s, Customer2& c) {
    if (&s == &cin) {
        cout << "Введите ФИО покупателя: ";
        if (s.peek() == '\n') s.ignore();
        while (getline(s, c.fullName) && c.fullName.empty()) {
            checkStreamStatus2(s);
            cout << "Строка не может быть пустой! Повторите ввод: ";
        }
        cout << "Введите телефон покупателя: ";
        while (getline(s, c.phone) && c.phone.empty()) {
            checkStreamStatus2(s);
            cout << "Строка не может быть пустой! Повторите ввод: ";
        }
    }
    else {
        getline(s, c.fullName);
        getline(s, c.phone);
    }
    return s;
}

ostream& operator<<(ostream& s, Customer2& c) {
    if (&s == &cout) {
        s << c.fullName << " (Тел: " << c.phone << ")";
    }
    else {
        s << c.fullName << "\n" << c.phone << "\n";
    }
    return s;
}

class OrderDetails2 {
private:
    Product2* product;
    int quantity;
    int catalogIndex;
public:
    static OrderRecord2 rec;

    OrderDetails2() : product(nullptr), quantity(0), catalogIndex(-1) {}
    OrderDetails2(Product2* p, int q, int cIdx) : product(p), quantity(q), catalogIndex(cIdx) {}

    void setProduct(Product2* p) { product = p; }
    void setQuantity(int q) { quantity = q; }
    void setCatalogIndex(int idx) { catalogIndex = idx; }

    Product2* getProduct() { return product; }
    int getQuantity() { return quantity; }
    int getCatalogIndex() { return catalogIndex; }

    double getTotalPositionPrice() {
        return product ? product->getPrice() * quantity : 0.0;
    }

    void zipstruct() {
        rec.catalogIndex = catalogIndex;
        rec.quantity = quantity;
        if (product) {
            copyStrToChar2(rec.productName, product->getProductName(), 50);
            copyStrToChar2(rec.brandName, product->getBrandName(), 30);
            copyStrToChar2(rec.country, product->getCountry(), 30);
            rec.price = product->getPrice();
        }
        else {
            copyStrToChar2(rec.productName, "-", 50);
            copyStrToChar2(rec.brandName, "-", 30);
            copyStrToChar2(rec.country, "-", 30);
            rec.price = 0.0;
        }
    }

    void fromstruct(Product2 catalog[], int catalogSize) {
        catalogIndex = rec.catalogIndex;
        quantity = rec.quantity;
        if (catalogIndex >= 0 && catalogIndex < catalogSize) {
            product = &catalog[catalogIndex];
        }
        else {
            product = nullptr;
        }
    }

    friend istream& operator>>(istream& s, OrderDetails2& od);
    friend ostream& operator<<(ostream& s, OrderDetails2& od);
};

OrderRecord2 OrderDetails2::rec;

istream& operator>>(istream& s, OrderDetails2& od) {
    s >> od.catalogIndex >> od.quantity;
    return s;
}

ostream& operator<<(ostream& s, OrderDetails2& od) {
    Product2* p = od.getProduct();
    string pName = p ? p->getProductName() : string(OrderDetails2::rec.productName);
    string bName = p ? p->getBrandName() : string(OrderDetails2::rec.brandName);
    string cName = p ? p->getCountry() : string(OrderDetails2::rec.country);
    double pr = p ? p->getPrice() : OrderDetails2::rec.price;
    double sum = pr * od.getQuantity();

    s.setf(ios::left, ios::adjustfield);
    s << setw(padW2(pName, 24)) << pName << " | "
        << setw(padW2(bName, 12)) << bName << " | "
        << setw(padW2(cName, 12)) << cName << " | ";

    s.setf(ios::right, ios::adjustfield);
    s.setf(ios::fixed, ios::floatfield);
    s.setf(ios::showpoint);
    s << setw(10) << setprecision(2) << pr << " | "
        << setw(8) << od.getQuantity() << " | "
        << setw(12) << setprecision(2) << sum;

    s.unsetf(ios::adjustfield);
    return s;
}

class Order2 {
private:
    int orderId;
    Customer2 customer;
    OrderDetails2 items[100];
    int count;
    string dataFilename;
    string customerFilename;

public:
    Order2(int id, Customer2 c, string dFile = "order_items2.dat", string cFile = "customer2.txt")
        : orderId(id), customer(c), count(0), dataFilename(dFile), customerFilename(cFile) {}

    int getCount() {
        return count;
    }
    Customer2& getCustomer() {
        return customer;
    }

    OrderDetails2 getItem(int index) {
        if (index >= 0 && index < count) return items[index];
        return OrderDetails2();
    }

    void saveCustomerToFile() {
        ofstream outFile;
        outFile.open(customerFilename.c_str(), ios::out | ios::trunc);
        if (!outFile.is_open() || !outFile.good()) {
            cout << "Ошибка открытия файла покупателя!\n";
            checkStreamStatus2(outFile);
            return;
        }
        outFile << customer;
        outFile.close();
    }

    void loadCustomerFromFile() {
        ifstream inFile;
        inFile.open(customerFilename.c_str(), ios::in);
        if (inFile.is_open() && inFile.good()) {
            inFile >> customer;
            inFile.close();
        }
    }

    int getFileRecordCount() {
        ifstream inf;
        inf.open(dataFilename.c_str(), ios::in | ios::binary);
        if (!inf.is_open() || !inf.good()) {
            return 0;
        }
        inf.seekg(0, ios::end);
        int endPos = (int)inf.tellg();
        inf.close();
        if (endPos <= 0) return 0;
        return endPos / sizeof(OrderRecord2);
    }

    void syncToFile() {
        ofstream file;
        file.open(dataFilename.c_str(), ios::out | ios::trunc | ios::binary);
        if (!file.is_open() || file.bad()) {
            cout << "Невозможно открыть бинарный файл для записи!\n";
            checkStreamStatus2(file);
            return;
        }
        file.seekp(0, ios::beg);
        for (int i = 0; i < count; ++i) {
            items[i].zipstruct();
            file.write(reinterpret_cast<char*>(&OrderDetails2::rec), sizeof(OrderRecord2));
            if (file.fail() || file.bad()) {
                cout << "Ошибка при записи структуры в файл!\n";
                checkStreamStatus2(file);
                file.close();
                return;
            }
        }
        file.close();
    }

    void loadFromFile(Product2 catalog[], int catalogSize) {
        ifstream file;
        file.open(dataFilename.c_str(), ios::in | ios::binary);
        if (!file.is_open() || !file.good()) {
            return;
        }

        for (int i = 0; i < count; ++i) {
            Product2* p = items[i].getProduct();
            if (p) p->setStock(p->getStock() + items[i].getQuantity());
        }
        count = 0;

        file.seekg(0, ios::beg);
        while (file.good()) {
            file.read(reinterpret_cast<char*>(&OrderDetails2::rec), sizeof(OrderRecord2));
            if (file.eof()) {
                break;
            }
            if (file.fail() || file.bad()) {
                checkStreamStatus2(file);
                break;
            }
            if (count < 100) {
                OrderDetails2 od;
                od.fromstruct(catalog, catalogSize);
                Product2* p = od.getProduct();
                if (p) {
                    int take = od.getQuantity();
                    if (take > p->getStock()) take = p->getStock();
                    p->setStock(p->getStock() - take);
                    od.setQuantity(take);
                }
                items[count++] = od;
            }
        }
        file.close();
    }

    bool addItemToFile(Product2 catalog[], int prodIndex, int qty) {
        Product2* p = &catalog[prodIndex];
        if (qty > p->getStock()) {
            cout << "Ошибка: недостаточно товара на складе! Доступно: " << p->getStock() << " шт.\n";
            return false;
        }

        for (int i = 0; i < count; ++i) {
            if (items[i].getProduct() == p) {
                items[i].setQuantity(items[i].getQuantity() + qty);
                p->setStock(p->getStock() - qty);
                syncToFile();
                return true;
            }
        }

        if (count >= 100) {
            cout << "Ошибка: превышен лимит позиций в заказе!\n";
            return false;
        }

        items[count] = OrderDetails2(p, qty, prodIndex);
        p->setStock(p->getStock() - qty);

        ofstream outFile;
        outFile.open(dataFilename.c_str(), ios::app | ios::out | ios::binary);
        if (!outFile.is_open() || !outFile.good()) {
            cout << "Ошибка открытия файла для дозаписи!\n";
            checkStreamStatus2(outFile);
            return false;
        }
        items[count].zipstruct();
        outFile.write(reinterpret_cast<char*>(&OrderDetails2::rec), sizeof(OrderRecord2));
        checkStreamStatus2(outFile);
        outFile.close();

        count++;
        return true;
    }

    bool removeItemFromFile(int index) {
        if (index < 0 || index >= count) return false;

        Product2* p = items[index].getProduct();
        if (p) {
            p->setStock(p->getStock() + items[index].getQuantity());
        }

        for (int i = index; i < count - 1; ++i) {
            items[i] = items[i + 1];
        }
        count--;
        syncToFile();
        return true;
    }

    bool editItemInFile(int index, Product2 catalog[], int newProdIndex, int newQty) {
        if (index < 0 || index >= count) return false;

        Product2* newP = &catalog[newProdIndex];
        Product2* oldP = items[index].getProduct();
        int oldQty = items[index].getQuantity();

        if (oldP) {
            oldP->setStock(oldP->getStock() + oldQty);
        }

        if (newQty > newP->getStock()) {
            cout << "Ошибка: недостаточно товара на складе! Максимально доступно: " << newP->getStock() << " шт.\n";
            if (oldP) {
                oldP->setStock(oldP->getStock() - oldQty);
            }
            return false;
        }

        newP->setStock(newP->getStock() - newQty);
        items[index].setProduct(newP);
        items[index].setQuantity(newQty);
        items[index].setCatalogIndex(newProdIndex);

        fstream file;
        file.open(dataFilename.c_str(), ios::in | ios::out | ios::binary);
        if (file.is_open() && file.good()) {
            int position = index * sizeof(OrderRecord2);
            file.seekp(position, ios::beg);
            items[index].zipstruct();
            file.write(reinterpret_cast<char*>(&OrderDetails2::rec), sizeof(OrderRecord2));
            checkStreamStatus2(file);
            file.close();
        }
        else {
            syncToFile();
        }
        return true;
    }

    void sortInFileByName() {
        for (int i = 0; i < count - 1; ++i) {
            for (int j = 0; j < count - i - 1; ++j) {
                if (items[j].getProduct()->getProductName() > items[j + 1].getProduct()->getProductName()) {
                    OrderDetails2 temp = items[j];
                    items[j] = items[j + 1];
                    items[j + 1] = temp;
                }
            }
        }
        syncToFile();
    }

    void searchInFileByName(string searchName) {
        ifstream inf;
        inf.open(dataFilename.c_str(), ios::in | ios::binary);
        if (!inf.is_open() || !inf.good()) {
            cout << "Файл заказа еще не создан или недоступен!\n";
            checkStreamStatus2(inf);
            return;
        }

        inf.seekg(0, ios::end);
        int totalBytes = (int)inf.tellg();
        int n = totalBytes / sizeof(OrderRecord2);
        cout << "Всего записей в файле: " << n << " (размер файла: " << totalBytes << " байт)\n";

        inf.seekg(0, ios::beg);
        bool found = false;
        int posNum = 1;

        while (!inf.eof()) {
            inf.read(reinterpret_cast<char*>(&OrderDetails2::rec), sizeof(OrderRecord2));
            if (inf.eof()) break;
            if (inf.fail() || inf.bad()) {
                checkStreamStatus2(inf);
                break;
            }
            if (string(OrderDetails2::rec.productName) == searchName) {
                cout << "Найдена позиция №" << posNum << " в файле:\n";
                cout << "  Товар: " << OrderDetails2::rec.productName
                    << " | Бренд: " << OrderDetails2::rec.brandName
                    << " | Цена: " << OrderDetails2::rec.price
                    << " $ | Кол-во: " << OrderDetails2::rec.quantity << "\n";
                found = true;
            }
            posNum++;
        }

        if (!found) {
            cout << "Товар с названием '" << searchName << "' в файле не найден.\n";
        }
        inf.close();
    }

    void searchInFileByRecordNumber(Product2 catalog[], int catalogSize) {
        ifstream infile;
        infile.open(dataFilename.c_str(), ios::in | ios::binary);
        if (!infile.is_open() || !infile.good()) {
            cout << "Файл заказа пуст или не существует!\n";
            return;
        }

        infile.seekg(0, ios::end);
        int endposition = (int)infile.tellg();
        int n = endposition / sizeof(OrderRecord2);

        if (n == 0) {
            cout << "В файле 0 записей.\n";
            infile.close();
            return;
        }

        cout << "В файле хранится записей: " << n << "\n";
        cout << "Введите номер записи для прямого чтения из файла (1-" << n << ", или 0 для отмены): ";
        int recNum;
        cin >> recNum;
        checkStreamStatus2(cin);
        while (cin.fail() || recNum < 0 || recNum > n) {
            cout << "Некорректный ввод! Введите число от 1 до " << n << " (или 0 для отмены): ";
            clearInputStream2();
            cin >> recNum;
            checkStreamStatus2(cin);
        }
        if (recNum == 0) {
            cout << "Поиск отменен.\n";
            infile.close();
            return;
        }

        int position = (recNum - 1) * sizeof(OrderRecord2);
        infile.seekg(position, ios::beg);
        infile.read(reinterpret_cast<char*>(&OrderDetails2::rec), sizeof(OrderRecord2));
        if (!infile.fail() && !infile.bad()) {
            OrderDetails2 tempOd;
            tempOd.fromstruct(catalog, catalogSize);
            cout << "\nСчитана запись №" << recNum << " (смещение " << position << " байт от начала файла):\n";
            cout << "| " << tempOd << " |\n";
        }
        else {
            checkStreamStatus2(infile);
        }
        infile.close();
    }

    double calculateTotalOrderPrice() {
        double total = 0.0;
        for (int i = 0; i < count; ++i) {
            total += items[i].getTotalPositionPrice();
        }
        return total;
    }

    void printTable(ostream& os) {
        const string eqLine(102, '=');
        const string dashLine(102, '-');

        os << "\n" << eqLine << "\n";
        os << " ЗАКАЗ №" << orderId << " | Покупатель: " << customer.getFullName()
            << " (Тел: " << customer.getPhone() << ") [Записей в файле: " << getFileRecordCount() << "]\n";
        os << eqLine << "\n";

        os << "| ";
        os.setf(ios::left, ios::adjustfield);
        os << setw(padW2("№", 3)) << "№" << " | "
            << setw(padW2("Наименование", 24)) << "Наименование" << " | "
            << setw(padW2("Бренд", 12)) << "Бренд" << " | "
            << setw(padW2("Страна", 12)) << "Страна" << " | ";

        os.setf(ios::right, ios::adjustfield);
        os << setw(padW2("Цена ($)", 10)) << "Цена ($)" << " | "
            << setw(padW2("Кол-во", 8)) << "Кол-во" << " | "
            << setw(padW2("Сумма ($)", 12)) << "Сумма ($)" << " |\n";

        os << dashLine << "\n";

        os.setf(ios::fixed, ios::floatfield);
        os.setf(ios::showpoint);

        if (count == 0) {
            os.setf(ios::left, ios::adjustfield);
            string emptyMsg = "Заказ пуст";
            os << "| " << setw(padW2(emptyMsg, 98)) << emptyMsg << " |\n";
        }
        else {
            for (int i = 0; i < count; ++i) {
                os << "| ";
                os.setf(ios::left, ios::adjustfield);
                os << setw(3) << (i + 1) << " | ";
                os << items[i] << " |\n";
            }
        }

        os << dashLine << "\n";

        os << "| ";
        os.setf(ios::right, ios::adjustfield);
        os.setf(ios::fixed, ios::floatfield);
        os.setf(ios::showpoint);
        string totalLabel = "Итого к оплате:";
        os << setw(padW2(totalLabel, 83)) << totalLabel << " | "
            << setw(12) << setprecision(2) << calculateTotalOrderPrice() << " |\n";
        os << eqLine << "\n";

        os.unsetf(ios::adjustfield);
        os.unsetf(ios::floatfield);
        os.unsetf(ios::showpoint);
    }

    void viewOrderFromFile(Product2 catalog[], int catalogSize) {
        loadCustomerFromFile();
        loadFromFile(catalog, catalogSize);
        printTable(cout);
    }

    void exportReportToTxt(string filename) {
        ofstream outFile;
        outFile.open(filename.c_str(), ios::out | ios::trunc);
        if (!outFile.is_open() || !outFile.good()) {
            cout << "Ошибка открытия текстового файла для записи отчета!\n";
            checkStreamStatus2(outFile);
            return;
        }
        printTable(outFile);
        checkStreamStatus2(outFile);
        outFile.close();
        cout << "Текстовый отчет успешно сохранен в файл '" << filename << "'.\n";
    }
};

void printCatalog2(Product2 catalog[], int size) {
    const string eqLine(95, '=');
    const string dashLine(95, '-');

    cout << "\n" << eqLine << "\n";
    cout << "|                                  КАТАЛОГ ДОСТУПНЫХ ТОВАРОВ                                  |\n";
    cout << eqLine << "\n";

    cout << "| ";
    cout.setf(ios::left, ios::adjustfield);
    cout << setw(padW2("№", 3)) << "№" << " | "
        << setw(padW2("Наименование", 24)) << "Наименование" << " | "
        << setw(padW2("Бренд", 12)) << "Бренд" << " | "
        << setw(padW2("Страна", 12)) << "Страна" << " | ";

    cout.setf(ios::right, ios::adjustfield);
    cout << setw(padW2("Цена ($)", 10)) << "Цена ($)" << " | "
        << setw(padW2("В наличии", 12)) << "В наличии" << " |\n";

    cout << dashLine << "\n";

    cout.setf(ios::fixed, ios::floatfield);
    cout.setf(ios::showpoint);

    for (int i = 0; i < size; ++i) {
        string pName = catalog[i].getProductName();
        string bName = catalog[i].getBrandName();
        string cName = catalog[i].getCountry();

        cout << "| ";
        cout.setf(ios::left, ios::adjustfield);
        cout << setw(3) << (i + 1) << " | "
            << setw(padW2(pName, 24)) << pName << " | "
            << setw(padW2(bName, 12)) << bName << " | "
            << setw(padW2(cName, 12)) << cName << " | ";

        cout.setf(ios::right, ios::adjustfield);
        cout << setw(10) << setprecision(2) << catalog[i].getPrice() << " | "
            << setw(12) << catalog[i].getStock() << " |\n";
    }

    cout << eqLine << "\n";

    cout.unsetf(ios::adjustfield);
    cout.unsetf(ios::floatfield);
    cout.unsetf(ios::showpoint);
}

void lab2() {
    system("CLS");

    Product2 catalog[10] = {
        Product2("Ноутбук Legion", 1200.50, "Lenovo", "Китай", 5),
        Product2("Монитор Ultra", 350.00, "LG", "Корея", 12),
        Product2("Мышь Gaming", 45.99, "Razer", "Китай", 30),
        Product2("Клавиатура Mech", 89.90, "Logitech", "Швейцария", 15),
        Product2("Наушники Pro", 150.00, "Sony", "Япония", 8),
        Product2("Смартфон Galaxy", 999.00, "Samsung", "Корея", 10),
        Product2("Планшет iPad", 799.50, "Apple", "США", 7),
        Product2("Смарт-часы Fit", 199.99, "Garmin", "США", 20),
        Product2("Внешний SSD 1TB", 120.00, "Kingston", "Тайвань", 25),
        Product2("Веб-камера Stream", 75.20, "Logitech", "Швейцария", 18)
    };


    Customer2 customer;
    Order2 myOrder(102, customer);

    ifstream checkCust("customer2.txt");
    if (checkCust.is_open() && checkCust.good()) {
        checkCust.close();
        myOrder.loadCustomerFromFile();
        myOrder.loadFromFile(catalog, 10);
        cout << "Загружены данные предыдущей сессии из файлов!\n";
        cout << "Покупатель: " << myOrder.getCustomer() << "\n";
        cout << "Хотите ввести нового покупателя и начать новый заказ? (1 - Да, 0 - Оставить текущий): ";
        int resetChoice;
        cin >> resetChoice;
        checkStreamStatus2(cin);
        while (cin.fail() || (resetChoice != 0 && resetChoice != 1)) {
            cout << "Введите 1 (новый заказ) или 0 (продолжить текущий): ";
            clearInputStream2();
            cin >> resetChoice;
            checkStreamStatus2(cin);
        }
        if (resetChoice == 1) {
            for (int i = myOrder.getCount() - 1; i >= 0; --i) {
                myOrder.removeItemFromFile(i);
            }
            cin >> myOrder.getCustomer();
            myOrder.saveCustomerToFile();
        }
    }
    else {
        cin >> myOrder.getCustomer();
        myOrder.saveCustomerToFile();
        myOrder.syncToFile();
    }

    int choice;
    do {
        cout << "\n--- МЕНЮ РАБОТЫ С ФАЙЛОМ ЗАКАЗА ---\n";
        cout << "1. Просмотр данных файла заказа\n";
        cout << "2. Добавление данных в файл\n";
        cout << "3. Удаление данных из файла \n";
        cout << "4. Редактирование данных в файле \n";
        cout << "5. Сортировка данных в файле по названию\n";
        cout << "6. Поиск данных в файле \n";
        cout << "7. Экспорт текстового отчета в файл (order_report2.txt)\n";
        cout << "0. Выход\n";

        cout << "Выберите действие: ";
        cin >> choice;
        checkStreamStatus2(cin);
        while (cin.fail() || choice < 0 || choice > 7) {
            cout << "Некорректный ввод! Введите число от 0 до 7: ";
            clearInputStream2();
            cin >> choice;
            checkStreamStatus2(cin);
        }

        switch (choice) {
        case 1:
            myOrder.viewOrderFromFile(catalog, 10);
            break;

        case 2: {
            printCatalog2(catalog, 10);
            int prodNum, qty;

            cout << "Выберите номер товара из каталога (1-10, или 0 для отмены): ";
            cin >> prodNum;
            checkStreamStatus2(cin);
            while (cin.fail() || prodNum < 0 || prodNum > 10) {
                cout << "Некорректный выбор! Введите номер от 1 до 10 (или 0 для отмены): ";
                clearInputStream2();
                cin >> prodNum;
                checkStreamStatus2(cin);
            }
            if (prodNum == 0) {
                cout << "Действие отменено.\n";
                break;
            }

            int available = catalog[prodNum - 1].getStock();
            if (available == 0) {
                cout << "К сожалению, данного товара сейчас нет в наличии!\n";
                break;
            }

            cout << "Введите количество (доступно: " << available << ", или 0 для отмены): ";
            cin >> qty;
            checkStreamStatus2(cin);
            while (cin.fail() || qty < 0 || qty > available) {
                if (!cin.fail() && qty > available) {
                    cout << "Ошибка: вы хотите взять больше, чем есть на складе! Доступно максимум " << available << " шт.\n";
                }
                cout << "Введите корректное количество (от 1 до " << available << ", или 0 для отмены): ";
                clearInputStream2();
                cin >> qty;
                checkStreamStatus2(cin);
            }
            if (qty == 0) {
                cout << "Добавление товара отменено.\n";
                break;
            }

            if (myOrder.addItemToFile(catalog, prodNum - 1, qty)) {
                cout << "Товар успешно добавлен и записан в файл!\n";
            }
            break;
        }

        case 3: {
            if (myOrder.getCount() == 0) {
                cout << "Файл заказа пуст!\n";
                break;
            }
            myOrder.viewOrderFromFile(catalog, 10);
            int idx;
            cout << "Введите номер позиции для удаления из файла (1-" << myOrder.getCount() << ", или 0 для отмены): ";
            cin >> idx;
            checkStreamStatus2(cin);
            while (cin.fail() || idx < 0 || idx > myOrder.getCount()) {
                cout << "Некорректный ввод! Введите число от 1 до " << myOrder.getCount() << " (или 0 для отмены): ";
                clearInputStream2();
                cin >> idx;
                checkStreamStatus2(cin);
            }
            if (idx == 0) {
                cout << "Удаление отменено.\n";
                break;
            }
            if (myOrder.removeItemFromFile(idx - 1)) {
                cout << "Позиция удалена из файла (товар возвращен на склад)!\n";
            }
            break;
        }

        case 4: {
            if (myOrder.getCount() == 0) {
                cout << "Файл заказа пуст!\n";
                break;
            }
            myOrder.viewOrderFromFile(catalog, 10);
            int idx;
            cout << "Введите номер позиции в файле для редактирования (1-" << myOrder.getCount() << ", или 0 для отмены): ";
            cin >> idx;
            checkStreamStatus2(cin);
            while (cin.fail() || idx < 0 || idx > myOrder.getCount()) {
                cout << "Некорректный ввод! Введите число от 1 до " << myOrder.getCount() << " (или 0 для отмены): ";
                clearInputStream2();
                cin >> idx;
                checkStreamStatus2(cin);
            }
            if (idx == 0) {
                cout << "Редактирование отменено.\n";
                break;
            }

            printCatalog2(catalog, 10);
            int prodNum, qty;

            cout << "Выберите новый товар из каталога (1-10, или 0 для отмены): ";
            cin >> prodNum;
            checkStreamStatus2(cin);
            while (cin.fail() || prodNum < 0 || prodNum > 10) {
                cout << "Некорректный выбор! Введите номер от 1 до 10 (или 0 для отмены): ";
                clearInputStream2();
                cin >> prodNum;
                checkStreamStatus2(cin);
            }
            if (prodNum == 0) {
                cout << "Редактирование отменено.\n";
                break;
            }

            Product2* selectedProduct = &catalog[prodNum - 1];
            OrderDetails2 currentItem = myOrder.getItem(idx - 1);

            int maxAvailable = selectedProduct->getStock();
            if (currentItem.getProduct() == selectedProduct) {
                maxAvailable += currentItem.getQuantity();
            }

            if (maxAvailable == 0) {
                cout << "Выбранного товара нет в наличии на складе!\n";
                break;
            }

            cout << "Введите новое количество (максимально доступно: " << maxAvailable << ", или 0 для отмены): ";
            cin >> qty;
            checkStreamStatus2(cin);
            while (cin.fail() || qty < 0 || qty > maxAvailable) {
                if (!cin.fail() && qty > maxAvailable) {
                    cout << "Ошибка: на складе нет такого количества! Доступно максимум: " << maxAvailable << " шт.\n";
                }
                cout << "Введите число от 1 до " << maxAvailable << " (или 0 для отмены): ";
                clearInputStream2();
                cin >> qty;
                checkStreamStatus2(cin);
            }
            if (qty == 0) {
                cout << "Редактирование отменено.\n";
                break;
            }

            if (myOrder.editItemInFile(idx - 1, catalog, prodNum - 1, qty)) {
                cout << "Позиция успешно перезаписана в файле!\n";
            }
            break;
        }

        case 5: {
            if (myOrder.getCount() == 0) {
                cout << "Файл заказа пуст, сортировать нечего!\n";
                break;
            }
            int confirm;
            cout << "Отсортировать записи в файле по названию? (1 - Да, 0 - Отмена): ";
            cin >> confirm;
            checkStreamStatus2(cin);
            while (cin.fail() || (confirm != 0 && confirm != 1)) {
                cout << "Введите 1 для подтверждения или 0 для отмены: ";
                clearInputStream2();
                cin >> confirm;
                checkStreamStatus2(cin);
            }
            if (confirm == 0) {
                cout << "Сортировка отменена.\n";
                break;
            }
            myOrder.sortInFileByName();
            cout << "Записи отсортированы и перезаписаны в файл.\n";
            break;
        }

        case 6: {
            if (myOrder.getCount() == 0) {
                cout << "Файл заказа пуст!\n";
                break;
            }
            int searchMode;
            cout << "Выберите способ поиска в файле:\n";
            cout << "1. По названию товара\n";
            cout << "2. По порядковому номеру записи (через seekg / tellg)\n";
            cout << "0. Отмена\n";
            cout << "Ваш выбор: ";
            cin >> searchMode;
            checkStreamStatus2(cin);
            while (cin.fail() || searchMode < 0 || searchMode > 2) {
                cout << "Введите 1, 2 или 0 для отмены: ";
                clearInputStream2();
                cin >> searchMode;
                checkStreamStatus2(cin);
            }
            if (searchMode == 0) {
                cout << "Поиск отменен.\n";
                break;
            }

            if (searchMode == 1) {
                string searchName;
                cout << "Введите название товара для поиска в файле (или 0 для отмены): ";
                if (cin.peek() == '\n') cin.ignore();
                while (getline(cin, searchName) && searchName.empty()) {
                    checkStreamStatus2(cin);
                    cout << "Строка не может быть пустой! Повторите ввод (или 0 для отмены): ";
                }
                if (searchName == "0") {
                    cout << "Поиск отменен.\n";
                    break;
                }
                myOrder.searchInFileByName(searchName);
            }
            else {
                myOrder.searchInFileByRecordNumber(catalog, 10);
            }
            break;
        }

        case 7: {
            int confirm;
            cout << "Экспортировать таблицу заказа в текстовый файл 'order_report2.txt'? (1 - Да, 0 - Отмена): ";
            cin >> confirm;
            checkStreamStatus2(cin);
            while (cin.fail() || (confirm != 0 && confirm != 1)) {
                cout << "Введите 1 для сохранения или 0 для отмены: ";
                clearInputStream2();
                cin >> confirm;
                checkStreamStatus2(cin);
            }
            if (confirm == 0) {
                cout << "Сохранение отменено.\n";
                break;
            }
            myOrder.exportReportToTxt("order_report2.txt");
            break;
        }

        case 0:
            cout << "Завершение работы лабораторной работы №2.\n";
            break;
        }
    } while (choice != 0);
}

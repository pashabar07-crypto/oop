#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <clocale>

using namespace std;

int padW(const string& s, int width) {
    int extraBytes = 0;
    for (unsigned char c : s) {
        if ((c & 0xC0) == 0x80) {
            extraBytes++;
        }
    }
    return width + extraBytes;
}

void clearInputStream() {
    if (cin.fail() || cin.bad()) {
        cin.clear();
    }
    cin.ignore(10000, '\n');
}

void checkStreamStatus(ios& stream) {
    if (stream.eof()) {
        cout << "[Предупреждение]: Достигнут конец потока (EOF).\n";
    }
    if (stream.bad()) {
        cout << "[Ошибка]: Потеря целостности буфера потока (badbit)!\n";
    }
    if (stream.fail() && !stream.eof()) {
        cout << "[Ошибка]: Ошибка форматирования или ввода/вывода (failbit).\n";
    }
}

class ProductInfo {
protected:
    string brandName;
    string country;
public:
    ProductInfo() : brandName("Неизвестно"), country("Неизвестно") {}
    ProductInfo(string b, string c) : brandName(b), country(c) {}
    virtual ~ProductInfo() {}

    void setBrandName(string b) { brandName = b; }
    void setCountry(string c) { country = c; }
    string getBrandName() { return brandName; }
    string getCountry() { return country; }
};

class Product : public ProductInfo {
private:
    string productName;
    double price;
    int stockQuantity;
public:
    Product() : ProductInfo(), productName("Без названия"), price(0.0), stockQuantity(0) {}
    Product(string pName, double pr, string brand, string cntr, int stock)
        : ProductInfo(brand, cntr), productName(pName), price(pr), stockQuantity(stock) {}

    void setProductName(string pName) { productName = pName; }
    void setPrice(double pr) { price = pr; }
    void setStock(int s) { stockQuantity = s; }

    string getProductName() { return productName; }
    double getPrice() { return price; }
    int getStock() { return stockQuantity; }
};

class Customer {
private:
    string fullName;
    string phone;
public:
    Customer() : fullName("Аноним"), phone("-") {}
    Customer(string name, string ph) : fullName(name), phone(ph) {}

    void setFullName(string name) { fullName = name; }
    void setPhone(string ph) { phone = ph; }
    string getFullName() { return fullName; }
    string getPhone() { return phone; }
};

class OrderDetails {
private:
    Product* product;
    int quantity;
public:
    OrderDetails() : product(nullptr), quantity(0) {}
    OrderDetails(Product* p, int q) : product(p), quantity(q) {}

    void setProduct(Product* p) { product = p; }
    void setQuantity(int q) { quantity = q; }
    Product* getProduct() { return product; }
    int getQuantity() { return quantity; }

    double getTotalPositionPrice() {
        return product ? product->getPrice() * quantity : 0.0;
    }
};

class Order {
private:
    int orderId;
    Customer customer;
    OrderDetails items[100];
    int count;

public:
    Order(int id, Customer c) : orderId(id), customer(c), count(0) {}

    int getCount() { return count; }

    OrderDetails getItem(int index) {
        if (index >= 0 && index < count) return items[index];
        return OrderDetails();
    }

    bool addItem(Product* p, int qty) {
        if (!p) return false;
        if (qty > p->getStock()) {
            cout << "Ошибка: недостаточно товара на складе! Доступно: " << p->getStock() << " шт.\n";
            return false;
        }

        for (int i = 0; i < count; ++i) {
            if (items[i].getProduct() == p) {
                items[i].setQuantity(items[i].getQuantity() + qty);
                p->setStock(p->getStock() - qty);
                return true;
            }
        }

        if (count >= 100) {
            cout << "Ошибка: превышен лимит позиций в заказе!\n";
            return false;
        }
        items[count] = OrderDetails(p, qty);
        p->setStock(p->getStock() - qty);
        count++;
        return true;
    }

    bool removeItem(int index) {
        if (index < 0 || index >= count) return false;

        Product* p = items[index].getProduct();
        if (p) {
            p->setStock(p->getStock() + items[index].getQuantity());
        }

        for (int i = index; i < count - 1; ++i) {
            items[i] = items[i + 1];
        }
        count--;
        return true;
    }

    bool editItem(int index, Product* newP, int newQty) {
        if (index < 0 || index >= count || !newP) return false;

        Product* oldP = items[index].getProduct();
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
        return true;
    }

    void sortByName() {
        for (int i = 0; i < count - 1; ++i) {
            for (int j = 0; j < count - i - 1; ++j) {
                if (items[j].getProduct()->getProductName() > items[j + 1].getProduct()->getProductName()) {
                    OrderDetails temp = items[j];
                    items[j] = items[j + 1];
                    items[j + 1] = temp;
                }
            }
        }
    }

    int findByName(string name) {
        for (int i = 0; i < count; ++i) {
            if (items[i].getProduct() && items[i].getProduct()->getProductName() == name) {
                return i;
            }
        }
        return -1;
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
            << " (Тел: " << customer.getPhone() << ")\n";
        os << eqLine << "\n";

        os << "| ";
        os.setf(ios::left, ios::adjustfield);
        os << setw(padW("№", 3)) << "№" << " | "
            << setw(padW("Наименование", 24)) << "Наименование" << " | "
            << setw(padW("Бренд", 12)) << "Бренд" << " | "
            << setw(padW("Страна", 12)) << "Страна" << " | ";

        os.setf(ios::right, ios::adjustfield);
        os << setw(padW("Цена ($)", 10)) << "Цена ($)" << " | "
            << setw(padW("Кол-во", 8)) << "Кол-во" << " | "
            << setw(padW("Сумма ($)", 12)) << "Сумма ($)" << " |\n";

        os << dashLine << "\n";

        os.setf(ios::fixed, ios::floatfield);
        os.setf(ios::showpoint);

        if (count == 0) {
            os.setf(ios::left, ios::adjustfield);
            string emptyMsg = "Заказ пуст";
            os << "| " << setw(padW(emptyMsg, 98)) << emptyMsg << " |\n";
        }
        else {
            for (int i = 0; i < count; ++i) {
                Product* p = items[i].getProduct();
                string pName = p ? p->getProductName() : "-";
                string bName = p ? p->getBrandName() : "-";
                string cName = p ? p->getCountry() : "-";

                os << "| ";
                os.setf(ios::left, ios::adjustfield);
                os << setw(3) << (i + 1) << " | "
                    << setw(padW(pName, 24)) << pName << " | "
                    << setw(padW(bName, 12)) << bName << " | "
                    << setw(padW(cName, 12)) << cName << " | ";

                os.setf(ios::right, ios::adjustfield);
                os << setw(10) << setprecision(2) << (p ? p->getPrice() : 0.0) << " | "
                    << setw(8) << items[i].getQuantity() << " | "
                    << setw(12) << setprecision(2) << items[i].getTotalPositionPrice() << " |\n";
            }
        }

        os << dashLine << "\n";

        os << "| ";
        os.setf(ios::right, ios::adjustfield);
        string totalLabel = "Итого к оплате:";
        os << setw(padW(totalLabel, 83)) << totalLabel << " | "
            << setw(12) << setprecision(2) << calculateTotalOrderPrice() << " |\n";
        os << eqLine << "\n";

        os.unsetf(ios::adjustfield);
        os.unsetf(ios::floatfield);
        os.unsetf(ios::showpoint);
    }

    void saveToFile(string filename) {
        ofstream outFile(filename.c_str());
        if (!outFile.is_open()) {
            cout << "Ошибка открытия файла для записи!\n";
            return;
        }
        printTable(outFile);
        checkStreamStatus(outFile);
        outFile.close();
        cout << "Данные заказа успешно сохранены в файл '" << filename << "'.\n";
    }
};

void printCatalog(Product catalog[], int size) {
    const string eqLine(95, '=');
    const string dashLine(95, '-');

    cout << "\n" << eqLine << "\n";
    cout << "|                                  КАТАЛОГ ДОСТУПНЫХ ТОВАРОВ                                  |\n";
    cout << eqLine << "\n";

    cout << "| ";
    cout.setf(ios::left, ios::adjustfield);
    cout << setw(padW("№", 3)) << "№" << " | "
        << setw(padW("Наименование", 24)) << "Наименование" << " | "
        << setw(padW("Бренд", 12)) << "Бренд" << " | "
        << setw(padW("Страна", 12)) << "Страна" << " | ";

    cout.setf(ios::right, ios::adjustfield);
    cout << setw(padW("Цена ($)", 10)) << "Цена ($)" << " | "
        << setw(padW("В наличии", 12)) << "В наличии" << " |\n";

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
            << setw(padW(pName, 24)) << pName << " | "
            << setw(padW(bName, 12)) << bName << " | "
            << setw(padW(cName, 12)) << cName << " | ";

        cout.setf(ios::right, ios::adjustfield);
        cout << setw(10) << setprecision(2) << catalog[i].getPrice() << " | "
            << setw(12) << catalog[i].getStock() << " |\n";
    }

    cout << eqLine << "\n";

    cout.unsetf(ios::adjustfield);
    cout.unsetf(ios::floatfield);
    cout.unsetf(ios::showpoint);
}

void lab1() {
    system("CLS");

    Product catalog[10] = {
        Product("Ноутбук Legion", 1200.50, "Lenovo", "Китай", 5),
        Product("Монитор Ultra", 350.00, "LG", "Корея", 12),
        Product("Мышь Gaming", 45.99, "Razer", "Китай", 30),
        Product("Клавиатура Mech", 89.90, "Logitech", "Швейцария", 15),
        Product("Наушники Pro", 150.00, "Sony", "Япония", 8),
        Product("Смартфон Galaxy", 999.00, "Samsung", "Корея", 10),
        Product("Планшет iPad", 799.50, "Apple", "США", 7),
        Product("Смарт-часы Fit", 199.99, "Garmin", "США", 20),
        Product("Внешний SSD 1TB", 120.00, "Kingston", "Тайвань", 25),
        Product("Веб-камера Stream", 75.20, "Logitech", "Швейцария", 18)
    };

    cout << "=== Система управления заказами магазина электроники ===\n\n";

    string custName, custPhone;

    cout << "Введите ФИО покупателя: ";
    if (cin.peek() == '\n') cin.ignore();
    while (getline(cin, custName) && custName.empty()) {
        checkStreamStatus(cin);
        cout << "Строка не может быть пустой! Повторите ввод: ";
    }

    cout << "Введите телефон покупателя: ";
    while (getline(cin, custPhone) && custPhone.empty()) {
        checkStreamStatus(cin);
        cout << "Строка не может быть пустой! Повторите ввод: ";
    }

    Customer customer(custName, custPhone);
    Order myOrder(101, customer);

    int choice;
    do {
        cout << "\n--- МЕНЮ УПРАВЛЕНИЯ ЗАКАЗОМ ---\n";
        cout << "1. Просмотр заказа (Таблица)\n";
        cout << "2. Добавить товар из каталога\n";
        cout << "3. Удалить позицию товара\n";
        cout << "4. Изменить товар/количество в позиции\n";
        cout << "5. Сортировать позиции по названию\n";
        cout << "6. Найти позицию по названию\n";
        cout << "7. Сохранить отчет в файл\n";
        cout << "0. Выход\n";

        cout << "Выберите действие: ";
        cin >> choice;
        checkStreamStatus(cin);
        while (cin.fail() || choice < 0 || choice > 7) {
            cout << "Некорректный ввод! Введите число от 0 до 7: ";
            clearInputStream();
            cin >> choice;
            checkStreamStatus(cin);
        }

        switch (choice) {
        case 1:
            myOrder.printTable(cout);
            break;
        case 2: {
            printCatalog(catalog, 10);
            int prodNum, qty;

            cout << "Выберите номер товара из каталога (1-10, или 0 для отмены): ";
            cin >> prodNum;
            checkStreamStatus(cin);
            while (cin.fail() || prodNum < 0 || prodNum > 10) {
                cout << "Некорректный выбор! Введите номер от 1 до 10 (или 0 для отмены): ";
                clearInputStream();
                cin >> prodNum;
                checkStreamStatus(cin);
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
            checkStreamStatus(cin);
            while (cin.fail() || qty < 0 || qty > available) {
                if (!cin.fail() && qty > available) {
                    cout << "Ошибка: вы хотите взять больше, чем есть на складе! Доступно максимум " << available << " шт.\n";
                }
                cout << "Введите корректное количество (от 1 до " << available << ", или 0 для отмены): ";
                clearInputStream();
                cin >> qty;
                checkStreamStatus(cin);
            }
            if (qty == 0) {
                cout << "Добавление товара отменено.\n";
                break;
            }

            if (myOrder.addItem(&catalog[prodNum - 1], qty)) {
                cout << "Товар успешно добавлен в заказ!\n";
            }
            break;
        }
        case 3: {
            if (myOrder.getCount() == 0) {
                cout << "Заказ пуст!\n";
                break;
            }
            myOrder.printTable(cout);
            int idx;
            cout << "Введите номер позиции для удаления (1-" << myOrder.getCount() << ", или 0 для отмены): ";
            cin >> idx;
            checkStreamStatus(cin);
            while (cin.fail() || idx < 0 || idx > myOrder.getCount()) {
                cout << "Некорректный ввод! Введите число от 1 до " << myOrder.getCount() << " (или 0 для отмены): ";
                clearInputStream();
                cin >> idx;
                checkStreamStatus(cin);
            }
            if (idx == 0) {
                cout << "Удаление отменено.\n";
                break;
            }
            if (myOrder.removeItem(idx - 1)) {
                cout << "Позиция удалена (товар возвращен на склад)!\n";
            }
            break;
        }
        case 4: {
            if (myOrder.getCount() == 0) {
                cout << "Заказ пуст!\n";
                break;
            }
            myOrder.printTable(cout);
            int idx;
            cout << "Введите номер позиции в заказе для изменения (1-" << myOrder.getCount() << ", или 0 для отмены): ";
            cin >> idx;
            checkStreamStatus(cin);
            while (cin.fail() || idx < 0 || idx > myOrder.getCount()) {
                cout << "Некорректный ввод! Введите число от 1 до " << myOrder.getCount() << " (или 0 для отмены): ";
                clearInputStream();
                cin >> idx;
                checkStreamStatus(cin);
            }
            if (idx == 0) {
                cout << "Изменение отменено.\n";
                break;
            }

            printCatalog(catalog, 10);
            int prodNum, qty;

            cout << "Выберите новый товар из каталога (1-10, или 0 для отмены): ";
            cin >> prodNum;
            checkStreamStatus(cin);
            while (cin.fail() || prodNum < 0 || prodNum > 10) {
                cout << "Некорректный выбор! Введите номер от 1 до 10 (или 0 для отмены): ";
                clearInputStream();
                cin >> prodNum;
                checkStreamStatus(cin);
            }
            if (prodNum == 0) {
                cout << "Изменение отменено.\n";
                break;
            }

            Product* selectedProduct = &catalog[prodNum - 1];
            OrderDetails currentItem = myOrder.getItem(idx - 1);

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
            checkStreamStatus(cin);
            while (cin.fail() || qty < 0 || qty > maxAvailable) {
                if (!cin.fail() && qty > maxAvailable) {
                    cout << "Ошибка: на складе нет такого количества! Доступно максимум: " << maxAvailable << " шт.\n";
                }
                cout << "Введите число от 1 до " << maxAvailable << " (или 0 для отмены): ";
                clearInputStream();
                cin >> qty;
                checkStreamStatus(cin);
            }
            if (qty == 0) {
                cout << "Изменение отменено.\n";
                break;
            }

            if (myOrder.editItem(idx - 1, selectedProduct, qty)) {
                cout << "Позиция успешно изменена!\n";
            }
            break;
        }
        case 5: {
            if (myOrder.getCount() == 0) {
                cout << "Заказ пуст, сортировать нечего!\n";
                break;
            }
            int confirm;
            cout << "Сортировать позиции по названию? (1 - Да, 0 - Отмена): ";
            cin >> confirm;
            checkStreamStatus(cin);
            while (cin.fail() || (confirm != 0 && confirm != 1)) {
                cout << "Введите 1 для подтверждения или 0 для отмены: ";
                clearInputStream();
                cin >> confirm;
                checkStreamStatus(cin);
            }
            if (confirm == 0) {
                cout << "Сортировка отменена.\n";
                break;
            }
            myOrder.sortByName();
            cout << "Позиции заказа отсортированы по наименованию.\n";
            break;
        }
        case 6: {
            if (myOrder.getCount() == 0) {
                cout << "Заказ пуст!\n";
                break;
            }
            string searchName;
            cout << "Введите название товара для поиска (или 0 для отмены): ";
            if (cin.peek() == '\n') cin.ignore();
            while (getline(cin, searchName) && searchName.empty()) {
                checkStreamStatus(cin);
                cout << "Строка не может быть пустой! Повторите ввод (или 0 для отмены): ";
            }
            if (searchName == "0") {
                cout << "Поиск отменен.\n";
                break;
            }

            int idx = myOrder.findByName(searchName);
            if (idx != -1) {
                cout << "Товар найден под номером позиции: " << (idx + 1) << "\n";
            }
            else {
                cout << "Товар с таким названием не найден в заказе.\n";
            }
            break;
        }
        case 7: {
            int confirm;
            cout << "Сохранить отчет в файл 'order_report.txt'? (1 - Да, 0 - Отмена): ";
            cin >> confirm;
            checkStreamStatus(cin);
            while (cin.fail() || (confirm != 0 && confirm != 1)) {
                cout << "Введите 1 для сохранения или 0 для отмены: ";
                clearInputStream();
                cin >> confirm;
                checkStreamStatus(cin);
            }
            if (confirm == 0) {
                cout << "Сохранение отменено.\n";
                break;
            }
            myOrder.saveToFile("order_report.txt");
            break;
        }
        case 0:
            cout << "Завершение работы программы.\n";
            break;
        }
    } while (choice != 0);
}

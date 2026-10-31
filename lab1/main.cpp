#include <iostream>
#include "1lab.h" 
#include "2lab.h"
#include "3lab.h"
#include "4lab.h"
using namespace std;

int main() {

    int n;
    do {
        cout << "\n=====================Меню====================\n\n";
        cout << "1. «Использование потоков ввода/вывода данных»\n";
        cout << "2. «Использование потоков файлового ввода / вывода данных» \n";
        cout << "3. «Последовательные классы контейнеры и классы - итераторы библиотеки STL» \n";
        cout << "4. «Ассоциативные классы контейнеры и классы - итераторы библиотеки STL» \n";
        cout << "0. Выход\n\n";
        cout << "=============================================\n";
        do {
            cout << "Ваш выбор: ";
            cin >> n;
            if (cin.good()) {
                if (n >= 0 && n <= 4)
                    break;
            }
            cin.clear();
            cout << "\nНеправильный ввод данных\n";
            cin.ignore(1000, '\n');


        } while (true);

        switch (n) {
        case 1: lab1(); break;
        case 2: lab2();  break;
        case 3: lab3();  break;
        case 4: lab4();  break;
        case 0: break;
        }
    } while (n!=0);

}
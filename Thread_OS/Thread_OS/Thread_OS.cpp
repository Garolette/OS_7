#include <iostream>
#include <Thread>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <string>
#include <fstream>
#include <Stdlib.h>
#include <mutex>

using namespace std;
mutex mtx1;
bool work = true;

void T1() {
    ofstream fout;
    srand(time(NULL));
    int num = rand() % 20;
    fout.open("..\\..\\F1.txt", ofstream::app);
    fout << num;
    fout.close();
    cout << "Поток  1 создал число: " << num << ". \n\t\t\t\t\t\tМагия, не иначе!" << endl;
    this_thread::sleep_for(chrono::milliseconds(rand() % 2000));
    while (work) {
        int num = rand() % 10;
        fout.open("..\\..\\F1.txt", ofstream::app);
        fout << '\n' << num;
        fout.close();
        cout << "Поток  1 создал число: " << num << ". \n\t\t\t\t\t\tМагия, не иначе!" << endl;
        this_thread::sleep_for(chrono::milliseconds(rand()%2000));
    }
}
void T2() {
    ifstream fin;
    ofstream fout;
    string boofer;
    string boofer_2;
    srand(time(NULL));
    int key = 0;
    int global_key = 0;
    int Summ = 0;
    int Chek = 0;
        while (work) {
            key = 0;
            mtx1.lock();
            fin.open("..\\..\\F1.txt");
            while (!fin.eof()) {
                getline(fin, boofer);
                key++;
            }
            fin.close();

            if (global_key + 1 < key) {

                fin.open("..\\..\\F1.txt");
                for (int i = 0; !fin.eof(); i++) {
                    getline(fin, boofer);
                    if (i == global_key) {
                        boofer_2 = boofer;
                        getline(fin, boofer);
                        break;
                    }
                }
                fin.close();
                global_key += 2;
                Summ = atoi(boofer_2.c_str()) + atoi(boofer.c_str());

                fin.open("..\\..\\F2.txt");
                while (!fin.eof()) {
                    getline(fin, boofer);
                    if (boofer != "\0") {
                        Chek++;
                    }
                }
                fin.close();
                if (Chek == 0) {
                    fout.open("..\\..\\F2.txt", ofstream::app);
                    fout << Summ;
                    fout.close();
                }
                else {
                    fout.open("..\\..\\F2.txt", ofstream::app);
                    fout << '\n' << Summ;
                    fout.close();
                }
                cout << "Потоку 2 удалось сложить числа и получить: " << Summ << ". \n\t\t\t\t\t\tПоток сделал важный вклад в файл 2. А чего добился ты?" << endl;
                Chek = 0;
            }
            mtx1.unlock();
        }
}
void T3() {
    ifstream fin;
    ofstream fout;
    string boofer;
    string boofer_2;
    int key = 0;
    int global_key = 0;
    int Pro = 0;
    int Check = 0;
    while (work) {
        key = 0;
        mtx1.lock();
        fin.open("..\\..\\F2.txt");
        while (!fin.eof()) {
            getline(fin, boofer);
            key++;
        }
        fin.close();
        if (global_key + 1 < key) {
            fin.open("..\\..\\F2.txt");
            for (int i = 0; !fin.eof(); i++) {
                getline(fin, boofer);
                if (i == global_key) {
                    boofer_2 = boofer;
                    getline(fin, boofer);
                    break;
                }
            }
            fin.close();
            global_key += 2;
            Pro = atoi(boofer_2.c_str()) * atoi(boofer.c_str());
            fin.open("..\\..\\F1.txt");
            while (!fin.eof()) {
                getline(fin, boofer);
                if (boofer != "\0") {
                    Check++;
                }
            }
            fin.close();
            if (Check == 0) {
                fout.open("F1.txt", ofstream::app);
                fout << Pro;
                fout.close();
            }
            else {
                fout.open("..\\..\\F1.txt", ofstream::app);
                fout << '\n' << Pro;
                fout.close();
            }
            cout << "Поток  3 умножил числа из файла 2: " << Pro << ". \n\t\t\t\t\t\tНе будем его за это осуждать. Просто сохраним число в первом файле." << endl;
            Check = 0;
        }
        mtx1.unlock();
    }
}
void start(int t) {
    ofstream fout;
    fout.open("..\\..\\F1.txt");
    fout.close();
    fout.open("..\\..\\F2.txt");
    fout.close();
    work = true;
    thread t1(T1);
    thread t2(T2);
    thread t3(T3);
    this_thread::sleep_for(chrono::seconds(t));
    work = false;
    t3.join();
    t1.join();
    t2.join();
}
int main()
{
    setlocale(0, "");
    int t = -1;
    cout << "__________Потоковый обработчик текстовых файлов запущен____________" << endl;
    while(t!=0) {
        cout << endl;
        cout << "Выберите длительность работы потокового обработчика текстовых файлов(для выхода введите 0): ";
        cin >> t;
        if (t > 0) {
            start(t);
            cout << "Потоки, остановитесь!" << endl;
        }
    }
    return 0;
}
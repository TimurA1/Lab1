#include <iostream>
#include <ctime>
#include <fstream>
#include <vector>
#include <chrono>
#include <algorithm>

using namespace std;

string inputFile = "data.txt";
string outputFile = "output.txt";
string timeStampsFile = "timestamps.txt";

ofstream foutTime(timeStampsFile);

int batchNum = 7;

int dims[7] = { 100, 500, 1000, 2000, 5000, 10000, 100000 };

//а, е, ж
//выбором, быстрая сортировка, сортировка слиянием

struct Automobile {
    string name;
    string brand;
    int year;
    string number;
    string color;

    Automobile() {

    }

    Automobile(string name, string brand, int year, string number, string color)
    {
        this->name = name;
        this->brand = brand;
        this->year = year;
        this->number = number;
        this->color = color;
    }

    friend bool operator== (const Automobile& a, const Automobile& b) {
        if (a.year == b.year && a.name == b.name && a.brand == b.brand
            && a.number == b.number && a.color == b.color)
            return true;
        return false;
    }

    friend bool operator< (const Automobile& a, const Automobile& b) {
        if (a.number < b.number || ((a.number == b.number) && (a.year < b.year)) ||
            ((a.number == b.number) && (a.year == b.year) && (a.brand < b.brand)) ||
            ((a.number == b.number) && (a.year == b.year) && (a.brand == b.brand) && (a.color < b.color)) ||
            ((a.number == b.number) && (a.year == b.year) && (a.brand == b.brand) && (a.color == b.color) && (a.name < b.name)))
            return true;
        return false;
    }

    friend bool operator<= (const Automobile& a, const Automobile& b) {
        if (a < b || a == b)
            return true;
        return false;
    }

    friend bool operator> (const Automobile& a, const Automobile& b) {
        if (!(a < b) && !(a == b))
            return true;
        return false;
    }

    friend bool operator>= (const Automobile& a, const Automobile& b) {
        if (!(a < b))
            return true;
        return false;
    }


    friend ostream& operator<<(ostream& os, const Automobile& a) {
        os << a.name << ' ' << a.brand << ' ' << a.year << ' ' << a.number << ' ' << a.color << '\n';
        return os;
    }
};

string randomString(const int len) {
    static const char alphanum[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    string tmp_s;
    tmp_s.reserve(len);
    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    return tmp_s;
}

void makeTxtFile() {
    srand((unsigned)time(NULL));
    ofstream fout(inputFile);

    for (int i = 0; i < batchNum; ++i) {
        //Вывод числа записей
        fout << dims[i] << "\n";
        for (int j = 0; j < dims[i]; ++j) {
            //Вывод полей по порядку
            fout << randomString(10) << "\n";   //Имя
            fout << randomString(8) << "\n";    //Марка
            fout << rand() % 10 + 1 << "\n";   //Год выпуска
            fout << randomString(8) << "\n";    //Гос номер
            fout << randomString(8) << "\n";    //Цвет
        }
    }
}

vector<vector<Automobile>> readTxtFile() {
    ifstream fin(inputFile);

    vector<vector<Automobile>> result;

    int dim;
    string name, brand, number, color;
    int year;

    for (int i = 0; i < batchNum; ++i) {
        //Ввод числа записей
        fin >> dim;
        vector<Automobile> v;
        for (int j = 0; j < dim; ++j) {
            //Ввод полей по порядку
            fin >> name >> brand >> year >> number >> color;
            Automobile temp(name, brand, year, number, color);
            v.push_back(temp);
        }
        result.push_back(v);
    }

    return result;
}

void selectSort(vector<Automobile>& a) {
    int min = 0; // индекс минимального значения

    for (int i = 0; i < a.size(); i++)
    {
        min = i; // изначально min - номер текущей ячейки

        for (int j = i + 1; j < a.size(); j++)
            min = (a[j] < a[min]) ? j : min;

        // после нахождения минимального элемента, поменяем его местами с текущим

        std::swap(a[i], a[min]);
    }
}

// function to rearrange array (find the partition point)
int partition(vector<Automobile>& array, int low, int high) {

    // select the rightmost element as pivot
    Automobile pivot = array[high];

    // pointer for greater element
    int i = (low - 1);

    // traverse each element of the array
    // compare them with the pivot
    for (int j = low; j < high; j++) {
        if (array[j] <= pivot) {

            // if element smaller than pivot is found
            // swap it with the greater element pointed by i
            i++;

            // swap element at i with element at j
            swap(array[i], array[j]);
        }
    }

    // swap pivot with the greater element at i
    swap(array[i + 1], array[high]);

    // return the partition point
    return (i + 1);
}

void quickSort(vector<Automobile>& array, int low, int high) {
    if (low < high) {

        // find the pivot element such that
        // elements smaller than pivot are on left of pivot
        // elements greater than pivot are on righ of pivot
        int pi = partition(array, low, high);

        // recursive call on the left of pivot
        quickSort(array, low, pi - 1);

        // recursive call on the right of pivot
        quickSort(array, pi + 1, high);
    }
}

// Merge two subarrays L and M into arr
void merge(vector<Automobile>& arr, int p, int q, int r) {

    // Create L ← A[p..q] and M ← A[q+1..r]
    int n1 = q - p + 1;
    int n2 = r - q;

    Automobile* L = new Automobile[n1];
    Automobile* M = new Automobile[n2];

    for (int i = 0; i < n1; i++)
        L[i] = arr[p + i];
    for (int j = 0; j < n2; j++)
        M[j] = arr[q + 1 + j];

    // Maintain current index of sub-arrays and main array
    int i, j, k;
    i = 0;
    j = 0;
    k = p;

    // Until we reach either end of either L or M, pick larger among
    // elements L and M and place them in the correct position at A[p..r]
    while (i < n1 && j < n2) {
        if (L[i] <= M[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = M[j];
            j++;
        }
        k++;
    }

    // When we run out of elements in either L or M,
    // pick up the remaining elements and put in A[p..r]
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = M[j];
        j++;
        k++;
    }
}

// Divide the array into two subarrays, sort them and merge them
void mergeSort(vector<Automobile>& arr, int l, int r) {
    if (l < r) {
        // m is the point where the array is divided into two subarrays
        int m = l + (r - l) / 2;

        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        // Merge the sorted subarrays
        merge(arr, l, m, r);
    }
}

void writeOutput(string title, vector<vector<Automobile>>& res) {
    ofstream fout(outputFile, ios::app);

    fout << title << "\n";

    for (int i = 0; i < batchNum; ++i) {
        //Вывод числа записей
        fout << "Размер: " << res[i].size() << "\n";
        for (int j = 0; j < res[i].size(); ++j) {
            //Вывод объекта
            fout << res[i][j] << "\n";
        }
    }
}

void writeTime(string title, vector<std::chrono::steady_clock::time_point> time) {
    foutTime << title << "\n";

    for (int i = 0; i < time.size() - 1; ++i)
        foutTime << "Время для сортировки " << dims[i] << " значений: " << chrono::duration_cast<chrono::milliseconds>(time[i + 1] - time[i]).count() << " [ms]\n";

    foutTime << endl;
}

int main()
{
    setlocale(LC_ALL, "Russian");
    makeTxtFile();

    vector<vector<Automobile>> initialArray = readTxtFile();

    vector<vector<Automobile>> selectSortArray = initialArray,
        quickSortArray = initialArray, mergeSortArray = initialArray;

    vector<std::chrono::steady_clock::time_point> time;

    time.push_back(std::chrono::steady_clock::now());

    for (int i = 0; i < batchNum; ++i) {
        selectSort(selectSortArray[i]);
        time.push_back(std::chrono::steady_clock::now());
    }

    writeOutput("Сортировка выбором:\n", selectSortArray);
    writeTime("Сортировка выбором:\n", time);
    time.clear();

    time.push_back(std::chrono::steady_clock::now());

    for (int i = 0; i < batchNum; ++i) {
        mergeSort(mergeSortArray[i], 0, mergeSortArray[i].size() - 1);
        time.push_back(std::chrono::steady_clock::now());
    }

    writeOutput("Сортировка слиянием:\n", mergeSortArray);
    writeTime("Сортировка слиянием:\n", time);
    time.clear();

    time.push_back(std::chrono::steady_clock::now());

    for (int i = 0; i < batchNum; ++i) {
        quickSort(quickSortArray[i], 0, quickSortArray[i].size() - 1);
        time.push_back(std::chrono::steady_clock::now());
    }

    writeOutput("Быстрая сортировка:\n", quickSortArray);
    writeTime("Быстрая сортировка:\n", time);
    time.clear();

    return 0;
}

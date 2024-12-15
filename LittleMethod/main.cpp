#define _CRT_SECURE_NO_WARNINGS

#include <conio.h>
#include <iostream>
#include <vector>

#include "LittleMethod.h"

using namespace std;

vector<vector<int>> cost = {
    {INT_MAX, 21, 15, 27, 26, 24},
    {15, INT_MAX, 12, 8, 17, 22},
    {9, 18, INT_MAX, 23, 0, 24},
    {5, 10, 26, INT_MAX, 7, 20},
    {23, 17, 15, 27, INT_MAX, 18},
    {24, 27, 11, 3, 26, INT_MAX}
};

vector<vector<int>> cost1 = {
    {INT_MAX, 1, 9, 7, 19, 14},
    {19, INT_MAX, 28, 24, 13, 14},
    {5, 12, INT_MAX, 28, 27, 21},
    {15, 5, 16, INT_MAX, 7, 7},
    {6, 1, 29, 20, INT_MAX, 21},
    {16, 2, 20, 8, 9, INT_MAX}
};

vector<vector<int>> cost2 = {
    {INT_MAX, 26, 18, 15, 3, 13},
    {24, INT_MAX, 19, 18, 7, 27},
    {5, 9, INT_MAX, 29, 24, 8},
    {13, 26, 20, INT_MAX, 26, 1},
    {0, 14, 15, 9, INT_MAX, 14},
    {9, 16, 21, 17, 16, INT_MAX}
};

int main()
{
    // Создаем объект метода Литтла с матрицей стоимостей
    LittleMethod* s = new LittleMethod(cost1);

    // Решаем задачу
    int result = s->Solve();

    // Вывод информации о том, какие рёбра исключены и какие включены
    cout << "\n\n\033[31m" << "Isklyuchenniye ryobra - krasniye\n";  // Красным цветом будут выделены исключенные рёбра
    cout << "\033[32m" << "Vklyuchenniye ryobra - zelyoniye\n" << "\033[0m";  // Зелёным цветом будут выделены включенные рёбра

    // Печать дерева решений
    BinaryTree::print(s->root);

    // Вывод результата (оптимальной стоимости)
    cout << "\n\n\033[35m" << "Rezultat - optimalnaya stoimost: " << "\033[0m" << result <<"\n\n";

    return 0;
}

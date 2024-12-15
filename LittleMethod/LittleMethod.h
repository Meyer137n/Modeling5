#pragma once
#include <vector>
#include <algorithm>
#include "BinaryTree.h"

using namespace std;

class LittleMethod {
private:
    // Матрица стоимостей для задачи
    vector<vector<int>> cost;
    // Количество вершин в задаче
    int n;

    // Путь, состоящий из пар (узел1, узел2), которые образуют оптимальный маршрут
    vector<pair<int, int>> path;
    
    // Оптимальное решение 
    int bestCost;

    // Функция для нахождения минимального значения в строке, исключая индекс
    int getMinInRow(const vector<vector<int>>& cost, int row, int excludeIndex = -1);

    // Функция для нахождения минимального значения в столбце, исключая индекс
    int getMinInCol(const vector<vector<int>>& cost, int col, int excludeIndex = -1);

public:
    // Корень бинарного дерева (дерево решений)
    Branch* root;

    // Конструктор, инициализирующий матрицу стоимостей
    LittleMethod(vector<vector<int>> cost);

    // Основной метод для решения задачи методом Литтла
    int Solve();

    // Метод для вычисления общей стоимости пути
    int getTotalCost(vector<pair<int, int>> path);

    // Метод для приведения матрицы стоимостей (редукция матрицы)
    int reduceMatrix(vector<vector<int>>& reduceCost);

    // Метод для вычисления штрафа, возвращает клетку с максимальным штрафом
    pair<int, pair<int, int>> calculateFint(const vector<vector<int>>& reduceCost);

    // Метод для создания левой матрицы и изменения пути
    void setLeftMatrix(Branch* left, pair<int, int> maxFint);

    // Метод для создания правой матрицы и изменения пути
    void setRightMatrix(Branch* right, pair<int, int> maxFint);

    // Метод для нахождения минимального листа в дереве
    Branch* findMinLeaf(const vector<Branch*>& leaves);

    // Метод для удаления листа из дерева
    void removeLeaf(vector<Branch*>& leaves, Branch* leaf);

    // Метод для печати пути (маршрута)
    void printPath(vector<pair<int, int>> path);
};

#pragma once
#include <vector>
#include <unordered_map>
#include <unordered_set>

using namespace std;

class Branch {
public:
    // Конструктор, инициализирующий ветвь с матрицей стоимостей, информацией о включении ребра и самим ребром
    Branch(vector<vector<int>> cost, bool includeEdge, pair<int, int> edge) 
    {
        this->cost = cost;                // Матрица стоимостей для текущей ветви
        this->includeEdge = includeEdge;  // Флаг, указывающий, включено ли это ребро в путь
        this->edge = edge;                // Ребро, представляющее переход между узлами
        leftBr = nullptr;                 // Указатель на левую ветвь (для дерева решений)
        rightBr = nullptr;                // Указатель на правую ветвь (для дерева решений)
        parent = nullptr;                 // Указатель на родительскую ветвь
    }

    vector<vector<int>> cost;        // Матрица стоимостей для текущей ветви
    int border;                      // Граничное значение для текущей ветви (не используется в этом фрагменте)
    bool includeEdge;                // Флаг, указывающий, включено ли текущее ребро в путь
    pair<int, int> edge;             // Пара (узел1, узел2) — ребро, представляющее переход между узлами

    Branch* leftBr;                  // Указатель на левую ветвь
    Branch* rightBr;                 // Указатель на правую ветвь
    Branch* parent;                  // Указатель на родительскую ветвь


	bool allIsNull(); // Метод для проверки, все ли указатели на дочерние ветви и родительский указатель равны nullptr

	// Функция для восстановления пути от текущей вершины до корня
	vector<pair<int, int>> getPath();

    // Метод для построения маршрута, включающего все ребра из переданного списка
	vector<pair<int, int>> buildRoute(const vector<pair<int, int>>& edges);

	// Метод для построения пути, начиная с заданного ребра
	vector<pair<int, int>> buildPathFromEdge(const vector<pair<int, int>>& edges, const pair<int, int>& startEdge);

	// Метод для построения пути, заканчивающегося заданным ребром
	vector<pair<int, int>> buildPathToEdge(const vector<pair<int, int>>& edges, const pair<int, int>& endEdge);

    // Метод для печати матрицы стоимостей для текущей ветви
	void printCost();
};



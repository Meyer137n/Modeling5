#include "Branch.h"
#include <iostream>

bool Branch::allIsNull()
{
	for (auto row : cost)
		for (auto el : row) if (el != INT_MAX && el) return false;
	return true;
}

vector<pair<int, int>> Branch::getPath()
{
	vector<pair<int, int>> path;
	if (allIsNull())
		for (int i = 0; i < cost.size(); i++)
			for (int j = 0; j < cost.size(); j++)
				if (!cost[i][j]) path.push_back(make_pair(i, j));

	Branch* current = this;
	while (current != nullptr && current->parent != nullptr) {
		if (current->includeEdge && current->edge != make_pair(-1, -1)) {
			path.push_back(current->edge);  // Добавляем ребро в путь
		}
		current = current->parent;  // Поднимаемся к родителю
	}
	return path;
}

vector<pair<int, int>> Branch::buildRoute(const vector<pair<int, int>>& edges)
{
	if (!edges.size()) return vector<pair<int, int>>{};
	// Словарь для хранения всех ребер
	unordered_map<int, int> nextVertex;
	unordered_map<int, int> inDegree;

	// Строим карту, где ключ - это вершина, а значение - вершина, в которую она ведет
	for (const auto& edge : edges) {
		nextVertex[edge.first] = edge.second;
		inDegree[edge.second]++;
		if (inDegree.find(edge.first) == inDegree.end()) {
			inDegree[edge.first] = 0;
		}
	}

	// Ищем вершину, которая не входит в другие ребра (она будет начальной)
	int startVertex = -1;
	for (const auto& edge : edges) {
		if (inDegree[edge.first] == 0) {
			startVertex = edge.first;
			break;
		}
	}

	// Если не нашли начальную вершину, то просто начинаем с первого ребра
	if (startVertex == -1) {
		startVertex = edges[0].first;
	}

	// Строим маршрут
	vector<pair<int, int>> route;
	int currentVertex = startVertex;

	unordered_set<int> visited; // Множество для отслеживания посещённых вершин

	while (nextVertex.find(currentVertex) != nextVertex.end()) {
		// Если вершина уже была посещена, это значит, что есть цикл
		if (visited.find(currentVertex) != visited.end()) {
			return route;
		}

		visited.insert(currentVertex);
		int next = nextVertex[currentVertex];
		route.push_back({ currentVertex, next });
		currentVertex = next;
	}

	return route;
}

vector<pair<int, int>> Branch::buildPathFromEdge(const vector<pair<int, int>>& edges, const pair<int, int>& startEdge)
{
	unordered_map<int, int> nextVertex;
	unordered_map<int, int> inDegree;

	// Строим карту, где ключ - это вершина, а значение - вершина, в которую она ведет
	for (const auto& edge : edges) {
		nextVertex[edge.first] = edge.second;
		inDegree[edge.second]++;
		if (inDegree.find(edge.first) == inDegree.end()) {
			inDegree[edge.first] = 0;
		}
	}

	// Пытаемся начать с указанного ребра
	int startVertex = startEdge.first;
	int currentVertex = startVertex;

	vector<pair<int, int>> route;
	bool foundStart = false;

	// Ищем начальную точку для маршрута
	while (nextVertex.find(currentVertex) != nextVertex.end()) {
		if (currentVertex == startVertex && !foundStart) {
			foundStart = true;
			route.push_back(startEdge);  // Начинаем с указанного ребра
			currentVertex = startEdge.second;
		}
		else {
			int next = nextVertex[currentVertex];
			route.push_back({ currentVertex, next });
			currentVertex = next;
		}
	}

	return route;
}

vector<pair<int, int>> Branch::buildPathToEdge(const vector<pair<int, int>>& edges, const pair<int, int>& endEdge)
{
	unordered_map<int, int> nextVertex;

	// Строим карту, где ключ - это вершина, а значение - вершина, в которую она ведет
	for (const auto& edge : edges) {
		nextVertex[edge.first] = edge.second;
	}

	// Начинаем с конечного ребра
	vector<pair<int, int>> route;
	int currentVertex = endEdge.second;

	// Строим маршрут, двигаясь от конечной вершины к начальной
	while (nextVertex.find(currentVertex) != nextVertex.end()) {
		int prevVertex = currentVertex;
		currentVertex = nextVertex[currentVertex];

		// Добавляем ребро в маршрут
		route.push_back({ currentVertex, prevVertex });
	}

	// Разворачиваем маршрут, чтобы он был построен от начальной вершины к конечной
	reverse(route.begin(), route.end());

	return route;
}

void Branch::printCost()
{
	string color = "";
	if (parent) color = includeEdge ? "\033[32m" : "\033[31m";
	for (auto row : cost) {
		for (auto el : row) {
			cout.width(4);
			if (el == INT_MAX) cout << "\t" << color << "INF";
			else cout << "\t" << color << el;
		}
		cout << "\n";
	}
	cout << "\033[0m";
}


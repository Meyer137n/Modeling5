#include "Branch.h"
#include <iostream>

//проблема из-за зацикливания в методах buildPathFromEdge и buildPathToEdge, хз как исправить

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
			path.push_back(current->edge);  // Äîáàâëÿåì ðåáðî â ïóòü
		}
		current = current->parent;  // Ïîäíèìàåìñÿ ê ðîäèòåëþ
	}
	return path;
}

vector<pair<int, int>> Branch::buildRoute(const vector<pair<int, int>>& edges)
{
	if (!edges.size()) return vector<pair<int, int>>{};
	// Ñëîâàðü äëÿ õðàíåíèÿ âñåõ ðåáåð
	unordered_map<int, int> nextVertex;
	unordered_map<int, int> inDegree;

	// Ñòðîèì êàðòó, ãäå êëþ÷ - ýòî âåðøèíà, à çíà÷åíèå - âåðøèíà, â êîòîðóþ îíà âåäåò
	for (const auto& edge : edges) {
		nextVertex[edge.first] = edge.second;
		inDegree[edge.second]++;
		if (inDegree.find(edge.first) == inDegree.end()) {
			inDegree[edge.first] = 0;
		}
	}

	// Èùåì âåðøèíó, êîòîðàÿ íå âõîäèò â äðóãèå ðåáðà (îíà áóäåò íà÷àëüíîé)
	int startVertex = -1;
	for (const auto& edge : edges) {
		if (inDegree[edge.first] == 0) {
			startVertex = edge.first;
			break;
		}
	}

	// Åñëè íå íàøëè íà÷àëüíóþ âåðøèíó, òî ïðîñòî íà÷èíàåì ñ ïåðâîãî ðåáðà
	if (startVertex == -1) {
		startVertex = edges[0].first;
	}

	// Ñòðîèì ìàðøðóò
	vector<pair<int, int>> route;
	int currentVertex = startVertex;

	unordered_set<int> visited; // Ìíîæåñòâî äëÿ îòñëåæèâàíèÿ ïîñåù¸ííûõ âåðøèí

	while (nextVertex.find(currentVertex) != nextVertex.end()) {
		// Åñëè âåðøèíà óæå áûëà ïîñåùåíà, ýòî çíà÷èò, ÷òî åñòü öèêë
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

	// Ñòðîèì êàðòó, ãäå êëþ÷ - ýòî âåðøèíà, à çíà÷åíèå - âåðøèíà, â êîòîðóþ îíà âåäåò
	for (const auto& edge : edges) {
		nextVertex[edge.first] = edge.second;
		inDegree[edge.second]++;
		if (inDegree.find(edge.first) == inDegree.end()) {
			inDegree[edge.first] = 0;
		}
	}

	// Ïûòàåìñÿ íà÷àòü ñ óêàçàííîãî ðåáðà
	int startVertex = startEdge.first;
	int currentVertex = startVertex;

	vector<pair<int, int>> route;
	bool foundStart = false;

	// Èùåì íà÷àëüíóþ òî÷êó äëÿ ìàðøðóòà
	while (nextVertex.find(currentVertex) != nextVertex.end()) {
		if (currentVertex == startVertex && !foundStart) {
			foundStart = true;
			route.push_back(startEdge);  // Íà÷èíàåì ñ óêàçàííîãî ðåáðà
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

	// Ñòðîèì êàðòó, ãäå êëþ÷ - ýòî âåðøèíà, à çíà÷åíèå - âåðøèíà, â êîòîðóþ îíà âåäåò
	for (const auto& edge : edges) {
		nextVertex[edge.first] = edge.second;
	}

	// Íà÷èíàåì ñ êîíå÷íîãî ðåáðà
	vector<pair<int, int>> route;
	int currentVertex = endEdge.second;

	// Ñòðîèì ìàðøðóò, äâèãàÿñü îò êîíå÷íîé âåðøèíû ê íà÷àëüíîé
	while (nextVertex.find(currentVertex) != nextVertex.end()) {
		int prevVertex = currentVertex;
		currentVertex = nextVertex[currentVertex];

		// Äîáàâëÿåì ðåáðî â ìàðøðóò
		route.push_back({ currentVertex, prevVertex });
	}

	// Ðàçâîðà÷èâàåì ìàðøðóò, ÷òîáû îí áûë ïîñòðîåí îò íà÷àëüíîé âåðøèíû ê êîíå÷íîé
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


#include "LittleMethod.h"
#include <algorithm>
#include <queue>

const bool PRINT_REDUCE = false;
const bool PRINT_REDUCE_R = true;
const bool PRINT_COST = true;
const bool PRINT_TREE_FOR_STEP = true;
const bool PRINT_PATH = true;
const bool PRINT_FINT = true;

int LittleMethod::getMinInRow(const vector<vector<int>>& cost, int row, int excludeIndex)
{
	int minCol = 0;
	// ������� ����������� ������� � ������, �������� ������� � �������� excludeIndex
	while (minCol == excludeIndex) minCol++;
	for (int j = 0; j < n; j++)
		if (j != excludeIndex && cost[row][j] < cost[row][minCol]) minCol = j;
	return cost[row][minCol];
}

int LittleMethod::getMinInCol(const vector<vector<int>>& cost, int col, int excludeIndex)
{
	int minRow = 0;
	// ������� ����������� ������� � �������, �������� ������� � �������� excludeIndex
	while (minRow == excludeIndex) minRow++;
	for (int i = 0; i < n; i++)
		if (i != excludeIndex && cost[i][col] < cost[minRow][col]) minRow = i;
	return cost[minRow][col];
}

LittleMethod::LittleMethod(vector<vector<int>> cost)
{
	this->cost = cost;
	n = cost.size();  // ���������� ����������� ������� ����������
	bestCost = INT_MAX;  // �������������� ���������� ��������� ��������� ��� INT_MAX
}

int LittleMethod::Solve()
{
	// TODO: ����������� ����� ������ � ������ ��� ������� ������ ������������
	// 0. ������������� ����� ������ ������� (����� �� ����������)
	// 1. ���� ������� �������� �� �� ����� (checkComplete), �������� ��������� � ���������� �������� (border)
	// 2. ���� ��������� - ����� �������, �� ����������� ������� (setLeftMatrix), ����� - (setRightMatrix)
	// ���� �����, �� ���������� (reduceMatrix), ����� ����� ������� ������ (calculateFint) � ����� �� ����� � ������ ������.
	// ���������, ���� �� ��������� 1 ��������� ����� (��� �1 - checkComplete)

	// ������� ������ ������ ������� � �������� �������� ����������
	root = new Branch(cost, false, {});

	if (PRINT_COST) {
		cout << "Matriza stoimostey:\n";  // ������� ������� ����������
		root->printCost();
	}

	// ��������� ������� � ��������� � �������
	root->border = reduceMatrix(root->cost);

	// �������� � ����� ������ � ���������� ���������
	vector<Branch*> leaves;  // ������ ��� �������� ���� �������
	leaves.push_back(root);  // ��������� ������ � ������ �������

	// �������� ���� ��������� ������
	while (true) {
		if (PRINT_TREE_FOR_STEP) {
			cout << "\033[33m" << "\n\nDerevo:\n" << "\033[0m";
			BinaryTree::print(root);  // ������ �������� ��������� ������ �������
		}

		// ������� ����� � ����������� ��������
		Branch* minBranch = findMinLeaf(leaves);

		// �������� ��������� �������
		auto path = minBranch->getPath();

		if (PRINT_PATH) {
			cout << "\033[33m" << "Tekushiy putb:\n\t" << "\033[0m";
			printPath(path);  // ������ �������� ����
		}

		if (PRINT_COST) {
			cout << "\n\nPreobrazovannaya matriza (r = " << minBranch->border;
			if (minBranch->parent) {
				cout << ", ";
				if (minBranch->includeEdge)cout << "\033[32m" << "Vklucheno";  // ��������� ����
				else cout << "\033[31m" << "Isklucheno";  // ���������� ����
				cout << "\033[0m" << " Granb: (" << minBranch->edge.first + 1 << ", " << minBranch->edge.second + 1 << ")";
			}
			cout << ")\n";
			minBranch->printCost();  // ������ ������� ���������� ����� ��������
		}

		// ������ ������� �� ������ ���������� ����
		auto route = minBranch->buildRoute(path);
		if (route.size() == n) {
			cout << "\033[33m" << "Primer marshruta:\n\t" << "\033[0m";
			printPath(route);  // ������ ������� ��������
			return getTotalCost(route);  // ���������� �������� ��������� ��������
		}

		// �������� ������������ ����� (Fint)
		auto maxFint = calculateFint(minBranch->cost);

		if (PRINT_FINT) {
			cout << "Maksimalniy Finansoviy shtraf = " << maxFint.first << " v yacheyke (" << maxFint.second.first + 1 << ", " << maxFint.second.second + 1 << ")\n";
		}

		// ��������� ���� �� ��� ���������
		if (!minBranch->leftBr) {
			Branch* leftBranch = new Branch(minBranch->cost, false, maxFint.second);
			minBranch->leftBr = leftBranch;
			leftBranch->parent = minBranch;

			setLeftMatrix(leftBranch, maxFint.second);  // ����������� ����� ���������
			if (PRINT_REDUCE) cout << "Usecheniye dlya levoy vetki:\n";  // ������� ���������� � ��������
			reduceMatrix(leftBranch->cost);  // ���������� ������� ��� ������ ���������
			leftBranch->border = minBranch->border + maxFint.first;

			if (PRINT_REDUCE_R) cout << "Levaya vetka r = " << minBranch->border << " + " << maxFint.first << " = " << leftBranch->border << "\n";

			leaves.push_back(leftBranch);  // ��������� ����� ��������� � ������ �������
		}
		if (!minBranch->rightBr) {
			Branch* rightBranch = new Branch(minBranch->cost, true, maxFint.second);
			minBranch->rightBr = rightBranch;
			rightBranch->parent = minBranch;
			setRightMatrix(rightBranch, maxFint.second);  // ����������� ������ ���������
			if (PRINT_REDUCE) cout << "\nUsecheniye dlya pravoy vetki:\n";  // ������� ���������� � ��������
			int r = reduceMatrix(rightBranch->cost);  // ���������� ������� ��� ������� ���������
			rightBranch->border = r + minBranch->border;

			if (PRINT_REDUCE_R) cout << "Pravaya vetka r = " << minBranch->border << " + " << r << " = " << rightBranch->border << "\n";

			leaves.push_back(rightBranch);  // ��������� ������ ��������� � ������ �������
		}

		// ������� ������� ���� �� ������� �������
		removeLeaf(leaves, minBranch);
	}
}

int LittleMethod::getTotalCost(vector<pair<int, int>> path)
{
	int totalCost = 0;
	for (auto& e : path)
		totalCost += cost[e.first][e.second];  // ��������� ��������� ������� ����
	return totalCost;
}

int LittleMethod::reduceMatrix(vector<vector<int>>& reduceCost)
{
	int currentR = 0;
	// �������� ����������� ������� �� �������
	int rowId = 1;
	for (auto& row : reduceCost) {
		auto minIter = min_element(row.begin(), row.end());
		int min = *minIter;

		if (PRINT_REDUCE) {
			cout << "Minimalniy element v stolbce #" << rowId++ << ": ";
			if (min == INT_MAX) cout << "M\n";
			else cout << min << "\n";
		}

		if (min == INT_MAX) continue;
		currentR += min;
		for (auto& el : row) if (el != INT_MAX) el -= min;
	}

	// �������� ����������� ������� �� ��������
	for (int j = 0; j < reduceCost[0].size(); j++) {
		// ������� ����������� � �������
		int minEl = getMinInCol(reduceCost, j);

		if (PRINT_REDUCE) {
			cout << "Minimalniy element v stolbce #" << j + 1 << ": ";
			if (minEl == INT_MAX) cout << "M\n";
			else cout << minEl << "\n";
		}

		if (minEl == INT_MAX) continue;
		currentR += minEl;
		// �������� ��� �� ������� �������� �������
		for (int i = 0; i < reduceCost.size(); i++) if (reduceCost[i][j] != INT_MAX) reduceCost[i][j] -= minEl;
	}

	return currentR;
}

pair<int, pair<int, int>> LittleMethod::calculateFint(const vector<vector<int>>& reduceCost)
{
	pair<int, int> v = { 0, 0 };
	int max = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (reduceCost[i][j]) continue;
			int minRowEl = getMinInRow(reduceCost, i, j);
			int minColEl = getMinInCol(reduceCost, j, i);
			int temp = minRowEl + minColEl;

			if (temp > max) {
				max = temp;
				v = { i, j };
			}
		}
	}
	return { max, v };
}

void LittleMethod::setLeftMatrix(Branch* left, pair<int, int> maxFint)
{
	left->cost[maxFint.first][maxFint.second] = INT_MAX;
}

void LittleMethod::setRightMatrix(Branch* right, pair<int, int> maxFint)
{
	for (int j = 0; j < n; j++) right->cost[maxFint.first][j] = INT_MAX;
	for (int i = 0; i < n; i++) right->cost[i][maxFint.second] = INT_MAX;
	right->cost[maxFint.second][maxFint.first] = INT_MAX;

	auto path = right->getPath();
	auto pathFrom = right->buildPathFromEdge(path, maxFint);
	auto pathEnd = right->buildPathToEdge(path, maxFint);
	if (pathFrom.size() > 0) {
		int from = pathFrom[pathFrom.size() - 1].second;
		int to = pathFrom[0].first;
		right->cost[from][to] = INT_MAX;
	}
	else if (pathEnd.size() > 0) {
		int from = pathEnd[pathEnd.size() - 1].second;
		int to = pathEnd[0].first;
		right->cost[from][to] = INT_MAX;
	}
}

Branch* LittleMethod::findMinLeaf(const vector<Branch*>& leaves)
{
	Branch* minBranch = nullptr;
	int minBorder = INT_MAX;

	// ���� ����� � ����������� �������� ����� ���� �������
	for (auto& branch : leaves) {
		if (branch->border < minBorder) {
			minBorder = branch->border;
			minBranch = branch;
		}
	}

	return minBranch;
}

void LittleMethod::removeLeaf(vector<Branch*>& leaves, Branch* leaf)
{
	// ������� � ������� ���� �� ������� �������
	auto it = find(leaves.begin(), leaves.end(), leaf);
	if (it != leaves.end()) leaves.erase(it);
}

void LittleMethod::printPath(vector<pair<int, int>> path)
{
	for (const auto& point : path)
		cout << "(" << point.first + 1 << ", " << point.second + 1 << ") ";  // ������ ������� ���� ����
	cout << "\n";
}

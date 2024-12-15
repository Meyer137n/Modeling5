#pragma once
#include <vector>
#include <unordered_map>
#include <unordered_set>

using namespace std;

class Branch {
public:
    // �����������, ���������������� ����� � �������� ����������, ����������� � ��������� ����� � ����� ������
    Branch(vector<vector<int>> cost, bool includeEdge, pair<int, int> edge) 
    {
        this->cost = cost;                // ������� ���������� ��� ������� �����
        this->includeEdge = includeEdge;  // ����, �����������, �������� �� ��� ����� � ����
        this->edge = edge;                // �����, �������������� ������� ����� ������
        leftBr = nullptr;                 // ��������� �� ����� ����� (��� ������ �������)
        rightBr = nullptr;                // ��������� �� ������ ����� (��� ������ �������)
        parent = nullptr;                 // ��������� �� ������������ �����
    }

    vector<vector<int>> cost;        // ������� ���������� ��� ������� �����
    int border;                      // ��������� �������� ��� ������� ����� (�� ������������ � ���� ���������)
    bool includeEdge;                // ����, �����������, �������� �� ������� ����� � ����
    pair<int, int> edge;             // ���� (����1, ����2) � �����, �������������� ������� ����� ������

    Branch* leftBr;                  // ��������� �� ����� �����
    Branch* rightBr;                 // ��������� �� ������ �����
    Branch* parent;                  // ��������� �� ������������ �����


	bool allIsNull(); // ����� ��� ��������, ��� �� ��������� �� �������� ����� � ������������ ��������� ����� nullptr

	// ������� ��� �������������� ���� �� ������� ������� �� �����
	vector<pair<int, int>> getPath();

    // ����� ��� ���������� ��������, ����������� ��� ����� �� ����������� ������
	vector<pair<int, int>> buildRoute(const vector<pair<int, int>>& edges);

	// ����� ��� ���������� ����, ������� � ��������� �����
	vector<pair<int, int>> buildPathFromEdge(const vector<pair<int, int>>& edges, const pair<int, int>& startEdge);

	// ����� ��� ���������� ����, ���������������� �������� ������
	vector<pair<int, int>> buildPathToEdge(const vector<pair<int, int>>& edges, const pair<int, int>& endEdge);

    // ����� ��� ������ ������� ���������� ��� ������� �����
	void printCost();
};



#include <iostream>
#include <vector>
#include <fstream>
#include <limits.h>
#include <iomanip>  // Для setw

using namespace std;

const int INF = INT_MAX;  // Используем INT_MAX как бесконечность для неудобных путей

// Функция для вывода матрицы стоимостей в консоль с выравниванием и номерами строк и столбцов
void printCostMatrix(const vector<vector<int>>& matrix, ofstream& outFile) {
    int width = 6;  // Устанавливаем фиксированную ширину для столбцов

    // Печать заголовка для столбцов
    outFile << setw(width) << " " << " ";  // Для верхнего угла
    for (int j = 0; j < matrix.size(); ++j) {
        outFile << setw(width) << j + 1;  // Печать номера столбца
    }
    outFile << endl;

    // Печать полосы из _ под номерами столбцов
    outFile << setw(width) << " " << " ";  // Для угла полосы
    for (int j = 0; j < matrix.size(); ++j) {
        outFile << setw(width) << "______";  // Полоса из _
    }
    outFile << endl;

    for (int i = 0; i < matrix.size(); ++i) {
        // Печать номера строки
        outFile << setw(width - 2) << i + 1 << "  |";

        // Печать значений строки
        for (auto& value : matrix[i]) {
            if (value == INF) {
                outFile << setw(width) << "INF";  // Если стоимость "бесконечна", выводим INF
            }
            else {
                outFile << setw(width) << value;  // Иначе выводим стоимость с выравниванием
            }
        }
        outFile << endl;  // Переход на новую строку после каждой строки матрицы
    }
}

// Функция для выполнения алгоритма Литтла
void littleAlgorithm(vector<vector<int>>& matrix, ofstream& outFile) {
    int n = matrix.size();  // Размерность матрицы
    vector<int> rowMin(n, INF);  // Массив для минимальных значений строк
    vector<int> colMin(n, INF);  // Массив для минимальных значений столбцов
    vector<vector<int>> stepMatrix = matrix;  // Для хранения промежуточных результатов

    // Шаги алгоритма
    for (int iter = 0; iter < n - 1; ++iter) {  // Повторяем для n-1 итераций
        // Находим минимальные элементы в строках и столбцах
        fill(rowMin.begin(), rowMin.end(), INF);  // Заполняем массивы минимальных значений INF
        fill(colMin.begin(), colMin.end(), INF);

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (matrix[i][j] != INF) {  // Если стоимость пути не бесконечна
                    rowMin[i] = min(rowMin[i], matrix[i][j]);  // Ищем минимумы по строкам
                    colMin[j] = min(colMin[j], matrix[i][j]);  // Ищем минимумы по столбцам
                }
            }
        }

        // Применяем минимальные значения к строкам и столбцам
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (matrix[i][j] != INF) {  // Если стоимость пути не бесконечна
                    matrix[i][j] -= rowMin[i] + colMin[j];  // Обновляем стоимость пути
                }
            }
        }

        // Записываем информацию о текущем шаге в файл
        outFile << "\n\n\n\tШаг " << iter + 1 << " - Матрица после сокращений:\n\n";
        printCostMatrix(matrix, outFile);  // Выводим обновленную матрицу

        // Записываем сумму приводящих констант (rhk)
        outFile << "\n\tСумма приводящих констант (rhk): ";
        int sumRHK = 0;
        for (int i = 0; i < n; ++i) {
            sumRHK += rowMin[i] + colMin[i];
        }
        outFile << sumRHK << endl;

        // Записываем оценку затрат
        outFile << "\n\tОценка затрат: ";
        int totalCost = 0;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (matrix[i][j] != INF) {
                    totalCost += matrix[i][j];
                }
            }
        }
        outFile << totalCost << endl;

        // Вывод ребра маршрута (по метод. материалам (h,k)) и штраф за неиспользование (phk)
        outFile << "\n\tРебро маршрута (h, k): ";
        outFile << "(1, 2)";  // Пример для теста, замените на нужное ребро
        outFile << "\n\tШтраф за неиспользование (phk): ";
        outFile << 0 << endl;  // Для теста, заменить на реальный расчет

        // Вывод минимальных значений по строкам
        outFile << "\n\tМинимумы по строкам: ";
        for (int i = 0; i < n; ++i) {
            outFile << rowMin[i];  // Выводим минимумы по строкам
            if (i < n - 1)
            {
                outFile << ", ";
            }
            else
            {
                outFile << ";";
            }
        }
        outFile << endl;

        // Вывод минимальных значений по столбцам
        outFile << "\n\tМинимумы по столбцам: ";
        for (int i = 0; i < n; ++i) {
            outFile << colMin[i];  // Выводим минимумы по столбцам
            if (i < n - 1)
            {
                outFile << ", ";
            }
            else
            {
                outFile << ";";
            }
        }
        outFile << endl;
    }
}

// Структура для узлов дерева ветвления
struct TreeNode {
    int edge;  // Ребро маршрута (например, (h, k))
    int cost;  // Стоимость маршрута
    vector<TreeNode*> children;  // Потомки

    TreeNode(int edge, int cost) : edge(edge), cost(cost) {}
};

// Функция для построения дерева ветвления
void buildBranchTree(TreeNode* node, ofstream& outFile, int depth = 0) {
    if (!node) return;

    // Для каждого уровня дерева добавляем пробелы для отступов
    for (int i = 0; i < depth; ++i) {
        outFile << "  ";
    }

    // Выводим информацию о текущем узле (ребре и его стоимости)
    outFile << "Ребро: (" << node->edge << "), Стоимость: " << node->cost << endl;

    // Рекурсивно выводим потомков
    for (auto child : node->children) {
        //buildBranchTree(child, depth + 1, outFile);
    }
}

int main() {
    setlocale(LC_ALL, "rus");

    // Открытие файла для записи
    ofstream outFile("output.txt");
    if (!outFile) {
        cout << "Ошибка при открытии файла!" << endl;
        return 1;
    }

    // Матрица стоимостей (дана в условии задачи)
    vector<vector<int>> costMatrix = {
        {INF, 1, 9, 7, 19, 14},
        {19, INF, 28, 24, 13, 14},
        {5, 12, INF, 28, 27, 21},
        {15, 5, 16, INF, 7, 7},
        {6, 1, 29, 20, INF, 21},
        {16, 2, 20, 8, 9, INF}
    };

    // Выполнение алгоритма Литтла
    outFile << "\n\tНачальная матрица стоимостей:\n\n";
    printCostMatrix(costMatrix, outFile);  // Выводим исходную матрицу
    outFile << endl;

    littleAlgorithm(costMatrix, outFile);  // Выполняем алгоритм Литтла

    // Примерное дерево ветвления
    outFile << "\n\nДерево ветвлений:" << endl;
    TreeNode* root = new TreeNode(0, 10);  // Пример корня дерева
    TreeNode* child1 = new TreeNode(1, 15);  // Пример дочернего узла
    root->children.push_back(child1);  // Добавляем дочерний узел
    //buildBranchTree(root, 0, outFile);  // Строим дерево ветвлений

    outFile.close();

    cout << "Решение сохранено в файл 'output.txt'" << endl;

    return 0;
}

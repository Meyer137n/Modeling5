#pragma once
#include <iostream>
#include <fstream>
#include "Branch.h"

using namespace std;

// Unicode псевдографика для отображения дерева
const string
ch_hor = "\xC4",             // Горизонтальная линия (─)
ch_ver = "\xB3",             // Вертикальная линия (│)
ch_ddia = "\xDA",            // Верхний левый угол (┌)
ch_rddia = "\xBF",           // Верхний правый угол (┐)
ch_udia = "\xC0",            // Нижний левый угол (└)
ch_ver_hor = "\xC3\xC4",     // Вертикальная линия с горизонтальной (├─)
ch_udia_hor = "\xC0\xC4",    // Нижний левый угол с горизонтальной линией (└─)
ch_ddia_hor = "\xDA\xC4",    // Верхний левый угол с горизонтальной линией (┌─)
ch_ver_spa = "\xB3 ";        // Вертикальная линия с пробелом (│ )

class BinaryTree
{
public:
    int size;    // Размер дерева
    Branch* root;  // Указатель на корень дерева

    // Конструктор по умолчанию
    BinaryTree();

    // Деструктор
    ~BinaryTree();

    // Статический метод для печати дерева начиная с указанной ветви
    // Использует псевдографику для красивого отображения дерева
    static void print(Branch* branch, string const& rpref = "\t", string const& cpref = "\t", string const& lpref = "\t");

    // Метод для удаления ветви
    void destroyBranch(Branch* branch);
};

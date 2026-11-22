#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <utility>
#include <string>
#include <clocale>
#include <limits>

//Структура ребра
struct Edge
{
    int from; //Откуда идёт ребро
    int to; //Куда идёт ребро
    int weight; //Вес ребра

    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

//Структура размеров графа
struct GraphSize
{
    int vertexCount;
    int edgeCount;
}; 

//Функиция очистки буффера
void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

//Функция ввода размеров графа
GraphSize inputGraphSize() {
    std::cout << "Введите количество вершин и ребер. Формат ввода (int int).\n";
    GraphSize size;
    if (!(std::cin >> size.vertexCount >> size.edgeCount)) {
        throw std::invalid_argument("Ошибка формата ввода. Требуется написать 2 целых числа через пробел.");
    }
    if (size.vertexCount <= 0) {
        throw std::invalid_argument("Количество вершин должно быть строго больше 0.");
    }
    if (size.edgeCount < 0) {
        throw std::invalid_argument("Количество ребер должно быть не меньше 0.");
    }

    return size;
}

//Функция ввода ребер
std::vector<Edge> inputEdges(GraphSize size) {
    std::vector<Edge> graph(size.edgeCount);
    std::cout << "Введите вершины и ребра. Формат ввода (откуда куда вес).\n";

    for (int i = 0; i < size.edgeCount; ++i) {
        if (!(std::cin >> graph[i].from >> graph[i].to >> graph[i].weight)) {
            throw std::invalid_argument("Некорректный формат данных ребра");
        }
        if (graph[i].from < 1 || graph[i].from > size.vertexCount || graph[i].to < 1 || graph[i].to > size.vertexCount) {
            throw std::invalid_argument("Номер вершины должен быть в пределах от 1 до " + std::to_string(size.vertexCount));
        }
    }
    return graph;
}

void findMinSpanningTree(std::vector<Edge>& graph, GraphSize size) {
    std::sort(graph.begin(), graph.end());
    std::vector<int> componentId(size.vertexCount + 1);

    for (int i = 0; i < componentId.size(); ++i) {
        componentId[i] = i;
    }

    std::vector<Edge> path;
    int totalWeight = 0;

    for (int i = 0; i < graph.size(); ++i) {
        if (componentId[graph[i].from] == componentId[graph[i].to]) {
            continue;
        }
        else {
            path.push_back(graph[i]);
            totalWeight += graph[i].weight;
            std::cout << "\nДобавлено ребро: " << graph[i].from << " - " << graph[i].to
                << " (вес " << graph[i].weight << ")\n";
            int newId = componentId[graph[i].from];
            int oldId = componentId[graph[i].to];
            for (int j = 1; j < componentId.size(); ++j) {
                if (componentId[j] == oldId) {
                    std::cout << "  componentId[" << j << "]: " << oldId << " -> " << newId << "\n";
                    componentId[j] = newId;
                }
            }
        }

        if (path.size() == size.vertexCount - 1) {
            break;
        }
    }

    if (path.size() != size.vertexCount - 1) {
        std::cout << "Граф несвязный, остовного дерева не существует.\n";
    }
    else {
        std::cout << "\nМинимальное остовное дерево:\n";
        for (const Edge& e : path) {
            std::cout << e.from << " - " << e.to << " (вес " << e.weight << ")\n";
        }
        std::cout << "Суммарный вес: " << totalWeight << "\n";
    }
}

int main()
{
    std::setlocale(LC_ALL, "Russian");
    
    GraphSize size;
    while (true) {
        try {
            size = inputGraphSize();
            break;
        }
        catch (std::invalid_argument& e) {
            std::cerr << "Ошибка: " << e.what() << "\nПопробуйте заново.\n\n";
            clearInputBuffer();
        }
    }

    std::vector<Edge> graph;
    while (true) {
        try {
            graph = inputEdges(size);
            break;
        }
        catch (std::invalid_argument& e) {
            std::cerr << "Ошибка: " << e.what() << "\nВведите все рёбра заново.\n\n";
            clearInputBuffer();
        }
    }
    std::cout << "\nДанные успешно обработаны!\n";
    
    findMinSpanningTree(graph, size);
    
    return 0;
}

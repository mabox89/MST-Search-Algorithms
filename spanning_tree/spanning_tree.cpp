#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <utility>
#include <string>
#include <clocale>
#include <limits>
#include <utility>

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

//Функция поиска, которая создает массив минимальных ребер, которые идут к i вершине
std::vector<std::vector<std::pair<int, int>>> findMinEdges(std::vector<Edge>& graph, GraphSize& size) {
    std::vector<std::vector<std::pair<int, int>>> minEdges(size.vertexCount + 1);
    for (const auto& edge : graph) {
        minEdges[edge.from].push_back({ edge.to, edge.weight });
        minEdges[edge.to].push_back({ edge.from, edge.weight });
    }

    return minEdges;
}

void primMST(GraphSize& size, std::vector<Edge>& graph) {
    std::vector<std::vector<std::pair<int, int>>> minEdges = findMinEdges(graph, size);
    std::vector<bool> visitedVertex(size.vertexCount + 1, false);
    std::vector<Edge> path;

    visitedVertex[1] = true;
    int totalWeight = 0;

    for (int i = 0; i < size.vertexCount - 1; ++i) {
        int minWeight = INT_MAX;
        int bestFrom = -1;
        int bestTo = -1;

        for (int from = 1; from <= size.vertexCount; ++from) {
            if (!visitedVertex[from]) continue;
            
            for (const auto& edge : minEdges[from]) {
                int to = edge.first;
                int weight = edge.second;

                if (!visitedVertex[to] && weight < minWeight) {
                    minWeight = weight;
                    bestFrom = from;
                    bestTo = to;
                }
            }
        }

        if (bestTo == -1) break;

        visitedVertex[bestTo] = true;
        path.push_back({ bestFrom, bestTo, minWeight });
        totalWeight += minWeight;
        std::cout << "\nДобавлено ребро: " << bestFrom << " - " << bestTo
            << " (вес " << minWeight << ")\n";
    }
    
    if (static_cast<int>(path.size()) != size.vertexCount - 1) {
        std::cout << "Граф несвязный, остовного дерева не существует.\n";
    }
    else {
        std::cout << "\nМинимальное остовное дерево (Прим):\n";
        for (const Edge& e : path) {
            std::cout << e.from << " - " << e.to << " (вес " << e.weight << ")\n";
        }
        std::cout << "Суммарный вес: " << totalWeight << "\n";
    }
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
std::vector<Edge> inputEdges(GraphSize& size) {
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
    
    std::cout << "\n=== Краскал ===\n";
    findMinSpanningTree(graph, size);

    std::cout << "\n=== Прим ===\n";
    primMST(size, graph);
    
    return 0;
}

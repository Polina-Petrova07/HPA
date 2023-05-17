// tryHPA.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
/*
{1, 2, 3, 4},
{5,6,7,8},
{9,10,11,12},
{13,14,15,16}

{1,2,3,4,5},
{6,7,8,9,10},
{11,12,13,14,15},
{16,17,18,19,20},
{21,22,23,24,25}

*/
#include <iostream>
#include <queue>
#include<algorithm>
#include "tiles.h"
#include "DrawFunctions.h"
#include <omp.h>
//#include <Windows.h>
//#include <wincodec.h>
//#include <gdiplus.h>
const int INF = 1000000000;
void tmpPtintMatrix(double** m, int n) {
    std::cout << std::endl << std::endl;
    std::cout << " === PRINT MATRIX === " << std::endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            std::cout << m[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

enum location { UPLEFT, UP, UPRIGHT, RIGHT, DOWNRIGHT, DOWN, DOWNLEFT, LEFT };
std::vector<std::pair<int, location>> findClusterNeighbors(int count, int clusterId) { // count - количесвто кластеров (обычно квадрат какого то числа)
    int size = (int)std::sqrt(count);
    std::vector<std::vector<int>> clusterMap;
    for (int i = 0; i < size; i++) {
        clusterMap.resize(size);
        for (int j = 0; j < size; j++)
            clusterMap[i].resize(size);
    }

    std::vector<int> temp;
    for (int i = 1; i <= count; i++)
        temp.push_back(i);
    int k = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            clusterMap[i][j] = temp[k];
            k++;
        }
    }

    std::pair<int, int> pos = findPos(size, clusterId);
    std::vector<std::pair<int, location>> masI(8);
    std::vector<std::pair<int, location>> masJ(8);
    masI[0].first = pos.first - 1;
    masI[0].second = UPLEFT;
    masI[1].first = pos.first - 1;
    masI[1].second = UP;
    masI[2].first = pos.first - 1;
    masI[2].second = UPRIGHT;
    masI[3].first = pos.first;
    masI[3].second = LEFT;
    masI[4].first = pos.first;
    masI[4].second=RIGHT;
    masI[5].first = pos.first + 1;
    masI[5].second = DOWNLEFT;
    masI[6].first = pos.first + 1;
    masI[6].second = DOWN;
    masI[7].first = pos.first + 1;
    masI[7].second = DOWNRIGHT;

    masJ[0].first = pos.second - 1;
    masJ[0].second =UPLEFT;
    masJ[1].first = pos.second;
    masJ[1].second = UP;
    masJ[2].first = pos.second + 1;
    masJ[2].second = UPRIGHT;
    masJ[3].first = pos.second - 1;
    masJ[3].second = LEFT;
    masJ[4].first = pos.second + 1;
    masJ[4].second = RIGHT;
    masJ[5].first = pos.second - 1;
    masJ[5].second = DOWNLEFT;
    masJ[6].first = pos.second;
    masJ[6].second = DOWN;
    masJ[7].first = pos.second + 1;
    masJ[7].second = DOWNRIGHT;

    /*int* masI = new int[8];
    int* masJ = new int[8];
    masI[0] = pos.first - 1;
    masI[1] = pos.first - 1;
    masI[2] = pos.first - 1;
    masI[3] = pos.first;
    masI[4] = pos.first;
    masI[5] = pos.first + 1;
    masI[6] = pos.first + 1;
    masI[7] = pos.first + 1;

    masJ[0] = pos.second - 1;
    masJ[1] = pos.second;
    masJ[2] = pos.second + 1;
    masJ[3] = pos.second - 1;
    masJ[4] = pos.second + 1;
    masJ[5] = pos.second - 1;
    masJ[6] = pos.second;
    masJ[7] = pos.second + 1;*/

    std::vector<std::pair<int, location>> neighbors;
    for (int i = 0; i < 8; i++) {
        if (masI[i].first < 0 || masI[i].first == clusterMap.size() || masJ[i].first < 0 || masJ[i].first == clusterMap.size())
            continue;
        if (clusterMap[masI[i].first][masJ[i].first] == 0)  // клетка - препядствие не помещается в вектор соседей
            continue;
        else {
            std::pair<int, location> tmp;
            //tmp.first= findVertex(masI[i].first, masJ[i].first, (int)clusterMap.size());
            tmp.first = (masI[i].first * (int)clusterMap.size() + masJ[i].first + 1);
            tmp.second = masI[i].second;
            neighbors.push_back(tmp);
        }
    }
    return neighbors;
}
std::vector<Entrance> findEnterance(std::vector<Cluster> V, Map &map) {
    const int maxLenght = 6;
    std::vector<Entrance> entrances;
    for (int i = 0; i < V.size(); i++) {
        Cluster curCluster = V[i];
        std::vector<std::pair<int, location>> currentNeighbors = findClusterNeighbors((int)V.size(), V[i].getId());  // первое значение - ID кластера, второе - location
        for (int j = 0; j < currentNeighbors.size(); j++) {
            int numE = 0;
            Cluster curNeighbor = V[currentNeighbors[j].first - 1];
            location currLoc = currentNeighbors[j].second;
            std::vector<std::pair<int,int>> l1;
            std::vector<std::pair<int, int>> l2;
            if ((currLoc == UPRIGHT) || (currLoc == DOWNRIGHT) || (currLoc == DOWNLEFT) || (currLoc == UPLEFT))
                continue;
            switch(currLoc) {
            case UP: {
                std::pair<int, int> startPoint_1 = curCluster.getStart();
                std::pair<int, int> startPoint_2 = std::make_pair(curNeighbor.getEnd().first, curNeighbor.getStart().second);
                std::pair<int, int> endPoint_1 = std::make_pair(curCluster.getStart().first, curCluster.getEnd().second);
                std::pair<int, int> endPoint_2 = curNeighbor.getEnd();
                int tmp = startPoint_1.second;
                while (tmp <= endPoint_1.second) {
                    l1.push_back(std::make_pair(startPoint_1.first, tmp));
                    //l1.push_back(map.getMaze()[startPoint_1.first][tmp]);
                    tmp++;
                }
                tmp = startPoint_2.second;
                while (tmp <= endPoint_2.second) {
                    l2.push_back(std::make_pair(startPoint_2.first, tmp));
                    //l2.push_back(map.getMaze()[startPoint_2.first][tmp]);
                    tmp++;
                }
                break;
            }
            case RIGHT: {
                std::pair<int, int> startPoint_1 = std::make_pair(curCluster.getStart().first, curCluster.getEnd().second);
                std::pair<int, int> startPoint_2 = curNeighbor.getStart();
                std::pair<int, int> endPoint_1 = curCluster.getEnd();
                std::pair<int, int> endPoint_2 = std::make_pair(curNeighbor.getEnd().first, curNeighbor.getStart().second);
                int tmp = startPoint_1.first;
                while (tmp <= endPoint_1.first) {
                    l1.push_back(std::make_pair(tmp, startPoint_1.second));
                    //l1.push_back(map.getMaze()[tmp][startPoint_1.second]);
                    tmp++;
                }
                tmp = startPoint_2.first;
                while (tmp <= endPoint_2.first) {
                    l2.push_back(std::make_pair(tmp, startPoint_2.second));
                    //l2.push_back(map.getMaze()[tmp][startPoint_2.second]);
                    tmp++;
                }
                break;
            }
            case DOWN: {
                std::pair<int, int> startPoint_1 = std::make_pair(curCluster.getEnd().first, curCluster.getStart().second);
                std::pair<int, int> startPoint_2 = curNeighbor.getStart();
                std::pair<int, int> endPoint_1 = curCluster.getEnd();
                std::pair<int, int> endPoint_2 = std::make_pair(curNeighbor.getStart().first, curNeighbor.getEnd().second);
                int tmp = startPoint_1.second;
                while (tmp <= endPoint_1.second) {
                    l1.push_back(std::make_pair(startPoint_1.first, tmp));
                    //l1.push_back(map.getMaze()[startPoint_1.first][tmp]);
                    tmp++;
                }
                tmp = startPoint_2.second;
                while (tmp <= endPoint_2.second) {
                    l2.push_back(std::make_pair(startPoint_2.first, tmp));
                    //l2.push_back(map.getMaze()[startPoint_2.first][tmp]);
                    tmp++;
                }
                break;
            }
            case LEFT: {
                std::pair<int, int> startPoint_1 = curCluster.getStart();
                std::pair<int, int> startPoint_2 = std::make_pair(curNeighbor.getStart().first, curNeighbor.getEnd().second);
                std::pair<int, int> endPoint_1 = std::make_pair(curCluster.getEnd().first, curCluster.getStart().second);
                std::pair<int, int> endPoint_2 = curNeighbor.getEnd();
                int tmp = startPoint_1.first;
                while (tmp <= endPoint_1.first) {
                    l1.push_back(std::make_pair(tmp, startPoint_1.second));
                    //l1.push_back(map.getMaze()[tmp][startPoint_1.second]);
                    tmp++;
                }
                tmp = startPoint_2.first;
                while (tmp <= endPoint_2.first) {
                    l2.push_back(std::make_pair(tmp, startPoint_2.second));
                    //l2.push_back(map.getMaze()[tmp][startPoint_2.second]);
                    tmp++;
                }
                break;
            }
            default: {
                std::cout << "ERROR! in function findEnterance." << std::endl;
                break;
            }
            }
            int len = 0;
            std::pair<int, int> s, e, sr, er;
            s = l1[0];
            sr = l2[0];
            for (int k = 0; k < l1.size(); k++) {
                if ((map.getMaze()[l1[k].first][l1[k].second] == 0) || (map.getMaze()[l2[k].first][l2[k].second] == 0)) { 
                    if (len > 0) {
                        if (k < l1.size() - 1) { 
                            Entrance curEntrance(std::make_pair(s.first, s.second), std::make_pair(e.first, e.second),
                                std::make_pair(sr.first, sr.second), std::make_pair(er.first, er.second), len, curCluster.getId(), curNeighbor.getId());
                            entrances.push_back(curEntrance);
                            numE++;
                            s = l1[k + 1];
                            sr = l2[k + 1];
                            len = 0;
                            continue;
                        }
                        else {
                            len++;
                            Entrance curEntrance(std::make_pair(s.first, s.second), std::make_pair(e.first, e.second),
                                std::make_pair(sr.first, sr.second), std::make_pair(er.first, er.second), len, curCluster.getId(), curNeighbor.getId());
                            entrances.push_back(curEntrance);
                            numE++;
                            s = l1[k];
                            sr = l2[k];
                            len = 0;
                            continue;
                        }
                    }
                    else {
                        if (k != l1.size() - 1) {
                            s = l1[k + 1];
                            sr = l2[k + 1];
                        }   
                        else 
                            if (k == l1.size() - 1) {
                                if ((map.getMaze()[s.first][s.second] == 0) || (map.getMaze()[l2[k].first][l2[k].second] == 0))  // но считается симметричный; если стартовая вершина не проходима или симметричная ей тоже не проходима
                                    continue;
                                else {
                                    e = s;
                                    er = sr;
                                    Entrance curEntrance(std::make_pair(s.first, s.second), std::make_pair(e.first, e.second),
                                        std::make_pair(sr.first, sr.second), std::make_pair(er.first, er.second), len, curCluster.getId(), curNeighbor.getId());
                                    entrances.push_back(curEntrance);
                                    numE++;
                                }
                            }
                            else
                                continue;
                    }

                    /*if (k < l1.size() - 1) { // как буд то не верно, найденный до границы выход стирается
                        s = l1[k + 1];
                        len = 0;
                        continue;
                    }
                    else {
                        s = l1[k];
                        len = 0;
                        continue;
                    }*/
                   
                }
                else {
                    if (len < maxLenght) {
                        if ((k + 1) != l1.size()) {
                            e = l1[k];
                            er = l2[k];
                            len++;
                        }
                        else {
                            if (numE >= 1) {
                                len++;
                            }
                            e = l1.back();
                            er = l2.back();
                            len++;
                            Entrance curEntrance(std::make_pair(s.first, s.second), std::make_pair(e.first, e.second),
                                std::make_pair(sr.first, sr.second), std::make_pair(er.first, er.second), len, curCluster.getId(), curNeighbor.getId());
                            entrances.push_back(curEntrance);
                            numE++;
                        }
                    }
                    else { // обработка случая, если на границе более одного входа
                        //len++; 
                        Entrance curEntrance(std::make_pair(s.first, s.second), std::make_pair(e.first, e.second),
                            std::make_pair(sr.first, sr.second), std::make_pair(er.first, er.second), len, curCluster.getId(), curNeighbor.getId());
                        entrances.push_back(curEntrance);
                        numE++;
                        len = 0;
                        s = l1[k];
                        sr = l2[k];
                    }
                }
            }
        }
    }
    //  удаление повторных
    if (entrances.size() == 0) {
        std::cout << "NO ENTRANCES!";
    }

    for (int i = 0; i < entrances.size(); i++) {
        std::pair<int, int> IDBetweenClusterCurEntrances = std::make_pair(entrances[i].getIdCluster_1(), entrances[i].getIdCluster_2());
        for (int j = 0; j < entrances.size(); j++) {
            if ((IDBetweenClusterCurEntrances.first==entrances[j].getIdCluster_2())&&(IDBetweenClusterCurEntrances.second==entrances[j].getIdCluster_1())) {
                entrances.erase(entrances.begin() + j);
                break;
            }
        }
    }
    return entrances;
}
/*елси изменить создания l1 l2 - для каждой вершины, добавляемой в вектор,
сохранять еще и координаты этой вершины, тогда мы по ним сможем найти нужную вершину в map
и проверить ее на равенство нулю*/

void createGraph(std::vector<Entrance>& entrances, int sizeMap, /**/Map &maze /**/, Graph& G) {
    //Graph G;

    std::vector<bool> visit_1;
    int tmp2 = sizeMap * sizeMap;
    for (int i = 0; i < tmp2; i++) {
        visit_1.push_back(false);
    }
    // ноды и inter ребра

        for (int i = 0; i < entrances.size(); i++) {
            Entrance currEntrance = entrances[i];

            //**/
            if (maze.getMaze()[currEntrance.getL1_s().first][currEntrance.getL1_s().second] == 0 &&
                maze.getMaze()[currEntrance.getL1_e().first][currEntrance.getL1_e().second] == 0)
                continue;
            //**/
            int currLen = currEntrance.getLenght();
            switch (currLen) {
            case 1: {
                int x = currEntrance.getL1_s().first;
                int y = currEntrance.getL1_s().second;
                //Node node1(findVertex(x, y, sizeMap), currEntrance.getIdCluster_1());
                Node node1(x * sizeMap + y + 1, currEntrance.getIdCluster_1());
                x = currEntrance.getL2_s().first;
                y = currEntrance.getL2_s().second;
                //Node node2(findVertex(x, y, sizeMap), currEntrance.getIdCluster_2());
                Node node2(x * sizeMap + y + 1, currEntrance.getIdCluster_2());

                if (visit_1[node1.getId()] == false) {
                    G.addNode(node1);
                    visit_1[node1.getId()] = true;
                }
                if (visit_1[node2.getId()] == false) {
                    G.addNode(node2);
                    visit_1[node2.getId()] = true;
                }

                Edge e(node1, node2, 1.0, INTER);  // вес ребра = 1, потому что оно INTER
                G.addEdge(e);
                break;
            }
            case 2: {
                int x = currEntrance.getL1_s().first;
                int y = currEntrance.getL1_s().second;
                //Node node1(findVertex(x, y, sizeMap), currEntrance.getIdCluster_1());
                Node node1(x * sizeMap + y + 1, currEntrance.getIdCluster_1());
                x = currEntrance.getL2_s().first;
                y = currEntrance.getL2_s().second;
                //Node node2(findVertex(x, y, sizeMap), currEntrance.getIdCluster_2());
                Node node2(x * sizeMap + y + 1, currEntrance.getIdCluster_2());
                if (visit_1[node1.getId()] == false) {
                    G.addNode(node1);
                    visit_1[node1.getId()] = true;
                }
                if (visit_1[node2.getId()] == false) {
                    G.addNode(node2);
                    visit_1[node2.getId()] = true;
                }

                Edge e(node1, node2, 1.0, INTER);  // вес ребра = 1, потому что оно INTER
                G.addEdge(e);
                break;
            }
            case 3: {
                int x1 = currEntrance.getL1_s().first;
                int y1 = currEntrance.getL1_s().second;

                int x2 = currEntrance.getL1_e().first;
                int y2 = currEntrance.getL1_e().second;


                if (x1 != x2) {
                    //Node node1(findVertex(x1 + 1, y1, sizeMap), currEntrance.getIdCluster_1());
                    Node node1((x1 + 1) * sizeMap + y1 + 1, currEntrance.getIdCluster_1());
                    //Node node2(findVertex(currEntrance.getL2_s().first + 1, currEntrance.getL2_s().second, sizeMap), currEntrance.getIdCluster_2());
                    Node node2((currEntrance.getL2_s().first + 1) * sizeMap + currEntrance.getL2_s().second + 1, currEntrance.getIdCluster_2());

                    if (visit_1[node1.getId()] == false) {
                        G.addNode(node1);
                        visit_1[node1.getId()] = true;
                    }
                    if (visit_1[node2.getId()] == false) {
                        G.addNode(node2);
                        visit_1[node2.getId()] = true;
                    }

                    Edge e(node1, node2, 1.0, INTER);  // вес ребра = 1, потому что оно INTER
                    G.addEdge(e);
                }
                else {
                    //Node node1(findVertex(x1, y1 + 1, sizeMap), currEntrance.getIdCluster_1());
                    Node node1(x1 * sizeMap + y1 + 1 + 1, currEntrance.getIdCluster_1());
                    //Node node2(findVertex(currEntrance.getL2_s().first, currEntrance.getL2_s().second + 1, sizeMap), currEntrance.getIdCluster_2());
                    Node node2(currEntrance.getL2_s().first * sizeMap + currEntrance.getL2_s().second + 1 + 1, currEntrance.getIdCluster_2());

                    if (visit_1[node1.getId()] == false) {
                        G.addNode(node1);
                        visit_1[node1.getId()] = true;
                    }
                    if (visit_1[node2.getId()] == false) {
                        G.addNode(node2);
                        visit_1[node2.getId()] = true;
                    }

                    Edge e(node1, node2, 1.0, INTER);  // вес ребра = 1, потому что оно INTER
                    G.addEdge(e);
                }
                break;
            }
            case 4: {
                int x1 = currEntrance.getL1_s().first;
                int y1 = currEntrance.getL1_s().second;

                int x2 = currEntrance.getL1_e().first;
                int y2 = currEntrance.getL1_e().second;


                if (x1 != x2) {
                    //Node node1(findVertex(x1 + 1, y1, sizeMap), currEntrance.getIdCluster_1());
                    Node node1((x1 + 1)* sizeMap + y1 + 1, currEntrance.getIdCluster_1());
                    //Node node2(findVertex(currEntrance.getL2_s().first + 1, currEntrance.getL2_s().second, sizeMap), currEntrance.getIdCluster_2());
                    Node node2((currEntrance.getL2_s().first + 1)* sizeMap + currEntrance.getL2_s().second + 1, currEntrance.getIdCluster_2());

                    if (visit_1[node1.getId()] == false) {
                        G.addNode(node1);
                        visit_1[node1.getId()] = true;
                    }
                    if (visit_1[node2.getId()] == false) {
                        G.addNode(node2);
                        visit_1[node2.getId()] = true;
                    }

                    //G.addNode(node1);
                    //G.addNode(node2);
                    Edge e(node1, node2, 1.0, INTER);  // вес ребра = 1, потому что оно INTER
                    G.addEdge(e);
                }
                else {
                    //Node node1(findVertex(x1, y1 + 1, sizeMap), currEntrance.getIdCluster_1());
                    Node node1(x1* sizeMap + y1 + 1 + 1, currEntrance.getIdCluster_1());
                    //Node node2(findVertex(currEntrance.getL2_s().first, currEntrance.getL2_s().second + 1, sizeMap), currEntrance.getIdCluster_2());
                    Node node2(currEntrance.getL2_s().first * sizeMap + currEntrance.getL2_s().second + 1 + 1, currEntrance.getIdCluster_2());

                    if (visit_1[node1.getId()] == false) {
                        G.addNode(node1);
                        visit_1[node1.getId()] = true;
                    }
                    if (visit_1[node2.getId()] == false) {
                        G.addNode(node2);
                        visit_1[node2.getId()] = true;
                    }

                    //G.addNode(node1);
                    //G.addNode(node2);
                    Edge e(node1, node2, 1.0, INTER);  // вес ребра = 1, потому что оно INTER
                    G.addEdge(e);
                }
                break;
            }
            case 5: {
                int x1 = currEntrance.getL1_s().first;
                int y1 = currEntrance.getL1_s().second;

                int x2 = currEntrance.getL1_e().first;
                int y2 = currEntrance.getL1_e().second;


                if (x1 != x2) {
                    //Node node1(findVertex(x1 + 2, y1, sizeMap), currEntrance.getIdCluster_1());
                    Node node1((x1 + 2)* sizeMap + y1 + 1, currEntrance.getIdCluster_1());
                    //Node node2(findVertex(currEntrance.getL2_s().first + 2, currEntrance.getL2_s().second, sizeMap), currEntrance.getIdCluster_2());
                    Node node2((currEntrance.getL2_s().first + 2)* sizeMap + currEntrance.getL2_s().second + 1, currEntrance.getIdCluster_2());

                    if (visit_1[node1.getId()] == false) {
                        G.addNode(node1);
                        visit_1[node1.getId()] = true;
                    }
                    if (visit_1[node2.getId()] == false) {
                        G.addNode(node2);
                        visit_1[node2.getId()] = true;
                    }

                    //G.addNode(node1);
                    //G.addNode(node2);
                    Edge e(node1, node2, 1.0, INTER);  // вес ребра = 1, потому что оно INTER
                    G.addEdge(e);
                }
                else {
                    //Node node1(findVertex(x1, y1 + 2, sizeMap), currEntrance.getIdCluster_1());
                    Node node1(x1* sizeMap + y1 + 2 + 1, currEntrance.getIdCluster_1());
                    //Node node2(findVertex(currEntrance.getL2_s().first, currEntrance.getL2_s().second + 2, sizeMap), currEntrance.getIdCluster_2());
                    Node node2(currEntrance.getL2_s().first* sizeMap + currEntrance.getL2_s().second + 2 + 1, currEntrance.getIdCluster_2());

                    if (visit_1[node1.getId()] == false) {
                        G.addNode(node1);
                        visit_1[node1.getId()] = true;
                    }
                    if (visit_1[node2.getId()] == false) {
                        G.addNode(node2);
                        visit_1[node2.getId()] = true;
                    }

                    //G.addNode(node1);
                    //G.addNode(node2);
                    Edge e(node1, node2, 1.0, INTER);  // вес ребра = 1, потому что оно INTER
                    G.addEdge(e);
                }
                break;
            }
            case 6: {
                int x = currEntrance.getL1_s().first;
                int y = currEntrance.getL1_s().second;
                //Node node1(findVertex(x, y, sizeMap), currEntrance.getIdCluster_1());
                Node node1(x* sizeMap + y + 1, currEntrance.getIdCluster_1());
                x = currEntrance.getL2_s().first;
                y = currEntrance.getL2_s().second;
                //Node node2(findVertex(x, y, sizeMap), currEntrance.getIdCluster_2());
                Node node2(x* sizeMap + y + 1, currEntrance.getIdCluster_2());
                x = currEntrance.getL1_e().first;
                y = currEntrance.getL1_e().second;
                //Node node3(findVertex(x, y, sizeMap), currEntrance.getIdCluster_1());
                Node node3(x* sizeMap + y + 1, currEntrance.getIdCluster_1());
                x = currEntrance.getL2_e().first;
                y = currEntrance.getL2_e().second;
                //Node node4(findVertex(x, y, sizeMap), currEntrance.getIdCluster_2());
                Node node4(x* sizeMap + y + 1, currEntrance.getIdCluster_2());

                if (visit_1[node1.getId()] == false) {
                    G.addNode(node1);
                    visit_1[node1.getId()] = true;
                }
                if (visit_1[node2.getId()] == false) {
                    G.addNode(node2);
                    visit_1[node2.getId()] = true;
                }
                if (visit_1[node3.getId()] == false) {
                    G.addNode(node3);
                    visit_1[node3.getId()] = true;
                }
                if (visit_1[node4.getId()] == false) {
                    G.addNode(node4);
                    visit_1[node4.getId()] = true;
                }

                //G.addNode(node1);
                //G.addNode(node2);
                //G.addNode(node3);
                //G.addNode(node4);
                Edge e(node1, node2, 1.0, INTER);  // вес ребра = 1, потому что оно INTER
                Edge e1(node3, node4, 1.0, INTER);
                G.addEdge(e);
                G.addEdge(e1);
                break;
            }
            default: {
                std::cout << "Lenght of entrance > 6! ERORR!" << std::endl;
                break;
            }
            }
        }
    
    visit_1.clear();

    // intra ребра
    std::vector<bool> visit_2;
    int tmp = sizeMap * sizeMap; 
    for (int i = 0; i < tmp; i++) {
        visit_2.push_back(false);
    }

    for (int i = 0; i < G.getNodes().size(); i++) {
        Node currNode = G.getNodes()[i];
        visit_2[currNode.getId()] = true;
        std::vector<Node> nodesInCluster;
        for (int j = 0; j < G.getNodes().size(); j++) {
            if (i == j)
                continue;
            else {
                if (currNode.getClusterId() == G.getNodes()[j].getClusterId()) {
                    nodesInCluster.push_back(G.getNodes()[j]);
                }
            }
        }
        if (nodesInCluster.size() == 0)
            continue;
        for (int j = 0; j < nodesInCluster.size(); j++) {
            if (visit_2[nodesInCluster[j].getId()] == true)
                continue;
            int currVert = currNode.getId();
            int currFinish = nodesInCluster[j].getId();
            int currDist = manhettenDist(sizeMap, currVert, currFinish);  // поиск манхетановского расстояния от currNode до всех осатльных, которые нашлись в таком же кластере.
            Edge currE(currNode, nodesInCluster[j], currDist, INTRA);
            G.addEdge(currE);      
        }
        // потом уже пройденная становится curr 
    }
    visit_2.clear();
    //return G;
}
// поиск расстояний между двумя вершинами
std::pair<int, std::vector<Node>> AbstractDistance(Graph G, Node start, Node goal) {
    int startP = start.getId();
    int goalP = goal.getId();
    int n = (int)G.getNodes().size();
    double** matrix = G.createAbstractMatrixTrue();
    std::vector<std::pair<Node, int>> navigation = G.getNovigationMaskDraft();// первой значение - ID верщины в общей матрице, вторая - номер в матрице смежности для абстрактоного графа
    for (int i = 0; i < n; i++) { // задание start и goal для матрицы графа
        if (navigation[i].first.getId() == startP)
            startP = navigation[i].second;
        if (navigation[i].first.getId() == goalP)
            goalP = navigation[i].second;
    }
    std::vector<int> dist(n, INF);
    dist[startP] = 0;
    std::priority_queue<std::pair<int, int>> q;
    q.push(std::make_pair(0, startP));
    while (!q.empty()) {
        int len = -q.top().first;
        int v = q.top().second;
        q.pop();
        if (len > dist[v]) continue;
        for (int i = 0; i < n; i++) {
            int to = i;
            double lenght = matrix[v][i];
            if (dist[to] > dist[v] + lenght && lenght > 0) {
                dist[to] = dist[v] + lenght;
                q.push(std::make_pair(-dist[to], to));
            }
        }
    }
    int d = dist[goalP];
    // восстановление пути
    /*std::vector<int> path(n, 0);
    path[0] = goal;
    int k = 1;
    int weight = dist[goal];
    while (goal != start) {
        for (int i = 0; i < n; i++)
            if (matrix[i][goal] != 0) {
                int temp = weight - matrix[i][goal];
                if (temp == dist[i]) {
                    weight = temp;
                    goal = i;
                    path[k] = i;
                    k++;
                }
            }
    }*/
    std::vector<Node> path;
    path.push_back(navigation[goalP].first);
    int weight = dist[goalP];
    while (goalP != startP) {
        for (int i = 0; i < n; i++)
            if (matrix[i][goalP] != 0) {
                int temp = weight - matrix[i][goalP];
                if (temp == dist[i]) {
                    weight = temp;
                    goalP = i;
                    path.push_back(navigation[i].first);

                }
            }
    }
    std::reverse(path.begin(), path.end());
    std::pair<int, std::vector<Node>> ans = std::make_pair(d, path);
    return ans;
}
std::pair<double, std::vector<Node>> itemizePath(Map& maze, std::vector<Cluster> clusters, Node start, Node goal) {

    // проверка корректности пришедших нод
    std::pair<int, int> tmp1 = findPos(maze.getSize(), start.getId());
    std::pair<int, int> tmp2 = findPos(maze.getSize(), goal.getId());
    if (maze.getMaze()[tmp1.first][tmp1.second] == 0) {
        std::vector<std::pair<int, double>> neigbors_tmp1 = maze.findNeighbors(start.getId());
        for (int i = 0; i < neigbors_tmp1.size(); i++) {
            std::pair<int, int> tmp3 = findPos(maze.getSize(), neigbors_tmp1[i].first);
            if (maze.getMaze()[tmp3.first][tmp3.second] == 0)
                continue;
            else {
                start.setId(neigbors_tmp1[i].first);
                break;
            }
        }
    }
    if (maze.getMaze()[tmp2.first][tmp2.second] == 0) {
        std::vector<std::pair<int, double>> neigbors_tmp1 = maze.findNeighbors(goal.getId());
        for (int i = 0; i < neigbors_tmp1.size(); i++) {
            std::pair<int, int> tmp3 = findPos(maze.getSize(), neigbors_tmp1[i].first);
            if (maze.getMaze()[tmp3.first][tmp3.second] == 0)
                continue;
            else {
                goal.setId(neigbors_tmp1[i].first);
                break;
            }
        }
    }
    std::pair<int, int> tmp4 = findPos(maze.getSize(), start.getId());
    std::pair<int, int> tmp5 = findPos(maze.getSize(), goal.getId());



    Cluster cluster = clusters[start.getClusterId() - 1];
    std::pair<int, int> startPos = cluster.getStart();
    std::pair<int, int> endPos = cluster.getEnd();
    int clusterSize = endPos.first - startPos.first + 1;

    int mapSize = clusterSize * clusterSize;
    int** matrixV = maze.createClusterMatrix(cluster.getId() - 1, clusterSize); //вырезка из общей карты
    Map clusterMap(matrixV, clusterSize); //вырезка из общей карты типа Map
    Map clusterMaze(mapSize); // матрица-маска для нахождения соседей

    for (int i = 0; i < clusterMap.getSize(); i++) {
        for (int j = 0; j < clusterMap.getSize(); j++) {
            if (clusterMap.getMaze()[i][j] == 0)
                clusterMaze.setItem(i, j, 0);
                //clusterMaze.getMaze()[i][j] = 0;
        }
    }

    double** matrixW = new double* [mapSize];
    for (int i = 0; i < mapSize; i++) {
        matrixW[i] = new double[mapSize];
    }
    for (int i = 0; i < mapSize; i++) {
        for (int j = 0; j < mapSize; j++) {
            matrixW[i][j] = 0;
        }
    }
    std::vector<std::pair<Node, int>> navigation(mapSize);// первое значение - ID вершины, второе - индекс в рамках матрицы
    int k = 0;
    for (int i = 0; i < clusterSize; i++) {
        for (int j = 0; j < clusterSize; j++) {
            Node tmp(matrixV[i][j], cluster.getId());
            navigation[k].first = tmp;
            navigation[k].second = k;
            k++;
        }
    }

    for (int i = 0; i < clusterSize; i++) {
        for (int j = 0; j < clusterSize; j++) {
            int currentVert = clusterMaze.getMaze()[i][j];
            if (currentVert == 0)
                continue;
            std::vector<std::pair<int, double>> currNeighbors = clusterMaze.findNeighbors(currentVert);
            for (int k = 0; k < currNeighbors.size(); k++) {
                std::pair<int, double> currNeighbor = currNeighbors[k];
                matrixW[currentVert - 1][currNeighbor.first - 1] = currNeighbor.second;
            }
        }
    }

    // tmpPtintMatrix(matrixW, mapSize);
    
    int startP = start.getId();
    int goalP = goal.getId();
    int startPDuplicate = 0;
    int goalPDuplicate = 0;
    for (int i = 0; i < mapSize; i++) { // задание start и goal для матрицы графа
        if (navigation[i].first.getId() == startP)
            startPDuplicate = navigation[i].second;
        if (navigation[i].first.getId() == goalP)
            goalPDuplicate = navigation[i].second;
    }
    std::vector<double> dist(mapSize, INF);
    dist[startPDuplicate] = 0;
    std::priority_queue<std::pair<int, int>> q;
    q.push(std::make_pair(0, startPDuplicate));
    while (!q.empty()) {
        int len = -q.top().first;
        int v = q.top().second;
        q.pop();
        if (len > dist[v]) continue;
        for (int i = 0; i < mapSize; i++) {
            int to = i;
            double lenght = matrixW[v][i];
            if (dist[to] > dist[v] + lenght && lenght > 0) {
                dist[to] = dist[v] + lenght;
                q.push(std::make_pair(-dist[to], to));
            }
        }
    }
    double d = dist[goalPDuplicate];


    // ввостановление пути
    std::vector<Node> path;
    path.push_back(navigation[goalPDuplicate].first);
    double weight = dist[goalPDuplicate];
    while (goalPDuplicate != startPDuplicate) {
        for (int i = 0; i < mapSize; i++)
            if (matrixW[i][goalPDuplicate] != 0) {
                double kk = matrixW[i][goalPDuplicate];
                double temp = weight - matrixW[i][goalPDuplicate];
                if (std::fabs(temp - dist[i]) < std::numeric_limits<double>::epsilon()) {
                    weight = temp;
                    goalPDuplicate = i;
                    path.push_back(navigation[i].first);
                }
            }
    }
    std::reverse(path.begin(), path.end());
    std::pair<double, std::vector<Node>> ans = std::make_pair(d, path);
    return ans;
}

int eyristic_funсtion(int current, int  goal, int size) {
    return manhettenDist(size, current, goal);
}

std::pair<double, std::vector<Node>> AStarDistance(Map& maze, std::vector<Cluster> clusters, Node start, Node goal) {
    Cluster cluster = clusters[start.getClusterId() - 1];
    std::pair<int, int> startPos = cluster.getStart();
    std::pair<int, int> endPos = cluster.getEnd();
    int clusterSize = endPos.first - startPos.first + 1;

    int mapSize = clusterSize * clusterSize;
    int** matrixV = maze.createClusterMatrix(cluster.getId() - 1, clusterSize); //вырезка из общей карты
    Map clusterMap(matrixV, clusterSize); //вырезка из общей карты типа Map
    Map clusterMaze(mapSize); // матрица-маска для нахождения соседей

    for (int i = 0; i < clusterMap.getSize(); i++) {
        for (int j = 0; j < clusterMap.getSize(); j++) {
            if (clusterMap.getMaze()[i][j] == 0)
                clusterMaze.setItem(i, j, 0);
            //clusterMaze.getMaze()[i][j] = 0;
        }
    }

    double** matrixW = new double* [mapSize];
    for (int i = 0; i < mapSize; i++) {
        matrixW[i] = new double[mapSize];
    }
    for (int i = 0; i < mapSize; i++) {
        for (int j = 0; j < mapSize; j++) {
            matrixW[i][j] = 0;
        }
    }
    std::vector<std::pair<Node, int>> navigation(mapSize);// первое значение - ID вершины, второе - индекс в рамках матрицы
    int k = 0;
    for (int i = 0; i < clusterSize; i++) {
        for (int j = 0; j < clusterSize; j++) {
            Node tmp(matrixV[i][j], cluster.getId());
            navigation[k].first = tmp;
            navigation[k].second = k;
            k++;
        }
    }

    for (int i = 0; i < clusterSize; i++) {
        for (int j = 0; j < clusterSize; j++) {
            int currentVert = clusterMaze.getMaze()[i][j];
            if (currentVert == 0)
                continue;
            std::vector<std::pair<int, double>> currNeighbors = clusterMaze.findNeighbors(currentVert);
            for (int k = 0; k < currNeighbors.size(); k++) {
                std::pair<int, double> currNeighbor = currNeighbors[k];
                matrixW[currentVert - 1][currNeighbor.first - 1] = currNeighbor.second;
            }
        }
    }

    //tmpPtintMatrix(matrixW, mapSize);

    int startP = start.getId();
    int goalP = goal.getId();
    int startPDuplicate = 0;
    int goalPDuplicate = 0;
    for (int i = 0; i < mapSize; i++) { // задание start и goal для матрицы графа
        if (navigation[i].first.getId() == startP)
            startPDuplicate = navigation[i].second;
        if (navigation[i].first.getId() == goalP)
            goalPDuplicate = navigation[i].second;
    }
    std::vector<double> dist(mapSize, INF);
    dist[startPDuplicate] = 0;
    std::priority_queue<std::pair<int, int>> q;
    q.push(std::make_pair(0, startPDuplicate));
    while (!q.empty()) {
        int len = -q.top().first;
        int v = q.top().second;
        q.pop();
        if (len > dist[v]) continue;
        for (int i = 0; i < mapSize; i++) {
            int to = i;
            double lenght = matrixW[v][i];
            if (dist[to] > dist[v] + lenght && lenght > 0) {
                dist[to] = dist[v] + lenght;
                int euristic = eyristic_funсtion(v, goal.getId(), clusterSize);
                q.push(std::make_pair(-dist[to] + euristic, to));
            }
        }
    }
    double d = dist[goalPDuplicate];

    // ввостановление пути
    std::vector<Node> path;
    path.push_back(navigation[goalPDuplicate].first);
    double weight = dist[goalPDuplicate];
    while (goalPDuplicate != startPDuplicate) {
        for (int i = 0; i < mapSize; i++)
            if (matrixW[i][goalPDuplicate] != 0) {
                double kk = matrixW[i][goalPDuplicate];
                double temp = weight - matrixW[i][goalPDuplicate];
                if (std::fabs(temp - dist[i]) < std::numeric_limits<double>::epsilon()) {
                    weight = temp;
                    goalPDuplicate = i;
                    path.push_back(navigation[i].first);
                }
            }
    }
    std::reverse(path.begin(), path.end());
    std::pair<double, std::vector<Node>> ans = std::make_pair(d, path);
    return ans;
}
std::pair<double, std::vector<Node>> createFullPath(Map &maze, Graph G, std::vector<Cluster> clusters, Node start, Node goal) {
    std::pair<double, std::vector<Node>> fullPath;
    double d = 0;
    fullPath.second.push_back(start);
    std::pair<int, std::vector<Node>> abstractPath = AbstractDistance(G, start, goal);


    for (int i = 0; i < abstractPath.second.size(); i++) {
        if (i == abstractPath.second.size() - 1)
            break;
        Node localStart = abstractPath.second[i];
        Node localGoal = abstractPath.second[i + 1];

        std::pair<int, int> tmp1 = findPos(maze.getSize(), localStart.getId());
        std::pair<int, int> tmp2 = findPos(maze.getSize(), localGoal.getId());

        if (localStart.getClusterId() != localGoal.getClusterId()) {
            d += 1;

            if (fullPath.second.back().getId() != localStart.getId()) {
                fullPath.second.push_back(localStart);
            }
            fullPath.second.push_back(localGoal);
        }
        else {
            std::pair<double, std::vector<Node>> localPath = itemizePath(maze, clusters, localStart, localGoal);
            d += localPath.first;
            for (int i = 0; i < localPath.second.size(); i++)
                if (fullPath.second.back().getId() != localPath.second[i].getId())
                    fullPath.second.push_back(localPath.second[i]);
        }  
    }
    fullPath.first = d;
    return fullPath;
}
std::pair<double, std::vector<Node>> createFullPathAStar(Map &maze, Graph G, std::vector<Cluster> clusters, Node start, Node goal) {
    std::pair<double, std::vector<Node>> fullPath;
    double d = 0;
    fullPath.second.push_back(start);
    std::pair<int, std::vector<Node>> abstractPath = AbstractDistance(G, start, goal);
    for (int i = 0; i < abstractPath.second.size(); i++) {
        if (i == abstractPath.second.size() - 1)
            break;
        Node localStart = abstractPath.second[i];
        Node localGoal = abstractPath.second[i + 1];

        if (localStart.getClusterId() != localGoal.getClusterId()) {
            d += 1;

            if (fullPath.second.back().getId() != localStart.getId()) {
                fullPath.second.push_back(localStart);
            }
            fullPath.second.push_back(localGoal);
        }
        else {
            std::pair<double, std::vector<Node>> localPath = AStarDistance(maze, clusters, localStart, localGoal);
            d += localPath.first;
            for (int i = 0; i < localPath.second.size(); i++)
                if (fullPath.second.back().getId() != localPath.second[i].getId())
                    fullPath.second.push_back(localPath.second[i]);
        }  
    }
    fullPath.first = d;
    return fullPath;
}


int main()
{

    //Gdiplus::GdiplusStartupInput gdiplusStrtupInput;
    //ULONG_PTR gdiplusToken;
    //Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStrtupInput, NULL);
    //{
    //    Gdiplus::Bitmap input_bmp(TEXT("input_500_500_3.bmp"));
    //    Node n1(250, 25);
    //    Node n2(249750, 2475);
    //    std::pair<INT, INT> currentCord;
    //    currentCord = findPos(500, n1.getId());
    //    input_bmp.SetPixel(currentCord.second, currentCord.first, Gdiplus::Color::Green);

    //    CLSID pngClsid;
    //    GetEncoderClsid(TEXT("image/bmp"), &pngClsid);
    //    input_bmp.Save(TEXT("outputDijkstra_500_500_3.bmp"), &pngClsid, NULL);
    //}
    //Gdiplus::GdiplusShutdown(gdiplusToken);
 

    auto impass = read_image(TEXT("input_500_500_3.bmp"));
    int size = impass.back();
    impass.pop_back();
    Map maze(size * size, impass);
    std::vector<Cluster> clusters = maze.clusteringMaze(25); // !
    std::cout << "I AM GOOD!" << std::endl;

    double start = omp_get_wtime();
    std::vector<Entrance> entrance = findEnterance(clusters, maze);
    double end = omp_get_wtime();
    std::cout << "time findEntrance = " << end - start << std::endl;
    std::cout << "I AM GOOD!" << std::endl;
    Graph GNew;

    start = omp_get_wtime();
    createGraph(entrance, maze.getSize(), maze, GNew);
    end = omp_get_wtime();
    std::cout << "time crateGraph = " << end - start << std::endl;

    std::cout << "I AM GOOD!" << std::endl;
    std::cout << "SIZE = " << GNew.getNodes().size() << std::endl << std::endl;
    //for (int i = 0; i < GNew.getNodes().size(); i++)
        //std::cout << GNew.getNodes()[i].getId() << " from cluster " << GNew.getNodes()[i].getClusterId() << std::endl;
    Node n1(250, 25);
    Node n2(249750,2475);

    GNew.addNodeAndEdges(n1, maze.getSize());
    GNew.addNodeAndEdges(n2, maze.getSize());
    std::cout << "I AM GOOD!" << std::endl;

    start = omp_get_wtime();
    std::pair<double, std::vector<Node>> Dijkstra = createFullPath(maze, GNew, clusters, n1, n2);
    end = omp_get_wtime();
    std::cout << "time Dijkstra = " << end - start << std::endl;

    std::cout << "I AM GOOD!" << std::endl;

    start = omp_get_wtime();
    std::pair<double, std::vector<Node>> AStar = createFullPathAStar(maze, GNew, clusters, n1, n2);
    end = omp_get_wtime();
    std::cout << "time AStar = " << end - start << std::endl;

    std::cout << "I AM GOOD!" << std::endl;
    draw_path(n1, n2, Dijkstra.second, maze.getSize(), TEXT("input_500_500_3.bmp"), TEXT("outputDijkstra_500_500_3_cluster_10.bmp"));
    draw_path(n1, n2, AStar.second, maze.getSize(), TEXT("input_500_500_3.bmp"), TEXT("outAStar_500_500_3_cluster_10.bmp"));

    /*std::vector<int> impass = {};
    Map maze(250000, impass);
    std::vector<Cluster> clusters = maze.clusteringMaze(10); // !
    std::cout << "I AM GOOD!" << std::endl;
    std::vector<Entrance> entrance = findEnterance(clusters, maze);
    std::cout << "I AM GOOD!" << std::endl;
    Graph GNew;
    GNew = createGraph(entrance, maze.getSize(), maze);
    std::cout << "I AM GOOD!" << std::endl;
    Node n1(1, 1);
    Node n2(250000, 2500);
    GNew.addNodeAndEdges(n1, maze.getSize());
    GNew.addNodeAndEdges(n2, maze.getSize());
    std::cout << "I AM GOOD!" << std::endl;
    std::pair<int, std::vector<Node>> path = AbstractDistance(GNew, n1, n2);
    std::pair<double, std::vector<Node>> testFullPath = createFullPath(maze, GNew, clusters, n1, n2);
    std::pair<double, std::vector<Node>> testFullPathAStar = createFullPathAStar(maze, GNew, clusters, n1, n2);*/
    return 0;
}


// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.

#pragma once
#include <iostream>
#include <vector>

std::pair<int, int> findPos(int size, int vert) {
	int str = 0;
	int stlb = 0;
	if ((vert / size) * size == vert)
		str = vert / size - 1;
	else
		str = vert / size;
	if ((vert % size) == 0)
		stlb = size - 1;
	else
		stlb = vert % size - 1;
	std::pair<int, int> pos;
	pos.first = str;
	pos.second = stlb;
	return pos;
}
int findVertex(int i, int j, int size) {
	int currI = 0;
	int currJ = 0;
	int vert = 0;
	if (i == 0)
		vert = j + 1;
	else {
		while (currI < i) {
			vert = vert + size;
			currI++;
		}
		if (j == (size - 1))
			vert = vert + size;
		else
			while (currJ <= j) {
				vert = vert + 1;
				currJ++;
			}
	}
	return vert;
}
int manhettenDist(int size, int vert, int finish) {
	std::pair<int, int> finishPos = findPos(size, finish);
	std::pair<int, int> vertPos = findPos(size, vert);
	int num = 0;
	if (finishPos.first == vertPos.first)
		goto next;
	if (finishPos.first < vertPos.first)
		while (finishPos.first < vertPos.first) {
			num++;
			vertPos.first--;
		}
	else
		while (finishPos.first > vertPos.first) {
			num++;
			vertPos.first++;
		}
next:
	if (finishPos.second == vertPos.second)
		goto ret;
	if (finishPos.second < vertPos.second)
		while (finishPos.second < vertPos.second) {
			num++;
			vertPos.second--;
		}
	else
		while (finishPos.second > vertPos.second) {
			num++;
			vertPos.second++;
		}
ret:
	return (num * 10);
}
class Cluster {
private:
	int id;
	std::pair<int, int> start;
	std::pair<int, int> end;
public:
	Cluster() :id{ NULL } {}
	Cluster(int id, std::pair<int, int> s, std::pair<int, int> e) {
		this->id = id;
		start = s;
		end = e;
	}
	int getId() { return id; };
	std::pair<int, int> getStart() { return start; };
	std::pair<int, int> getEnd() { return end; };
	void print() {
		std::cout << "===Cluster===" << std::endl;
		std::cout << "id: " << id << std::endl;
		std::cout << "start: (" << start.first << "," << start.second << ")" << std::endl; // номер строки и номер столбца (не самой ячейки)
		std::cout << "end: (" << end.first << "," << end.second << ")" << std::endl;  // ---=---=---=---
	}
};
class Entrance {
private:
	std::pair<int, int> l1_s;  // строка-столбец первой клетки в 1 кластере
	std::pair<int, int> l1_e;  // строка-столбец последней клетки в 1 кластере
	std::pair<int, int> l2_s;  // строка-столбец первой клетки во 2 кластере
	std::pair<int, int> l2_e;  // строка-столбец последней клетки во 2 кластере
	int lenght;
	int idClauster_1;
	int idClauster_2;
public:
	Entrance() :lenght{ NULL }, idClauster_1{NULL}, idClauster_2{ NULL } {}
	Entrance(std::pair<int, int> l1_s, std::pair<int, int> l1_e, std::pair<int, int> l2_s, std::pair<int, int> l2_e, int lenght, int idCluster1, int idCluster2) {
		this->l1_s = l1_s;
		this->l1_e = l1_e;
		this->l2_s = l2_s;
		this->l2_e = l2_e;
		this->lenght = lenght;
		idClauster_1 = idCluster1;
		idClauster_2 = idCluster2;
	}
	~Entrance() {};
	std::pair<int, int> getL1_s() { return l1_s; }
	std::pair<int, int> getL1_e() { return l1_e; }
	std::pair<int, int> getL2_s() { return l2_s; }
	std::pair<int, int> getL2_e() { return l2_e; }
	int getLenght() { return lenght; }
	int getIdCluster_1() { return idClauster_1; }
	int getIdCluster_2() { return idClauster_2; }
	void print() {
		std::cout << "===Entrance===" << std::endl;
		std::cout << "Between clasters: " << this->idClauster_1 << " and " << this->idClauster_2 << std::endl;
		std::cout << "Len entrance = " << this->lenght << std::endl;
		std::cout << "Start point left: (" << l1_s.first << "," << l1_s.second << ")" << std::endl;
		std::cout << "End point left: (" << l1_e.first << "," << l1_e.second << ")" << std::endl;
		std::cout << "Start point right: (" << l2_s.first << "," << l2_s.second << ")" << std::endl;
		std::cout << "End point right: (" << l2_e.first << "," << l2_e.second << ")" << std::endl;
	}
};

class Map {
private:
	std::vector<std::vector<int>> maze;
	int size;
public:
	Map() :size{NULL} {};
	Map(int n) {  // n - количество вершин для квадратной матрицы (25 вершин - матрица 5х5)
		this->size = (int)std::sqrt(n);
		maze.resize(this->size);
		for (int i = 0; i < this->size; i++) {
			maze[i].resize(this->size);
		}
		std::vector<int> tmp;
		for (int i = 1; i <= n; i++)
			tmp.push_back(i);
		int k = 0;
		for (int i = 0; i < this->size; i++) {
			for (int j = 0; j < this->size; j++) {
				maze[i][j] = tmp[k];
				k++;
			}
		}
	}
	Map(int n, std::vector<int> impassable) {  // n - количество вершин для квадратной матрицы (25 вершин - матрица 5х5), impassble - непроходимые вершины
		this->size = static_cast<int> (std::sqrt(n));
		maze.resize(this->size);
		for (int i = 0; i < this->size; i++) {
			maze[i].resize(this->size);
		}
		std::vector<int> tmp;
		for (int i = 1; i <= n; i++)
			tmp.push_back(i);
		for (int i = 0; i < impassable.size(); i++)
			tmp[impassable[i] - 1] = 0;
		int k = 0;
		for (int i = 0; i < this->size; i++) {
			for (int j = 0; j < this->size; j++) {
				maze[i][j] = tmp[k];
				k++;
			}
		}
	}
	Map(std::vector<std::vector<int>>& M) {
		this->size = (int)M.size();
		this->maze = M;
	}
	Map(int** matrix, int n) {
		this->size = n;
		this->maze.resize(n);
		for (int i = 0; i < n; i++) {
			maze[i].resize(n);
		}
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				this->maze[i][j] = matrix[i][j];
			}
		}
	}
	~Map() {};

	int getSize() { return this->size; }
	std::vector<std::vector<int>>& getMaze() { return this->maze; };
	int getItem(int i, int j) { return this->maze[i][j]; }
	void setItem(int i, int j, int value) {
		this->maze[i][j] = value;
	}
	std::vector<std::pair<int, double>> findNeighbors(int vertex) {
		std::pair<int, int> pos = findPos(this->size, vertex);
		int* masI = new int[8];
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
		masJ[7] = pos.second + 1;
		std::vector<std::pair<int, double>> neighbors;
		for (int i = 0; i < 8; i++) {
			if (masI[i] < 0 || masI[i] == this->size || masJ[i] < 0 || masJ[i] == this->size)
				continue;
			if (maze[masI[i]][masJ[i]] == 0)  // клетка - препядствие не помещается в вектор соседей
				continue;
			else {
				std::pair<int, double> tmp;
				tmp.first = findVertex(masI[i], masJ[i], this->size);
				neighbors.push_back(tmp);
			}
		}
		// подсчет расстояний в массиве соседей
		for (int i = 0; i < neighbors.size(); i++) {
			std::pair<int, int> currNeighborPos = findPos(this->size, neighbors[i].first);
			if ((currNeighborPos.first == pos.first) || (currNeighborPos.second == pos.second)) {
				neighbors[i].second = 10.0;
			}
			else
				neighbors[i].second = sqrt(200);
		}
		delete[] masI;
		delete[] masJ;
		return neighbors;
	}

	std::vector<Cluster> clusteringMaze(int clusterSize) { // board - массив вершин, после которых "граница" класетра, разбивается карата (лабиринт - map)
		std::vector<int> board;  // end vertex in cluster
		//board.push_back(0);
		for (int i = 0; i < this->size; i++) {
			if (((i + 1) % clusterSize) != 0)
				continue;
			else
				board.push_back(i);
		}
		std::vector<int> nearBord;  // start vertex in cluster
		nearBord.push_back(0);
		for (int i = 0; i < board.size(); i++)
			nearBord.push_back(board[i] + 1);

		std::vector<Cluster> clusters;
		int id = 1;
		for (int i = 0; i < board.size(); i++) {
			for (int j = 0; j < board.size(); j++) {
				std::pair<int, int> s = std::make_pair(nearBord[i],nearBord[j]);
				std::pair<int, int> e = std::make_pair(board[i], board[j]);
				Cluster tmp(id, s, e);
				clusters.push_back(tmp);
				id++;
			}
		}
		return clusters;
	}
	int** createClusterMatrix(int clusterId, int clusterSize) {
		std::vector<Cluster> clusters = this->clusteringMaze(clusterSize);
		Cluster neededCluster = clusters[clusterId];
		std::pair<int, int> start = neededCluster.getStart();
		std::pair<int, int> end = neededCluster.getEnd();
		int** matrix = new int* [clusterSize];
		for (int i = 0; i < clusterSize; i++) {
			matrix[i] = new int[clusterSize];
		}
		int mPosX = 0;
		int mPosY = 0;
		for (int i = start.first; i <= end.first; i++) {
			for (int j = start.second; j <= end.second; j++) {
				matrix[mPosX][mPosY] = this->maze[i][j];
				mPosY++;
			}
			mPosX++;
			mPosY = 0;
		}
		return matrix;
	}

	double** getFullMatrix() {
		int n = size * size;
		double** matrix = new double* [n];
		for (int i = 0; i < n; i++) {
			matrix[i] = new double[n];
		}

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				matrix[i][j] = 0;
			}
		}

		int vert = 0;
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				std::vector<std::pair<int, double>> currentNeighbors = this->findNeighbors(maze[i][j]);
				for (int k = 0; k < currentNeighbors.size(); k++) {
					matrix[vert][currentNeighbors[k].first-1] = currentNeighbors[k].second;
				}
				vert++;
			}
		}

		//std::cout << "NEW MATRIX:" << std::endl;
		//for (int i = 0; i < n; i++) {
		//	for (int j = 0; j < n; j++) {
		//		std::cout << matrix[i][j] << " ";
		//	}
		//	std::cout << std::endl;
		//}
		return matrix;
	}
	void printMaze() {
		for (int i = 0; i < this->size; i++) {
			for (int j = 0; j < this->size; j++) {
				std::cout << maze[i][j] << " ";
			}
			std::cout << std::endl;
		}
	}
};


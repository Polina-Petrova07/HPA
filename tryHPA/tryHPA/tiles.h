#pragma once
#include<iostream>
#include <iomanip>
#include <fstream>
#include <vector>

#include "map.h"

enum typeEdge {INTER, INTRA};
class Node {
protected:
	
private:
	int id;  //  клетка в map (по ней мы можем найти row и col)
	int ClusterId;
	//double weight; все строчки в классе Tiles, исполбзующие это поле закомментированы!
public:
	Node() : id{ NULL }, ClusterId{NULL} { }
	Node(int id, int ClusterId) {
		this->id = id;
		this->ClusterId = ClusterId;
	}
	int getId() { return this->id; }
	int getClusterId() { return this->ClusterId; }
	void setId(int id) { this->id = id; }
	~Node() {};
	void print() {
		std::cout << "===Node===" << std::endl;
		std::cout << "Id: " << this->id << std::endl;
		std::cout << "ClusterId: " << this->ClusterId << std::endl;
	}
	bool operator ==(const Node& n) {
		if (this->id == n.id)
			return true;
		else
			return false;
	}
};
class Edge {
private:
	Node one;
	Node two;
	double weight;
	typeEdge type;
public:
	Edge() : weight{ NULL }, type{} {}
	Edge(Node one, Node two, double w, typeEdge type) {
		this->one = one;
		this->two = two;
		weight = w;
		this->type = type;
	}
	Node getOneNode() { return this->one; }
	Node getTwoNode() { return this->two; }
	double getWeight() { return this->weight; }
	typeEdge getTypeEdge() { return this->type; }
	void setOneNode(Node node) { this->one = node; }
	void setTwoNode(Node node) { this->two = node; }
	void setWeight(double w) { weight = w; }
	void setType(typeEdge type) { this->type = type; }
	~Edge() {};
	void print() {
		std::cout << "=====Edge=====" << std::endl;
		std::cout << "*Node 1*" << std::endl;
		this->one.print();
		std::cout << "*Node 2*" << std::endl;
		this->two.print();
		std::cout << std::endl;
		std::cout << "Weight = " << this->weight << std::endl;
		std::cout << "Type of Edge: " << this->type;
		std::cout << std::endl << std::endl;
	}
};

class Graph {
private:
	std::vector<Node> nodes;
	std::vector<Edge> edges;
public:
	Graph() {}
	Graph(std::vector<Node> nods, std::vector<Edge> edges) {
		this->nodes = nods;
		this->edges = edges;
	}
	std::vector<Node>& getNodes() { return this->nodes; }
	std::vector<Edge>& getEdges() { return this->edges; }
	void addNode(Node n) {
		nodes.push_back(n);
	}
	void addEdge(Edge e) {
		edges.push_back(e);
	}
	~Graph() {
		this->nodes.clear();
		this->edges.clear();
	}
	void addNodeAndEdges(Node node, int sizeMap) { // может быть вызвана только после создания графа
		int currId = node.getClusterId();
		std::vector<Node> currNodeInCluster;
		for (int i = 0; i < nodes.size(); i++) {
			if (nodes[i].getClusterId() == currId)
				currNodeInCluster.push_back(nodes[i]);
		}
		for (int i = 0; i < currNodeInCluster.size(); i++) {
			int tmp = manhettenDist(sizeMap, node.getId(), currNodeInCluster[i].getId());
			Edge tmpE(node, currNodeInCluster[i], tmp, INTRA);
			edges.push_back(tmpE);
		}
		nodes.push_back(node);
	}
	std::vector<std::pair<int, int>> getNovigationMask() {
		std::vector<std::pair<int, int>> navigation(nodes.size()); // первой значение - ID верщины в общей матрице, вторая - номер в матрице смежности для абстрактоного графа
		for (int i = 0; i < nodes.size(); i++) {
			navigation[i].first = nodes[i].getId();
			navigation[i].second = i;
		}
		return navigation;
	}
	std::vector<std::pair<Node, int>> getNovigationMaskDraft() {
		std::vector<std::pair<Node, int>> navigation(nodes.size()); // первой значение - ID верщины в общей матрице, вторая - номер в матрице смежности для абстрактоного графа
		for (int i = 0; i < nodes.size(); i++) {
			navigation[i].first = nodes[i];
			navigation[i].second = i;
		}
		return navigation;
	}
	double** createAbstractMatrixTrue() {
		double** matrix = new double* [nodes.size()];
		for (int i = 0; i < nodes.size(); i++) {
			matrix[i] = new double[nodes.size()];
		}
		for (int i = 0; i < nodes.size(); i++) {
			for (int j = 0; j < nodes.size(); j++) {
				matrix[i][j] = 0;
			}
		}
		std::vector<std::pair<int, int>> navigation(nodes.size()); // первой значение - ID верщины в общей матрице, вторая - номер в матрице смежности для абстрактоного графа
		for (int i = 0; i < nodes.size(); i++) {
			navigation[i].first = nodes[i].getId();
			navigation[i].second = i;
		}
		for (int i = 0; i < edges.size(); i++) {
			int currFirst = edges[i].getOneNode().getId();
			int currSecond = edges[i].getTwoNode().getId();
			int currI = 0, currJ = 0;
			for (int j = 0; j < navigation.size(); j++) {
				if (navigation[j].first == currFirst)
					currI = navigation[j].second;
				if (navigation[j].first == currSecond)
					currJ = navigation[j].second;
			}
			matrix[currI][currJ] = matrix[currJ][currI] = edges[i].getWeight();
		}

		// строящийся граф неориентированный (матрица симметрична)
		return matrix;
	}
	void print() {
		int flag;
		std::cout << "You need to see all nodes? (if <<NO = 0>> here will be only edges)" << std::endl;
		std::cin >> flag;
		if (flag == 0) {
			for (int i = 0; i < edges.size(); i++)
				edges[i].print();
		}
		else {
			for (int i = 0; i < nodes.size(); i++) {
				nodes[i].print();
				std::cout << std::endl;
			}
			std::cout << std::endl;
			for (int i = 0; i < edges.size(); i++) {
				edges[i].print();
				std::cout << std::endl;
			}	
		}	
	}
};

class Tiles {
private:
	std::vector<std::vector<Node>> Tile;
	int size;
public:
	Tiles() :size{NULL} {};
	Tiles(std::vector<std::vector<double>> M) {
		this->size = (int)M.size();
		Tile.resize(this->size);
		for (int i = 0; i < M.size();i++)
			Tile[i].resize(this->size);

		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				//Tile[i][j].id = j;
				//Tile[i][j].row = i;
				//Tile[i][j].cell = j;
				//if (M[i][j] != 0)
					//Tile[i][j].weight = M[i][j];
				//else
					//Tile[i][j].weight = 0;
			}
		}
	}
	Tiles(Map M) {
		this->size = M.getSize() * M.getSize();
		Tile.resize(this->size);
		for (int i = 0; i < this->size; i++)
			Tile[i].resize(this->size);

		for (int i = 0; i < this->size; i++) {
			for (int j = 0; j < this->size; j++) {
				//Tile[i][j].id = j;
				//Tile[i][j].row = i;
				//Tile[i][j].cell = j;
				//Tile[i][j].weight = 0;
			}
		}

		int mazeSize = M.getSize();
		int vertex = 1;
		int row = 0;
		for (int i = 0; i < mazeSize; i++) {
			for (int j = 0; j < mazeSize; j++) {
				if (M.getMaze()[i][j] != 0) {
					std::vector<std::pair<int, double>> tmp;
					tmp = M.findNeighbors(vertex);
					for (int k = 0; k < tmp.size(); k++) {
						//this->Tile[row][tmp[k].first - 1].weight = tmp[k].second;
						//std::pair<int, int> currPos = findPos(M.getSize(), tmp[j].first);
						//this->Tile[currPos.first][currPos.second].weight = tmp[j].second;
					}
					vertex++;
					row++;
				}
				else { vertex++; row++; continue; };
			}
		}
	}

	int getSize() { return this->size; };

	std::vector<std::vector<Node>> getTile() { return this->Tile; };

	void print() {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				//std::cout << i << "-ya vertex connected with " << j << " by rib = " << Tile[i][j].weight << " (" << Tile[i][j].row << ", " << Tile[i][j].cell << ") ";
				std::cout << std::endl;
			}
			std::cout << std::endl;
		}
	}

	void printMatrixInFile() {
		std::fstream file("C:\\Users\\user\\Desktop\\test.txt");
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				//file << std::setw(8) << this->Tile[i][j].weight;
				file << " ";
			}
			file << std::endl;
		}
		file.close();
	}
};

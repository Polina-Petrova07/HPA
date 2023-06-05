#pragma once
#include<omp.h>
#include "map.h"
const int INF = 1000000000;

class CRSType {
private:
	std::vector<int> Adjncy; // neigbors
	std::vector<int> Xadj;
	std::vector<double> Eweights;
public:
	CRSType(Map maze) {
		this->Xadj.push_back(0);
		for (int i = 0; i < maze.getSize(); i++) {
			for (int j = 0; j < maze.getSize(); j++) {
				std::vector<std::pair<int, double>> currentNeighbors = maze.findNeighbors(maze.getMaze()[i][j]);
				this->Xadj.push_back(Xadj.back() + currentNeighbors.size());
				for (int k = 0; k < currentNeighbors.size(); k++) {
					this->Adjncy.push_back(currentNeighbors[k].first);
					this->Eweights.push_back(currentNeighbors[k].second);
				}

			}
		}
	}
	CRSType(double** matrix, int size) {
		this->Xadj.push_back(0);
		int num = 0;
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				if (matrix[i][j] != 0) {
					num++;
					this->Adjncy.push_back(j + 1);
					this->Eweights.push_back(matrix[i][j]);
				}
			}
			this->Xadj.push_back(Xadj.back() + num);
			num = 0;
		}
	}
	double Dijkstra(int start, int goal) {
		double time, t1, t2;
		start--;
		goal--;
		int numNodes = this->Xadj.size() - 1;
		t1 = omp_get_wtime();
		std::vector<double> dist(numNodes, INF);
		dist[start] = 0;
		std::priority_queue<std::pair<double, int>> q;
		q.push(std::make_pair(0, start));
		//std::pair<int, int> indexV = std::make_pair(this->Xadj[start], this->Xadj[start + 1]);
		while (!q.empty()) {
			double len = -q.top().first;
			int v = q.top().second;
			std::pair<int, int> indexV = std::make_pair(this->Xadj[v], this->Xadj[v + 1]); //
			q.pop();
			if (len > dist[v]) continue;
			for (int i = indexV.first; i < indexV.second; i++) {
				int to = this->Adjncy[i] - 1;
				double lenght = this->Eweights[i];
				if (dist[to] > dist[v] + lenght && lenght > 0) {
					dist[to] = dist[v] + lenght;
					q.push(std::make_pair(-dist[to], to));
				}
			}
			//std::pair<int, int> indexV = std::make_pair(this->Xadj[v], this->Xadj[v + 1]); // 
		}
		double d = dist[goal];
		t2 = omp_get_wtime();

		time = t2 - t1;
		return time;
	}


	void printData() {
		std::cout << "Xadj: ";
		for (int i = 0; i < Xadj.size(); i++)
			std::cout << Xadj[i] << " ";
		std::cout << std::endl;

		std::cout << "Adjncy: ";
		for (int i = 0; i < Adjncy.size(); i++)
			std::cout << Adjncy[i] << " ";
		std::cout << std::endl;

		std::cout << "Eweights: ";
		for (int i = 0; i < Eweights.size(); i++)
			std::cout << Eweights[i] << " ";
		std::cout << std::endl;
	}
};

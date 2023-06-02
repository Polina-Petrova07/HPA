#pragma once
#include "map.h"

class CRSType {
private:
	std::vector<int> Adjncy;
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
	void Dijkstra() {

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

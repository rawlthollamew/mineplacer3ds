#pragma once
#include <3ds.h>
#include <citro2d.h>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <algorithm>

struct Score
{
	std::string name;
	int finalScore;
	int epochSet;
};

class Scores
{
public:
	Scores(std::string _filename);
	std::vector<Score> lb;
	void loadScores();
	void insertScore(int _finalTime);
private:
	int lbSize = 10;
	void saveScore(std::string _name, int _finalTime);
	std::string filename;
};
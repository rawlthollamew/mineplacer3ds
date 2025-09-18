#include "scores.h"

Scores::Scores(std::string _filename)
{
	filename = _filename;
	loadScores();
}

void Scores::loadScores()
{
	FILE* scoresFile = fopen(filename.c_str(), "r");
	
	// fix a crash caused by scoresFile not existing beforehand.
	if (!scoresFile) return;

	std::string line;
	
	char buf[256];
	lb.clear();
	
	while (fgets(buf, sizeof(buf), scoresFile))
	{
		std::string line(buf);
		if (!line.empty() && line.back() == '\n') line.pop_back();

		auto pos1 = line.find(',');
		auto pos2 = line.find(',', pos1 + 1);

		if (pos1 == std::string::npos || pos2 == std::string::npos) continue;

		Score newScore;
		newScore.name = line.substr(0, pos1);
		newScore.finalScore = std::stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));
		newScore.epochSet = std::stoi(line.substr(pos2 + 2));
		
		lb.push_back(newScore);
	}
	fclose(scoresFile);

	std::sort(lb.begin(), lb.end(), [](Score a, Score b)
	{
		return a.finalScore < b.finalScore;
	});

	if ((int)lb.size() > lbSize) lb.resize(lbSize);
}

void Scores::insertScore(int _finalTime)
{
	std::string hintText = "Please enter your name to submit your score (" + std::to_string(_finalTime) + ")";
	
	SwkbdState swkbd;
	char buf[60];
	SwkbdButton button = SWKBD_BUTTON_NONE;
	
	swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 2, -1);
	swkbdSetValidation(&swkbd, SWKBD_NOTEMPTY_NOTBLANK, 0, 0);
	swkbdSetFeatures(&swkbd, SWKBD_ALLOW_HOME | SWKBD_ALLOW_RESET | SWKBD_ALLOW_POWER);
	swkbdSetHintText(&swkbd, hintText.c_str());

	bool shouldQuit = false;
	buf[0] = 0;
	do
	{
		swkbdSetInitialText(&swkbd, buf);
		button = swkbdInputText(&swkbd, buf, sizeof(buf));
		if (button != SWKBD_BUTTON_NONE) break;

		SwkbdResult res = swkbdGetResult(&swkbd);
		if (res == SWKBD_RESETPRESSED)
		{
			shouldQuit = true;
			aptSetChainloaderToSelf();
			return;
		}
		else if (res != SWKBD_HOMEPRESSED && res != SWKBD_POWERPRESSED) return;
	
		shouldQuit = !aptShouldClose();

	} while (!shouldQuit);

	saveScore(buf, _finalTime);
	// reload scores
	loadScores();
}

void Scores::saveScore(std::string _name, int _finalTime)
{
	Score newScore;
	newScore.name = _name;
	newScore.finalScore = _finalTime;
	newScore.epochSet = time(0);
	
	FILE* scoresFile = fopen(filename.c_str(), "a");
	if (!scoresFile) return;
	fprintf(scoresFile, "%s,%d,%d\n", newScore.name.c_str(), newScore.finalScore, newScore.epochSet);
	fclose(scoresFile);
}
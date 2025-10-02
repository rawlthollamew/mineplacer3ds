#include "replay_manager.h"

ReplayManager::ReplayManager(C2D_SpriteSheet& _sheet, Difficulty _difficulty, std::string _directory)
	: player(_sheet, _difficulty), directory(_directory)
{
	mkdir(_directory.c_str(), 0777);
	getScores(_difficulty);
}

std::string ReplayManager::playerInput(int _finalTime)
{
	std::string hintText = "Please enter your name to submit your score (" + std::to_string(_finalTime) + ")";

	SwkbdState swkbd;
	// usernames will be 40 bytes long.
	char buf[40];
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
			return std::string(" ");
		}
		else if (res != SWKBD_HOMEPRESSED && res != SWKBD_POWERPRESSED) return std::string(" ");
	
		shouldQuit = !aptShouldClose();

	} while (!shouldQuit);

	return std::string(buf);
}

std::string ReplayManager::diffToString(Difficulty _difficulty)
{
	if (_difficulty.mineCount == Settings::easyDiff.mineCount &&
		_difficulty.dimentions == Settings::easyDiff.dimentions) return "easy";
	else if (_difficulty.mineCount == Settings::mediumDiff.mineCount && 
		_difficulty.dimentions == Settings::mediumDiff.dimentions) return "medium";
	else if (_difficulty.mineCount == Settings::hardDiff.mineCount &&
		_difficulty.dimentions == Settings::hardDiff.dimentions) return "hard";
	else return "custom";
}

std::string ReplayManager::epochToDate(time_t _epoch)
{
	char result[50];
	strftime(result, sizeof(result), "%F %H-%M-%S", localtime(&_epoch));

	return result;
}

void ReplayManager::saveReplay(std::vector<Move> _moves, std::vector<std::vector<bool>> _map, int _finalTime, Difficulty _difficulty)
{
	json_t* root = json_object();
	json_t* map = json_array();
	std::string username = playerInput(_finalTime);

	json_object_set_new(root, "username", json_string(username.c_str()));
	json_object_set_new(root, "time", json_integer(_finalTime));

	json_t* difficulty = json_object();
	json_t* dimentions = json_array();
	
	json_object_set_new(difficulty, "textSize", json_real(_difficulty.textSize));
	json_object_set_new(difficulty, "tileSize", json_integer(_difficulty.tileSize));
	json_object_set_new(difficulty, "mineCount", json_integer(_difficulty.mineCount));

	json_array_append_new(dimentions, json_integer(_difficulty.dimentions.x));
	json_array_append_new(dimentions, json_integer(_difficulty.dimentions.y));
	json_object_set_new(difficulty, "dimentions", dimentions);
	
	json_object_set_new(difficulty, "scaling", json_real(_difficulty.scaling));

	json_object_set_new(root, "difficulty", difficulty);

	for (int y = 0; y < (int)_map.size(); y++)
	{
		json_t* jsonRow = json_array();

		for (int x = 0; x < (int)_map[y].size(); x++)
		{
			json_array_append_new(jsonRow, json_boolean(_map[y][x]));
		}
		json_array_append_new(map, jsonRow);
	}
	json_object_set_new(root, "map", map);

	json_t* moves = json_array();
	for (Move currentMove : _moves)
	{
		json_t* moveObject = json_object();
		json_t* jsonPosition = json_array();

		json_array_append_new(jsonPosition, json_integer(currentMove.position.x));
		json_array_append_new(jsonPosition, json_integer(currentMove.position.y));
		json_object_set_new(moveObject, "position", jsonPosition);
		json_object_set_new(moveObject, "time", json_integer(currentMove.ms));
		json_array_append_new(moves, moveObject);
	}
	json_object_set_new(root, "moves", moves);

	std::string filename = directory + diffToString(_difficulty) + " - "+ std::to_string(_finalTime) + " - " + epochToDate(time(0)) + ".json";

	json_dump_file(root, filename.c_str(), JSON_INDENT(4));
	json_decref(root);
}

void ReplayManager::getScores(Difficulty _difficulty)
{
	scores.clear();
	DIR* replaysDir = opendir(directory.c_str());
	if (!replaysDir) return ;

	dirent* entry;
	// hate how this looks but each call to readdir advances it one down the filelist.
	while ((entry = readdir(replaysDir)) != NULL)
	{
		std::string filename = entry->d_name;
		// check for json
		if (filename.size() > 5 && filename.substr(filename.size() - 5) == ".json")
		{
			std::string path = directory + filename;
			
			json_error_t error;
			json_t* root = json_load_file(path.c_str(), 0, &error);
			if (!root) return;

			Score newScore;
			newScore.username = json_string_value(json_object_get(root, "username"));
			newScore.time = (int)json_integer_value(json_object_get(root, "time"));

			json_t* diffJson = json_object_get(root, "difficulty");
			newScore.difficulty.textSize = (float)json_real_value(json_object_get(diffJson, "textSize"));
			newScore.difficulty.tileSize = (int)json_integer_value(json_object_get(diffJson, "tileSize"));
			newScore.difficulty.mineCount = (int)json_integer_value(json_object_get(diffJson, "mineCount"));
			newScore.difficulty.scaling = (float)json_real_value(json_object_get(diffJson, "scaling"));
			newScore.difficulty.dimentions = {
				(int)json_integer_value(json_array_get(json_object_get(diffJson, "dimentions"), 0)),
				(int)json_integer_value(json_array_get(json_object_get(diffJson, "dimentions"), 1))
			};

			if (newScore.difficulty.mineCount != _difficulty.mineCount || newScore.difficulty.dimentions != _difficulty.dimentions) continue;
			
			json_t* movesNode = json_object_get(root, "moves");
			json_t* mapNode = json_object_get(root, "map");
			
			Vector2i mapSize = {
				(int)json_array_size(json_array_get(mapNode, 0)),
				(int)json_array_size(mapNode)
			};
			newScore.mineMap.assign(mapSize.y, std::vector<bool>(mapSize.x, false));

			int movesCount = (int)json_array_size(movesNode);
			newScore.moves.assign(movesCount, {0});
			
			for (int y = 0; y < mapSize.y; y++)
			{
				for (int x = 0; x < mapSize.x; x++)
				{
					newScore.mineMap[y][x] = (bool)json_boolean_value(json_array_get(json_array_get(mapNode, y), x));
				}
			}
			
			for (int i = 0; i < movesCount; i++)
			{
				Move newMove;
				json_t* currentMoveNode = json_array_get(movesNode, i);
				
				newMove.ms = (int)json_integer_value(json_object_get(currentMoveNode, "time"));
				json_t* positionNode = json_object_get(currentMoveNode, "position");
				newMove.position = {
					(int)json_integer_value(json_array_get(positionNode, 0)),
					(int)json_integer_value(json_array_get(positionNode, 1))
				};

				newScore.moves[i] = newMove;
			}

			scores.push_back(newScore);
			json_decref(root);
		}
	}
	closedir(replaysDir);

	std::sort(scores.begin(), scores.end(), [](Score& _s1, Score& _s2)
	{
		return _s1.time < _s2.time;
	});
}
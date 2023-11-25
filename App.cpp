#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>
#include <conio.h>
#include <stdio.h>
#include <Windows.h>
#include "Player.h"
#include "Spike.h"
#include "Block.h"
#include "Console.h"
#include "Portal.h"
#include "rapidxml.hpp"

// remove stupid warnings
#pragma warning (disable : 6262)
#pragma warning (disable : 26812)

#define DEBUG_MODE 0

Player player;

bool quit, paused, debug, mainMenu, editor, levelSelect;
screensize_t screenSize;
int groundY, camera, finish;
std::vector<Spike> spikes, spikesEditor;
std::vector<Block> blocks, blocksEditor;
std::vector<Portal> portals, portalsEditor;

int editorX, editorY, editorSelectedID;

int levelSelectCursor, levelSelectLevels;
std::vector<std::string> levels;

const char* SelectedIDToStr(void)
{
	if (editorSelectedID == 1)
	{
		return "Spike";
	}
	else if (editorSelectedID == 2)
	{
		return "Block";
	}
	else if (editorSelectedID == 3)
	{
		return "Slab";
	}
	else if (editorSelectedID == 4)
	{
		return "Cube Portal";
	}
	else if (editorSelectedID == 5)
	{
		return "Ship Portal";
	}
	return "Error";
}

void LoadLevel(const char* path)
{
	blocks.clear();
	spikes.clear();
	portals.clear();

	std::ifstream levelFile(path);

	if (!levelFile.is_open())
	{
		std::cout << "Failed to open level file (" << path << ")" << std::endl;
		std::cin.get();
		return;
	}

	std::string levelData((std::istreambuf_iterator<char>(levelFile)),
		std::istreambuf_iterator<char>());

	rapidxml::xml_document<> doc;
	doc.parse<0>(const_cast<char*>(levelData.c_str()));

	rapidxml::xml_node<>* root = doc.first_node();
	ClearScreen();

	// iterate through all nodes in root
	for (rapidxml::xml_node<>* node = root->first_node(); node; node = node->next_sibling())
	{
		if (node->name() == std::string("spike"))
		{
			int x = atoi(node->first_attribute("x")->value()) + player.GetStartX();
			int y = groundY - atoi(node->first_attribute("y")->value());
			spikes.push_back(Spike(x, y));
			std::cout << "Spike (" << x << ", " << y << ")\n";
		}
		else if (node->name() == std::string("block"))
		{
			int x = atoi(node->first_attribute("x")->value()) + player.GetStartX();
			int y = groundY - atoi(node->first_attribute("y")->value());

			bool slab = false;
			if (node->first_attribute("slab") != NULL && node->first_attribute("slab")->value() == std::string("true"))
			{
				slab = true;
			}

			blocks.push_back(Block(x, y, slab));
			std::cout << "Block (" << x << ", " << y << ")\n";
		}
		else if (node->name() == std::string("portal"))
		{
			int x = atoi(node->first_attribute("x")->value()) + player.GetStartX();
			int y = groundY - atoi(node->first_attribute("y")->value());
			GameMode mode = StringToGameMode(std::string(node->first_attribute("mode")->value()));
			portals.push_back(Portal(x, y, mode));
			std::cout << "Portal (" << x << ", " << y << ")\n";
		}
		else
		{
			std::cout << "Unknown node: " << node->name() << std::endl;
		}
	}

	int maxX = 0;
	for (auto block : blocks)
	{
		if (block.GetX() > maxX)
		{
			maxX = block.GetX();
		}
	}
	for (auto spike : spikes)
	{
		if (spike.GetX() > maxX)
		{
			maxX = spike.GetX();
		}
	}
	for (auto portal : portals)
	{
		if (portal.GetX() > maxX)
		{
			maxX = portal.GetX();
		}
	}
	finish = maxX + 12;
}

void LoadLevelEditor(const char *path)
{
	LoadLevel(path);
	// copy blocks and spikes to editor
	spikesEditor = spikes;
	blocksEditor = blocks;
	portalsEditor = portals;

	spikes.clear();
	blocks.clear();
	portals.clear();
}

void Restart(void)
{
	camera = 0;
	player.Reset();
	paused = false;
}

void ResetEditorCursor(void)
{
	editorY = groundY - 1;
	editorX = player.GetStartX();
}

void Input(void)
{
	if (_kbhit())
	{
		if (!mainMenu && !editor && !levelSelect)
		{
			if (paused)
			{
				// pause menu keys
				switch (_getch())
				{
				case 27:
					paused = false;
					break;
				case 'm':
				case 'M':
					mainMenu = true;
					paused = false;
					player.SetLevelComplete(false);
					blocks.clear();
					spikes.clear();
					break;
				case 'r':
				case 'R':
					Restart();
					break;
				}
			}
			else
			{
				// gameplay keys
				switch (_getch())
				{
				case 27:
					paused = player.IsLevelComplete() ? false : true;
					break;
				case 'd':
				case 'D':
					debug = !debug;
					break;
				case ' ':
					player.Jump();
					if (player.IsLevelComplete())
					{
						mainMenu = true;
						player.SetLevelComplete(false);
						blocks.clear();
						spikes.clear();
					}
					break;
				case 'i':
				case 'I':
					if (debug)
						player.ToggleInvincibility();
					break;
				case 'r':
				case 'R':
					if (player.IsLevelComplete())
					{
						Restart();
					}
					break;
				}
			}
		}
		else
		{
			if (editor)
			{
				// editor keys
				switch (_getch())
				{
				case 27: // quit
					editor = false;
					mainMenu = true;
					break;
				// wasd movement
				case 'w':
				case 'W':
					editorY--;
					break;
				case 'a':
				case 'A':
					editorX--;
					break;
				case 's':
				case 'S':
					editorY++;
					break;
				case 'd':
				case 'D':
					editorX++;
					break;
				case 'i':
				case 'I': // prev obj
					editorSelectedID--;
					if (editorSelectedID < 1)
					{
						editorSelectedID = 1;
					}
					break;
				case 'o':
				case 'O': // next obj
					editorSelectedID++;
					if (editorSelectedID > 5)
					{
						editorSelectedID = 5;
					}
					break;
				case ' ': // place obj
					if (editorSelectedID == 1) // spike
					{
						spikesEditor.push_back(Spike(editorX, editorY));
					}
					else if (editorSelectedID == 2) // block
					{
						blocksEditor.push_back(Block(editorX, editorY, false));
					}
					else if (editorSelectedID == 3) // slab
					{
						blocksEditor.push_back(Block(editorX, editorY, true));
					}
					break;
				case 'p':
				case 'P': // erase obj
					for (size_t i = 0; i < spikesEditor.size(); i++)
					{
						Spike spike = spikesEditor[i];
						if (spike.GetX() == editorX && spike.GetY() == editorY)
						{
							spikesEditor.erase(spikesEditor.begin() + i);
						}
					}
					for (size_t i = 0; i < blocksEditor.size(); i++)
					{
						Block block = blocksEditor[i];
						if (block.GetX() == editorX && block.GetY() == editorY)
						{
							blocksEditor.erase(blocksEditor.begin() + i);
						}
					}
					break;
				case 'x':
				case 'X': // save to file
					CreateDirectoryA("levels", NULL);
					std::string name = "";
					ClearScreen();
					GotoXY(0, 0);
					printf("Enter level name: ");
					std::cin >> name;
					name += ".xml";
					printf("Creating level data...\n");

					std::string levelData = "";

					levelData += "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<level>\n";

					for (auto spike : spikesEditor)
					{
						std::ostringstream ss;
						ss << "\t<spike x=\"" << spike.GetGridX(player.GetStartX()) << "\" y=\"" << spike.GetGridY(groundY) << "\" />\n";
						levelData += ss.str();
					}

					for (auto block : blocksEditor)
					{
						std::string slabStr = "";
						if (block.IsSlab())
						{
							slabStr = "slab=\"true\" "; // extra space is important
						}
						std::ostringstream ss;
						ss << "\t<block x=\"" << block.GetGridX(player.GetStartX()) << "\" y=\"" << block.GetGridY(groundY) << "\" " << slabStr << "/>\n";
						levelData += ss.str();
					}

					levelData += "</level>\n";
					printf(levelData.c_str());

					printf("Writing to file...\n");

					std::ofstream os;
					os.open(std::string("levels\\") + name);
					os << levelData;
					os.close();
				}
			}
			else if (levelSelect)
			{
				// level select menu keys
				switch (_getch())
				{
				case 27:
					levelSelect = false;
					paused = false;
					debug = false;
					mainMenu = true;
					editor = false;
					break;
				case 'w':
				case 'W':
					levelSelectCursor--;
					if (levelSelectCursor < 0)
					{
						levelSelectCursor = levelSelectLevels - 1;
					}
					break;
				case 's':
				case 'S':
					levelSelectCursor++;
					if (levelSelectCursor > levelSelectLevels - 1)
					{
						levelSelectCursor = 0;
					}
					break;
				case ' ': {
					std::string levelPath = "levels\\" + levels[levelSelectCursor];
					LoadLevel(levelPath.c_str());
					mainMenu = false;
					levelSelect = false;
					levelSelectCursor = 0;
					editor = false;
					paused = false;
					debug = false;
					break;
				}
				case 'e': {
					std::string levelPathh = "levels\\" + levels[levelSelectCursor];
					LoadLevelEditor(levelPathh.c_str());
					mainMenu = false;
					levelSelect = false;
					levelSelectCursor = 0;
					editor = true;
					paused = false;
					debug = false;
					break;
				}
				}
			}
			else
			{
				// main menu keys
				switch (_getch())
				{
				case 27:
					quit = true;
					break;
				case ' ':
					mainMenu = false;
					paused = false;
					debug = false;
					editor = false;
					levelSelect = true;
					break;
				case 'e':
				case 'E':
					mainMenu = false;
					paused = false;
					debug = false;
					editor = true;
					camera = 0;
					break;
				}
			}
		}
	}
}

void Update(void)
{
	if (editor || mainMenu || levelSelect)
	{
		camera = 0;
	}

	if (paused || mainMenu || editor || levelSelect)
	{
		return;
	}

	if (!player.IsLevelComplete())
	{
		camera++;
	}
	player.Update(blocks, &camera, finish);
	for (auto spike : spikes)
	{
		// when player touch spike player die
		if (player.GetX() == spike.GetX() && player.GetY() == spike.GetY())
		{
			player.Die(&camera);
		}
	}
#if DEBUG_MODE
	player.SetInvincibility(true);
#endif
}

void Draw(void)
{
	SetConsoleColor(ConsoleColor::White, ConsoleColor::Black);
	ClearScreen();

	if (mainMenu) // main menu
	{
		const char *title = "Terminal Dash";
		const char *press1 = "Press Space to play";
		const char *press2 = "Press E to open level editor";
		const char *press3 = "Press Escape to quit the game";
		const char *gameBy = "Game by lispy2010 - lispy2010.github.io";
		const char *version = "Version 0.1";
		const int lenTitle = strlen(title);
		const int lenPress1 = strlen(press1);
		const int lenPress2 = strlen(press2);
		const int lenPress3 = strlen(press3);
		const int lenVersion = strlen(version);
		GotoXY(screenSize.width / 2 - lenTitle / 2, 3);
		printf(title);
		GotoXY(screenSize.width / 2 - lenPress1 / 2, 6);
		printf(press1);
		GotoXY(screenSize.width / 2 - lenPress2 / 2, 7);
		printf(press2);
		GotoXY(screenSize.width / 2 - lenPress3 / 2, 8);
		printf(press3);
		GotoXY(2, screenSize.height - 2);
		printf(gameBy);
		GotoXY(screenSize.width - 2 - lenVersion, screenSize.height - 2);
		printf(version);

		return;
	}

	if (editor) // editor
	{
		// draw ground
		for (int i = 0; i < screenSize.width; i++)
		{
			GotoXY(i, groundY);
			SetConsoleColor(ConsoleColor::Green, ConsoleColor::Black);
			printf("\"");
			SetConsoleColor(ConsoleColor::White, ConsoleColor::Black);
		}

		// draw blocks
		for (auto block : blocksEditor)
		{
			block.Draw(camera);
		}

		// draw spikes
		for (auto spike : spikesEditor)
		{
			spike.Draw(camera);
		}

		// draw cursor
		GotoXY(editorX, editorY);
		SetConsoleColor(ConsoleColor::Black, ConsoleColor::Blue);
		printf(" ");
		SetConsoleColor(ConsoleColor::White, ConsoleColor::Black);

		// draw instructions
		const char *wasdKeys = "Use WASD to move cursor";
		const char *escape = "Press Escape to quit";
		const char *selectID = "Press I and O to select object";
		const char *space = "Press Space to place object";
		const char *erase = "Press P to erase object";
		const char *save = "Press X to save level";
		const int lenWasdKeys = strlen(wasdKeys);
		const int lenEscape = strlen(escape);
		const int lenSelectID = strlen(selectID);
		const int lenSpace = strlen(space);
		const int lenErase = strlen(erase);
		const int lenSave = strlen(save);
		// 2 in the x pos calculation is offset from right side
		// 2 in the x pos calculation is offset from right side
		GotoXY(screenSize.width - 2 - lenWasdKeys, 1);
		printf(wasdKeys);
		GotoXY(screenSize.width - 2 - lenEscape, 2);
		printf(escape);
		GotoXY(screenSize.width - 2 - lenSelectID, 3);
		printf(selectID);
		GotoXY(screenSize.width - 2 - lenSpace, 4);
		printf(space);
		GotoXY(screenSize.width - 2 - lenErase, 5);
		printf(erase);
		GotoXY(screenSize.width - 2 - lenSave, 6);
		printf(save);

		// editor info
		GotoXY(1, screenSize.height - 2);
		printf("%s selected", SelectedIDToStr());

		return;
	}

	if (levelSelect) // level select screen
	{
		GotoXY(10, 5);
		printf("Level select");
		GotoXY(10, 6);
		printf("Press Space to play selected level");
		GotoXY(10, 7);
		printf("Press Escape to quit this menu");
		GotoXY(10, 8);
		printf("Press W and S to select levels");
		GotoXY(10, 9);
		printf("Press E to edit level");

		CreateDirectoryA("levels", NULL); // just in case (if it doesn't exist)
		std::vector<std::string> files;

		// get all xml files in levels folder
		// https://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c
		// answer by herohuyongtao (modified to actually work)
		WIN32_FIND_DATA fd;
		HANDLE hFind = ::FindFirstFile(L"levels\\*.xml", &fd);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					char filename[260] = {0};
					WCHAR *wfn = fd.cFileName;
					sprintf_s(filename, "%ws", wfn);

					files.push_back(filename);
				}
			} while (::FindNextFile(hFind, &fd));
			::FindClose(hFind);
		}
		else
		{
			SetConsoleColor(ConsoleColor::Black, ConsoleColor::White);
			printf("No levels found.");
			SetConsoleColor(ConsoleColor::White, ConsoleColor::Black);
		}

		levels = files;
		levelSelectLevels = files.size();

		for (size_t i = 0; i < files.size(); i++)
		{
			GotoXY(10, 12 + (short)i);
			if (levelSelectCursor == i)
			{
				SetConsoleColor(ConsoleColor::Black, ConsoleColor::White);
			}
			printf("%d. %s", i + 1, files[i].c_str());
			SetConsoleColor(ConsoleColor::White, ConsoleColor::Black);
		}

		return;
	}

	// gameplay drawing

	// draw ground
	for (int i = 0; i < screenSize.width; i++)
	{
		GotoXY(i, groundY);
		SetConsoleColor(ConsoleColor::Green, ConsoleColor::Black);
		printf("\"");
		SetConsoleColor(ConsoleColor::White, ConsoleColor::Black);
	}

	// draw spikes
	for (auto spike : spikes)
	{
		spike.Draw(camera);
	}

	// draw blocks
	for (auto block : blocks)
	{
		block.Draw(camera);
	}

	// draw second ground
	if (!player.IsGameMode(GameMode::Cube))
	{
		for (int i = 0; i < screenSize.width; i++)
		{
			GotoXY(i, 4);
			SetConsoleColor(ConsoleColor::Green, ConsoleColor::Black);
			printf("\"");
			SetConsoleColor(ConsoleColor::White, ConsoleColor::Black);
		}
	}

	SetConsoleColor(ConsoleColor::LightGreen, ConsoleColor::Black);
	// draw finish line
	for (int i = 0; i < groundY; i++)
	{
		GotoXY(finish - camera, i);
		printf("|");
	}

	// draw player
	if (!player.IsLevelComplete())
	{
		player.Draw();
	}
	SetConsoleColor(ConsoleColor::White, ConsoleColor::Black);

	// draw attempt count
	GotoXY(1, screenSize.height - 2);
	printf("Attempt %d", player.GetAttempt());

	if (paused)
	{
		const char *text1 = "Press M to go to menu";
		const char *text2 = "Press R to restart";
		const int len1 = strlen(text1);
		const int len2 = strlen(text2);
		GotoXY(screenSize.width / 2 - 3, screenSize.height / 2);
		printf("-PAUSED-");
		GotoXY(screenSize.width / 2 - len1 / 2, screenSize.height / 2 + 1);
		printf(text1);
		GotoXY(screenSize.width / 2 - len2 / 2, screenSize.height / 2 + 2);
		printf(text2);
	}

	if (player.IsLevelComplete())
	{
		const char *levelComplete = "       Level complete!       ";
		const char *press1 = "  Press Space to go to menu  ";
		const int lenLevelComplete = strlen(levelComplete);
		const int lenPress1 = strlen(press1);
		SetConsoleColor(ConsoleColor::Black, ConsoleColor::LightGreen);
		GotoXY(screenSize.width / 2 - lenLevelComplete / 2, screenSize.height / 2 - 1);
		printf(levelComplete);
		GotoXY(screenSize.width / 2 - lenPress1 / 2, screenSize.height / 2);
		printf(press1);
		SetConsoleColor(ConsoleColor::White, ConsoleColor::Black);
	}

	if (debug)
	{
		GotoXY(1, 1);
		printf("Debug Info");
		GotoXY(1, 2);
		printf("CWD: %s", _getcwd(NULL, 0));
		GotoXY(1, 3);
		printf("Player X: %d Y: %d", player.GetX(), player.GetY());
	}
}

int main(void)
{
	// init
	player = Player(12, 19);
	quit = paused = debug = editor = false;
	mainMenu = true;
	screenSize = GetScreenSize();
	groundY = 20;
	camera = 0;
	editorSelectedID = 1;
	levelSelectCursor = 0;
	blocks.clear();
	spikes.clear();
	ResetEditorCursor();
	HideCursor();
	DisableConsoleSelection();

	while (!quit)
	{
		Input();
		Update();
		Draw();
		Sleep(100);
	}
	return 0;
}

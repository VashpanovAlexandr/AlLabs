#include <iostream>
#include <random>
#include <fstream>
#include <string>
#include <windows.h>
#include "GameState.h"

int main() {
    SetConsoleOutputCP(65001);
    GameState game;
    std::string saveFileName = "save.dat";

    if (std::ifstream checkFile(saveFileName); checkFile.good())
    {
        std::cout << "Найдено сохранение игры (" << saveFileName << "). Загрузить? (y/n): ";
        char loadChoice;
        std::cin >> loadChoice;

        if (loadChoice == 'y' || loadChoice == 'Y')
        {
            if (game.loadFromFile(saveFileName))
            {
                std::cout << "Игра загружена. Продолжаем с " << game.getRound() << " года.\n";
            }
            else
            {
                std::cout << "Ошибка загрузки сохранения\n";
                game = GameState();
            }
        }
        else
        {
            std::cout << "Начинаем новую игру.\n";
            game = GameState();
        }
    }
    else
    {
        std::cout << "Начинаем новую игру.\n";
    }

    bool gameRunning = true;
    while (gameRunning)
    {
        game.getReport();

        std::cout << "Сохранить игру и выйти? (y/n): ";
        char exitChoice;
        std::cin >> exitChoice;

        if (exitChoice == 'y' || exitChoice == 'Y')
        {
            game.saveToFile(saveFileName);
            std::cout << "Игра сохранена\n";
            return 0;
        }
        else if (exitChoice == 'n' || exitChoice == 'N')
        {
            GameState::ResourcesToUse playerDecision = game.getPlayerInput();
            bool success = game.processRound(playerDecision.landToBuy, playerDecision.landToSell, playerDecision.grainToPlant, playerDecision.grainToEat);

            if (!success)
            {
                if (game.getRound() > 10)
                {
                    game.getFinalEvaluation();
                }
                gameRunning = false;
            }
        }
    }

    return 0;
}
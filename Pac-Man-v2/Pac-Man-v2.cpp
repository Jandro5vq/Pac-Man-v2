#include "Enemy.h"
#include "TimeManager.h"
#include <vector>

/// <summary>
/// Sets the needed variables
/// </summary>
void Setup();
/// <summary>
/// Handles the inputs
/// </summary>
void Input();
/// <summary>
/// Handles the logic of the game
/// </summary>
void Logic();
/// <summary>
/// Draws the screen
/// </summary>
void Draw();

enum USER_INPUTS { NONE, UP, DOWN, RIGHT, LEFT, QUIT };
Map pacman_map = Map();
std::vector<Enemy> enemigos = std::vector<Enemy>();
char player_char = 'O';
int player_x = 1;
int player_y = 1;
int player_points = 0;
USER_INPUTS input = USER_INPUTS::NONE;
bool run = true;
bool win = false;

int main()
{
    Setup();
    while (run)
    {
        Input();
        Logic();
        Draw();
    }
}

void Setup()
{
    std::cout.sync_with_stdio(false);

    srand(time(NULL));

    int enemyN = 0;
    ConsoleUtils::Console_SetColor(ConsoleUtils::CONSOLE_COLOR::YELLOW);
    std::cout << "Pac-Man";
    ConsoleUtils::Console_SetColor(ConsoleUtils::CONSOLE_COLOR::WHITE);
    std::cout << " por ";
    ConsoleUtils::Console_SetColor(ConsoleUtils::CONSOLE_COLOR::MAGENTA);
    std::cout << "Alejandro V." << std::endl << std::endl;
    ConsoleUtils::Console_SetColor(ConsoleUtils::CONSOLE_COLOR::CYAN);
    std::cout << "Cuantos enemigos quieres?";
    std::cin >> enemyN;
    for (size_t i = 0; i < enemyN; i++)
    {
        enemigos.push_back(Enemy(pacman_map.spawn_enemy));
    }



    player_x = pacman_map.spawn_player.X;
    player_y = pacman_map.spawn_player.Y;
}

void Input()
{
    input = USER_INPUTS::NONE;
    if (ConsoleUtils::KeyPressed(VK_UP) || ConsoleUtils::KeyPressed('W'))
    {
        input = USER_INPUTS::UP;
    }
    if (ConsoleUtils::KeyPressed(VK_DOWN) || ConsoleUtils::KeyPressed('S'))
    {
        input = USER_INPUTS::DOWN;
    }
    if (ConsoleUtils::KeyPressed(VK_RIGHT) || ConsoleUtils::KeyPressed('D'))
    {
        input = USER_INPUTS::RIGHT;
    }
    if (ConsoleUtils::KeyPressed(VK_LEFT) || ConsoleUtils::KeyPressed('A'))
    {
        input = USER_INPUTS::LEFT;
    }
    if (ConsoleUtils::KeyPressed(VK_ESCAPE) || ConsoleUtils::KeyPressed('Q'))
    {
        input = USER_INPUTS::QUIT;
    }
}

void Logic()
{
    if (win)
    {
        switch (input)
        {
        case QUIT:
            run = false;
            break;
        }
    }
    else
    {
        COORD playerPos;
        playerPos.X = player_x;
        playerPos.Y = player_y;
        bool playerDie = false;

        for (size_t i = 0; i < enemigos.size(); i++)
        {
            if (enemigos[i].Logic(&pacman_map, playerPos)) {
                playerDie = true;
            }
        }
        if (playerDie)
        {
            player_x = pacman_map.spawn_player.X;
            player_y = pacman_map.spawn_player.Y;
        }
        int player_y_new = player_y;
        int player_x_new = player_x;
        switch (input)
        {
        case UP:
            player_y_new--;
            break;
        case DOWN:
            player_y_new++;
            break;
        case RIGHT:
            player_x_new++;
            break;
        case LEFT:
            player_x_new--;
            break;
        case QUIT:
            run = false;
            break;
        }
        if (player_x_new < 0)
        {
            player_x_new = pacman_map.Width - 1;
        }
        player_x_new %= pacman_map.Width;
        if (player_y_new < 0)
        {
            player_y_new = pacman_map.Height - 1;
        }
        player_y_new %= pacman_map.Height;

        switch (pacman_map.GetTile(player_x_new, player_y_new))
        {
        case Map::MAP_TILES::MAP_WALL:
            player_y_new = player_y;
            player_x_new = player_x;
            break;
        case Map::MAP_TILES::MAP_POINT:
            pacman_map.points--;
            player_points++;
            pacman_map.SetTile(player_x_new, player_y_new, Map::MAP_TILES::MAP_EMPTY);
            break;
        }

        player_y = player_y_new;
        player_x = player_x_new;
        if (pacman_map.points <= 0)
        {
            win = true;
        }
    }
}

void Draw()
{
    if (!win)
    {
        ConsoleUtils::Console_SetPos(0, 0);
        pacman_map.Draw();
        for (size_t i = 0; i < enemigos.size(); i++)
        {
            enemigos[i].Draw();
        }
        ConsoleUtils::Console_SetPos(player_x, player_y);
        ConsoleUtils::Console_SetColor(ConsoleUtils::CONSOLE_COLOR::YELLOW);
        std::cout << player_char;
        ConsoleUtils::Console_ClearCharacter({ 0,(short)pacman_map.Height });
        ConsoleUtils::Console_SetColor(ConsoleUtils::CONSOLE_COLOR::CYAN);
        std::cout << "Puntuacion actual: " << player_points << " Puntuacion pendiente: " << pacman_map.points << std::endl;

        std::cout << "Fotogramas: " << TimeManager::getInstance().frameCount << std::endl;
        std::cout << "Time: " << TimeManager::getInstance().time << std::endl;
        std::cout << "DeltaTime: " << TimeManager::getInstance().deltaTime << std::endl;
    }
    else
    {
        system("CLS");
        ConsoleUtils::Console_SetColor(ConsoleUtils::CONSOLE_COLOR::GREEN);
        std::cout << "\033[96m" <<
            R"(
			 ___  ___  ______    ____  ____      __   __  ___   __    _____  ___   
			|"  \/"  |/    " \  ("  _||_ " |    |"  |/  \|  "| |" \  (\"   \|"  \  
			 \   \  /// ____  \ |   (  ) : |    |'  /    \:  | ||  | |.\\   \    | 
			  \\  \//  /    ) :)(:  |  | . )    |: /'        | |:  | |: \.   \\  | 
			  /   /(: (____/ //  \\ \__/ //      \//  /\'    | |.  | |.  \    \. | 
			 /   /  \        /   /\\ __ //\      /   /  \\   | /\  |\|    \    \ | 
			|___/    \"_____/   (__________)    |___/    \___|(__\_|_)\___|\____\) 
			)" << "\033[92m" <<
            R"(
									By Alejandro Vazquez
			)" << "\033[0m" << "- Press Q to exit.";
    }

    TimeManager::getInstance().nextFrame();
}
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>

#define height 22
#define width 12
#define tetrisWidth 4

enum Colors 
{
	Black = 8,
	Blue,
	Green,
	Cyan,
	Red,
	Purple,
	Yellow,
	White
};

bool gameOver;
char tempTetris[4][4] = {};
char tetris[7][4][4] = { 
'0', '0', '0', '0',
'#', '#', '#', '#',
'0', '0', '0', '0',
'0', '0', '0', '0',

'0', '0', '0', '0',
'0', '#', '#', '#',
'0', '0', '#', '0',
'0', '0', '0', '0',

'0', '0', '0', '0',
'0', '#', '#', '0',
'0', '#', '#', '0',
'0', '0', '0', '0',

'0', '0', '0', '0',
'0', '#', '#', '0',
'0', '0', '#', '#',
'0', '0', '0', '0',

'0', '0', '0', '0',
'0', '0', '#', '#',
'0', '#', '#', '0',
'0', '0', '0', '0',

'0', '0', '0', '0',
'0', '#', '#', '#',
'0', '#', '0', '0',
'0', '0', '0', '0',

'0', '0', '0', '0',
'0', '#', '#', '#',
'0', '0', '0', '#',
'0', '0', '0', '0'
};

char tetrisMap[height][width] = {};
char pressedKey;
int move, level, random, nextRandom, speed, score, difficultyLevel;
bool tetrisOnProcess;

//Функция для возврада на указанные координаты в консоли
void setCursorPosition(int x, int y)
{
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout.flush();
	COORD coord = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(hOut, coord);
}

//Функция для проверки коллизий при движении в лево
bool leftCollision(){
	for (int i = 0; i < tetrisWidth; i++)
	{
		for (int j = 0; j < tetrisWidth; j++)
		{
			if (tetris[random][i][j] == tetrisMap[level + i][move + tetrisWidth - 1 + j])
			{
				return true;
			}
		}
	}
	return false;
}

//Функция для проверки коллизий при движении в право
bool rightCollision(){
	for (int i = 0; i < tetrisWidth; i++)
	{
		for (int j = tetrisWidth - 1; j >= 0; j--)
		{
			if (tetris[random][i][j] == tetrisMap[level + i][move + tetrisWidth * 2 - (3 - j)])
			{
				return true;
			}
		}
	}
	return false;
}

//Функция для проверки коллизий при движении вниз
bool bottomCollision(){
	for (int i = 0; i < tetrisWidth; i++)
	{
		for (int j = 3; j >= 0; j--)
		{
			if (tetris[random][j][i] == tetrisMap[level + j][move + tetrisWidth + i] || tetris[random][j][i] == tetrisMap[level + j - 1 ][move + tetrisWidth + i])
			{
				return true;
			}
		}
	}
	return false;
}

//Функция для проверки заполненного ряда
bool ifRowIsFilled(int row){
	for (int i = 0; i < width; i++)
	{
		if (tetrisMap[row][i] != '#')
		{
			return false;
		}
	}
	return true;
}

//Функция для проверки пространства для поворота
bool ifRotationIsPossible(){
	for (int i = 0; i < tetrisWidth; i++)
	{
		for (int j = 0; j < tetrisWidth; j++)
		{
			if (pressedKey == 'q' && tetrisMap[i + level][j + move + 4] == '#' && tetris[random][j][3 - i] == '#')
			{
				return false;
			}
			else if (pressedKey == 'e' && tetrisMap[i + level][j + move + 4] == '#' && tetris[random][3 - j][i] == '#')
			{
				return false;
			}
		}
	}
	return true;
}

void startSetup(){
	setlocale(LC_ALL, "Russian");
	srand(time(0));
	random = nextRandom;
	nextRandom = rand() % 7;
	score = 0;
	level = 0;
	move = 0;
	difficultyLevel = 0;
	speed = 500 - difficultyLevel * 10;
	tetrisOnProcess = false;
	gameOver = true;

	for (int i = 1; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (i == height - 1 || j == 0 || j == width - 1)
			{
				tetrisMap[i][j] = '#';
			}
		}
	}
}

void setup(){
	level = 0;
	move = 0;
	tetrisOnProcess = false;	
	random = nextRandom;
	nextRandom = rand() % 7;
	speed = 500 - difficultyLevel * 10;
}

void print(){
	for (int i = 0; i < height; i++)
	{
		setCursorPosition(40, 3 + i);
		for (int j = 0; j < width; j++)
		{
			if (tetrisMap[i][j] == '#')
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Cyan);
				if (i == height - 1 && j == 0)
				{				
					setlocale(LC_CTYPE, "C");
					std::cout << char(200);
					setlocale(LC_CTYPE, "Rus");
				}
				else if (i == height - 1 && j == width - 1)
				{
					setlocale(LC_CTYPE, "C");
					std::cout << char(188);
					setlocale(LC_CTYPE, "Rus");
				}
				else if (j == 0 || j == width - 1)
				{
					setlocale(LC_CTYPE, "C");
					std::cout << char(186);
					setlocale(LC_CTYPE, "Rus");
					
				}
				else if (i == height - 1)
				{
					setlocale(LC_CTYPE, "C");
					std::cout << char(205);
					setlocale(LC_CTYPE, "Rus");
				}
				else
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Green);
					std::cout << '#';
				}
					
			}
			else if (j > tetrisWidth - 1 + move && j < (tetrisWidth) * 2 + move && i >= level && i < level + tetrisWidth && tetris[random][i - level][j - 4 - move] != '0')
			{			
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Purple);			
				setlocale(LC_CTYPE, "C");
				std::cout << tetris[random][i - level][j - 4 - move];
				setlocale(LC_CTYPE, "Rus");
			}
			else
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), White);
				setlocale(LC_CTYPE, "C");
				if (i)
				{
					std::cout << char(249);
				}
				else
				{
					std::cout << ' ';
				}
				setlocale(LC_CTYPE, "Rus");
			}
		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Yellow);
		if (i == 3)
		{
			std::cout << "   Управление: ";
		}
		else if (i == 5)
		{
			std::cout << "   Налево / Направо: a / d"; 
		}
		else if (i == 6)
		{
			std::cout << "   Ускорить падение: s";
		}
		else if (i == 7)
		{
			std::cout << "   Повернуть налево / направо: q / e";
		}
		else if (i == 8)
		{
			std::cout << "   Выход из игры: x";
		}
		else if (i == 11)
		{
			std::cout << "   Очки: " << score;
		}
		else if (i == 13)
		{
			std::cout << "   Уровень сложности: " << difficultyLevel;
		}
		else if (i > 13 && i < 18)
		{
			if (i == 15)
			{			
				std::cout << "   След. блок: ";
			}
			else
			{
				std::cout << "   \t\t   ";
			}
			
			for (int j = 0; j < 4; j++)
			{
				if (tetris[nextRandom][i - 14][j] != '0')
				{
					std::cout << tetris[nextRandom][i - 14][j];
				}
				else
				{
					std::cout << ' ';
				}
			}
		}
		std::cout << "\n";
	}

	//std::cout << "Очки: " << score << " ,высота = " << level << " , скорость уровня = " << speed << "\n";	

	//for (int f = 0; f < 16; f++)
	//{
	//	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), f);
	//	std::cout << "Colors ##### " << f << std::endl;
	//	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), White);
	//}

	//std::cout.flush();

	//Debug
	//for (int i = 0; i < height; i++)
	//{
	//	for (int j = 0; j < width; j++)
	//	{
	//		std::cout << tetrisMap[i][j];
	//	}
	//	std::cout << std::endl;
	//}
}

void input(){
	if (_kbhit())
	{
		pressedKey = _getch();
		switch (pressedKey)
		{
		case 'a':		
			if (!leftCollision())
			{
				move--;
			}		
		break;
		case 'd':
			if (!rightCollision())
			{
				move++;
			}
		break;
		case 'q':
			if (ifRotationIsPossible())
			{
				for (int i = 0; i < 4; i++)
				{
					for (int j = 0; j < 4; j++)
					{
						tempTetris[i][j] = tetris[random][i][j];
					}
				}

				for (int i = 0; i < 4; i++)
				{
					for (int j = 0; j < 4; j++)
					{
						tetris[random][i][j] = tempTetris[j][3 - i];
					}
				}
			}
			break;
		case 'e':
			if (ifRotationIsPossible())
			{
				for (int i = 0; i < 4; i++)
				{
					for (int j = 0; j < 4; j++)
					{
						tempTetris[i][j] = tetris[random][i][j];
					}
				}

				for (int i = 0; i < 4; i++)
				{
					for (int j = 0; j < 4; j++)
					{
						tetris[random][i][j] = tempTetris[3 - j][i];
					}
				}
			}			
			break;
		case 's':
			speed = 5;
			break;
		case 'x':
			gameOver = true;
			return;
		}
	}
}

void logic(){
	level++;
	if (!tetrisOnProcess)
	{
		level = 1;
		score += 5;
		tetrisOnProcess = true;
		if (bottomCollision())
		{
			gameOver = true;
			return;
		}
	}
	else if (bottomCollision())
	{
		for (int j = 3; j >= 0; j--)
		{
			for (int i = 0; i < tetrisWidth; i++)
			{
				if (tetris[random][j][i] != '0')
				{
					tetrisMap[level + j - 1][move + tetrisWidth + i] = tetris[random][j][i];
				}
			}
		}
			
		tetrisOnProcess = false;
		setup();
	}

	for (int i = 0; i < height - 1; i++)
	{
		if (ifRowIsFilled(i))
		{
			for (int k = i; k > 0; k--)
			{
				for (int j = 1; j < width - 1; j++)
				{
					if (k == 0)
					{
						tetrisMap[k][j] = '0';
					}
					else
					{
						tetrisMap[k][j] = tetrisMap[k - 1][j];
					}
				}
			}
			score += 50;
			difficultyLevel++;
			level++;
		}
	}
}

void main() {
	startSetup();

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Yellow);
	for (int i = 5; i < 12; i++)
	{
		setCursorPosition(30, 5 + i);
		switch (i)
		{
		case 5:
			std::cout << "___________________________________________.___  _________";
			break;
		case 6:
			std::cout << "\\__    ___/\\_   _____/\\__    ___/\\______   \\   |/   _____/";
			break;
		case 7:
			std::cout << "  |    |    |    __)_   |    |    |       _/   |\\_____  \\ ";
			break;
		case 8:
			std::cout << "  |    |    |        \\  |    |    |    |   \\   |/        \\";
			break;
		case 9:
			std::cout << "  |____|   /_______  /  |____|    |____|_  /___/_______  /";
			break;
		case 10:
			std::cout << "                   \\/                    \\/            \\/ ";
			break;
		default:
			setCursorPosition(30, 18);
			std::cout << "\t\t    Press any key to start playing...";
			break;
		}
	}
	if (_getch())
	{
		gameOver = false;
		system("cls");
	}

	while (!gameOver)
	{
		print();
		input();
		input();
		input();
		input();
		logic();
		Sleep(speed);
	}

	system("cls");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Red);

	for (int i = 5; i < 12; i++)
	{
		setCursorPosition(15, 5 + i);
		switch (i)
		{
		case 5:
			std::cout << "  ________    _____      _____  ___________    ____________   _________________________ ";
			break;
		case 6:
			std::cout << " /  _____/   /  _  \\    /     \\ \\_   _____/    \\_____  \\   \\ /   /\\_   _____/\\______   \\";
			break;
		case 7:
			std::cout << "/   \\  ___  /  /_\\  \\  /  \\ /  \\ |    __)_      /   |   \\   Y   /  |    __)_  |       _/";
			break;
		case 8:
			std::cout << "\\    \\_\\  \\/    |    \\/    Y    \\|        \\    /    |    \\     /   |        \\ |    |   \\";
			break;
		case 9:
			std::cout << " \\______  /\\____|__  /\\____|__  /_______  /    \\_______  /\\___/   /_______  / |____|_  /";
			break;
		case 10:
			std::cout << "        \\/         \\/         \\/        \\/             \\/                 \\/         \\/ ";
			break;
		default:
			setCursorPosition(50, 7 + i);
			std::cout << "Your score = " << score << std::endl;
			setCursorPosition(35, 20);
			std::cout << "\t\tPress ESC to exit...";
			break;
		}
	}
	while (_getch() != 27)
	{

	}
	setCursorPosition(43, 22);
}

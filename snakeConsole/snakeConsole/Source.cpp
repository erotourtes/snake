#include<iostream>
#include<windows.h>
#include<conio.h> 
#include<ctime>
#include<string>
using namespace std;

void writeLevelInfo(const int& Width, const int& Height, int& score);
void setcur(int x, int y);
void generatePosition(const int& width, const int& height);


enum Direction { Stop = 0, Left, Right, Up, Down };
Direction Dir;
int positionX;
int positionY;
int fruitX;
int fruitY;
bool pause = false;
string Check, CheckSetTeleport, mode, checkRetry = "y";
int speed;
int difficult = 2;
int sIze = 0;
int Speed[9]{ 200, 175, 150, 125, 100, 75, 50, 25, 0 };
int* tailX = new int [1];
int* tailY = new int [1];


int Time = 0;
int Time2 = 0;
void Menu(int& W, int& H)
{
	SetConsoleTitle(L"Snake(Console)");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_BLUE |
		(FOREGROUND_RED | FOREGROUND_GREEN) | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	if (Time == 0)
	{
		setcur(35, 9);
		cout << "Menu";
		setcur(10, 10);
		cout << "Set by default (y / n) ->\t\t\t";
		cin >> Check;
	}
	if (Check == "n")
	{
		//check size
		if (W < 7)
		{
			setcur(10, 11);
			cout << "Input width (recommend - 20 (min -5)) ->\t";
			cin >> W;
			W = W + 2;
		}
		if (H < 5)
		{
			setcur(10, 12);
			cout << "Input height (recommend - 20 (min -5)) ->\t";
			cin >> H;

		}
		if ((W < 7) || (H < 5))
		{
			++Time;
			Menu(W, H);

		}
		if (Time2 == 0)
		{
			//check die or teleport
			setcur(10, 13);
			cout << "Set \"Teleport\" mode (y / n) -> \t\t";
			cin >> CheckSetTeleport;

			//check speed
			setcur(10, 14);
			cout << "Set speed level (1 - 9) -> \t\t\t";
			cin >> speed;
			speed--;

			//check mode
			setcur(10, 15);
			cout << "Set mode (\"S\"urvival / \"I\"nfinity) -> \t";
			cin >> mode;

			//check difficult
			if (mode == "I")
			{
				setcur(10, 16);
				cout << "Set difficult (recommend (1 - 5)) -> \t\t";
				cin >> difficult;
			}
			++Time2;
		}
	}
	else if (Check == "y")
		{
			W = 22;
			H = 20;
			speed = 3;
			CheckSetTeleport = "n";
			mode = "S";
		}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
	system("cls");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_GREEN | BACKGROUND_BLUE |
		(FOREGROUND_RED | FOREGROUND_GREEN) | FOREGROUND_INTENSITY);
	
}

void setUp(int &score, bool &gameover, const int& W, const int& H)
{
	 score = 0;
	 gameover = false;
	 Dir = Stop;
	 positionX = W / 2;
	 positionY = H / 2;
	 fruitX = rand() % (W - 3) + 1;
	 fruitY = rand() % (H - 1) + 1;
		

}

void generateMap( const int&  Width, const int& Height, int& score, int* BlocksY, int* BlocksX)
{
	setcur(0, 0);
	for (int i = 0; i < Width; i++)
		cout << "#";
	cout << endl;
	
	//middle
	for (int i = 0; i < Height; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			if ((j == 0) || (j == Width - 1))
				cout << "#";
			else if ((j == positionX) && (i == positionY))
			{
				cout << '0';
			}
			else if ((j == fruitX) && (i == fruitY))
			{
				cout << 'F';
			}
			else
			{

				bool print = true;

				if (difficult > 1)
				{
					for (int k = 0; k < sIze; k++)
					{
						if ((i == BlocksY[k]) && (j == BlocksX[k]))
						{
							if ((BlocksY[k] == tailY[k]) && (BlocksX[k] == tailX[k]))
							{
								generatePosition(Width, Height);
							}
							else
							{
								cout << "#";
								print = false;
							}
						}
					}
				}

				for (int k = 0; k < score / 10; k++)
				{
					if ((tailX[k] == j) && (tailY[k] == i))
					{
						print = false;
						cout << 'o';
					}
				}
				if (print)
					cout << ' ';
			}
		}
		cout << endl;
	}
	for (int i = 0; i < Width; i++)
		cout << "#";
	writeLevelInfo(Width, Height, score);
}

void generateBlocks(int* BlocksX, int* BlocksY, int width, int height)
{
	for (int k = 0; k < sIze; k++)
	{
		BlocksX[k] = rand() % (width-3) + 1;
		BlocksY[k] = rand() % (height -1) + 1;
	}
}

void generatePosition(const int& width, const int& height)
{
	fruitX = rand() % (width - 3) + 1;
	fruitY = rand() % (height -1) + 1;
}

void setcur(int x, int y)//установка курсора на позицию  x y
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
};

void checkDie(bool& gameover, const int& width, const int&height)
{
	if ((positionX <= 0 ) || (positionX >= width - 1) || (positionY <= 0) || (positionY >= height))
	{
		gameover = true;
	}
}
void checkTeleport(const int& width, const int&height)
{
	if (positionX <= 0) 
	{
		positionX = width-1;
		Dir = Left;
	}
	else if (positionX >= width-1)
	{
		positionX = 0;
		Dir = Right;
	}
	if (positionY <= -1)
	{
		positionY = height;
		Dir = Up; 
	}
	else if (positionY >= height)
	{
		positionY = -1;
		Dir = Down;
	}
}

void movement(bool& gameover)
{
	if (_kbhit())
	{
		switch (_getch())
		{
		case 'a':
			Dir = Left;
			break;
		case 'd':
			Dir = Right;
			break;
		case 'w':
			Dir = Up;
			break;
		case 's':
			Dir = Down;
			break;
		case 'x':
			gameover = true;
			break;
		case 'z':
			pause = true;
			break;
		}
	}
}
void move/*(and logic of game)*/(const int& width, const int& height, int& score, bool& gameover, int* BlocksX, int* BlocksY)
{
	//tail
	
	int prevX = tailX[0];
	int prevY = tailY[0];
	int prevTempX, prevTempY;
	tailX[0] = positionX;
	tailY[0] = positionY;
	

	for (int i = 1; i < (score / 10); i++)
	{
		prevTempX = tailX[i];
		prevTempY = tailY[i];
		tailX[i] = prevX;
		tailY[i] = prevY;

		prevX = prevTempX;
		prevY = prevTempY;
	}

	//move
	switch (Dir)
	{
	case Left:
		positionX--;
		break;
	case Right:
		positionX++;
		break;
	case Up:
		positionY--;
		break;
	case Down:
		positionY++;
		break;
	}
	//check if u've ate # on pitch
	for (int k = 0; k < sIze; k++)
	{
		if ((BlocksX[k] == positionX) && (BlocksY[k] == positionY))
		{
			gameover = true;
		}
	}

	//check if u've ate tail
	for (int i = 0; i < (score / 10); i++)
	{
		if ((tailX[i] == positionX) && (tailY[i] == positionY))
		{
			gameover = true;
		}
	}

	//check if fruit position on tail position (doesnt work)
	for (int k = 0; k < score / 10; k++)
	{
		if ((BlocksY[k] == tailY[k]) && (BlocksX[k] == tailX[k]))
		{
			generatePosition(width, height);
		}
	}

	//check if u've ate fruit 
	if ((positionX == fruitX) && (positionY == fruitY))
	{
		score += 10;
		generatePosition(width, height);

		//add new element to array
		int* tailX1 = new int[(score + 10) / 10];
		int* tailY1 = new int[(score + 10) / 10];
		for (int i = 0; i < (score / 10); i++)
		{
			tailX1[i] = tailX[i];
			tailY1[i] = tailY[i];
		}
		//tailX1[score / 10] = 0;
		//tailY1[score / 10] = 0;
		delete[] tailX;
		delete[] tailY;
		tailX = tailX1;
		tailY = tailY1;
	}


}

void writeWinOrLose/*(and some information)*/(const int& width, const int& height, int& score)
{	
	// write "Win" or "lose"
	setcur(width / 2 - 4, height / 2);
	if (score <= ceil((width + height) * 15))
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (FOREGROUND_RED | FOREGROUND_INTENSITY));
		cout << "Gameover";
		setcur(width / 2 - 4, height / 2 + 1);
		cout << "retry (y / n) ->\t";
		cin >> checkRetry;
		setcur(0, 50);
	}
	else
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (FOREGROUND_GREEN | FOREGROUND_INTENSITY));
		cout << "Victory";
		setcur(width / 2 - 4, height / 2 + 1);
		cout << "retry (y / n) ->\t";
		cin >> checkRetry;
		setcur(0, 50);
	}

}
void writeLevelInfo(const int& Width, const int& Height, int& score)
{
		setcur(Width + 5, 1);
		cout << "Score: " << score << endl;
		setcur(Width + 5, 2);
		cout << "Score to win: " << ceil((Width + Height) * 15) << endl;
		setcur(Width + 5, 3);
		cout << "mode: " << mode << endl;
		setcur(Width + 5, 4);
		cout << "Speed level: " << speed << endl;
		setcur(Width + 5, 5);
		cout << "difficult: " << difficult << endl;
		setcur(Width + 5, 6);
		cout << "Teleport or Die: " << CheckSetTeleport << endl;
		setcur(Width + 5, 7);
		cout << "Width: " << (Width - 2) << endl;
		setcur(Width + 5, 8);
		cout << "Height: " << Height << endl;
}

int main()
{
	while (checkRetry == "y")
	{
		system("cls");
		int score = 0;
		bool gameover;
		int W; //Width
		int H; // Height
		Menu(W, H);
		const int width = W;
		const int height = H;

		// array of blocks
		int *BlocksX = nullptr;
		int *BlocksY = nullptr;
		
		if (difficult > 1)
		{
			sIze = difficult * (width - 2 + height) / 10;
			BlocksX = (int *)calloc(sizeof(int), difficult * (width - 2 + height) / 10);
			BlocksY = (int *)calloc(sizeof(int), difficult * (width - 2 + height) / 10);

			generateBlocks(BlocksX, BlocksY, width, height);

		}
			

		setUp(score, gameover, width, height);

		while ((!gameover) && !(score >= ceil((width + height) * 15))) // start
		{

			//Move
			movement(gameover);
			move(width, height, score, gameover, BlocksX, BlocksY);

			srand(time(nullptr));
			
			generateMap(width, height, score, BlocksY, BlocksX);

			// Check teleport or Die
			if (CheckSetTeleport == "y")
				checkTeleport(width, height);
			else
				checkDie(gameover, width, height);

			//Speed
			Sleep(Speed[speed]);
			
		}

		
		writeWinOrLose(width, height, score);
		//system("pause");
		
		if (mode == "S")
		{
			difficult++;
		}
	}

	delete[] tailX;
	delete[] tailY;
	
	return 0;
}
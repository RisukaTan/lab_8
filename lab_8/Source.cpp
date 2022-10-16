#include<stdio.h>//65010536
#include<windows.h>
#include<conio.h>
#include<time.h>
#include<string.h>
#define screen_x 80
#define screen_y 25
#define scount 80
HANDLE rHnd;
HANDLE wHnd;
DWORD fdwMode;
COORD bufferSize = { screen_x,screen_y };
COORD characterPos = { 0,0 };
CHAR_INFO consoleBuffer[screen_x * screen_y];
SMALL_RECT windowSize = { 0,0,screen_x - 1,screen_y - 1 };
COORD star[scount];
int setConsole(int x, int y)
{
	wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleWindowInfo(wHnd, TRUE, &windowSize);
	SetConsoleScreenBufferSize(wHnd, bufferSize);
	return 0;
}
int setMode()
{
	rHnd = GetStdHandle(STD_INPUT_HANDLE);
	fdwMode = ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT |
		ENABLE_MOUSE_INPUT;
	SetConsoleMode(rHnd, fdwMode);
	return 0;
}
void clear_buffer()
{
	for (int y = 0; y < screen_y; ++y) {
		for (int x = 0; x < screen_x; ++x) {
			consoleBuffer[x + screen_x * y].Char.AsciiChar = ' ';
			consoleBuffer[x + screen_x * y].Attributes = 7;
		}
	}
}
void fill_buffer_to_console()
{
	WriteConsoleOutputA(wHnd, consoleBuffer, bufferSize, characterPos,
		&windowSize);
}
void init_star()
{
	int i;
	for (i = 0; i < scount; i++) {
		star[i].X = { short(rand() % screen_x) };
		star[i].Y = { short(rand() % screen_y) };
		star[i] = { short(star[i].X),short(star[i].Y) };
	}

}
void star_fall()
{
	int i;
	for (i = 0; i < scount; i++) {
		if (star[i].Y >= screen_y - 1) {
			star[i] = { short(rand() % screen_x),1 };
		}
		else {
			star[i] = { short(star[i].X),short(star[i].Y + 1) };
		}
	}
}
void fill_star_to_buffer()
{
	int i, x, y;
	for (int i = 0; i < scount; ++i) {
		x = star[i].X;
		y = star[i].Y;
		consoleBuffer[x + screen_x * y].Char.AsciiChar = '*';
		consoleBuffer[x + screen_x * y].Attributes = 7;
	}
}
void setcursor(bool visble)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visble;
	lpCursor.dwSize = 20;
	SetConsoleCursorInfo(console, &lpCursor);
}
void gotoxy(int x, int y)
{
	COORD c = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
void setcolor(int fg, int bg)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, bg * 16 + fg);
}
void erase_colour(int x, int y) {
	setcolor(7, 0);
	printf(" ");
}
void erase_ship(int x, int y)
{
	setcolor(7, 0);
	printf("       ");
}
void draw_ship(int x, int y)
{
	gotoxy(x, y);
	setcolor(7, 0);
	printf(" <-0-> ");
}
void Ammo(int x, int y)
{
	setcolor(7, 0);
	printf("   |   ");
}
char cursor(int x, int y) {
	HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
	char buf[2]; COORD c = { x,y }; DWORD num_read;
	if (
		!ReadConsoleOutputCharacter(hStd, (LPTSTR)buf, 1, c, (LPDWORD)&num_read))
		return '\0';
	else
		return buf[0];
}
void meteo(int x, int y)
{//65010536
	int i, r, R;
	srand(time(NULL));
	for (i = 0; i < 20; i++)
	{
		r = rand();
		R = rand();
		r = r % 10;
		R = R % 100;
		if (r < 2)
		{
			r = 2;
		}
		if (r > 5)
		{
			r = r - 3;
		}
		if (R < 10)
		{
			R = R * 10;
		}
		if (R > 90)
		{
			R = R / 2;
		}
		gotoxy(R, r);
		setcolor(7, 0);
		printf("*");
	}
}
void score(int x, int y)
{
	gotoxy(100, 1);
	printf("SCORE : %d", x);
}
int main()
{
	bool play = true;
	DWORD numEvents = 0;
	DWORD numEventsRead = 0;
	setConsole(screen_x, screen_y);
	setMode();
	int i = 0;
	int s = 0, c = 7;
	srand(time(NULL));
	setConsole(screen_x, screen_y);
	init_star();
	int x = 0, y = 0;
	while (play)
	{//65010536
		star_fall();
		fill_star_to_buffer();
		fill_buffer_to_console();

		GetNumberOfConsoleInputEvents(rHnd, &numEvents);
		if (numEvents != 0) {
			INPUT_RECORD* eventBuffer = new INPUT_RECORD[numEvents];
			ReadConsoleInput(rHnd, eventBuffer, numEvents, &numEventsRead);
			for (DWORD i = 0; i < numEventsRead; ++i) {
				if (eventBuffer[i].EventType == KEY_EVENT &&
					eventBuffer[i].Event.KeyEvent.bKeyDown == true) {
					if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE || s == 10) {
						clear_buffer();
						fill_buffer_to_console();
						play = false;
					}

				}
				else if (eventBuffer[i].EventType == MOUSE_EVENT) {
					int posx = eventBuffer[i].Event.MouseEvent.dwMousePosition.X;
					int posy = eventBuffer[i].Event.MouseEvent.dwMousePosition.Y;
					if (eventBuffer[i].Event.MouseEvent.dwButtonState &
						FROM_LEFT_1ST_BUTTON_PRESSED) {

					}
					else if (eventBuffer[i].Event.MouseEvent.dwButtonState &
						RIGHTMOST_BUTTON_PRESSED) {

					}
					else if (eventBuffer[i].Event.MouseEvent.dwEventFlags & MOUSE_MOVED) {
						draw_ship(posx, posy);
						x = posx;
						y = posy;
					}
				}
			}
			delete[] eventBuffer;
		}
		clear_buffer();
		draw_ship(x, y);
		if (cursor(x, y) == '*') {
			s++;
		}
		Sleep(100);
	}
	return 0;
}
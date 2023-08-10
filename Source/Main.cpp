#include <iostream>
#include <Windows.h>
#include <cmath>
#include <vector>
#include <thread>
struct Render_State {
	int height, width;
	void* memory;

	BITMAPINFO bitmap_info;
};
Render_State render_state;
std::thread changeGridColor;
unsigned int grid_color; 
bool win = false;
std::string diagonal_string;
std::string oneline_string;
std::string oneline_string1;
std::string oneline_string2;
std::string up_string;
std::string up_string1;
std::string up_string2;
std::string current_state = "x";
std::string board[3][3] = { { "", "", "" }, { "", "", "" }, { "", "", "" }};
// DEFINING ALL THE FIELDS POSITIONS
// SIZE OF ONE CELL: 130 PX
RECT rect1 = { 0, 0, 130, 130 };
RECT rect2 = { 130, 0, 260, 130 };
RECT rect3 = { 260, 0, 390, 130 };
RECT rect4 = { 0, 130, 130, 260 };
RECT rect5 = { 130, 130, 260, 260 };
RECT rect6 = { 260, 130, 390, 260 };
RECT rect7 = { 0, 260, 130, 390 };
RECT rect8 = { 130, 260, 260, 390 };
RECT rect9 = { 260, 260, 390, 390 };
// END DEFINING ALL THE FIELDS POSITIONS
std::vector<int> rainbowColors =
{
	0xff0000, // RED
	0xff7f00, // ORANGE
	0xffff00, // YELLOW
	0x00ff00, // GREEN
	0x0000ff, // BLUE
	0x4b0082, // INDIGO
	0x9400d3  // VIOLET
};
int current_i_colors = 0;

void set_background(unsigned int color)
{
	unsigned int* pixel = (unsigned int*)render_state.memory;
	for (int y = 0; y < render_state.height; y++) {
		for (int x = 0; x < render_state.width; x++) {
			*pixel++ = color;
		}
	}
}

void draw_rect(int x0, int y0, int x1, int y1, unsigned int color)
{
	for (int y = y0; y < y1; y++) {
		unsigned int* pixel = (unsigned int*)render_state.memory + x0 + y * render_state.width;

		for (int x = x0; x < x1; x++) {
			*pixel++ = color;
		}
	}
}

void draw_grid(int canvasWidth, int canvasHeight, unsigned int color)
{
	// BIG WARNING: This code is VERY VERY DIRTY :(
	// needs fix...
	// BIG WARNING: This code is VERY VERY HARD to READ... :(
	// needs fix too...
	int thickness = 2;
	// MAIN LINES (HALF OF GRID)
	for (int i = 1; i < 3; i++)
	{
		// HORIZONTAL
		draw_rect(canvasWidth / 4, (canvasHeight / 4) * i, (canvasWidth / 4) * 3, (canvasHeight / 4) * i + thickness, color); 
		// temp: ( x0 ) + canvasWidth / 4, y0 + canvasHeight / 4, x1 + canvasWidth, y1 + canvasHeight (+ 4 -> thickness)
		// VERTICAL
		draw_rect((canvasWidth / 4) * i, canvasHeight / 4, (canvasHeight / 4) * i + thickness, (canvasWidth / 4) * 3, color);
		// temp: x0, y0, x1, y1 (unfinished and unclean), see above for more information about making the grid. (also unfinished and unclean)
	}
	// FINSISHING LINES (OTHER HALF OF GRID)

	// HORIZONTAL
	draw_rect((canvasWidth / 4) - 125, (canvasHeight / 4) / 1.01 + 2, (canvasWidth / 4), (canvasHeight / 4) + 2, color);
	draw_rect((canvasWidth / 4) - 125, (canvasHeight / 4) / 1.01 + 127, (canvasWidth / 4), (canvasHeight / 4) + 127, color);
	// VERTICAL
	draw_rect((canvasHeight / 4) / 1.01 + 127, (canvasWidth / 4) - 125, (canvasHeight / 4) + 127, (canvasWidth / 4), color);
	draw_rect((canvasHeight / 4) / 1.01 + 2, (canvasWidth / 4) - 125, (canvasHeight / 4) + 2, (canvasHeight / 4), color);

}
void detect_win()
{
	// detecting for any wins in the bord

	// DIAGONAL WIN L->R
	diagonal_string = "";
	for (int i = 0; i < 3; ++i) 
	{
		for (int j = 0; j < 3; ++j)
		{
			if (j == 0 && i == 0)
			{
				diagonal_string += board[i][j];
			}
			else if (j == 1 && i == 1) 
			{
				diagonal_string += board[i][j];
			}
			else if (j == 2 && i == 2)
			{
				diagonal_string += board[i][j];
			}
		};
	};
	if (diagonal_string == "xxx")
	{
		MessageBox(NULL, L"Good work!'X' has won!(L->R Diagonal)", L"Game End", MB_OK | MB_ICONINFORMATION);
		return;
	}
	else if (diagonal_string == "ooo")
	{
		MessageBox(NULL, L"Good work! 'O' has won! (L->R Diagonal)", L"Game End", MB_OK | MB_ICONINFORMATION);
		return;
	}
	// DIAGONAL WIN L<-R
	// reset diagonal string
	diagonal_string = "";
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			if (j == 2 && i == 0)
			{
				diagonal_string += board[i][j];
			} 
			else if (j == 1 && i == 1)
			{
				diagonal_string += board[i][j];
			}
			else if (j == 0 && i == 2)
			{
				diagonal_string += board[i][j];
			}

		}
	}
	if (diagonal_string == "xxx")
	{
		MessageBox(NULL, L"Good work!'X' has won!(L<-R Diagonal)", L"Game End", MB_OK | MB_ICONINFORMATION);
		return;
	}
	else if (diagonal_string == "ooo")
	{
		MessageBox(NULL, L"Good work! 'O' has won! (L<-R Diagonal)", L"Game End", MB_OK | MB_ICONINFORMATION);
		return;
	}
	// WIN L -> R
	oneline_string = "";
	oneline_string1 = "";
	oneline_string2 = "";
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			if (j == 0 && i == 0)
			{
				oneline_string += board[i][j];
			}
			else if (j == 1 && i == 0)
			{
				oneline_string += board[i][j];
			}
			else if (j == 2 && i == 0)
			{
				oneline_string += board[i][j];
			}
			else if (j == 0 && i == 1)
			{
				oneline_string1 += board[i][j];
			}
			else if (j == 1 && i == 1)
			{
				oneline_string1 += board[i][j];
			}
			else if (j == 2 && i == 1)
			{
				oneline_string1 += board[i][j];
			}
			else if (j == 1 && i == 2)
			{
				oneline_string2 += board[i][j];
			}
			else if (j == 1 && i == 2)
			{
				oneline_string2 += board[i][j];
			}
			else if (j == 2 && i == 2)
			{
				oneline_string2 += board[i][j];
			}

		}
	}
	if (oneline_string == "xxx")
	{
		MessageBox(NULL, L"Good work!'X' has won!(L-><-R)", L"Game End", MB_OK | MB_ICONINFORMATION);
		return;
	}
	else if (oneline_string == "ooo")
	{
		MessageBox(NULL, L"Good work! 'O' has won! (L-><-R)", L"Game End", MB_OK | MB_ICONINFORMATION);
		return;
	}
	if (oneline_string1 == "xxx")
	{
		MessageBox(NULL, L"Good work!'X' has won!(L-><-R)", L"Game End", MB_OK | MB_ICONINFORMATION);
		return;
	}
	else if (oneline_string1 == "ooo")
	{
		MessageBox(NULL, L"Good work! 'O' has won! (L-><-R)", L"Game End", MB_OK | MB_ICONINFORMATION);
		return;
	}
	if (oneline_string2 == "xxx")
	{
		MessageBox(NULL, L"Good work!'X' has won!(L-><-R)", L"Game End", MB_OK | MB_ICONINFORMATION);
		return;
	}
	else if (oneline_string2 == "ooo")
	{
		MessageBox(NULL, L"Good work! 'O' has won! (L-><-R)", L"Game End", MB_OK | MB_ICONINFORMATION);
		return;
	}
	up_string = "";
	up_string1 = "";
	up_string2 = "";
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			if (j == 0 && i == 0)
			{
				up_string += board[i][j];
			}
			else if (j == 0 && i == 1)
			{
				up_string += board[i][j];
			}
			else if (j == 0 && i == 2)
			{
				up_string += board[i][j];
			}
			else if (j == 1 && i == 0)
			{
				up_string1 += board[i][j];
			}
			else if (j == 1 && i == 1)
			{
				up_string1 += board[i][j];
			}
			else if (j == 1 && i == 2)
			{
				up_string1 += board[i][j];
			}
			else if (j == 2 && i == 0)
			{
				up_string2 += board[i][j];
			}
			else if (j == 2 && i == 1)
			{
				up_string2 += board[i][j];
			}
			else if (j == 2 && i == 2)
			{
				up_string2 += board[i][j];
			}

		}
	}
	if (up_string == "xxx")
	{
		MessageBox(NULL, L"Good work!'X' has won!(U->D)", L"Game End", MB_OK | MB_ICONINFORMATION);
		return;
	}
	else if (up_string == "ooo")
	{
		MessageBox(NULL, L"Good work! 'O' has won! (U->D)", L"Game End", MB_OK | MB_ICONINFORMATION);
		return;
	}
	if (up_string1 == "xxx")
	{
		MessageBox(NULL, L"Good work!'X' has won!(U->D)", L"Game End", MB_OK | MB_ICONINFORMATION);
		return;
	}
	else if (up_string1 == "ooo")
	{
		MessageBox(NULL, L"Good work! 'O' has won! (U->D)", L"Game End", MB_OK | MB_ICONINFORMATION);
		return;
	}
	if (up_string2 == "xxx")
	{
		MessageBox(NULL, L"Good work!'X' has won!(U->D)", L"Game End", MB_OK | MB_ICONINFORMATION);
		return;
	}
	else if (up_string2 == "ooo")
	{
		MessageBox(NULL, L"Good work! 'O' has won! (U->D)", L"Game End", MB_OK | MB_ICONINFORMATION);
		return;
	}
}
void draw_x(int x, int y, unsigned int color) 
{
	int posX;
	int posY;

	switch (x)
	{
	case 1:
		posX = 10;
		break;
	case 2:
		posX = 135;
		break;
	case 3:
		posX = 260;
		break;
	default:
		std::cout << "TICTACTOE::ERR? Unknow x axis value at drawning a X.. (invalid 0=>3)." << std::endl;
		posX = 10;
		break;
	}
	switch (y)
	{
	case 1:
		posY = 260;
		break;
	case 2:
		posY = 142;
		break;
	case 3:
		posY = 15;
		break;
	default:
		std::cout << "TICTACTOE::ERR? Unknow y axis value at drawning a X... (invalid 0=>3)" << std::endl;
		posY = 260;
		break;
	}
	int len = 100;
	int thickness = 2;

	// METHOD:
	// [][][][] => Gets drawn on the first thing
	// [][][][] => Gets drawn on the second thing
	// thanks, my friend bing for the idea of creating a cross
	for (int i = 0; i < len; ++i) // i => pixels used to write...
	{
		draw_rect(posX + i, posY + i, posX + i + thickness, posY + i + thickness, color);
		draw_rect(posX + i, posY + len - i - thickness, posX + i + thickness, posY + len - i, color);
	}

};
void draw_o(int x, int y, unsigned int color)
{
	int posX;
	int posY;

	switch (x)
	{
	case 1:
		posX = 62;
		break;
	case 2:
		posX = 185;
		break;
	case 3:
		posX = 315;
		break;
	default:
		std::cout << "TICTACTOE::ERR? Unknow x axis value at drawning a O... (invalid 0=>3)" << std::endl;
		posX = 10;
		break;
	}
	switch (y)
	{
	case 1:
		posY = 315;
		break;
	case 2:
		posY = 190;
		break;
	case 3:
		posY = 65;
		break;
	default:
		std::cout << "TICTACTOE::ERR? Unknow y axis value at drawning a O... (invalid 0=>3)" << std::endl;
		posY = 260;
		break;
	}

	// DRAWING 'O'
	// thanks my good old friend bing
	int radius = 50;
	int thickness = 2;
	for (int angle = 0; angle < 360; ++angle)
	{ 
		// calculations of the rectangle used to draw circle...
		int xPos = posX + static_cast<int>(radius * cos(angle * 3.14 / 180));
		int yPos = posY + static_cast<int>(radius * sin(angle * 3.14 / 180)); // 3.14 is PI, if you didnt already learned...
		draw_rect(xPos, yPos, xPos + thickness, yPos + thickness, color);
	}


};
void update_grid_color()
{
	while (true)
	{
		Sleep(1000); // 100 milliseconds => 1 second
		grid_color = rainbowColors[current_i_colors];
		if (current_i_colors == 6)
		{
			current_i_colors = 0;
		}
		else
		{
			current_i_colors++;
		}
	}

}
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{

	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		changeGridColor.detach();
		return 0;
		break;


		break;
	case WM_LBUTTONDOWN:

		POINT cursorScreenPos;
		GetCursorPos(&cursorScreenPos);

		ScreenToClient(hWnd, &cursorScreenPos);
		
		
		std::cout << "[!] TICTACTOE?INFO << Posting to board:  X: " << cursorScreenPos.x << " and Y: " << cursorScreenPos.y << std::endl;
		
		// not beautiful, i know, i know... (your probably yelling at me)
		if (PtInRect(&rect1, cursorScreenPos))
		{
			if (board[0][0] == current_state || board[0][0] == "")
			{
				board[0][0] = current_state;
			}
			else
			{
				std::cout << "[!!!] WARNING: Cannot change an already placed spot..." << std::endl;
				// update it, so it gets back updated
				if (current_state == "x")
				{
					current_state = "o";
				}
				else if (current_state == "o")
				{
					current_state = "x";
				}
			};

		} 
		else if (PtInRect(&rect2, cursorScreenPos))
		{
			if (board[0][1] == current_state || board[0][1] == "")
			{
				board[0][1] = current_state;
			}
			else
			{
				std::cout << "[!!!] WARNING: Cannot change an already placed spot..." << std::endl;
				// update it, so it gets back updated
				if (current_state == "x")
				{
					current_state = "o";
				}
				else if (current_state == "o")
				{
					current_state = "x";
				}
			};
		}
		else if (PtInRect(&rect3, cursorScreenPos))
		{
			if (board[0][2] == current_state || board[0][2] == "")
			{
				board[0][2] = current_state;
			}
			else
			{
				std::cout << "[!!!] WARNING: Cannot change an already placed spot..." << std::endl;
				// update it, so it gets back updated
				if (current_state == "x")
				{
					current_state = "o";
				}
				else if (current_state == "o")
				{
					current_state = "x";
				}
			};
		}
		else if (PtInRect(&rect4, cursorScreenPos))
		{
			if (board[1][0] == current_state || board[1][0] == "")
			{
				board[1][0] = current_state;
			}
			else
			{
				std::cout << "[!!!] WARNING: Cannot change an already placed spot..." << std::endl;
				// update it, so it gets back updated
				if (current_state == "x")
				{
					current_state = "o";
				}
				else if (current_state == "o")
				{
					current_state = "x";
				}
			};
		}
		else if (PtInRect(&rect5, cursorScreenPos))
		{
			if (board[1][1] == current_state || board[1][1] == "")
			{
				board[1][1] = current_state;
			}
			else
			{
				std::cout << "[!!!] WARNING: Cannot change an already placed spot..." << std::endl;
				// update it, so it gets back updated
				if (current_state == "x")
				{
					current_state = "o";
				}
				else if (current_state == "o")
				{
					current_state = "x";
				}
			};
		}
		else if (PtInRect(&rect6, cursorScreenPos))
		{
			if (board[1][2] == current_state || board[1][2] == "")
			{
				board[1][2] = current_state;
			}
			else
			{
				std::cout << "[!!!] WARNING: Cannot change an already placed spot..." << std::endl;
				// update it, so it gets back updated
				if (current_state == "x")
				{
					current_state = "o";
				}
				else if (current_state == "o")
				{
					current_state = "x";
				}
			};
		}
		else if (PtInRect(&rect7, cursorScreenPos))
		{
			if (board[2][0] == current_state || board[2][0] == "")
			{
				board[2][0] = current_state;
			}
			else
			{
				std::cout << "[!!!] WARNING: Cannot change an already placed spot..." << std::endl;
				// update it, so it gets back updated
				if (current_state == "x")
				{
					current_state = "o";
				}
				else if (current_state == "o")
				{
					current_state = "x";
				}
			};
		}
		else if (PtInRect(&rect8, cursorScreenPos))
		{
			if (board[2][1] == current_state || board[2][1] == "")
			{
				board[2][1] = current_state;
			}
			else
			{
				std::cout << "[!!!] WARNING: Cannot change an already placed spot..." << std::endl;
				// update it, so it gets back updated
				if (current_state == "x")
				{
					current_state = "o";
				}
				else if (current_state == "o")
				{
					current_state = "x";
				}
			};
		} 
		else if (PtInRect(&rect9, cursorScreenPos))
		{
			if (board[2][2] == current_state || board[2][2] == "")
			{
				board[2][2] = current_state;
			}
			else
			{
				std::cout << "[!!!] WARNING: Cannot change an already placed spot..." << std::endl;
				// update it, so it gets back updated
				if (current_state == "x")
				{
					current_state = "o";
				}
				else if (current_state == "o")
				{
					current_state = "x";
				}
			};
		}
		else
		{
			std::cout << "TICTACTOE?ERROR << Invalid mouse click on TicTacToe board... (Maybe this is because you maximized it => undo it) DETAILS: X: " << cursorScreenPos.x << " and Y: " << cursorScreenPos.y << std::endl;
		}

		if (current_state == "x")
		{
			current_state = "o";
		}
		else if (current_state == "o")
		{
			current_state = "x";
		}
		// PRINTING THE CURRENT BOARD
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				std::cout << board[i][j] << " ";
			}
			std::cout << std::endl;
		}




		break;
	case WM_SIZE:
		RECT rect;
		GetClientRect(hWnd, &rect);
		render_state.width = rect.right - rect.left;
		render_state.height = rect.bottom - rect.top;

		int size = render_state.width * render_state.height * sizeof(unsigned int);

		if (render_state.memory) 
			VirtualFree(render_state.memory, 0, MEM_RELEASE);
		render_state.memory = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		render_state.bitmap_info.bmiHeader.biSize = sizeof(render_state.bitmap_info.bmiHeader);
		render_state.bitmap_info.bmiHeader.biWidth = render_state.width;
		render_state.bitmap_info.bmiHeader.biHeight = render_state.height;
		render_state.bitmap_info.bmiHeader.biPlanes = 1;
		render_state.bitmap_info.bmiHeader.biBitCount = 32;
		render_state.bitmap_info.bmiHeader.biCompression = BI_RGB;
		break;
	}
	return DefWindowProcW(hWnd, msg, wParam, lParam);
};


int WINAPI main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	WNDCLASS window_class = {};
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpszClassName = L"TicTacToe Class";
	window_class.hInstance = hInstance;
	window_class.lpfnWndProc = WndProc;


	RegisterClass(&window_class);
	HWND hWnd = CreateWindowW(window_class.lpszClassName, L"TicTacToe Game", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 390, 415, NULL, NULL, NULL, NULL);


	MSG msg = { 0 };

	HDC hdc = GetDC(hWnd);
	changeGridColor = std::thread(update_grid_color);
	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		int canvasWidth = 500;
		int canvasHeight = 500;
		TranslateMessage(&msg);
		DispatchMessage(&msg);



		set_background(0xffffff);
		draw_grid(canvasWidth, canvasHeight, grid_color);

		for (int row = 0; row < 3; ++row) {
			for (int col = 0; col < 3; ++col) {
				if (board[row][col] == "o") {
					draw_o(col + 1, row + 1, grid_color);
				}
				else if (board[row][col] == "x") {
					draw_x(col + 1, row + 1, grid_color);
				}
			}
		}
		detect_win();
		StretchDIBits(hdc, 0, 0, render_state.width, render_state.height, 0, 0, render_state.width, render_state.height, render_state.memory, &render_state.bitmap_info, DIB_RGB_COLORS, SRCCOPY);
	};
	return 0;
};

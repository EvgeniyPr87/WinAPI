#include<Windows.h>
#include"resource.h"
#include<string>
#include<sstream>

CONST CHAR g_sz_CLASS_NAME[] = "Calc_SPU_411";


CONST INT g_i_BUTTON_SIZE =60;
CONST INT g_i_INTERVAL =2;
CONST INT g_i_BUTTON_DOUBLE_SIZE =g_i_BUTTON_SIZE*2+g_i_INTERVAL;

CONST INT g_i_START_X = 10;
CONST INT g_i_START_Y = 10;

CONST INT g_i_SCREEN_WIDTH = (g_i_BUTTON_SIZE + g_i_INTERVAL) * 5-g_i_INTERVAL;
CONST INT g_i_SCREEN_HEIGHT = g_i_BUTTON_SIZE;

CONST INT g_i_BUTTON_START_X = g_i_START_X;
CONST INT g_i_BUTTON_START_Y = g_i_START_Y+g_i_SCREEN_HEIGHT+g_i_INTERVAL*4;

CONST CHAR g_OPERATIONS[] = "+-*/";
CONST INT g_i_OPERATIONS_START_X = g_i_BUTTON_START_X+(g_i_BUTTON_SIZE + g_i_INTERVAL)*3;

#define BUTTON_SHIFT_X(shift) g_i_BUTTON_START_X+ (g_i_BUTTON_SIZE+g_i_INTERVAL)*(shift)
#define BUTTON_SHIFT_Y(shift) g_i_BUTTON_START_Y+ (g_i_BUTTON_SIZE+g_i_INTERVAL)*(shift)

//Состояния калькулятора:
std::string g_s_currentInput = "0";
std::string g_s_storedNumber = "0";

CHAR g_sz_operation = '\0';
BOOL newUserInput = true;
HWND hEdit=NULL ;





//void UpdateDisplay(HWND hEdit, const std::string& text);

void ProcessingNumbers(HWND hEdit, char symbol);

std::string Operation(const std::string& sNumber1, const std::string& sNumber2, char operation);

void Clear();

void Delete();


LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);//процедура окна

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	//1.Регистрация класса окна:
	WNDCLASSEX wClass;// создаем экземпляр структуры класса
	ZeroMemory(&wClass, sizeof(wClass));//

	wClass.style = 0;
	wClass.cbSize = sizeof(wClass);
	wClass.cbWndExtra = 0;
	wClass.cbClsExtra = 0;

	wClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wClass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wClass.hbrBackground = (HBRUSH)COLOR_WINDOW;

	wClass.hInstance = hInstance;
	wClass.lpfnWndProc = WndProc;
	wClass.lpszMenuName = NULL;
	wClass.lpszClassName = g_sz_CLASS_NAME;

	if (!RegisterClassEx(&wClass))
	{
		MessageBox(NULL, "Class registration failed", "", MB_OK | MB_ICONERROR);
		return 0;
	}
	
	//2.Создание окна:
	
	HWND hwnd = CreateWindowEx
	(
NULL,
g_sz_CLASS_NAME,
g_sz_CLASS_NAME,
WS_OVERLAPPEDWINDOW^WS_THICKFRAME^WS_MAXIMIZEBOX,
CW_USEDEFAULT,CW_USEDEFAULT,
g_i_SCREEN_WIDTH + 2 * g_i_START_X+16,
g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL)*4+16+32,
NULL,
NULL,
hInstance,
NULL
	);
	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window creation failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	//3.Запуск цикла сообщений:
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;

}
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		


		HWND hEdit = CreateWindowEx
		(
			NULL, "Edit", "",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT,
			10, 10,
			g_i_SCREEN_WIDTH, g_i_SCREEN_HEIGHT,
			hwnd,
			(HMENU)IDC_EDIT,
			GetModuleHandle(NULL),
			NULL
		); 
		if (hEdit != NULL) { SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)g_s_currentInput.c_str()); }
		
		INT digit = 1;
		CHAR sz_digit[2] = "";
		for (int i = 6; i >= 0; i -= 3)
		{
			for (int j = 0; j < 3; j++)
			{
				sz_digit[0] = digit++ + '0';
				CreateWindowEx
				(
					NULL, "Button", sz_digit,
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					g_i_BUTTON_START_X + (g_i_BUTTON_SIZE + g_i_INTERVAL) * j,
					g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * i/3,
					g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
					hwnd,
					(HMENU)(IDC_BUTTON_0 + digit-1),
					GetModuleHandle(NULL),
				NULL
				);

				CreateWindowEx
				(
					NULL, "Button", "<-",
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					BUTTON_SHIFT_X(4), BUTTON_SHIFT_Y(0),
					g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
					hwnd,
					(HMENU)IDC_BUTTON_BSP,
					GetModuleHandle(NULL),
					NULL
				);

			}
		}
		CreateWindowEx
		(
			NULL, "Button", "0",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			g_i_BUTTON_START_X, g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 3,
			g_i_BUTTON_DOUBLE_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)(IDC_BUTTON_0),
			GetModuleHandle(NULL),
			NULL
			);

		CreateWindowEx
		(
			NULL, "Button", ".",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			//g_i_BUTTON_START_X + g_i_BUTTON_DOUBLE_SIZE + g_i_INTERVAL,
			//g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 3,
			BUTTON_SHIFT_X(2),BUTTON_SHIFT_Y(3),
			g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)(IDC_BUTTON_POINT),
			GetModuleHandle(NULL),
			NULL
		);

		CreateWindowEx
		(
			NULL, "Button", "C",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			BUTTON_SHIFT_X(4), BUTTON_SHIFT_Y(1),
			g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_CLR,
			GetModuleHandle(NULL), 
			NULL
		);

		CreateWindowEx
		(
			NULL, "Button", "=",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			BUTTON_SHIFT_X(4), BUTTON_SHIFT_Y(2),
			g_i_BUTTON_SIZE, g_i_BUTTON_DOUBLE_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_EQUAL,
			GetModuleHandle(NULL),
			NULL
		);

		CHAR operation[2] = "";
		for (int i = 0; i < 4; i++)
		{
			operation[0] = g_OPERATIONS[i];



			CreateWindowEx
			(
				NULL, "Button", operation,
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				g_i_OPERATIONS_START_X,
				g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE+g_i_INTERVAL) * (3 - i),
				g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
				hwnd,
				(HMENU)(IDC_BUTTON_PLUS +1),
				GetModuleHandle(NULL),
				NULL
			);
		}
	}
		break;
	case WM_COMMAND:
	{
		INT wmId = LOWORD(wParam);
		
		HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_0:ProcessingNumbers(hEdit,'0'); break;
		case IDC_BUTTON_1:ProcessingNumbers(hEdit,'1'); break;
		case IDC_BUTTON_2:ProcessingNumbers(hEdit,'2'); break;
		case IDC_BUTTON_3:ProcessingNumbers(hEdit,'3'); break;
		case IDC_BUTTON_4:ProcessingNumbers(hEdit,'4'); break;
		case IDC_BUTTON_5:ProcessingNumbers(hEdit,'5'); break;
		case IDC_BUTTON_6:ProcessingNumbers(hEdit,'6'); break;
		case IDC_BUTTON_7:ProcessingNumbers(hEdit,'7'); break;
		case IDC_BUTTON_8:ProcessingNumbers(hEdit,'8'); break;
		case IDC_BUTTON_9:ProcessingNumbers(hEdit,'9'); break;
		case IDC_BUTTON_POINT:ProcessingNumbers(hEdit, '.'); break;
		case IDC_BUTTON_CLR:Clear(); break;
		case IDC_BUTTON_BSP:Delete(); break;
		
		case IDC_BUTTON_PLUS:
		case IDC_BUTTON_MINUS:
		case IDC_BUTTON_ASTER:
		case IDC_BUTTON_SLASH:
		{
			if (g_sz_operation != '\0')
			{
				g_s_currentInput = Operation(g_s_storedNumber, g_s_currentInput, g_sz_operation);
				if (g_s_storedNumber == "Error")
				{
					SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)g_s_currentInput.c_str());
					g_sz_operation = '\0';
					newUserInput = true;
				}
				else
				{
					SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)g_s_currentInput.c_str());
					g_s_currentInput = g_s_storedNumber;
				}
			}
			else if (!g_s_currentInput.empty())
			{
				g_s_storedNumber = g_s_currentInput;
			}
			if (wmId==IDC_BUTTON_PLUS) g_sz_operation = '+';
			else if (wmId == IDC_BUTTON_MINUS) g_sz_operation = '-';
			else if (wmId == IDC_BUTTON_ASTER) g_sz_operation = '*';
			else if (wmId == IDC_BUTTON_SLASH) g_sz_operation = '/';
			newUserInput = true;
		}
		break;

		case IDC_BUTTON_EQUAL:
			if (g_sz_operation != '\0' && g_s_currentInput != "Error")
			{
				g_s_currentInput = Operation(g_s_currentInput, g_s_storedNumber, g_sz_operation);
				SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)g_s_currentInput.c_str());

				g_s_storedNumber = g_s_currentInput;
				g_sz_operation = '\0';
				newUserInput = true;
			}
			else if (g_s_currentInput == "Error") SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)g_s_currentInput.c_str());
			break;

		default: return FALSE;
		}
		return TRUE;
	}

		
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		SendMessage(hwnd, WM_DESTROY, 0, 0);
		break;
	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
	return FALSE;
}

void ProcessingNumbers(HWND hEdit, char symbol)
{
	if (newUserInput)
	{
		if ( g_s_currentInput == "0") g_s_currentInput = "";
		
		else if (!g_s_currentInput.empty())
		{
			g_s_currentInput = "";
			newUserInput = false;
		}
	}
	if (symbol == '.')
	{
		if (g_s_currentInput.find('.') == std::string::npos)
		{
			if (newUserInput)
			{
				g_s_currentInput = "0.";
				newUserInput = false;
			}
			else
			{
				g_s_currentInput += '.';
				newUserInput = false;
			}
		}
	}
	else
	{
		if (newUserInput)
		{
			if (symbol == '0')  g_s_currentInput = "0";
			else
			{
				g_s_currentInput = symbol;
				newUserInput = false;
			}
		}
		else if (newUserInput == false) g_s_currentInput += symbol;
	}
	SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)g_s_currentInput.c_str());
}



std::string Operation(const std::string& sNumber1, const std::string& sNumber2, char g_sz_operation)
{
	DOUBLE number1 = std::stod(sNumber1);
	DOUBLE number2 = std::stod(sNumber2);
	DOUBLE result = 0;
	switch (g_sz_operation)
	{
	case '+': result = number1 + number2; break;
	case '-': result = number1 - number2; break;
	case '*': result = number1 * number2; break;
	case'/': if (number2 != 0)
	{
		result = number1 / number2; break;
	}
		
	default: return "Error";
	}
	std::string sResult = std::to_string(result);;
	return  sResult;
}

void Clear()
{
	g_s_currentInput = "0";
	g_s_storedNumber = "0";
	g_sz_operation = '\0';
	SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)g_s_currentInput.c_str());
	newUserInput = true;
}
void Delete()
{
	if (!g_s_currentInput.empty())  g_s_currentInput.pop_back();
	if (g_s_currentInput.empty()) { g_s_currentInput = "0"; newUserInput = true; }
	else if (g_s_currentInput == ".") g_s_currentInput = "0";
	SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)g_s_currentInput.c_str());
}

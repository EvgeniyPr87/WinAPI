#include<Windows.h>
#include"resource.h"
#include<string>
#include<format>

CONST CHAR g_sz_WND_CLASS_NAME[] = "My Windows Class";//создаем имя класса окна 

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	//1.Регистрация класса окна:
	WNDCLASSEX wClass;//создание и инициализирование структуры
	ZeroMemory(&wClass, sizeof(wClass));//обнуление
	// далее инициализирукем структуру четырьмя блоками: 
	//первый блок- инициализируем только размеры
	wClass.style = 0;
	wClass.cbSize = sizeof(wClass);
	wClass.cbWndExtra = 0;
	wClass.cbClsExtra = 0;

	//второй блок- инициализирует внешний вид окон класса
	//wClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	//wClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wClass.hIcon = (HICON)LoadImage(hInstance, "Bitcoin.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	wClass.hIconSm =(HICON) LoadImage(hInstance, "Palm.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	
	//wClass.hCursor = LoadCursor(hInstance,MAKEINTRESOURCE(IDC_CURSOR1));
	wClass.hCursor = (HCURSOR)LoadImage(hInstance, "mb_diagonal_left_resize.cur", IMAGE_CURSOR, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	wClass.hbrBackground = (HBRUSH)COLOR_WINDOW;

	//третий блок- инициализирует 
	wClass.hInstance = hInstance;
	wClass.lpszClassName = g_sz_WND_CLASS_NAME;
	wClass.lpszMenuName = NULL;
	wClass.lpfnWndProc = WndProc;

	if (!RegisterClassEx(&wClass))
	{
		MessageBox(NULL, "Class registration failed", "", MB_OK | MB_ICONERROR);
	}
	// ---------------------------------------
	INT screenWidth = GetSystemMetrics(SM_CXSCREEN);
	INT screenHight = GetSystemMetrics(SM_CYSCREEN);
	DOUBLE persentWindow = 0.75;

	INT windowWidth = screenWidth * persentWindow;
	INT windowHight = screenHight * persentWindow;

	INT positionX = (screenWidth - windowWidth) / 2;
	INT positionY = (screenHight - windowHight) / 2;

	//-----------------------------------------



	//Создание окна:
	HWND hwnd = CreateWindowEx
	(
		NULL,//exStyles
		g_sz_WND_CLASS_NAME, //Class name
		((std::string)(g_sz_WND_CLASS_NAME)+
		"    " + "[Window size]:"+" " + std::to_string(windowWidth) + "x" + std::to_string(windowHight) +
		"    "+"[Window position]:"+" " + std::to_string(positionX) + "x" + std::to_string(positionY)).c_str(), //Window title
		WS_OVERLAPPEDWINDOW,
		positionX, positionY,//Window position
		windowWidth, windowHight,//Window size
		NULL,//parent window
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
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) //Процедура обычного окна
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		HWND hStatic = CreateWindowEx
		(
			NULL,
			"Static",
			"Этот Static-text создан при помощи функции CreateWindowEx();",
			WS_CHILD| WS_VISIBLE,
			10, 10,
			500, 25,
			hwnd,
			(HMENU)1000,
			GetModuleHandle(NULL),
			NULL
		);
		HWND hEdit = CreateWindowEx
		(
			NULL,
			"Edit",
			"",
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			10, 45,
			500, 20,
			hwnd,
			(HMENU)1001,
			GetModuleHandle(NULL),
			NULL
		);
		HWND hButton = CreateWindowEx
		(
			NULL,
			"Button",
			"Применить",
			WS_CHILD | WS_VISIBLE,
			430, 70,
			80, 32,
			hwnd,
			(HMENU)1002,
			GetModuleHandle(NULL),
			NULL

		);
	}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 1002:
		{
			MessageBox(hwnd, "Привет", "Привет", MB_OK | MB_ICONINFORMATION);
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE] = {};
			HWND hStatic = GetDlgItem(hwnd,1000);
			HWND hEdit = GetDlgItem(hwnd, 1001);

			SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			SendMessage(hStatic, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		}
		break;
		}

		break;
	case WM_DESTROY:
		MessageBox(NULL, "Лудше дверь закройте...", "Finita la comedia", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		//DestroyWindow(hwnd);
		if (MessageBox(hwnd, "Вы действительно хотите закрыть окно?", "Че, в натуре?", MB_YESNO | MB_ICONINFORMATION) == IDYES)
			SendMessage(hwnd, WM_DESTROY, 0, 0);
		break;
	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return FALSE;
}
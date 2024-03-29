// lab4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "math.h"
#include "iostream"
#include <windows.h>
#include "windowsx.h"
#include <gdiplus.h>
#include <conio.h>

using namespace Gdiplus;
using namespace std;

float FPart(float x);
void Line(int x1, int y1, int x2, int y2);
void PutPixel(int x, int y, int alpha);
void DrawWuLine(int x0, int y0, int x1, int y1);
int IPart(float x);
int Sign(double n);

static const ARGB Black = 0xFF000000;
HWND hwnd = GetConsoleWindow();
HDC hdc = GetDC(hwnd);
struct Data {
	int height;
	int width;
	int size;
} d;

//Метод, устанавливающий пиксел с заданной  прозрачностью
void PutPixel(int x, int y, int alpha) {
	if (alpha > 127) {

		//        BYTE trans = (BYTE)(255- alpha);
		//        COLORREF clr = RGB(trans, trans, trans);
		//        HBRUSH Brush = CreateSolidBrush(clr);
		//        SelectBrush(hdc, Brush);
		COLORREF clr = RGB(0, 0, 0);
		HBRUSH Brush = CreateSolidBrush(clr);
		SelectBrush(hdc, Brush);
		Rectangle(hdc, (x * d.size), ((d.height - y) * d.size), ((x + 1) * d.size), ((d.height - y - 1) * d.size));

		//
		// mid grey
		//    if (alpha >= 60 && alpha <= 90) {
		//
		//        COLORREF clr = RGB(120, 120, 120);
		//        HBRUSH Brush = CreateSolidBrush(clr);
		//        SelectBrush(hdc, Brush);
		//
		//        Rectangle(hdc, (x * d.size), ((d.height - y) * d.size), ((x + 1) * d.size), ((d.height - y - 1) * d.size));
		//        //black
		//    } else if (alpha >= 100) {
		//        COLORREF clr = RGB(0, 0, 0);
		//        HBRUSH Brush = CreateSolidBrush(clr);
		//        SelectBrush(hdc, Brush);
		//        Rectangle(hdc, (x * d.size), ((d.height - y) * d.size), ((x + 1) * d.size), ((d.height - y - 1) * d.size));
		//    }
		//        //light grey
		//    else if (alpha < 5 ) {
		//        COLORREF clr = RGB(200, 200, 200);
		//        HBRUSH Brush = CreateSolidBrush(clr);
		//        SelectBrush(hdc, Brush);
		//        Rectangle(hdc, (x * d.size), ((d.height - y) * d.size), ((x + 1) * d.size), ((d.height - y - 1) * d.size));
		//    } else {
		//        BYTE trans = (BYTE)(255 - alpha);
		//        COLORREF clr = RGB(trans, trans, trans);
		//        HBRUSH Brush = CreateSolidBrush(clr);
		//        SelectBrush(hdc, Brush);
		//        Rectangle(hdc, (x * d.size), ((d.height - y) * d.size), ((x + 1) * d.size), ((d.height - y - 1) * d.size));
		//    }
	}
	else {
		COLORREF clr = RGB(222 - alpha, 222 - alpha, 222 - alpha);
		HBRUSH Brush = CreateSolidBrush(clr);
		SelectBrush(hdc, Brush);
		Rectangle(hdc, (x * d.size), ((d.height - y) * d.size), ((x + 1) * d.size), ((d.height - y - 1) * d.size));
	}
}
//Целая часть числа
int IPart(float x) {
	return (int)x;
}

//дробная часть числа
float FPart(float x) {
	while (x >= 0)
		x--;
	x++;
	return x;
}

void DrawWuLine(int x0, int y0, int x1, int y1) {


	//Вычисление изменения координат
	int dx = (x1 > x0) ? (x1 - x0) : (x0 - x1);
	int dy = (y1 > y0) ? (y1 - y0) : (y0 - y1);
	//Если линия параллельна одной из осей, рисуем обычную линию - заполняем все пикселы в ряд
	if (dx == 0 || dy == 0) {
		Line(x0, y0, x1, y1);
		return;
	}

	//Для Х-линии (коэффициент наклона < 1)
	if (dy < dx) {
		//Первая точка должна иметь меньшую координату Х
		if (x1 < x0) {
			x1 += x0;
			x0 = x1 - x0;
			x1 -= x0;
			y1 += y0;
			y0 = y1 - y0;
			y1 -= y0;
		}
		//Относительное изменение координаты Y
		float grad = (float)dy / dx;
		//Промежуточная переменная для Y
		float intery = y0 + grad;
		//Первая точка
		PutPixel(x0, y0, 255);

		for (int x = x0 + 1; x < x1; x++) {
			//Верхняя точка
			PutPixel(x, IPart(intery), (int)(255 - FPart(intery) * 255));
			//Нижняя точка
			PutPixel(x, IPart(intery) + 1, (int)(FPart(intery) * 255));
			//Изменение координаты Y
			intery += grad;
		}
		//Последняя точка
		PutPixel(x1, y1, 255);
	}
	//Для Y-линии (коэффициент наклона > 1)
	else {
		//Первая точка должна иметь меньшую координату Y
		if (y1 < y0) {
			x1 += x0;
			x0 = x1 - x0;
			x1 -= x0;
			y1 += y0;
			y0 = y1 - y0;
			y1 -= y0;
		}
		//Относительное изменение координаты X
		float grad = (float)dx / dy;
		//Промежуточная переменная для X
		float interx = x0 + grad;
		//Первая точка
		PutPixel(x0, y0, 255);

		for (int y = y0 + 1; y < y1; y++) {
			//Верхняя точка
			PutPixel(IPart(interx), y, 255 - (int)(FPart(interx) * 255));
			//Нижняя точка
			PutPixel(IPart(interx) + 1, y, (int)(FPart(interx) * 255));
			//Изменение координаты X
			interx += grad;
		}
		//Последняя точка
		PutPixel(x1, y1, 255);
	}
}

void DrawGrid() {
	Rectangle(hdc, 0, 0, d.width * d.size, d.height * d.size);
	int x, y, x1, y1;
	x = y = x1 = y1 = 0;//устанавливаем начальное положение пера

						//рисование по y
	for (int i = 0; i <= (d.height); i++) {
		MoveToEx(hdc, x, y, NULL);
		LineTo(hdc, x + (d.width * d.size), y);
		y += (d.size);
	}

	//рисование по x
	for (int j = 0; j <= d.width; j++) {
		MoveToEx(hdc, x1, y1, NULL);
		LineTo(hdc, x1, y1 + (d.height * d.size));
		x1 += (d.size);
	}
}

void Line(int x1, int y1, int x2, int y2) {
	Color col(0, 0, 0);
	double Dx, Dy, length, x, y;
	if (abs(x2 - x1) >= abs(y2 - y1)) length = abs(x2 - x1);
	else length = abs(y2 - y1);
	Dx = (x2 - x1) / length;
	Dy = (y2 - y1) / length; // или Dх или Dу равно 1
	x = x1 + 0.5 * Sign(Dx);
	y = y1 + 0.5 * Sign(Dy); // начальные точки
	int i = 0;
	while (i <= length) {
		PutPixel(x, y, 100);
		x += Dx;
		y += Dy;
		i++;
	}
}

int Sign(double n) {
	if (n < 0) {
		return -1;
	}
	else return 1;
}

int main() {
	HPEN blackPen = GetStockPen(BLACK_PEN);
	HBRUSH blackBrush = GetStockBrush(BLACK_BRUSH);
	int x1, y1, x2, y2;

	cout << "Enter height of grid" << endl;
	cin >> d.height;
	cout << "Enter width of grid" << endl;
	cin >> d.width;
	cout << "Enter size of cell" << endl;
	cin >> d.size;

	cout << "X1 = " << endl;
	cin >> x1;
	cout << "Y1 = " << endl;
	cin >> y1;
	cout << "X2 = " << endl;
	cin >> x2;
	cout << "Y2 = " << endl;
	cin >> y2;


	system("cls");

	SelectBrush(hdc, blackBrush);
	FloodFill(hdc, 0, 0, RGB(0, 0, 1));
	MoveToEx(hdc, 0, 0, NULL);
	LineTo(hdc, 100, 100);
	HBRUSH brush = GetStockBrush(WHITE_BRUSH);
	SelectBrush(hdc, brush);
	SelectPen(hdc, blackPen);
	DrawGrid();
	DeleteObject(blackPen);
	DeleteObject(blackBrush);

	DrawWuLine(x1, y1, x2, y2);
	_getch();
	return 0;
}

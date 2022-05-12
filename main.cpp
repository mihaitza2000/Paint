#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <time.h>
#include <stdlib.h>
using namespace std;
using namespace sf;
int width = 800, height = 800;
int dimPixel = 40;
RenderWindow window(VideoMode(width, height), "Colorare", Style::Default);
RectangleShape pixel(Vector2f(dimPixel, dimPixel));
string v[20][20];
struct pos
{
	int posX;
	int posY;
	pos* next;
	pos* prev;
};
bool isEmpty(pos* head)
{
	if (head == NULL)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void Put(pos*& head, pos*& tail, int x, int y)
{
	pos* p = new pos;
	p->posX = x;
	p->posY = y;
	p->prev = NULL;
	p->next = NULL;
	if (isEmpty(head))
	{
		head = p;
	}
	else
	{
		if (tail == NULL)
		{
			p->next = head;
			head->prev = p;
			tail = p;
		}
		else
		{
			p->next = tail;
			tail->prev = p;
			tail = p;
		}
	}
}
void Get(pos*& head, pos*& tail)
{
	pos* p = head;
	if (head != NULL)
	{
		if (head->prev != NULL)
		{
			head = head->prev;
			head->next = NULL;
			delete p;
		}
		else
		{
			head = NULL;
			tail = NULL;
			delete p;
		}
	}
}
void color(pos* head, pos* tail, pos* pi)
{
	Put(head, tail, pi->posX, pi->posY);
	string col = v[pi->posX][pi->posY];
	v[pi->posX][pi->posY] = "black";
	while (!isEmpty(head))
	{
		int x = head->posX;
		int y = head->posY;  
		Get(head, tail);
		int  i = 0;
		int dirX[8] = {-1, -1,  0,  1, 1, 1, 0, -1};
		int dirY[8] = { 0, -1, -1, -1, 0, 1, 1,  1};
		do
		{
			if ((x + dirX[i] >= 0 && y + dirX[i] < 20) &&
				(y + dirY[i] >= 0 && y + dirY[i] < 20) &&
				v[x + dirX[i]][y + dirY[i]] == col 
				)
			{
				Put(head, tail, x + dirX[i], y + dirY[i]);
				v[x + dirX[i]][y + dirY[i]] = "black";
			}
			i++;
		} while (i != 8);
	}
}
int main()
{
	srand(time(NULL));
	pos* head = NULL;
	pos* tail = NULL;
	int x;
	string colList[] = { "red","green"};
	string col;
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			x = rand() % 2;
			v[i][j] = colList[x];
		}
	}
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			window.clear();
			for (int i = 0; i < 20; i++)
			{
				for (int j = 0; j < 20; j++)
				{
					pixel.setPosition(j * dimPixel, i * dimPixel);
					if (v[i][j] == "red")
					{
						pixel.setFillColor(Color::Red);
					}
					if (v[i][j] == "green")
					{
						pixel.setFillColor(Color::Green);
					}
					if (v[i][j] == "black")
					{
						pixel.setFillColor(Color::Black);
					}
					window.draw(pixel);
				}
			}
			window.display();
			switch (event.type)
			{
			case Event::EventType::Closed:
				window.close();
				break;
			case Event::EventType::MouseButtonPressed:
				if (Mouse::Button::Left == event.mouseButton.button)
				{
					if (Mouse::getPosition(window).x >= 0 &&
						Mouse::getPosition(window).x <= 800 &&
						Mouse::getPosition(window).y >= 0 &&
						Mouse::getPosition(window).y <= 800 && 
						v[Mouse::getPosition(window).y / 40][Mouse::getPosition(window).x / 40] != "black")
					{

						{
							pos* pi = new pos;
							pi->posX = Mouse::getPosition(window).y / 40;
							pi->posY = Mouse::getPosition(window).x / 40;
							pi->next = NULL;
							pi->prev = NULL;
							color(head, tail, pi);
							delete pi;
						}
					}
				}
				break;
			default:
				break;
			}
		} 
		
	}
	return 0;
}
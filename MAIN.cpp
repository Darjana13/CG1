/*
Отключено консольное (главное) окно:
	Linker ->  Advanced -> Entery Point := "mainCRTStartup"
	Linker ->  System -> SubSystem := "Windows (/SUBSYSTEM:WINDOWS)"
*/

#include <stdlib.h>
#include "glut.h"
#include <vector>
#include <windows.h>
#include <math.h>

using namespace std;

GLint Width = 512, Height = 512;

enum keys {
	Empty, KeyR, KeyG, KeyB, KeyW, KeyA, KeyS, KeyD, KeyP,
	KeyO, KeyK, KeyL, Keyt, Keyy, Keyn, Keym, keyz, KeyX
};

struct type_point
{
	GLint x, y;
	type_point() { x = 0; y = 0; }
	type_point(GLint _x, GLint _y) { x = _x; y = _y; }
};
struct type_color
{
	GLubyte r, g, b;
	type_color(GLubyte _r, GLubyte _g, GLubyte _b)
	{
		r = _r;
		g = _g;
		b = _b;
	}
	type_color()
	{
		r = 255;
		g = 255;
		b = 255;
	}
};

struct type_primitive
{
	GLdouble angle = 0;
	type_point center;
	vector <type_point> Points;
	type_primitive()
	{
		Points.resize(3);
	}
};

struct type_group
{
	vector <type_primitive> Primitives;
	vector<type_point> Points;
	type_color Color;
	GLint x = 0, y = 0;

	type_group(vector <type_primitive> _Primitives, type_color _Color)
	{
		Primitives = _Primitives;
		Color.r = _Color.r;
		Color.g = _Color.g;
		Color.b = _Color.b;
		Points.reserve(2);
	}
	type_group(type_color _Color)
	{
		Color.r = _Color.r;
		Color.g = _Color.g;
		Color.b = _Color.b;
		Points.reserve(2);
	}
	type_group()
	{
		Color.r = 255;
		Color.g = 255;
		Color.b = 255;
		Points.reserve(2);
	}
};

vector <type_group> Groups;
vector <type_point> Cur_points;
type_color Default_color;
int Selected_group = -1;
int Selected_primitive = -1;

void get_center(type_primitive& Triang)
{
	double sx = 0, sy = 0;
	int i;
	for (i = 0; i < Triang.Points.size(); i++)
	{
		sx += Triang.Points[i].x;
		sy += Triang.Points[i].y;
	}
	if (i != 0)
	{
		sx /= i;
		sy /= i;
	}
	Triang.center.x = sx;
	Triang.center.y = sy;
}

/* Функция вывода на экран */
void Display(void)
{
	glClearColor(0.5, 0.5, 0.5, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(6);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_STIPPLE);
	type_group* Cur_group;
	type_color* Cur_color;
	type_primitive* Cur_primitive;

	for (int i = 0; i < Groups.size(); i++)
	{
		//glLoadIdentity();
		//glTranslatef(Cur_group->x, Cur_group->y, 0);

		if (Selected_group != i)
		{
			Cur_group = &Groups[i];
			Cur_color = &Cur_group->Color;
			glColor3ub(Cur_color->r, Cur_color->g, Cur_color->b);

			for (int j = 0; j < Cur_group->Primitives.size(); j++)
			{
				Cur_primitive = &(Groups[i].Primitives[j]);

				//glPushMatrix();
				glLoadIdentity();
				glTranslatef(Cur_group->x + Cur_primitive->center.x, Cur_group->y + Cur_primitive->center.y, 0);
				glRotatef(Cur_primitive->angle, 0, 0, 1);
				glTranslatef(-Cur_primitive->center.x, -Cur_primitive->center.y, 0);

				glBegin(GL_TRIANGLES);
				glVertex2i(Cur_primitive->Points[0].x,
					Cur_primitive->Points[0].y);
				glVertex2i(Cur_primitive->Points[1].x,
					Cur_primitive->Points[1].y);
				glVertex2i(Cur_primitive->Points[2].x,
					Cur_primitive->Points[2].y);
				glEnd();

				//glPopMatrix;
			}

			glLoadIdentity();
			glTranslatef(Cur_group->x, Cur_group->y, 0);
			glBegin(GL_POINTS);
			for (int j = 0; j < Cur_group->Points.size(); j++)
				glVertex2i(Cur_group->Points[j].x,
					Groups[i].Points[j].y);
			glEnd();
		}
	}
	// выбранная группа
	if (Selected_group != -1)
	{
		Cur_group = &Groups[Selected_group];
		Cur_color = &Cur_group->Color;

		//glLoadIdentity();
		//glTranslatef(Cur_group->x, Cur_group->y, 0);

		for (int i = 0; i < Cur_group->Primitives.size(); i++)
		{
			glColor3ub(Cur_color->r, Cur_color->g, Cur_color->b);
			Cur_primitive = &(Cur_group->Primitives[i]);

			//glPushMatrix();
			glLoadIdentity();
			glTranslatef(Cur_group->x + Cur_primitive->center.x, Cur_group->y + Cur_primitive->center.y, 0);
			glRotatef(Cur_primitive->angle, 0, 0, 1);
			glTranslatef(-Cur_primitive->center.x, -Cur_primitive->center.y, 0);

			glBegin(GL_TRIANGLES);
			glVertex2i(Cur_primitive->Points[0].x,
				Cur_primitive->Points[0].y);
			glVertex2i(Cur_primitive->Points[1].x,
				Cur_primitive->Points[1].y);
			glVertex2i(Cur_primitive->Points[2].x,
				Cur_primitive->Points[2].y);
			glEnd();

			glColor3ub(Groups[Selected_group].Color.r + 50, Groups[Selected_group].Color.g + 50, Groups[Selected_group].Color.b + 50);
			glBegin(GL_POINTS);
			for (int j = 0; j < 3; j++)
				glVertex2i(Cur_primitive->Points[j].x,
					Cur_primitive->Points[j].y);
			glEnd();

			if (Selected_primitive == i)
			{
				glPointSize(3);
				glDisable(GL_POINT_SMOOTH);

				glBegin(GL_POINTS);
				glVertex2i(Cur_primitive->center.x, Cur_primitive->center.y);
				glEnd();

				glPointSize(6);
				glEnable(GL_POINT_SMOOTH);
			}

			//glPopMatrix();
		}
		glLoadIdentity();
		glTranslatef(Cur_group->x, Cur_group->y, 0);

		glColor3ub(Cur_color->r, Cur_color->g, Cur_color->b);
		glBegin(GL_POINTS);
		for (int j = 0; j < Cur_group->Points.size(); j++)
			glVertex2i(Cur_group->Points[j].x,
				Cur_group->Points[j].y);
		glEnd();
	}
	glLoadIdentity();
	glDisable(GL_POINT_SMOOTH);
	glFinish();
}
/* Функция изменения размеров окна */
void Reshape(GLint w, GLint h)
{
	Width = w; Height = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
/* Функция обработки сообщений от клавиатуры */
void Keyboard(unsigned char key, int x, int y)
{
	type_group* Cur_group = Groups.size() ? &Groups[Selected_group] : NULL;
	type_color* Cur_color = Cur_group ? &Cur_group->Color : &Default_color;
	type_primitive* Cur_triangle = Cur_group && Cur_group->Primitives.size() ? &Cur_group->Primitives[Selected_primitive] : NULL;

	type_group g(Default_color);

	switch (key)
	{
		/* Изменение RGB-компонент цвета точек */
	case (unsigned char)'r':
	case (unsigned char)'R':
	case (unsigned char)'к':
	case (unsigned char)'К':
		Cur_color->r += 5;
		break;
	case (unsigned char)'g':
	case (unsigned char)'G':
	case (unsigned char)'п':
	case (unsigned char)'П':
		Cur_color->g += 5;
		break;
	case (unsigned char)'b':
	case (unsigned char)'B':
	case (unsigned char)'и':
	case (unsigned char)'И':
		Cur_color->b += 5;
		break;
		/* Изменение XY-кординат точек */
	case (unsigned char)'w':
	case (unsigned char)'W':
	case (unsigned char)'ц':
	case (unsigned char)'Ц':
		if (Cur_group)
			Cur_group->y += 9;
		break;
	case (unsigned char)'s':
	case (unsigned char)'S':
	case (unsigned char)'ы':
	case (unsigned char)'Ы':
		if (Cur_group)
			Cur_group->y -= 9;
		break;
	case (unsigned char)'a':
	case (unsigned char)'A':
	case (unsigned char)'ф':
	case (unsigned char)'Ф':
		if (Cur_group)
			Cur_group->x -= 9;
		break;
	case (unsigned char)'d':
	case (unsigned char)'D':
	case (unsigned char)'в':
	case (unsigned char)'В':
		if (Cur_group)
			Cur_group->x += 9;
		break;
	case (unsigned char)'z': // сброс перемещения выбранной группы
	case (unsigned char)'Z':
	case (unsigned char)'я':
	case (unsigned char)'Я':
		if (Cur_group)
		{
			Cur_group->x = 0;
			Cur_group->y = 0;
		}
		break;
	case (unsigned char)'p':  /* завершить группу */
	case (unsigned char)'P':
	case (unsigned char)'з':
	case (unsigned char)'З':
		if (Cur_group && (Cur_group->Points.size() || Cur_group->Primitives.size()))
		{
			if (Groups[Groups.size() - 1].Points.size() || Groups[Groups.size() - 1].Primitives.size())
			{
				Groups.push_back(g);
			}
			Selected_group = Groups.size() - 1;
			Selected_primitive = -1;
		}
		break;

	case (unsigned char)'n':     /* предыдущая группа */
	case (unsigned char)'N':
	case (unsigned char)'т':
	case (unsigned char)'Т':
		if (Cur_group)
		{
			if (Selected_group > 0)
				Selected_group--;
			else
				Selected_group = Groups.size() - 1;
			Selected_primitive = Groups[Selected_group].Primitives.size() - 1;
		}
		break;
	case (unsigned char)'m':     /* следующая группа */
	case (unsigned char)'M':
	case (unsigned char)'ь':
	case (unsigned char)'Ь':
		if (Cur_group)
		{
			if (Selected_group < Groups.size() - 1)
				Selected_group++;
			else
				Selected_group = 0;
			Selected_primitive = Groups[Selected_group].Primitives.size() - 1;
		}
		break;
	case (unsigned char)',':     /* предыдущий примитив */
	case (unsigned char)'<':
	case (unsigned char)'б':
	case (unsigned char)'Б':
		if (Cur_group)
		{
			if (Selected_primitive > 0)
				Selected_primitive--;
			else
				Selected_primitive = Cur_group->Primitives.size() - 1;
		}
		break;
	case (unsigned char)'.':     /* следующий примитив */
	case (unsigned char)'>':
	case (unsigned char)'ю':
	case (unsigned char)'Ю':
		if (Cur_group)
		{
			if (Selected_primitive < Cur_group->Primitives.size() - 1)
				Selected_primitive++;
			else
				Selected_primitive = 0;

		}
		break;
	case (unsigned char)'o':     /* удалить выбранную группу */
	case (unsigned char)'O':
	case (unsigned char)'щ':
	case (unsigned char)'Щ':
		if (!Groups.empty())
		{
			vector<type_group>::iterator iterator = Groups.begin() + Selected_group;
			Groups.erase(iterator);
			if (Groups.empty())
			{
				Selected_group = -1;
				Selected_primitive = -1;
			}
			else
			{
				Selected_group = Selected_group - 1 < 0 ? 0 : Selected_group - 1;
				Selected_primitive = Groups[Selected_group].Primitives.size() - 1;
			}

		}
		break;
	case (unsigned char)'k':     /* поворот влево примитива*/
	case (unsigned char)'K':
	case (unsigned char)'л':
	case (unsigned char)'Л':
		if (Cur_triangle)
			Cur_triangle->angle -= 5;
		break;
	case (unsigned char)'l':     /* поворот вправо примитива*/
	case (unsigned char)'L':
	case (unsigned char)'д':
	case (unsigned char)'Д':
		if (Cur_triangle)
			Cur_triangle->angle += 5;
		break;
	case (unsigned char)'x':     /* cброс угла поворота выбранного примитива */
	case (unsigned char)'X':
	case (unsigned char)'ч':
	case (unsigned char)'Ч':
		if (Cur_triangle)
			Cur_triangle->angle = 0;
		break;

	default:
		break;
	}
	glutPostRedisplay();
}
/* Функция обработки сообщения от мыши */
void Mouse(int button, int state, int x, int y)
{
	if (!Groups.size())
	{
		Groups.resize(1);
		type_group g(Default_color);
		Groups[0] = g;
		Selected_group = 0;
	}
	/* клавиша была нажата, но не отпущена */
	if (state != GLUT_DOWN)
		return;
	/* новая точка по левому клику */
	if (button == GLUT_LEFT_BUTTON)
	{
		type_point p(x - Groups[Selected_group].x, Height - y - Groups[Selected_group].y);

		if (Groups[Selected_group].Points.size() < 2)
			Groups[Selected_group].Points.push_back(p);
		else if (p.x == Groups[Selected_group].Points[0].x &&
			p.x == Groups[Selected_group].Points[1].x ||
			p.y == Groups[Selected_group].Points[0].y &&
			p.y == Groups[Selected_group].Points[1].y)
		{
			LPCTSTR s1 = L"Новая точка на одной линии с двумя предыдущими, не удается нарисовать треугольник.",
				s2 = L"Вы не можете добавить эту точку";
			MessageBox
			(
				NULL, // дескриптор окна владельца
				s1, // адрес текста в окне сообщений
				s2, // адрес заголовка в окне сообщений
				MB_OK // стиль окна сообщений
			);
		}
		else
		{
			type_primitive temp;
			temp.Points[0] = Groups[Selected_group].Points[0];
			temp.Points[1] = Groups[Selected_group].Points[1];
			temp.Points[2] = p;
			Groups[Selected_group].Points.clear();
			get_center(temp);
			Groups[Selected_group].Primitives.push_back(temp);
			Selected_primitive = Groups[Selected_group].Primitives.size() - 1;
		}
	}
	/* удаление последней точки по клику по колесику мыши*/
	if (button == GLUT_MIDDLE_BUTTON)
	{
		if (Groups[Selected_group].Points.size())
			Groups[Selected_group].Points.pop_back();
		else if (Groups[Selected_group].Primitives.size())
		{
			Groups[Selected_group].Points.push_back(Groups[Selected_group].Primitives[Groups[Selected_group].Primitives.size() - 1].Points[0]);
			Groups[Selected_group].Points.push_back(Groups[Selected_group].Primitives[Groups[Selected_group].Primitives.size() - 1].Points[1]);
			Groups[Selected_group].Primitives.pop_back();
			Selected_primitive--;
		}
		vector<type_group>::iterator iterator = Groups.begin() + Selected_group;
		if (Selected_group != Groups.size() - 1 && !Groups[Selected_group].Points.size() && !Groups[Selected_group].Primitives.size())
		{
			Groups.erase(iterator);
			Selected_group = Selected_group - 1 < 0 ? 0 : Selected_group - 1;
			Selected_primitive = Groups[Selected_group].Primitives.size() - 1;
		}
	}

	glutPostRedisplay();
}

void Menu(int pos)
{
	int key = (keys)pos;

	switch (key)
	{
	case KeyR: Keyboard('r', 0, 0); break;
	case KeyG: Keyboard('g', 0, 0); break;
	case KeyB: Keyboard('b', 0, 0); break;
	case KeyW: Keyboard('w', 0, 0); break;
	case KeyS: Keyboard('s', 0, 0); break;
	case KeyA: Keyboard('a', 0, 0); break;
	case KeyD: Keyboard('d', 0, 0); break;
	case KeyP: Keyboard('p', 0, 0); break;
	case KeyO: Keyboard('o', 0, 0); break;
	case KeyK: Keyboard('k', 0, 0); break;
	case KeyL: Keyboard('l', 0, 0); break;
	case Keyt: Keyboard(',', 0, 0); break;
	case Keyy: Keyboard('.', 0, 0); break;
	case Keyn: Keyboard('n', 0, 0); break;
	case Keym: Keyboard('m', 0, 0); break;
	case KeyX: Keyboard('x', 0, 0); break;
	case keyz: Keyboard('z', 0, 0); break;

	default:
		int menu_color = glutCreateMenu(Menu);
		glutAddMenuEntry("Компонента R (r)", KeyR);
		glutAddMenuEntry("Компонента G (g)", KeyG);
		glutAddMenuEntry("Компонента B (b)", KeyB);

		int menu_move = glutCreateMenu(Menu);
		glutAddMenuEntry("Вверх (w)", KeyW);
		glutAddMenuEntry("Вниз (s)", KeyS);
		glutAddMenuEntry("Bлево (a)", KeyA);
		glutAddMenuEntry("Вправо (d)", KeyD);
		glutAddMenuEntry("Поворот вправо (l)", KeyL);
		glutAddMenuEntry("Поворот влево (k)", KeyK);
		glutAddMenuEntry("Сброс угла поворота (x)", KeyX);
		glutAddMenuEntry("Сброс перемещения группы (z)", keyz);

		int menu_group = glutCreateMenu(Menu);
		glutAddMenuEntry("Завершить группу (p)", KeyP);
		glutAddMenuEntry("Удалить выбранную группу (o)", KeyO);

		int menu_choise = glutCreateMenu(Menu);
		glutAddMenuEntry("Перейти к предыдущему примитиву (,)", Keyt);
		glutAddMenuEntry("Перейти к следующему примитиву (.)", Keyy);
		glutAddMenuEntry("Перейти к следующей группе (m)", Keym);
		glutAddMenuEntry("Перейти к предыдущей группе (n)", Keyn);

		int menu = glutCreateMenu(Menu);
		glutAddSubMenu("Смена цвета", menu_color);
		glutAddSubMenu("Перемещение", menu_move);
		glutAddSubMenu("Группы", menu_group);
		glutAddSubMenu("Выбор", menu_choise);

		glutAttachMenu(GLUT_RIGHT_BUTTON);
		Keyboard(Empty, 0, 0);
	}
}

/* Головная программа */
void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(Width, Height);
	glutCreateWindow("Рисуем треугольники");
	Menu(Empty);
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMainLoop();
}
/********************************************************************
 *                                                                  *
 * Project Type  : Win32 Console Application                        *
 * Project Name  : LabHuman                                         *
 * Programmer(s) : Мурчиков М.Ю.                                    *
 * Created       : 28.03.2020                                       *
 * Last Revision : 06.05.2020                                       *
 * Comment(s)    : Using builds tools for MCVS 2015                 *
 *                 (Platform Toolset = 'v140')                      *
 *                 Target platform version 8.1                      *
 *                                                                  *
 *******************************************************************/

// freeglut - это свободное программное обеспечение с открытым исходным кодом,
// альтернативное библиотеке OpenGL Utility Toolkit (GLUT).
// Больше информации: http://freeglut.sourceforge.net/
#include <GL/freeglut.h>

#include <iostream>
#include <cmath>
#include <locale>

// Заголовочный файл с определением фигулы "человечек"
#include "human.hpp"

// Определение константы числа "Пи" для случая, если используемая вами версия
// cmath её не определяет
#ifndef M_PI
#define M_PI 3.141592653589793
#endif  // !M_PI

// Определение как часто будут обновлятся кадры в окне
#define FPS 60

// Определение длины и ширины окна для отрисовки фигур
#define WINDOW_WIDTH  900
#define WINDOW_HEIGHT 700

// Определение координат для границ окна
#define LEFT_BORDER    -WINDOW_WIDTH  / 2
#define RIGHT_BORDER    WINDOW_WIDTH  / 2
#define TOP_BORDER      WINDOW_HEIGHT / 2
#define BOTTOM_BORDER  -WINDOW_HEIGHT / 2

// Массив для отслеживания состояния каждой кнопки
bool keyDown[256];

// Дисплей-функция, отвечающая за рисование в окне
void display();
// Функция вызываемая каждый раз когда окно меняет размер
void reshape(int width, int height);
// Функция вызываемая через определённые промежутки времени
void timer(int init);

// Функции обрабатывающие нажатие клавиш
void handleKeyPress(unsigned char key, int mouse_x, int mouse_y);
void handleKeyUp(unsigned char key, int mouse_x, int mouse_y);
void handleSpecialKeyPress(int key, int mouse_x, int mouse_y);
void handleSpecialKeyUp(int key, int mouse_x, int mouse_y);

int main(int argc, char** argv) {
  // Инициализация
  glutInit(&argc, argv);

  // Устанавливаем режим отображения окна:
  //  * GLUT_RGB - для отображения графической информации используются 3
  //  компоненты
  //    цвета RGB.
  //  * GLUT_SINGLE - вывод в окно осуществляется с использованием 2 буферов.
  //  Обычно используется для вывода анимаций.
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

  // Задание положение создаваемого окна относительно верхнего левого угла
  // экрана
  glutInitWindowPosition(80, 80);
  // Задание размеров окна
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutCreateWindow("Display window");

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutTimerFunc(1000, timer, 0);

  glutKeyboardFunc(handleKeyPress);
  glutKeyboardUpFunc(handleKeyUp);
  glutSpecialFunc(handleSpecialKeyPress);
  glutSpecialUpFunc(handleSpecialKeyUp);

  // Очистка окна и его заливка в чёрный цвет
  glClearColor(0.0, 0.0, 0.0, 1.0);

  setlocale(LC_ALL, "RUS");

  glutMainLoop();

  return 0;
}

// ========================================================================= //

// Дисплей-функция, отвечающая за рисование в окне
void display() {
  // Очищаем кадр
  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();

  // Задание зоны внутри окна
  static Line RightBorder(400, -300, 400, 300);
  static Line LeftBorder(-400, -300, -400, 300);
  static Line BottomBorder(-400, -300, 400, -300);
  static Line TopBorder(-400, 300, 400, 300);

  // Неккоторый склон внутри активной зоны
  static Line Slope(-400, -200, 200, -300);

  // Шар внутри активной зоны
  static Circle ball(-200, 0, 100);

  // Создаём объект класса человечек
  static Human human(100, 0);
  // Создаём указатель на родительский класс и кладём туда
  // объект класса человечек
  static Point* pHuman = &human;

  // Создаём указатель на интерфейсный класс и кладём туда
  // объект класса человечек
  static Entity* pHumanEntity = &human;

  // Задание шага перемещения по экрану
  static int step = 5;

  // Обработка кодов нажатых клавиш
  static bool movingUP = false;
  static bool movingDOWN = false;
  static bool movingLEFT = false;
  static bool movingRIGHT = false;

  // Определяем куда должен двигатся человечек в зависимости от нажатых клавиш
  if (keyDown[56] || keyDown[119] || keyDown[246]) {
    movingUP = true;
  } else {
    movingUP = false;
  }
  if (keyDown[50] || keyDown[115] || keyDown[251]) {
    movingDOWN = true;
  } else {
    movingDOWN = false;
  }
  if (keyDown[52] || keyDown[97] || keyDown[244]) {
    movingLEFT = true;
  } else {
    movingLEFT = false;
  }
  if (keyDown[54] || keyDown[100] || keyDown[226]) {
    movingRIGHT = true;
  } else {
    movingRIGHT = false;
  }

  // *** Проверяем пересечение человечка с объектами на экране  *** //
  // *** через указатель на интерфейсный класс                  *** //
  if (pHumanEntity->intersects(ball)) {
    ball.Show(1.0f, 1.0, 0.0);  // Yellow
  } else {
    ball.Show();
  }
  if (pHumanEntity->intersects(Slope)) {
    Slope.Show(0.0f, 1.0, 0.0);  // Green
    pHuman->MoveTo(pHuman->GetX(), pHuman->GetY() + step);
  } else {
    Slope.Show();
  }

  if (pHumanEntity->intersects(TopBorder)) {
    movingUP = false;
    pHuman->MoveTo(pHuman->GetX(), pHuman->GetY() - step);
    TopBorder.Show(1.0f, 0.0, 0.0);  // Red
  } else {
    TopBorder.Show();
  }
  if (pHumanEntity->intersects(BottomBorder)) {
    movingDOWN = false;
    pHuman->MoveTo(pHuman->GetX(), pHuman->GetY() + step);
    BottomBorder.Show(1.0f, 0.0, 0.0);  // Red
  } else {
    BottomBorder.Show();
  }
  if (pHumanEntity->intersects(LeftBorder)) {
    movingLEFT = false;
    pHuman->MoveTo(pHuman->GetX() + step, pHuman->GetY());
    LeftBorder.Show(1.0f, 0.0, 0.0);  // Red
  } else {
    LeftBorder.Show();
  }
  if (pHumanEntity->intersects(RightBorder)) {
    movingRIGHT = false;
    pHuman->MoveTo(pHuman->GetX() - step, pHuman->GetY());
    RightBorder.Show(1.0f, 0.0, 0.0);  // Red
  } else {
    RightBorder.Show();
  }

  // *** Двигаем человечка с помощью указателя на родительский класс *** //
  // Движение вверх
  if (movingUP) {
    // std::cout << "circle movingUP" << std::endl;  // Эхо-печать
    pHuman->MoveTo(pHuman->GetX(), pHuman->GetY() + step);
  }
  // Движение вниз
  if (movingDOWN) {
    // std::cout << "circle movingDOWN" << std::endl;  // Эхо-печать
    pHuman->MoveTo(pHuman->GetX(), pHuman->GetY() - step);
  }
  // Движение влево
  if (movingLEFT) {
    // std::cout << "circle movingLEFT" << std::endl;  // Эхо-печать
    pHuman->MoveTo(pHuman->GetX() - step, pHuman->GetY());
  }
  // Движение вправо
  if (movingRIGHT) {
    // std::cout << "circle movingRIGHT" << std::endl;  // Эхо-печать
    pHuman->MoveTo(pHuman->GetX() + step, pHuman->GetY());
  }

  // Отрисовываем человечка
  pHuman->Show();

  // Выводим очередной кадр на экран
  glutSwapBuffers();
}

// Функция вызываемая каждый раз когда окно меняет размер
void reshape(int width, int height) {
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(LEFT_BORDER, RIGHT_BORDER, BOTTOM_BORDER, TOP_BORDER);
  glMatrixMode(GL_MODELVIEW);
}

// Функция вызываемая через определённые промежутки времени
void timer(int init) {
  glutPostRedisplay();
  glutTimerFunc(1000 / FPS, timer, 0);
}

// ================= Функции обрабатывающие нажатие клавиш ================= //

void handleKeyPress(unsigned char key, int mouse_x, int mouse_y) {
  // std::cout << key << " = " << (int)key << " is pressed" << std::endl;
  keyDown[key] = true;
}

void handleKeyUp(unsigned char key, int mouse_x, int mouse_y) {
  // std::cout << key << " = " << (int)key << " is released" << std::endl;
  keyDown[key] = false;
}

void handleSpecialKeyPress(int key, int mouse_x, int mouse_y) {
  if (key == GLUT_KEY_UP) {
    // std::cout << "UP_ARROW is pressed" << std::endl;
    keyDown[119] = true;
  }
  if (key == GLUT_KEY_DOWN) {
    // std::cout << "DOWN_ARROW is pressed" << std::endl;
    keyDown[115] = true;
  }
  if (key == GLUT_KEY_LEFT) {
    // std::cout << "LEFT_ARROW is pressed" << std::endl;
    keyDown[97] = true;
  }
  if (key == GLUT_KEY_RIGHT) {
    // std::cout << "RIGHT_ARROW is pressed" << std::endl;
    keyDown[100] = true;
  }
}

void handleSpecialKeyUp(int key, int mouse_x, int mouse_y) {
  if (key == GLUT_KEY_UP) {
    // std::cout << "UP_ARROW is released" << std::endl;
    keyDown[119] = false;
  }
  if (key == GLUT_KEY_DOWN) {
    // std::cout << "DOWN_ARROW is released" << std::endl;
    keyDown[115] = false;
  }
  if (key == GLUT_KEY_LEFT) {
    // std::cout << "LEFT_ARROW is released" << std::endl;
    keyDown[97] = false;
  }
  if (key == GLUT_KEY_RIGHT) {
    // std::cout << "RIGHT_ARROW is released" << std::endl;
    keyDown[100] = false;
  }
}

// ========================================================================= //

/********************************************************************
 *                                                                  *
 * Project Type  : Win32 Console Application                        *
 * Project Name  : LabHuman                                         *
 * Programmer(s) : Мурчиков М.Ю.                                    *
 * Created       : 28.03.2020                                       *
 * Last Revision : 22.05.2020                                       *
 * Comment(s)    : Using builds tools for MCVS 2015                 *
 *                 (Platform Toolset = 'v140')                      *
 *                 Target platform version 8.1                      *
 *                                                                  *
 *******************************************************************/

// freeglut - это свободное программное обеспечение с открытым исходным кодом,
// альтернативное библиотеке OpenGL Utility Toolkit (GLUT).
// Больше информации: http://freeglut.sourceforge.net/
#include <GL/freeglut.h>

#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <locale>
#include <vector>

// Заголовочный файл с определением фигулы "человечек"
#include "factory.hpp"
#include "human.hpp"

// Определение константы числа "Пи" для случая, если используемая вами версия
// cmath её не определяет
#ifndef M_PI
#define M_PI 3.141592653589793
#endif  // !M_PI

// Определение как часто будут обновлятся кадры в окне
#define FPS 60

// Определение длины и ширины окна для отрисовки фигур
#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 720

// Определение координат для границ окна
#define LEFT_BORDER   -WINDOW_WIDTH  / 2 // + 50
#define RIGHT_BORDER   WINDOW_WIDTH  / 2 // - 50
#define TOP_BORDER     WINDOW_HEIGHT / 2 // - 50
#define BOTTOM_BORDER -WINDOW_HEIGHT / 2 // + 50

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

  // Задание дисплей функции
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutTimerFunc(1000, timer, 0);

  // Задание callback функций для обработки нажатий клавиш 
  glutKeyboardFunc(handleKeyPress);
  glutKeyboardUpFunc(handleKeyUp);
  glutSpecialFunc(handleSpecialKeyPress);
  glutSpecialUpFunc(handleSpecialKeyUp);

  // Очистка окна и его заливка в чёрный цвет
  glClearColor(0.0, 0.0, 0.0, 1.0);

  setlocale(LC_ALL, "RUS");
  std::srand(unsigned(std::time(0)));

  glutMainLoop();

  return 0;
}

// ========================================================================= //

// ============ Дисплей-функция, отвечающая за рисование в окне ============ //

void display() {

  // Задание зоны внутри окна
  static Line RightBorder( RIGHT_BORDER, TOP_BORDER,
                           RIGHT_BORDER, BOTTOM_BORDER);
  static Line LeftBorder(  LEFT_BORDER,  TOP_BORDER, 
                           LEFT_BORDER,  BOTTOM_BORDER);
  static Line BottomBorder(LEFT_BORDER,  BOTTOM_BORDER,
                           RIGHT_BORDER, BOTTOM_BORDER);
  static Line TopBorder(   LEFT_BORDER,  TOP_BORDER,
                           RIGHT_BORDER, TOP_BORDER);
  
  // Вектор границ игровой зоны
  static std::vector<Line*> borders = {&RightBorder,  &LeftBorder,
                                       &BottomBorder, &TopBorder};

  // Пораждающие классы
  static BigBonusFactory*     bigBonusFactory     = new BigBonusFactory;
  static AvarageBonusFactory* avarageBonusFactory = new AvarageBonusFactory;
  static SmallBonusFactory*   smallBonusFactory   = new SmallBonusFactory;
  static KillingBonusFactory* killingBonusFactory = new KillingBonusFactory;

  // Вектор шариков-бонусов
  static std::vector<Bonus*> bonuses;

  // Кол-во шариков бонусов на экране
  static unsigned int BonusesNumber = 5;

  // Фигура которой отображается игрок
  static Human human(100, 0);

  static Circle SafeZone(RIGHT_BORDER, BOTTOM_BORDER, 300);

  static int SafeZoneTimer = 0;

  // Задание шага (в пикселях) перемещения по экрану
  static int step = 5;

  // Счёт
  static int score = 0;
  static int SavedScore = 0;

  // Обработка кодов нажатых клавиш
  static bool movingUP    = false;
  static bool movingDOWN  = false;
  static bool movingLEFT  = false;
  static bool movingRIGHT = false;

  // Очищаем кадр
  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();  

  // Если шариков-бонусов меньше заданного кол-ва, то создаём новый шарик
  if (bonuses.size() < BonusesNumber) {
    // Случайно выбираем один из четырёх видов бонусов
    unsigned int random = std::rand() % (BonusesNumber - 1);
    if (random == 0) {
      bonuses.push_back(bigBonusFactory->createBonus());
      bonuses[bonuses.size() - 1]->MoveTo(
          std::rand() % (WINDOW_WIDTH - 100) - (WINDOW_WIDTH - 100) / 2,
          std::rand() % (WINDOW_HEIGHT - 100) - (WINDOW_HEIGHT - 100) / 2);
    } else if (random == 1) {
      bonuses.push_back(avarageBonusFactory->createBonus());
      bonuses[bonuses.size() - 1]->MoveTo(
          std::rand() % (WINDOW_WIDTH - 100) - (WINDOW_WIDTH - 100) / 2,
          std::rand() % (WINDOW_HEIGHT - 100) - (WINDOW_HEIGHT - 100) / 2);
    } else if (random == 2) {
      bonuses.push_back(smallBonusFactory->createBonus());
      bonuses[bonuses.size() - 1]->MoveTo(
          std::rand() % (WINDOW_WIDTH - 100) - (WINDOW_WIDTH - 100) / 2,
          std::rand() % (WINDOW_HEIGHT - 100) - (WINDOW_HEIGHT - 100) / 2);
    } else if (random == 3) {
      bonuses.push_back(killingBonusFactory->createBonus());
      bonuses[bonuses.size() - 1]->MoveTo(
          std::rand() % (WINDOW_WIDTH - 100) - (WINDOW_WIDTH - 100) / 2,
          std::rand() % (WINDOW_HEIGHT - 100) - (WINDOW_HEIGHT - 100) / 2);
    }
    // Если новый созданный шарик-бонус пересекает человечка, границы игровой
    // зоны или какой либо из уже созданных бонусов, то удалить его и создать
    // заного
    if (human.intersects(*bonuses[bonuses.size() - 1])) {
      delete bonuses[bonuses.size() - 1];
      bonuses.erase(bonuses.begin() + bonuses.size() - 1);
    } else if (bonuses[bonuses.size() - 1]->intersects(SafeZone)) {
      delete bonuses[bonuses.size() - 1];
      bonuses.erase(bonuses.begin() + bonuses.size() - 1);
    } else if(borders[0]->intersects(*bonuses[bonuses.size() - 1])) {
      delete bonuses[bonuses.size() - 1];
      bonuses.erase(bonuses.begin() + bonuses.size() - 1);
    } else if (borders[1]->intersects(*bonuses[bonuses.size() - 1])) {
      delete bonuses[bonuses.size() - 1];
      bonuses.erase(bonuses.begin() + bonuses.size() - 1);
    } else if (borders[2]->intersects(*bonuses[bonuses.size() - 1])) {
      delete bonuses[bonuses.size() - 1];
      bonuses.erase(bonuses.begin() + bonuses.size() - 1);
    } else if (borders[3]->intersects(*bonuses[bonuses.size() - 1])) {
      delete bonuses[bonuses.size() - 1];
      bonuses.erase(bonuses.begin() + bonuses.size() - 1);
    } else {
      for (int i = 0; i < bonuses.size() - 1; i++) {
        if (bonuses[i]->intersects(*bonuses[bonuses.size() - 1])) {
          delete bonuses[bonuses.size() - 1];
          bonuses.erase(bonuses.begin() + bonuses.size() - 1);
        }
      }
    }
  }

  // Определяем куда двигать человечка в зависимости от нажатых клавиш
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

  // Двигаем человечка
  // Движение вверх
  if (movingUP) {
    // std::cout << "circle movingUP" << std::endl;  // Эхо-печать
    human.MoveTo(human.GetX(), human.GetY() + step);
  }
  // Движение вниз
  if (movingDOWN) {
    // std::cout << "circle movingDOWN" << std::endl;  // Эхо-печать
    human.MoveTo(human.GetX(), human.GetY() - step);
  }
  // Движение влево
  if (movingLEFT) {
    // std::cout << "circle movingLEFT" << std::endl;  // Эхо-печать
    human.MoveTo(human.GetX() - step, human.GetY());
  }
  // Движение вправо
  if (movingRIGHT) {
    // std::cout << "circle movingRIGHT" << std::endl;  // Эхо-печать
    human.MoveTo(human.GetX() + step, human.GetY());
  }

  // Проверка пересечения человечка с границами игровой зоны
  if (human.intersects(TopBorder)) {
    movingUP = false;
    human.MoveTo(human.GetX(), human.GetY() - step);
    TopBorder.Show(1.0f, 0.0, 0.0);  // Red
  } else {
    TopBorder.Show();
  }
  if (human.intersects(BottomBorder)) {
    movingDOWN = false;
    human.MoveTo(human.GetX(), human.GetY() + step);
    BottomBorder.Show(1.0f, 0.0, 0.0);  // Red
  } else {
    BottomBorder.Show();
  }
  if (human.intersects(LeftBorder)) {
    movingLEFT = false;
    human.MoveTo(human.GetX() + step, human.GetY());
    LeftBorder.Show(1.0f, 0.0, 0.0);  // Red
  } else {
    LeftBorder.Show();
  }
  if (human.intersects(RightBorder)) {
    movingRIGHT = false;
    human.MoveTo(human.GetX() - step, human.GetY());
    RightBorder.Show(1.0f, 0.0, 0.0);  // Red
  } else {
    RightBorder.Show();
  }

  for (int i = 0; i < bonuses.size(); i++) {
    // Проверяем пересекается ли человечек с шариками-бонусов
    if (human.intersects(*bonuses[i])) {
      // Если пересекает то увеличиваем счёт и удаляем шарик-бонус
      score += bonuses[i]->GetValue();
      std::cout << "New score = " << score << std::endl;
      delete bonuses[i];
      bonuses.erase(bonuses.begin() + i);
      continue;
    }
    /*if (bonuses[i]->type() == -1) {
      (static_cast<KillingBonus*>(bonuses[i]))->takeDamage();
      if ((static_cast<KillingBonus*>(bonuses[i]))->isDead()) {
        delete bonuses[i];
        bonuses.erase(bonuses.begin() + i);
        continue;
      }
    }*/
  }

  // Отрисовываем шарики
  for (int i = 0; i < bonuses.size(); i++) {
    bonuses[i]->Show();
  }

  if (human.intersects(SafeZone)) {
    SafeZoneTimer += 1;
  } else {
    SafeZoneTimer = 0;  
  }

  if (SafeZoneTimer > 150) {
    for (int i = 0; i < bonuses.size(); i++) {
      delete bonuses[i];
      bonuses.erase(bonuses.begin() + i);
    }
    SavedScore += score;
    std::cout << "Score saved! Saved score: " << SavedScore << std::endl;
    score = 0;
    SafeZoneTimer = 0;
  }

  // Отрисовываем человечка
  human.Show();

  // Рисуем безопасную зону
  SafeZone.Show(0.0f, 1.0f, 0.0f);

  if (score < 0) {
    std::cout << "GAME OVER!!!" << std::endl;
    glBegin(GL_POLYGON);
    glColor4f(0.5f, 0.1f, 0.1f, 0.0f);
    glVertex2i(LEFT_BORDER, TOP_BORDER);
    glVertex2i(RIGHT_BORDER, TOP_BORDER);
    glVertex2i(RIGHT_BORDER, BOTTOM_BORDER);
    glVertex2i(LEFT_BORDER, BOTTOM_BORDER);
    glEnd();
    glutSwapBuffers();

    system("pause");
    glutLeaveMainLoop();
  }
  // Выводим очередной кадр на экран
  glutSwapBuffers();
}


// ========================================================================= //

// Функция вызываемая каждый раз когда окно меняет размер
void reshape(int width, int height) {
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(-WINDOW_WIDTH  / 2, WINDOW_WIDTH  / 2,
             -WINDOW_HEIGHT / 2, WINDOW_HEIGHT / 2);
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
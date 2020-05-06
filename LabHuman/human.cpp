#include "human.hpp"

#include <cmath>
#include <iostream>

#ifndef M_PI
#define M_PI 3.141592653589793
#endif  // !M_PI

/* ========================= Methods of class Human ======================== */

Human::Human(int InitX, int InitY, int InitHeadRadius, int InitBodyLen,
             int InitArmsLen, int InitLegsLen)
    : Point(InitX, InitY) {
  // Инициализируем человечка
  headRadius = InitHeadRadius;

  bodyLen = InitBodyLen;

  armsLen = InitArmsLen;
  armsAngle = (M_PI / 180.0) * 30.0;

  legsLen = InitLegsLen;
  legsAngle = (M_PI / 180.0) * 20.0;

  // Инициализируем все фигуры, задающие человечка
  update();
  std::cout << "    Создан человек " << this << std::endl;
}

Human::~Human() { std::cout << "    Удалён человек " << this << std::endl; }

void Human::Show(float R, float G, float B) {
  head.Show(R, G, B);
  body.Show(R, G, B);
  leftArm.Show(R, G, B);
  rightArm.Show(R, G, B);
  leftLeg.Show(R, G, B);
  rightLeg.Show(R, G, B);
}

void Human::update() {
  // Установка начальной точки и радиуса для головы
  head.SetRadius(headRadius);
  head.SetX(X);
  head.SetY(Y + bodyLen / 2 + headRadius);

  // Установка начальной и конечной точек для тела
  body.SetStart(    X, Y + bodyLen / 2);
  body.SetEnd(      X, Y - bodyLen / 2);

  //Установка начальной и конечной точек для левой руки
  leftArm.SetStart( X, 
                    Y + bodyLen / 2);
  leftArm.SetEnd(   X - (int)(armsLen * sin(armsAngle)),
                    Y + bodyLen / 2 - (int)(armsLen * cos(armsAngle)));

  //Установка начальной и конечной точек для правой руки
  rightArm.SetStart(X, 
                    Y + bodyLen / 2);
  rightArm.SetEnd(  X + (int)(armsLen * sin(armsAngle)),
                    Y + bodyLen / 2 - (int)(armsLen * cos(armsAngle)));

  // Установка начальной и конечной точек для левой ноги
  leftLeg.SetStart( X, 
                    Y - bodyLen / 2);
  leftLeg.SetEnd(   X - (int)(legsLen * sin(legsAngle)),
                    Y - bodyLen / 2 - (int)(legsLen * cos(legsAngle)));

  // Установка начальной и конечной точек для правой ноги
  rightLeg.SetStart(X, 
                    Y - bodyLen / 2);
  rightLeg.SetEnd(  X + (int)(legsLen * sin(legsAngle)),
                    Y - bodyLen / 2 - (int)(legsLen * cos(legsAngle)));
}

void Human::MoveTo(int NewX, int NewY) {
  // Задаём новые координаты
  X = NewX;
  Y = NewY;

  // Обновляем начало и конец каждой фигуры, задающей человечка
  update();
}

void Human::Scale(double Scale) {
  // Задаём новые параметры, задающие человечка
  bodyLen = (int)(bodyLen * Scale);
  armsLen = (int)(armsLen * Scale);
  legsLen = (int)(legsLen * Scale);
  headRadius = (int)(headRadius * Scale);

  // Обновляем начало и конец каждой фигуры
  update();
}

bool Human::intersects(Circle& circle) {
  if (head.intersects(circle) || body.intersects(circle) ||
      leftArm.intersects(circle) || rightArm.intersects(circle) ||
      leftLeg.intersects(circle) || rightLeg.intersects(circle)) {
    return true;
  }
  return false;
}

bool Human::intersects(Line& line) {
  if (head.intersects(line) || body.intersects(line) ||
      leftArm.intersects(line) || rightArm.intersects(line) ||
      leftLeg.intersects(line) || rightLeg.intersects(line)) {
    return true;
  }
  return false;
}

// ======================= END methods of class Human ====================== //

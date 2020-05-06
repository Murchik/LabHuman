#include "shapes.hpp"

#include <GL/freeglut.h>

#include <cmath>
#include <iostream>

#ifndef M_PI
#define M_PI 3.141592653589793
#endif  // !M_PI

/* ======================= Methods of class Location ======================= */

Location::Location(int InitX, int InitY) {
  X = InitX;
  Y = InitY;
  std::cout << "Создана Location " << this << std::endl;
}

Location::~Location() { std::cout << "Удалена Location " << this << std::endl; }

void Location::SetX(int NewX) { X = NewX; }

int Location::GetX() { return X; }

void Location::SetY(int NewY) { Y = NewY; }

int Location::GetY() { return Y; }

// ===================== END methods of class Location ===================== //

/* ======================== Methods of class Point ========================= */

Point::Point(int InitX, int InitY) : Location(InitX, InitY) {
  std::cout << " Создана Точка " << this << std::endl;
}

Point::~Point() { std::cout << " Удалена Точка " << this << std::endl; }

void Point::Show(float R, float G, float B) {
  glBegin(GL_POINTS);  // Начинаем рисовать точки
  glColor3f(R, G, B);  // Задаём цвет
  glVertex2i(X, Y);  // Ставим точку в соотв. координаты
  glEnd();           // Заканчиваем рисовать точку
}

void Point::MoveTo(int NewX, int NewY) {
  X = NewX;
  Y = NewY;
}

// ======================= END methods of class Point ====================== //

/* ========================= Methods of class Line ========================= */

Line::Line(int startX, int startY, int endX, int endY)
    : start(startX, startY), end(endX, endY) {
  std::cout << "   Создана линия " << this << std::endl;
}

Line::~Line() { std::cout << "   Удалена линия " << this << std::endl; }

// Определение новых координат, для точки задающей начало линии
void Line::SetStart(int startX, int startY) {
  start.SetX(startX);
  start.SetY(startY);
}

// Определение новых координат, для точки задающей конец линии
void Line::SetEnd(int endX, int endY) {
  end.SetX(endX);
  end.SetY(endY);
}

int Line::GetStartX() { return start.GetX(); }

int Line::GetStartY() { return start.GetY(); }

int Line::GetEndX() { return end.GetX(); }

int Line::GetEndY() { return end.GetY(); }

void Line::Show(float R, float G, float B) {
  glBegin(GL_LINES);   // Начинаем рисовать линию
  glColor3f(R, G, B);  // Задаём белый цвет
  glVertex2i(start.GetX(), start.GetY());  // Ставим вершину в точку начала
  glVertex2i(end.GetX(), end.GetY());  // Ставим вершину в точку конца
  glEnd();  // Заканчиваем рисовать линию
}

// Функция перемещения линии, рисует линию между новыми заданными координатами
// точек начала и конца
void Line::MoveTo(int NewStartX, int NewStartY, int NewEndX, int NewEndY) {
  start.SetX(NewStartX);
  start.SetY(NewStartY);
  end.SetX(NewEndX);
  end.SetY(NewEndY);
}

bool Line::intersects(Line& line) {
  float p0_x = start.GetX();
  float p0_y = start.GetY();
  float p1_x = end.GetX();
  float p1_y = end.GetY();

  float p2_x = line.GetStartX();
  float p2_y = line.GetStartY();
  float p3_x = line.GetEndX();
  float p3_y = line.GetEndY();

  float s1_x, s1_y, s2_x, s2_y;
  s1_x = p1_x - p0_x;
  s1_y = p1_y - p0_y;
  s2_x = p3_x - p2_x;
  s2_y = p3_y - p2_y;

  float s, t;
  float i_x = 0, i_y = 0;  // Координаты точки пересечения

  s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) /
      (-s2_x * s1_y + s1_x * s2_y);
  t = (s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) /
      (-s2_x * s1_y + s1_x * s2_y);

  if (s >= 0 && s <= 1 && t >= 0 && t <= 1) {
    // Collision detected
    if (i_x != NULL) i_x = p0_x + (t * s1_x);
    if (i_y != NULL) i_y = p0_y + (t * s1_y);
    return true;
  }

  return false;  // No collision
}

bool Line::intersects(Circle& circle) {
  double a1 = sqrt(pow(circle.GetX() - start.GetX(), 2.0) +
                   pow(circle.GetY() - start.GetY(), 2.0));

  double a2 = sqrt(pow(circle.GetX() - end.GetX(), 2.0) +
                   pow(circle.GetY() - end.GetY(), 2.0));

  double len = sqrt(pow(end.GetX() - start.GetX(), 2.0) +
                    pow(end.GetY() - start.GetY(), 2.0));

  double NewStartX = start.GetX() - circle.GetX();
  double NewStartY = start.GetY() - circle.GetY();
  double NewEndX = end.GetX() - circle.GetX();
  double NewEndY = end.GetY() - circle.GetY();

  double A = NewStartY - NewEndY;
  double B = NewEndX - NewStartX;
  double C = (NewStartX * NewEndY) - (NewEndX * NewStartY);

  double pointX = circle.GetX() - (A * C) / (A * A + B * B);
  double pointY = circle.GetY() - (B * C) / (A * A + B * B);

  double d1 =
      sqrt(pow(circle.GetX() - pointX, 2.0) + pow(circle.GetY() - pointY, 2.0));
  double d2 =
      sqrt(pow(end.GetX() - pointX, 2.0) + pow(end.GetY() - pointY, 2.0));
  double d3 =
      sqrt(pow(start.GetX() - pointX, 2.0) + pow(start.GetY() - pointY, 2.0));

  if (a1 < circle.GetRadius() || a2 < circle.GetRadius()) {
    return true;
  } else if (d1 < circle.GetRadius()) {
    if (d2 < len && d3 < len) {
      return true;
    }
  }

  return false;
}

// ======================= END methods of class Line ======================= //

/* ======================== Methods of class Circle ======================== */

Circle::Circle(int InitX, int InitY, int InitRadius) : Point(InitX, InitY) {
  radius = InitRadius;
  std::cout << "   Создан круг " << this << std::endl;
}

Circle::~Circle() { std::cout << "   Удалён круг " << this << std::endl; }

void Circle::SetRadius(int NewRadius) { radius = NewRadius; }

int Circle::GetRadius() { return radius; }

void Circle::Show(float R, float G, float B) {
  int amountSegments = 360;
  int i;
  float angle;
  float dx, dy;

  glBegin(GL_LINE_LOOP);
  glColor3f(R, G, B);

  for (i = 0; i < amountSegments; i++) {
    angle = 2.0 * M_PI * float(i) / float(amountSegments);

    dx = radius * cosf(angle);
    dy = radius * sinf(angle);

    glVertex2f(GetX() + dx, GetY() + dy);
  }

  glEnd();
}

bool Circle::intersects(Circle& other) {
  // if (sqrt((x2 - x1)^2 + (y2 - y1)^2) <= R1 + R2) then true else false;
  if (pow(other.GetX() - GetX(), 2.0) + pow(other.GetY() - GetY(), 2.0) <=
      pow(GetRadius() + other.GetRadius(), 2.0)) {
    return true;
  }
  return false;
}

bool Circle::intersects(Line& line) {
  double a1 =
      sqrt(pow(X - line.GetStartX(), 2.0) + pow(Y - line.GetStartY(), 2.0));

  double a2 = sqrt(pow(X - line.GetEndX(), 2.0) + pow(Y - line.GetEndY(), 2.0));

  double len = sqrt(pow(line.GetEndX() - line.GetStartX(), 2.0) +
                    pow(line.GetEndY() - line.GetStartY(), 2.0));

  double NewStartX = line.GetStartX() - X;
  double NewStartY = line.GetStartY() - Y;
  double NewEndX = line.GetEndX() - X;
  double NewEndY = line.GetEndY() - Y;

  double A = NewStartY - NewEndY;
  double B = NewEndX - NewStartX;
  double C = (NewStartX * NewEndY) - (NewEndX * NewStartY);

  double pointX = X - (A * C) / (A * A + B * B);
  double pointY = Y - (B * C) / (A * A + B * B);

  double d1 = sqrt(pow(X - pointX, 2.0) + pow(Y - pointY, 2.0));
  double d2 = sqrt(pow(line.GetEndX() - pointX, 2.0) +
                   pow(line.GetEndY() - pointY, 2.0));
  double d3 = sqrt(pow(line.GetStartX() - pointX, 2.0) +
                   pow(line.GetStartY() - pointY, 2.0));

  if (a1 < radius || a2 < radius) {
    return true;
  } else if (d1 < radius) {
    if (d2 < len && d3 < len) {
      return true;
    }
  }

  return false;
}

// ====================== END methods of class Circle ====================== //

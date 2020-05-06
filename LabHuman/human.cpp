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
  // �������������� ���������
  headRadius = InitHeadRadius;

  bodyLen = InitBodyLen;

  armsLen = InitArmsLen;
  armsAngle = (M_PI / 180.0) * 30.0;

  legsLen = InitLegsLen;
  legsAngle = (M_PI / 180.0) * 20.0;

  // �������������� ��� ������, �������� ���������
  update();
  std::cout << "    ������ ������� " << this << std::endl;
}

Human::~Human() { std::cout << "    ����� ������� " << this << std::endl; }

void Human::Show(float R, float G, float B) {
  head.Show(R, G, B);
  body.Show(R, G, B);
  leftArm.Show(R, G, B);
  rightArm.Show(R, G, B);
  leftLeg.Show(R, G, B);
  rightLeg.Show(R, G, B);
}

void Human::update() {
  // ��������� ��������� ����� � ������� ��� ������
  head.SetRadius(headRadius);
  head.SetX(X);
  head.SetY(Y + bodyLen / 2 + headRadius);

  // ��������� ��������� � �������� ����� ��� ����
  body.SetStart(    X, Y + bodyLen / 2);
  body.SetEnd(      X, Y - bodyLen / 2);

  //��������� ��������� � �������� ����� ��� ����� ����
  leftArm.SetStart( X, 
                    Y + bodyLen / 2);
  leftArm.SetEnd(   X - (int)(armsLen * sin(armsAngle)),
                    Y + bodyLen / 2 - (int)(armsLen * cos(armsAngle)));

  //��������� ��������� � �������� ����� ��� ������ ����
  rightArm.SetStart(X, 
                    Y + bodyLen / 2);
  rightArm.SetEnd(  X + (int)(armsLen * sin(armsAngle)),
                    Y + bodyLen / 2 - (int)(armsLen * cos(armsAngle)));

  // ��������� ��������� � �������� ����� ��� ����� ����
  leftLeg.SetStart( X, 
                    Y - bodyLen / 2);
  leftLeg.SetEnd(   X - (int)(legsLen * sin(legsAngle)),
                    Y - bodyLen / 2 - (int)(legsLen * cos(legsAngle)));

  // ��������� ��������� � �������� ����� ��� ������ ����
  rightLeg.SetStart(X, 
                    Y - bodyLen / 2);
  rightLeg.SetEnd(  X + (int)(legsLen * sin(legsAngle)),
                    Y - bodyLen / 2 - (int)(legsLen * cos(legsAngle)));
}

void Human::MoveTo(int NewX, int NewY) {
  // ����� ����� ����������
  X = NewX;
  Y = NewY;

  // ��������� ������ � ����� ������ ������, �������� ���������
  update();
}

void Human::Scale(double Scale) {
  // ����� ����� ���������, �������� ���������
  bodyLen = (int)(bodyLen * Scale);
  armsLen = (int)(armsLen * Scale);
  legsLen = (int)(legsLen * Scale);
  headRadius = (int)(headRadius * Scale);

  // ��������� ������ � ����� ������ ������
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

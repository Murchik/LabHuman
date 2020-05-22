#ifndef HUMAN_HPP
#define HUMAN_HPP

// ����������� ������������� ����� � ���������������� �������� ��� ���������
// ���������
#ifndef SHAPES_HPP
#include "shapes.hpp"
#endif

class Human : public Point {
 private:
  // ��������� ��������� ��������:
  float armsAngle;  // ���� ��� ������� ���� ��������� �� ��������� � ����
  float legsAngle;  // ���� ��� ������� ���� ��������� �� ��������� � ����
  // ����������� �����, �������� ���������
  Line body;
  Circle head;
  Line leftArm;
  Line rightArm;
  Line leftLeg;
  Line rightLeg;

  // ����� �������� ��������
  Point origin;

 protected:
  int headRadius;  // ������ ��� ������

  int bodyLen;  // ����� ����

  int armsLen;       // ����� ���

  int legsLen;       // ����� ���

 public:
  // ������ ��������� � ������� � �������� �����������, � ��������� ������ ���
  // � ������ ���
  Human(int InitX = 0, int InitY = 0, int headRadius = 20, int InitBodyLen = 60,
        int InitArmsLen = 50, int InitLegsLen = 70);
  virtual ~Human();

  void Show(float R = 1.0f, float G = 1.0f, float B = 1.0f) override;
  void MoveTo(int NewX, int NewY) override;

  // ������� ���������� ������ ���������
  void Scale(double Scale);

  // ������� ����������� ������, ����� �/��� ����� ������ ������, ��������
  // ��������� � ����������� �� ��� ����������
  void update();

  // ������ ������������ ������������ �� ��������� � ��� ����
  bool intersects(Circle &circle);
  bool intersects(Line &line);
};

#endif  // !HUMAN_HPP

#ifndef HUMAN_HPP
#define HUMAN_HPP

// Подключение заголовочного файла с вспомогательными фигурами для рисования
// человечка
#ifndef SHAPES_HPP
#include "shapes.hpp"
#endif

class Human : public Point {
 private:
  // Параметры человечка задающие:
  float armsAngle;  // Угол под которым руки находятся по отношению к телу
  float legsAngle;  // Угол под которым ноги находятся по отношению к телу
  // Определение фигур, задающих человечка
  Line body;
  Circle head;
  Line leftArm;
  Line rightArm;
  Line leftLeg;
  Line rightLeg;

  // Точка создания человека
  Point origin;

 protected:
  int headRadius;  // Размер его головы

  int bodyLen;  // Длину тела

  int armsLen;       // Длину рук

  int legsLen;       // Длину ног

 public:
  // Создаёт человечка с центром в заданных координатах, с заданными длиной рук
  // и длиной ног
  Human(int InitX = 0, int InitY = 0, int headRadius = 20, int InitBodyLen = 60,
        int InitArmsLen = 50, int InitLegsLen = 70);
  virtual ~Human();

  void Show(float R = 1.0f, float G = 1.0f, float B = 1.0f) override;
  void MoveTo(int NewX, int NewY) override;

  // Функция изменяющая размер человечка
  void Scale(double Scale);

  // Функция обновляющая начало, конец и/или центр каждой фигуры, задающей
  // человечка в зависимости от его параметров
  void update();

  // Методы определяющие пересекается ли человечек с чем либо
  bool intersects(Circle &circle);
  bool intersects(Line &line);
};

#endif  // !HUMAN_HPP

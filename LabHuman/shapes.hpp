#ifndef SHAPES_HPP
#define SHAPES_HPP

class Location;
class Point;
class Line;
class Circle;

class Location {
 protected:
  int X;
  int Y;

 public:
  Location(int InitX = 0, int InitY = 0);
  ~Location();

  void SetX(int NewX);
  void SetY(int NewY);
  int GetX();
  int GetY();

};  // ====================== End of class Location ======================== //

class Entity {
 public:
  virtual bool intersects(Line &line) = 0;
  virtual bool intersects(Circle &circle) = 0;
  virtual ~Entity() = default;

};  // ======================== END of class Entity ========================= //

class Point : public Location {
 public:
  Point(int InitX = 0, int InitY = 0);
  ~Point();

  virtual void Show(float R = 1.0f, float G = 1.0f, float B = 1.0f);

  virtual void MoveTo(int NewX, int NewY);

};  // ======================== End of class Point ========================= //

class Line : public Entity {
 protected:
  Point start;
  Point end;

 public:
  Line(int startX = 0, int startY = 0, int endX = 0, int endY = 0);
  virtual ~Line();

  void SetStart(int startX, int startY);
  void SetEnd(int endX, int endY);
  int GetStartX();
  int GetStartY();
  int GetEndX();
  int GetEndY();

  void Show(float R = 1.0f, float G = 1.0f, float B = 1.0f);
  void MoveTo(int NewStartX, int NewStartY, int NewEndX, int NewEndY);

  bool intersects(Line &line);
  bool intersects(Circle &circle);

};  // ======================== End of class Line ========================== //

class Circle : public Point, public Entity {
 protected:
  int radius;

 public:
  Circle(int InitX = 0, int InitY = 0, int InitRadius = 20);
  virtual ~Circle();

  void SetRadius(int NewRadius);
  int GetRadius();

  void Show(float R = 1.0f, float G = 1.0f, float B = 1.0f) override;

  bool intersects(Line &line);
  bool intersects(Circle &circle);

};  // ======================= End of class Circle ========================= //

#endif  // !SHAPES_HPP

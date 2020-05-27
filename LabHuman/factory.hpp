#ifndef FACTORY_HPP
#define FACTORY_HPP

#ifndef SHAPES_HPP
#include "shapes.hpp"
#endif

class Bonus : public Circle {
 public:
  virtual int GetValue() = 0;
  virtual int type() = 0;
  virtual void Show(float R = 1.0f, float G = 1.0f, float B = 1.0f) = 0;
  virtual ~Bonus(){};
};

class BigBonus : public Bonus {
 public:
  BigBonus();
  ~BigBonus();
  int type() override;
  void Show(float R = 0.8f, float G = 0.0f, float B = 0.8f) override;
  virtual int GetValue();
};

class AvarageBonus : public Bonus {
 public:
  AvarageBonus();
  ~AvarageBonus();
  int type() override;
  void Show(float R = 0.5f, float G = 0.5f, float B = 1.0f) override;
  virtual int GetValue();
};

class SmallBonus : public Bonus {
 public:
  SmallBonus();
  ~SmallBonus();
  int type() override;
  void Show(float R = 0.1f, float G = 0.1f, float B = 1.0f) override;
  virtual int GetValue();
};

class KillingBonus : public Bonus {
 protected:
  int health;

 public:
  KillingBonus();
  ~KillingBonus();
  int type() override;
  void takeDamage();
  bool isDead();
  void Show(float R = 1.0f, float G = 0.0f, float B = 0.0f) override;
  virtual int GetValue();
};

class Factory {
 public:
  virtual Bonus *createBonus() = 0;
  virtual ~Factory(){};
};

class BigBonusFactory : public Factory {
 public:
  virtual Bonus *createBonus();
};

class AvarageBonusFactory : public Factory {
 public:
  virtual Bonus *createBonus();
};

class SmallBonusFactory : public Factory {
 public:
  virtual Bonus *createBonus();
};

class KillingBonusFactory : public Factory {
 public:
  virtual Bonus *createBonus();
};

#endif  // !FACTORY_HPP

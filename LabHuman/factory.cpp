#include "factory.hpp"

#include <GL/freeglut.h>

#include <cmath>

#ifndef M_PI
#define M_PI 3.141592653589793
#endif  // !M_PI

/* ======================= Methods of class BigBonus ======================= */

BigBonus::BigBonus() { radius = 80; }

BigBonus::~BigBonus() {}

int BigBonus::type() { return 3; }

void BigBonus::Show(float R, float G, float B) {
  int amountSegments = 360;
  int i;
  float angle;
  float dx, dy;

  glBegin(GL_POLYGON);
  glColor3f(1.0f, 0.0f, 1.0f);

  for (i = 0; i < amountSegments; i++) {
    angle = 2.0 * M_PI * float(i) / float(amountSegments);

    dx = radius * cosf(angle);
    dy = radius * sinf(angle);

    glVertex2f(GetX() + dx, GetY() + dy);
  }

  glEnd();
}

int BigBonus::GetValue() { return 300; }

// ===================== END methods of class BigBonus ===================== //

/* ===================== Methods of class AvarageBonus ===================== */

AvarageBonus::AvarageBonus() { radius = 50; }

AvarageBonus::~AvarageBonus() {}

int AvarageBonus::type() { return 2; }

void AvarageBonus::Show(float R, float G, float B) {
  int amountSegments = 360;
  int i;
  float angle;
  float dx, dy;

  glBegin(GL_POLYGON);
  glColor3f(0.0f, 1.0f, 1.0f);

  for (i = 0; i < amountSegments; i++) {
    angle = 2.0 * M_PI * float(i) / float(amountSegments);

    dx = radius * cosf(angle);
    dy = radius * sinf(angle);

    glVertex2f(GetX() + dx, GetY() + dy);
  }

  glEnd();
}

int AvarageBonus::GetValue() { return 100; }

// =================== END methods of class AvarageBonus =================== //

/* ====================== Methods of class SmallBonus ====================== */

SmallBonus::SmallBonus() { radius = 30; }

SmallBonus::~SmallBonus() {}

int SmallBonus::type() { return 1; }

void SmallBonus::Show(float R, float G, float B) {
  int amountSegments = 360;
  int i;
  float angle;
  float dx, dy;

  glBegin(GL_POLYGON);
  glColor3f(0.0f, 1.0f, 0.0f);

  for (i = 0; i < amountSegments; i++) {
    angle = 2.0 * M_PI * float(i) / float(amountSegments);

    dx = radius * cosf(angle);
    dy = radius * sinf(angle);

    glVertex2f(GetX() + dx, GetY() + dy);
  }

  glEnd();
}

int SmallBonus::GetValue() { return 50; }

// =================== END methods of class SmallBonus ===================== //

/* ==================== Methods of class KillingBonus ====================== */

KillingBonus::KillingBonus() {
  radius = 60;
  health = 310;
}

KillingBonus::~KillingBonus() {}

int KillingBonus::type() { return -1; }

void KillingBonus::takeDamage() { health -= 1; }

bool KillingBonus::isDead() {
  if (health < 0) {
    return true;
  }
  return false;
}

void KillingBonus::Show(float R, float G, float B) {
  int amountSegments = 360;
  int i;
  float angle;
  float dx, dy;

  glBegin(GL_POLYGON);
  glColor3f(1.0f, 0.0f, 0.0f);

  for (i = 0; i < amountSegments; i++) {
    angle = 2.0 * M_PI * float(i) / float(amountSegments);

    dx = radius * cosf(angle);
    dy = radius * sinf(angle);

    glVertex2f(GetX() + dx, GetY() + dy);
  }

  glEnd();
}

int KillingBonus::GetValue() { return -500; }

// =================== END methods of class KillingBonus =================== //

/* ========================================================================= */

Bonus* BigBonusFactory::createBonus() { return new BigBonus; }

Bonus* AvarageBonusFactory::createBonus() { return new AvarageBonus; }

Bonus* SmallBonusFactory::createBonus() { return new SmallBonus; }

Bonus* KillingBonusFactory::createBonus() { return new KillingBonus; }

// ========================================================================= //

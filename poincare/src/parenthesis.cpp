extern "C" {
#include <assert.h>
#include <stdlib.h>
}
#include <poincare/parenthesis.h>
#include "layout/parenthesis_layout.h"

Parenthesis::Parenthesis(Expression * operand, bool cloneOperands) {
  assert(operand != nullptr);
  if (cloneOperands) {
    m_operand = operand->clone();
  } else {
    m_operand = operand;
  }
}

Parenthesis::~Parenthesis() {
  delete m_operand;
}

int Parenthesis::numberOfOperands() const {
  return 1;
}

const Expression * Parenthesis::operand(int i) const {
  assert(i == 0);
  return m_operand;
}

Expression * Parenthesis::clone() const {
  return this->cloneWithDifferentOperands((Expression**) &m_operand, 1, true);
}

ExpressionLayout * Parenthesis::createLayout(DisplayMode displayMode) const {
  return new ParenthesisLayout(m_operand->createLayout(displayMode));
}

float Parenthesis::approximate(Context& context, AngleUnit angleUnit) const {
  return m_operand->approximate(context, angleUnit);
}

Expression * Parenthesis::evaluate(Context& context, AngleUnit angleUnit) const {
  return m_operand->evaluate(context, angleUnit);
}

Expression::Type Parenthesis::type() const {
  return Type::Parenthesis;
}

Expression * Parenthesis::cloneWithDifferentOperands(Expression** newOperands,
    int numberOfOperands, bool cloneOperands) const {
  assert(numberOfOperands == 1);
  assert(newOperands != nullptr);
  return new Parenthesis(newOperands[0], cloneOperands);
}

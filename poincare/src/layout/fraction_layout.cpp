#include "fraction_layout.h"
#include <poincare/expression_layout_cursor.h>
#include <string.h>
#include <assert.h>

namespace Poincare {

FractionLayout::FractionLayout(ExpressionLayout * numerator, ExpressionLayout * denominator, bool cloneOperands) :
  StaticLayoutHierarchy<2>(numerator, denominator, cloneOperands)
{
  m_baseline = numeratorLayout()->size().height()
    + k_fractionLineMargin + k_fractionLineHeight;
}

ExpressionLayout * FractionLayout::clone() const {
  FractionLayout * layout = new FractionLayout(const_cast<FractionLayout *>(this)->numeratorLayout(), const_cast<FractionLayout *>(this)->denominatorLayout(), true);
  return layout;
}

bool FractionLayout::moveLeft(ExpressionLayoutCursor * cursor) {
  // Case: Left of the numerator or the denominator.
  // Go Left of the fraction.
   if (((numeratorLayout() && cursor->pointedExpressionLayout() == numeratorLayout())
        || (denominatorLayout() && cursor->pointedExpressionLayout() == denominatorLayout()))
      && cursor->position() == ExpressionLayoutCursor::Position::Left)
  {
    cursor->setPointedExpressionLayout(this);
    return true;
  }
  assert(cursor->pointedExpressionLayout() == this);
  // Case: Right.
  // Go to the denominator.
  if (cursor->position() == ExpressionLayoutCursor::Position::Right) {
    assert(denominatorLayout() != nullptr);
    cursor->setPointedExpressionLayout(denominatorLayout());
    return true;
  }
  // Case: Left.
  // Ask the parent.
  assert(cursor->position() == ExpressionLayoutCursor::Position::Left);
  if (m_parent) {
    return m_parent->moveLeft(cursor);
  }
  return false;
}

bool FractionLayout::moveRight(ExpressionLayoutCursor * cursor) {
  // Case: Right of the numerator or the denominator.
  // Go Right of the fraction.
   if (((numeratorLayout() && cursor->pointedExpressionLayout() == numeratorLayout())
        || (denominatorLayout() && cursor->pointedExpressionLayout() == denominatorLayout()))
      && cursor->position() == ExpressionLayoutCursor::Position::Right)
  {
    cursor->setPointedExpressionLayout(this);
    return true;
  }
  assert(cursor->pointedExpressionLayout() == this);
  // Case: Left.
  // Go to the numerator.
  if (cursor->position() == ExpressionLayoutCursor::Position::Left) {
    assert(numeratorLayout() != nullptr);
    cursor->setPointedExpressionLayout(numeratorLayout());
    return true;
  }
  // Case: Right.
  // Ask the parent.
  assert(cursor->position() == ExpressionLayoutCursor::Position::Right);
  if (m_parent) {
    return m_parent->moveRight(cursor);
  }
  return false;
}

bool FractionLayout::moveUp(ExpressionLayoutCursor * cursor, ExpressionLayout * previousLayout, ExpressionLayout * previousPreviousLayout) {
  // If the cursor is inside denominator, move it to the numerator.
  if (denominatorLayout() && previousLayout == denominatorLayout()) {
    assert(numeratorLayout() != nullptr);
    return numeratorLayout()->moveUpInside(cursor);
  }
  // If the cursor is Left or Right, move it to the numerator.
  if (cursor->pointedExpressionLayout() == this){
    assert(numeratorLayout() != nullptr);
    return numeratorLayout()->moveUpInside(cursor);
  }
  return ExpressionLayout::moveUp(cursor, previousLayout, previousPreviousLayout);
}

bool FractionLayout::moveDown(ExpressionLayoutCursor * cursor, ExpressionLayout * previousLayout, ExpressionLayout * previousPreviousLayout) {
  // If the cursor is inside numerator, move it to the denominator.
  if (numeratorLayout() && previousLayout == numeratorLayout()) {
    assert(denominatorLayout() != nullptr);
    return denominatorLayout()->moveDownInside(cursor);
  }
  // If the cursor is Left or Right, move it to the denominator.
  if (cursor->pointedExpressionLayout() == this){
    assert(denominatorLayout() != nullptr);
    return denominatorLayout()->moveDownInside(cursor);
  }
  return ExpressionLayout::moveDown(cursor, previousLayout, previousPreviousLayout);
}

void FractionLayout::render(KDContext * ctx, KDPoint p, KDColor expressionColor, KDColor backgroundColor) {
  KDCoordinate fractionLineY = p.y() + numeratorLayout()->size().height() + k_fractionLineMargin;
  ctx->fillRect(KDRect(p.x()+k_fractionBorderMargin, fractionLineY, size().width()-2*k_fractionBorderMargin, 1), expressionColor);
}

KDSize FractionLayout::computeSize() {
  KDCoordinate width = max(numeratorLayout()->size().width(), denominatorLayout()->size().width())
    + 2*k_fractionBorderLength+2*k_fractionBorderMargin;
  KDCoordinate height = numeratorLayout()->size().height()
    + k_fractionLineMargin + k_fractionLineHeight + k_fractionLineMargin
    + denominatorLayout()->size().height();
  return KDSize(width, height);
}

KDPoint FractionLayout::positionOfChild(ExpressionLayout * child) {
  KDCoordinate x = 0;
  KDCoordinate y = 0;
  if (child == numeratorLayout()) {
    x = (KDCoordinate)((size().width() - numeratorLayout()->size().width())/2);
  } else if (child == denominatorLayout()) {
    x = (KDCoordinate)((size().width() - denominatorLayout()->size().width())/2);
    y = (KDCoordinate)(numeratorLayout()->size().height() + 2*k_fractionLineMargin + k_fractionLineHeight);
  } else {
    assert(false); // Should not happen
  }
  return KDPoint(x, y);
}

ExpressionLayout * FractionLayout::numeratorLayout() {
  return editableChild(0);
}

ExpressionLayout * FractionLayout::denominatorLayout() {
  return editableChild(1);
}

}

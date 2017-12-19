#include "nth_root_layout.h"
#include <poincare/expression_layout_cursor.h>
#include <string.h>
#include <assert.h>

namespace Poincare {

const uint8_t radixPixel[NthRootLayout::k_leftRadixHeight][NthRootLayout::k_leftRadixWidth] = {
  {0x00, 0xFF, 0xFF, 0xFF, 0xFF},
  {0xFF, 0x00, 0xFF, 0xFF, 0xFF},
  {0xFF, 0x00, 0xFF, 0xFF, 0xFF},
  {0xFF, 0xFF, 0x00, 0xFF, 0xFF},
  {0xFF, 0xFF, 0x00, 0xFF, 0xFF},
  {0xFF, 0xFF, 0xFF, 0x00, 0xFF},
  {0xFF, 0xFF, 0xFF, 0x00, 0xFF},
  {0xFF, 0xFF, 0xFF, 0xFF, 0x00},
};

NthRootLayout::NthRootLayout(ExpressionLayout * radicand, ExpressionLayout * index, bool cloneOperands) :
  StaticLayoutHierarchy<2>(radicand, index, cloneOperands)
{
  if (indexLayout() != nullptr) {
    m_baseline = max(radicandLayout()->baseline() + k_radixLineThickness + k_heightMargin,
      indexLayout()->size().height() + k_indexHeight);
  } else {
    m_baseline = radicandLayout()->baseline() + k_radixLineThickness + k_heightMargin;
  }
}

ExpressionLayout * NthRootLayout::clone() const {
  NthRootLayout * layout = new NthRootLayout(const_cast<NthRootLayout *>(this)->radicandLayout(), const_cast<NthRootLayout *>(this)->indexLayout(), true);
  return layout;
}

bool NthRootLayout::moveLeft(ExpressionLayoutCursor * cursor) {
  // Case: Left of the radicand.
  // Go the index if there is one, else go Left of the root.
  if (radicandLayout()
    && cursor->pointedExpressionLayout() == radicandLayout()
    && cursor->position() == ExpressionLayoutCursor::Position::Left)
  {
    if (indexLayout()) {
      cursor->setPointedExpressionLayout(indexLayout());
      cursor->setPosition(ExpressionLayoutCursor::Position::Right);
      return true;
    }
    cursor->setPointedExpressionLayout(this);
    return true;
  }
  // Case: Left of the index.
  // Go Left of the root.
  if (indexLayout()
    && cursor->pointedExpressionLayout() == indexLayout()
    && cursor->position() == ExpressionLayoutCursor::Position::Left)
  {
    cursor->setPointedExpressionLayout(this);
    return true;
  }
  assert(cursor->pointedExpressionLayout() == this);
  // Case: Right.
  // Go Right of the radicand.
  if (cursor->position() == ExpressionLayoutCursor::Position::Right) {
    assert(radicandLayout() != nullptr);
    cursor->setPointedExpressionLayout(radicandLayout());
    return true;
  }
  assert(cursor->position() == ExpressionLayoutCursor::Position::Left);
  // Case: Left.
  // Ask the parent.
  if (m_parent) {
    return m_parent->moveLeft(cursor);
  }
  return false;
}

bool NthRootLayout::moveRight(ExpressionLayoutCursor * cursor) {
  // Case: Right of the radicand.
  // Go the Right of the root.
  if (radicandLayout()
    && cursor->pointedExpressionLayout() == radicandLayout()
    && cursor->position() == ExpressionLayoutCursor::Position::Right)
  {
    cursor->setPointedExpressionLayout(this);
    return true;
  }
  // Case: Right of the index.
  // Go Left of the integrand.
  if (indexLayout()
    && cursor->pointedExpressionLayout() == indexLayout()
    && cursor->position() == ExpressionLayoutCursor::Position::Right)
  {
    assert(radicandLayout() != nullptr);
    cursor->setPointedExpressionLayout(radicandLayout());
    cursor->setPosition(ExpressionLayoutCursor::Position::Left);
    return true;
  }
  assert(cursor->pointedExpressionLayout() == this);
  // Case: Left.
  // Go index if there is one, else go to the radicand.
  if (cursor->position() == ExpressionLayoutCursor::Position::Left) {
    if (indexLayout()) {
      cursor->setPointedExpressionLayout(indexLayout());
      return true;
    }
    assert(radicandLayout() != nullptr);
    cursor->setPointedExpressionLayout(radicandLayout());
    return true;
  }
  assert(cursor->position() == ExpressionLayoutCursor::Position::Right);
  // Case: Right.
  // Ask the parent.
  if (m_parent) {
    return m_parent->moveRight(cursor);
  }
  return false;
}

bool NthRootLayout::moveUp(ExpressionLayoutCursor * cursor, ExpressionLayout * previousLayout, ExpressionLayout * previousPreviousLayout) {
  // If the cursor is Left of the radicand, move it to the index.
  if (radicandLayout()
      && previousLayout == radicandLayout()
      && cursor->positionIsEquivalentTo(radicandLayout(), ExpressionLayoutCursor::Position::Left))
  {
    assert(indexLayout() != nullptr);
    cursor->setPointedExpressionLayout(indexLayout());
    cursor->setPosition(ExpressionLayoutCursor::Position::Right);
    cursor->setPositionInside(0);
    return true;
  }
  // If the cursor is Left, move it to the index.
  if (cursor->pointedExpressionLayout() == this
      && cursor->position() == ExpressionLayoutCursor::Position::Left)
  {
    assert(indexLayout() != nullptr);
    cursor->setPointedExpressionLayout(indexLayout());
    cursor->setPosition(ExpressionLayoutCursor::Position::Left);
    cursor->setPositionInside(0);
    return true;
  }
  return ExpressionLayout::moveUp(cursor, previousLayout, previousPreviousLayout);
}

bool NthRootLayout::moveDown(ExpressionLayoutCursor * cursor, ExpressionLayout * previousLayout, ExpressionLayout * previousPreviousLayout) {
  if (indexLayout() && previousLayout == indexLayout()) {
    // If the cursor is Right of the index, move it to the radicand.
    if (cursor->positionIsEquivalentTo(indexLayout(), ExpressionLayoutCursor::Position::Right)) {
      assert(radicandLayout() != nullptr);
      cursor->setPointedExpressionLayout(radicandLayout());
      cursor->setPosition(ExpressionLayoutCursor::Position::Left);
      cursor->setPositionInside(0);
      return true;
    }
    // If the cursor is Left of the index, move it Left .
    if (cursor->positionIsEquivalentTo(indexLayout(), ExpressionLayoutCursor::Position::Left)) {
      cursor->setPointedExpressionLayout(this);
      cursor->setPosition(ExpressionLayoutCursor::Position::Left);
      cursor->setPositionInside(0);
      return true;
    }
  }
  return ExpressionLayout::moveDown(cursor, previousLayout, previousPreviousLayout);
}

void NthRootLayout::render(KDContext * ctx, KDPoint p, KDColor expressionColor, KDColor backgroundColor) {
  KDSize radicandSize = radicandLayout()->size();
  KDSize indexSize = indexLayout() != nullptr ? indexLayout()->size() : KDSize(k_leftRadixWidth,0);
  KDColor workingBuffer[k_leftRadixWidth*k_leftRadixHeight];
  KDRect leftRadixFrame(p.x() + indexSize.width() + k_widthMargin - k_leftRadixWidth,
    p.y() + m_baseline + radicandSize.height() - radicandLayout()->baseline() + k_heightMargin - k_leftRadixHeight,
    k_leftRadixWidth, k_leftRadixHeight);
  ctx->blendRectWithMask(leftRadixFrame, expressionColor, (const uint8_t *)radixPixel, (KDColor *)workingBuffer);
  if (indexSize.height() + k_indexHeight > radicandLayout()->baseline() + k_radixLineThickness + k_heightMargin) {
    ctx->fillRect(KDRect(p.x() + indexSize.width() + k_widthMargin,
                         p.y() + indexSize.height() + k_indexHeight - radicandLayout()->baseline() - k_radixLineThickness - k_heightMargin,
                         k_radixLineThickness,
                         radicandSize.height() + 2*k_heightMargin + k_radixLineThickness), expressionColor);
    ctx->fillRect(KDRect(p.x() + indexSize.width() + k_widthMargin,
                         p.y() + indexSize.height() + k_indexHeight - radicandLayout()->baseline() - k_radixLineThickness - k_heightMargin,
                         radicandSize.width() + 2*k_widthMargin,
                         k_radixLineThickness), expressionColor);
    ctx->fillRect(KDRect(p.x() + indexSize.width() + k_widthMargin + radicandSize.width() + 2*k_widthMargin,
                         p.y() + indexSize.height() + k_indexHeight - radicandLayout()->baseline() - k_radixLineThickness - k_heightMargin,
                         k_radixLineThickness,
                         k_rightRadixHeight + k_radixLineThickness), expressionColor);
  } else {
    ctx->fillRect(KDRect(p.x() + indexSize.width() + k_widthMargin,
                         p.y(),
                         k_radixLineThickness,
                         radicandSize.height() + 2*k_heightMargin + k_radixLineThickness), expressionColor);
    ctx->fillRect(KDRect(p.x() + indexSize.width() + k_widthMargin,
                         p.y(),
                         radicandSize.width() + 2*k_widthMargin,
                         k_radixLineThickness), expressionColor);
    ctx->fillRect(KDRect(p.x() + indexSize.width() + k_widthMargin + radicandSize.width() + 2*k_widthMargin,
                         p.y(),
                         k_radixLineThickness,
                         k_rightRadixHeight + k_radixLineThickness), expressionColor);
  }

}

KDSize NthRootLayout::computeSize() {
  KDSize radicandSize = radicandLayout()->size();
  KDSize indexSize = indexLayout() != nullptr ? indexLayout()->size() : KDSize(k_leftRadixWidth,0);
  return KDSize(
      indexSize.width() + 3*k_widthMargin + 2*k_radixLineThickness + radicandSize.width(),
      m_baseline + radicandSize.height() - radicandLayout()->baseline() + k_heightMargin
      );
}

KDPoint NthRootLayout::positionOfChild(ExpressionLayout * child) {
  KDCoordinate x = 0;
  KDCoordinate y = 0;
  KDSize indexSize = indexLayout() != nullptr ? indexLayout()->size() : KDSize(k_leftRadixWidth,0);
  if (child == indexLayout()) {
    x = 0;
    y = m_baseline - indexSize.height() -  k_indexHeight;
  } else if (child == radicandLayout()) {
    x = indexSize.width() + 2*k_widthMargin + k_radixLineThickness;
    y = m_baseline - radicandLayout()->baseline();
  } else {
    assert(false);
  }
  return KDPoint(x,y);
}

ExpressionLayout * NthRootLayout::radicandLayout() {
  return editableChild(0);
}

ExpressionLayout * NthRootLayout::indexLayout() {
  return editableChild(1);
}

}

#include "tetrisshape.h"
#include <random>
#include <time.h>

// initialize static class fields
bool TetrisShape::s_constructOnce = false;
std::array<std::array<int, 2>, 4> aaa = { std::array<int, 2>{ 0,0 }, { 0,1 }, { 0,2 }, { 0,3 } };
const int TetrisShape::TETRIS_SHAPES [TetrisShape::NUMBER_OF_SHAPES][4][2] = {
    { { 0,0 }, { 0,1 }, { 0,2 }, { 0,3 } }, // line
    { { 0,0 }, { 0,1 }, { 1,0 }, { 1,1 } }, // square
    { { 0,0 }, { 1,0 }, { 1,1 }, { 1,2 } }, // lshape
    { { 0,0 }, { 1,0 }, { 2,0 }, { 2,1 } }, // jshape
    { { 1,0 }, { 0,1 }, { 1,1 }, { 2,1 } }, // tee
    { { 0,0 }, { 1,0 }, { 1,1 }, { 2,1 } }, // zshape
    { { 0,0 }, { 0,1 }, { 1,1 }, { 1,2 } } // sshape
};

TetrisShape::TetrisShape(ShapeType shape)
    : m_shape(shape), m_rotation(0) {
    // set random time seed
    if (!TetrisShape::s_constructOnce) {
        TetrisShape::s_constructOnce = true;
        srand(time(nullptr));
    }
}

void TetrisShape::setShape(const TetrisShape &shape) {
    m_shape = shape.m_shape;
    m_rotation = shape.m_rotation;
}

void TetrisShape::setShapeRandom() {
    m_shape = static_cast<ShapeType>(std::rand() % 7);
    m_rotation = std::rand() % 4;
}

void TetrisShape::rotate(QPoint *coordinate) {
    int saveWidth = width();
    int saveHeight = height();
    m_rotation = (m_rotation + 1) % 4;
    if (coordinate != nullptr) {
        coordinate->setX(coordinate->x() + (saveWidth - width()) / 2);
        coordinate->setY(coordinate->y() + (saveHeight - height()) / 2);
    }
}

int TetrisShape::TetrisShape::width() const {
    if (m_shape == none)
        return 0;
    int shapeIndex = static_cast<int>(m_shape);
    return m_rotation % 2 ? TETRIS_SHAPES[shapeIndex][3][1] +1 : TETRIS_SHAPES[shapeIndex][3][0] +1;
}

int TetrisShape::TetrisShape::height() const {
    if (m_shape == none)
        return 0;
    int shapeIndex = static_cast<int>(m_shape);
    return m_rotation % 2 ? TETRIS_SHAPES[shapeIndex][3][0] +1 : TETRIS_SHAPES[shapeIndex][3][1] +1;
}

int TetrisShape::getCoordinate (int index, int baseCoord) const {
    if (m_shape == none)
        return 0;

    int shapeIndex = static_cast<int>(m_shape);
    int baseW = TETRIS_SHAPES[shapeIndex][3][0];
    int baseH = TETRIS_SHAPES[shapeIndex][3][1];
    int baseX = TETRIS_SHAPES[shapeIndex][index][0];
    int baseY = TETRIS_SHAPES[shapeIndex][index][1];
    switch (m_rotation) {

    // rotation 90°
    case 1:
        return !baseCoord ? baseY : baseW - baseX;

    // rotation 180°
    case 2:
        return !baseCoord ? baseW - baseX : baseH - baseY;

    // rotation 270°
    case 3:
        return !baseCoord ? baseH - baseY : baseX;

    // no rotation
    default:
        return !baseCoord ? baseX : baseY;
    }
}

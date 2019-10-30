#ifndef TETRISSHAPE_H
#define TETRISSHAPE_H

#include <array>
#include <QtCore>

class TetrisShape
{
public:
    static const int NUMBER_OF_SHAPES = 7;
    enum ShapeType { line, square, lshape, jshape, tee, zshape, sshape, none };

	static const int TETRIS_SHAPES[ NUMBER_OF_SHAPES ][4][2];

    TetrisShape( ShapeType shape = none );

    bool operator !=( const TetrisShape &shape ) { return( this->m_shape != shape.m_shape || this->m_rotation != shape.m_rotation ); }
    bool operator ==( const ShapeType shape ) { return this->m_shape == shape; }
    void setShape( const TetrisShape &shape );
    void setShapeRandom();
    void rotate( QPoint *coordinate = nullptr );
    ShapeType shapeType() const { return m_shape; }
    int shapeIndex() const { return static_cast<int>( m_shape ); }
    int x( int index ) const { return getCoordinate( index, 0 ); }
    int y( int index ) const { return getCoordinate( index, 1 ); }
    int width() const;
    int height() const;
    int countSquares() const { return 4; }
private:
    static bool s_constructOnce;
    int getCoordinate( int index, int baseCoord ) const;
    ShapeType m_shape;
    int m_rotation;
};

#endif // TETRISSHAPE_H

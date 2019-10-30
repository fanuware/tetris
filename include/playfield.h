#ifndef PLAYFIELD_H
#define PLAYFIELD_H

#include <QFrame>
#include <QtGui>
#include <QtCore>
#include <QLabel>
#include "tetrisshape.h"

#define WIDTH 18
#define HEIGHT 22

// color of each tetris shape
class ShapesColor{
public:
    QColor colorSet[ TetrisShape::NUMBER_OF_SHAPES ] = {
        QColor( "crimson" ),
        QColor( "limegreen" ),
        QColor( "lightskyblue" ),
        QColor( "mediumorchid" ),
        QColor( "mediumturquoise" ),
        QColor( "orange" ),
        QColor( "lightslategrey" )
    };
    QColor &operator [] (TetrisShape i) { return colorSet[static_cast<int>(i.shapeType())]; }
};

// play field to control the whole game
class PlayField: public QFrame
{
    Q_OBJECT

    enum GameState { STARTED, STOPPED };

public:

    // game settings
    struct Settings {
        int cycleSpeed = 40;
        int previewSize = 50;
        bool showGrid = false;
        QColor gridColor = QColor( "white" );
        QColor background = QColor( "white" );
        ShapesColor shapesColor;
    };
    void saveSettings();
    void restoreSettings();

    PlayField( QWidget* parent);
    void addLabels( QLabel *lblPreview, QLabel *lblLevel, QLabel *lblScore );
    void setSettings(const Settings &s);
    Settings settings() const { return m_settings; }
protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void timerEvent(QTimerEvent *event);
public slots:
    void startGame();
	void stopGame();
	void containerResized(int width, int height);
private:
    void turnShape();
    void paintSquare( QPainter *painter, const TetrisShape &shape,
                      int posX, int posY,
                      int squareWidth, int squareHeight );
    void showPreview();
    void clearPlayField();
    bool checkCollision( const QPoint &point, const TetrisShape &shape );
    bool moveShape( int x, int y );
    bool spawnShape();
    void removeFullLines();
    void removeLine( int indexHeight );
    void actualizeScoreLevel();
    void restartTime();
    int calculateCycle();

    int m_timerId = 0;
    int m_squareWidth = 20;
    int m_squareHeight = 20;
    int m_score = 0;
    int m_level = 1;
    const int LEVEL_RAISE = 500;
    QLabel *m_lblPreview;
    QLabel *m_lblLevel;
    QLabel *m_lblScore;
    QPoint m_actShapePosition;
    TetrisShape m_actShape;
    TetrisShape m_nextShape;
    TetrisShape::ShapeType m_matrix[ WIDTH ][ HEIGHT ];
    GameState m_gameState;
    Settings m_settings;
};

#endif // PLAYFIELD_H

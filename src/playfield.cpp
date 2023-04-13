#include "playfield.h"

void PlayField::saveSettings() {
    QSettings mySettings("Fanu", "Tetris");
    mySettings.beginGroup("MainWindow");
    mySettings.setValue("cycleSpeed", m_settings.cycleSpeed);
    mySettings.setValue("previewSize", m_settings.previewSize);
    mySettings.setValue("showGrid", m_settings.showGrid);
    mySettings.setValue("gridColor", m_settings.gridColor);
    mySettings.setValue("background", m_settings.background);
    for (int i = 0; i < TetrisShape::NUMBER_OF_SHAPES; i++ )
        mySettings.setValue(QString("shapesColor").append(QString::number(i)), m_settings.shapesColor.colorSet[i]);
    mySettings.endGroup();
}

void PlayField::restoreSettings() {
    QSettings mySettings("Fanu", "Tetris");
    mySettings.beginGroup("MainWindow");
    if (mySettings.contains("mySettings")) {
        m_settings.cycleSpeed = mySettings.value("cycleSpeed").value<int>();
        m_settings.previewSize = mySettings.value("previewSize").value<int>();
        m_settings.showGrid = mySettings.value("showGrid").value<bool>();
        m_settings.gridColor = mySettings.value("gridColor").value<QColor>();
        m_settings.background = mySettings.value("background").value<QColor>();
        for (int i = 0; i < TetrisShape::NUMBER_OF_SHAPES; i++ )
            m_settings.shapesColor.colorSet[i] = mySettings.value(QString("shapesColor").append(QString::number(i))).value<QColor>();
    }
    mySettings.endGroup();
}

PlayField::PlayField(QWidget* parent)
    : QFrame(parent) {
    setFocusPolicy(Qt::StrongFocus);
    setFrameStyle(QFrame::Panel | QFrame::Sunken);

    // restore user settings
    restoreSettings();

    // initialize members
    m_lblPreview = nullptr;
    m_gameState = STOPPED;
    clearPlayField();
}

void PlayField::addLabels(QLabel *lblPreview, QLabel *lblLevel, QLabel *lblScore) {
    m_lblPreview = lblPreview;
    m_lblLevel = lblLevel;
    m_lblScore = lblScore;
    showPreview();
}

void PlayField::setSettings(const PlayField::Settings &s) {
    if (m_gameState == STARTED && m_settings.cycleSpeed != s.cycleSpeed) {
        m_settings.cycleSpeed = s.cycleSpeed;
        restartTime();
    }
    if ((m_nextShape != TetrisShape::none) && m_settings.previewSize != s.previewSize)
        m_settings.previewSize = s.previewSize;
        showPreview();
    m_settings = s;

    // save user settings
    saveSettings();
}

void PlayField::paintEvent(QPaintEvent *event) {
    QFrame::paintEvent(event);

    QPainter painter(this);
    QRect rect = contentsRect();
    painter.drawText(rect, Qt::AlignCenter, tr("Pause"));

    // draw background
    painter.setPen(QPen(Qt::NoPen));
    painter.setBrush(m_settings.background);
    painter.drawRect(0, 0, this->width(), this->height());

    // draw grid
    if (m_settings.showGrid) {
        QPen pen(Qt::DotLine);
        pen.setWidth(2);
        pen.setColor(m_settings.gridColor);
        painter.setPen(pen);

        // X-grid
        for (int i = 1; i < HEIGHT; i++ )
            painter.drawLine(0, i * m_squareHeight, WIDTH * m_squareWidth, i * m_squareHeight  );

        // Y-grid
        for (int i = 1; i < WIDTH; i++ )
            painter.drawLine(i * m_squareWidth, 0, i * m_squareWidth, HEIGHT * m_squareHeight);
    }

    // draw background squares
    for(int i=0; i<WIDTH; i++ ) {
        for (int j=0; j<HEIGHT; j++) {
            paintSquare(&painter, m_matrix[i][j],
                        i * m_squareWidth, j * m_squareHeight,
                        m_squareWidth, m_squareHeight);
        }
    }

    // draw active shape
    for(int i=0; i<4; i++ )
        paintSquare(&painter, m_actShape,
                    (m_actShape.x(i) + m_actShapePosition.x()) * m_squareWidth,
                    (m_actShape.y(i) + m_actShapePosition.y()) * m_squareHeight,
                    m_squareWidth, m_squareHeight);
}

void PlayField::keyPressEvent(QKeyEvent *event) {
    if (m_gameState != STARTED)
        return;
    switch (event->key()) {
    case Qt::Key_Left:
        moveShape(-1, 0);
        break;
    case Qt::Key_Right:
        moveShape(1, 0);
        break;
    case Qt::Key_Down:
        moveShape(0, 1);
        break;
    case Qt::Key_Up:
        turnShape();
        break;
    case Qt::Key_Space:

        // move down as long as no collision
        while(moveShape(0, 1)){};
        break;
    }
    update();
}

void PlayField::timerEvent(QTimerEvent *event) {
    if (m_gameState == STARTED) {
        moveShape(0, 1);
        update();
    }
    else
        killTimer(event->timerId());
}

void PlayField::startGame() {
    clearPlayField();
    spawnShape();

    // reset game state
    m_score = 0;
    m_level = 1;
    actualizeScoreLevel();

    // make sure to start timer only once
    // note: avoid multiple timers
    if (m_gameState != STARTED) {
        m_gameState = STARTED;
        m_timerId = startTimer(calculateCycle());
    }
}

void PlayField::stopGame() {
    m_gameState = STOPPED;
}

void PlayField::containerResized(int width, int height) {
    int setWidth = height * WIDTH / HEIGHT;
    int setHeight = width * HEIGHT / WIDTH;
    if (width < setWidth) {
        setWidth = width;
    }
    else {
        setHeight = height;
    }
    m_squareWidth = m_squareHeight = setWidth / WIDTH;
    setMaximumSize(m_squareWidth * WIDTH, m_squareHeight * HEIGHT);
}

void PlayField::turnShape() {
    QPoint newPoint = m_actShapePosition;
    TetrisShape newShape = m_actShape;
    newShape.rotate(&newPoint);
    if (checkCollision(newPoint, newShape)) {
         m_actShapePosition = newPoint;
         m_actShape = newShape;
         update();
    }
}

void PlayField::paintSquare(QPainter *painter, const TetrisShape &shape,
                            int posX, int posY,
                            int squareWidth, int squareHeight) {
    if (shape.shapeType() == TetrisShape::none)
        return;
    QColor color(m_settings.shapesColor[shape]);
    QPen pen(color.lighter(130));
    QBrush brush(color);

    // draw square
    // note: penWidth is added centered to squace outline
    const int penWidth = 1;
    pen.setWidth(penWidth);
    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawRect(posX + penWidth / 2, posY + penWidth / 2, squareWidth - penWidth, squareHeight - penWidth);
}

void PlayField::showPreview() {
    if (m_lblPreview != nullptr) {

        // calculate square size
        int width = m_lblPreview->width() / 4;
        int height = m_lblPreview->height() / 4;
        int size = width < height ? width : height;
        int range = ((m_settings.previewSize + 1) * 90 / 100) + 10;
        size = size * range / 100;

        // create pixmap to display shape
        QPixmap pixmap(m_nextShape.width() * size, m_nextShape.height() * size);
        QPainter painter(&pixmap);
        painter.fillRect(pixmap.rect(), m_lblPreview->palette().window());
        for(int i=0; i<4; i++ ) {
        paintSquare(&painter, m_nextShape,
                    m_nextShape.x(i) * size, m_nextShape.y(i) * size,
                    size, size);
        }
        m_lblPreview->setPixmap(pixmap);
    }
}

void PlayField::clearPlayField() {
    for(int i=0; i<WIDTH; i++ ) {
        for (int j=0; j<HEIGHT; j++)
            m_matrix[i][j] = TetrisShape::none;
    }
    update();
}

bool PlayField::checkCollision(const QPoint &point, const TetrisShape &shape) {
    int x = point.x();
    int y = point.y();

    // border collision
    if  ((x < 0) || (x + shape.width() > WIDTH)
          && (y < 0) || (y + shape.height() > HEIGHT)) {
        return false;
    }

    // collision within frame
    for(int i=0; i<4; i++ ) {
        if (m_matrix[x + shape.x(i)]
                     [y + shape.y(i)] != TetrisShape::none)
            return false;
    }
    return true;
}

bool PlayField::moveShape(int x, int y) {
    QPoint newPoint = m_actShapePosition;
    TetrisShape newShape = m_actShape;
    newPoint.setX(newPoint.x() + x);
    newPoint.setY(newPoint.y() + y);

    // position valid
    if (checkCollision(newPoint, newShape)) {
        m_actShapePosition = newPoint;
        m_actShape = newShape;
        return true;
    }

    // Y-position invalid
    else if (y) {

        // add active shape to matrix
        for(int i=0; i<4; i++ )
            m_matrix[m_actShapePosition.x() + m_actShape.x(i)]
                    [m_actShapePosition.y() + m_actShape.y(i)] = m_actShape.shapeType();
        removeFullLines();
        int saveActShapeIndex(m_actShape.shapeIndex());
        if (!spawnShape())
            m_gameState = STOPPED;
        else {

            // actualize score
            m_score += 10 + saveActShapeIndex;
            actualizeScoreLevel();
        }
    }
    return false;
}

bool PlayField::spawnShape() {
    if (m_nextShape == TetrisShape::none) {
        m_nextShape.setShapeRandom();
        m_actShape.setShapeRandom();
    }

    // set start position (centered on top)
    QPoint nextShapePosition((WIDTH - m_nextShape.width()) / 2, 0);

    // start position valid
    if (checkCollision(nextShapePosition, m_nextShape)) {
        m_actShapePosition = nextShapePosition;
        m_actShape.setShape(m_nextShape);
        m_nextShape.setShapeRandom();
        showPreview();
        return true;
    }
    return false;
}

void PlayField::removeFullLines() {
    // find full lines
    int fullLines[HEIGHT] = { 0 };
    for(int i=HEIGHT-1; i>=0; i-- ) {
        for(int j=0; j<WIDTH; j++ ) {
            if (m_matrix[j][i] == TetrisShape::none)
                break;
            fullLines[i]++;
        }
    }

    // remove full line
    int lineOffset = 0;
    for(int i=HEIGHT-1; i>=0; i-- ) {
        if (fullLines[i] >= WIDTH) {
            removeLine(i + lineOffset);
            lineOffset++;
        }
    }
}

void PlayField::removeLine(int indexHeight) {
    // actualize score
    m_score += WIDTH * 2;
    actualizeScoreLevel();

    // remove line
    for(int i=indexHeight; i>=1; i-- ) {
        for (int j=0; j<WIDTH; j++ ) {
            m_matrix[j][i] = m_matrix[j][i-1];
        }
    }
    for(int i=0; i<WIDTH; i++ )
        m_matrix[i][0] = TetrisShape::none;
}

void PlayField::actualizeScoreLevel() {
    // determine level
    int saveLevel = m_level;
    m_level = m_score / LEVEL_RAISE + 1;
    if (m_level != saveLevel) {
        restartTime();
    }

    // actualize labels
    m_lblScore->setText(QString::number(m_score));
    m_lblLevel->setText(QString::number(m_level));
}

void PlayField::restartTime() {
    killTimer(m_timerId);
    m_timerId = startTimer(calculateCycle());
}

int PlayField::calculateCycle() {
    float baseTime = (1000.0 - (m_settings.cycleSpeed * 95.0 / 100.0) * 10.0);
    baseTime -= static_cast<int>(static_cast<float>(baseTime) * ((m_level - 1) * 0.5 / 10));
    if (baseTime < 20)
        baseTime = 20;
    return baseTime;
}

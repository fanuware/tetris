#ifndef OPTIONS_H
#define OPTIONS_H

#include <QDialog>
#include "playfield.h"

namespace Ui {
class Options;
}

class Options : public QDialog
{
    Q_OBJECT

public:
    explicit Options(PlayField *PlayField, QWidget *parent = nullptr);
    ~Options();

public slots:
    void resetDefault();
    void refreshWidgetSettings();
    void ok();
    void changePreview();
    void comboBoxColorChanged();
    void checkBoxGrid();
    void speedSlider();
    void previewSize();

private:
    Ui::Options *ui;
    PlayField::Settings m_settings;
    void paintSquare( QPainter *painter, const TetrisShape &shape,
                      int posX, int posY,
                      int squareWidth, int squareHeight );
    void showPreview(QWidget *widget, TetrisShape shape, int width, int height, bool transparent=false );
    void paintColorButtons();
    void paintPreview();

    PlayField *m_playField;
    TetrisShape m_previewShape = TetrisShape::line;
    int m_squarePreviewWidth = 40;
    int m_squarePreviewHeight = 40;
    int m_squareButtonWidth = 10;
    int m_squareButtonHeight = 10;
    QPushButton *m_btnShapes[7];
};

#endif // OPTIONS_H

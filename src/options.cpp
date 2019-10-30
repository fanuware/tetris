#include "options.h"
#include "ui_options.h"
#include <QtCore>
#include <QtGui>

Options::Options(PlayField *PlayField, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Options)
{
    ui->setupUi(this);
    window()->layout()->setSizeConstraint( QLayout::SetFixedSize );
    setWindowIcon( QPixmap( ":/res/settings.png" ) );

    ui->tabWidget->setCurrentIndex(0);

    m_playField = PlayField;

    // get shapes color from playfield
    m_settings = m_playField->settings();

    // assign shape buttons to array for later convenience
    m_btnShapes[0] = ui->btnShape1;
    m_btnShapes[1] = ui->btnShape2;
    m_btnShapes[2] = ui->btnShape3;
    m_btnShapes[3] = ui->btnShape4;
    m_btnShapes[4] = ui->btnShape5;
    m_btnShapes[5] = ui->btnShape6;
    m_btnShapes[6] = ui->btnShape7;

    // show preview
    paintColorButtons();
    paintPreview();

    // combobox colors
    QStringList colorNames = QColor::colorNames();

    // start list most popular colors (non alphabetic)
    QString firstColors[] = { "white", "black", "brown", "red",
                                            "orange", "yellow", "green", "blue",
                                            "purple", "gray", "pink", "turquoise" };
    constexpr int FIRST_COLORS = sizeof(firstColors) / sizeof(*firstColors);

    int i = 0;
    for (auto s : firstColors) {
        ui->cbxShapeColor->insertItem(i, s );
        ui->cbxShapeColor->setItemData(i, QColor( s ), Qt::DecorationRole);
        ui->cbxBackgroundColor->insertItem(i, s );
        ui->cbxBackgroundColor->setItemData(i, QColor( s ), Qt::DecorationRole);
        ui->cbxGridColor->insertItem(i, s );
        ui->cbxGridColor->setItemData(i, QColor( s ), Qt::DecorationRole);
        i++;
    }

    // finish list in alphapetic order
    for (int i = 0; i < colorNames.size(); ++i) {
        QColor color(colorNames[ i ]);
        ui->cbxShapeColor->insertItem(i + FIRST_COLORS, colorNames[ i ]);
        ui->cbxShapeColor->setItemData(i + FIRST_COLORS, color, Qt::DecorationRole);
        ui->cbxBackgroundColor->insertItem(i + FIRST_COLORS, colorNames[ i ]);
        ui->cbxBackgroundColor->setItemData(i + FIRST_COLORS, color, Qt::DecorationRole);
        ui->cbxGridColor->insertItem(i + FIRST_COLORS, colorNames[ i ]);
        ui->cbxGridColor->setItemData(i + FIRST_COLORS, color, Qt::DecorationRole);
    }

    // refresh widget setting (slider, checkbox,..)
    refreshWidgetSettings();

    // connections
    connect(ui->btnBox->button(QDialogButtonBox::Reset) , &QPushButton::clicked, this, &Options::resetDefault);
    connect(ui->btnBox->button(QDialogButtonBox::Ok) , &QPushButton::clicked, this, &Options::ok);
    connect(ui->btnBox->button(QDialogButtonBox::Cancel) , &QPushButton::clicked, this, &QDialog::close);
    connect(ui->cbxShapeColor , static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), this, &Options::comboBoxColorChanged);
    connect(ui->cbxBackgroundColor , static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), this, &Options::comboBoxColorChanged);
    connect(ui->cbxGridColor , static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), this, &Options::comboBoxColorChanged);
    connect(ui->cbShowGrid , &QCheckBox::toggled, this, &Options::checkBoxGrid);
    for (int i=0; i < TetrisShape::none; i++) {
        connect(m_btnShapes[i] , &QPushButton::clicked, this, &Options::changePreview);
    }
    connect(ui->slrSpeed , &QSlider::valueChanged, this, &Options::speedSlider);
    connect(ui->slrPreviewSize , &QSlider::valueChanged, this, &Options::previewSize);
}

Options::~Options()
{
    delete ui;
}

void Options::resetDefault()
{
    PlayField::Settings factorySettings;
    if (ui->tabWidget->currentIndex() == 0 ) {
        m_settings.shapesColor = factorySettings.shapesColor;
        m_settings.background = factorySettings.background;
        m_settings.gridColor = factorySettings.gridColor;
        m_settings.showGrid = factorySettings.showGrid;
        paintColorButtons();
        paintPreview();
    }
    else {
        m_settings.cycleSpeed = factorySettings.cycleSpeed;
        m_settings.previewSize = factorySettings.previewSize;
    }
    refreshWidgetSettings();
}

void Options::refreshWidgetSettings()
{

    // buttons
    ui->cbxShapeColor->setCurrentIndex( ui->cbxShapeColor->findData( m_settings.shapesColor[m_previewShape], Qt::DecorationRole ) );
    ui->cbxBackgroundColor->setCurrentIndex( ui->cbxShapeColor->findData( m_settings.background, Qt::DecorationRole ) );
    ui->cbxGridColor->setCurrentIndex( ui->cbxShapeColor->findData( m_settings.gridColor, Qt::DecorationRole ) );

    // grid
    ui->cbShowGrid->setChecked(m_settings.showGrid);
    ui->cbxGridColor->setDisabled(!m_settings.showGrid);

    // slider
    ui->slrSpeed->setValue(m_settings.cycleSpeed);
    ui->slrPreviewSize->setValue(m_settings.previewSize);
}

void Options::ok()
{
    m_playField->setSettings( m_settings );
    close();
}

void Options::changePreview()
{
    QPushButton *button = (QPushButton *)sender();
    int i;
    for ( i=0; i < TetrisShape::none; i++) {
        if ( button == m_btnShapes[i] ) {
            m_previewShape = static_cast<TetrisShape::ShapeType>(i);
            continue;
        }
    }
    paintPreview();
    refreshWidgetSettings();
}

void Options::comboBoxColorChanged()
{
    m_settings.shapesColor[m_previewShape] = QColor( ui->cbxShapeColor->currentText() );
    m_settings.background = QColor( ui->cbxBackgroundColor->currentText() );
    m_settings.gridColor = QColor( ui->cbxGridColor->currentText() );
    paintPreview();
    int i = m_previewShape.shapeIndex();
    showPreview(m_btnShapes[i], m_previewShape, 40-8, 40-8, true);
}

void Options::checkBoxGrid()
{
    m_settings.showGrid = ui->cbShowGrid->isChecked();
    refreshWidgetSettings();
}

void Options::speedSlider()
{
    m_settings.cycleSpeed = ui->slrSpeed->value();

    // show immediate on playfield
    PlayField::Settings temp = m_playField->settings();
    temp.cycleSpeed = m_settings.cycleSpeed;
    m_playField->setSettings( temp );
}

void Options::previewSize()
{
    m_settings.previewSize = ui->slrPreviewSize->value();

    // show immediate on playfield
    PlayField::Settings temp = m_playField->settings();
    temp.previewSize = m_settings.previewSize;
    m_playField->setSettings( temp );
}

void Options::paintSquare( QPainter *painter, const TetrisShape &shape,
                           int posX, int posY,
                           int squareWidth, int squareHeight )
{
    if ( shape.shapeType() == TetrisShape::none )
        return;
    QColor color( m_settings.shapesColor[shape] );
    QPen pen( color.lighter(130) );
    QBrush brush( color );

    // draw square
    // note: penWidth is added centered to squace outline
    const int penWidth = 1;
    pen.setWidth( penWidth );
    painter->setPen( pen );
    painter->setBrush( brush );
    painter->drawRect( posX + penWidth / 2, posY + penWidth / 2, squareWidth - penWidth, squareHeight - penWidth );
}

void Options::showPreview(QWidget *widget, TetrisShape shape, int width, int height, bool transparent )
{
    // compute average sqare size
    const int SQUARES = shape.countSquares();
    int squareW = width / shape.width();
    int squareH = height / shape.height();
    int squareSize = ( squareW < squareH ) ? squareW : squareH;
    bool isStretchW = squareSize == squareW;
    bool isStretchH = squareSize == squareH;

    // setup pixmap
    QSize size(width, height);
    QPixmap pixmap( size );
    if ( transparent )
        pixmap.fill(Qt::transparent);
    else
        pixmap.fill(m_settings.background);
    QPainter painter( &pixmap );

    // stretch squares if needed
    int *stretchW = new int[ SQUARES ];
    int *stretchH = new int[ SQUARES ];
    int countStretchW = isStretchW ? ( width % squareSize ) : 0;
    int countStretchH = isStretchH ? ( height % squareSize ) : 0;
    for( int i = 0; i < SQUARES; i++ ) {
        stretchW[i] = ( countStretchW-- > 0 ) ? 1 : 0;
        stretchH[i] = ( countStretchH-- > 0 ) ? 1 : 0;
    }

    // paint each square
    int offsetX = isStretchW ? 0 : ( width - ( shape.width() * squareSize ) ) / 2;
    int offsetY = isStretchH ? 0 : ( height - ( shape.height() * squareSize ) ) / 2;
    for( int i = 0; i < SQUARES; i++ ) {

        // start position X
        int posX = ( shape.x(i) * squareSize ) + offsetX;
        for ( int j = 0; j < shape.x(i); j++ )
            posX += stretchW[j];

        // start position Y
        int posY = ( shape.y(i) * squareSize ) + offsetY;
        for ( int j = 0; j < shape.y(i); j++ )
            posY += stretchH[j];
        paintSquare( &painter, shape, posX, posY, squareSize + stretchW[shape.x(i)], squareSize + stretchH[shape.y(i)] );
    }
    delete[] stretchW;
    delete[] stretchH;

    // paint on widget
    QPushButton *pBtn = dynamic_cast<QPushButton*>( widget );
    if ( pBtn ) {
        pBtn->setIconSize( size );
        pBtn->setIcon( pixmap );
    }
    QLabel *pLbl = dynamic_cast<QLabel*>( widget );
    if ( pLbl )
        pLbl->setPixmap( pixmap );
}

void Options::paintColorButtons()
{
    for (int i=0; i < TetrisShape::none; i++) {
        showPreview(m_btnShapes[i], static_cast<TetrisShape::ShapeType>(i), 40-8, 40-8, true);
    }
}

void Options::paintPreview()
{
    int width = ui->lblPreview->width();
    int height = ui->lblPreview->height();
    int size = width < height ? width : height;
    showPreview( ui->lblPreview, m_previewShape, size, size );
}












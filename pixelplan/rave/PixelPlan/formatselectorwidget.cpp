#include "FormatSelectorWidget.h"
#include <QPushButton>
#include <QFontComboBox>
#include <QSpinBox>
#include <QComboBox>
#include <QLabel>
#include <QGridLayout>
#include <QGroupBox>
#include <QFontDialog>
#include <QColorDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QMessageBox>
#include <QApplication>
#include <QPalette>

FormatSelectorWidget::FormatSelectorWidget(const QString& title, QWidget *parent)
    : QWidget(parent)
    , m_currentFont(QApplication::font())
    , m_currentFontColor(Qt::black)
    , m_currentBackgroundColor(Qt::white)
    , m_headerSpacing(0)
    , m_footerSpacing(0)
    , m_alignment("left")
{
    setupUi(title);
    updateFontPreview();
}

void FormatSelectorWidget::setupUi(const QString& title)
{
    QGroupBox* groupbox = new QGroupBox(title, this);
    QGridLayout* groupLayout = new QGridLayout(groupbox);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(groupbox);
    mainLayout->setContentsMargins(0,0,0,0);
    
    // Font selection
    QLabel* fontLabel = new QLabel("Font:", this);
    m_fontButton = new QPushButton("Select Font...", this);
    m_fontButton->setMinimumWidth(150);
    connect(m_fontButton, &QPushButton::clicked, this, &FormatSelectorWidget::onFontButtonClicked);
    
    // Font preview
    m_fontPreviewLabel = new QLabel("Preview Text", this);
    m_fontPreviewLabel->setAlignment(Qt::AlignCenter);
    m_fontPreviewLabel->setMinimumHeight(50);
    m_fontPreviewLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    
    // Font color
    QLabel* fontColorLabel = new QLabel("Font Color:", this);
    m_fontColorButton = new QPushButton(this);
    m_fontColorButton->setMinimumWidth(100);
    m_fontColorButton->setMaximumWidth(100);
    updateColorPreview(m_fontColorButton, m_currentFontColor);
    connect(m_fontColorButton, &QPushButton::clicked, this, &FormatSelectorWidget::onFontColorButtonClicked);
    
    // Background color
    QLabel* bgColorLabel = new QLabel("Background Color:", this);
    m_backgroundColorButton = new QPushButton(this);
    m_backgroundColorButton->setMinimumWidth(100);
    m_backgroundColorButton->setMaximumWidth(100);
    updateColorPreview(m_backgroundColorButton, m_currentBackgroundColor);
    connect(m_backgroundColorButton, &QPushButton::clicked, this, &FormatSelectorWidget::onBackgroundColorButtonClicked);
    
    // Header spacing
    QLabel* headerSpacingLabel = new QLabel("Header Spacing:", this);
    m_headerSpacingSpinBox = new QSpinBox(this);
    m_headerSpacingSpinBox->setRange(0, 5);
    m_headerSpacingSpinBox->setValue(0);
    //m_headerSpacingSpinBox->setSuffix(" px");
    connect(m_headerSpacingSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &FormatSelectorWidget::onSettingChanged);
    
    // Footer spacing
    QLabel* footerSpacingLabel = new QLabel("Footer Spacing:", this);
    m_footerSpacingSpinBox = new QSpinBox(this);
    m_footerSpacingSpinBox->setRange(0, 5);
    m_footerSpacingSpinBox->setValue(0);
    //m_footerSpacingSpinBox->setSuffix(" px");
    connect(m_footerSpacingSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &FormatSelectorWidget::onSettingChanged);
    
    // Alignment
    QLabel* alignmentLabel = new QLabel("Alignment:", this);
    m_alignmentComboBox = new QComboBox(this);
    m_alignmentComboBox->addItem("Left");
    m_alignmentComboBox->addItem("Center");
    m_alignmentComboBox->addItem("Right");
    connect(m_alignmentComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FormatSelectorWidget::onSettingChanged);
    
    // Layout
    int row = 0;
    groupLayout->addWidget(fontLabel, row, 0);
    groupLayout->addWidget(m_fontButton, row, 1, 1, 2);
    
    row++;
    groupLayout->addWidget(m_fontPreviewLabel, row, 0, 1, 3);
    
    row++;
    groupLayout->addWidget(fontColorLabel, row, 0);
    groupLayout->addWidget(m_fontColorButton, row, 1);
    
    row++;
    groupLayout->addWidget(bgColorLabel, row, 0);
    groupLayout->addWidget(m_backgroundColorButton, row, 1);
    
    row++;
    groupLayout->addWidget(headerSpacingLabel, row, 0);
    groupLayout->addWidget(m_headerSpacingSpinBox, row, 1);
    
    row++;
    groupLayout->addWidget(footerSpacingLabel, row, 0);
    groupLayout->addWidget(m_footerSpacingSpinBox, row, 1);
    
    row++;
    groupLayout->addWidget(alignmentLabel, row, 0);
    groupLayout->addWidget(m_alignmentComboBox, row, 1);
    
    groupLayout->setColumnStretch(2, 1);
    groupLayout->setRowStretch(row + 1, 1);
}

void FormatSelectorWidget::updateFontPreview()
{
    m_fontPreviewLabel->setFont(m_currentFont);
    m_fontPreviewLabel->setStyleSheet(QString("color: %1; background-color: %2;")
        .arg(m_currentFontColor.name())
        .arg(m_currentBackgroundColor.name()));
    
    // Update alignment in preview
    Qt::Alignment alignment;
    if (m_alignment == "center")
        alignment = Qt::AlignCenter;
    else if (m_alignment == "right")
        alignment = Qt::AlignRight | Qt::AlignVCenter;
    else
        alignment = Qt::AlignLeft | Qt::AlignVCenter;
    
    m_fontPreviewLabel->setAlignment(alignment);
}

void FormatSelectorWidget::updateColorPreview(QPushButton* button, const QColor& color)
{
    QString styleSheet = QString("QPushButton { background-color: %1; border: 1px solid gray; }").arg(color.name());
    button->setStyleSheet(styleSheet);
}

void FormatSelectorWidget::onFontButtonClicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, m_currentFont, this, "Select Font");
    if (ok) {
        m_currentFont = font;
        m_fontButton->setText(QString("%1 %2").arg(font.family()).arg(font.pointSize()));
        updateFontPreview();
        emit settingsChanged();
    }
}

void FormatSelectorWidget::onFontColorButtonClicked()
{
    QColor color = QColorDialog::getColor(m_currentFontColor, this, "Select Font Color");
    if (color.isValid()) {
        m_currentFontColor = color;
        updateColorPreview(m_fontColorButton, color);
        updateFontPreview();
        emit settingsChanged();
    }
}

void FormatSelectorWidget::onBackgroundColorButtonClicked()
{
    QColor color = QColorDialog::getColor(m_currentBackgroundColor, this, "Select Background Color");
    if (color.isValid()) {
        m_currentBackgroundColor = color;
        updateColorPreview(m_backgroundColorButton, color);
        updateFontPreview();
        emit settingsChanged();
    }
}

void FormatSelectorWidget::onSettingChanged()
{
    m_headerSpacing = m_headerSpacingSpinBox->value();
    m_footerSpacing = m_footerSpacingSpinBox->value();
    m_alignment = m_alignmentComboBox->currentText().toLower();
    updateFontPreview();
    emit settingsChanged();
}

// Getters
QFont FormatSelectorWidget::getFont() const { return m_currentFont; }
QColor FormatSelectorWidget::getFontColor() const { return m_currentFontColor; }
QColor FormatSelectorWidget::getBackgroundColor() const { return m_currentBackgroundColor; }
int FormatSelectorWidget::getHeaderSpacing() const { return m_headerSpacing; }
int FormatSelectorWidget::getFooterSpacing() const { return m_footerSpacing; }
QString FormatSelectorWidget::getAlignment() const { return m_alignment; }

// Setters
void FormatSelectorWidget::setFont(const QFont& font)
{
    m_currentFont = font;
    m_fontButton->setText(QString("%1 %2").arg(font.family()).arg(font.pointSize()));
    updateFontPreview();
}

void FormatSelectorWidget::setFontColor(const QColor& color)
{
    m_currentFontColor = color;
    updateColorPreview(m_fontColorButton, color);
    updateFontPreview();
}

void FormatSelectorWidget::setBackgroundColor(const QColor& color)
{
    m_currentBackgroundColor = color;
    updateColorPreview(m_backgroundColorButton, color);
    updateFontPreview();
}

void FormatSelectorWidget::setHeaderSpacing(int spacing)
{
    m_headerSpacing = spacing;
    m_headerSpacingSpinBox->setValue(spacing);
}

void FormatSelectorWidget::setFooterSpacing(int spacing)
{
    m_footerSpacing = spacing;
    m_footerSpacingSpinBox->setValue(spacing);
}

void FormatSelectorWidget::setAlignment(const QString& alignment)
{
    m_alignment = alignment.toLower();
    int index = m_alignmentComboBox->findText(alignment, Qt::MatchFixedString);
    if (index >= 0)
        m_alignmentComboBox->setCurrentIndex(index);
}

// JSON serialization
QJsonObject FormatSelectorWidget::toJson() const
{
    QJsonObject json;
    
    // Font
    QJsonObject fontObj;
    fontObj["family"] = m_currentFont.family();
    fontObj["pointSize"] = m_currentFont.pointSize();
    fontObj["bold"] = m_currentFont.bold();
    fontObj["italic"] = m_currentFont.italic();
    fontObj["underline"] = m_currentFont.underline();
    json["font"] = fontObj;
    
    // Colors
    json["fontColor"] = m_currentFontColor.name();
    json["backgroundColor"] = m_currentBackgroundColor.name();
    
    // Spacing
    json["headerSpacing"] = m_headerSpacing;
    json["footerSpacing"] = m_footerSpacing;
    
    // Alignment
    json["alignment"] = m_alignment;
    
    return json;
}

void FormatSelectorWidget::fromJson(const QJsonObject& json)
{
    // Font
    if (json.contains("font") && json["font"].isObject()) {
        QJsonObject fontObj = json["font"].toObject();
        QFont font;
        if (fontObj.contains("family"))
            font.setFamily(fontObj["family"].toString());
        if (fontObj.contains("pointSize"))
            font.setPointSize(fontObj["pointSize"].toInt());
        if (fontObj.contains("bold"))
            font.setBold(fontObj["bold"].toBool());
        if (fontObj.contains("italic"))
            font.setItalic(fontObj["italic"].toBool());
        if (fontObj.contains("underline"))
            font.setUnderline(fontObj["underline"].toBool());
        setFont(font);
    }
    
    // Colors
    if (json.contains("fontColor"))
        setFontColor(QColor(json["fontColor"].toString()));
    if (json.contains("backgroundColor"))
        setBackgroundColor(QColor(json["backgroundColor"].toString()));
    
    // Spacing
    if (json.contains("headerSpacing"))
        setHeaderSpacing(json["headerSpacing"].toInt());
    if (json.contains("footerSpacing"))
        setFooterSpacing(json["footerSpacing"].toInt());
    
    // Alignment
    if (json.contains("alignment"))
        setAlignment(json["alignment"].toString());
}

bool FormatSelectorWidget::saveToFile(const QString& filename) const
{
    QJsonObject json = toJson();
    QJsonDocument doc(json);
    
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(nullptr, "Error", QString("Could not save to file: %1").arg(filename));
        return false;
    }
    
    file.write(doc.toJson());
    file.close();
    return true;
}

bool FormatSelectorWidget::loadFromFile(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(nullptr, "Error", QString("Could not load from file: %1").arg(filename));
        return false;
    }
    
    QByteArray data = file.readAll();
    file.close();
    
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject()) {
        QMessageBox::warning(nullptr, "Error", "Invalid JSON format in file");
        return false;
    }
    
    fromJson(doc.object());
    return true;
}

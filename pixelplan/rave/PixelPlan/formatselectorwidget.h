#ifndef FORMATSELECTORWIDGET_H
#define FORMATSELECTORWIDGET_H

#include <QWidget>
#include <QFont>
#include <QColor>
#include <QJsonObject>

QT_BEGIN_NAMESPACE
class QPushButton;
class QFontComboBox;
class QSpinBox;
class QComboBox;
class QLabel;
class QCheckBox;
QT_END_NAMESPACE

class FormatSelectorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FormatSelectorWidget(const QString& title, QWidget *parent = nullptr);
    
    // Get current settings
    QFont getFont() const;
    QColor getFontColor() const;
    QColor getBackgroundColor() const;
    int getHeaderSpacing() const;
    int getFooterSpacing() const;
    QString getAlignment() const;
    
    // Set current settings
    void setFont(const QFont& font);
    void setFontColor(const QColor& color);
    void setBackgroundColor(const QColor& color);
    void setHeaderSpacing(int spacing);
    void setFooterSpacing(int spacing);
    void setAlignment(const QString& alignment);
    
    // JSON serialization
    QJsonObject toJson() const;
    void fromJson(const QJsonObject& json);
    
    // File operations
    bool saveToFile(const QString& filename) const;
    bool loadFromFile(const QString& filename);

signals:
    void settingsChanged();

private slots:
    void onFontButtonClicked();
    void onFontColorButtonClicked();
    void onBackgroundColorButtonClicked();
    void onSettingChanged();

private:
    void setupUi(const QString& title);
    void updateFontPreview();
    void updateColorPreview(QPushButton* button, const QColor& color);
    
    // UI Elements
    QPushButton* m_fontButton;
    QPushButton* m_fontColorButton;
    QPushButton* m_backgroundColorButton;
    QSpinBox* m_headerSpacingSpinBox;
    QSpinBox* m_footerSpacingSpinBox;
    QComboBox* m_alignmentComboBox;
    QLabel* m_fontPreviewLabel;
    
    // Current settings
    QFont m_currentFont;
    QColor m_currentFontColor;
    QColor m_currentBackgroundColor;
    int m_headerSpacing;
    int m_footerSpacing;
    QString m_alignment;
};

#endif // FORMATSELECTORWIDGET_H

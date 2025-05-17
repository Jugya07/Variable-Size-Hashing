#ifndef HASH_GUI_H
#define HASH_GUI_H

#include <QWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QTextEdit>
#include <QPushButton>

class HashGUI : public QWidget
{
    Q_OBJECT

public:
    explicit HashGUI(QWidget *parent = nullptr);

private slots:
    void computeHash();
    void runTests();

private:
    QLineEdit *inputEdit;
    QSpinBox *sizeEdit;
    QSpinBox *costEdit;
    QTextEdit *outputEdit;
    QPushButton *hashButton;
    QPushButton *testButton;
};

#endif // HASH_GUI_H
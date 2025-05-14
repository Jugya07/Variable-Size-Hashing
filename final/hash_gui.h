#ifndef HASH_GUI_H
#define HASH_GUI_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QSpinBox>

class HashGui : public QMainWindow
{
    Q_OBJECT

public:
    HashGui(QWidget *parent = nullptr);

private slots:
    void computeHash();
    void runTests();

private:
    QLineEdit *inputField;
    QSpinBox *sizeField;
    QPushButton *hashButton;
    QPushButton *testButton;
    QTextEdit *outputArea;
};

#endif // HASH_GUI_H
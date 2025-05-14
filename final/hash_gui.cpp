#include "hash_gui.h"
#include "hash_core.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QWidget>
#include <stdexcept>

HashGui::HashGui(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("Custom Hashing Algorithm Demo");
    resize(600, 400);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    // Input field
    layout->addWidget(new QLabel("Input String:"));
    inputField = new QLineEdit();
    layout->addWidget(inputField);

    // Output size field
    layout->addWidget(new QLabel("Output Size (bits):"));
    sizeField = new QSpinBox();
    sizeField->setRange(1, 1024);
    sizeField->setValue(64);
    layout->addWidget(sizeField);

    // Buttons
    hashButton = new QPushButton("Compute Hash");
    testButton = new QPushButton("Run Tests");
    layout->addWidget(hashButton);
    layout->addWidget(testButton);

    // Output area
    layout->addWidget(new QLabel("Output:"));
    outputArea = new QTextEdit();
    outputArea->setReadOnly(true);
    layout->addWidget(outputArea);

    // Connect signals
    connect(hashButton, &QPushButton::clicked, this, &HashGui::computeHash);
    connect(testButton, &QPushButton::clicked, this, &HashGui::runTests);
}

void HashGui::computeHash()
{
    std::string input = inputField->text().toStdString();
    int output_size_bits = sizeField->value();
    try
    {
        std::string hash = cryptographic_hash(input, output_size_bits);
        outputArea->setText(QString::fromStdString("Hash: " + hash));
    }
    catch (const std::exception &e)
    {
        outputArea->setText(QString::fromStdString("Error: " + std::string(e.what())));
    }
}

void HashGui::runTests()
{
    std::string output = run_tests();
    outputArea->setText(QString::fromStdString(output));
}
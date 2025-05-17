#include "hash_gui.h"
#include "hash_core.h"
#include <QVBoxLayout>
#include <QLabel>
#include <stdexcept>
#include <chrono>

HashGUI::HashGUI(QWidget *parent) : QWidget(parent)
{
    // Initialize widgets
    inputEdit = new QLineEdit(this);
    sizeEdit = new QSpinBox(this);
    costEdit = new QSpinBox(this);
    outputEdit = new QTextEdit(this);
    hashButton = new QPushButton("Compute Hash", this);
    testButton = new QPushButton("Run Tests", this);

    // Configure widgets
    inputEdit->setPlaceholderText("Enter input string");
    sizeEdit->setRange(4, 1000000);
    sizeEdit->setSingleStep(4);
    sizeEdit->setValue(128);
    costEdit->setRange(1, 100000);
    costEdit->setValue(124);
    outputEdit->setReadOnly(true);

    // Layout
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(new QLabel("Input String:"));
    layout->addWidget(inputEdit);
    layout->addWidget(new QLabel("Output Size (bits, multiple of 4):"));
    layout->addWidget(sizeEdit);
    layout->addWidget(new QLabel("Cost (1–100000):"));
    layout->addWidget(costEdit);
    layout->addWidget(hashButton);
    layout->addWidget(testButton);
    layout->addWidget(new QLabel("Output:"));
    layout->addWidget(outputEdit);
    setLayout(layout);

    // Connect signals
    connect(hashButton, &QPushButton::clicked, this, &HashGUI::computeHash);
    connect(testButton, &QPushButton::clicked, this, &HashGUI::runTests);
}

void HashGUI::computeHash()
{
    QString input = inputEdit->text();
    int size = sizeEdit->value();
    int cost = costEdit->value();
    try
    {
        if (size % 4 != 0)
        {
            throw std::runtime_error("Size must be a multiple of 4");
        }
        auto start = std::chrono::high_resolution_clock::now();
        std::string hash = cryptographic_hash(input.toStdString(), size, cost);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        outputEdit->setText(QString("Hash: %1\nTime: %2 ms").arg(QString::fromStdString(hash)).arg(duration));
    }
    catch (const std::exception &e)
    {
        outputEdit->setText("Error: " + QString(e.what()));
    }
}

void HashGUI::runTests()
{
    std::string result = run_tests();
    outputEdit->setText(QString::fromStdString(result));
}
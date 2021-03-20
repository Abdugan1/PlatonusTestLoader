#ifndef TESTBUTTON_H
#define TESTBUTTON_H

#include <QPushButton>

class TestButton : public QPushButton
{
    Q_OBJECT
public:
    TestButton(const QString& text, QWidget* parent = nullptr)
        : QPushButton(text, parent) {}

    struct TestData
    {
        QString name;
        QString id;
    };
    TestData testData;
private:
};

#endif // TESTBUTTON_H

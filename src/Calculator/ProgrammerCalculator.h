#pragma once

#include "Button.h"
#include <QWidget>

class ProgrammerCalculator : public QWidget
{
    Q_OBJECT
public:
    explicit ProgrammerCalculator(QWidget *parent = 0);
    ~ProgrammerCalculator();

private slots:
    void digitClicked();

private:
    Button *createButton(const QString &text, const char *member);

    QLineEdit *display;
    double sumInMemory;  // the value stored in the calculator's memory
    double sumSoFar;     // the value accumulated so far
    double factorSoFar;  // a temporary value when doing multiplications and divisions
    QString pendingAdditiveOperator;       // the last additive operator clicked by the user
    QString pendingMultiplicativeOperator; // the last multiplicative operator clicked by the user
    bool waitingForOperand;  // True represents the calculator is expecting the user to start typing an operand

    enum { NumDigitButtons = 10, NumLetterButtons = 6};
    Button *digitButtons[NumDigitButtons];
    Button *letterButtons[NumLetterButtons];
};
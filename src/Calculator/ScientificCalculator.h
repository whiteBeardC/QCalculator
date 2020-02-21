#pragma once

#include "Button.h"
#include <QWidget>
#include <QLineEdit>
#include <QKeyEvent>
#include <QMap>

class ScientificCalculator : public QWidget
{
    Q_OBJECT
public:
    explicit ScientificCalculator(QWidget *parent = nullptr);
    ~ScientificCalculator();


private slots:
    void digitClicked();
    void unaryOperatorClicked();
    void additiveOperatorClicked();
    void multiplicativeOperatorClicked();
    void equalClicked();
    void pointClicked();
    void changeSignClicked();
    void backspaceClicked();
    void clear();
    void clearAll();
    void clearMemory();
    void readMemory();
    void setMemory();
    void addToMemory();

private:
    QLineEdit *display;  // 显示框
    double sumInMemory;  // 计算器内存中的值
    double sumSoFar;     // 累积到现在的值
    double factorSoFar;  // 进行乘除运算时的临时结果
    QString pendingAdditiveOperator;       // 最后一次输入的加减运算符
    QString pendingMultiplicativeOperator; // 最后一次输入的乘除运算符
    bool waitingForOperand;  // true 表示等待输入一个操作数

    enum { NumDigitButtons = 10 };
    Button *digitButtons[NumDigitButtons];
    Button *clearMemoryButton;
    Button *readMemoryButton;
    Button *setMemoryButton;
    Button *addToMemoryButton;
    Button *pointButton;
    Button *changeSignButton;
    Button *backspaceButton;
    Button *clearButton;
    Button *clearAllButton;
    Button *divisionButton;
    Button *timesButton;
    Button *minusButton;
    Button *plusButton;
    Button *leftBracketButton;
    Button *rightBracketButton;
    Button *piButton;
    Button *factorialButton;
    Button *sinButton;
    Button *cosButton;
    Button *tanButton;
    Button *logButton;
    Button *expButton;
    Button *modButton;
    Button *squareRootButton;
    Button *powerButton;
    Button *squareButton;
    Button *tenExponentButton;
    Button *shiftButton;
    Button *equalButton;

    QMap<Qt::Key, Button*> *mapKeyButton;

    Button *createButton(const QString &text, const char *member);
    void divideByZero();
    void invalidInput();
    bool calculate(double rightOperand, const QString &pendingOperator);
    void keyPressEvent(QKeyEvent *event);
    void buildMapKeyButton();

};
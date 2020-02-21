#include "ScientificCalculator.h"
#include <QGridLayout>
#include <cmath>

ScientificCalculator::ScientificCalculator(QWidget *parent)
    : QWidget(parent)
{
    sumInMemory = 0.0;
    sumSoFar = 0.0;
    factorSoFar = 0.0;
    pendingAdditiveOperator = "";
    pendingMultiplicativeOperator = "";
    waitingForOperand = true;

    // 显示框
    display = new QLineEdit("0");
    display->setReadOnly(true);   // 文本只读
    display->setAlignment(Qt::AlignRight);
    display->setMaxLength(20);
    display->setMinimumHeight(60);
    QFont font = display->font(); // 获取当前的字体样式
    font.setPointSize(font.pointSize() + 15);
    display->setFont(font);

    // MC
    clearMemoryButton = createButton(tr("MC"), SLOT(clearMemory()));
    // MR
    readMemoryButton = createButton(tr("MR"), SLOT(readMemory()));
    // MS
    setMemoryButton = createButton(tr("MS"), SLOT(setMemory()));
    // M+
    addToMemoryButton = createButton(tr("M+"), SLOT(addToMemory()));
    // 数字键
    for (int i = 0; i < NumDigitButtons; ++i)
    {
        digitButtons[i] = createButton(QString::number(i), SLOT(digitClicked()));
        digitButtons[i]->setFont(QFont("Microsoft YaHei UI", 15, 50));
        digitButtons[i]->setStyleSheet("QToolButton{background-color:rgba(192,192,192,50%)}");
    }
    // 小数点
    pointButton = createButton(tr("."), SLOT(pointClicked()));
    // 正负号
    changeSignButton = createButton(tr("\302\261"), SLOT(changeSignClicked()));
    // 退格键
    backspaceButton = createButton(tr("\342\214\253"), SLOT(backspaceClicked()));
    // CE Button
    clearButton = createButton(tr("CE"), SLOT(clear()));
    // C Button
    clearAllButton = createButton(tr("C"), SLOT(clearAll()));
    // 除号
    divisionButton = createButton(tr("\303\267"), SLOT(multiplicativeOperatorClicked()));
    // 乘号
    timesButton = createButton(tr("\303\227"), SLOT(multiplicativeOperatorClicked()));
    // 减号
    minusButton = createButton(tr("-"), SLOT(additiveOperatorClicked()));
    // 加号
    plusButton = createButton(tr("+"), SLOT(additiveOperatorClicked()));
    // 左括号
    leftBracketButton = createButton(tr("("), SLOT(leftBracket()));
    // 右括号
    rightBracketButton = createButton(tr(")"), SLOT(rightBracket()));
    // pi
    piButton = createButton(tr("\317\200"), SLOT(digitClicked()));
    // n! 阶乘
    factorialButton = createButton(tr("n!"), SLOT(factorial()));
    // sin
    sinButton = createButton(tr("sin"), SLOT(unaryOperatorClicked()));
    // cos
    cosButton = createButton(tr("cos"), SLOT(unaryOperatorClicked()));
    // tan
    tanButton = createButton(tr("tan"), SLOT(unaryOperatorClicked()));
    // log
    logButton = createButton(tr("log"), SLOT(unaryOperatorClicked()));
    // exp
    expButton = createButton(tr("Exp"), SLOT(unaryOperatorClicked()));
    // mod
    modButton = createButton(tr("Mod"), SLOT(factorial()));
    // 根号
    squareRootButton = createButton(tr("\342\210\232"), SLOT(unaryOperatorClicked()));
    // x^y
    powerButton = createButton(tr("x^y"), SLOT(unaryOperatorClicked()));
    // x^2
    squareButton = createButton(tr("x\302\262"), SLOT(unaryOperatorClicked()));
    // 10^x
    tenExponentButton = createButton(tr("10^x"), SLOT(unaryOperatorClicked()));
    // shift
    shiftButton = createButton(tr("\342\206\221"), SLOT(unaryOperatorClicked()));
    // 等号
    equalButton = createButton(tr("="), SLOT(equalClicked()));

    // Layout
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetDefaultConstraint);

    mainLayout->addWidget(display, 0, 0, 1, 5);  // 显示框

    mainLayout->addWidget(clearMemoryButton, 1, 0);  // MC
    mainLayout->addWidget(readMemoryButton, 1, 1);   // MR
    mainLayout->addWidget(addToMemoryButton, 1, 2);  // M+
    mainLayout->addWidget(setMemoryButton, 1, 3);    // MS

    mainLayout->addWidget(squareButton, 2, 0); // x^2
    mainLayout->addWidget(powerButton, 2, 1);  // x^y
    mainLayout->addWidget(sinButton, 2, 2);    // sin
    mainLayout->addWidget(cosButton, 2, 3);    // cos
    mainLayout->addWidget(tanButton, 2, 4);    // tan

    mainLayout->addWidget(squareRootButton, 3, 0);  // 根号
    mainLayout->addWidget(tenExponentButton, 3, 1);  // 10^x
    mainLayout->addWidget(logButton, 3, 2);  // log
    mainLayout->addWidget(expButton, 3, 3);  // Exp
    mainLayout->addWidget(modButton, 3, 4);  // Mod

    mainLayout->addWidget(shiftButton, 4, 0);     // shift
    mainLayout->addWidget(clearButton, 4, 1);     // CE
    mainLayout->addWidget(clearAllButton, 4, 2);  // C
    mainLayout->addWidget(backspaceButton, 4, 3); // 退格键
    mainLayout->addWidget(divisionButton, 4, 4);  // 除号

    mainLayout->addWidget(timesButton, 5, 4); // 乘号
    mainLayout->addWidget(minusButton, 6, 4); // 减号
    mainLayout->addWidget(plusButton, 7, 4);  // 加号
    mainLayout->addWidget(equalButton, 8, 4); // 等号

    mainLayout->addWidget(piButton, 5, 0);  // pi
    mainLayout->addWidget(factorialButton, 6, 0);  // 阶乘
    mainLayout->addWidget(changeSignButton, 7, 0); // 正负号
    mainLayout->addWidget(leftBracketButton, 8, 0); // 左括号

    mainLayout->addWidget(rightBracketButton, 8, 1); // 右括号
    mainLayout->addWidget(digitButtons[0], 8, 2);
    mainLayout->addWidget(pointButton, 8, 3);

    // 数字键 1-9
    for (int i = 1; i < NumDigitButtons; ++i)
    {
        int row = ((9 - i) / 3) + 5;
        int column = (i - 1) % 3 + 1;
        mainLayout->addWidget(digitButtons[i], row, column);
    }

    setLayout(mainLayout);
    buildMapKeyButton();
}

ScientificCalculator::~ScientificCalculator()
{

}

Button *ScientificCalculator::createButton(const QString &text, const char *member)
{
    Button *button = new Button(text);
    connect(button, SIGNAL(clicked()), this, member);
    return button;
}

void ScientificCalculator::digitClicked()
{
    Button *clickedButton = qobject_cast<Button *>(sender());
    int digitValue = clickedButton->text().toInt();

    // 如果LineEdit显示的是 0 ，并且输入的是 0， 那么仍然显示 0 ,而不是 00
    if (display->text() == "0" && digitValue == 0.0)
    {
        return;
    }

    // 如果当前计算器的状态是等待输入一个操作数,那么新的数字将是新的操作数的第一个数字
    // 在这种情况下,之前的计算结果要被清除
    if (waitingForOperand)
    {
        display->clear();
        waitingForOperand = false;
    }
    display->setText(display->text() + QString::number(digitValue));
}


// 点击小数点键, 效果: 在当前显示中添加小数点
void ScientificCalculator::pointClicked()
{
    if (waitingForOperand)
    {
        display->setText("0");
    }
    if (!display->text().contains('.'))
    {
        display->setText(display->text() + tr("."));
    }
    waitingForOperand = false;
}

// 点击正负号键, 效果: 改变符号
void ScientificCalculator::changeSignClicked()
{
    QString text = display->text();
    double value = text.toDouble();

    if (value > 0.0)
    {
        text.prepend(tr("-"));
    }
    else if (value < 0.0)
    {
        text.remove(0, 1);
    }
    display->setText(text);
}

// 点击退格键, 效果: 删除当前显示的最后一个字符
void ScientificCalculator::backspaceClicked()
{
    if (waitingForOperand)
    {
        return;
    }

    QString text = display->text();
    text.chop(1);  // 移除字符串中的最后一个字符
    if (text.isEmpty())
    {
        text = "0";
        waitingForOperand = true;
    }
    display->setText(text);
}

// 点击一元运算符(百分号、根号、x^2、1/x)键, 效果: 一元计算
void ScientificCalculator::unaryOperatorClicked()
{
    Button *clickedButton = qobject_cast<Button *>(sender());
    QString clickedOperator = clickedButton->text();
    double operand = display->text().toDouble();
    double result = 0.0;

    if (clickedOperator == tr("sin"))
    {
        result = sin(operand);
    }
    else if (clickedOperator == tr("cos"))
    {
        result = cos(operand);
    }
    else if (clickedOperator == tr("tan"))
    {
        result = tan(operand);
    }
    else if (clickedOperator == tr("log"))
    {
        if (operand < 0.0)
        {
            invalidInput();
            return;
        }
        result = log(operand);
    }
    else if (clickedOperator == tr("Exp"))
    {
        result = exp(operand);
    }
    else if (clickedOperator == tr("\342\210\232"))
    {
        if (operand < 0.0)
        {
            invalidInput();
            return;
        }
        result = std::sqrt(operand);
    }
    else if (clickedOperator == tr("x\302\262"))
    {
        result = std::pow(operand, 2.0);
    }
    else if (clickedOperator == tr("10^x"))
    {
        result = pow(10,operand);
    }
    display->setText(QString::number(result));
    waitingForOperand = true;
}

// 点击加减运算符键, 效果: 加减计算
void ScientificCalculator::additiveOperatorClicked()
{
    Button *clickedButton = qobject_cast<Button *>(sender());
    QString clickedOperator = clickedButton->text();
    double operand = display->text().toDouble();

    if (!pendingMultiplicativeOperator.isEmpty())
    {
        if (!calculate(operand, pendingMultiplicativeOperator))
        {
            divideByZero();
            return;
        }
        display->setText(QString::number(factorSoFar));
        operand = factorSoFar;
        factorSoFar = 0.0;
        pendingMultiplicativeOperator.clear();
    }

    if (!pendingAdditiveOperator.isEmpty())
    {
        if (!calculate(operand, pendingAdditiveOperator))
        {
            divideByZero();
            return;
        }
        display->setText(QString::number(sumSoFar));
    }
    else
    {
        sumSoFar = operand;
    }

    pendingAdditiveOperator = clickedOperator;
    waitingForOperand = true;
}

// 点击乘除运算符键, 效果: 乘除计算
void ScientificCalculator::multiplicativeOperatorClicked()
{
    Button *clickedButton = qobject_cast<Button *>(sender());
    QString clickedOperator = clickedButton->text();
    double operand = display->text().toDouble();

    if (!pendingMultiplicativeOperator.isEmpty())
    {
        if (!calculate(operand, pendingMultiplicativeOperator))
        {
            divideByZero();
            return;
        }
        display->setText(QString::number(factorSoFar));
    }
    else
    {
        factorSoFar = operand;
    }

    pendingMultiplicativeOperator = clickedOperator;
    waitingForOperand = true;
}

// 点击等号键, 效果: 显示计算结果
void ScientificCalculator::equalClicked()
{
    double operand = display->text().toDouble();

    if (!pendingMultiplicativeOperator.isEmpty())
    {
        if (!calculate(operand, pendingMultiplicativeOperator))
        {
            divideByZero();
            return;
        }
        operand = factorSoFar;
        factorSoFar = 0.0;
        pendingMultiplicativeOperator.clear();
    }
    if (!pendingAdditiveOperator.isEmpty())
    {
        if (!calculate(operand, pendingAdditiveOperator))
        {
            divideByZero();
            return;
        }
        pendingAdditiveOperator.clear();
    }
    else
    {
        sumSoFar = operand;
    }

    display->setText(QString::number(sumSoFar));
    sumSoFar = 0.0;
    waitingForOperand = true;
}

// 点击CE键, 效果: 将当前操作数重置为 0
void ScientificCalculator::clear()
{
    if (waitingForOperand)
        return;

    display->setText("0");
    waitingForOperand = true;
}

// 点击C键, 效果: 将计算器重置为初始状态
void ScientificCalculator::clearAll()
{
    sumSoFar = 0.0;
    factorSoFar = 0.0;
    pendingAdditiveOperator.clear();
    pendingMultiplicativeOperator.clear();
    display->setText("0");
    waitingForOperand = true;
}

// 点击MC键, 效果: sumInMemory置为 0
void ScientificCalculator::clearMemory()
{
    sumInMemory = 0.0;
}

// 点击MR键, 效果: 显示 sumInMemory 的值
void ScientificCalculator::readMemory()
{
    display->setText(QString::number(sumInMemory));
    waitingForOperand = true;
}

// 点击MS键, 效果: 进行等号运算之后, 将sumInMemory重置为当前计算结果
void ScientificCalculator::setMemory()
{
    equalClicked();
    sumInMemory = display->text().toDouble();
}

// 点击M+键, 效果: 进行等号运算之后, 将sumInMemory重置为计算结果与原sumInMemory之和
void ScientificCalculator::addToMemory()
{
    equalClicked();
    sumInMemory += display->text().toDouble();
}

// 除数是 0 时, 显示 "Cannot divide by zero"
void ScientificCalculator::divideByZero()
{
    clearAll();
    display->setText(tr("Cannot divide by zero"));
}

// 开根号时, 操作数不能为 0
void ScientificCalculator::invalidInput()
{
    clearAll();
    display->setText(tr("Invalid input"));
}

// rightOperand 表示 右操作数
// 对于加减运算, 左操作数是 sumSoFar
// 对于乘除运算, 左操作数是 factorSoFar
bool ScientificCalculator::calculate(double rightOperand, const QString &pendingOperator)
{
    if (pendingOperator == tr("+")) {
        sumSoFar += rightOperand;
    }
    else if (pendingOperator == tr("-")) {
        sumSoFar -= rightOperand;
    }
    else if (pendingOperator == tr("\303\227")) {
        factorSoFar *= rightOperand;
    }
    else if (pendingOperator == tr("\303\267")) {
        if (rightOperand == 0.0) {
            return false;
        }
        factorSoFar /= rightOperand;
    }
    return true;
}

// 响应键盘事件
void ScientificCalculator::keyPressEvent(QKeyEvent *event)
{
    if(mapKeyButton->find((Qt::Key)event->key()) != mapKeyButton->end())
    {
        (*mapKeyButton)[(Qt::Key)event->key()]->click();
    }
}

void ScientificCalculator::buildMapKeyButton()
{
    mapKeyButton = new QMap<Qt::Key, Button*>;

    for(int i=0; i<9; i++)
    {
        (*mapKeyButton)[(Qt::Key)(0x30 + i)] = digitButtons[i];
    }

    (*mapKeyButton)[Qt::Key_Plus] = plusButton;       // 加号
    (*mapKeyButton)[Qt::Key_Minus] = minusButton;     // 减号
    (*mapKeyButton)[Qt::Key_Asterisk] = timesButton;  // 乘号
    (*mapKeyButton)[Qt::Key_Slash] = divisionButton;  // 除号
    (*mapKeyButton)[Qt::Key_Period] = pointButton;    // 小数点
    (*mapKeyButton)[Qt::Key_Equal] = equalButton;     // 等号
    (*mapKeyButton)[Qt::Key_Enter] = equalButton;
    (*mapKeyButton)[Qt::Key_Backspace] = backspaceButton; // 退格键
    (*mapKeyButton)[Qt::Key_Delete] = clearAllButton; // C

}
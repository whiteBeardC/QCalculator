#include "StandardCalculator.h"
#include <QGridLayout>

StandardCalculator::StandardCalculator(QWidget *parent)
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
    // CE
    clearButton = createButton(tr("CE"), SLOT(clear()));
    // C
    clearAllButton = createButton(tr("C"), SLOT(clearAll()));
    // 除号
    divisionButton = createButton(tr("\303\267"), SLOT(multiplicativeOperatorClicked()));
    // 乘号
    timesButton = createButton(tr("\303\227"), SLOT(multiplicativeOperatorClicked()));
    // 减号
    minusButton = createButton(tr("-"), SLOT(additiveOperatorClicked()));
    // 加号
    plusButton = createButton(tr("+"), SLOT(additiveOperatorClicked()));
    // 百分号
    percentButton = createButton(tr("%"), SLOT(unaryOperatorClicked()));
    // 根号
    squareRootButton = createButton(tr("\342\210\232"), SLOT(unaryOperatorClicked()));
    // x^2
    squareButton = createButton(tr("x\302\262"), SLOT(unaryOperatorClicked()));
    // 1/x
    reciprocalButton = createButton(tr("1/x"), SLOT(unaryOperatorClicked()));
    // 等号
    equalButton = createButton(tr("="), SLOT(equalClicked()));

    // Layout
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetDefaultConstraint);

    mainLayout->addWidget(display, 0, 0, 1, 4);  // 显示框

    mainLayout->addWidget(clearMemoryButton, 1, 0);  // MC
    mainLayout->addWidget(readMemoryButton, 1, 1);   // MR
    mainLayout->addWidget(addToMemoryButton, 1, 2);  // M+
    mainLayout->addWidget(setMemoryButton, 1, 3);    // MS

    mainLayout->addWidget(percentButton, 2, 0);    // %
    mainLayout->addWidget(squareRootButton, 2, 1); // 根号
    mainLayout->addWidget(squareButton, 2, 2);     // x^2
    mainLayout->addWidget(reciprocalButton, 2, 3); // 1/x

    mainLayout->addWidget(clearButton, 3, 0);     // CE
    mainLayout->addWidget(clearAllButton, 3, 1);  // C
    mainLayout->addWidget(backspaceButton, 3, 2); // 退格键
    mainLayout->addWidget(divisionButton, 3, 3);  // 除号

    mainLayout->addWidget(timesButton, 4, 3);  // 乘号
    mainLayout->addWidget(minusButton, 5, 3);  // 减号
    mainLayout->addWidget(plusButton, 6, 3);   // 加号
    mainLayout->addWidget(equalButton, 7, 3);  // 等号

    mainLayout->addWidget(changeSignButton, 7, 0); // 正负号
    mainLayout->addWidget(digitButtons[0], 7, 1);  // 数字键 0
    mainLayout->addWidget(pointButton, 7, 2);      // 小数点

    // 数字键 1-9
    for (int i = 1; i < NumDigitButtons; ++i)
    {
        int row = ((9 - i) / 3) + 4;
        int column = (i - 1) % 3;
        mainLayout->addWidget(digitButtons[i], row, column);
    }

    setLayout(mainLayout);
    buildMapKeyButton();
}

StandardCalculator::~StandardCalculator()
{

}

// 创建按钮
Button *StandardCalculator::createButton(const QString &text, const char *member)
{
    Button *button = new Button(text);
    connect(button, SIGNAL(clicked()), this, member);
    return button;
}


void StandardCalculator::digitClicked()
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
void StandardCalculator::pointClicked()
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
void StandardCalculator::changeSignClicked()
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
void StandardCalculator::backspaceClicked()
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
void StandardCalculator::unaryOperatorClicked()
{
    Button *clickedButton = qobject_cast<Button *>(sender());
    QString clickedOperator = clickedButton->text();
    double operand = display->text().toDouble();
    double result = 0.0;

    if (clickedOperator == tr("%"))
    {
        result = operand / 100;
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
    else if (clickedOperator == tr("1/x"))
    {
        if (operand == 0.0)
        {
            divideByZero();
            return;
        }
        result = 1.0 / operand;
    }
    display->setText(QString::number(result));
    waitingForOperand = true;
}

// 点击加减运算符键, 效果: 加减计算
void StandardCalculator::additiveOperatorClicked()
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
void StandardCalculator::multiplicativeOperatorClicked()
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
void StandardCalculator::equalClicked()
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
void StandardCalculator::clear()
{
    if (waitingForOperand)
        return;

    display->setText("0");
    waitingForOperand = true;
}

// 点击C键, 效果: 将计算器重置为初始状态
void StandardCalculator::clearAll()
{
    sumSoFar = 0.0;
    factorSoFar = 0.0;
    pendingAdditiveOperator.clear();
    pendingMultiplicativeOperator.clear();
    display->setText("0");
    waitingForOperand = true;
}

// 点击MC键, 效果: sumInMemory置为 0
void StandardCalculator::clearMemory()
{
    sumInMemory = 0.0;
}

// 点击MR键, 效果: 显示 sumInMemory 的值
void StandardCalculator::readMemory()
{
    display->setText(QString::number(sumInMemory));
    waitingForOperand = true;
}

// 点击MS键, 效果: 进行等号运算之后, 将sumInMemory重置为当前计算结果
void StandardCalculator::setMemory()
{
    equalClicked();
    sumInMemory = display->text().toDouble();
}

// 点击M+键, 效果: 进行等号运算之后, 将sumInMemory重置为计算结果与原sumInMemory之和
void StandardCalculator::addToMemory()
{
    equalClicked();
    sumInMemory += display->text().toDouble();
}

// 除数是 0 时, 显示 "Cannot divide by zero"
void StandardCalculator::divideByZero()
{
    clearAll();
    display->setText(tr("Cannot divide by zero"));
}

// 开根号时, 操作数不能为 0
void StandardCalculator::invalidInput()
{
    clearAll();
    display->setText(tr("Invalid input"));
}

// rightOperand 表示 右操作数
// 对于加减运算, 左操作数是 sumSoFar
// 对于乘除运算, 左操作数是 factorSoFar
bool StandardCalculator::calculate(double rightOperand, const QString &pendingOperator)
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
void StandardCalculator::keyPressEvent(QKeyEvent *event)
{
    if(mapKeyButton->find((Qt::Key)event->key()) != mapKeyButton->end())
    {
        (*mapKeyButton)[(Qt::Key)event->key()]->click();
    }
}

void StandardCalculator::buildMapKeyButton()
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
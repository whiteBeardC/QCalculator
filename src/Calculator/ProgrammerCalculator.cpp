#include "ProgrammerCalculator.h"
#include <QGridLayout>

ProgrammerCalculator::ProgrammerCalculator(QWidget *parent) : QWidget(parent)
{
    // 显示框
    display = new QLineEdit("0");
    display->setReadOnly(true);   // 文本只读
    display->setAlignment(Qt::AlignRight);
    display->setMaxLength(20);
    display->setMinimumHeight(60);
    QFont font = display->font(); // 获取当前的字体样式
    font.setPointSize(font.pointSize() + 15);
    display->setFont(font);

    // 数字键
    for (int i = 0; i < NumDigitButtons; ++i)
    {
        digitButtons[i] = createButton(QString::number(i), SLOT(digitClicked()));
        digitButtons[i]->setFont(QFont("Microsoft YaHei UI", 15, 50));
        digitButtons[i]->setStyleSheet("QToolButton{background-color:rgba(192,192,192,50%)}");
    }
    // 字母键
    letterButtons[0] = createButton(tr("A"), SLOT(digitClicked()));
    letterButtons[1] = createButton(tr("B"), SLOT(digitClicked()));
    letterButtons[2] = createButton(tr("C"), SLOT(digitClicked()));
    letterButtons[3] = createButton(tr("D"), SLOT(digitClicked()));
    letterButtons[4] = createButton(tr("E"), SLOT(digitClicked()));
    letterButtons[5] = createButton(tr("F"), SLOT(digitClicked()));
    // 小数点
    Button *pointButton = createButton(tr("."), SLOT(pointClicked()));
    // 正负号
    Button *changeSignButton = createButton(tr("\302\261"), SLOT(changeSignClicked()));
    // 退格键
    Button *backspaceButton = createButton(tr("\342\214\253"), SLOT(backspaceClicked()));
    // CE Button
    Button *clearButton = createButton(tr("CE"), SLOT(clear()));
    // C Button
    Button *clearAllButton = createButton(tr("C"), SLOT(clearAll()));
    // 除号
    Button *divisionButton = createButton(tr("\303\267"), SLOT(multiplicativeOperatorClicked()));
    // 乘号
    Button *timesButton = createButton(tr("\303\227"), SLOT(multiplicativeOperatorClicked()));
    // 减号
    Button *minusButton = createButton(tr("-"), SLOT(additiveOperatorClicked()));
    // 加号
    Button *plusButton = createButton(tr("+"), SLOT(additiveOperatorClicked()));
    // 左括号
    Button *leftBracketButton = createButton(tr("("), SLOT(leftBracket()));
    // 右括号
    Button *rightBracketButton = createButton(tr(")"), SLOT(rightBracket()));
    // mod
    Button *modButton = createButton(tr("Mod"), SLOT(factorial()));
    // shift
    Button *shiftButton = createButton(tr("\342\206\221"), SLOT(unaryOperatorClicked()));
    // Lsh
    Button *lshButton = createButton(tr("Lsh"), SLOT(unaryOperatorClicked()));
    // Rsh
    Button *rshButton = createButton(tr("Rsh"), SLOT(unaryOperatorClicked()));
    // Or
    Button *orButton = createButton(tr("Or"), SLOT(unaryOperatorClicked()));
    // Xor
    Button *xorButton = createButton(tr("Xor"), SLOT(unaryOperatorClicked()));
    // Not
    Button *notButton = createButton(tr("Not"), SLOT(unaryOperatorClicked()));
    // And
    Button *andButton = createButton(tr("And"), SLOT(unaryOperatorClicked()));
    // 等号
    Button *equalButton = createButton(tr("="), SLOT(equalClicked()));

    // Layout
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetDefaultConstraint);

    mainLayout->addWidget(display, 0, 0, 1, 6);  // 显示框

    mainLayout->addWidget(lshButton, 1, 0);  // Lsh
    mainLayout->addWidget(rshButton, 1, 1);   // Rsh
    mainLayout->addWidget(orButton, 1, 2);  // Or
    mainLayout->addWidget(xorButton, 1, 3);    // Xor
    mainLayout->addWidget(notButton, 1, 4);    // Not
    mainLayout->addWidget(andButton, 1, 5);    // And

    mainLayout->addWidget(shiftButton, 2, 0);     // shift
    mainLayout->addWidget(modButton, 2, 1);       // Mod
    mainLayout->addWidget(clearButton, 2, 2);     // CE
    mainLayout->addWidget(clearAllButton, 2, 3);  // C
    mainLayout->addWidget(backspaceButton, 2, 4); // 退格键
    mainLayout->addWidget(divisionButton, 2, 5);  // 除号

    mainLayout->addWidget(timesButton, 3, 5); // 乘号
    mainLayout->addWidget(minusButton, 4, 5); // 减号
    mainLayout->addWidget(plusButton, 5, 5);  // 加号
    mainLayout->addWidget(equalButton, 6, 5); // 等号

    mainLayout->addWidget(leftBracketButton, 6, 0); // 左括号
    mainLayout->addWidget(rightBracketButton, 6, 1); // 右括号
    mainLayout->addWidget(changeSignButton, 6, 2); // 正负号
    mainLayout->addWidget(digitButtons[0], 6, 3);  // 数字键 0
    mainLayout->addWidget(pointButton, 6, 4);   // 小数点

    // 字母键 A-F
    for (int i = 0; i < NumLetterButtons; ++i)
    {
        int row = (i / 2) + 3;
        int column = i % 2;
        mainLayout->addWidget(letterButtons[i], row, column);
    }
    // 数字键 1-9
    for (int i = 1; i < NumDigitButtons; ++i)
    {
        int row = ((9 - i) / 3) + 3;
        int column = (i - 1) % 3 + 2;
        mainLayout->addWidget(digitButtons[i], row, column);
    }

    setLayout(mainLayout);

}

ProgrammerCalculator::~ProgrammerCalculator()
{

}

Button *ProgrammerCalculator::createButton(const QString &text, const char *member)
{
    Button *button = new Button(text);
    connect(button, SIGNAL(clicked()), this, member);
    return button;
}

void ProgrammerCalculator::digitClicked()
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
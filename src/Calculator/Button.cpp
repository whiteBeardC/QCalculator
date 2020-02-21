#include "Button.h"

Button::Button(const QString &text, QWidget *parent)
    : QToolButton(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred); // 设置布局样式
    setText(text);   // 设置按钮显示的文本
    setFont(QFont("Microsoft YaHei UI", 12, 40));  // 设置文本格式
}

QSize Button::sizeHint() const
{
    QSize size = QToolButton::sizeHint();
    size.rheight() += 20;
    size.rwidth() = qMax(size.width(), size.height());
    return size;
}

#include "leetcode.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStack>
#include "ElaText.h"
#include "ElaPlainTextEdit.h"
#include "ElaToolButton.h"
#include "ElaComboBox.h"
leetcode::leetcode(QWidget *parent)
    :BasePage(parent){
    createCustomWidget("在输入框内输入算法输入数据\n键入实现的算法后,实现输出");
}

leetcode::~leetcode()
{
    delete _comboBox;
    delete _outputedit;
    delete _inputedit;
}

void leetcode::createCustomWidget(QString desText)
{
    /*--------------------内容生产--------------------*/

    QWidget* customWidget = new QWidget(this);
    ElaText* descText = BasePage::setElaText(desText,customWidget,13);
    ElaText* TitleText = BasePage::setElaText("LEETCODETEST",customWidget,35,true);
    _inputedit = new ElaPlainTextEdit(this);
    _outputedit = new ElaPlainTextEdit(this);
    ElaToolButton* onbutton = BasePage::setElaTool("",40,ElaIconType::ArrowRightLong,this,false);
    onbutton->setToolButtonStyle(Qt::ToolButtonIconOnly);
    onbutton->setFixedWidth(90);
    _outputedit->setReadOnly(true);
    _comboBox = new ElaComboBox(this);
    QStringList comboList{
        "括号匹配",
        "插入字符串",
        "两数之和",
    };
    _comboBox->addItems(comboList);
    _comboBox->setFixedSize(QSize(350,50));
    _comboBox->setBorderRadius(3);
    /*--------------------布局设置--------------------*/
    QHBoxLayout* viewLayout = new QHBoxLayout();
    viewLayout->setContentsMargins(0, 0, 12, 0);
    viewLayout->addWidget(_inputedit);
    viewLayout->addWidget(onbutton);
    viewLayout->addWidget(_outputedit);
    QHBoxLayout* comboxLayout = new QHBoxLayout();
    comboxLayout->addSpacing(200);
    comboxLayout->addWidget(_comboBox);
    comboxLayout->addStretch();
    QVBoxLayout* centerVLayout = new QVBoxLayout(customWidget);
    centerVLayout->setContentsMargins(0, 0, 0, 0);
    centerVLayout->addWidget(TitleText);
    centerVLayout->addSpacing(20);
    centerVLayout->addWidget(descText);
    centerVLayout->addLayout(viewLayout);
    centerVLayout->addLayout(comboxLayout);
    centerVLayout->addStretch();

    setCustomWidget(customWidget);

/*--------------------功能实现--------------------*/
    /** 绑定信号*/
    connect(onbutton, &ElaToolButton::clicked, this, &leetcode::onButtonClicked);
    connect(_comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &leetcode::onComboBoxChanged);
}
void leetcode::onButtonClicked() {
    QString input = _inputedit->toPlainText().trimmed();
    QString selected = _comboBox->currentText(); // 获取选中项
    QString result;
/** 在此添加新函数*/
    if (selected == "括号匹配") {
        result = handleBracketMatching(input);
    } else if (selected == "插入字符串") {
        result = handleInsertString(input);
    } else if (selected == "两数之和") {
        result = handleTwoSum(input);
    }
    else {
        result = "错误：未定义的算法选项";
    }
    _outputedit->setPlainText(result);
    _outputedit->update();
}
/** 提示输入逻辑*/
void leetcode::onComboBoxChanged(int index) {
    QString hint;
    switch(index) {
    case 0: hint = "请输入包含括号的字符串（如: {[()]}）"; break;
    case 1: hint = "格式：原字符串,插入位置,插入内容（如: abc,1,xyz）"; break;
    case 2: hint = "格式：数组元素空格分隔 + 目标值（如: 2 7 11 15=9）"; break;
    }
    _inputedit->setPlaceholderText(hint);
    _inputedit->update();
}
/** 括号匹配*/
QString leetcode::handleBracketMatching(const QString& input) {
    QStack<QChar> stack;
    const QHash<QChar, QChar> pairs = {{')','('}, {']','['}, {'}','{'},};
    const QSet<QChar> leftBraces = {'(', '[', '{'};
    for (const QChar& c : input) {
        if (leftBraces.contains(c)) {
            stack.push(c);
        } else if (pairs.contains(c)) {
            if (stack.isEmpty() || stack.pop() != pairs[c]) {
                return "错误：括号不匹配";
            }
        }
    }
    return stack.isEmpty() ? "成功：括号完全匹配" : "错误：存在未闭合括号";
}
/** 字符串插入*/
QString leetcode::handleInsertString(const QString& input) {
    QStringList parts = input.split(',');
    if(parts.length()!=3){
        return "请按照'输入字符串','要插入的位置','插入字符串'标准输入";
    }
    bool ok;
    int pos = parts[1].toInt(&ok);
    if(!ok||pos<0||pos>input.length()){
        return "错误：无效的插入位置";
    }
    return parts[0].left(pos)+parts[2]+parts[0].mid(pos);
}
/** 两数之和*/
QString leetcode::handleTwoSum(const QString& input) {
    QStringList parts = input.split('=');
    if (parts.size() != 2) return "错误：输入格式应为 '数组元素空格分隔=目标值'";

    QStringList nums = parts[0].split(' ', Qt::SkipEmptyParts);
    bool ok;
    int target = parts[1].toInt(&ok);
    if (!ok) return "错误：无效的目标值";

    QHash<int, int> hash;
    for (int i = 0; i < nums.size(); ++i) {
        int num = nums[i].toInt(&ok);
        if (!ok) return "错误：非数字元素";

        if (hash.contains(target - num)) {
            return QString("索引: [%1, %2]").arg(hash[target - num]).arg(i);
        }
        hash[num] = i;
    }
    return "未找到符合条件的两个数";
}










#ifndef LEETCODE_H
#define LEETCODE_H
#include "basepage.h"
class ElaComboBox;
class ElaPlainTextEdit;
class leetcode: public BasePage
{
    Q_OBJECT
public:
    explicit leetcode(QWidget * parent = nullptr);
    ~leetcode();

protected:
    void createCustomWidget(QString desText);
private slots:
    void onButtonClicked(); // 按钮点击槽函数
    void onComboBoxChanged(int index); // 下拉框选项变化槽函数（可选）

private:
    ElaComboBox* _comboBox{nullptr};
    ElaPlainTextEdit* _inputedit{nullptr};
    ElaPlainTextEdit* _outputedit{nullptr};
    // 算法处理函数
    QString handleBracketMatching(const QString& input);
    QString handleInsertString(const QString& input);
    QString handleTwoSum(const QString& input);

};

#endif // LEETCODE_H

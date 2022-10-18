#ifndef EXPR_EVAL_H
#define EXPR_EVAL_H
#include <QString>
#include <vector>
#include <stack>
#include <QStringList>
#include <exception>

class div_zero : public std::invalid_argument {
public:
    using std::invalid_argument::invalid_argument;
};

class bracket_not_match : public std::logic_error {
public:
    using std::logic_error::logic_error;
    bracket_not_match();
};

class no_operand : public std::logic_error {
public:
    using std::logic_error::logic_error;
};
//括号是否匹配，适用于token_list
bool bracket_match(const QStringList & token_list);
//括号是否匹配，适用于expr: QString
bool bracket_match(const QString & expr);

// 中缀表达式转换为后缀表达式
QString infix_to_postfix(const QString & expr);

// 中缀表达式转换为后缀表达式辅助
QStringList infix_to_postfix_helper(const QStringList & str_list);

// 后缀表达式求值辅助
template <typename T>
T postfix_eval_helper(const QStringList & expr)
{
    const static QString key_words("+-*/()x");
    std::stack<T> num_stack;
    for(const auto & str : expr) {
        if(key_words.contains(str)) {
            auto right_num = num_stack.top();
            num_stack.pop();
            auto left_num = num_stack.top();
            num_stack.pop();
            auto ans = do_cacl(left_num,right_num,str);
            num_stack.push(ans);
        } else {
            if(typeid(T).name() == typeid(int).name()) {
                num_stack.push(str.toInt());
            } else {
                num_stack.push(str.toDouble());
            }
        }
    }
    return num_stack.top();
}

// 将表达式拆分为QStringList
QStringList split_expr(const QString & expr);


//判断表达式类型
bool judged_expr_type(const QString & expr);

// 后缀表达式求值
template <typename T>
T postfix_eval(const QString & expr)
{
    auto token_list = expr.split(' ');
    bool float_eval = false;
    //如果表达式中有'.' 或者 '/'认为这个表达式的结果是一个浮点值
    //否则，认为结果是一个整数值
    if(judged_expr_type(expr) == true) {
        return postfix_eval_helper<double>(token_list);
    } else {
        return postfix_eval_helper<int>(token_list);
    }
}

//中缀表达式求值
template <typename T>
T infix_eval(const QString & expr) {
    if(!bracket_match(expr)) {
        throw bracket_not_match();
    }
    auto token_list = split_expr(expr);
    return postfix_eval_helper<T>(infix_to_postfix_helper(token_list));
}


//是否是运算符
inline bool is_op(const QString & str);

//最具体的运算
template <typename T>
T do_cacl(const T & left_num, const T & right_num, const QString & op) {
    if(op == '+') {
        return left_num + right_num;
    } else if (op == '-') {
        return left_num - right_num;
    } else if (op == '*' || op == "x") {
        return left_num * right_num;
    } else {
        if(right_num == 0) {
            throw div_zero("Error: " + QString::number(left_num).toStdString() + "/0");
        } else {
            return left_num / right_num;
        }
    }
}

bool is_postfix(const QString & expr);
bool is_postfix_helper(const QStringList & token_list);
bool token_is_num(const QString & token);


#endif // EXPR_EVAL_H

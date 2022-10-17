#include "expr_eval.h"
#include <stack>
#include <QMap>
#include <algorithm>

//字符串的形式返回，用空格分开
QString infix_to_postfix(const QString &expr)
{
    auto str_list = split_expr(expr);
    return infix_to_postfix_helper(str_list).join(" ");
}



QStringList split_expr(const QString & expr)
{
    const static QString key_words("+-*/()x");
    QString trim_expr = expr;
    trim_expr.removeIf([](QChar ch){ return ch == ' '; });
    QStringList ret;
    QString tmp;
    bool pre_is_op = false;
    int i = 0;
    for(const auto & ch : trim_expr) {
        if(key_words.contains(ch)) {
            //如果第一个字符是符号位，则其是负数的一部分
            if(i == 0 && (ch == '+' || ch == '-')) {
                tmp.push_back(ch);
                //如果前一个char是key_words其中的一个但不是'('，则当前char是数字的符号
            } else if (i >= 1
                       && (key_words.contains(trim_expr[i-1]) && expr[i-1] != ')')
                       && is_op(QString(ch))) {
                tmp.push_back(ch);
            } else {
                if (!tmp.isEmpty()) {
                    ret.push_back(tmp);
                    tmp.clear();
                }
                ret.push_back(ch);
            }
        } else {
            tmp.push_back(ch);
        }
        ++i;
    }
    if (!tmp.isEmpty()) {
        ret.push_back(tmp);
    }
    return ret;
}

QStringList infix_to_postfix_helper(const QStringList &str_list)
{
    const static QString key_words("+-*/()x");
    const static QMap<QString,int> prec {
        {"(",1}, {")",1},
        {"+",2}, {"-",2},
        {"*",3}, {"x",3}, {"/",3},
    };
    std::stack<QString> op_stack;
    QStringList postfix;
    for(const auto & item : str_list) {
       //是操作数,push_back postfix
       if(!key_words.contains(item)) {
           postfix.push_back(item);
       } else if (item == "(") {
           op_stack.push(item);
       } else if (item == ")") {
           while (op_stack.top() != "(") {
               postfix.push_back(op_stack.top());
               op_stack.pop();
           }
           op_stack.pop();
       //如果是运算符
       } else if (is_op(item)) {
           while(!op_stack.empty() &&
                 prec[op_stack.top()] >= prec[item]) {
                postfix.push_back(op_stack.top());
                op_stack.pop();
           }
           op_stack.push(item);
       }
    }
    while(!op_stack.empty()) {
        postfix.push_back(op_stack.top());
        op_stack.pop();
    }
    return postfix;
}


bool is_op(const QString &str)
{
    return str == "+"
            || str == "-"
            || str == "*"
            || str == "x"
            || str == "/";
}


bool judged_expr_type(const QString &expr)
{
    bool float_eval = false;
    for(const auto & ch : expr) {
        if(ch == '.' || ch == '/') {
            float_eval = true;
            break;
        }
    }
    return float_eval;
}

//将expr以空格拆分为QStringList
bool is_postfix(const QString &expr)
{
    auto token_list = expr.split(' ');
    return is_postfix_helper(token_list);
}

//传入的token_list应当是合法的，务必在调用这个函数前检查合法性
//如果传入的token_list不合法，这是一个未定义的行为
//如果前两个token均为数字，则这个表达式被认定为后序列表达式
//如果仅仅有一个token, 也被认定为一个后缀表达式
bool is_postfix_helper(const QStringList &token_list)
{
    if(token_list.size() == 1) {
        if(token_is_num(token_list[0])) {
            return true;
        } else {
            return false;
        }
    }
    auto t1 = token_list[0];
    auto t2 = token_list[1];
    if(token_is_num(t1) && token_is_num(t2)) {
        return true;
    }
    return false;
}

bool token_is_num(const QString &token)
{
    for(int i=0;i<token.size();++i) {
        if(token[i] < '0' || token[i] > '9') {
            if((i==0 && (token[i] == '+' || token[i] == '-'))
                    || (token[i] == '.')) {
                continue;
            }
            return false;
        }
    }
    return true;
}

bool bracket_match(const QString &expr)
{
    std::stack<QChar> s;
    for(const auto & ch : expr) {
        if(ch == '(') {
            s.push(ch);
        } else if(ch == ')') {
            s.pop();
        }
    }
    return s.empty();
}

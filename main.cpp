#include "mainwindow.h"
#include <iostream>
#include <QApplication>
#include "expr_eval.h"
using std::cout;
using std::endl;

/*OK*/
void test_infix_to_postfix() {
    QString s1("1 + 2 * 3");
    auto ans = infix_to_postfix(s1);
    cout << ans.toStdString() << endl;

    QString s2("111 + 222 + 333 * 444");
    ans = infix_to_postfix(s2);
    cout << ans.toStdString() << endl;

    QString s3("2*3+1");
    ans = infix_to_postfix(s3);
    cout << ans.toStdString() << endl;

    //float
    QString s4("3.14+1.56*3+53");
    ans = infix_to_postfix(s4);
    cout << ans.toStdString() << endl;

    //()
    QString s5("(1+2)*(3*4)");
    ans = infix_to_postfix(s5);
    cout << ans.toStdString() << endl;

    QString s6("(1+2)*(3+4)");
    ans = infix_to_postfix(s6);
    cout << ans.toStdString() << endl;

    QString s7("(1+2)x(3+4)");
    ans = infix_to_postfix(s7);
    cout << ans.toStdString() << endl;

    QString s8 = "(-1*-2)+(3*-4)*(-1+66)";
    ans = infix_to_postfix(s8);
    cout << ans.toStdString() << endl;
}

/*OK*/
void test_infix_eval() {
    assert(infix_eval<int>("1+2") == 3);
    assert(infix_eval<double>("3.14*100+3.55+0.05*10") == 318.05);
    assert(infix_eval<int>("-1+1") == 0);
    cout << "test_postfix_eval: OK!" << endl;
}

/*OK*/
void test_split_expr() {
    QString s1 ("1+2*3");
    for(auto & str : split_expr(s1)) {
        cout << str.toStdString() << " ";
    }
    cout  << endl;
    QString s2("-1*-2+3*-4");
    for(auto & str : split_expr(s2)) {
        cout << str.toStdString() << " ";
    }

    cout << endl;
    s2 = "(-1*-2)+(3*-4)*(-1+66)";
    for(auto & str : split_expr(s2)) {
        cout << str.toStdString() << " ";
    }

}

void test_token_is_num() {
    assert(token_is_num("11"));
    assert(token_is_num("+11"));
    assert(token_is_num("-1201"));
    assert(!token_is_num("(1+2)"));
    cout << "test_token_is_num: OK" << endl;

}
void test_is_postfix() {
    assert(is_postfix("11 22 +"));
//    assert(!is_postfix("1+2*3"));
    cout << "test_is_postfix: OK" << endl;
}

void test_bracket_match() {
    assert(bracket_match("(1+2)"));
    assert(!bracket_match("(1+2)("));
    assert(!bracket_match("())"));
    assert(!bracket_match("(()"));
    assert(bracket_match("(1+2)*(3+4)"));
    assert(bracket_match("(1+2)*(3*5+6)+()"));
    cout << "test_bracket_match: OK" << endl;
}
void test_all() {
    test_infix_eval();
    test_bracket_match();
}
//#define DEBUG
int main(int argc, char *argv[])
{
#if !defined(DEBUG)
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
#endif
#if defined(DEBUG)
//    test_infix_to_postfix();
//    test_split_expr();
    test_all();
#endif
    return 0;

}

#include "mainwindow.h"
#include "expr_eval.h"
#include "./ui_mainwindow.h"
#include <algorithm>
#include <QDebug>
#include <QKeyEvent>

const QString DEFALUT_FONT = "JetBrains Mono";

#define NUM_BUTTON_CLICKED(X) 				\
void MainWindow::on_num_##X##_clicked() { 	\
    auto s = ui->num_line->text(); 			\
    s.push_back(""#X"");					\
    ui->num_line->setText(s); 				\
}

#define CHAR_BUTTON_CLICKED(NAME,OP) 		\
void MainWindow::on_##NAME##_clicked() { 	\
    auto s = ui->num_line->text(); 			\
    s.push_back(""#OP""); 					\
    ui->num_line->setText(s); 				\
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    set_keybord_font(DEFALUT_FONT);
    ui->statusbar->showMessage("Welcome To Cacl!");
    ui->num_line->setAlignment(Qt::AlignRight);
    ui->num_line->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//按下Enter键, 计算中缀/后缀表达式
//可以自动识别表达式是中序表达式或者后序表达式
//后序表达式注意在两个数之间使用空格代替
void MainWindow::on_enter_clicked()
{
    auto expr = ui->num_line->text();
    if(expr.isEmpty()) {
        return;
    }
    auto p = judged_expr_type(expr);
    //结果为浮点数
    if(p) {
        qDebug() << "into p";
        double ans;
        if(is_postfix(expr)) {
            qDebug() << "into is_postfix";
            ans = postfix_eval<double>(expr);
        } else {
            qDebug() << "into not is_postfix";
            ans = infix_eval<double>(expr);
        }
        ui->num_line->setText(QString::number(ans));
    //结果为整数
    } else {
        qDebug() << "into not p";
        int ans;
        if(is_postfix(expr)) {
            qDebug() << "into is_postfix";
            ans = postfix_eval<int>(expr);
        } else {
            qDebug() << "into not is_postfix";
            ans = infix_eval<int>(expr);
        }
        ui->num_line->setText(QString::number(ans));
    }
    //状态栏显示被计算的表达式
    ui->statusbar->showMessage("expression: " + expr);
    //将表达式录入到历史记录中
    m_history.push_back(expr);
    m_his_idx = m_history.size()-1;
}


//清空文本
void MainWindow::on_clear_clicked()
{
    ui->num_line->clear();
}


//对得到的结果向下取整
void MainWindow::on_round_down_clicked()
{
    auto token_list = split_expr(ui->num_line->text());
    if(token_list.length() == 1) {
        int ans = token_list[0].toDouble();
        ui->num_line->setText(QString::number(ans));
    }
}

void MainWindow::on_open_paren_clicked()
{
    auto s = ui->num_line->text();
    s.push_back("(");
    ui->num_line->setText(s);
}


void MainWindow::on_close_paren_clicked()
{
    auto s = ui->num_line->text();
    s.push_back(")");
    ui->num_line->setText(s);
}

void MainWindow::set_keybord_font(const QString &font)
{
    for(int i=0;i<ui->keybord_layout->count();++i) {
        auto item = ui->keybord_layout->itemAt(i);
        QPushButton * button = qobject_cast<QPushButton*>(item->widget());
        button->setFont(QFont(font,16));
    }
}


//重写键盘按压事件
//按压按键，就相当与按压对应的按钮
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    auto key = event->key();
    if(event->modifiers() == Qt::ControlModifier) {
        //Ctrl + C 清除
        if(key == Qt::Key_C) {
            ui->clear->click();
        }
        //Ctrl + D 向下取整
        else if(key == Qt::Key_D) {
            ui->round_down->click();
        //Ctrl + A 设置num_line的可编辑性
        } else if(key == Qt::Key_A) {
            ui->action_edit_able_enable->trigger();
        //Ctrl + T 转为后缀表达式
        } else if (key == Qt::Key_T) {
            ui->action_to_postfix->trigger();
        }
    } else if (event->modifiers() == Qt::ShiftModifier) {
        //"(" and ")"
        //TODO
    } else {
        if(key == Qt::Key_0) {
            ui->num_0->click();
        } else if (key == Qt::Key_1) {
            ui->num_1->click();
        } else if (key == Qt::Key_2) {
            ui->num_2->click();
        } else if (key == Qt::Key_3) {
            ui->num_3->click();
        } else if (key == Qt::Key_4) {
            ui->num_4->click();
        } else if (key == Qt::Key_5) {
            ui->num_5->click();
        } else if (key == Qt::Key_6) {
            ui->num_6->click();
        } else if (key == Qt::Key_7) {
            ui->num_7->click();
        } else if (key == Qt::Key_8) {
            ui->num_8->click();
        } else if (key == Qt::Key_9) {
            ui->num_9->click();
        } else if (key == Qt::Key_Plus) {
            ui->add->click();
        } else if (key == Qt::Key_Minus) {
            ui->sub->click();
        } else if (key == Qt::Key_Asterisk) {
            ui->mul->click();
        } else if (key == Qt::Key_Slash) {
            ui->div->click();
        } else if (key == Qt::Key_Enter) {
            ui->enter->click();
        } else if (key == Qt::Key_Backspace) {
            auto s = ui->num_line->text();
            s = s.left(s.size()-1);
            ui->num_line->setText(s);
        } else if (key == Qt::Key_ParenRight) {
            qDebug() << "press (";
        } else if (key == Qt::Key_K) {
            if(!m_history.empty()) {
                m_his_idx--;
                if(m_his_idx < 0) {
                    m_his_idx = m_history.size()-1;
                }
                ui->num_line->setText(m_history[m_his_idx]);
            }
        } else if (key == Qt::Key_J) {
            if(!m_history.empty()) {
                m_his_idx++;
                if(m_his_idx == m_history.size()) {
                    m_his_idx = 0;
                }
                ui->num_line->setText(m_history[m_his_idx]);
            }
        }
    }
    QMainWindow::keyPressEvent(event);
}


void MainWindow::on_action_to_postfix_triggered()
{
    auto expr = ui->num_line->text();
    m_history.push_back(expr);
    ++m_his_idx;
    if(!is_postfix(expr)) {
        auto postfix = infix_to_postfix(expr);
        ui->num_line->setText(postfix);
    }
}



void MainWindow::on_action_edit_able_enable_triggered()
{
    ui->num_line->setEnabled(!ui->num_line->isEnabled());
}


void MainWindow::on_action_his_clear_triggered()
{
    m_history.clear();
    m_his_idx = 0;
}

void MainWindow::on_action_his_union_triggered()
{
    QStringList new_history;
    new_history.reserve(m_history.size());
    for(const auto & his : m_history) {
        if(!new_history.contains(his)) {
            new_history.emplaceBack(std::move(his));
        }
    }
    m_history = std::move(new_history);
    m_his_idx = m_history.size()-1;
}


//新建一个窗口列出所有的历史记录
void MainWindow::on_action_history_triggered()
{

}


/*--------------------------*/
CHAR_BUTTON_CLICKED(dot,.)
CHAR_BUTTON_CLICKED(add,+)
CHAR_BUTTON_CLICKED(sub,-)
CHAR_BUTTON_CLICKED(mul,*)
CHAR_BUTTON_CLICKED(div,/)

NUM_BUTTON_CLICKED(1)
NUM_BUTTON_CLICKED(2)
NUM_BUTTON_CLICKED(3)
NUM_BUTTON_CLICKED(4)
NUM_BUTTON_CLICKED(5)
NUM_BUTTON_CLICKED(6)
NUM_BUTTON_CLICKED(7)
NUM_BUTTON_CLICKED(8)
NUM_BUTTON_CLICKED(9)
NUM_BUTTON_CLICKED(0)




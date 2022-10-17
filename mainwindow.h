#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_num_1_clicked();
    void on_num_2_clicked();
    void on_num_3_clicked();
    void on_num_4_clicked();
    void on_num_5_clicked();
    void on_num_6_clicked();
    void on_num_7_clicked();
    void on_num_8_clicked();
    void on_num_9_clicked();
    void on_num_0_clicked();
    void on_dot_clicked();
    void on_add_clicked();
    void on_sub_clicked();
    void on_mul_clicked();
    void on_div_clicked();
    void on_enter_clicked();

    void on_clear_clicked();

    void on_round_down_clicked();

    void on_open_paren_clicked();

    void on_close_paren_clicked();
    void set_keybord_font(const QString & font);
    void on_action_to_postfix_triggered();

    void on_action_edit_able_enable_triggered();

private:
    Ui::MainWindow *ui;
    QStringList m_history;
    int m_his_idx = 0;
    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *event) override;
};
#endif // MAINWINDOW_H

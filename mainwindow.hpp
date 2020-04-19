#ifndef PSKR_MAINWINDOW_HPP_INCLUDED
#define PSKR_MAINWINDOW_HPP_INCLUDED

#include <QWidget>
#include <QPainter>
#include <QVector>
#include <QLabel>
#include <algorithm>
#include <random>
#include <utility>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QCoreApplication>
#include <QAbstractAnimation>

class MyLabel : public QLabel{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor)
 public:
    MyLabel(QWidget *parent = 0):QLabel(parent){}
    MyLabel(QString str,QWidget *parent = 0):QLabel(parent){
        setText(str);
    }
      void setColor (QColor color){
        setStyleSheet("background-color: " + QString("rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue()));
      }
      QColor color() const{
        return Qt::black;
      }

      QColor getBackgroundColor() const{
          return palette().color(QWidget::backgroundRole());

      }


};


class MainWindow : public QWidget
{
    Q_OBJECT
    QVector<MyLabel*> left;
    QVector<MyLabel*> right;
    QPlainTextEdit* input;
    QPlainTextEdit* output;
    QPushButton* input_text;
public:
    MainWindow();
    void paintEvent(QPaintEvent* );
    void resizeEvent(QResizeEvent* );
public slots:
    void text_entered();
};


#endif // PSKR_MAINWINDOW_HPP_INCLUDED

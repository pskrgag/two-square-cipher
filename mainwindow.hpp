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
#include <iostream>
#include <QRadioButton>



#define FIRST_OPEN QColor(0,255,0)
#define SECOND_OPEN QColor(255,0,0)
#define FIRST_CIPHER QColor(7,59,7)
#define SECOND_CIPHER QColor(79,11,11)

#define START_GRAD "rgb(0,255,0)"
#define END_GRAD "rgb(255,0,0)"




class MyLabel : public QLabel{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor)
    Q_PROPERTY(int palete READ pallete WRITE setPallete)

    QString first = QString("* {background: qlineargradient( x1:0 y1:0, x2:1 y2:0, stop:0 ") + QString(START_GRAD) + ", stop:1 " + END_GRAD + ");}";
    QString def = "background-color:#f0f0f0";
    QString second = QString("* {background: qlineargradient( x1:0 y1:0, x2:1 y2:0, stop:0 ") + QString(END_GRAD) + " , stop:1 " + START_GRAD + ");}";

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
      int pallete() const{
          return 0;
      }

      void setPallete(int i){
          if(i == 0) return;
          switch (i) {
          case -1:
                setStyleSheet(def);
              break;
          case 1:
                setStyleSheet(first);
              break;
          case -100:
              setStyleSheet(second);
              break;
          }

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
    QPushButton* rand_but;
    QRadioButton* but1;
    QRadioButton* but2;
public:
    MainWindow();
    void paintEvent(QPaintEvent* );
    void resizeEvent(QResizeEvent* );
public slots:
    void text_entered();
    void sq_shuffle();
};


#endif // PSKR_MAINWINDOW_HPP_INCLUDED

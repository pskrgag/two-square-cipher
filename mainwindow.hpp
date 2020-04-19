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

class MyLabel : public QLabel{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor)
    Q_PROPERTY(int palete READ pallete WRITE setPallete)

    QString first = "* {background: qlineargradient( x1:0 y1:0, x2:1 y2:0, stop:0 rgb(0,255,0), stop:1 rgb(255,0,0));}";
    QString def = "background-color:#f0f0f0";
    QString second = "* {background: qlineargradient( x1:0 y1:0, x2:1 y2:0, stop:0 rgb(255,0,0), stop:1 rgb(0,255,0));}";

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
public:
    MainWindow();
    void paintEvent(QPaintEvent* );
    void resizeEvent(QResizeEvent* );
public slots:
    void text_entered();
};


#endif // PSKR_MAINWINDOW_HPP_INCLUDED

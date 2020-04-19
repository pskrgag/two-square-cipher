#include "mainwindow.hpp"

#include <iostream>


static std::pair<int, int> find_symbol(const QVector<MyLabel*> vec, char symbol){
    if(symbol > 96 && symbol < 123) symbol -= 32;
    for(int i = 0; i < vec.size(); ++i){
        if(vec[i]->text() == QString("   " + QString(symbol))) return {i/6, i%6};
    }
    return {-1, -1};

}

MainWindow::MainWindow():
    left(36),
    right(36),
    input(new QPlainTextEdit(this)),
    input_text(new QPushButton("Enter text", this)),
    output(new QPlainTextEdit(this))
{
    output->setReadOnly(true);
    connect(input_text, &QPushButton::clicked, this, &MainWindow::text_entered);
    resize(639, 478);
    setMinimumSize(639, 566);
    for(int i = 0; i < 36; ++i){
        if(i < 26){
            left[i] = new MyLabel(QString("   " + QString(i + 65)) , this);
            right[i] = new MyLabel(QString("   " + QString(i + 65)) , this);
        }
        else if(i != 35){
            right[i] = new MyLabel("   " +QString::number(i - 26) , this);
            left[i] = new MyLabel("   " + QString::number(i - 26) , this);
        }
        else{
            right[i] = new MyLabel("    " , this);
            left[i] = new MyLabel("    ", this);
        }
    }
    std::shuffle(left.begin(), left.end(), std::mt19937(std::random_device()()));
    std::shuffle(right.begin(), right.end(), std::mt19937(std::random_device()()));

}
void MainWindow::resizeEvent(QResizeEvent *){
    std::cout << find_symbol(right, 'k').first << find_symbol(right, 'K').second << std::endl;
}
void MainWindow::paintEvent(QPaintEvent *){
    input->move(0 ,height()*4/5);
    input_text->move(input->width() + 25, height()*8/9);
    output->move(width() - output->width(), height()*4/5);
    QPainter paint(this);
    QBrush brush(Qt::black);
    QPen pen(brush, 3);
    QFont font("Times", 15);
    paint.setPen(pen);
    static const std::size_t square_size = height()/5;
    static const std::size_t mini_square_size = height()/15;
    for(int i = 0; i < 6; ++i){
        for(int j = 0; j < 6; ++j){
            paint.drawRect(width()/7 + j*mini_square_size, square_size + mini_square_size*i, mini_square_size, mini_square_size);
            left[6*i + j]->setGeometry(width()/7 + j*mini_square_size + 1.5, square_size + mini_square_size*i +1.5 , mini_square_size - 2.5, mini_square_size - 2.5);
            left[6*i + j]->show();
            paint.drawRect(width()/7 + square_size + width() / 5 + j*mini_square_size, square_size + mini_square_size*i, mini_square_size, mini_square_size);
            right[6*i + j]->setGeometry(width()/7 + square_size + width() / 5 + j*mini_square_size + 1.5,  square_size + mini_square_size*i + 1.5, mini_square_size - 2.5, mini_square_size - 2.5);
            right[6*i + j]->show();
        }

    }

}

void MainWindow::text_entered(){
    if(input->toPlainText().size()%2) QMessageBox::critical(this, "Error", "Text size should be even");
    else{
        output->clear();

        QColor def = left[0]->palette().color(QWidget::backgroundRole());
        std::string text = input->toPlainText().toStdString();
        input->setReadOnly(true);
        input_text->setDisabled(true);
        for(int i = 0; i < text.size(); i+=2){
               char first = text[i];
               char second = text[i+1];
               auto first_pos = find_symbol(left, first);
               auto second_pos = find_symbol(right, second);
               if(second_pos.first == -1 || first_pos.first == -1){
                   QMessageBox::critical(this, "Error", "You cannot cypher this symbol " +
                                                  QString(first_pos.first == -1?first:second));
                   input->setReadOnly(false);
                   input_text->setDisabled(false);
                   return;
               }
               if(first_pos.first != second_pos.first){

                    QPropertyAnimation* left_anim1 = new QPropertyAnimation(left[first_pos.first*6 + first_pos.second], "color", this);
                    left_anim1->setStartValue(QColor(0,255,0));
                    left_anim1->setEndValue(def);
                    left_anim1->setDuration(12000);
                    left_anim1->start(QAbstractAnimation::DeleteWhenStopped); //first open text
                    left_anim1->setEasingCurve(QEasingCurve::InElastic);
                    {
                        auto start = std::chrono::system_clock::now();
                        auto end = std::chrono::system_clock::now();
                        while((std::chrono::duration_cast<std::chrono::seconds>(end - start).count() != 3)){
                                end = std::chrono::system_clock::now();
                                QCoreApplication::processEvents();
                        }

                    }
                    QPropertyAnimation* right_anim1 = new QPropertyAnimation(right[second_pos.first*6 + second_pos.second], "color", this);
                    right_anim1->setStartValue(QColor(255,0,0));
                    right_anim1->setEndValue(def);
                    right_anim1->setDuration(9000);
                    right_anim1->start(QAbstractAnimation::DeleteWhenStopped); //second open text
                    right_anim1->setEasingCurve(QEasingCurve::InElastic);
                    {
                        auto start = std::chrono::system_clock::now();
                        auto end = std::chrono::system_clock::now();
                        while((std::chrono::duration_cast<std::chrono::seconds>(end - start).count() != 3)){
                                end = std::chrono::system_clock::now();
                                QCoreApplication::processEvents();
                        }

                    }

                   // while (right_anim1->state() != QAbstractAnimation::Stopped) QCoreApplication::processEvents();

                    QPropertyAnimation* right_anim = new QPropertyAnimation(right[first_pos.first*6 + second_pos.second], "color", this);
                    right_anim->setStartValue(QColor(7,59,7));
                    right_anim->setEndValue(def);
                    right_anim->setDuration(6000);
                    right_anim->start(QAbstractAnimation::DeleteWhenStopped); //first cipher text
                    right_anim->setEasingCurve(QEasingCurve::InElastic);
                    {
                        auto start = std::chrono::system_clock::now();
                        auto end = std::chrono::system_clock::now();
                        while((std::chrono::duration_cast<std::chrono::seconds>(end - start).count() != 3)){
                                end = std::chrono::system_clock::now();
                                QCoreApplication::processEvents();
                        }

                    }


                   // while (right_anim->state() != QAbstractAnimation::Stopped) QCoreApplication::processEvents();
                    output->insertPlainText(right[first_pos.first*6 + second_pos.second]->text().at(3));

                    QPropertyAnimation* left_anim = new QPropertyAnimation(left[second_pos.first*6 + first_pos.second], "color", this);
                    left_anim->setStartValue(QColor(79,11,11));
                    left_anim->setEndValue(def);
                    left_anim->setDuration(3000);
                    left_anim->start(QAbstractAnimation::DeleteWhenStopped); //second cipher text
                    left_anim->setEasingCurve(QEasingCurve::InElastic);

                    while (left_anim->state() != QAbstractAnimation::Stopped) QCoreApplication::processEvents();
                    output->insertPlainText(left[second_pos.first*6 + first_pos.second]->text().at(3));





               }

               else if (first_pos.first == second_pos.first && first_pos.second != second_pos.second){
                   QPropertyAnimation* left_anim = new QPropertyAnimation(left[second_pos.first*6 + first_pos.second], "color", this);
                   left_anim->setStartValue(QColor(0,255,0));
                   left_anim->setEndValue(def);
                   left_anim->setDuration(12000);
                   left_anim->start(QAbstractAnimation::DeleteWhenStopped); //first open
                   left_anim->setEasingCurve(QEasingCurve::InElastic);
                   {
                       auto start = std::chrono::system_clock::now();
                       auto end = std::chrono::system_clock::now();
                       while((std::chrono::duration_cast<std::chrono::seconds>(end - start).count() != 3)){
                               end = std::chrono::system_clock::now();
                               QCoreApplication::processEvents();
                       }

                   }


                   QPropertyAnimation* right_anim1 = new QPropertyAnimation(right[second_pos.first*6 + second_pos.second], "color", this);
                   right_anim1->setStartValue(QColor(255,0,0));
                   right_anim1->setEndValue(def);
                   right_anim1->setDuration(9000);
                   right_anim1->start(QAbstractAnimation::DeleteWhenStopped); //second open
                   right_anim1->setEasingCurve(QEasingCurve::InElastic);
                   {
                       auto start = std::chrono::system_clock::now();
                       auto end = std::chrono::system_clock::now();
                       while((std::chrono::duration_cast<std::chrono::seconds>(end - start).count() != 3)){
                               end = std::chrono::system_clock::now();
                               QCoreApplication::processEvents();
                       }

                   }


                   QPropertyAnimation* left_anim1 = new QPropertyAnimation(right[first_pos.first*6 + first_pos.second], "color", this);
                   left_anim1->setStartValue(QColor(7,59,7));
                   left_anim1->setEndValue(def);
                   left_anim1->setDuration(6000);
                   left_anim1->start(QAbstractAnimation::DeleteWhenStopped); //first cipher
                   left_anim1->setEasingCurve(QEasingCurve::InElastic);
                   {
                       auto start = std::chrono::system_clock::now();
                       auto end = std::chrono::system_clock::now();
                       while((std::chrono::duration_cast<std::chrono::seconds>(end - start).count() != 3)){
                               end = std::chrono::system_clock::now();
                               QCoreApplication::processEvents();
                       }

                   }
                   output->insertPlainText(right[first_pos.first*6 + first_pos.second]->text().at(3));

                   QPropertyAnimation* right_anim = new QPropertyAnimation(left[first_pos.first*6 + second_pos.second], "color", this);
                   right_anim->setStartValue(QColor(79,11,11));
                   right_anim->setEndValue(def);
                   right_anim->setDuration(3000);
                   right_anim->start(QAbstractAnimation::DeleteWhenStopped); //secon cipher
                   right_anim->setEasingCurve(QEasingCurve::InElastic);
                   while (right_anim->state() != QAbstractAnimation::Stopped) QCoreApplication::processEvents();
                   output->insertPlainText(left[first_pos.first*6 + second_pos.second]->text().at(3));







               }


               else if(first_pos.first == second_pos.first &&
                       first_pos.second == second_pos.second){
                   QPropertyAnimation* left_anim = new QPropertyAnimation(left[second_pos.first*6 + first_pos.second], "color", this);
                   left_anim->setStartValue(QColor(0,255,0));
                   left_anim->setEndValue(def);
                   left_anim->setDuration(12000);
                   left_anim->start(QAbstractAnimation::DeleteWhenStopped); //first open
                   left_anim->setEasingCurve(QEasingCurve::InElastic);
                   {
                       auto start = std::chrono::system_clock::now();
                       auto end = std::chrono::system_clock::now();
                       while((std::chrono::duration_cast<std::chrono::seconds>(end - start).count() != 3)){
                               end = std::chrono::system_clock::now();
                               QCoreApplication::processEvents();
                       }

                   }


                   QPropertyAnimation* right_anim1 = new QPropertyAnimation(right[second_pos.first*6 + second_pos.second], "color", this);
                   right_anim1->setStartValue(QColor(255,0,0));
                   right_anim1->setEndValue(def);
                   right_anim1->setDuration(9000);
                   right_anim1->start(QAbstractAnimation::DeleteWhenStopped); //second open
                   right_anim1->setEasingCurve(QEasingCurve::InElastic);
                   {
                       auto start = std::chrono::system_clock::now();
                       auto end = std::chrono::system_clock::now();
                       while((std::chrono::duration_cast<std::chrono::seconds>(end - start).count() != 3)){
                               end = std::chrono::system_clock::now();
                               QCoreApplication::processEvents();
                       }

                   }

                   QPropertyAnimation* right_anim = new QPropertyAnimation(right[second_pos.first*6 + second_pos.second], "color", this);
                   right_anim->setStartValue(QColor(7,59,7));
                   right_anim->setEndValue(def);
                   right_anim->setDuration(6000);
                   right_anim->start(QAbstractAnimation::DeleteWhenStopped); //second open
                   right_anim->setEasingCurve(QEasingCurve::InElastic);
                   {
                       auto start = std::chrono::system_clock::now();
                       auto end = std::chrono::system_clock::now();
                       while((std::chrono::duration_cast<std::chrono::seconds>(end - start).count() != 3)){
                               end = std::chrono::system_clock::now();
                               QCoreApplication::processEvents();
                       }

                   }
                   output->insertPlainText(right[second_pos.first*6 + second_pos.second]->text().at(3));


                   QPropertyAnimation* left_anim1 = new QPropertyAnimation(left[second_pos.first*6 + first_pos.second], "color", this);
                   left_anim1->setStartValue(QColor(79,11,11));
                   left_anim1->setEndValue(def);
                   left_anim1->setDuration(3000);
                   left_anim1->start(QAbstractAnimation::DeleteWhenStopped); //first open
                   left_anim1->setEasingCurve(QEasingCurve::InElastic);
                   while (left_anim1->state() != QAbstractAnimation::Stopped) QCoreApplication::processEvents();
                   output->insertPlainText(left[second_pos.first*6 + second_pos.second]->text().at(3));





               }

        }


    }
    input->setReadOnly(false);
    input_text->setDisabled(false);


}

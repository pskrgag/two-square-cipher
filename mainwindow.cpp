#include "mainwindow.hpp"

#include <iostream>
//1071

static std::pair<int, int> find_symbol(const QVector<MyLabel*> vec, QChar symbola){
    QChar symbol;
    if(symbola.unicode() > 1071 && symbola.unicode() < (1071+33)) symbol = symbola.unicode() - 32;
    else symbol = symbola;
    for(int i = 0; i < vec.size(); ++i){
        if(vec[i]->text() == QString("   " + QString(symbol))) return {i/6, i%6};
    }
    return {-1, -1};

}

MainWindow::MainWindow():
    left(36),
    right(36),
    input(new QPlainTextEdit(this)),   
    output(new QPlainTextEdit(this)),
    input_text(new QPushButton("Вперед", this)),
    rand_but(new QPushButton("Перемешать", this)),
    but1(new QRadioButton("шифровать", this)),
    but2(new QRadioButton("дешифровать", this))
{

    std::cout << palette().color(QWidget::backgroundRole()).name().toStdString() << std::endl;
     but1->setChecked(true);


    output->setReadOnly(true);
    connect(input_text, &QPushButton::clicked, this, &MainWindow::text_entered);
    connect(rand_but, &QPushButton::clicked, this, &MainWindow::sq_shuffle);
    resize(700, 378);
    setMaximumSize(700,378);
    setMinimumSize(700,378);
    for(int i = 0; i < 36; ++i){
        if(i < 32){
            left[i] = new MyLabel(QString("   " + QString(i + 1040)) , this);
            right[i] = new MyLabel(QString("   " + QString(i + 1040)) , this);
        }
        else if(i == 32){
            right[i] = new MyLabel("   -" , this);
            left[i] = new MyLabel("   -", this);

        }
        else if(i == 34){
            right[i] = new MyLabel("    " , this);
            left[i] = new MyLabel("    ", this);
        }
        else if(i == 35){
            right[i] = new MyLabel("   ," , this);
            left[i] = new MyLabel("   ,", this);
        }
        else if(i == 33){
            right[i] = new MyLabel("   ." , this);
            left[i] = new MyLabel("   .", this);

        }
    }
    srand(time(0));
    std::random_shuffle(left.begin(), left.end());
    std::random_shuffle(right.begin(), right.end());
        std::cout << 578 << " " << 700;

}
void MainWindow::resizeEvent(QResizeEvent *){
}
void MainWindow::paintEvent(QPaintEvent *){

    input->move(width()  - input->width(),578/10);
    input->resize(input->width(), 70);
    input_text->move(width() - input->width() + 70, 578/4);
    rand_but->move(700/4, 578/20);
    output->move(width() - input->width(), 578/3);
    output->resize(input->width(), 80);
    but1->move(700  - 100, 578/20);
    but2->move(700  - 250, 578/20);


    QPainter paint(this);
    QBrush brush(Qt::black);
    QPen pen(brush, 3);
    QFont font("Times", 15);
    paint.setPen(pen);
    static const std::size_t square_size = 578/8;
    static const std::size_t mini_square_size = 578/18;


    for(int i = 0; i < 6; ++i){
        for(int j = 0; j < 6; ++j){
            paint.drawRect(j*mini_square_size, square_size + mini_square_size*i, mini_square_size, mini_square_size);
            left[6*i + j]->setGeometry(j*mini_square_size + 2, square_size + mini_square_size*i + 2 , mini_square_size - 2.5, mini_square_size - 2.5);
            left[6*i + j]->show();
            paint.drawRect(square_size + 700 / 5 + j*mini_square_size, square_size + mini_square_size*i, mini_square_size, mini_square_size);
            right[6*i + j]->setGeometry( square_size + 700 / 5 + j*mini_square_size + 2,  square_size + mini_square_size*i + 2, mini_square_size - 2.5, mini_square_size - 2.5);
            right[6*i + j]->show();
        }

    }

}

void MainWindow::text_entered(){
    if(!input->toPlainText().size())QMessageBox::critical(this, "Ошибка", "Размер текста должен быть не равен 0");
    if(input->toPlainText().size()%2) QMessageBox::critical(this, "Ошибка", "Text size should be even");
    else if(but1->isChecked()){
        output->clear();

        QColor def = left[0]->palette().color(QWidget::backgroundRole());
        QString text = input->toPlainText();
        input->setReadOnly(true);
        input_text->setDisabled(true);
        for(int i = 0; i < text.size(); i+=2){
               QChar first = text[i];
               QChar second = text[i+1];
               std::cout << text[i].unicode() << std::endl;
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
                    left_anim1->setStartValue(FIRST_OPEN);
                    left_anim1->setEndValue(def);
                    left_anim1->setDuration(4000);
                    left_anim1->start(QAbstractAnimation::DeleteWhenStopped); //first open text
                    left_anim1->setEasingCurve(QEasingCurve::InElastic);
                    {
                        auto start = std::chrono::system_clock::now();
                        auto end = std::chrono::system_clock::now();
                        while((std::chrono::duration_cast<std::chrono::seconds>(end - start).count() != 1)){
                                end = std::chrono::system_clock::now();
                                QCoreApplication::processEvents();
                        }

                    }
                    QPropertyAnimation* right_anim1 = new QPropertyAnimation(right[second_pos.first*6 + second_pos.second], "color", this);
                    right_anim1->setStartValue(SECOND_OPEN);
                    right_anim1->setEndValue(def);
                    right_anim1->setDuration(3000);
                    right_anim1->start(QAbstractAnimation::DeleteWhenStopped); //second open text
                    right_anim1->setEasingCurve(QEasingCurve::InElastic);
                    {
                        auto start = std::chrono::system_clock::now();
                        auto end = std::chrono::system_clock::now();
                        while((std::chrono::duration_cast<std::chrono::seconds>(end - start).count() != 1)){
                                end = std::chrono::system_clock::now();
                                QCoreApplication::processEvents();
                        }

                    }

                   // while (right_anim1->state() != QAbstractAnimation::Stopped) QCoreApplication::processEvents();

                    QPropertyAnimation* right_anim = new QPropertyAnimation(right[first_pos.first*6 + second_pos.second], "color", this);
                    right_anim->setStartValue(FIRST_CIPHER);
                    right_anim->setEndValue(def);
                    right_anim->setDuration(2000);
                    right_anim->start(QAbstractAnimation::DeleteWhenStopped); //first cipher text
                    right_anim->setEasingCurve(QEasingCurve::InElastic);
                    {
                        auto start = std::chrono::system_clock::now();
                        auto end = std::chrono::system_clock::now();
                        while((std::chrono::duration_cast<std::chrono::seconds>(end - start).count() != 1)){
                                end = std::chrono::system_clock::now();
                                QCoreApplication::processEvents();
                        }

                    }


                   // while (right_anim->state() != QAbstractAnimation::Stopped) QCoreApplication::processEvents();
                    output->insertPlainText(right[first_pos.first*6 + second_pos.second]->text().at(3));

                    QPropertyAnimation* left_anim = new QPropertyAnimation(left[second_pos.first*6 + first_pos.second], "color", this);
                    left_anim->setStartValue(SECOND_CIPHER);
                    left_anim->setEndValue(def);
                    left_anim->setDuration(1000);
                    left_anim->start(QAbstractAnimation::DeleteWhenStopped); //second cipher text
                    left_anim->setEasingCurve(QEasingCurve::InElastic);

                    while (left_anim->state() != QAbstractAnimation::Stopped) QCoreApplication::processEvents();
                    output->insertPlainText(left[second_pos.first*6 + first_pos.second]->text().at(3));





               }

               else if (first_pos.first == second_pos.first && first_pos.second != second_pos.second){
                   QPropertyAnimation* left_anim = new QPropertyAnimation(left[second_pos.first*6 + first_pos.second], "color", this);
                   left_anim->setStartValue(FIRST_OPEN);
                   left_anim->setEndValue(def);
                   left_anim->setDuration(4000);
                   left_anim->start(QAbstractAnimation::DeleteWhenStopped); //first open
                   left_anim->setEasingCurve(QEasingCurve::InElastic);
                   {
                       auto start = std::chrono::system_clock::now();
                       auto end = std::chrono::system_clock::now();
                       while((std::chrono::duration_cast<std::chrono::seconds>(end - start).count() != 1)){
                               end = std::chrono::system_clock::now();
                               QCoreApplication::processEvents();
                       }

                   }


                   QPropertyAnimation* right_anim1 = new QPropertyAnimation(right[second_pos.first*6 + second_pos.second], "color", this);
                   right_anim1->setStartValue(SECOND_OPEN);
                   right_anim1->setEndValue(def);
                   right_anim1->setDuration(3000);
                   right_anim1->start(QAbstractAnimation::DeleteWhenStopped); //second open
                   right_anim1->setEasingCurve(QEasingCurve::InElastic);
                   {
                       auto start = std::chrono::system_clock::now();
                       auto end = std::chrono::system_clock::now();
                       while((std::chrono::duration_cast<std::chrono::seconds>(end - start).count() != 1)){
                               end = std::chrono::system_clock::now();
                               QCoreApplication::processEvents();
                       }

                   }


                   QPropertyAnimation* left_anim1 = new QPropertyAnimation(right[first_pos.first*6 + first_pos.second], "color", this);
                   left_anim1->setStartValue(FIRST_CIPHER);
                   left_anim1->setEndValue(def);
                   left_anim1->setDuration(2000);
                   left_anim1->start(QAbstractAnimation::DeleteWhenStopped); //first cipher
                   left_anim1->setEasingCurve(QEasingCurve::InElastic);
                   {
                       auto start = std::chrono::system_clock::now();
                       auto end = std::chrono::system_clock::now();
                       while((std::chrono::duration_cast<std::chrono::seconds>(end - start).count() != 1)){
                               end = std::chrono::system_clock::now();
                               QCoreApplication::processEvents();
                       }

                   }
                   output->insertPlainText(right[first_pos.first*6 + first_pos.second]->text().at(3));

                   QPropertyAnimation* right_anim = new QPropertyAnimation(left[first_pos.first*6 + second_pos.second], "color", this);
                   right_anim->setStartValue(SECOND_CIPHER);
                   right_anim->setEndValue(def);
                   right_anim->setDuration(1000);
                   right_anim->start(QAbstractAnimation::DeleteWhenStopped); //secon cipher
                   right_anim->setEasingCurve(QEasingCurve::InElastic);
                   while (right_anim->state() != QAbstractAnimation::Stopped) QCoreApplication::processEvents();
                   output->insertPlainText(left[first_pos.first*6 + second_pos.second]->text().at(3));







               }


               else if(first_pos.first == second_pos.first &&
                       first_pos.second == second_pos.second){

                   QPropertyAnimation* left_anim = new QPropertyAnimation(left[second_pos.first*6 + first_pos.second], "color", this);
                   left_anim->setStartValue(FIRST_OPEN);
                   left_anim->setEndValue(def);
                   left_anim->setDuration(2000);
                   left_anim->start(QAbstractAnimation::DeleteWhenStopped); //first open
                   left_anim->setEasingCurve(QEasingCurve::InElastic);
                   {
                       auto start = std::chrono::system_clock::now();
                       auto end = std::chrono::system_clock::now();
                       while((std::chrono::duration_cast<std::chrono::seconds>(end - start).count() != 1)){
                               end = std::chrono::system_clock::now();
                               QCoreApplication::processEvents();
                       }

                   }


                   QPropertyAnimation* right_anim1 = new QPropertyAnimation(right[second_pos.first*6 + second_pos.second], "color", this);
                   right_anim1->setStartValue(SECOND_OPEN);
                   right_anim1->setEndValue(def);
                   right_anim1->setDuration(1000);
                   right_anim1->start(QAbstractAnimation::DeleteWhenStopped); //second open
                   right_anim1->setEasingCurve(QEasingCurve::InElastic);
                   {
                       auto start = std::chrono::system_clock::now();
                       auto end = std::chrono::system_clock::now();
                       while((std::chrono::duration_cast<std::chrono::seconds>(end - start).count() != 1)){
                               end = std::chrono::system_clock::now();
                               QCoreApplication::processEvents();
                       }

                   }



                   QPropertyAnimation* right_anim = new QPropertyAnimation(right[second_pos.first*6 + second_pos.second], "palete", this);

                   right_anim->setStartValue(1);
                   right_anim->setEndValue(-1);
                   right_anim->setDuration(2000);
                   right_anim->start(QAbstractAnimation::DeleteWhenStopped); //second open
                   right_anim->setEasingCurve(QEasingCurve::InElastic);
                   {
                       auto start = std::chrono::system_clock::now();
                       auto end = std::chrono::system_clock::now();
                       while((std::chrono::duration_cast<std::chrono::seconds>(end - start).count() != 1)){
                               end = std::chrono::system_clock::now();
                               QCoreApplication::processEvents();
                       }

                   }
                   output->insertPlainText(right[second_pos.first*6 + second_pos.second]->text().at(3));

                   QPropertyAnimation* left_anim1 = new QPropertyAnimation(left[second_pos.first*6 + first_pos.second], "palete", this);
                   left_anim1->setStartValue(-100);
                   left_anim1->setEndValue(-1);
                   left_anim1->setDuration(1000);
                   left_anim1->start(QAbstractAnimation::DeleteWhenStopped); //first open
                   left_anim1->setEasingCurve(QEasingCurve::InElastic);
                   while (left_anim1->state() != QAbstractAnimation::Stopped) QCoreApplication::processEvents();
                   output->insertPlainText(left[second_pos.first*6 + second_pos.second]->text().at(3));





               }

        }


    }
    else{
        output->clear();

        QColor def = left[0]->palette().color(QWidget::backgroundRole());
        QString text = input->toPlainText();
        input->setReadOnly(true);
        input_text->setDisabled(true);
        for(int i = 0; i < text.size(); i+=2){
               QChar first = text[i];
               QChar second = text[i+1];
               std::cout << text[i].unicode() << std::endl;
               auto first_pos = find_symbol(right, first);
               auto second_pos = find_symbol(left, second);
               if(second_pos.first == -1 || first_pos.first == -1){
                   QMessageBox::critical(this, "Error", "You cannot cypher this symbol " +
                                                  QString(first_pos.first == -1?first:second));
                   input->setReadOnly(false);
                   input_text->setDisabled(false);
                   return;
               }
               if(first_pos.first != second_pos.first){

                    QPropertyAnimation* left_anim1 = new QPropertyAnimation(right[first_pos.first*6 + first_pos.second], "color", this);
                    left_anim1->setStartValue(FIRST_OPEN);
                    left_anim1->setEndValue(def);
                    left_anim1->setDuration(4000);
                    left_anim1->start(QAbstractAnimation::DeleteWhenStopped); //first open text
                    left_anim1->setEasingCurve(QEasingCurve::InElastic);
                    {
                        auto start = std::chrono::system_clock::now();
                        auto end = std::chrono::system_clock::now();
                        while((std::chrono::duration_cast<std::chrono::seconds>(end - start).count() != 1)){
                                end = std::chrono::system_clock::now();
                                QCoreApplication::processEvents();
                        }

                    }
                    QPropertyAnimation* right_anim1 = new QPropertyAnimation(left[second_pos.first*6 + second_pos.second], "color", this);
                    right_anim1->setStartValue(SECOND_OPEN);
                    right_anim1->setEndValue(def);
                    right_anim1->setDuration(3000);
                    right_anim1->start(QAbstractAnimation::DeleteWhenStopped); //second open text
                    right_anim1->setEasingCurve(QEasingCurve::InElastic);
                    {
                        auto start = std::chrono::system_clock::now();
                        auto end = std::chrono::system_clock::now();
                        while((std::chrono::duration_cast<std::chrono::seconds>(end - start).count() != 1)){
                                end = std::chrono::system_clock::now();
                                QCoreApplication::processEvents();
                        }

                    }

                   // while (right_anim1->state() != QAbstractAnimation::Stopped) QCoreApplication::processEvents();

                    QPropertyAnimation* right_anim = new QPropertyAnimation(left[first_pos.first*6 + second_pos.second], "color", this);
                    right_anim->setStartValue(FIRST_CIPHER);
                    right_anim->setEndValue(def);
                    right_anim->setDuration(2000);
                    right_anim->start(QAbstractAnimation::DeleteWhenStopped); //first cipher text
                    right_anim->setEasingCurve(QEasingCurve::InElastic);
                    {
                        auto start = std::chrono::system_clock::now();
                        auto end = std::chrono::system_clock::now();
                        while((std::chrono::duration_cast<std::chrono::seconds>(end - start).count() != 1)){
                                end = std::chrono::system_clock::now();
                                QCoreApplication::processEvents();
                        }

                    }


                   // while (right_anim->state() != QAbstractAnimation::Stopped) QCoreApplication::processEvents();
                    output->insertPlainText(left[first_pos.first*6 + second_pos.second]->text().at(3));

                    QPropertyAnimation* left_anim = new QPropertyAnimation(right[second_pos.first*6 + first_pos.second], "color", this);
                    left_anim->setStartValue(SECOND_CIPHER);
                    left_anim->setEndValue(def);
                    left_anim->setDuration(1000);
                    left_anim->start(QAbstractAnimation::DeleteWhenStopped); //second cipher text
                    left_anim->setEasingCurve(QEasingCurve::InElastic);

                    while (left_anim->state() != QAbstractAnimation::Stopped) QCoreApplication::processEvents();
                    output->insertPlainText(right[second_pos.first*6 + first_pos.second]->text().at(3));





               }

               else if (first_pos.first == second_pos.first && first_pos.second != second_pos.second){
                   QPropertyAnimation* left_anim = new QPropertyAnimation(right[second_pos.first*6 + first_pos.second], "color", this);
                   left_anim->setStartValue(FIRST_OPEN);
                   left_anim->setEndValue(def);
                   left_anim->setDuration(4000);
                   left_anim->start(QAbstractAnimation::DeleteWhenStopped); //first open
                   left_anim->setEasingCurve(QEasingCurve::InElastic);
                   {
                       auto start = std::chrono::system_clock::now();
                       auto end = std::chrono::system_clock::now();
                       while((std::chrono::duration_cast<std::chrono::seconds>(end - start).count() != 1)){
                               end = std::chrono::system_clock::now();
                               QCoreApplication::processEvents();
                       }

                   }


                   QPropertyAnimation* right_anim1 = new QPropertyAnimation(left[second_pos.first*6 + second_pos.second], "color", this);
                   right_anim1->setStartValue(SECOND_OPEN);
                   right_anim1->setEndValue(def);
                   right_anim1->setDuration(3000);
                   right_anim1->start(QAbstractAnimation::DeleteWhenStopped); //second open
                   right_anim1->setEasingCurve(QEasingCurve::InElastic);
                   {
                       auto start = std::chrono::system_clock::now();
                       auto end = std::chrono::system_clock::now();
                       while((std::chrono::duration_cast<std::chrono::seconds>(end - start).count() != 1)){
                               end = std::chrono::system_clock::now();
                               QCoreApplication::processEvents();
                       }

                   }


                   QPropertyAnimation* left_anim1 = new QPropertyAnimation(left[first_pos.first*6 + first_pos.second], "color", this);
                   left_anim1->setStartValue(FIRST_CIPHER);
                   left_anim1->setEndValue(def);
                   left_anim1->setDuration(2000);
                   left_anim1->start(QAbstractAnimation::DeleteWhenStopped); //first cipher
                   left_anim1->setEasingCurve(QEasingCurve::InElastic);
                   {
                       auto start = std::chrono::system_clock::now();
                       auto end = std::chrono::system_clock::now();
                       while((std::chrono::duration_cast<std::chrono::seconds>(end - start).count() != 1)){
                               end = std::chrono::system_clock::now();
                               QCoreApplication::processEvents();
                       }

                   }
                   output->insertPlainText(left[first_pos.first*6 + first_pos.second]->text().at(3));

                   QPropertyAnimation* right_anim = new QPropertyAnimation(right[first_pos.first*6 + second_pos.second], "color", this);
                   right_anim->setStartValue(SECOND_CIPHER);
                   right_anim->setEndValue(def);
                   right_anim->setDuration(1000);
                   right_anim->start(QAbstractAnimation::DeleteWhenStopped); //secon cipher
                   right_anim->setEasingCurve(QEasingCurve::InElastic);
                   while (right_anim->state() != QAbstractAnimation::Stopped) QCoreApplication::processEvents();
                   output->insertPlainText(right[first_pos.first*6 + second_pos.second]->text().at(3));







               }


               else if(first_pos.first == second_pos.first &&
                       first_pos.second == second_pos.second){

                   QPropertyAnimation* left_anim = new QPropertyAnimation(right[second_pos.first*6 + first_pos.second], "color", this);
                   left_anim->setStartValue(FIRST_OPEN);
                   left_anim->setEndValue(def);
                   left_anim->setDuration(2000);
                   left_anim->start(QAbstractAnimation::DeleteWhenStopped); //first open
                   left_anim->setEasingCurve(QEasingCurve::InElastic);
                   {
                       auto start = std::chrono::system_clock::now();
                       auto end = std::chrono::system_clock::now();
                       while((std::chrono::duration_cast<std::chrono::seconds>(end - start).count() != 1)){
                               end = std::chrono::system_clock::now();
                               QCoreApplication::processEvents();
                       }

                   }


                   QPropertyAnimation* right_anim1 = new QPropertyAnimation(left[second_pos.first*6 + second_pos.second], "color", this);
                   right_anim1->setStartValue(SECOND_OPEN);
                   right_anim1->setEndValue(def);
                   right_anim1->setDuration(1000);
                   right_anim1->start(QAbstractAnimation::DeleteWhenStopped); //second open
                   right_anim1->setEasingCurve(QEasingCurve::InElastic);
                   {
                       auto start = std::chrono::system_clock::now();
                       auto end = std::chrono::system_clock::now();
                       while((std::chrono::duration_cast<std::chrono::seconds>(end - start).count() != 1)){
                               end = std::chrono::system_clock::now();
                               QCoreApplication::processEvents();
                       }

                   }



                   QPropertyAnimation* right_anim = new QPropertyAnimation(left[second_pos.first*6 + second_pos.second], "palete", this);

                   right_anim->setStartValue(1);
                   right_anim->setEndValue(-1);
                   right_anim->setDuration(2000);
                   right_anim->start(QAbstractAnimation::DeleteWhenStopped); //second open
                   right_anim->setEasingCurve(QEasingCurve::InElastic);
                   {
                       auto start = std::chrono::system_clock::now();
                       auto end = std::chrono::system_clock::now();
                       while((std::chrono::duration_cast<std::chrono::seconds>(end - start).count() != 1)){
                               end = std::chrono::system_clock::now();
                               QCoreApplication::processEvents();
                       }

                   }
                   output->insertPlainText(left[second_pos.first*6 + second_pos.second]->text().at(3));

                   QPropertyAnimation* left_anim1 = new QPropertyAnimation(right[second_pos.first*6 + first_pos.second], "palete", this);
                   left_anim1->setStartValue(-100);
                   left_anim1->setEndValue(-1);
                   left_anim1->setDuration(1000);
                   left_anim1->start(QAbstractAnimation::DeleteWhenStopped); //first open
                   left_anim1->setEasingCurve(QEasingCurve::InElastic);
                   while (left_anim1->state() != QAbstractAnimation::Stopped) QCoreApplication::processEvents();
                   output->insertPlainText(right[second_pos.first*6 + second_pos.second]->text().at(3));





               }

        }




    }
    input->setReadOnly(false);
    input_text->setDisabled(false);


}



void MainWindow::sq_shuffle(){
    std::random_shuffle(left.begin(), left.end());
    std::random_shuffle(right.begin(), right.end());
    update();
}

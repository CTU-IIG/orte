#ifndef MAINFORM_H
#define MAINFORM_H

#include <QDialog>

#include "ui_MainForm.h"

class MainForm : public QDialog, public Ui::MainForm
{
    Q_OBJECT

public:
    MainForm(QWidget *parent = 0);

private slots:
    void about();
    void viewPublSource();
    void viewSubsSource();
    void addPublisher();
    void addSubscriber();
};

#endif

#include "conversations.h"
#include "ui_conversations.h"

Conversations::Conversations(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Conversations)
{
    ui->setupUi(this);
}

Conversations::~Conversations()
{
    delete ui;
}

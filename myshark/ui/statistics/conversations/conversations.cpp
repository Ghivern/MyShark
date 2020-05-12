#include "conversations.h"
#include "ui_conversations.h"

#include "../../../../dissector/dissres/tcp_ip_protocol_family/dissectresultlinklayer.h"
#include "../../../../dissector/dissres/tcp_ip_protocol_family/dissectresultipv4.h"
#include "../../../../dissector/dissres/tcp_ip_protocol_family/dissectresulttcp.h"
#include "../../../../dissector/dissres/tcp_ip_protocol_family/dissectresultudp.h"

Conversations::Conversations(Capturer *capturer,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Conversations)
{
    ui->setupUi(this);

    ConversationTableWidget *widget = new ConversationTableWidget(tcp_ip_protocol_family::DissectResultLinkLayer::GetStreamRecorder(),capturer,true);
    ui->tabWidget->addTab(widget,QString("Ethernet (%1)").arg(widget->rowCount()));

    widget = new ConversationTableWidget(tcp_ip_protocol_family::DissectResultIpv4::GetStreamRecorder(),capturer,false);
    ui->tabWidget->addTab(widget,QString("Ipv4 (%1)").arg(widget->rowCount()));

    widget = new ConversationTableWidget(tcp_ip_protocol_family::DissectResultTcp::GetStreamRecorder(),capturer,false);
    ui->tabWidget->addTab(widget,QString("Tcp (%1)").arg(widget->rowCount()));

    widget = new ConversationTableWidget(tcp_ip_protocol_family::DissectResultUdp::GetStreamRecorder(),capturer,false);
    ui->tabWidget->addTab(widget,QString("Udp (%1)").arg(widget->rowCount()));
}

Conversations::~Conversations()
{
    delete ui;
}

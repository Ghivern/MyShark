#include "endpoints.h"
#include "ui_endpoints.h"


#include "../../../../dissector/dissres/tcp_ip_protocol_family/dissectresultlinklayer.h"
#include "../../../../dissector/dissres/tcp_ip_protocol_family/dissectresultipv4.h"
#include "../../../../dissector/dissres/tcp_ip_protocol_family/dissectresulttcp.h"
#include "../../../../dissector/dissres/tcp_ip_protocol_family/dissectresultudp.h"


Endpoints::Endpoints(Capturer *capturer,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Endpoints)
{
    ui->setupUi(this);
    EndpointsTableWidget *widget = new EndpointsTableWidget(tcp_ip_protocol_family::DissectResultLinkLayer::GetStreamRecorder(),capturer,true);
    ui->tabWidget->addTab(widget,QString("Ethernet (%1)").arg(widget->rowCount()));

    widget = new EndpointsTableWidget(tcp_ip_protocol_family::DissectResultIpv4::GetStreamRecorder(),capturer,false);
    ui->tabWidget->addTab(widget,QString("Ipv4 (%1)").arg(widget->rowCount()));

    widget = new EndpointsTableWidget(tcp_ip_protocol_family::DissectResultTcp::GetStreamRecorder(),capturer,false,true);
    ui->tabWidget->addTab(widget,QString("Tcp (%1)").arg(widget->rowCount()));

    widget = new EndpointsTableWidget(tcp_ip_protocol_family::DissectResultUdp::GetStreamRecorder(),capturer,false,false);
    ui->tabWidget->addTab(widget,QString("Udp (%1)").arg(widget->rowCount()));
}

Endpoints::~Endpoints()
{
    delete ui;
}

#include "endpointstablewidget.h"

EndpointsTableWidget::EndpointsTableWidget(Stream &stream,Capturer *capturer,bool linklayer,bool tcp,QWidget *parent)
    :QTableWidget(parent),stream(stream),capturer(capturer),linklayer(linklayer),tcp(tcp)
{
    QStringList headers = {"Address","Port","Packets","Bytes","Send Packets","Send Bytes","Receive Packets","Receive Bytes"};
    this->setColumnCount(headers.length());
    if( !this->stream.HavePort())
        this->hideColumn(PORT);
    this->setHorizontalHeaderLabels(headers);

    this->horizontalHeader()->setStretchLastSection(true);

    this->setSortingEnabled(true);

    this->startStatics();

    this->addToTable();
}

void EndpointsTableWidget::startStatics(){
    QString srcAddr;
    QString dstAddr;
    quint16 srcPort = 0;
    quint16 dstPort = 0;
    tcp_ip_protocol_family::DissectResultLinkLayer *linklayer = nullptr;
    tcp_ip_protocol_family::DissectResultIpv4 *ipv4 = nullptr;
    tcp_ip_protocol_family::DissectResultTcp *tcp = nullptr;
    tcp_ip_protocol_family::DissectResultUdp *udp = nullptr;

    QString srcHash;
    QString dstHash;

    for( qint32 index = 0; index < this->capturer->GetCount(); index++){
        srcAddr.clear();
        dstAddr.clear();
        srcHash.clear();
        dstHash.clear();
        srcPort = 0;
        dstPort = 0;
        linklayer = this->capturer->GetDissectResultFrameByIndex(index)->GetTcpIpProtocolFamilyBaseLayer();
        if( this->linklayer ){
            srcAddr.append(linklayer->GetSourceAddressStr());
            dstAddr.append(linklayer->GetDestinationAddressStr());
        }else{
            if( linklayer->GetDissectResultBase()->ContainProtocol("ipv4")){
                ipv4 = (tcp_ip_protocol_family::DissectResultIpv4*)linklayer->GetNextLayer();
                srcAddr.append(
                            ((tcp_ip_protocol_family::DissectResultIpv4*)
                             this->capturer->GetDissectResultFrameByIndex(index)->GetTcpIpProtocolFamilyBaseLayer()->GetNextLayer())
                            ->GetSourceAddressStr()
                            );
                dstAddr.append(
                            ((tcp_ip_protocol_family::DissectResultIpv4*)
                             this->capturer->GetDissectResultFrameByIndex(index)->GetTcpIpProtocolFamilyBaseLayer()->GetNextLayer())
                            ->GetDestinationAddressStr()
                            );
                if( this->stream.HavePort()){
                    if( ipv4->GetDissectResultBase()->ContainProtocol("tcp") || ipv4->GetDissectResultBase()->ContainProtocol("udp") ){
                            if( ipv4->GetDissectResultBase()->ContainProtocol("tcp") && this->tcp){
                                tcp = (tcp_ip_protocol_family::DissectResultTcp*)ipv4->GetNextLayer();
                                srcPort = tcp->GetSourcePort();
                                dstPort = tcp->GetDestinationPort();
                                //goto start_statics;
                            }else if( ipv4->GetDissectResultBase()->ContainProtocol("udp") && !this->tcp ){
                                udp = (tcp_ip_protocol_family::DissectResultUdp*)ipv4->GetNextLayer();
                                srcPort = udp->GetSourcePort();
                                dstPort = udp->GetDestinationPort();
                            }else{
                                continue;
                            }
                    }else{
                        continue;
                    }
                }
            }else{
                continue;
            }
        }

        //开始统计
            srcHash = srcAddr + "::" + QString("%1").arg(srcPort);
            dstHash = dstAddr + "::" + QString("%1").arg(dstPort);
            if( this->indexes.contains(srcHash) ){
                results[indexes.value(srcHash)].sendPackets++;
                results[indexes.value(srcHash)].sendBytes += linklayer->GetDissectResultBase()->GetPkthdr()->caplen;
            }else{
                endPoint_t endpoint = {"",0,0,0,0,0};
                endpoint.port = srcPort;
                endpoint.address = srcAddr;
                endpoint.sendPackets++;
                endpoint.sendBytes += linklayer->GetDissectResultBase()->GetPkthdr()->caplen;
                this->indexes.insert(srcHash,this->results.length());
                this->results.append(endpoint);
            }

            if( this->indexes.contains(dstHash) ){
                results[indexes.value(dstHash)].receivedPackets++;
                results[indexes.value(dstHash)].receivedBytes += linklayer->GetDissectResultBase()->GetPkthdr()->caplen;
            }else{
                endPoint_t endpoint = {"",0,0,0,0,0};
                endpoint.port = dstPort;
                endpoint.address = dstAddr;
                endpoint.sendPackets++;
                endpoint.sendBytes += linklayer->GetDissectResultBase()->GetPkthdr()->caplen;
                this->indexes.insert(dstHash,this->results.length());
                this->results.append(endpoint);
            }
        }
}

void EndpointsTableWidget::addToTable(){
    QTableWidgetItem *item = nullptr;
    quint32 row = 0;



    for( qint64 index = 0; index < this->results.length(); index++){
        this->insertRow(row);

        item = new QTableWidgetItem(QString("%1").arg(this->results.at(index).address));
        this->setItem(row,ADDR,item);

        item = new QTableWidgetItem(QString("%1").arg(this->results.at(index).port));
        this->setItem(row,PORT,item);

        //计算统计量
        qint64 sendPackets = this->results.at(index).sendPackets;
        qint64 sendBytes = this->results.at(index).sendBytes;
        qint64 receivedPackets = this->results.at(index).receivedPackets;
        qint64 receivedBytes = this->results.at(index).receivedBytes;

        item = new QTableWidgetItem(QString("%1").arg(sendPackets + receivedPackets));
        this->setItem(row,PACKETS,item);

        item = new QTableWidgetItem(QString("%1 K").arg( qCeil((sendBytes + receivedBytes)/1024.0) ));
        this->setItem(row,BYTES,item);

        item = new QTableWidgetItem(QString("%1").arg(sendPackets));
        this->setItem(row,S_PACKETS,item);

        item = new QTableWidgetItem(QString("%1 K").arg( qCeil(sendBytes/1024.0) ));
        this->setItem(row,S_BYTES,item);

        item = new QTableWidgetItem(QString("%1").arg(receivedPackets));
        this->setItem(row,R_PACKETS,item);

        item = new QTableWidgetItem(QString("%1 K").arg( qCeil(receivedBytes/1024.0) ));
        this->setItem(row,R_BYTES,item);

        row++;
    }
}

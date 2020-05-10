#include "conversationtablewidget.h"

ConversationTableWidget::ConversationTableWidget(Stream &stream,Capturer *capturer,bool linklayer,QWidget *parent)
    :QTableWidget(parent),stream(stream),capturer(capturer),linklayer(linklayer)
{
    this->linklayer = linklayer;
    QStringList headers = {"Address A","Port A","Address B","Port B","Packets","Bytes",
                           "Packets A->B","Bytes A->B","Packets B->A","Bytes B->A",
                           "Relative Start","Duration","Bits/s A->B","Bits/s B->A"};
    this->setColumnCount(headers.length());
    if( !this->stream.HavePort() ){
        this->hideColumn(A_PORT);
        this->hideColumn(B_PORT);
    }
    this->setHorizontalHeaderLabels(headers);

    this->horizontalHeader()->setStretchLastSection(true);

    this->addToTable();
}

void ConversationTableWidget::addToTable(){
    QTableWidgetItem *item = nullptr;
    quint32 row = 0;

    for(qint32 streamIndexPlusOne = 1; streamIndexPlusOne <= this->stream.GetStreamCount(); streamIndexPlusOne++){
        this->insertRow(row);

        item = new QTableWidgetItem(QString("%1").arg(this->stream.GetSourcePort(streamIndexPlusOne)));
        this->setItem(row,A_PORT,item);

        item = new QTableWidgetItem(QString("%1").arg(this->stream.GetDestinationPort(streamIndexPlusOne)));
        this->setItem(row,B_PORT,item);

        /*计算统计量*/
        QList<quint64> packetIndexList = this->stream.GetPacketsIndexListByStream(streamIndexPlusOne);
        QList<quint64> rPacketIndexList = this->stream.GetPacketsIndexListByStream(-streamIndexPlusOne);

        tcp_ip_protocol_family::DissectResultLinkLayer *linklayer = capturer->GetDissectResultFrameByIndex(packetIndexList.first())->GetTcpIpProtocolFamilyBaseLayer();
        if( this->linklayer ){
            item = new QTableWidgetItem(linklayer->GetSourceAddressStr());
            this->setItem(row,A_ADDR,item);
            item = new QTableWidgetItem(linklayer->GetDestinationAddressStr());
            this->setItem(row,B_ADDR,item);
        }else{
            void *networklayer = linklayer->GetNextLayer();
            if( linklayer->GetDissectResultBase()->ContainProtocol("ipv4") ){
                tcp_ip_protocol_family::DissectResultIpv4 *ipv4 = (tcp_ip_protocol_family::DissectResultIpv4*)networklayer;
                item = new QTableWidgetItem(ipv4->GetSourceAddressStr());
                this->setItem(row,A_ADDR,item);
                item = new QTableWidgetItem(ipv4->GetDestinationAddressStr());
                this->setItem(row,B_ADDR,item);
            }else{

            }
        }

        qint64 a_b_packets = packetIndexList.length();
        qint64 b_a_packets = rPacketIndexList.length();
        qint64 a_b_bytes = 0;
        qint64 b_a_bytes = 0;
        for( qint32 index = 0; index < a_b_packets; index++){
            if( index < a_b_packets ){
                a_b_bytes += capturer->GetDissectResultFrameByIndex(packetIndexList.at(index))->GetCapLen();
            }
            if( index < b_a_packets ){
                b_a_bytes += capturer->GetDissectResultFrameByIndex(rPacketIndexList.at(index))->GetCapLen();
            }
        }

        float relStart = 0;
        float duration = 0;
        if( !packetIndexList.isEmpty() ){
            relStart = capturer->GetDissectResultFrameByIndex(packetIndexList.first())->GetRelativeTimeSinceFirstPacket();
            if( !rPacketIndexList.isEmpty() ){
                duration = qMax<float>(capturer->GetDissectResultFrameByIndex(packetIndexList.last())->GetRelativeTimeSinceFirstPacket()
                                       ,capturer->GetDissectResultFrameByIndex(rPacketIndexList.last())->GetRelativeTimeSinceFirstPacket())
                           - relStart;
            }else{
                duration = capturer->GetDissectResultFrameByIndex(packetIndexList.last())->GetRelativeTimeSinceFirstPacket() - relStart;
            }
        }

        //Packets
        item = new QTableWidgetItem(QString("%1").arg(a_b_packets + b_a_packets));
        this->setItem(row,PACKETS,item);

        //Bytes
        item = new QTableWidgetItem(QString("%1 K").arg( qCeil((a_b_bytes + b_a_bytes) * 1.0 / 1024) ) );
        this->setItem(row,BYTES,item);

        //Pcakets A -> B
        item = new QTableWidgetItem(QString("%1").arg(a_b_packets));
        this->setItem(row,A_B_PACKETS,item);

        //Bytes A -> B
        item = new QTableWidgetItem(QString("%1 K").arg(qCeil(a_b_bytes * 1.0 / 1024 )));
        this->setItem(row,A_B_BYTES,item);

        //Packets B -> A
        item = new QTableWidgetItem(QString("%1").arg(b_a_packets));
        this->setItem(row,B_A_PACKETS,item);

        //Bytes B -> A
        item = new QTableWidgetItem(QString("%1 K").arg(qCeil(b_a_bytes * 1.0 / 1024 )));
        this->setItem(row,B_A_BYTES,item);

        //Rel Start
        item = new QTableWidgetItem(QString::asprintf("%.6f",relStart));
        this->setItem(row,REL_START,item);

        //Duration
        item = new QTableWidgetItem(QString::asprintf("%.6f",duration));
        this->setItem(row,DURATION,item);

        //Bits/s A -> B
        item = new QTableWidgetItem(QString("%1").arg(
                                        duration < 0.000001 ?
                                        "-"
                                        :QString("%1 K").arg(qCeil(a_b_bytes * 8.0 / 1024 / duration)))
                                    );
        this->setItem(row,A_B_BITS_PER_SECOND,item);

        //Bits/s B -> A
        item = new QTableWidgetItem(QString("%1").arg(
                                        duration < 0.000001 ?
                                        "-"
                                        :QString("%1 K").arg(qCeil(b_a_bytes * 8.0 / 1024 / duration)))
                                    );
        this->setItem(row,B_A_BITS_PER_SECOND,item);

        row++;
    }
}

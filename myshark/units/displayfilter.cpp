#include "displayfilter.h"

DisplayFilter::DisplayFilter()
{
    this->filterStr = "";
    this->displayedCount = 0;
}

bool DisplayFilter::SetFilter(QString filterStr){
    this->mutex.tryLock();
    this->filterStr.clear();
    if( !filterStr.isEmpty() )
        this->filterStr.append(filterStr.trimmed());
    this->displayedCount = 0;
    this->mutex.unlock();
    return true;
}

bool DisplayFilter::FilterIsValied(QString filterStr){
   // bool allNumber = true;
    QStringList ev = filterStr.trimmed().split(" == ",QString::SkipEmptyParts);
    if( ev.length() == 2 ){
        if( ev.first() == "ipv4.addr" || ev.first() == "ipv4.saddr" || ev.first() == "ipv4.daddr"){
            QStringList addrValues = ev.last().split(".",QString::SkipEmptyParts);
            if( addrValues.length() == 4 ){
                for( qint32 i = 0; i < addrValues.length(); i++){
                    QString value = addrValues.at(i).trimmed();
                    if( i == addrValues.length() - 1 )
                        value = value.trimmed();
                    for( qint32 j = 0; j < value.length(); j++ ){
                        if( !value.at(j).isNumber() )
                            return false;
                    }
                }
            }else{
                return false;
            }
            return true;
        }else if( ev.first() == "tcp.port" || ev.first() == "tcp.sport" || ev.first() == "tcp.dport"
                  || ev.first() == "udp.port" || ev.first() == "udp.sport" || ev.first() == "udp.dport"
                  || ev.first() == "tcp.stream" || ev.first() == "udp.stream"){
            QString port = ev.last().trimmed();
            for( qint32 i = 0; i < port.length(); i++){
                if( !port.at(i).isNumber() )
                    return false;
            }
            return true;
        }else{
            return false;
        }
    }else{
        if( filterStr.isEmpty() )
            return true;

        if( !filterStr.contains("==") && !filterStr.contains(".") && valiedCondition.contains(ev.first()) )
            return true;
        else
            return false;
    }
}

bool DisplayFilter::Filte(DissectResultFrame *frame){
    TcpInfo *tcpInfo = (TcpInfo*)frame->GetDissectResultBase()->GetAdditionalPtr(TCP_INFO_PTR);
    this->mutex.tryLock();
    if( !this->filterStr.isEmpty() ){
        if( frame->GetDissectResultBase()->ContainProtocol("tcp")
                && qAbs(tcpInfo->streamPlusOne) - 1 == this->filterStr.toInt()){
            this->displayedCount++;
            this->mutex.unlock();
            return true;
        }else{
            this->mutex.unlock();
            return false;
        }
    }else{
        this->displayedCount++;
        this->mutex.unlock();
        return true;
    }
}

bool DisplayFilter::FilteWithoutLogicOperator(DissectResultFrame *frame){
    QStringList ev = filterStr.split(" == ",QString::SkipEmptyParts);

    tcp_ip_protocol_family::DissectResultIpv4 *ipv4 = nullptr;
    tcp_ip_protocol_family::DissectResultUdp *udp = nullptr;
    tcp_ip_protocol_family::DissectResultTcp *tcp = nullptr;


    this->mutex.tryLock();
    if( !this->filterStr.isEmpty() ){
        if( frame->GetDissectResultBase()->ContainProtocol(ev.first().split(".").first().trimmed()) ){
            if( ev.length() == 1 ){
                goto display_dissect_result;
            }
            if( ev.length() == 2){ // ev.length == 2
                if( frame->GetDissectResultBase()->ContainProtocol("ipv4") ){
                    ipv4 = (tcp_ip_protocol_family::DissectResultIpv4*)
                            frame->GetTcpIpProtocolFamilyBaseLayer()->GetNextLayer();
                    if( ev.first().split(".").first() == "ipv4" ){
                        if( ev.first() == "ipv4.saddr" ){
                                if( ipv4->GetSourceAddressStr() == ev.last() ){
                                    goto display_dissect_result;
                                }
                        }else if( ev.first() == "ipv4.daddr" ){
                             if( ipv4->GetDestinationAddressStr() == ev.last() ){
                                goto display_dissect_result;
                            }
                        }else{
                            if( ipv4->GetSourceAddressStr() == ev.last() || ipv4->GetDestinationAddressStr() == ev.last() ){
                                goto display_dissect_result;
                            }
                        }
                    }
                }
                if( frame->GetDissectResultBase()->ContainProtocol("tcp") && ipv4 != nullptr){
                    tcp = (tcp_ip_protocol_family::DissectResultTcp*)ipv4->GetNextLayer();
                        //TcpInfo *tcpInfo = (TcpInfo*)frame->GetDissectResultBase()->GetAdditionalPtr(TCP_INFO_PTR);
                    if( ev.first().contains("tcp") ){
                        if( ev.first() == "tcp.stream"){
                            if( ev.last().toInt() == tcp->GetStreamIndex()){
                                goto display_dissect_result;
                            }
                        }else if( ev.first() == "tcp.sport" ){
                            if( ev.last().toInt() == tcp->GetSourcePort() ){
                                goto display_dissect_result;
                            }
                        }else if( ev.first() == "tcp.dport" ){
                            if( ev.last().toInt() == tcp->GetDestinationPort() ){
                                goto display_dissect_result;
                            }
                        }else{
                            if( ev.last().toInt() == tcp->GetSourcePort() || ev.last().toInt() == tcp->GetDestinationPort() ){
                                goto display_dissect_result;
                            }
                        }
                    }
                }

                if( frame->GetDissectResultBase()->ContainProtocol("udp") && ipv4 != nullptr){
                    udp = (tcp_ip_protocol_family::DissectResultUdp*)ipv4->GetNextLayer();
                    if( ev.first().contains("udp") ){
                        if( ev.first() == "udp.stream"){
                            if( ev.last().toInt() == udp->GetStreamIndex() ){
                                goto display_dissect_result;
                            }
                        }else if( ev.first() == "udp.sport" ){
                            if( ev.last().toInt() == udp->GetSourcePort() ){
                                goto display_dissect_result;
                            }
                        }else if( ev.first() == "udp.dport" ){
                            if( ev.last().toInt() == udp->GetDestinationPort() ){
                                goto display_dissect_result;
                            }
                        }else{
                            if( ev.last().toInt() == udp->GetSourcePort() || ev.last().toInt() == udp->GetDestinationPort() ){
                                goto display_dissect_result;
                            }
                        }
                    }
                }
            }
        }
    }else{
        goto display_dissect_result;
    }

    this->mutex.unlock();
    return false;

    display_dissect_result:
    this->displayedCount++;
    this->mutex.unlock();
    return true;
}



qint64 DisplayFilter::GetDisplayedCount(){
    return this->displayedCount;
}

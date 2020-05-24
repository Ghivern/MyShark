#include "capturer.h"



Capturer::Capturer(){
    this->capHandle = nullptr;
    this->tempfile = nullptr;
    this->dissectorOptions = DissectResultBase::GetDissectorOptionPtr();
    this->dumper = nullptr;
    this->tempFileHaveBeenSaved = false;
}

Capturer::~Capturer(){

}

qint64 Capturer::GetCount(){
    return this->dissectResultFrameList.length();
}

DissectResultFrame* Capturer::GetDissectResultFrameByIndex(qint64 index){
    return this->dissectResultFrameList.at(index);
}

QString Capturer::GetFilePath(){
    if( this->capHandle != nullptr ){
        if( this->capHandle->FromFile() )
            return this->capHandle->GetFileName();
        else
            if( this->tempfile != nullptr )
                return this->tempfile->fileName();
    }
    return "";
}

CapHandle* Capturer::GetCapHandle(){
    return this->capHandle;
}

//Public Slots
bool Capturer::slot_startThread(QString interfaceOrFile,bool fromFile){
    if( this->updateCaptureThread(interfaceOrFile,fromFile)){
        this->start();
        return true;
    }else{
        return false;
    }
}

bool Capturer::slot_stopThread(){
    try {
        this->capHandle->SetNonBlock(1);
    } catch (QString e) {
        QMessageBox::critical(nullptr,"Error",e);
    }
    this->terminate();
    if( this->GetCount() == 0 ){
        StopWithoutAnyPacket stopWithoutAnyPacket;
        stopWithoutAnyPacket.exec();
        this->clearCaptureThread();
        return true;
    }
    return false;
}

bool Capturer::slot_clearThread(qint32 newCapOrFileOrCloseOrQuit){
    return this->clearCaptureThread(newCapOrFileOrCloseOrQuit);
}

bool Capturer::slot_saveTempFile(){
    return this->saveTempFile();
}

void Capturer::run(){
    const uchar *raw;
    struct pcap_pkthdr *pkthdr;
    qint64 index = 0;
    qint32 res;
//    QList<void*> *reserves = new QList<void*> {dissectorOptions};
    this->dissectResultFrameList.clear();
    this->tempFileHaveBeenSaved = false;
    DissectResultBase::SetInterfaceInfo(this->capHandle->GetDeviceIndex()
                                        ,this->capHandle->GetDeviceName()
                                        ,this->capHandle->GetLinkType()
                                        ,this->capHandle->GetLinkTypeName());
    DissectResultFrame::Clear();
    while(true){
        if((res = pcap_next_ex(this->capHandle->GetPcapHandle(),&pkthdr,&raw)) == 1){
            switch (this->capHandle->GetLinkType()) {
            case 1:
            {
                this->dissectResultFrameList.append(new DissectResultFrame(raw,pkthdr,index
                    ,DissectResultFrame::PROTOCOL_FAMILY_TYPE::TCP_IP_PROTOCOL_FAMILY,nullptr));
                this->dumper->Dump(pkthdr,raw);
                this->dumper->Flush();
                emit onePacketCaptured(this->dissectResultFrameList.at(index));
                index++;
                break;
            }
            default:
            {
                qDebug() << "未知链路层类型";
//                dissRes = new DissRes(index);
                break;
            }
            }

        }else if(res == 0){
            qDebug() << "Capturer : timeout";
        }else if(res == -1){
            qDebug() << "Capturer : error, 没有权限";
        }else if(res == -2){
            qDebug() << "Capturer : come to file end";
            break;
        }else{
            qDebug() << pcap_geterr(this->capHandle->GetPcapHandle());
        }
    }
}

bool Capturer::updateCaptureThread(QString interfaceOrFile, bool fromFile){
    try{
        CapHandle *tempHandle = new CapHandle(interfaceOrFile,fromFile);

        if( !fromFile )
            tempHandle->ActivateHandleWithParas();

        if( this->clearCaptureThread(fromFile ? true : false)){
            this->capHandle = tempHandle;
        }else{
            tempHandle->Close();
            delete tempHandle;
            return false;
        }
    }catch(QString e){
        QMessageBox::critical(nullptr,"Error",e);
        return false;
    }

    if( this->capHandle->GetLinkType() != 1){
        QMessageBox::critical(nullptr,"Error",capHandle->GetLinkTypeName()
                                 + " Corresponding parser has not been added yet");
        this->capHandle->Close();
        delete this->capHandle;
        this->capHandle = nullptr;
        return false;
    }

    this->tempfile = new QTemporaryFile;
    this->tempfile->open();
    this->tempfile->close();
    this->dumper = new Dumper(this->capHandle,this->tempfile->fileName());
    return true;
}

bool Capturer::clearCaptureThread(qint32 newCapOrFileOrCloseOrQuit){
    if( this->capHandle == nullptr ){
        return true;
    }

    if( !this->capHandle->FromFile() && this->GetCount() > 0 && !this->tempFileHaveBeenSaved ){
        SaveOrCloseFileDialog saveOrCloseFileDialog;
        switch (newCapOrFileOrCloseOrQuit) {
        case 1:
            saveOrCloseFileDialog.SetContentForOpenFile();
            break;
        case 2:
            saveOrCloseFileDialog.SetContentForClose();
            break;
        case 3:
            saveOrCloseFileDialog.SetContentForQuit();
            break;
        default:
            saveOrCloseFileDialog.SetContentForNewCapture();
        }
        saveOrCloseFileDialog.exec();
        if( saveOrCloseFileDialog.result() == QDialog::Rejected ){
            if( saveOrCloseFileDialog.IsContinueWithSaving() ){
                if( this->saveTempFile() ){
                    goto clear_capThread;
                }else{
                    return false;
                }
            }else{
                return false;
            }
        }
    }

    clear_capThread:
    if( this->tempfile != nullptr )
        this->tempfile->deleteLater();
    this->capHandle->Close();
    delete this->capHandle;
    if( this->dumper != nullptr ){
        this->dumper->Close();
        delete this->dumper;
    }
    this->capHandle = nullptr;
    return true;
}

bool Capturer::saveTempFile(){
    if( this->capHandle != nullptr && this->tempfile != nullptr ){
        QString path = QFileDialog::getSaveFileName(nullptr,"Save file",QDir::currentPath(),"pcap(*.pcap)");
        if( !path.isEmpty() ){
            this->dumper->Flush();
            QFile::copy(this->tempfile->fileName(),path);
            this->tempFileHaveBeenSaved = true;
            return true;
        }else{
            return false;
        }
    }
    return false;
}


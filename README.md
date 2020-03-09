# MyShark
This project is my undergraduate graduate disign named MyShark which is similar with WireShark,a network packet analysis tool.

## 开发工具
Qt Creator

## 第三方库
libpcap

## 抓包模块(Capturer:public QThread)
libpcap::pcap_next_ex():非阻塞，循环
	若获取包成功，则将抓到的原始数据添加到QList中
	emit onePacketCaptured(qint64 index) : 发射信号，用于驱动解析器
	
## 解析器模块(Dissector)


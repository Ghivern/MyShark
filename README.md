# MyShark
This project is my undergraduate graduate disign named MyShark which is similar with WireShark,a network packet analysis tool.

## 一、开发工具

Qt Creator

## 二、第三方库

libpcap

## 三、抓包模块

Capturer:public QThread

libpcap::pcap_next_ex():非阻塞，循环

   若获取包成功，则将抓到的原始数据添加到RawList_t(README.RawList_t)列表中

   emit onePacketCaptured(qint64 index) : 发射信号，用于驱动解析器
	
## 四、解析器模块

DissectorBase: 解析器基类

   -virtual Dissect(rawList_t *raw,DissResList_t *dissResList,qint64 index,Info *info)
   
   info用于区别详细解析和简单解析，详细解析时携带硬件信息
   
   简单解析：解析结果存入DissResList_t(DissResList_t)列表，序号和原始数据在RawList_t中的位置相对应
   
   详细解析：解析结果放入协议树proTree(README.ProTree),函数返回其指针
   
3.1 DissectorXXX:public Dissector (XXX 对应某一Frame顶层头类型，比如eth)

   - override Dissect();		    

3.2 通用解析Frame非数据信息

DissectorFrame

   - static qint32 flag
   
   - static Dissect()

3.3 针对XXX的顶层协议解析器的高层协议解析器(eth为例子)

DissectorEth:public DissectorBase

   - static qint32 flag
   
   - Dissect()
   
以下均为XXX顶层协议下的高层协议解析器，通过通过协议号或端口号进行选择使用

DissectorIp

   - static qint32 flag
   
   - static Dissect()
   
DissectorIpv6

   - static qint32 flag
   
   - static Dissect()
   
DissectorUdp

   - static qint32 flag
   
   - static Dissect()
   
DissectorTcp

   - static qint32 flag
   
   - static Dissect()
   

3.4 集合所有的XXX解析器Loader

Loader

   - QHash<datalink,DissectorBase>
   
   将DissectorXXX的实例和对应datalink存入QHash;通过datalink获取相应的DissectorXXX解析器

3.5 调用解析器

Dissector

   - DissResList_t *dissResList
   
   - Loader *loader
   
   - Capturer *capturer
   
   接收Capturer的信号(onePacketCaptured(qint64 index))

       进行简单解析：loader->GetDissector(datalink)->Dissect();
   
       emit onePacketDissected(qint64 index):用于GUI接收信号，并将简单解析结果显示到GUI
   
   详细解析响应GUI事件，并返回协议树，用于进一步将协议树显示到GUI

   
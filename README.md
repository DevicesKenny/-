本项目功能描述：
	
	整个系统主要分成安防、图书入库和途虎借阅三个部分，其中安防模块通过烟雾传感器采集环境信息，
监测是否发生浓烟，一旦发生，报警信息传至服务器提醒管理员。在入馆处，装有门禁系统（电子锁），对
入馆人员的身份进行确认，借阅者凭借书卡入馆。图书馆闭馆后，将红外对射模块开启，进行布防，当有人
非法闯入时，报警信息传至服务器端，提醒值班人员，同时报警灯报警。图书馆入库模块使用贴在图书封面
或封底的RFID标签标识每本图书，RFID标签记录图书的基本信息和存放位置。图书借阅模块通过将记录借阅
者的信息的射频卡与图书的RFID标签相关联，通过RFID阅读器实现图书的自动借阅和归还。在这套系统中，
服务器与局域网相连，读者使用个人电脑、移动设备科随时随地地通过局域网查阅图书借阅信息。

	本项目使用数据帧与服务器进行通信，具体帧使用可查看系统数据帧汇总。
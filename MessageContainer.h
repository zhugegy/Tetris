#pragma once

//MessageContainer类

/*
在此程序中，用于存储、读取电脑AI操作的指令。指令的形式是整形数据。
暂时是固定长度的一个整形数组。没有长度检查。没有自动扩容。如果以后有需求，再完善这个类。
*/

//
#define MESSAGE_CONTAINER_MAX_NUM 100

class MessageContainer
{
private:
  int MessageList[MESSAGE_CONTAINER_MAX_NUM];
  int nReadingPostion;
  int nWritingPostion;

public:
  MessageContainer();
  ~MessageContainer();

  int reset_message_list();

  int write_message(int *pMsgList, int nMessageNum);
  int write_message(int nCmd);

  int read_message();

  int backwards_reading_postion();

};


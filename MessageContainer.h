#pragma once

//MessageContainer��

/*
�ڴ˳����У����ڴ洢����ȡ����AI������ָ�ָ�����ʽ���������ݡ�
��ʱ�ǹ̶����ȵ�һ���������顣û�г��ȼ�顣û���Զ����ݡ�����Ժ�����������������ࡣ
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


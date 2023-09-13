#include "guichat.h"
#include "msgworker.h"
#include <cstring>
#include <iostream>

MsgWorker::MsgWorker() {}

void MsgWorker::doWork(GUIChat* caller) {
	while (1) {
		read(caller->getClientSocket(), this->msg_buffer.name, sizeof(this->msg_buffer.name));
		read(caller->getClientSocket(), this->msg_buffer.msg, sizeof(this->msg_buffer.msg));
		caller->notify();
	}
}

void MsgWorker::getMsg(char* name_buffer, char* msg_buffer) {
	strcpy(name_buffer, this->msg_buffer.name);
	strcpy(msg_buffer, this->msg_buffer.msg);
}
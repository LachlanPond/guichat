#include "guichat.h"
#include "msgworker.h"
#include <cstring>

MsgWorker::MsgWorker() {}

void MsgWorker::doWork(GUIChat* caller) {
	while (1) {
		read(caller->getClientSocket(), this->msg_buffer, sizeof(this->msg_buffer));
		caller->notify();
	}
}

void MsgWorker::getMsg(char* msg_buffer) {
	strcpy(msg_buffer, this->msg_buffer);
}
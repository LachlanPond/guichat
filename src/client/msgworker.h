#ifndef GTKMM_MSGWORKER_H
#define GTKMM_MSGWORKER_H

#include <gtkmm.h>
#include <thread>
#include "../defines.h"

class GUIChat;

class MsgWorker {
public:
	MsgWorker();

	// Thread function
	void doWork(GUIChat* caller);
	void getMsg(char* name_buffer, char* msg_buffer);

private:
	// char msg_buffer[512];
	struct {
		char name[NAME_SIZE];
		char msg[MSG_SIZE];
	} msg_buffer;
};

#endif // GTKMM_MSGWORKER_H
#ifndef GTKMM_MSGWORKER_H
#define GTKMM_MSGWORKER_H

#include <gtkmm.h>
#include <thread>

class GUIChat;

class MsgWorker {
public:
	MsgWorker();

	// Thread function
	void doWork(GUIChat* caller);
	void getMsg(char* msg_buffer);

private:
	char msg_buffer[512];
};

#endif // GTKMM_MSGWORKER_H
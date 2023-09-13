#ifndef GTKMM_GUICHAT_H
#define GTKMM_GUICHAT_H

#include <gtkmm/button.h>
#include <gtkmm/box.h>
#include <gtkmm/textview.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <gtkmm/frame.h>
#include <gtkmm/window.h>
#include <thread>
#include "msgworker.h"
#include "../defines.h"

#define SERVER_PORT 8083

class GUIChat : public Gtk::Window {
	Glib::ustring name;
	Glib::ustring ip;

	int client_socket;
	bool connected;

public:
	GUIChat();
	void setName(Glib::ustring name);
	Glib::ustring getName();
	void setIP(Glib::ustring ip);
	Glib::ustring getIP();
	int getClientSocket();
	void sendChatMessage(Glib::ustring msg);
	void sendChatMessage(Glib::ustring msg, std::string colour);
	void notify();

protected:
	// Signal handlers:
	void on_connect_button_clicked();
	void on_msg_entry_submit();
	void on_notification_from_worker_thread();
	// void handleServerMessage();

	// Member widgets:
	Gtk::Box m_VBox, m_Name_HBox, m_IP_HBox;
	Gtk::Label m_Name_Label, m_IP_Label;
	Gtk::Entry m_Name_Entry, m_IP_Entry, m_Msg_Entry;
	Gtk::Frame m_Chat_Frame;
	Gtk::TextView m_Chat_TextView;
	Gtk::Button m_Connect_Button;

	Glib::Dispatcher m_Dispatcher;
	MsgWorker m_Msg_Worker;
	std::thread* m_MessageReceiveThread;
};

#endif // GTKMM_GUICHAT_H
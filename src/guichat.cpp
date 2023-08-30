
#include "guichat.h"

GUIChat::GUIChat() : 
	m_VBox(Gtk::Orientation::VERTICAL),
	m_Name_HBox(Gtk::Orientation::HORIZONTAL),
	m_IP_HBox(Gtk::Orientation::HORIZONTAL),
	m_Name_Label("Name: "),
	m_IP_Label("Server: "),
	m_Name_Entry(),
	m_IP_Entry(),
	m_Msg_Entry(),
	m_Chat_Frame("Chat"),
	m_Chat_TextView(),
	m_Connect_Button("Connect")
{
	set_title("GUIChat");
	set_default_size(500,600);

	set_child(m_VBox);

	m_VBox.append(m_Name_HBox);
	m_Name_HBox.append(m_Name_Label);
	m_Name_Entry.set_max_length(50);
	m_Name_HBox.append(m_Name_Entry);

	m_VBox.append(m_IP_HBox);
	m_IP_HBox.append(m_IP_Label);
	m_IP_Entry.set_max_length(50);
	m_IP_HBox.append(m_IP_Entry);

	m_VBox.append(m_Connect_Button);

	m_Chat_Frame.set_child(m_Chat_TextView);
	m_Chat_Frame.set_size_request(-1,500);

	m_VBox.append(m_Chat_Frame);

	m_VBox.append(m_Msg_Entry);

};


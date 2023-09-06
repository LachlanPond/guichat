
#include "guichat.h"
#include <iostream>

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

	m_Connect_Button.signal_clicked().connect(sigc::mem_fun(*this,
		&GUIChat::on_connect_button_clicked));

	// Pressing 'Enter' triggers the connect button
	m_Name_Entry.set_activates_default();
	m_IP_Entry.set_activates_default();

	set_default_widget(m_Connect_Button);
};

void GUIChat::setName(std::string name) {
	this->name = name;
	std::cout << this->name << std::endl;
}

std::string GUIChat::getName() {
	return this->name;
}

void GUIChat::setIP(std::string ip) {
	this->ip = ip;
}

std::string GUIChat::getIP() {
	return this->ip;
}

void GUIChat::on_connect_button_clicked() {
	setName(m_Name_Entry.get_text());
	setIP(m_IP_Entry.get_text());
}
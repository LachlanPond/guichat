
#include "guichat.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>

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

	m_Chat_TextView.set_editable(false);
	m_Chat_Frame.set_child(m_Chat_TextView);
	m_Chat_Frame.set_size_request(200,500);

	m_VBox.append(m_Chat_Frame);

	m_VBox.append(m_Msg_Entry);

	// Signal event listeners
	m_Connect_Button.signal_clicked().connect(sigc::mem_fun(*this,
		&GUIChat::on_connect_button_clicked));

	m_Msg_Entry.signal_activate().connect(sigc::mem_fun(*this,
		&GUIChat::on_msg_entry_submit));

	// Pressing 'Enter' triggers the connect button
	m_Name_Entry.set_activates_default();
	m_IP_Entry.set_activates_default();

	set_default_widget(m_Connect_Button);
	this->connected = false;
};

void GUIChat::setName(Glib::ustring name) {
	this->name = name;
}

Glib::ustring GUIChat::getName() {
	return this->name;
}

void GUIChat::setIP(Glib::ustring ip) {
	this->ip = ip;
}

Glib::ustring GUIChat::getIP() {
	return this->ip;
}

void GUIChat::sendChatMessage(Glib::ustring msg, std::string colour) {
	Glib::RefPtr<Gtk::TextBuffer> text_buffer = m_Chat_TextView.get_buffer();

	text_buffer->insert_markup(text_buffer->end(),
		"<span color=\"" + colour + "\">" + msg + "</span>\n");
}

void GUIChat::sendChatMessage(Glib::ustring msg) {
	Glib::RefPtr<Gtk::TextBuffer> text_buffer = m_Chat_TextView.get_buffer();

	text_buffer->insert(text_buffer->end(),msg + "\n");
}

// Event functions
void GUIChat::on_connect_button_clicked() {
	if (this->connected) {
		::close(this->client_socket);
		m_Connect_Button.set_label("Connect");
		this->connected = false;
		return;
	}

	this->name = m_Name_Entry.get_text();
	this->ip = m_IP_Entry.get_text();

	if (this->name == "") {
		GUIChat::sendChatMessage("Your name cannot be blank", "red");
		return;
	}

	if (this->ip == "") {
		GUIChat::sendChatMessage("The server address cannot be blank", "red");
		return;
	}

	sockaddr_in server_address;

	this->client_socket = socket(AF_INET, SOCK_STREAM, 0);

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(8081);
	server_address.sin_addr.s_addr = INADDR_ANY;

	connect(this->client_socket, (struct sockaddr *)&server_address, sizeof(server_address));
	this->connected = true;
	m_Connect_Button.set_label("Disconnect");
}

void GUIChat::on_msg_entry_submit() {
	Glib::ustring msg;
	msg = m_Msg_Entry.get_text();
	m_Msg_Entry.set_text("");

	GUIChat::sendChatMessage(this->name + ": " + msg);
}
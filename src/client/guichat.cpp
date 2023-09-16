#include "guichat.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <thread>

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
	m_Connect_Button("Connect"),
	m_Dispatcher(),
	m_Msg_Worker(),
	m_MessageReceiveThread(nullptr)
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

	m_Dispatcher.connect(sigc::mem_fun(*this, &GUIChat::on_notification_from_worker_thread));
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

int GUIChat::getClientSocket() {
	return this->client_socket;
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
		::shutdown(this->client_socket, SHUT_RDWR);
		m_MessageReceiveThread->join();
		m_Connect_Button.set_label("Connect");
		GUIChat::sendChatMessage("Disconnected from the server", "red");
		m_Name_Entry.set_editable(true);
		m_IP_Entry.set_editable(true);
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

	struct hostent* server_ip_he;
	server_ip_he = gethostbyname(this->ip.c_str());
	if (server_ip_he == NULL) {
		perror("getbyhostname");
		GUIChat::sendChatMessage("The server address cannot be resolved", "red");
		return;
	}

	sockaddr_in server_address;
	this->client_socket = socket(AF_INET, SOCK_STREAM, 0);

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(SERVER_PORT);
	server_address.sin_addr = *((struct in_addr*)server_ip_he->h_addr);

	connect(this->client_socket, (struct sockaddr *)&server_address, sizeof(server_address));
	this->connected = true;
	m_Connect_Button.set_label("Disconnect");

	char name_buffer[NAME_SIZE];
	std::size_t length = this->name.copy(name_buffer,sizeof(name_buffer)-1,0);
	name_buffer[length] = '\0';
	::send(this->client_socket, name_buffer, sizeof(name_buffer), 0);

	m_Name_Entry.set_editable(false);
	m_IP_Entry.set_editable(false);

	m_MessageReceiveThread = new std::thread([this]{m_Msg_Worker.doWork(this);});

	GUIChat::sendChatMessage("Connected to the server (" + this->ip + ")", "green");
}

void GUIChat::on_msg_entry_submit() {
	Glib::ustring msg;
	char msg_buffer[MSG_SIZE];
	msg = m_Msg_Entry.get_text();
	m_Msg_Entry.set_text("");

	std::size_t length = msg.copy(msg_buffer,sizeof(msg_buffer),0);
	msg_buffer[length] = '\0';

	::send(this->client_socket, msg_buffer, sizeof(msg_buffer), 0);
}

void GUIChat::notify() {
	m_Dispatcher.emit();
}

void GUIChat::on_notification_from_worker_thread() {
	char name_buffer_temp[NAME_SIZE];
	char msg_buffer_temp[MSG_SIZE];
	char chat_msg_buffer[NAME_SIZE + MSG_SIZE + 2];	// +2 for the ": " added to the message

	// Copy data from the MsgWorker msg_buffer struct to temp variables
	m_Msg_Worker.getMsg(name_buffer_temp, msg_buffer_temp);
	int name_length = strlen(name_buffer_temp);

	// Build the message to send to the chat box
	strcpy(chat_msg_buffer, name_buffer_temp);
	strcpy(&chat_msg_buffer[name_length], ": ");
	strcpy(&chat_msg_buffer[name_length+2], msg_buffer_temp);
	
	GUIChat::sendChatMessage(chat_msg_buffer);
}
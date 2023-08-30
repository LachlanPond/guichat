#ifndef GTKMM_GUICHAT_H
#define GTKMM_GUICHAT_H

#include <gtkmm/button.h>
#include <gtkmm/box.h>
#include <gtkmm/textview.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <gtkmm/frame.h>
#include <gtkmm/window.h>

class GUIChat : public Gtk::Window {
public:
	GUIChat();

protected:
	// Signal handlers:

	// Member widgets:
	Gtk::Box m_VBox;
	Gtk::Box m_Name_HBox;
	Gtk::Box m_IP_HBox;
	Gtk::Label m_Name_Label;
	Gtk::Label m_IP_Label;
	Gtk::Entry m_Name_Entry;
	Gtk::Entry m_IP_Entry;
	Gtk::Entry m_Msg_Entry;
	Gtk::Frame m_Chat_Frame;
	Gtk::TextView m_Chat_TextView;
	Gtk::Button m_Connect_Button;
};

#endif // GTKMM_GUICHAT_H
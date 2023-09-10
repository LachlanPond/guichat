#include "guichat.h"
#include <gtkmm/application.h>

int main(int argc, char* argv[]) {
	auto app = Gtk::Application::create("org.guichat.main");
	return app->make_window_and_run<GUIChat>(argc,argv);
}
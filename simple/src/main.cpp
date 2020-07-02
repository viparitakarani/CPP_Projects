#include <iostream>
using namespace std;

int main()
{    
    int number;

    cout << "Enter an integer: ";
    cin >> number;

    cout << "You entered " << number <<"\n";    
    return 0;
}

/*#include "../include/helloworld.h"
#include <gtkmm/application.h>

int main (int argc, char *argv[])
{  auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

  HelloWorld helloworld;

  Shows the window and returns when it is closed.
  return app->run(helloworld);/
}*/

/*#include <gtkmm.h>

int main(int argc, char *argv[])
{
  auto app =
    Gtk::Application::create(argc, argv,
      "org.gtkmm.examples.base");

  Gtk::Window window;
  window.set_default_size(200, 200);

  return app->run(window);
}*/
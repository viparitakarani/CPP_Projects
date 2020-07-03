// File: helloworld.cc

#include "../include/helloworld.h"
#include <iostream>

//HelloWorld Class inherit the Gtk::Window Class
HelloWorld::HelloWorld()//: m_button("Hello World")   button constructor can be called like this
{
  m_button.add_pixlabel("m_button_id","Hello World");
  m_button.set_label("Hello!!!");
  m_button.set_border_width(10);

  // When the button receives the "clicked" signal, it will call the
  // on_button_clicked() method defined below.
  // access on_button_clicked from statick object of the HeloWorld class
  m_button.signal_clicked().connect( sigc::mem_fun(*this, &HelloWorld::on_button_clicked) );

  // The final step is to display this newly created widget...
  //m_button.show();  // no need anymore cuz we call show_all_children() from the window object


  // This packs the button into the Window (a container). Window.add(Button)
  this->add(m_button);  //this use pointer that's why call like this->
  this->set_title("Window Hello World!");
  this->show_all_children();

  
}


HelloWorld::~HelloWorld()
{
}

void HelloWorld::on_button_clicked()
{
  std::cout << "Hello World" << std::endl;
}
#ifndef qdude_PORT_WINDOW_H
#define qdude_PORT_WINDOW_H

#include <QDialog>
#include "ui_Port_window.h"
#include "main_window.hpp"

namespace qdude {

class PortWindow : public QDialog {
Q_OBJECT

public:
	PortWindow(int argc, char** argv, MainWindow *main_window, QWidget *parent = 0);
	~PortWindow();

public Q_SLOTS:
	/******************************************
	** Auto-connections (connectSlotsByName())
	*******************************************/
    void on_buttonBox_accepted();
    //void on_Close_clicked(bool check );

private:
	Ui::PortWindowDesign ui;
	MainWindow *m_main_window;

};

}  // namespace qdude

#endif // qdude_USB_WINDOW_H

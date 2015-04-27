/**
 * @file /include/qdude/main_window.hpp
 *
 * @brief Qt based gui for qdude.
 *
 * @date November 2010
 **/
#ifndef qdude_MAIN_WINDOW_H
#define qdude_MAIN_WINDOW_H

/*****************************************************************************
** Includes
*****************************************************************************/
#include <string>
#include <QtGui/QMainWindow>
#include "ui_main_window.h"
#include "ui_USB_window.h"
#include "qnode.hpp"

/*****************************************************************************
** Namespace
*****************************************************************************/

namespace qdude {

/*****************************************************************************
** Interface [MainWindow]
*****************************************************************************/
/**
 * @brief Qt central, all operations relating to the view part here.
 */
class MainWindow : public QMainWindow {
Q_OBJECT

public:
	MainWindow(int argc, char** argv, QWidget *parent = 0);
	~MainWindow();

	void ReadSettings(); // Load up qt program settings at startup
	void WriteSettings(); // Save qt program settings when closing

	void closeEvent(QCloseEvent *event); // Overloaded function
	void showNoMasterMessage();
	std::string m_port;
public Q_SLOTS:
	/******************************************
	** Auto-connections (connectSlotsByName())
	*******************************************/
    void on_Go_clicked(bool check );
    void on_Close_clicked(bool check );
    void on_Locations_currentIndexChanged(int index);
    void on_actionMetadata_From_File_triggered();
    void on_actionMetadata_From_USB_triggered();
    void on_actionPort_triggered();
    void on_actionPose_Estimate_triggered();

private:
	Ui::MainWindowDesign ui;
	QNode qnode;
};	

}  // namespace qdude

#endif // qdude_MAIN_WINDOW_H

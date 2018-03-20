#include <QDebug>
#include <QFileDialog>

#include "CanUpdaterWindow.h"
#include "UiCanUpdaterWindow.h"

CanUpdaterWindow::CanUpdaterWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CanUpdaterWindow)
{
    ui->setupUi(this);

    // Disable text box so it's only modifiable with browse function
    ui->firmwareImageText->setEnabled(false);

    // Disable upload button until user enters something
    ui->uploadButton->setEnabled(false);

    // Setup button connections
    connect(ui->uploadButton,
    		SIGNAL( clicked() ),
			this,
			SLOT( handleFirmwareUpdate() ));
    connect(ui->firmwareImageText,
    		SIGNAL( textChanged() ),
			this,
			SLOT( textBoxChanged() ));
    connect(ui->browseButton,
    		SIGNAL( clicked() ),
			this,
			SLOT( browseImages() ));
    connect(ui->closeButton,
    		SIGNAL( clicked() ),
			this,
			SLOT( reject() ));
}

CanUpdaterWindow::~CanUpdaterWindow()
{
    delete ui;
}

void CanUpdaterWindow::textBoxChanged()
{
	qDebug() << "CanUpdaterWindow::textBoxChanged()";

	/*
	 * Unable upload button if there is any text in the box,
	 * otherwise leave it disabled.
	 */
	if (ui->firmwareImageText->toPlainText().isEmpty())
	{
		ui->uploadButton->setEnabled(false);
	}
	else
	{
		ui->uploadButton->setEnabled(true);
	}
}

void CanUpdaterWindow::browseImages()
{
	qDebug() << "CanUpdaterWindow::browseImages()";

	// Prompt user to select firmware image
	firmwareFilePath_ = QFileDialog::getOpenFileName(
			this,
			tr("Open Firmware Image"),
			QDir::currentPath(),
			tr("Binary files (*.bin);;All files (*.*)"));

	// Set firmware image
	ui->firmwareImageText->setPlainText(firmwareFilePath_);
}

void CanUpdaterWindow::handleFirmwareUpdate()
{
	qDebug() << "CanUpdaterWindow::handleFirmwareUpdate()";
	QDialog::accept();
}

std::string CanUpdaterWindow::getFirmwarePath()
{
	return firmwareFilePath_.toStdString();
}

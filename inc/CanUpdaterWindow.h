#ifndef CANUPDATERWINDOW_H
#define CANUPDATERWINDOW_H

#include <QDialog>

namespace Ui {
class CanUpdaterWindow;
}

/**
 * This class extends QDialog. It prompts the user to enter or
 * browse for the image file path. Clicking upload should start
 * uploading the firmware file via CAN nodes.
 */
class CanUpdaterWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CanUpdaterWindow(QWidget *parent = 0);
    ~CanUpdaterWindow();

    /**
     * Returns String representation of firmware file path.
     */
    std::string getFirmwarePath();

public slots:

	/**
	 * This method will execute when text box is changed.
	 * Enables or disables upload button.
	 */
	void textBoxChanged();

	/**
	 * This method will execute when user clicks on browse.
	 * It will prompt the user to find the firmware image.
	 */
	void browseImages();

	/**
	 * If this method executes it means user entered a path in the textbox.
	 */
	void handleFirmwareUpdate();

private:

    /**
     * QDialog UI handle.
     */
    Ui::CanUpdaterWindow *ui;

	/**
	 * Internal reference to the firmware file path.
	 */
	QString firmwareFilePath_;
};

#endif // CANUPDATERWINDOW_H

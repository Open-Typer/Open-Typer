#ifndef SIMPLECOLORDIALOG_H
#define SIMPLECOLORDIALOG_H

#include <QDialog>

namespace Ui {
class SimpleColorDialog;
}

class SimpleColorDialog : public QDialog
{
	Q_OBJECT
	public:
		explicit SimpleColorDialog(QWidget *parent = nullptr);
		~SimpleColorDialog();
		int redColor, greenColor, blueColor;
		void setColor(int r, int g, int b);
		void updateColor(void);

	private:
		Ui::SimpleColorDialog *ui;
	
	private slots:
		void setRedColorValue(int val);
		void setGreenColorValue(int val);
		void setBlueColorValue(int val);
		void setRedColor(void);
		void setBrownColor(void);
		void setOrangeColor(void);
		void setYellowColor(void);
		void setGreenColor(void);
		void setLightBlueColor(void);
		void setBlueColor(void);
		void setPinkColor(void);
		void setPurpleColor(void);
		void setBlackColor(void);
		void setGrayColor(void);
		void setWhiteColor(void);
};

#endif // SIMPLECOLORDIALOG_H

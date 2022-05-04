/*
 * exportdialog.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2022 - adazem009
 *
 * Open-Typer is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Open-Typer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Open-Typer. If not, see <http://www.gnu.org/licenses/>.
 */

#include "exportdialog.h"
#include "ui_exportdialog.h"

/*! Constructs exportDialog. */
exportDialog::exportDialog(QString text, QVariantMap result, QList<QVariantMap> mistakes, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::exportDialog),
	inputText(text),
	performanceResult(result),
	recordedMistakes(mistakes)
{
	ui->setupUi(this);
	// Set input text
	QString finalText = "<body>";
	QStringList lines = inputText.split('\n');
	int longestLineLength = 0;
	for(int i=0; i < lines.count(); i++)
	{
		if(lines[i].count() > longestLineLength)
			longestLineLength = lines[i].count();
	}
	QMap<int, QVariantMap*> mistakesMap;
	for(int i=0; i < recordedMistakes.count(); i++)
		mistakesMap[recordedMistakes[i]["pos"].toInt()] = &recordedMistakes[i];
	int pos = 0;
	for(int i=0; i < lines.count(); i++)
	{
		if(i > 0)
		{
			pos++;
			finalText += "<br>";
		}
		QString line = lines[i];
		int lineMistakes = 0;
		for(int i2=0; i2 < line.count(); i2++)
		{
			QString append = QString(line[i2]).toHtmlEscaped().replace(" ", "&nbsp;");
			if(mistakesMap.contains(pos))
			{
				lineMistakes++;
				finalText += "<u>" + append + "</u>";
			}
			else
				finalText += append;
			pos++;
		}
		finalText += QString("&nbsp;").repeated(longestLineLength - line.count() + 4) + QString("/").repeated(lineMistakes);
	}
	finalText += "</body>";
	QString exportHtml = finalText;
	ui->exportText->setHtml(exportHtml);
	// Set font
	QFont textFont = themeEngine::font();
	textFont.setPointSize(12);
	ui->exportText->setFont(textFont);
	// Fill table
	ui->exportTable->setRowCount(10);
	ui->exportTable->setColumnCount(4);
	ui->exportTable->setSpan(0, 0, 1, 4);
	ui->exportTable->setSpan(2, 2, 2, 1);
	ui->exportTable->setSpan(2, 3, 2, 1);
	ui->exportTable->setSpan(4, 2, 4, 1);
	ui->exportTable->setSpan(4, 3, 4, 1);
	ui->exportTable->setSpan(8, 2, 2, 1);
	ui->exportTable->setSpan(8, 3, 2, 1);
	updateTable();
	// Connections
	connect(ui->printButton, &QToolButton::clicked, this, &exportDialog::printResult);
	connect(ui->nameEdit, &QLineEdit::textChanged, this, &exportDialog::updateTable);
	connect(ui->classEdit, &QLineEdit::textChanged, this, &exportDialog::updateTable);
	connect(ui->numberEdit, &QLineEdit::textChanged, this, &exportDialog::updateTable);
	connect(ui->markEdit, &QLineEdit::textChanged, this, &exportDialog::updateTable);
	connect(ui->closeButton, &QPushButton::clicked, this, &QDialog::close);
#ifdef Q_OS_WASM
	ui->printButton->hide();
#endif
}

/*! Destroys the exportDialog object. */
exportDialog::~exportDialog()
{
	delete ui;
}

/*! Updates the table. */
void exportDialog::updateTable(void)
{
	ui->exportTable->clearContents();
	QFont boldFont;
	boldFont.setBold(true);
	boldFont.setPointSize(14);
	QFont boldFont2;
	boldFont2.setBold(true);
	QTableWidgetItem *item;
	// Caption
	item = new QTableWidgetItem(tr("Typewriting performance result"));
	item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	item->setFont(boldFont);
	ui->exportTable->setItem(0, 0, item);
	// Name
	item = new QTableWidgetItem(tr("Name: %1").arg(ui->nameEdit->text()));
	item->setFont(boldFont2);
	ui->exportTable->setItem(1, 0, item);
	// Class
	item = new QTableWidgetItem(tr("Class: %1").arg(ui->classEdit->text()));
	item->setFont(boldFont2);
	ui->exportTable->setItem(1, 1, item);
	// Date
	item = new QTableWidgetItem(tr("Date: %1").arg(QDate::currentDate().toString()));
	item->setFont(boldFont2);
	ui->exportTable->setItem(1, 2, item);
	// Number
	item = new QTableWidgetItem(tr("Number: %1").arg(ui->numberEdit->text()));
	item->setFont(boldFont2);
	ui->exportTable->setItem(1, 3, item);
	// Number of hits in finished lines
	ui->exportTable->setItem(2, 0, new QTableWidgetItem(tr("Number of hits in finished lines")));
	ui->exportTable->setItem(2, 1, new QTableWidgetItem(QString::number(performanceResult["finishedLinesHits"].toInt())));
	// Number of hits in unfinished lines
	ui->exportTable->setItem(3, 0, new QTableWidgetItem(tr("Number of hits in unfinished lines")));
	ui->exportTable->setItem(3, 1, new QTableWidgetItem(QString::number(performanceResult["unfinishedLinesHits"].toInt())));
	// Number of gross hits
	item = new QTableWidgetItem(tr("Number of gross hits"));
	item->setFont(boldFont2);
	ui->exportTable->setItem(4, 0, item);
	ui->exportTable->setItem(4, 1, new QTableWidgetItem(QString::number(performanceResult["grossHits"].toInt())));
	// Number of mistakes
	ui->exportTable->setItem(5, 0, new QTableWidgetItem(tr("Number of mistakes")));
	ui->exportTable->setItem(5, 1, new QTableWidgetItem(QString::number(performanceResult["mistakes"].toInt())));
	// Mistake penalty
	ui->exportTable->setItem(6, 0, new QTableWidgetItem(tr("Mistake penalty")));
	ui->exportTable->setItem(6, 1, new QTableWidgetItem(QString::number(performanceResult["penalty"].toInt())));
	// Number of penalty hits
	ui->exportTable->setItem(7, 0, new QTableWidgetItem(tr("Number of penalty hits")));
	int penaltyHits = performanceResult["penalty"].toInt() * performanceResult["mistakes"].toInt();
	ui->exportTable->setItem(7, 1, new QTableWidgetItem(QString::number(penaltyHits)));
	// Number of net hits
	ui->exportTable->setItem(8, 0, new QTableWidgetItem(tr("Number of net hits")));
	ui->exportTable->setItem(8, 1, new QTableWidgetItem(QString::number(performanceResult["netHits"].toInt())));
	// Number of net hits per minute
	item = new QTableWidgetItem(tr("Number of net hits per minute"));
	item->setFont(boldFont2);
	ui->exportTable->setItem(9, 0, item);
	ui->exportTable->setItem(9, 1, new QTableWidgetItem(QString::number(performanceResult["netHitsPerMinute"].toDouble())));
	// Inaccuracy
	ui->exportTable->setItem(2, 2, new QTableWidgetItem(tr("Inaccuracy")));
	double inaccuracy = (performanceResult["mistakes"].toInt() * 100.0) / (double) performanceResult["grossHits"].toInt();
	ui->exportTable->setItem(2, 3, new QTableWidgetItem(QString::number(inaccuracy)));
	// Achieved performance
	ui->exportTable->setItem(4, 2, new QTableWidgetItem(tr("Achieved performance")));
	ui->exportTable->setItem(4, 3, new QTableWidgetItem(QString::number((int) performanceResult["netHitsPerMinute"].toDouble())));
	// Mark
	item = new QTableWidgetItem(tr("Mark"));
	item->setFont(boldFont);
	item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	ui->exportTable->setItem(8, 2, item);
	ui->exportTable->setItem(8, 3, new QTableWidgetItem(ui->markEdit->text()));
	// Adjust size
	ui->exportTable->resizeColumnsToContents();
	ui->exportTable->resizeRowsToContents();
}

/*! Prints the result. */
void exportDialog::printResult(void)
{
#ifndef Q_OS_WASM
	// Set up printer
	QPrinter printer(QPrinter::HighResolution);
	QPrinter *printerPtr = &printer;
	QPrintPreviewDialog dialog(&printer);
	connect(&dialog, &QPrintPreviewDialog::paintRequested, this, [this, printerPtr]() {
		bool visibility = isVisible();
		if(!visibility)
		{
			show();
			ui->exportTable->adjustSize();
			adjustSize();
		}
		// Print
		printerPtr->setPageMargins(QMarginsF(25, 25, 15, 25), QPageLayout::Millimeter);
		QPainter painter;
		painter.begin(printerPtr);
		painter.setFont(ui->exportText->font());
		QTextDocument *document = ui->exportText->document()->clone();
		document->documentLayout()->setPaintDevice(printerPtr);
		document->setDefaultStyleSheet("body { color: black; }");
		int fontHeight = QFontMetrics(painter.font(), printerPtr).height();
		QStringList lines = document->toHtml().split("<br>");
		int relativeLine = 0, page = 0;
		for(int i=0; i < lines.count(); i++)
		{
			if(fontHeight*relativeLine > printerPtr->pageRect(QPrinter::DevicePixel).height())
			{
				printerPtr->newPage();
				relativeLine = 0;
				page++;
			}
			document->setHtml(lines[i]);
			painter.resetTransform();
			painter.translate(0, fontHeight*relativeLine);
			document->drawContents(&painter);
			relativeLine++;
		}
		painter.resetTransform();
		double scale = printerPtr->pageRect(QPrinter::DevicePixel).width() / double(ui->exportTable->width());
		painter.scale(scale, scale);
		int tablePos = (printerPtr->pageRect(QPrinter::DevicePixel).height() - (ui->exportTable->height()*scale)) / scale;
		if(((fontHeight*relativeLine) / scale > tablePos) || (page == 0))
			printerPtr->newPage();
		ui->exportTable->render(&painter, QPoint(0, tablePos));
		painter.end();
		setVisible(visibility);
	});
	dialog.exec();
#endif // Q_OS_WASM
}

/*! Sets the student name. */
void exportDialog::setStudentName(QString name)
{
	ui->nameEdit->setText(name);
}

/*! Returns the student name. */
QString exportDialog::studentName(void)
{
	return ui->nameEdit->text();
}

/*! Sets the class name. */
void exportDialog::setClassName(QString name)
{
	ui->classEdit->setText(name);
}

/*! Returns the class name. */
QString exportDialog::className(void)
{
	return ui->classEdit->text();
}

/*! Sets the exercise number. */
void exportDialog::setNumber(QString number)
{
	ui->numberEdit->setText(number);
}

/*! Returns the exercise number. */
QString exportDialog::number(void)
{
	return ui->numberEdit->text();
}

/*! Sets the mark. */
void exportDialog::setMark(QString mark)
{
	ui->markEdit->setText(mark);
}

/*! Returns the mark. */
QString exportDialog::mark(void)
{
	return ui->markEdit->text();
}

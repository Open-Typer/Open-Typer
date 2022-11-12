/*
 * ExportDialog.cpp
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

#include "ExportDialog.h"
#include "ui_ExportDialog.h"

/*! Constructs ExportDialog. */
ExportDialog::ExportDialog(QString text, QVariantMap result, QList<QVariantMap> mistakes, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ExportDialog),
	inputText(text),
	performanceResult(result),
	recordedMistakes(mistakes)
{
	ui->setupUi(this);
	// Set input text
	QString finalText = "";
	QStringList lines = inputText.split('\n');
	int longestLineLength = 0;
	for(int i = 0; i < lines.count(); i++)
	{
		if(lines[i].count() > longestLineLength)
			longestLineLength = lines[i].count();
	}
	QMap<int, QVariantMap *> mistakesMap;
	for(int i = 0; i < recordedMistakes.count(); i++)
		mistakesMap[recordedMistakes[i]["pos"].toInt()] = &recordedMistakes[i];
	int pos = 0;
	for(int i = 0; i < lines.count(); i++)
	{
		QString line = lines[i];
		int lineMistakes = 0;
		for(int i2 = 0; i2 <= line.count(); i2++)
		{
			QString append;
			if(i2 < line.count())
				append = QString(line[i2]).toHtmlEscaped().replace(" ", "&nbsp;");
			else
				append = "";
			if(mistakesMap.contains(pos))
			{
				QVariantMap *currentMistake = mistakesMap[pos];
				if(!(currentMistake->contains("disable") && currentMistake->value("disable").toBool()))
					lineMistakes++;
				if(append == "")
					append.prepend("&nbsp;");
				finalText += "<u>" + append + "</u>";
			}
			else
				finalText += append;
			pos++;
		}
		finalText += QString("&nbsp;").repeated(longestLineLength - line.count() + 4) + QString("/").repeated(lineMistakes);
		finalText += "<br>";
	}
	exportHtml = finalText;
	ui->exportText->setHtml(exportHtml);
	// Set font
	QFont textFont = ThemeEngine::font();
	textFont.setPointSize(12);
	ui->exportText->setFont(textFont);
	// Fill table
	ui->exportTable->setRowCount(9);
	ui->exportTable->setColumnCount(4);
	ui->exportTable->setSpan(0, 0, 1, 4);
	ui->exportTable->setSpan(2, 2, 2, 1);
	ui->exportTable->setSpan(2, 3, 2, 1);
	ui->exportTable->setSpan(4, 2, 2, 1);
	ui->exportTable->setSpan(4, 3, 2, 1);
	ui->exportTable->setSpan(6, 2, 3, 1);
	ui->exportTable->setSpan(6, 3, 3, 1);
	updateTable();
	// Connections
	connect(ui->printButton, &QToolButton::clicked, this, &ExportDialog::printResult);
	connect(ui->nameEdit, &QLineEdit::textChanged, this, &ExportDialog::updateTable);
	connect(ui->classEdit, &QLineEdit::textChanged, this, &ExportDialog::updateTable);
	connect(ui->numberEdit, &QLineEdit::textChanged, this, &ExportDialog::updateTable);
	connect(ui->gradeEdit, &QLineEdit::textChanged, this, &ExportDialog::updateTable);
	connect(ui->closeButton, &QPushButton::clicked, this, &QDialog::close);
#ifdef Q_OS_WASM
	ui->printButton->hide();
#endif
}

/*! Destroys the ExportDialog object. */
ExportDialog::~ExportDialog()
{
	delete ui;
}

/*! Updates the table. */
void ExportDialog::updateTable(void)
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
	// Number of gross hits
	item = new QTableWidgetItem(tr("Number of gross hits"));
	item->setFont(boldFont2);
	ui->exportTable->setItem(2, 0, item);
	ui->exportTable->setItem(2, 1, new QTableWidgetItem(QString::number(performanceResult["grossHits"].toInt())));
	// Number of mistakes
	ui->exportTable->setItem(3, 0, new QTableWidgetItem(tr("Number of mistakes")));
	ui->exportTable->setItem(3, 1, new QTableWidgetItem(QString::number(performanceResult["mistakes"].toInt())));
	// Mistake penalty
	ui->exportTable->setItem(4, 0, new QTableWidgetItem(tr("Mistake penalty")));
	ui->exportTable->setItem(4, 1, new QTableWidgetItem(QString::number(performanceResult["penalty"].toInt())));
	// Number of penalty hits
	ui->exportTable->setItem(5, 0, new QTableWidgetItem(tr("Number of penalty hits")));
	int penaltyHits = performanceResult["penalty"].toInt() * performanceResult["mistakes"].toInt();
	ui->exportTable->setItem(5, 1, new QTableWidgetItem(QString::number(penaltyHits)));
	// Number of net hits
	ui->exportTable->setItem(6, 0, new QTableWidgetItem(tr("Number of net hits")));
	ui->exportTable->setItem(6, 1, new QTableWidgetItem(QString::number(performanceResult["netHits"].toInt())));
	// Time
	ui->exportTable->setItem(7, 0, new QTableWidgetItem(tr("Time (min)", "How many minutes the exercise took")));
	ui->exportTable->setItem(7, 1, new QTableWidgetItem(QString::number(performanceResult["time"].toDouble(), 'g', 2)));
	// Number of net hits per minute
	item = new QTableWidgetItem(tr("Number of net hits per minute"));
	item->setFont(boldFont2);
	ui->exportTable->setItem(8, 0, item);
	ui->exportTable->setItem(8, 1, new QTableWidgetItem(QString::number(performanceResult["netHitsPerMinute"].toDouble())));
	// Inaccuracy
	ui->exportTable->setItem(2, 2, new QTableWidgetItem(tr("Inaccuracy")));
	double inaccuracy = (performanceResult["mistakes"].toInt() * 100.0) / (double) performanceResult["grossHits"].toInt();
	ui->exportTable->setItem(2, 3, new QTableWidgetItem(QString::number(inaccuracy)));
	// Achieved performance
	ui->exportTable->setItem(4, 2, new QTableWidgetItem(tr("Achieved performance")));
	ui->exportTable->setItem(4, 3, new QTableWidgetItem(QString::number((int) performanceResult["netHitsPerMinute"].toDouble())));
	// Grade
	item = new QTableWidgetItem(tr("Grade"));
	item->setFont(boldFont);
	item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	ui->exportTable->setItem(6, 2, item);
	ui->exportTable->setItem(6, 3, new QTableWidgetItem(ui->gradeEdit->text()));
	// Adjust size
	ui->exportTable->resizeColumnsToContents();
	ui->exportTable->resizeRowsToContents();
}

/*! Prints the result. */
void ExportDialog::printResult(void)
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
		QTextDocument *document = ui->exportText->document()->clone(this);
		document->documentLayout()->setPaintDevice(printerPtr);
		document->setDefaultStyleSheet("body { color: black; }");
		QFont font = document->defaultFont();
		font.setPointSize(50);
		document->setDefaultFont(font);
		double textScale = printerPtr->pageRect(QPrinter::DevicePixel).width() / double(document->size().width());
		int fontHeight = QFontMetrics(font, printerPtr).height();
		QStringList lines = exportHtml.split("<br>");
		int relativeLine = 0, page = 0, fromPage = printerPtr->fromPage() - 1, toPage = printerPtr->toPage() - 1;
		for(int i = 0; i < lines.count(); i++)
		{
			int rangeEnd = toPage;
			if(rangeEnd == -1)
				rangeEnd = page + 1;
			if(fontHeight * textScale * (relativeLine + 1) > printerPtr->pageRect(QPrinter::DevicePixel).height())
			{
				if(((page + 1 >= fromPage) && (page + 1 <= rangeEnd)) && ((page >= fromPage) && (page <= rangeEnd)))
					printerPtr->newPage();
				relativeLine = 0;
				page++;
			}
			document->setHtml("<body>" + lines[i] + "</body>");
			if((page >= fromPage) && (page <= rangeEnd))
			{
				painter.resetTransform();
				painter.scale(textScale, textScale);
				painter.translate(0, fontHeight * relativeLine);
				document->drawContents(&painter);
			}
			relativeLine++;
		}
		painter.resetTransform();
		double scale = printerPtr->pageRect(QPrinter::DevicePixel).width() / double(ui->exportTable->width());
		painter.scale(scale, scale);
		int tablePos = (printerPtr->pageRect(QPrinter::DevicePixel).height() - (ui->exportTable->height() * scale)) / scale;
		int rangeEnd = toPage;
		if(rangeEnd == -1)
			rangeEnd = page + 1;
		if(((fontHeight * relativeLine) / scale > tablePos) || (page == 0))
		{
			if(((page + 1 >= fromPage) && (page + 1 <= rangeEnd)) && ((page >= fromPage) && (page <= rangeEnd)))
				printerPtr->newPage();
			page++;
		}
		if((page >= fromPage) && (page <= rangeEnd))
			ui->exportTable->render(&painter, QPoint(0, tablePos));
		painter.end();
		setVisible(visibility);
	});
	dialog.exec();
#endif // Q_OS_WASM
}

/*! Sets the student name. */
void ExportDialog::setStudentName(QString name)
{
	ui->nameEdit->setText(name);
}

/*! Returns the student name. */
QString ExportDialog::studentName(void)
{
	return ui->nameEdit->text();
}

/*! Sets the class name. */
void ExportDialog::setClassName(QString name)
{
	ui->classEdit->setText(name);
}

/*! Returns the class name. */
QString ExportDialog::className(void)
{
	return ui->classEdit->text();
}

/*! Sets the exercise number. */
void ExportDialog::setNumber(QString number)
{
	ui->numberEdit->setText(number);
}

/*! Returns the exercise number. */
QString ExportDialog::number(void)
{
	return ui->numberEdit->text();
}

/*! Sets the grade. */
void ExportDialog::setGrade(QString grade)
{
	ui->gradeEdit->setText(grade);
}

/*! Returns the grade. */
QString ExportDialog::grade(void)
{
	return ui->gradeEdit->text();
}

/*
 * ExportWidget.cpp
 * This file is part of Open-Typer
 *
 * Copyright (C) 2023 - adazem009
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

#include <QObject>
#ifndef Q_OS_WASM
#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QPainter>
#include <QAbstractTextDocumentLayout>
#endif // Q_OS_WASM
#include "ExportProvider.h"

/*! Constructs ExportProvider. */
ExportProvider::ExportProvider(QObject *parent) :
	QObject(parent) { }

/*! The ExerciseValidator used to get the input text and mistakes. */
IExerciseValidator *ExportProvider::validator(void)
{
	return m_validator;
}

void ExportProvider::setValidator(IExerciseValidator *validator)
{
	m_validator = validator;
	if(!validator)
		return;
	QString inputText = validator->inputText();
	auto mistakes = validator->mistakes();
	QString finalText = "";
	QStringList lines = inputText.split('\n');
	int longestLineLength = 0;
	for(int i = 0; i < lines.count(); i++)
	{
		if(lines[i].length() > longestLineLength)
			longestLineLength = lines[i].length();
	}
	QMap<int, MistakeRecord *> mistakesMap;
	for(int i = 0; i < mistakes.length(); i++)
		mistakesMap[mistakes[i].position()] = &mistakes[i];
	int pos = 0;
	for(int i = 0; i < lines.count(); i++)
	{
		QString line = lines[i];
		int lineMistakes = 0;
		for(int i2 = 0; i2 <= line.length(); i2++)
		{
			QString append;
			if(i2 < line.length())
				append = QString(line[i2]).toHtmlEscaped().replace(" ", "&nbsp;");
			else
				append = "";
			if(mistakesMap.contains(pos))
			{
				MistakeRecord *currentMistake = mistakesMap[pos];
				if(currentMistake->isEnabled())
					lineMistakes++;
				if(append == "")
					append.prepend("&nbsp;");
				finalText += "<u>" + append + "</u>";
			}
			else
				finalText += append;
			pos++;
		}
		finalText += QString("&nbsp;").repeated(longestLineLength - line.length() + 4) + QString("/").repeated(lineMistakes);
		finalText += "<br>";
	}
	m_exportText = finalText;
	emit exportTextChanged(m_exportText);
	emit validatorChanged(validator);
}

/*! The exported input text. */
QString ExportProvider::exportText(void)
{
	return m_exportText;
}

/*! The ExportTable that is going to be printed. */
ExportTable *ExportProvider::table(void)
{
	return m_table;
}

void ExportProvider::setTable(ExportTable *table)
{
	m_table = table;
	emit tableChanged(table);
}

/*! Prints the exported text and table. */
void ExportProvider::print(void)
{
#ifndef Q_OS_WASM
	// Set up printer
	QPrinter printer(QPrinter::HighResolution);
	QPrinter *printerPtr = &printer;
	QPrintPreviewDialog dialog(&printer);
	connect(&dialog, &QPrintPreviewDialog::paintRequested, this, [this, printerPtr]() {
		// Print
		printerPtr->setPageMargins(QMarginsF(25, 25, 15, 25), QPageLayout::Millimeter);
		QPainter painter;
		painter.begin(printerPtr);
		QFont font = themeEngine()->font();
		font.setPointSize(12);
		painter.setFont(font);
		QTextEdit *textEdit = new QTextEdit;
		textEdit->setHtml(m_exportText);
		textEdit->setFont(font);
		textEdit->setLineWrapMode(QTextEdit::NoWrap);
		//textEdit->show();
		textEdit->adjustSize();
		QTextDocument *document = textEdit->document()->clone(this);
		document->documentLayout()->setPaintDevice(printerPtr);
		document->setDefaultStyleSheet("body { color: black; }");
		font = document->defaultFont();
		font.setPointSize(50);
		document->setDefaultFont(font);
		double textScale = printerPtr->pageRect(QPrinter::DevicePixel).width() / double(document->size().width());
		int fontHeight = QFontMetrics(font, printerPtr).height();
		QStringList lines = m_exportText.split("<br>");
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
		m_table->updateStyle(true);
		m_table->setGeometry(0, 0, m_table->contentWidth(), m_table->contentHeight());
		double scale = printerPtr->pageRect(QPrinter::DevicePixel).width() / double(m_table->width());
		painter.scale(scale, scale);
		int tablePos = (printerPtr->pageRect(QPrinter::DevicePixel).height() - (m_table->height() * scale)) / scale;
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
			m_table->render(&painter, QPoint(0, tablePos), QRegion(0, 0, m_table->width(), m_table->height()));
		m_table->updateStyle(false);
		painter.end();
	});
	dialog.exec();
#endif // Q_OS_WASM
}

/*! Prints the exercise text. */
void ExportProvider::printExercise(int lesson, int sublesson, int exercise, QString text)
{
#ifndef Q_OS_WASM
	// Set up printer
	QPrinter printer(QPrinter::HighResolution);
	QPrinter *printerPtr = &printer;
	QPrintPreviewDialog dialog(&printer);
	auto dialogPtr = &dialog;
	QObject::connect(&dialog, &QPrintPreviewDialog::paintRequested, &dialog, [printerPtr, dialogPtr, lesson, sublesson, exercise, text]() {
		// Print
		printerPtr->setPageMargins(QMarginsF(25, 25, 15, 25), QPageLayout::Millimeter);
		QPainter painter;
		painter.begin(printerPtr);
		QFont font = themeEngine()->font();
		painter.setFont(font);
		QTextEdit textEdit; // is there any other way to get rid of line wrap?
		textEdit.setPlainText(text);
		textEdit.setLineWrapMode(QTextEdit::NoWrap);
		QTextDocument *document = textEdit.document()->clone(dialogPtr);
		QString title = "";
		if(lesson + sublesson + exercise != 0)
			title = QString("<u>%1 / %2 / %3</u><br><br>").arg(ConfigParser::lessonTr(lesson), ConfigParser::sublessonName(sublesson), ConfigParser::exerciseTr(exercise));
		document->setHtml(QString("<body>%1%2</body>").arg(title, text.toHtmlEscaped().replace("\n", "<br>")));
		font.setPointSize(50);
		document->adjustSize();
		document->setDefaultFont(font);
		document->documentLayout()->setPaintDevice(printerPtr);
		document->setDefaultStyleSheet("body { color: black; }");
		document->setPageSize(printerPtr->pageRect(QPrinter::DevicePixel).size());
		double scale = printerPtr->pageRect(QPrinter::DevicePixel).width() / double(document->size().width());
		int fontHeight = QFontMetrics(painter.font(), printerPtr).height();
		QStringList lines = document->toHtml().split("<br>");
		int relativeLine = 0, page = 0, fromPage = printerPtr->fromPage() - 1, toPage = printerPtr->toPage() - 1;
		for(int i = 0; i < lines.count(); i++)
		{
			int rangeEnd = toPage;
			if(rangeEnd == -1)
				rangeEnd = page + 1;
			if(fontHeight * relativeLine > printerPtr->pageRect(QPrinter::DevicePixel).height())
			{
				if(((page + 1 >= fromPage) && (page + 1 <= rangeEnd)) && ((page >= fromPage) && (page <= rangeEnd)))
					printerPtr->newPage();
				relativeLine = 0;
				page++;
			}
			document->setHtml(lines[i]);
			if((page >= fromPage) && (page <= rangeEnd))
			{
				painter.resetTransform();
				painter.scale(scale, scale);
				painter.translate(0, fontHeight * relativeLine);
				document->drawContents(&painter);
			}
			relativeLine++;
		}
		painter.end();
	});
	dialog.exec();
#endif // Q_OS_WASM
}

/*! Prints the text of a custom exercise. */
void ExportProvider::printExercise(QString text)
{
	printExercise(0, 0, 0, text);
}

/*
 * packeditor.h
 * This file is part of Open-Typer
 *
 * Copyright (C) 2021 - adazem009
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

#ifndef PACKEDITOR_H
#define PACKEDITOR_H

#include <QDialog>
#include <QCloseEvent>
#include "packEditor/packview.h"
#include "packEditor/packselector.h"
#include "core/configfile.h"

namespace Ui {
	class packEditor;
}

/*!
 * \brief The packEditor class is the pack editor window.
 *
 * \image html packEditor.png
 *
 * Usage example:
 * \code
 * packEditor editorWindow;
 * editorWindow.init();
 * editorWindow.exec();
 * \endcode
 *
 * It is possible to create a new file when the editor starts:
 * \code
 * editorWindow.setNewFile(true);
 * editorWindow.init();
 * editorWindow.exec();
 * \endcode
 * Note that init() must be run for the settings to take effect.
 *
 * \see packView
 * \see packSelector
 */
class packEditor : public QDialog
{
	Q_OBJECT
	public:
		explicit packEditor(QWidget *parent = nullptr);
		~packEditor();
		Ui::packEditor *ui;
		void init(void);
		void setNewFile(bool value);
		void setFileName(QString newFileName, QWidget *sourceWidget);
		void fixDuplicates(void);
		int fileID;

	protected:
		void closeEvent(QCloseEvent *event);

	private:
		bool newFile;
		bool closeAll(void);
		QString defaultFileName;

	private slots:
		void createNewFile(void);
		void openFile(void);
		void openPrebuilt(void);
		void close(void);
		void closeTab(int id);
};

#endif // PACKEDITOR_H

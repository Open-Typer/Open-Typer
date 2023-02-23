/*
 * MenuBarManager.qml
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

import QtQuick 2.12
import OpenTyper 1.0

Item {
	signal openToggled()
	signal printToggled()
	signal typingTestToggled()
	signal exerciseHistoryToggled()
	signal timedExToggled()
	signal errorWordsToggled()
	signal reverseTextToggled()
	signal preferencesToggled()
	signal aboutProgramToggled()

	Connections {
		target: AppMenuBar.openAction

		onClicked: {
			openToggled();
		}
	}

	Connections {
		target: AppMenuBar.printAction

		onClicked: {
			printToggled();
		}
	}

	Connections {
		target: AppMenuBar.typingTestAction

		onClicked: {
			typingTestToggled();
		}
	}

	Connections {
		target: AppMenuBar.exerciseHistoryAction

		onClicked: {
			exerciseHistoryToggled();
		}
	}

	Connections {
		target: AppMenuBar.timedExAction

		onClicked: {
			timedExToggled();
		}
	}

	Connections {
		target: AppMenuBar.errorWordsAction

		onClicked: {
			errorWordsToggled();
		}
	}

	Connections {
		target: AppMenuBar.reverseTextAction

		onClicked: {
			reverseTextToggled();
		}
	}

	Connections {
	target: AppMenuBar.preferencesAction

		onClicked: {
			preferencesToggled();
		}
	}

	Connections {
	target: AppMenuBar.aboutProgramAction

		onClicked: {
			aboutProgramToggled();
		}
	}
}

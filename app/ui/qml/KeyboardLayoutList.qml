/*
 * KeyboardLayoutList.qml
 * This file is part of Open-Typer
 *
 * Copyright (C) 2022-2023 - adazem009
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

import QtQuick 2.9
import QtQuick.Controls 2.2
import OpenTyper 1.0

ListView {
	property var items: []
	signal itemsLoaded()
	id: root
	currentIndex: -1
	model: ListModel {
		function loadItems() {
			items = BuiltInPacks.keyboardLayouts();
			clear();
			for(var i = 0; i < items.length; i++)
				append({"name": items[i]});
			if(Settings.containsLessonPack() && (Settings.lessonPack() !== ""))
				currentIndex = items.indexOf(BuiltInPacks.keyboardLayoutForPack(Settings.lessonPack()));
			itemsLoaded();
		}
		Component.onCompleted: loadItems()
	}
	delegate: ItemDelegate {
		text: name
		width: root.width
		highlighted: ListView.isCurrentItem
		onClicked: {
			root.currentIndex = index
		}
	}
	clip: true
}

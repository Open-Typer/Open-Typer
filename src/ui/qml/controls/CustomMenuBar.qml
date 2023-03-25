/*
 * CustomMenuBar.qml
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

import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.5
import QtQuick.Layouts 1.12
import Qt.labs.platform 1.1 as Platform
import OpenTyper 1.0

MenuBar {
	id: root
	readonly property var menuList: AppMenuBar.menus
	property var menuObjects: []

	function getComponentString(typeName) {
		var imports = "import QtQuick 2.12; import QtQuick.Controls 2.5; import Qt.labs.platform 1.1 as Platform;"
		return imports + " Component { " + typeName + " {} }";
	}

	function createMenuBar(parentItem, menuType, menuItemType, menuSeparatorType) {
		for(var i = 0; i < menuList.length; i++)
		{
			var menuParent = parentItem;
			var menuComponent = Qt.createQmlObject(getComponentString(menuType), menuParent);
			var menu = menuComponent.createObject(menuParent);
			menu.title = menuList[i].title;
			menuObjects[menuObjects.length] = menu;
			parentItem.addMenu(menu);
			createMenu(menu, menuList[i].items, menuType, menuItemType, menuSeparatorType);
		}
	}

	function createMenu(parentItem, itemList, menuType, menuItemType, menuSeparatorType) {
		for(var j = 0; j < itemList.length; j++)
		{
			var itemData = itemList[j];
			var itemComponent;
			var item;
			var overrideAddItem = false;
			if(itemData.isSeparator)
			{
				itemComponent = Qt.createQmlObject(getComponentString(menuSeparatorType), parentItem);
				item = itemComponent.createObject(null);
			}
			else if(itemData.submenu === null)
			{
				itemComponent = Qt.createQmlObject(getComponentString(menuItemType), parentItem);
				item = itemComponent.createObject(null);
				item.text = itemData.text;
				item.checkable = itemData.checkable;
				item.checked = itemData.checked;
				item.onCheckedChanged.connect(function() { itemData.checked = item.checked; });
				itemData.onCheckedChanged.connect(function() { item.checked = itemData.checked; });
				if(typeof itemData.onClicked != "undefined")
					item.onTriggered.connect(itemData.onClicked);
			}
			else
			{
				itemComponent = Qt.createQmlObject(getComponentString(menuType), parentItem)
				item = itemComponent.createObject(parentItem);
				item.title = itemData.text;
				parentItem.addMenu(item);
				createMenu(item, itemData.submenu.items, menuType, menuItemType, menuSeparatorType);
				overrideAddItem = true;
			}
			if(!overrideAddItem)
				parentItem.addItem(item);
		}
	}

	background: Rectangle {
		color: ThemeEngine.panelColor
	}

	delegate: MenuBarItem {
		id: menuBarItem
		function replaceText(txt)
		{
			var index = txt.indexOf("&");
			if(index >= 0)
				txt = txt.replace(txt.substr(index, 2), ("<u>" + txt.substr(index + 1, 1) +"</u>"));
			return txt;
		}
		implicitHeight: contentItem.implicitHeight + topPadding + bottomPadding
		leftPadding: 10
		rightPadding: 10
		topPadding: 5
		bottomPadding: 5
		font.pointSize: 10
		Material.background: Qt.rgba(0, 0, 0, 0)
		Material.foreground: ThemeEngine.theme === ThemeEngine.DarkTheme ? "white" : "black"
		contentItem: Label {
			text: replaceText(menuBarItem.text)
			font: menuBarItem.font
		}
		onDoubleClicked: clicked()
	}

	function reload() {
		if(QmlUtils.nativeMenuBar())
		{
			root.visible = false;
			return;
		}
		var oldObjects = [];
		for(var i = 0; i < menuObjects.length; i++)
			oldObjects.push(menuObjects[i]);
		menuObjects = [];
		createMenuBar(root, "CustomMenu", "CustomMenuItem", "MenuSeparator");
		for(i = 0; i < oldObjects.length; i++)
			removeMenu(oldObjects[i]);
	}

	Connections {
		target: QmlUtils
		onMenuBarReloadTriggered: {
			for(var i = 0; i < root.count; i++)
				root.menuAt(i).close();
			root.reload();
		}
	}

	Component.onCompleted: reload();

	onEnabledChanged: platformMenuBar.reload();

	Platform.MenuBar {
		id: platformMenuBar
		function reload() {
			clear();
			if(QmlUtils.nativeMenuBar() && root.enabled)
				createMenuBar(platformMenuBar, "Platform.Menu", "Platform.MenuItem", "Platform.MenuSeparator");
		}

		Connections {
			target: QmlUtils
			onMenuBarReloadTriggered: {
				platformMenuBar.reload();
			}
		}

		Component.onCompleted: reload();
	}
}

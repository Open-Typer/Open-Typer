/*
 * CustomMenuBar.qml
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

import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Layouts 1.3
import Qt.labs.platform 1.0 as Platform
import OpenTyper 1.0

Panel {
	id: root
	property list<Item> menuList: [
		Item {
			property string text: qsTr("File");
			property list<Item> itemList: [
				Item {
					property string type: "item"
					property string text: qsTr("New...")
				},
				Item {
					property string type: "item"
					property string text: qsTr("Open...")
				},
				Item {
					property string type: "separator"
				},
				Item {
					property string type: "item"
					property string text: qsTr("Quit")
					function onClicked() { Qt.quit(); }
				}
			]
		}
	]

	function getComponentString(typeName) {
		var imports = "import QtQuick 2.9; import QtQuick.Controls 2.2; import Qt.labs.platform 1.0 as Platform;"
		return imports + " Component { " + typeName + " {} }";
	}

	function createMenuBar(parentItem, buttonType, menuType, menuItemType, menuSeparatorType) {
		for(var i = 0; i < menuList.length; i++)
		{
			if(buttonType !== "")
			{
				var buttonComponent = Qt.createQmlObject(getComponentString(buttonType), parentItem);
				var button = buttonComponent.createObject(parentItem);
				button.text = menuList[i].text;
			}
			var menuComponent = Qt.createQmlObject(getComponentString(menuType), parentItem);
			var menu = menuComponent.createObject(parentItem);
			if(buttonType === "")
			{
				menu.title = menuList[i].text;
				parentItem.addMenu(menu);
			}
			else
				button.menu = menu;
			createMenu(menu, menuList[i].itemList, buttonType, menuType, menuItemType, menuSeparatorType);
		}
	}

	function createMenu(parentItem, itemList, buttonType, menuType, menuItemType, menuSeparatorType) {
		for(var j = 0; j < itemList.length; j++)
		{
			var itemData = itemList[j];
			var itemComponent;
			var item;
			var overrideAddItem = false;
			if(itemData.type === "item")
			{
				itemComponent = Qt.createQmlObject(getComponentString(menuItemType), parentItem);
				item = itemComponent.createObject(null);
				item.text = itemData.text;
				if(typeof itemData.onClicked != "undefined")
					item.onTriggered.connect(itemData.onClicked);
			}
			else if(itemData.type === "separator")
			{
				itemComponent = Qt.createQmlObject(getComponentString(menuSeparatorType), parentItem);
				item = itemComponent.createObject(null);
			}
			else if(itemData.type === "menu")
			{
				if(buttonType === "")
				{
					itemComponent = Qt.createQmlObject(getComponentString(menuType), parentItem)
					item = itemComponent.createObject(parentItem);
					item.title = itemData.title;
					parentItem.addMenu(item);
					createMenu(item, itemData.itemList, buttonType, menuType, menuItemType, menuSeparatorType);
					overrideAddItem = true;
				}
				else {
					itemComponent = Qt.createQmlObject(getComponentString(buttonType), parentItem)
					item = itemComponent.createObject(null);
					item.text = itemData.title;
					var buttonTextComponent = Qt.createQmlObject(getComponentString("Text"), item);
					var buttonText = buttonTextComponent.createObject(item);
					buttonText.text = Qt.binding(function() { return item.text });
					buttonText.font = Qt.binding(function() { return item.font });
					buttonText.color = Qt.binding(function() { return Material.foreground });
					buttonText.horizontalAlignment = Qt.AlignLeft;
					buttonText.verticalAlignment = Qt.AlignVCenter;
					item.contentItem = buttonText;
					item.implicitWidth = buttonText.implicitWidth;
					item.implicitHeight = 50;
					item.padding = 20;
					var menuComponent = Qt.createQmlObject(getComponentString(menuType), item)
					var menu = menuComponent.createObject(item);
					menu.isSubMenu = true;
					menu.onClosed.connect(function() {
						if(!menu.parent.ignoreClosing)
							parentItem.close();
						menu.parent.ignoreClosing = false;
					})
					createMenu(menu, itemData.itemList, buttonType, menuType, menuItemType, menuSeparatorType);
					item.menu = menu;
				}
			}
			if(!overrideAddItem)
				parentItem.addItem(item);
		}
	}

	control: RowLayout {
		id: mainLayout
		Component.onCompleted: {
			if(QmlUtils.nativeMenuBar())
				root.visible = false;
			createMenuBar(mainLayout, "MenuButton", "CustomMenu", "CustomMenuItem", "MenuSeparator");
		}
	}

	Platform.MenuBar {
		id: platformMenuBar
		Component.onCompleted: { createMenuBar(platformMenuBar, "", "Platform.Menu", "Platform.MenuItem", "Platform.MenuSeparator"); }
	}
}

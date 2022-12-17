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

Panel {
	property list<Item> menuList: [
		Item {
			property string text: qsTr("&File");
			property list<Item> itemList: [
				Item {
					property string type: "item"
					property string text: qsTr("&New...")
				},
				Item {
					property string type: "item"
					property string text: qsTr("&Open...")
				},
				Item {
					property string type: "separator"
				},
				Item {
					property string type: "item"
					property string text: qsTr("&Quit")
					function onClicked() { Qt.quit(); }
				}
			]
		}
	]

	function getComponentString(typeName) {
		var imports = "import QtQuick 2.9; import QtQuick.Controls 2.2;"
		return imports + " Component { " + typeName + " {} }";
	}

	function createMenuBar(parentItem, buttonType, menuType, menuItemType, menuSeparatorType) {
		for(var i = 0; i < menuList.length; i++)
		{
			var buttonComponent = Qt.createQmlObject(getComponentString(buttonType), parentItem);
			var button = buttonComponent.createObject(parentItem);
			button.text = menuList[i].text;
			var menuComponent = Qt.createQmlObject(getComponentString(menuType), parentItem);
			var menu = menuComponent.createObject(parentItem);
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
			if(itemData.type === "item")
			{
				itemComponent = Qt.createQmlObject(getComponentString(menuItemType), parentItem);
				item = itemComponent.createObject(null);
				item.text = itemData.text;
				if(typeof itemData.onClicked != "undefined")
					item.onClicked.connect(itemData.onClicked);
			}
			else if(itemData.type === "separator")
			{
				itemComponent = Qt.createQmlObject(getComponentString(menuSeparatorType), parentItem);
				item = itemComponent.createObject(null);
			}
			else if(itemData.type === "menu")
			{
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
				menu.onClosed.connect(parentItem.close);
				createMenu(menu, itemData.itemList, buttonType, menuType, menuItemType, menuSeparatorType);
				item.menu = menu;
			}
			parentItem.addItem(item);
		}
	}

	control: RowLayout {
		id: mainLayout
		Component.onCompleted: { createMenuBar(mainLayout, "MenuButton", "CustomMenu", "CustomMenuItem", "MenuSeparator"); }
	}

	function backgroundColor() {
		if(Material.theme === Material.Light)
			return Qt.rgba(1, 1, 1, 1);
		else
			return Qt.rgba(0, 0, 0, 1);
	}
}

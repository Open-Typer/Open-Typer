/*
 * IAddon.h
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

#ifndef IADDON_H
#define IADDON_H

#include <QVector>
#include <QtPlugin>

#if defined CORE_SHARED_LIB
#define CORE_LIB_EXPORT Q_DECL_EXPORT
#else
#define CORE_LIB_EXPORT Q_DECL_IMPORT
#endif

/*! \brief The IAddon class provides an interface for addons. */
class CORE_LIB_EXPORT IAddon
{
	public:
		enum Event
		{
			Event_InitSettings = 0,
			Event_InitApp = 1,
			Event_InitMenu = 2,
			Event_InitMenuActions = 3,
			Event_InitButtons = 4,
			Event_InitButtonsFinalize = 5,
			Event_RefreshApp = 6,
			Event_InitExercise = 7,
			Event_InitTopBarWidgets = 8,
			Event_InitTopBarWidgetsFinalize = 9,
			Event_ExerciseFinalInit = 10,
			Event_EndStockExercise = 11,
			Event_EndTypingTest = 12,
			Event_ChangeMode = 13,
			Event_OpenLoadExDialog = 14,
			Event_CustomExLoaded = 15
		};

		virtual ~IAddon(void) = default;
		virtual void addonEvent(Event type, QVariantMap args) = 0;
};

Q_DECLARE_INTERFACE(IAddon, "opentyper.addon")

extern QVector<IAddon *> loadedAddons;

#endif // IADDON_H

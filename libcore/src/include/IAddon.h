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
			Event_InitSettings,
			Event_InitApp,
			Event_InitMenu,
			Event_InitMenuActions,
			Event_InitButtons,
			Event_InitButtonsFinalize,
			Event_RefreshApp,
			Event_InitExercise,
			Event_InitTopBarWidgets,
			Event_InitTopBarWidgetsFinalize,
			Event_ExerciseFinalInit,
			Event_EndStockExercise,
			Event_EndTypingTest,
			Event_ChangeMode,
			Event_OpenLoadExDialog,
			Event_CustomExLoaded,
			Event_LoadExStats
		};

		virtual ~IAddon(void) = default;
		virtual void addonEvent(Event type, QVariantMap args) = 0;
};

Q_DECLARE_INTERFACE(IAddon, "opentyper.addon")

extern QVector<IAddon *> loadedAddons;
extern QStringList loadedAddonsClasses;

#endif // IADDON_H

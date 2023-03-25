/*
 * IAddon.h
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

#ifndef IADDON_H
#define IADDON_H

#include <QVector>
#include <QtPlugin>
#include "AddonApi.h"

/*! \brief The IAddon class provides an interface for addons. */
class Q_DECL_EXPORT IAddon
{
	public:
		virtual ~IAddon(void) = default;
		virtual void addonEvent(AddonApi::Event type, QVariantMap args) = 0;
		virtual QString version() = 0;
};

Q_DECLARE_INTERFACE(IAddon, "opentyper.addon")

#endif // IADDON_H

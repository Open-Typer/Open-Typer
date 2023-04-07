/*
 * ModulesIoC.h
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

#ifndef MODULESIOC_H
#define MODULESIOC_H

#include <QString>
#include <map>
#include <memory>
#include "IModuleExportInterface.h"

class ModulesIoC
{
	public:
		static void init()
		{
			m_initialized = true;
		}

		static ModulesIoC *instance()
		{
			if(!m_initialized)
			{
				if(m_initialized == false)
					Q_ASSERT(false);
				return nullptr;
			}
			static ModulesIoC instance;
			if(instance.m_customInstance)
				return instance.m_customInstance;
			else
				return &instance;
		}

		static void setCustomInstance(ModulesIoC *i)
		{
			Q_ASSERT(i);
			init();
			instance()->m_customInstance = i;
		}

		template <class I>
		void registerExport(std::shared_ptr<I> instance)
		{
			QString id = typeid(I).name();
			auto it = m_map.find(id);
			if(it != m_map.end())
				Q_ASSERT(false);
			auto sharedInstance = std::shared_ptr<I>(instance);
			auto interface = std::dynamic_pointer_cast<IModuleExportInterface>(sharedInstance);
			if(!interface)
			{
				Q_ASSERT(interface);
				return;
			}
			m_map[id] = { interface };
		}

		std::shared_ptr<IModuleExportInterface> doResolvePtrById(const QString &id)
		{
			auto it = m_map.find(id);
			if(it == m_map.end())
			{
				Q_ASSERT(false);
				return nullptr;
			}

			Service &service = it->second;
			if(service.p)
				return service.p;

			return nullptr;
		}

		template <class I>
		std::shared_ptr<I> resolve()
		{
			QString id = typeid(I).name();
			std::shared_ptr<IModuleExportInterface> service = doResolvePtrById(id);
			return std::static_pointer_cast<I>(service);
		}

	private:
		struct Service
		{
				std::shared_ptr<IModuleExportInterface> p;
		};

		std::map<QString, Service> m_map;
		ModulesIoC *m_customInstance = nullptr;
		inline static bool m_initialized = false;
};

#endif // MODULESIOC_H

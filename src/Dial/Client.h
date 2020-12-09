/**
 * Client.h
 *
 * Copyright 2020 slaff <slaff@attachix.com>
 *
 * This file is part of the Sming DIAL Library
 *
 * This library is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, version 3 or later.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with FlashString.
 * If not, see <https://www.gnu.org/licenses/>.
 *
 ****/

#pragma once

#include <Network/UPnP/ControlPoint.h>
#include <Network/UPnP/dial-multiscreen-org/device/dial1.h>
#include "App.h"

/** @defgroup   DIAL client
 *  @brief      Provides DIAL client
 *  @ingroup    multimedia SSDP
 *  @{
 */

namespace Dial
{
class Client : public UPnP::dial_multiscreen_org::device::dial1
{
public:
	using dial1::dial1;

	using Discovered = Delegate<bool(Client&)>;

	static const UPnP::ObjectClass class_;

	const UPnP::ObjectClass& getClass() const override
	{
		return class_;
	}

	static UPnP::Object* createObject(UPnP::DeviceControl* owner)
	{
		return new Client(owner);
	}

	/**
	 * @brief Get application object by name
	 * @param applicationId the unique application.
	 * 				A list of registered ids can be found here: http://www.dial-multiscreen.org/dial-registry/namespace-database#TOC-Registered-Names
	 * @retval App& Application object reference
	 */
	App& getApp(const String& applicationId);

	bool sendRequest(HttpRequest* request)
	{
		return controlPoint().sendRequest(request);
	}

	void onConnected(HttpConnection& connection) override;

	Url getApplicationUrl() const
	{
		return applicationUrl;
	}

private:
	using AppMap = ObjectMap<String, App>;

	Url applicationUrl;
	AppMap apps; // <<< list of invoked apps
};

bool discover(UPnP::ControlPoint& controlPoint, Client::Discovered callback);

} // namespace Dial

/** @} */

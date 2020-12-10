/**
 * App.h
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

#include <WString.h>
#include <Delegate.h>
#include <Network/Http/HttpRequest.h>
#include <Network/Http/HttpResponse.h>

/** @defgroup   DIAL application
 *  @brief      Provides DIAL application control
 *  @ingroup    DIAL client
 *  @{
 */

namespace Dial
{
class Client;

class App
{
public:
	using ResponseCallback = Delegate<void(App& app, HttpResponse& response)>;

	App(Client& client, const String& name) : client(client), name(name)
	{
	}

	Client& getClient() const
	{
		return client;
	}

	String getName() const
	{
		return name;
	}

	bool status(ResponseCallback onResponse);

	/**
	 *
	 */
	bool run(ResponseCallback onResponse = nullptr)
	{
		return sendRunRequest(createApplicationRequest(), onResponse);
	}

	bool run(const String& body, MimeType mime, ResponseCallback onResponse = nullptr);

	bool run(const HttpParams& params, ResponseCallback onResponse = nullptr);

	bool stop(ResponseCallback onResponse = nullptr);

private:
	bool sendRunRequest(HttpRequest* request, ResponseCallback onResponse);
	HttpRequest* createApplicationRequest();

	Client& client;
	String name;
	String instanceUrl;
};

} // namespace Dial

/** @} */

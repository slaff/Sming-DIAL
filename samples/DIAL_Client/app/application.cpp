#include <SmingCore.h>
#include <Dial/Client.h>

// If you want, you can define WiFi settings globally in Eclipse Environment Variables
#ifndef WIFI_SSID
#define WIFI_SSID "PleaseEnterSSID" // Put you SSID and Password here
#define WIFI_PWD "PleaseEnterPass"
#endif

static UPnP::ControlPoint controlPoint;

void onRun(Dial::App& app, HttpResponse& response)
{
	if(response.isSuccess()) {
		auto timer = new AutoDeleteTimer;
		timer->initializeMs<20000>([&]() {
			// Once started the app can also be stopped using the command below
			Serial.print(_F("Stopping application: "));
			Serial.println(app.getName());
			app.stop();
		});
		timer->startOnce();
	}
}

void onStatus(Dial::App& app, HttpResponse& response)
{
	if(!response.isSuccess()) {
		Serial.print(_F("Error locating application '"));
		Serial.print(app.getName());
		Serial.print(_F("' on device '"));
		Serial.print(app.getClient().friendlyName());
		Serial.print("': ");
		Serial.println(toString(response.code));
		return;
	}

	HttpParams params;
	params["v"] = "fC9HdQUaFtA";
	app.run(params, onRun);
}

void discoverDialDevices()
{
	Dial::discover(controlPoint, [](Dial::Client& client) {
		Serial.print(_F("New DIAL device found, friendly name '"));
		Serial.print(_F("Friendly name '"));
		Serial.print(client.friendlyName());
		Serial.print(_F("', url "));
		Serial.println(client.getApplicationUrl().toString());

		auto& app = client.getApp("YouTube");
		app.status(onStatus);

		// Keep this device
		return true;
	});
}

void connectOk(IpAddress ip, IpAddress mask, IpAddress gateway)
{
	Serial.print(_F("I'm CONNECTED to "));
	Serial.println(ip);

	discoverDialDevices();
}

void connectFail(const String& ssid, MacAddress bssid, WifiDisconnectReason reason)
{
	// The different reason codes can be found in user_interface.h. in your SDK.
	Serial.print(_F("Disconnected from \""));
	Serial.print(ssid);
	Serial.print(_F("\", reason: "));
	Serial.println(WifiEvents.getDisconnectReasonDesc(reason));
}

void init()
{
	Serial.begin(SERIAL_BAUD_RATE);
	Serial.systemDebugOutput(true); // Allow debug print to serial

	// Station - WiFi client
	WifiStation.enable(true);
	WifiStation.config(F(WIFI_SSID), F(WIFI_PWD));

	// Set callback that should be triggered when we have assigned IP
	WifiEvents.onStationGotIP(connectOk);

	// Set callback that should be triggered if we are disconnected or connection attempt failed
	WifiEvents.onStationDisconnect(connectFail);
}

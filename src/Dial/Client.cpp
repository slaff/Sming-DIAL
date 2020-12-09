#include "Client.h"
#include <FlashString/Vector.hpp>

namespace Dial
{
using dial1 = UPnP::dial_multiscreen_org::device::dial1;

const UPnP::ObjectClass Client::class_ PROGMEM = {
	.kind_ = Urn::Kind::device,
	.version_ = 1,
	.domain_ = dial1::class_.domain_,
	.type_ = dial1::class_.type_,
	.createObject_ = createObject,
	{.device_ = dial1::class_.device_},
};

DEFINE_FSTR_VECTOR_LOCAL(classes, UPnP::ObjectClass, &Client::class_)

bool discover(UPnP::ControlPoint& controlPoint, Client::Discovered callback)
{
	UPnP::ControlPoint::registerClasses(*Client::class_.domain_, classes);
	return controlPoint.beginSearch(callback);
}

void Client::onConnected(HttpConnection& connection)
{
	String url = connection.getResponse()->headers[_F("Application-URL")];
	// Make sure url has trailing /
	if(!url.endsWith('/')) {
		url += '/';
	}
	applicationUrl = url;
}

App& Client::getApp(const String& applicationId)
{
	AppMap::Value app = apps[applicationId];
	if(!app) {
		app = new App(*this, applicationId);
	}

	return *app;
}

} // namespace Dial

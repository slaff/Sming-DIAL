#include "Client.h"
#include <FlashString/Vector.hpp>

namespace Dial
{
DEFINE_FSTR_LOCAL(domain, "dial-multiscreen-org")
DEFINE_FSTR_LOCAL(type, "dial")

constexpr UPnP::ObjectClass Client::class_ PROGMEM = {
	.kind_ = Urn::Kind::device,
	.version_ = 1,
	.domain_ = &domain,
	.type_ = &type,
	.createObject_ = createObject,
};

DEFINE_FSTR_VECTOR_LOCAL(classes, UPnP::ObjectClass, &Client::class_)

bool discover(UPnP::ControlPoint& controlPoint, Client::Discovered callback)
{
	UPnP::ControlPoint::registerClasses(domain, classes);
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

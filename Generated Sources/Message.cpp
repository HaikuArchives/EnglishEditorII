/* Message.cpp */

#include "Message.h"



Message::Message(unsigned long what)
	: ownsMessage(true)
{
	message = new BMessage(what);
}


Message::~Message()
{
	if (ownsMessage)
		delete message;
}


unsigned long Message::What()
{
	return message->what;
}


Message::Message(BMessage* nativeMessage)
	: message(nativeMessage), ownsMessage(false)
{
}


BMessage* Message::NativeMessage()
{
	return message;
}



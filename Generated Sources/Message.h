/* Message.h */

#ifndef _H_Message_
#define _H_Message_

#include <Message.h>

class Message {
public:
	Message(unsigned long what);
	~Message();
	unsigned long	What();
	Message(BMessage* nativeMessage);
	BMessage*    	NativeMessage();

protected:
	BMessage*	message;
	bool     	ownsMessage;
};


#endif

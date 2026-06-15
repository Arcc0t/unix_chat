#include "serialize.h"
#include <stdio.h>
#include <string.h>
int serialize(struct message *msg, char *buf, int bufsize)
{
	if(msg->type == SERVER_CLIENT)
		return snprintf(buf, bufsize, 
			"ID: %d\nReply-ID: %d\nUsername: %s\nBody-Length: %d\nBody:\n%s", 
			msg->id, msg->reply_id, msg->username, msg->body_length, msg->body);
	else
		return snprintf(buf, bufsize, 
				"Reply-ID: %d\nBody-Length: %d\nBody:\n%s", 
				msg->reply_id, msg->body_length, msg->body);
}

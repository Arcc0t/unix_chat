#include <stdio.h>
#include <string.h>
#include "defines.h"
#include "message.h"
void serialise(struct message m, char *s){
    if(m.type == SERVER_CLIENT)
        sprintf(s,"ID: %d\nReply-ID: %d\nUsername: %s\nBody: %s", m.id, m.reply_id, m.username, m.body);
    else
        sprintf(s,"Reply-ID: %d\nBody: %s", m.reply_id, m.body);
}
int main(){
	
}
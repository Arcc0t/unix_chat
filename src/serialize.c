void serialize(struct message m, char *s){
    if(m.type == SERVER_CLIENT)
        sprintf(s,"ID: %d\nReply-ID: %d\nUsername: %s\nBody: %s", m.id, m.reply_id, m.username, m.body);
    else
        sprintf(s,"Reply-ID: %d\nBody: %s", m.reply_id, m.body);
z
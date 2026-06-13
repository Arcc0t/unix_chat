void serialize(struct message m, char *s){
    if(m.type == SERVER_CLIENT)
        snprintf(s,sizeof(s),"ID: %d\nReply-ID: %d\nUsername: %s\nBody: %s", m.id, m.reply_id, m.username, m.body);
    else
        snprintf(s,sizeof(s),"Reply-ID: %d\nBody: %s", m.reply_id, m.body);
}
BOOL serialize(struct message *m, char *s){
	int n;
	if(m.type == SERVER_CLIENT)
		n = snprintf(s,sizeof(s),"ID: %d\nReply-ID: %d\nUsername: %s\nBody:\n%s", m->id, m->reply_id, m->username, m->body);
	else
		n = snprintf(s,sizeof(s),"Reply-ID: %d\nBody:\n%s", m->reply_id, m->body);
	return n < sizeof(s) || s[n-1]=='\0';
}
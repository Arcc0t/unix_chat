BOOL serialize(struct message *m, char *s, int N){
	int n;
	if(m.type == SERVER_CLIENT)
		n = snprintf(s, N, "ID: %d\nReply-ID: %d\nUsername: %s\nBody:\n%s", m->id, m->reply_id, m->username, m->body);
	else
		n = snprintf(s, N, "Reply-ID: %d\nBody:\n%s", m->reply_id, m->body);
	return n < N || s[n-1] == '\0';
}

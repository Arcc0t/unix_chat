int serialize(struct message *m, char *s, int N){
	if(m.type == SERVER_CLIENT)
		return snprintf(s, N, "ID: %d\nReply-ID: %d\nUsername: %s\nBody:\n%s", m->id, m->reply_id, m->username, m->body);
	else
		return snprintf(s, N, "Reply-ID: %d\nBody:\n%s", m->reply_id, m->body);
}

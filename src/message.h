#ifndef MESSAGE_H_GUARD
#define MESSAGE_H_GUARD

enum {USERNAME_MAXLEN = 32, BODY_MAXLEN = 4096};

enum field_type {
	FIELD_ID = 0,
	FIELD_REPLY_ID,
	FIELD_USERNAME,
	FIELD_BODY,
	FIELD_COUNT /* number of fields overall */
};

enum message_type {CLIENT_SERVER, SERVER_CLIENT};

struct message {
	enum message_type type;
	int id;
	int reply_id;
	char username[USERNAME_MAXLEN];
	int body_len;
	char body[BODY_MAXLEN];
};

enum {MESSAGE_LEN = sizeof(struct message)}; /* might be more than 
						needed to store 
						a message due to
						alignment */


#endif /*MESSAGE_H_GUARD*/

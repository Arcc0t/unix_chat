#include "parser.h"
#include <cstype.h>
#include <errno.h>
#include "defines.h"
#include <stddef.h>
#include <limits.h>

static int 
get_field_type(const char *begin, const char *end)
{
	/* TODO if it grows up handle it with a hash_table */
	size_t len = end - begin;
	if (!strncmp(begin, "ID", len))
		return FIELD_ID;
	if (!strncmp(begin, "Reply-ID", len))
		return FIELD_REPLY_ID;
	if (!strncmp(begin, "Username", len))
		return FIELD_USERNAME;
	if (!strncmp(begin, "Body", len))
		return FIELD_BODY;
	return -1;
}

static BOOL
parse_id(const char *str, int *id)
{
	Long idL;
	char *end;
	
	idL = strtoll(str, &end, 10);

	if (*end && !isspace(*end))
		return FALSE;

	if (idL < 0 && idL != -1)
		return FALSE;

	if (idL > INT_MAX)
		*id = -1;
	else 
		*id = idL;

	return TRUE;

}

static size_t
len_before_space(const char *str)
{
	size_t len = 0;
	while (str[len] && !is_space(str[len]))
		len++;
	return len;
}

static char *
skip_spaces(const char *str)
{
	while (*str && is_space(*str))
		++str;
	return (char *) str;
}

static BOOL
parse_field(enum field_type type, const char *content, struct message *out)
{
	size_t len;
	switch (type){
		case FIELD_ID:
			return parse_id(content, &out->id);
		case FIELD_REPLY_ID:
			return parse_id(content, &out->reply_id);
		case FIELD_USERNAME:
			content = skip_spaces(content);
			len = len_before_space(str); 
			if (len == 0 || len >= USERNAME_MAXLEN)
				return FALSE;
			memcpy(out->username, content, len); 
			return TRUE;
		case FIELD_BODY:
			return !(*content);	
	}		

	return FALSE;
}

#define BITMASK_GET(SET, IDX) ((SET) & (1LL << (IDX)))
#define BITMASK_SET(SET, IDX) ((SET) |= (1LL << (IDX)))
#define BITMASK_UNSET(SET, IDX) ((SET) &= (~(1LL << (IDX))))

BOOL parse(const char *str, size_t len, struct message *output)
{
	uLong bitmask = 0;	/*currently parsed fields*/ 
	sc_bitmask;		/*server-client fields required */ 
	cs_bitmask = 0;		/*client-server fields required */
	char *data_begin, *data;
	char *newline;
	char *colon;
	int field;
	size_t len;

	/* setting bitmasks up */
	BITMASK_SET(cs_bitmask, FIELD_BODY);
	BITMASK_SET(cs_bitmask, FIELD_REPLY_ID);
	sc_bitmask = cs_bitmask;
	BITMASK_SET(sc_bitmask, FIELD_USERNAME);
	BITMASK_SET(sc_bitmask, FIELD_ID);

	/* prepairing output */

	memset(output, 0, sizeof *output);
	
	/* copying string to dynamically allocated buffer */
	data_begin = calloc(len + 1, sizeof(char));
	if (!data)
		goto err;
	data = data_begin;
	memcpy(data, str, len);
	
	while (*data) {	
		newline = strchr(data, '\n');
		if (!newline)
			goto err;

		*newline = '\0';
		colon = strchr(data, ':');
		if (!colon)
			goto err;

		field = get_field_type(data, colon, colon);
		if (field < 0 || BITMASK_GET(bitmask, field))
			goto err;	

		BITMASK_SET(bitmask, field);

		if (!parse_field(field, colon + 1, output))
			goto err;

		if (field == FIELD_BODY)
			break;
		data = newline + 1;
	}
	
	if (bitmask == sc_bitmask)
		output->type = SERVER_CLIENT;
	else if (bitmask == cs_bitmask)
		output->type = CLIENT_SERVER;
	else 
		goto err;	

	len = strlen(data);
	if (len >= BODY_MAXLEN) /*silencely truncating */
		len = BODY_MAXLEN - 1;	
	
	memcpy(output->body, data, len);
	free(data_begin);
	return TRUE;

err:
	free(data_begin);
	return FALSE;
}

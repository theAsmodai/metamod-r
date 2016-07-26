#include "precompiled.h"

bool is_yes(const char* str)
{
	return !strcmp(str, "true") || !strcmp(str, "yes") || !strcmp(str, "1");
}

bool is_no(const char* str)
{
	return !strcmp(str, "false") || !strcmp(str, "no") || !strcmp(str, "0");
}

char* ENTITY_KEYVALUE(edict_t* entity, char* key)
{
	return (INFOKEY_VALUE(GET_INFOKEYBUFFER(entity), key));
}

const char* LOCALINFO(char* key)
{
	return (ENTITY_KEYVALUE(NULL, key));
}

#pragma once

// Comments from SDK dlls/util.h:
//! This is the glue that hooks .MAP entity class names to our CPP classes.
//! The _declspec forces them to be exported by name so we can do a lookup with GetProcAddress().
//! The function is used to intialize / allocate the object for the entity.

// Adapted from LINK_ENTITY_TO_FUNC in adminmod linkfunc.cpp.
typedef void (*ENTITY_FN)(entvars_t *);

// Function to perform common code of LINK_ENTITY_TO_GAME.
void do_link_ent(ENTITY_FN* pfnEntity, int* missing, const char* entStr, entvars_t* pev);

#define LINK_ENTITY_TO_GAME(entityName) \
	C_DLLEXPORT void entityName(entvars_t *pev) { \
		static ENTITY_FN pfnEntity = nullptr; \
		static int missing = 0; \
		do_link_ent(&pfnEntity, &missing, STRINGIZE(entityName, 0), pev); \
	}

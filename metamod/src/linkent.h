#pragma once

// Comments from SDK dlls/util.h:
//! This is the glue that hooks .MAP entity class names to our CPP classes.
//! The _declspec forces them to be exported by name so we can do a lookup with GetProcAddress().
//! The function is used to intialize / allocate the object for the entity.

// Adapted from LINK_ENTITY_TO_FUNC in adminmod linkfunc.cpp.
typedef void (*ENTITY_FN)(entvars_t *);

// Function to perform common code of LINK_ENTITY_TO_GAME.
void do_link_ent(ENTITY_FN *pfnEntity, int *missing, const char *entStr, entvars_t *pev);

#define LINK_ENTITY_TO_GAME(entityName) \
	C_DLLEXPORT void entityName(entvars_t *pev); \
	void entityName(entvars_t *pev) { \
		static ENTITY_FN pfnEntity = NULL; \
		static int missing = 0; \
		do_link_ent(&pfnEntity, &missing, STRINGIZE(entityName, 0), pev); \
	}

// For now, we have to explicitly export functions for plugin entities,
// just as for gamedll entities.  Ideally, this could be generalized in
// some manner, so that plugins can declare and use their own entities
// without having them explicitly supported by metamod, but I don't know
// yet if that is actually possible.
//
// LINK_ENTITY_TO_PLUGIN
//  - if plugin not loaded & running, return
//  - plugin has to be set loadable=startup only, else log error, return
//  - (plugin loaded) if func missing, return
//  - (plugin loaded) if func not found, dlsym
//  - (plugin loaded) if func still not found, set missing, return
//  - (plugin loaded, func found) call func
#define LINK_ENTITY_TO_PLUGIN(entityName, pluginName) \
	C_DLLEXPORT void entityName(entvars_t *pev); \
	void entityName(entvars_t *pev) { \
		static ENTITY_FN pfnEntity = NULL; \
		static int missing = 0; \
		const char *entStr; \
		MPlugin *findp; \
		entStr = STRINGIZE(entityName, 0); \
		if (missing) \
			return; \
		if (!pfnEntity) { \
			if (!(findp = g_plugins->find_match(pluginName))) { \
				META_ERROR("Couldn't find loaded plugin '%s' for plugin entity '%s'", pluginName, entStr); \
				missing = 1; \
				return; \
			} \
			if (findp->info && findp->info->loadable != PT_STARTUP) { \
				META_ERROR("Can't link entity '%s' for plugin '%s'; loadable != startup: %s", entStr, pluginName, findp->str_loadable()); \
				missing = 1; \
				return; \
			} \
			META_DEBUG(9, ("Looking up plugin entity '%s'", entStr)); \
			pfnEntity = (ENTITY_FN) DLSYM(findp->handle, entStr); \
		} \
		if (!pfnEntity) { \
			META_ERROR("Couldn't find plugin entity '%s' in plugin DLL '%s'", entStr, findp->file); \
			missing = 1; \
			return; \
		} \
		META_DEBUG(8, ("Linking plugin entity '%s'", entStr)); \
		(*pfnEntity)(pev); \
	}

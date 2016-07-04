#ifndef REG_SUPPORT_H
#define REG_SUPPORT_H

#include "mreg.h"			// REG_CMD_FN, etc

// these are only 'hidden' because called from outside (plugins and engine)
void DLLHIDDEN meta_command_handler(void);
void DLLHIDDEN meta_AddServerCommand(const char *cmd_name, REG_CMD_FN function);
void DLLHIDDEN meta_CVarRegister(cvar_t *pCvar);
int DLLHIDDEN meta_RegUserMsg(const char *pszName, int iSize);
void DLLHIDDEN meta_QueryClientCvarValue(const edict_t *player, const char *cvarName);

#endif /* REG_SUPPORT_H */

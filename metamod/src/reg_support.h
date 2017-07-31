#pragma once

void meta_command_handler();
void meta_AddServerCommand(const char* cmd_name, REG_CMD_FN function);
void meta_CVarRegister(cvar_t* pCvar);
int meta_RegUserMsg(const char* pszName, int iSize);
void meta_QueryClientCvarValue(const edict_t* player, const char* cvarName);

// vi: set ts=4 sw=4 :
// vim: set tw=75 :

// engine_api.h - prototypes and typedefs for Half-Life engine functions

/*
 * Copyright (c) 2001-2006 Will Day <willday@hpgx.net>
 *
 *    This file is part of Metamod.
 *
 *    Metamod is free software; you can redistribute it and/or modify it
 *    under the terms of the GNU General Public License as published by the
 *    Free Software Foundation; either version 2 of the License, or (at
 *    your option) any later version.
 *
 *    Metamod is distributed in the hope that it will be useful, but
 *    WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with Metamod; if not, write to the Free Software Foundation,
 *    Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *    In addition, as a special exception, the author gives permission to
 *    link the code of this program with the Half-Life Game Engine ("HL
 *    Engine") and Modified Game Libraries ("MODs") developed by Valve,
 *    L.L.C ("Valve").  You must obey the GNU General Public License in all
 *    respects for all of the code used other than the HL Engine and MODs
 *    from Valve.  If you modify this file, you may extend this exception
 *    to your version of the file, but you are not obligated to do so.  If
 *    you do not wish to do so, delete this exception statement from your
 *    version.
 *
 */

#ifndef ENGINE_API_H
#define ENGINE_API_H

#include "comp_dep.h"

// Plugin's GetEngineFunctions, called by metamod.
typedef int (*GET_ENGINE_FUNCTIONS_FN) (enginefuncs_t *pengfuncsFromEngine, int *interfaceVersion);

// According to SDK engine/eiface.h:
//! enginefuncs_t
//! ONLY ADD NEW FUNCTIONS TO THE END OF THIS STRUCT.  INTERFACE VERSION IS FROZEN AT 138
#define ENGINE_INTERFACE_VERSION 138

// Protect against other projects which use this include file but use the
// normal enginefuncs_t type for their meta_engfuncs.
#ifdef __METAMOD_BUILD__
#  include "meta_eiface.h"    // meta_enginefuncs_t
extern meta_enginefuncs_t meta_engfuncs DLLHIDDEN;
#else
extern enginefuncs_t meta_engfuncs DLLHIDDEN;
#endif

// Typedefs for the above functions:

typedef int (*FN_PRECACHEMODEL) (char* s);
typedef int (*FN_PRECACHESOUND) (char* s);
typedef void (*FN_SETMODEL) (edict_t *e, const char *m);
typedef int (*FN_MODELINDEX) (const char *m);
typedef int (*FN_MODELFRAMES) (int modelIndex);
typedef void (*FN_SETSIZE) (edict_t *e, const float *rgflMin, const float *rgflMax);
typedef void (*FN_CHANGELEVEL) (char *s1, char *s2);
typedef void (*FN_GETSPAWNPARMS) (edict_t *ent);
typedef void (*FN_SAVESPAWNPARMS) (edict_t *ent);
typedef float (*FN_VECTOYAW) (const float *rgflVector);
typedef void (*FN_VECTOANGLES) (const float *rgflVectorIn, float *rgflVectorOut);
typedef void (*FN_MOVETOORIGIN) (edict_t *ent, const float *pflGoal, float dist, int iMoveType);
typedef void (*FN_CHANGEYAW) (edict_t *ent);
typedef void (*FN_CHANGEPITCH) (edict_t *ent);
typedef edict_t * (*FN_FINDENTITYBYSTRING) (edict_t *pEdictStartSearchAfter, const char *pszField, const char *pszValue);
typedef int (*FN_GETENTITYILLUM) (edict_t *pEnt);
typedef edict_t * (*FN_FINDENTITYINSPHERE) (edict_t *pEdictStartSearchAfter, const float *org, float rad);
typedef edict_t * (*FN_FINDCLIENTINPVS) (edict_t *pEdict);
typedef edict_t * (*FN_ENTITIESINPVS) (edict_t *pplayer);
typedef void (*FN_MAKEVECTORS) (const float *rgflVector);
typedef void (*FN_ANGLEVECTORS) (const float *rgflVector, float *forward, float *right, float *up);
typedef edict_t * (*FN_CREATEENTITY) (void);
typedef void (*FN_REMOVEENTITY) (edict_t *e);
typedef edict_t * (*FN_CREATENAMEDENTITY) (int className);
typedef void (*FN_MAKESTATIC) (edict_t *ent);
typedef int (*FN_ENTISONFLOOR) (edict_t *e);
typedef int (*FN_DROPTOFLOOR) (edict_t *e);
typedef int (*FN_WALKMOVE) (edict_t *ent, float yaw, float dist, int iMode);
typedef void (*FN_SETORIGIN) (edict_t *e, const float *rgflOrigin);
typedef void (*FN_EMITSOUND) (edict_t *entity, int channel, const char *sample, /*int*/float volume, float attenuation, int fFlags, int pitch);
typedef void (*FN_EMITAMBIENTSOUND) (edict_t *entity, float *pos, const char *samp, float vol, float attenuation, int fFlags, int pitch);
typedef void (*FN_TRACELINE) (const float *v1, const float *v2, int fNoMonsters, edict_t *pentToSkip, TraceResult *ptr);
typedef void (*FN_TRACETOSS) (edict_t *pent, edict_t *pentToIgnore, TraceResult *ptr);
typedef int (*FN_TRACEMONSTERHULL) (edict_t *pEdict, const float *v1, const float *v2, int fNoMonsters, edict_t *pentToSkip, TraceResult *ptr);
typedef void (*FN_TRACEHULL) (const float *v1, const float *v2, int fNoMonsters, int hullNumber, edict_t *pentToSkip, TraceResult *ptr);
typedef void (*FN_TRACEMODEL) (const float *v1, const float *v2, int hullNumber, edict_t *pent, TraceResult *ptr);
typedef const char * (*FN_TRACETEXTURE) (edict_t *pTextureEntity, const float *v1, const float *v2 );
typedef void (*FN_TRACESPHERE) (const float *v1, const float *v2, int fNoMonsters, float radius, edict_t *pentToSkip, TraceResult *ptr);
typedef void (*FN_GETAIMVECTOR) (edict_t *ent, float speed, float *rgflReturn);
typedef void (*FN_SERVERCOMMAND) (char *str);
typedef void (*FN_SERVEREXECUTE) (void);
typedef void (*FN_CLIENTCOMMAND_ENG) (edict_t *pEdict, char *szFmt, ...);
typedef void (*FN_PARTICLEEFFECT) (const float *org, const float *dir, float color, float count);
typedef void (*FN_LIGHTSTYLE) (int style, char *val);
typedef int (*FN_DECALINDEX) (const char *name);
typedef int (*FN_POINTCONTENTS) (const float *rgflVector);
typedef void (*FN_MESSAGEBEGIN) (int msg_dest, int msg_type, const float *pOrigin, edict_t *ed);
typedef void (*FN_MESSAGEEND) (void);
typedef void (*FN_WRITEBYTE) (int iValue);
typedef void (*FN_WRITECHAR) (int iValue);
typedef void (*FN_WRITESHORT) (int iValue);
typedef void (*FN_WRITELONG) (int iValue);
typedef void (*FN_WRITEANGLE) (float flValue);
typedef void (*FN_WRITECOORD) (float flValue);
typedef void (*FN_WRITESTRING) (const char *sz);
typedef void (*FN_WRITEENTITY) (int iValue);
typedef void (*FN_CVARREGISTER) (cvar_t *pCvar);
typedef float (*FN_CVARGETFLOAT) (const char *szVarName);
typedef const char * (*FN_CVARGETSTRING) (const char *szVarName);
typedef void (*FN_CVARSETFLOAT) (const char *szVarName, float flValue);
typedef void (*FN_CVARSETSTRING) (const char *szVarName, const char *szValue);
typedef void (*FN_ALERTMESSAGE) (ALERT_TYPE atype, char *szFmt, ...);
#ifdef HLSDK_3_2_OLD_EIFACE
typedef void (*FN_ENGINEFPRINTF) (FILE *pfile, char *szFmt, ...);
typedef void * (*FN_PVALLOCENTPRIVATEDATA) (edict_t *pEdict, long cb);
#else
typedef void (*FN_ENGINEFPRINTF) (void *pfile, char *szFmt, ...);
typedef void * (*FN_PVALLOCENTPRIVATEDATA) (edict_t *pEdict, int32 cb);
#endif
typedef void * (*FN_PVENTPRIVATEDATA) (edict_t *pEdict);
typedef void (*FN_FREEENTPRIVATEDATA) (edict_t *pEdict);
typedef const char * (*FN_SZFROMINDEX) (int iString);
typedef int (*FN_ALLOCSTRING) (const char *szValue);
typedef struct entvars_s * (*FN_GETVARSOFENT) (edict_t *pEdict);
typedef edict_t * (*FN_PENTITYOFENTOFFSET) (int iEntOffset);
typedef int (*FN_ENTOFFSETOFPENTITY) (const edict_t *pEdict);
typedef int (*FN_INDEXOFEDICT) (const edict_t *pEdict);
typedef edict_t * (*FN_PENTITYOFENTINDEX) (int iEntIndex);
typedef edict_t * (*FN_FINDENTITYBYVARS) (struct entvars_s *pvars);
typedef void * (*FN_GETMODELPTR) (edict_t *pEdict);
typedef int (*FN_REGUSERMSG) (const char *pszName, int iSize);
typedef void (*FN_ANIMATIONAUTOMOVE) (const edict_t *pEdict, float flTime);
typedef void (*FN_GETBONEPOSITION) (const edict_t *pEdict, int iBone, float *rgflOrigin, float *rgflAngles );
#ifdef HLSDK_3_2_OLD_EIFACE
typedef unsigned long (*FN_FUNCTIONFROMNAME) ( const char *pName );
typedef const char * (*FN_NAMEFORFUNCTION) ( unsigned long function );
#else
typedef uint32 (*FN_FUNCTIONFROMNAME) ( const char *pName );
typedef const char * (*FN_NAMEFORFUNCTION) ( uint32 function );
#endif
typedef void (*FN_CLIENTPRINTF) ( edict_t *pEdict, PRINT_TYPE ptype, const char *szMsg );
typedef void (*FN_SERVERPRINT) ( const char *szMsg );
typedef const char * (*FN_CMD_ARGS) ( void );
typedef const char * (*FN_CMD_ARGV) ( int argc );
typedef int (*FN_CMD_ARGC) ( void );
typedef void (*FN_GETATTACHMENT) (const edict_t *pEdict, int iAttachment, float *rgflOrigin, float *rgflAngles );
typedef void (*FN_CRC32_INIT) (CRC32_t *pulCRC);
typedef void (*FN_CRC32_PROCESSBUFFER) (CRC32_t *pulCRC, void *p, int len);
typedef void (*FN_CRC32_PROCESSBYTE) (CRC32_t *pulCRC, unsigned char ch);
typedef CRC32_t (*FN_CRC32_FINAL) (CRC32_t pulCRC);
#ifdef HLSDK_3_2_OLD_EIFACE
typedef long (*FN_RANDOMLONG) (long lLow, long lHigh);
#else
typedef int32 (*FN_RANDOMLONG) (int32 lLow, int32 lHigh);
#endif
typedef float (*FN_RANDOMFLOAT) (float flLow, float flHigh);
typedef void (*FN_SETVIEW) (const edict_t *pClient, const edict_t *pViewent );
typedef float (*FN_TIME) ( void );
typedef void (*FN_CROSSHAIRANGLE) (const edict_t *pClient, float pitch, float yaw);
typedef byte * (*FN_LOADFILEFORME) (char *filename, int *pLength);
typedef void (*FN_FREEFILE) (void *buffer);
typedef void (*FN_ENDSECTION) (const char *pszSectionName);
typedef int (*FN_COMPAREFILETIME) (char *filename1, char *filename2, int *iCompare);
typedef void (*FN_GETGAMEDIR) (char *szGetGameDir);
typedef void (*FN_CVAR_REGISTERVARIABLE) (cvar_t *variable);
typedef void (*FN_FADECLIENTVOLUME) (const edict_t *pEdict, int fadePercent, int fadeOutSeconds, int holdTime, int fadeInSeconds);
typedef void (*FN_SETCLIENTMAXSPEED) (const edict_t *pEdict, float fNewMaxspeed);
typedef edict_t * (*FN_CREATEFAKECLIENT) (const char *netname);
typedef void (*FN_RUNPLAYERMOVE) (edict_t *fakeclient, const float *viewangles, float forwardmove, float sidemove, float upmove, unsigned short buttons, byte impulse, byte msec );
typedef int (*FN_NUMBEROFENTITIES) (void);
typedef char * (*FN_GETINFOKEYBUFFER) (edict_t *e);
typedef char * (*FN_INFOKEYVALUE) (char *infobuffer, char *key);
typedef void (*FN_SETKEYVALUE) (char *infobuffer, char *key, char *value);
typedef void (*FN_SETCLIENTKEYVALUE) (int clientIndex, char *infobuffer, char *key, char *value);
typedef int (*FN_ISMAPVALID) (char *filename);
typedef void (*FN_STATICDECAL) ( const float *origin, int decalIndex, int entityIndex, int modelIndex );
typedef int (*FN_PRECACHEGENERIC) (char *s);
typedef int (*FN_GETPLAYERUSERID) (edict_t *e );
typedef void (*FN_BUILDSOUNDMSG) (edict_t *entity, int channel, const char *sample, /*int*/float volume, float attenuation, int fFlags, int pitch, int msg_dest, int msg_type, const float *pOrigin, edict_t *ed);
typedef int (*FN_ISDEDICATEDSERVER) (void);
typedef cvar_t * (*FN_CVARGETPOINTER) (const char *szVarName);
typedef unsigned int (*FN_GETPLAYERWONID) (edict_t *e);
typedef void (*FN_INFO_REMOVEKEY) ( char *s, const char *key );
typedef const char * (*FN_GETPHYSICSKEYVALUE) ( const edict_t *pClient, const char *key );
typedef void (*FN_SETPHYSICSKEYVALUE) ( const edict_t *pClient, const char *key, const char *value );
typedef const char * (*FN_GETPHYSICSINFOSTRING) ( const edict_t *pClient );
typedef unsigned short (*FN_PRECACHEEVENT) ( int type, const char *psz );
typedef void (*FN_PLAYBACKEVENT) ( int flags, const edict_t *pInvoker, unsigned short eventindex, float delay, float *origin, float *angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2 );
typedef unsigned char * (*FN_SETFATPVS) ( float *org );
typedef unsigned char * (*FN_SETFATPAS) ( float *org );
typedef int (*FN_CHECKVISIBILITY) ( const edict_t *entity, unsigned char *pset );
typedef void (*FN_DELTASETFIELD) ( struct delta_s *pFields, const char *fieldname );
typedef void (*FN_DELTAUNSETFIELD) ( struct delta_s *pFields, const char *fieldname );
typedef void (*FN_DELTAADDENCODER) ( char *name, void (*conditionalencode)( struct delta_s *pFields, const unsigned char *from, const unsigned char *to ) );
typedef int (*FN_GETCURRENTPLAYER) ( void );
typedef int (*FN_CANSKIPPLAYER) ( const edict_t *player );
typedef int (*FN_DELTAFINDFIELD) ( struct delta_s *pFields, const char *fieldname );
typedef void (*FN_DELTASETFIELDBYINDEX) ( struct delta_s *pFields, int fieldNumber );
typedef void (*FN_DELTAUNSETFIELDBYINDEX) ( struct delta_s *pFields, int fieldNumber );
typedef void (*FN_SETGROUPMASK) ( int mask, int op );
typedef int (*FN_CREATEINSTANCEDBASELINE) ( int classname, struct entity_state_s *baseline );
typedef void (*FN_CVAR_DIRECTSET) ( struct cvar_s *var, char *value );
typedef void (*FN_FORCEUNMODIFIED) ( FORCE_TYPE type, float *mins, float *maxs, const char *filename );
typedef void (*FN_GETPLAYERSTATS) ( const edict_t *pClient, int *ping, int *packet_loss );
typedef void (*FN_ADDSERVERCOMMAND) ( char *cmd_name, void (*function) (void) );
// Added in SDK 2.2:
typedef qboolean (*FN_VOICE_GETCLIENTLISTENING) (int iReceiver, int iSender);
typedef qboolean (*FN_VOICE_SETCLIENTLISTENING) (int iReceiver, int iSender, qboolean bListen);
// Added for HL 1109 (no SDK update):
typedef const char * (*FN_GETPLAYERAUTHID) (edict_t *e);
// Added 2003/11/10 (no SDK update):
typedef sequenceEntry_s * (*FN_SEQUENCEGET) (const char* fileName, const char* entryName);
typedef sentenceEntry_s * (*FN_SEQUENCEPICKSENTENCE) (const char* groupName, int pickMethod, int *picked);
typedef int (*FN_GETFILESIZE) (char *filename);
typedef unsigned int (*FN_GETAPPROXWAVEPLAYLEN) (const char *filepath);
typedef int (*FN_ISCAREERMATCH) (void);
typedef int (*FN_GETLOCALIZEDSTRINGLENGTH) (const char *label);
typedef void (*FN_REGISTERTUTORMESSAGESHOWN) (int mid);
typedef int (*FN_GETTIMESTUTORMESSAGESHOWN) (int mid);
typedef void (*FN_PROCESSTUTORMESSAGEDECAYBUFFER) (int *buffer, int bufferLength);
typedef void (*FN_CONSTRUCTTUTORMESSAGEDECAYBUFFER) (int *buffer, int bufferLength);
typedef void (*FN_RESETTUTORMESSAGEDECAYDATA) (void);
// Added 2005/08/11 (no SDK update):
typedef void (*FN_QUERYCLIENTCVARVALUE) ( const edict_t *player, const char *cvarName );
// Added 2005/11/21 (no SDK update):
typedef void (*FN_QUERYCLIENTCVARVALUE2) ( const edict_t *player, const char *cvarName, int requestID );
// Added 2009/06/17 (no SDK update):
typedef void (*FN_ENGCHECKPARM) ( const char *pchCmdLineToken, char **pchNextVal );

#endif /* ENGINE_API_H */

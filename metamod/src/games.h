// vi: set ts=4 sw=4 :
// vim: set tw=75 :
 
// games.h - list of supported game mods and their data
 
/*
 * Copyright (c) 2001-2008 Will Day <willday@hpgx.net>
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
 
// This list is now kept in a separate file to facilitate generating the
// list from game data stored in a convenient db.

#ifdef __amd64__
#  define MODARCH "_amd64"
#else
#  define MODARCH "_i386"
#endif

{"action",         "ahl"MODARCH".so",        "ahl.dll",        "none",            "Action Half-Life"},
{"ag",             "ag"MODARCH".so",         "ag.dll",         "none",            "Adrenaline Gamer Steam"},
{"ag3",            "hl"MODARCH".so",         "hl.dll",         "none",            "Adrenalinegamer 3.x"},
{"aghl",           "ag"MODARCH".so",         "ag.dll",         "none",            "Adrenalinegamer 4.x"},
{"arg",            "arg"MODARCH".so",        "hl.dll",         "none",            "Arg!"},
{"asheep",         "hl"MODARCH".so",         "hl.dll",         "none",            "Azure Sheep"},
{"bg",             "bg"MODARCH".so",         "bg.dll",         "none",            "The Battle Grounds"},
{"bot",            "bot"MODARCH".so",        "bot.dll",        "none",            "Bot"},
{"brainbread",     "bb"MODARCH".so",         "bb.dll",         "none",            "Brain Bread"},
{"bumpercars",     "hl"MODARCH".so",         "hl.dll",         "none",            "Bumper Cars"},
{"buzzybots",      "bb"MODARCH".so",         "bb.dll",         "none",            "BuzzyBots"},
{"cs13",           "cs"MODARCH".so",         "mp.dll",         "none",            "Counter-Strike 1.3"},
{"cstrike",        "cs"MODARCH".so",         "mp.dll",         "cs.dylib",        "Counter-Strike"},
{"csv15",          "cs"MODARCH".so",         "mp.dll",         "none",            "CS 1.5 for Steam"},
{"czero",          "cs"MODARCH".so",         "mp.dll",         "cs.dylib",        "Counter-Strike:Condition Zero"},
{"dcrisis",        "dc"MODARCH".so",         "dc.dll",         "none",            "Desert Crisis"},
{"dmc",            "dmc"MODARCH".so",        "dmc.dll",        "dmc.dylib",       "Deathmatch Classic"},
{"dod",            "dod"MODARCH".so",        "dod.dll",        "dod.dylib",       "Day of Defeat"},
{"dpb",            "pb.i386.so",             "pb.dll",         "none",            "Digital Paintball"},
{"dragonmodz",     "hl"MODARCH".so",         "mp.dll",         "none",            "Dragon Mod Z"},
{"esf",            "hl"MODARCH".so",         "hl.dll",         "none",            "Earth's Special Forces"},
{"existence",      "ex"MODARCH".so",         "existence.dll",  "none",            "Existence"},
{"firearms",       "fa"MODARCH".so",         "firearms.dll",   "none",            "Firearms"},
{"firearms25",     "fa"MODARCH".so",         "firearms.dll",   "none",            "Retro Firearms"},
{"freeze",         "mp"MODARCH".so",         "mp.dll",         "none",            "Freeze"},
{"frontline",      "front"MODARCH".so",      "frontline.dll",  "none",            "Frontline Force"},
{"gangstawars",    "gangsta"MODARCH".so",    "gwars27.dll",    "none",            "Gangsta Wars"},
{"gangwars",       "mp"MODARCH".so",         "mp.dll",         "none",            "Gangwars"},
{"gearbox",        "opfor"MODARCH".so",      "opfor.dll",      "opfor.dylib",     "Opposing Force"},
{"globalwarfare",  "gw"MODARCH".so",         "mp.dll",         "none",            "Global Warfare"},
{"goldeneye",      "golden"MODARCH".so",     "mp.dll",         "none",            "Goldeneye"},
{"hl15we",         "hl"MODARCH".so",         "hl.dll",         "none",            "Half-Life 1.5: Weapon Edition"},
{"hlrally",        "hlr"MODARCH".so",        "hlrally.dll",    "none",            "HL-Rally"},
{"holywars",       "hl"MODARCH".so",         "holywars.dll",   "none",            "Holy Wars"},
{"hostileintent",  "hl"MODARCH".so",         "hl.dll",         "none",            "Hostile Intent"},
{"ios",            "ios"MODARCH".so",        "ios.dll",        "none",            "International Online Soccer"},
{"judgedm",        "judge"MODARCH".so",      "mp.dll",         "none",            "Judgement"},
{"kanonball",      "hl"MODARCH".so",         "kanonball.dll",  "none",            "Kanonball"},
{"monkeystrike",   "ms"MODARCH".so",         "monkey.dll",     "none",            "Monkeystrike"},
{"MorbidPR",       "morbid"MODARCH".so",     "morbid.dll",     "none",            "Morbid Inclination"},
{"movein",         "hl"MODARCH".so",         "hl.dll",         "none",            "Move In!"},
{"ns",             "ns"MODARCH".so",         "ns.dll",         "none",            "Natural Selection"},
{"nsp",            "ns"MODARCH".so",         "ns.dll",         "none",            "Natural Selection Beta"},
{"oel",            "hl"MODARCH".so",         "hl.dll",         "none",            "OeL Half-Life"},
{"og",             "og"MODARCH".so",         "og.dll",         "none",            "Over Ground"},
{"ol",             "ol"MODARCH".so",         "hl.dll",         "none",            "Outlawsmod"},
{"ops1942",        "spirit"MODARCH".so",     "spirit.dll",     "none",            "Operations 1942"},
{"osjb",           "osjb"MODARCH".so",       "jail.dll",       "none",            "Open-Source Jailbreak"},
{"outbreak",       "none",                   "hl.dll",         "none",            "Out Break"},
{"oz",             "mp"MODARCH".so",         "mp.dll",         "none",            "Oz Deathmatch"},
{"paintball",      "pb"MODARCH".so",         "mp.dll",         "none",            "Paintball"},
{"penemy",         "pe"MODARCH".so",         "pe.dll",         "none",            "Public Enemy"},
{"phineas",        "phineas"MODARCH".so",    "phineas.dll",    "none",            "Phineas Bot"},
{"ponreturn",      "ponr"MODARCH".so",       "mp.dll",         "none",            "Point of No Return"},
{"pvk",            "hl"MODARCH".so",         "hl.dll",         "none",            "Pirates, Vikings and Knights"},
{"rc2",            "rc2"MODARCH".so",        "rc2.dll",        "none",            "Rocket Crowbar 2"},
{"retrocs",        "rcs"MODARCH".so",        "rcs.dll",        "none",            "Retro Counter-Strike"},
{"rewolf",         "hl"MODARCH".so",         "gunman.dll",     "none",            "Gunman Chronicles"},
{"ricochet",       "ricochet"MODARCH".so",   "mp.dll",         "ricochet.dylib",  "Ricochet"},
{"rockcrowbar",    "rc"MODARCH".so",         "rc.dll",         "none",            "Rocket Crowbar"},
{"rspecies",       "hl"MODARCH".so",         "hl.dll",         "none",            "Rival Species"},
{"scihunt",        "shunt.so",               "shunt.dll",      "none",            "Scientist Hunt"},
{"sdmmod",         "sdmmod"MODARCH".so",     "sdmmod.dll",     "none",            "Special Death Match"},
{"Ship",           "ship"MODARCH".so",       "ship.dll",       "none",            "The Ship"},
{"si",             "si"MODARCH".so",         "si.dll",         "none",            "Science & Industry"},
{"snow",           "snow"MODARCH".so",       "snow.dll",       "none",            "Snow-War"},
{"stargatetc",     "hl"MODARCH".so",         "hl.dll",         "none",            "StargateTC"},
{"svencoop",       "hl"MODARCH".so",         "hl.dll",         "none",            "Sven Coop"},
{"swarm",          "swarm"MODARCH".so",      "swarm.dll",      "none",            "Swarm"},
{"tfc",            "tfc"MODARCH".so",        "tfc.dll",        "tfc.dylib",       "Team Fortress Classic"},
{"thewastes",      "thewastes"MODARCH".so",  "thewastes.dll",  "none",            "The Wastes"},
{"timeless",       "pt"MODARCH".so",         "timeless.dll",   "none",            "Project Timeless"},
{"tod",            "hl"MODARCH".so",         "hl.dll",         "none",            "Tour of Duty"},
{"trainhunters",   "th"MODARCH".so",         "th.dll",         "none",            "Train Hunters"},
{"trevenge",       "trevenge.so",            "trevenge.dll",   "none",            "The Terrorist Revenge"},
{"TS",             "ts"MODARCH".so",         "mp.dll",         "none",            "The Specialists"},
{"tt",             "tt"MODARCH".so",         "tt.dll",         "none",            "The Trenches"},
{"underworld",     "uw"MODARCH".so",         "uw.dll",         "none",            "Underworld Bloodline"},
{"valve",          "hl"MODARCH".so",         "hl.dll",         "hl.dylib",        "Half-Life Deathmatch"},
{"vs",             "vs"MODARCH".so",         "mp.dll",         "none",            "VampireSlayer"},
{"wantedhl",       "hl"MODARCH".so",         "wanted.dll",     "none",            "Wanted!"},
{"wasteland",      "whl_linux.so",           "mp.dll",         "none",            "Wasteland"},
{"weapon_wars",    "ww"MODARCH".so",         "hl.dll",         "none",            "Weapon Wars"},
{"wizwars",        "mp"MODARCH".so",         "hl.dll",         "none",            "Wizard Wars"},
{"wormshl",        "wormshl_i586.so",        "wormshl.dll",    "none",            "WormsHL"},
{"zp",             "none",                   "mp.dll",         "none",            "Zombie Panic"},

// vi: set ts=4 sw=4 :
// vim: set tw=75 :
 
// games.h - list of supported game mods and their data
 
/*
 * Copyright (c) 2001-2013 Will Day <willday@hpgx.net>
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

#if defined(__x86_64__) || defined(__amd64__)
#  define MODARCH "_amd64"
#else
#  define MODARCH "_i386"
#endif

	{"action",            "ahl"MODARCH".so",          "ahl.dll",           "Action Half-Life"},
	{"ag",                "ag"MODARCH".so",           "ag.dll",            "Adrenaline Gamer Steam"},
	{"ag3",               "hl"MODARCH".so",           "hl.dll",            "Adrenalinegamer 3.x"},
	{"aghl",              "ag"MODARCH".so",           "ag.dll",            "Adrenalinegamer 4.x"},
	{"arg",               "arg"MODARCH".so",          "hl.dll",            "Arg!"},
	{"asheep",            "hl"MODARCH".so",           "hl.dll",            "Azure Sheep"},
	{"bg",                "bg"MODARCH".so",           "bg.dll",            "The Battle Grounds"},
	{"bot",               "bot"MODARCH".so",          "bot.dll",           "Bot"},
	{"brainbread",        "bb"MODARCH".so",           "bb.dll",            "Brain Bread"},
	{"bumpercars",        "hl"MODARCH".so",           "hl.dll",            "Bumper Cars"},
	{"buzzybots",         "bb"MODARCH".so",           "bb.dll",            "BuzzyBots"},
	{"cs13",              "cs"MODARCH".so",           "mp.dll",            "Counter-Strike 1.3"},
	{"cstrike",           "cs"MODARCH".so",           "mp.dll",            "Counter-Strike"},
	{"csv15",             "cs"MODARCH".so",           "mp.dll",            "CS 1.5 for Steam"},
	{"czero",             "cs"MODARCH".so",           "mp.dll",            "Counter-Strike:Condition Zero"},
	{"dcrisis",           "dc"MODARCH".so",           "dc.dll",            "Desert Crisis"},
	{"dmc",               "dmc"MODARCH".so",          "dmc.dll",           "Deathmatch Classic"},
	{"dod",               "dod"MODARCH".so",          "dod.dll",           "Day of Defeat"},
	{"dpb",               "pb.i386.so",               "pb.dll",            "Digital Paintball"},
	{"dragonmodz",        "hl"MODARCH".so",           "mp.dll",            "Dragon Mod Z"},
	{"esf",               "hl"MODARCH".so",           "hl.dll",            "Earth's Special Forces"},
	{"existence",         "ex"MODARCH".so",           "existence.dll",     "Existence"},
	{"firearms",          "fa"MODARCH".so",           "firearms.dll",      "Firearms"},
	{"firearms25",        "fa"MODARCH".so",           "firearms.dll",      "Retro Firearms"},
	{"freeze",            "mp"MODARCH".so",           "mp.dll",            "Freeze"},
	{"frontline",         "front"MODARCH".so",        "frontline.dll",     "Frontline Force"},
	{"gangstawars",       "gangsta"MODARCH".so",      "gwars27.dll",       "Gangsta Wars"},
	{"gangwars",          "mp"MODARCH".so",           "mp.dll",            "Gangwars"},
	{"gearbox",           "opfor"MODARCH".so",        "opfor.dll",         "Opposing Force"},
	{"globalwarfare",     "gw"MODARCH".so",           "mp.dll",            "Global Warfare"},
	{"goldeneye",         "golden"MODARCH".so",       "mp.dll",            "Goldeneye"},
	{"hl15we",            "hl"MODARCH".so",           "hl.dll",            "Half-Life 1.5: Weapon Edition"},
	{"hlrally",           "hlr"MODARCH".so",          "hlrally.dll",       "HL-Rally"},
	{"holywars",          "hl"MODARCH".so",           "holywars.dll",      "Holy Wars"},
	{"hostileintent",     "hl"MODARCH".so",           "hl.dll",            "Hostile Intent"},
	{"ios",               "ios"MODARCH".so",          "ios.dll",           "International Online Soccer"},
	{"judgedm",           "judge"MODARCH".so",        "mp.dll",            "Judgement"},
	{"kanonball",         "hl"MODARCH".so",           "kanonball.dll",     "Kanonball"},
	{"monkeystrike",      "ms"MODARCH".so",           "monkey.dll",        "Monkeystrike"},
	{"MorbidPR",          "morbid"MODARCH".so",       "morbid.dll",        "Morbid Inclination"},
	{"movein",            "hl"MODARCH".so",           "hl.dll",            "Move In!"},
	{"ns",                "ns"MODARCH".so",           "ns.dll",            "Natural Selection"},
	{"nsp",               "ns"MODARCH".so",           "ns.dll",            "Natural Selection Beta"},
	{"oel",               "hl"MODARCH".so",           "hl.dll",            "OeL Half-Life"},
	{"og",                "og"MODARCH".so",           "og.dll",            "Over Ground"},
	{"ol",                "ol"MODARCH".so",           "hl.dll",            "Outlawsmod"},
	{"ops1942",           "spirit"MODARCH".so",       "spirit.dll",        "Operations 1942"},
	{"osjb",              "osjb"MODARCH".so",         "jail.dll",          "Open-Source Jailbreak"},
	{"outbreak",          "none",                     "hl.dll",            "Out Break"},
	{"oz",                "mp"MODARCH".so",           "mp.dll",            "Oz Deathmatch"},
	{"paintball",         "pb"MODARCH".so",           "mp.dll",            "Paintball"},
	{"penemy",            "pe"MODARCH".so",           "pe.dll",            "Public Enemy"},
	{"phineas",           "phineas"MODARCH".so",      "phineas.dll",       "Phineas Bot"},
	{"ponreturn",         "ponr"MODARCH".so",         "mp.dll",            "Point of No Return"},
	{"pvk",               "hl"MODARCH".so",           "hl.dll",            "Pirates, Vikings and Knights"},
	{"rc2",               "rc2"MODARCH".so",          "rc2.dll",           "Rocket Crowbar 2"},
	{"retrocs",           "rcs"MODARCH".so",          "rcs.dll",           "Retro Counter-Strike"},
	{"rewolf",            "hl"MODARCH".so",           "gunman.dll",        "Gunman Chronicles"},
	{"ricochet",          "ricochet"MODARCH".so",     "mp.dll",            "Ricochet"},
	{"rockcrowbar",       "rc"MODARCH".so",           "rc.dll",            "Rocket Crowbar"},
	{"rspecies",          "hl"MODARCH".so",           "hl.dll",            "Rival Species"},
	{"scihunt",           "shunt.so",                 "shunt.dll",         "Scientist Hunt"},
	{"sdm",               "sdmmod"MODARCH".so",       "sdmmod.dll",        "Special Death Match"},
	{"Ship",              "ship"MODARCH".so",         "ship.dll",          "The Ship"},
	{"si",                "si"MODARCH".so",           "si.dll",            "Science & Industry"},
	{"snow",              "snow"MODARCH".so",         "snow.dll",          "Snow-War"},
	{"stargatetc",        "hl"MODARCH".so",           "hl.dll",            "StargateTC"},
	{"svencoop",          "hl"MODARCH".so",           "hl.dll",            "Sven Coop"},
	{"swarm",             "swarm"MODARCH".so",        "swarm.dll",         "Swarm"},
	{"tfc",               "tfc"MODARCH".so",          "tfc.dll",           "Team Fortress Classic"},
	{"thewastes",         "thewastes"MODARCH".so",    "thewastes.dll",     "The Wastes"},
	{"timeless",          "pt"MODARCH".so",           "timeless.dll",      "Project Timeless"},
	{"tod",               "hl"MODARCH".so",           "hl.dll",            "Tour of Duty"},
	{"trainhunters",      "th"MODARCH".so",           "th.dll",            "Train Hunters"},
	{"trevenge",          "trevenge.so",              "trevenge.dll",      "The Terrorist Revenge"},
	{"TS",                "ts"MODARCH".so",           "mp.dll",            "The Specialists"},
	{"tt",                "tt"MODARCH".so",           "tt.dll",            "The Trenches"},
	{"underworld",        "uw"MODARCH".so",           "uw.dll",            "Underworld Bloodline"},
	{"valve",             "hl"MODARCH".so",           "hl.dll",            "Half-Life Deathmatch"},
	{"vs",                "vs"MODARCH".so",           "mp.dll",            "VampireSlayer"},
	{"wantedhl",          "hl"MODARCH".so",           "wanted.dll",        "Wanted!"},
	{"wasteland",         "whl_linux.so",             "mp.dll",            "Wasteland"},
	{"weapon_wars",       "ww"MODARCH".so",           "hl.dll",            "Weapon Wars"},
	{"wizwars",           "mp"MODARCH".so",           "hl.dll",            "Wizard Wars"},
	{"wormshl",           "wormshl_i586.so",          "wormshl.dll",       "WormsHL"},
	{"zp",                "none",                     "mp.dll",            "Zombie Panic"},

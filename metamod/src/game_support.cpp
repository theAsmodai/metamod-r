#include "precompiled.h"

// Adapted from adminmod h_export.cpp:
//! this structure contains a list of supported mods and their dlls names
//! To add support for another mod add an entry here, and add all the
//! exported entities to link_func.cpp
const game_modinfo_t g_known_games[] = {
	// name/gamedir linux_so        win_dll         desc
	//
	// Previously enumerated in this sourcefile, the list is now kept in a
	// separate file, generated based on game information stored in a
	// convenient db.
	{ "action",             "ahl.so",                   "ahl.dll",                   "Action Half-Life"                                 }, // _i386.so,  + director's cut [2016], updated linux binary name
	{ "ag",                 "ag.so",                    "ag.dll",                    "Adrenaline Gamer"                                 }, // .so file by OpenAG fork
	{ "asheep",             nullptr,                    "hl.dll",                    "Azure Sheep"                                      }, // have no linux binary found!
	{ "bdef",               "server.so",                "server.dll",                "Base Defense"                                     }, // placed in normal dll folder [2017]
	{ "bg",                 "bg.so",                    "bg.dll",                    "The Battle Grounds"                               },
	{ "bhl",                nullptr,                    "bhl.dll",                   "Brutal Half-Life"                                 }, // have no linux binary found!
	{ "brainbread",         nullptr,                    "bb.dll",                    "Brain Bread"                                      }, // have no linux binary found!
	{ "bshift",             "bshift.so",                "hl.dll",                    "Half-Life: Blue Shift"                            },
	{ "bumpercars",         nullptr,                    "hl.dll",                    "Bumper Cars"                                      }, // have no linux binary found!
	{ "buzzybots",          nullptr,                    "bb.dll",                    "BuzzyBots"                                        }, // have no linux binary found!
	{ "ckf3",               nullptr,                    "mp.dll",                    "Chicken Fortress 3"                               }, // have no linux binary found!, checked all versions (latest - Alpha 4)
	{ "cs10",               nullptr,                    "mp.dll",                    "Counter-Strike 1.0"                               }, // have no linux binary found!
	{ "csv15",              nullptr,                    "mp.dll",                    "Counter-Strike 1.5"                               }, // have no linux binary found!
	{ "cstrike",            "cs.so",                    "mp.dll",                    "Counter-Strike 1.6"                               },
	{ "czero",              "cs.so",                    "mp.dll",                    "Counter-Strike:Condition Zero"                    },
	{ "czeror",             "cz.so",                    "cz.dll",                    "Counter-Strike:Condition Zero Deleted Scenes"     },
	{ "dcrisis",            "dc.so",                    "dc.dll",                    "Desert Crisis"                                    }, // _i386.so, updated linux binary name [2010]
	{ "decay",              nullptr,                    "decay.dll",                 "Half-Life: Decay"                                 }, // have no linux binary!
	{ "dmc",                "dmc.so",                   "dmc.dll",                   "Deathmatch Classic"                               },
	{ "dod",                "dod.so",                   "dod.dll",                   "Day of Defeat"                                    },
	{ "dpb",                "pb.i386.so",               "pb.dll",                    "Digital Paintball"                                }, // ...
	{ "esf",                "../linuxdll/hl.so",        "hl.dll",                    "Earth's Special Forces (old)"                     }, // _i386.so, workaround for basic-linux version
	{ "esf",                "hl.so",           		    "hl.dll",                    "Earth's Special Forces"                           }, // _i386.so, full linux version
	{ "existence",          nullptr,                    "existence.dll",             "Existence"                                        }, // have no linux binary found!
	{ "firearms",           nullptr,                    "firearms.dll",              "Firearms"                                         }, // have no linux binary found!
	{ "frontline",          "front.so",                 "frontline.dll",             "Frontline Force"                                  }, // _i386.so, updated linux binary name [2012]
	{ "gangstawars",        nullptr,                    "gwars27.dll",               "Gangsta Wars"                                     }, // have no linux binary found!
	{ "gangwars",           nullptr,                    "mp.dll",                    "Gangwars"                                         }, // have no linux binary found!
	{ "gearbox",            "opfor.so",                 "opfor.dll",                 "Opposing Force"                                   },
	{ "globalwarfare",      "gw.so",                    "mp.dll",                    "Global Warfare"                                   }, // _i386.so, updated linux binary name [2012]
	{ "goldeneye",          nullptr,                    "mp.dll",                    "Goldeneye"                                        }, // have no linux binary found!
	{ "hcfrenzy",           "hcfrenzy.so",              "hcfrenzy.dll",              "Headcrab Frenzy"                                  },
	{ "hl15we",             "hl.so",                    "hl.dll",                    "Half-Life 1.5: Weapon Edition"                    },
	{ "hlrally",            "hlr.so",                   "hlrally.dll",               "HL-Rally"                                         },
	{ "holywars",           "hl.so",                    "holywars.dll",              "Holy Wars"                                        },
	{ "hostileintent",      "hl.so",                    "hl.dll",                    "Hostile Intent"                                   },
	{ "ios",                "ios.so",                   "ios.dll",                   "International Online Soccer"                      },
	{ "judgedm",            "judge.so",                 "mp.dll",                    "Judgement"                                        },
	{ "kanonball",          "hl.so",                    "kanonball.dll",             "Kanonball"                                        },
	{ "monkeystrike",       "ms.so",                    "monkey.dll",                "Monkeystrike"                                     },
	{ "MorbidPR",           "morbid.so",                "morbid.dll",                "Morbid Inclination"                               },
	{ "movein",             "hl.so",                    "hl.dll",                    "Move In!"                                         },
	{ "msc",                nullptr,                    "ms.dll",                    "Master Sword Continued"                           },
	{ "ns",                 "ns.so",                    "ns.dll",                    "Natural Selection"                                },
	{ "nsp",                "ns.so",                    "ns.dll",                    "Natural Selection Beta"                           },
	{ "og",                 "og.so",                    "og.dll",                    "Over Ground"                                      },
	{ "ol",                 "ol.so",                    "hl.dll",                    "Outlawsmod"                                       },
	{ "ops1942",            "spirit.so",                "spirit.dll",                "Operations 1942"                                  },
	{ "osjb",               "osjb.so",                  "jail.dll",                  "Open-Source Jailbreak"                            },
	{ "outbreak",           nullptr,                    "hl.dll",                    "Out Break"                                        }, // have no linux binary found!
	{ "oz",                 "mp.so",                    "mp.dll",                    "Oz Deathmatch"                                    },
	{ "paintball",          "pb.so",                    "mp.dll",                    "Paintball"                                        },
	{ "penemy",             "pe.so",                    "pe.dll",                    "Public Enemy"                                     },
	{ "ponreturn",          "ponr.so",                  "mp.dll",                    "Point of No Return"                               },
	{ "pvk",                "hl.so",                    "hl.dll",                    "Pirates, Vikings and Knights"                     },
	{ "rc2",                "rc2.so",                   "rc2.dll",                   "Rocket Crowbar 2"                                 },
	{ "recbb2",             "recb.so",                  "recb.dll",                  "Resident Evil : Cold Blood"                       },
	{ "rewolf",             nullptr,                    "gunman.dll",                "Gunman Chronicles"                                }, // have no linux binary found!
	{ "ricochet",           "ricochet.so",              "mp.dll",                    "Ricochet"                                         },
	{ "rockcrowbar",        "rc.so",                    "rc.dll",                    "Rocket Crowbar"                                   }, // + _i386.so
	{ "rspecies",           "hl.so",                    "hl.dll",                    "Rival Species"                                    },
	{ "scihunt",            "shunt.so",                 "shunt.dll",                 "Scientist Hunt"                                   },
	{ "ship",               "ship.so",                  "ship.dll",                  "The Ship"                                         },
	{ "si",                 "si.so",                    "si.dll",                    "Science & Industry"                               },
	{ "snow",               "snow.so",                  "snow.dll",                  "Snow-War"                                         },
	{ "stargatetc",         "hl.so",                    "hl.dll",                    "StargateTC (Old, 1.x)"                            },
	{ "stargatetc",         "stc.so",                   "hl.dll",                    "StargateTC (Updated, 2.x)"                        }, // _i386.so
	{ "stargatetc",         "stc_i386_opt.so",          "hl.dll",                    "StargateTC (Updated, 2.x)"                        }, // ...
	{ "svencoop",           "hl.so",                    "hl.dll",                    "Sven Coop (Old)"                                  },
	{ "svencoop",           "server.so",                "server.dll",                "Sven Coop (Steam)"                                },
	{ "swarm",              "swarm.so",                 "swarm.dll",                 "Swarm"                                            },
	{ "tfc",                "tfc.so",                   "tfc.dll",                   "Team Fortress Classic"                            },
	{ "thewastes",          "thewastes.so",             "thewastes.dll",             "The Wastes"                                       }, 
	{ "timeless",           "pt.so",                    "timeless.dll",              "Project Timeless"                                 },
	{ "tod",                "hl.so",                    "hl.dll",                    "Tour of Duty"                                     },
	{ "trainhunters",       "th.so",                    "th.dll",                    "Train Hunters"                                    },
	{ "ts",	                "ts.so",                    "mp.dll",		         	 "The Specialists"                                  }, //_i686.so, _i386.so 
	{ "tt",                 "tt.so",                    "tt.dll",                    "The Trenches"                                     },
	{ "underworld",         "uw.so",                    "uw.dll",                    "Underworld Bloodline"                             },
	{ "valve",              "hl.so",                    "hl.dll",                    "Half-Life"                                        },
	{ "vs",                 "vs.so",                    "mp.dll",                    "VampireSlayer"                                    },
	{ "wantedhl",           "hl.so",                    "wanted.dll",                "Wanted!"                                          },
	{ "wizardwars",         "wizardwars.so",    		"wizardwars.dll",            "Wizard Wars (Steam)"                              },
	{ "wizardwars_beta",    "wizardwars.so",     		"wizardwars.dll",            "Wizard Wars Beta (Steam)"                         }, // folder's name is not same as original. not duplicate
	{ "wizwars",            "mp.so",                    "hl.dll",                    "Wizard Wars (Old)"                                },
	{ "wormshl",            "wormshl.so",               "wormshl.dll",               "WormsHL"                                          }, // _i586.so old, _i686.so steam
	{ "zp",                 "hl.so",                    "mp.dll",                    "Zombie Panic"                                     }, // _i386.so


	// End of list terminator:
	{ nullptr, nullptr, nullptr, nullptr }
};

// Find a modinfo corresponding to the given game name.
static const game_modinfo_t *lookup_game(const char *name)
{
	for (auto& known : g_known_games) {
		if (known.name && !Q_stricmp(known.name, name))
			return &known;
	}

	// no match found
	return nullptr;
}

bool lookup_game_postfixes(gamedll_t *gamedll)
{
	char pathname[MAX_PATH];
	static char postfix_path[MAX_PATH] = "";

	Q_strlcpy(pathname, gamedll->pathname);

	// find extensions and skip
	char *pos = Q_strrchr(pathname, '.');
	if (pos) {
		*pos = '\0';
	}

	for (size_t i = 0; i < arraysize(g_platform_postfixes); i++)
	{
		postfix_path[0] = '\0';
		Q_strlcat(postfix_path, pathname);
		Q_strlcat(postfix_path, g_platform_postfixes[i]);

		if (is_file_exists_in_gamedir(postfix_path)) {
			Q_strlcpy(gamedll->pathname, postfix_path);
			Q_strlcpy(gamedll->real_pathname, postfix_path);
			gamedll->file = postfix_path;

			return true;
		}
	}

	return false;
}

// Installs gamedll from Steam cache
bool install_gamedll(char *from, const char *to)
{
	if (!from)
		return false;

	if (!to)
		to = from;

	int length_in;
	byte *cachefile = LOAD_FILE_FOR_ME(from, &length_in);

	// If the file seems to exist in the cache.
	if (cachefile) {
		int fd = open(to, O_WRONLY | O_CREAT | O_EXCL | O_BINARY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
		if (fd < 0) {
			META_DEBUG(3, "Installing gamedll from cache: Failed to create file %s: %s", to, strerror(errno));
			FREE_FILE(cachefile);
			return false;
		}

		int length_out = write(fd, cachefile, length_in);
		FREE_FILE(cachefile);
		close(fd);

		// Writing the file was not successfull
		if (length_out != length_in) {
			META_DEBUG(3, "Installing gamedll from chache: Failed to write all %d bytes to file, only %d written: %s", length_in, length_out, strerror(errno));

			// Let's not leave a mess but clean up nicely.
			if (length_out >= 0)
				_unlink(to);

			return false;
		}

		META_LOG("Installed gamedll %s from cache.", to);
	}
	else {
		META_DEBUG(3, "Failed to install gamedll from cache: file %s not found in cache.", from);
		return false;
	}

	return true;
}

// Set all the fields in the gamedll struct, - based either on an entry in
// known_games matching the current gamedir, or on one specified manually
// by the server admin.
//
// meta_errno values:
//  - ME_NOTFOUND	couldn't recognize game
bool setup_gamedll(gamedll_t *gamedll)
{
	bool override = false;
	const game_modinfo_t *known;
	const char *knownfn = nullptr;

	// First, look for a known game, based on gamedir.
	if ((known = lookup_game(gamedll->name))) {
#ifdef _WIN32
		knownfn = known->win_dll;
#else
		knownfn = known->linux_so;
#endif
	}

	// Neither override nor known-list found a gamedll.
	if ((!known || !knowfn) && !g_config->m_gamedll)
		return false;

	// Use override-dll if specified.
	if (g_config->m_gamedll) {
		Q_strlcpy(gamedll->pathname, g_config->m_gamedll);

		// If the path is relative, the gamedll file will be missing and
		// it might be found in the cache file.
		if (!is_abs_path(gamedll->pathname)) {
			char szInstallPath[MAX_PATH];
			Q_snprintf(szInstallPath, sizeof(szInstallPath), "%s/%s", gamedll->gamedir, gamedll->pathname);

			// If we could successfully install the gamedll from the cache we
			// rectify the pathname to be a full pathname.
			if (install_gamedll(gamedll->pathname, szInstallPath)) {
				Q_strlcpy(gamedll->pathname, szInstallPath);
			}
		}

		override = true;
	}
	// Else use Known-list dll.
	else if (known) {
		Q_snprintf(gamedll->pathname, sizeof(gamedll->pathname), "%s/dlls/%s", gamedll->gamedir, knownfn);
	}
	else {
		// Neither override nor known-list found a gamedll.
		return false;
	}

	// get filename from pathname
	char *cp = Q_strrchr(gamedll->pathname, '/');
	if (cp)
		cp++;
	else
		cp = gamedll->pathname;

	gamedll->file = cp;

	// If found, store also the supposed "real" dll path based on the
	// gamedir, in case it differs from the "override" dll path.
	if (known && override) {
		Q_snprintf(gamedll->real_pathname, sizeof(gamedll->real_pathname), "%s/dlls/%s", gamedll->gamedir, knownfn);
	}
	else {
		Q_strlcpy(gamedll->real_pathname, gamedll->pathname);
	}

	if (override) {
		// generate a desc
		Q_snprintf(gamedll->desc, sizeof(gamedll->desc), "%s (override)", gamedll->file);

		// log result
		META_LOG("Overriding game '%s' with dllfile '%s'", gamedll->name, gamedll->file);
	}
	else if (known) {
		Q_strlcpy(gamedll->desc, known->desc);

#if !defined(_WIN32)
		if (!is_file_exists_in_gamedir(gamedll->pathname))
		{
			// trying lookup gamedll with postfixes ie _i386.so
			if (lookup_game_postfixes(gamedll)) {
				META_DEBUG(3, "dll: Trying lookup to gamedll with postfixes was a success. Game '%s'", gamedll->pathname);
			}
		}
#endif

		META_LOG("Recognized game '%s'; using dllfile '%s'", gamedll->name, gamedll->file);
	}

	return true;
}

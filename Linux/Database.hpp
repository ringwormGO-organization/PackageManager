/**
 * @author: Andrej Bartulin
 * PROJECT: Package Manager - Linux version
 * LICENSE: BSD-3-Clause-License
 * DESCRIPTION: Database
 * 
*/

#pragma once

/* include */
#include <iostream>
#include <map>

/* commands */
std::string opengl_link;
std::string opengl_version_link;

std::string lua_link;
std::string lua_version_link;

/* link database */
static std::map<const std::string, const std::string> database_link = 
{
    {"OpenGL", opengl_version_link},
    {"Termi-Lua", lua_version_link}
};

/* database */
static std::map<const std::string, const std::string> database = 
{
    {"OpenGL", opengl_link},
    {"Termi-Lua", lua_link}
};
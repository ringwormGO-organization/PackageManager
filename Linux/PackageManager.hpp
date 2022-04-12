/**
 * @author: Andrej Bartulin
 * PROJECT: Package Manager - Linux version
 * LICENSE: BSD-3-Clause-License
 * DESCRIPTION: Main header file for package manager
 * 
*/

#pragma once

#include <iostream>
#include <map>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "indicators.hpp"

/* GitHub repo link */
const char* repo_link = "https://github.com/ringwormGO-organization/Termi";

/* struct which contains settings for package manager */
struct Settings
{
    int install_mode;
    std::string default_path;
};

/* Struct which contains information about Termi installation */
struct Install
{
    std::string install_path;
    bool isAdditionEnabled;
};

/* Required for curl */
size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) 
{
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

/* Required for us */
int isEnding (std::string const &fullString, std::string const &ending) 
{
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } 

    else 
    {
        return 1;
    }
}

/* Class which contains functions  */
class Functions
{
    public:
        /* Set up what we need */
        Functions();

        /* Help function */
        void Help(int type, char* argument);

        /* Serach function - returns a command or link */
        const char* Search(std::string name);

        /* Add function */
        void Add(std::string name, std::string link);

        /* Remove function */
        void Remove(std::string name);

        /* Install functions */
        int Install(std::string name);
        int Install(std::string name, std::string link);

        /* Uninstall functions */
        void Uninstall(std::string name);
        void Uninstall(std::string name, std::string path);

        /* Settings function */
        void Settings();

    private:
        /* Download a file or folder - found link from database */
        int Download(const char* name);
        /* Download a file or folder*/
        int Download(const char* name, const char* link);

        static int Progress(void* ptr, double TotalToDownload, double NowDownloaded, double TotalToUpload, double NowUploaded);
};

extern Settings settings;
extern Install install;

extern Functions* functions;

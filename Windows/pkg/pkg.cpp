/**
 * @author: Andrej Bartulin
 * PROJECT: Package Manager - Windows version
 * LICENSE: BSD-3-Clause-License
 * DESCRIPTION: Main C++ file for package manager
 * 
*/


#include "PackageManager.hpp"
#include "Database.hpp"

#include <curl/curl.h>
#include <curl/easy.h>

#include <Windows.h>

#pragma warning(disable : 4996)

Settings settings;
Install install;

Functions* functions;

/* Required for curl */
// https://curl.se/libcurl/c/CURLOPT_XFERINFOFUNCTION.html
int download_progress_callback(void* clientp, curl_off_t dltotal,
                               curl_off_t dlnow, curl_off_t ultotal,
                               curl_off_t ulnow)
{
    indicators::ProgressBar* progress_bar =
        static_cast<indicators::ProgressBar*>(clientp);

    if (progress_bar->is_completed())
    {
        ;
    }
    else if (dltotal == 0)
    {
        progress_bar->set_progress(0);
    }
    else
    {
        int percentage =
            static_cast<float>(dlnow) / static_cast<float>(dltotal) * 100;
        progress_bar->set_progress(percentage);
    }

    // If your callback function returns CURL_PROGRESSFUNC_CONTINUE it will
    // cause libcurl to continue executing the default progress function. return
    // CURL_PROGRESSFUNC_CONTINUE;

    return 0;
}

int download_progress_default_callback(void* clientp, curl_off_t dltotal,
                                       curl_off_t dlnow, curl_off_t ultotal,
                                       curl_off_t ulnow)
{
    return CURL_PROGRESSFUNC_CONTINUE;
}

Functions::Functions()
{

}

/* Help function */
void Functions::Help(int type, char* argument)
{
    /**
     * 0: Do not print any argument warning and/or error
     * 1: Print no argument error.
     * 2. Print wrong argument error.
    */

    printf("Termi package manager designed to install addition to Termi\n");
    printf("Termi's GitHub repo: %s\n\n", repo_link);

    switch (type)
    {
        case 0:
            break;

        case 1:
            printf("There is no enough arguments! See help down below\n\n");
            break;

        case 2:
            printf("'%s' is wrong argument! See help down below\n\n", argument);
            break;

        default:
            break;
    }
    
    printf("search <software name> - search software in database\n");
    printf("add <software name> - add software download link to database\n");
    printf("rm <software name> - remove software from database\n");
    printf("install <software name> - install software, if it is not in database, you can add it or type download link\n");
    printf("uninstall <software name> - uninstall software from computer\n");
    printf("settings - open settings\n\n");
    printf("Sometimes, instead of download link, you can use command which you normally paste to terminal\n");
    printf("Also is recommended to run this package manager as administartor/sudo");
    printf("\n");
}

/* Serach function - returns a command or link */
const char* Functions::Search(std::string name)
{
    auto search = database.find(name);

    if (search != database.end())
    {
        std::cout << "Found " << search->first << " as " << search->second << '\n';
    }

    else
    {
        std::cout << "Unable to found " << name << "!\n";
        return "Unable to find package";
    }

    return search->second.c_str();
}

const char* Functions::SearchLink(std::string name)
{
    auto search = database_link.find(name);

    if (search != database_link.end()) 
    {
        return search->second.c_str();
    }

    else
    {
        std::cout << "Unable to found version link from package " << name << "!\n";
        return "Unable to found package";
    }
}

/* Add function */
void Functions::Add(std::string name, std::string version_link, std::string download_link)
{
    auto position1(end(database_link));
    position1 = database_link.insert
    (
        position1,
        { 
            name, 
            version_link
        }
    );

    auto position2(end(database));
    position2 = database.insert
    (
        position2,
        { 
            name, 
            download_link 
        }
    );

    SearchLink(name);
    Search(name);
}

/* Remove function */
void Functions::Remove(std::string name)
{
    database_link.erase(name);
    database.erase(name);

    SearchLink(name);
    Search(name);
}

/* Update function */
int Functions::Update(std::string name)
{
    if (!strcmp(SearchLink(name), "Unable to found package"))
    {
        return 1;
    }

    else
    {
        return Install(name);
    }
}

/* Install functions */
int Functions::Install(std::string name)
{
    if (Download(name.c_str()) == 0)
    {
        if (isEnding(name, ".tar.gz") | isEnding(name, ".zip"))
        {
            /* remove older version */
            /* unpack */
        }

        else
        {
            /* remove older version */
            /* move to right folder */
        }

        return 0;
    }
    else
    {
        printf("Package Manager can't find name or link. Try add name or link to database or try call this function with following arguments: \n");
        printf(" <name> <link> \n");
        printf("If you still can't download, report an issue on Termi's GitHub (see help for link).\n");
        return 1;
    }
}

int Functions::Install(std::string name, std::string link)
{
    if (Download(name.c_str(), link.c_str()) == 0)
    {
        if (isEnding(name, ".tar.gz") | isEnding(name, ".zip"))
        {
            /* unpack */
        }

        else
        {
            /* move to right folder */
        }

        return 0;
    }
    
    else
    {
        return 1;
    }
}

/* Uninstall functions */
void Functions::Uninstall(std::string name)
{

}

void Functions::Uninstall(std::string name, std::string path)
{

}

/* Settings function */
void Functions::Settings()
{
    int id;

    std::string temp_path;

    printf("Enter id of setting to change (zero is help): ");
    scanf("%d", &id);

    switch (id)
    {
        default:
            printf("Wrong setting id!");
            break;
    }
}

/* PRIVATE STUFF OF CLASS */

/* Download a file or folder */
int Functions::Download(const char* name)
{
    CURL *curl;
    FILE *fp;
    CURLcode res;
    const char* url = Search(name);
    const char* outfilename = name;

    char tmpfilename[PATH_MAX];
    snprintf(tmpfilename, PATH_MAX - 1, "./%s.XXXXXX", outfilename);
    //int fd = mkstemp(tmpfilename);

    indicators::ProgressBar progress_bar
    {
        indicators::option::BarWidth{30}, indicators::option::Start{" ["},
        indicators::option::Fill{"???"}, indicators::option::Lead{"???"},
        indicators::option::Remainder{"-"}, indicators::option::End{"]"},
        indicators::option::PrefixText{outfilename},
        // indicators::option::ForegroundColor{indicators::Color::yellow},
        indicators::option::ShowElapsedTime{true},
        indicators::option::ShowRemainingTime{true},
        // indicators::option::FontStyles{
        //     std::vector<indicators::FontStyle>{indicators::FontStyle::bold}}
    };

    curl = curl_easy_init();                                                                                                                                                                                                                                                           
    if (curl)
    {   
        fp = fopen(tmpfilename, "wb");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION,
                         download_progress_callback);
        curl_easy_setopt(curl, CURLOPT_XFERINFODATA,
                         static_cast<void*>(&progress_bar));
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
        // Perform a file transfer synchronously.
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);

        if (res == CURLE_OK)
        {
            printf("Downloaded %s as %s.\n", tmpfilename, outfilename);
        }
        
        else
        {
            printf("Unable to download %s as %s!\n", tmpfilename, outfilename);
        }
    }

    else
    {
        return 1;
    }

    rename(tmpfilename, outfilename);

    return 0;
}

int Functions::Download(const char* name, const char* link)
{
    CURL *curl;
    FILE *fp;
    CURLcode res;
    const char* url = link;
    const char* outfilename = name;

    char tmpfilename[PATH_MAX];
    snprintf(tmpfilename, PATH_MAX - 1, "./%s.XXXXXX", outfilename);
    //int fd = mkstemp(tmpfilename);

    indicators::ProgressBar progress_bar
    {
        indicators::option::BarWidth{30}, indicators::option::Start{" ["},
        indicators::option::Fill{"???"}, indicators::option::Lead{"???"},
        indicators::option::Remainder{"-"}, indicators::option::End{"]"},
        indicators::option::PrefixText{outfilename},
        // indicators::option::ForegroundColor{indicators::Color::yellow},
        indicators::option::ShowElapsedTime{true},
        indicators::option::ShowRemainingTime{true},
        // indicators::option::FontStyles{
        //     std::vector<indicators::FontStyle>{indicators::FontStyle::bold}}
    };

    curl = curl_easy_init();                                                                                                                                                                                                                                                           
    if (curl)
    {   
        fp = fopen(tmpfilename, "wb");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION,
                         download_progress_callback);
        curl_easy_setopt(curl, CURLOPT_XFERINFODATA,
                         static_cast<void*>(&progress_bar));
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
        // Perform a file transfer synchronously.
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);

        if (res == CURLE_OK)
        {
            printf("Downloaded %s as %s.\n", tmpfilename, outfilename);
        }
        
        else
        {
            printf("Unable to download %s as %s!\n", tmpfilename, outfilename);
        }
    }

    else
    {
        return 1;
    }

    rename(tmpfilename, outfilename);

    return 0;
}

/* main function */
int main(int argc, char** argv)
{
    if (argc == 1)
    {
        functions->Help(1, NULL);
        return 0;
    }

    if (!strcmp(argv[1], "search"))
    {
        if (argv[2] == NULL)
        {
            printf("There is no software name!\n");
        }
        else
        {
            functions->Search(argv[2]);
        }
    }

    else if (!strcmp(argv[1], "add"))
    {
        if (argv[2] == NULL)
        {
            printf("There is no software name!\n");
        }
        else
        {
            if (argv[3] == NULL | argv[4] == NULL)
            {
                printf("There is no version link and/or download link\n");
            }
            else
            {
                functions->Add(argv[2], argv[3], argv[4]);
            }
        }
    }

    else if (!strcmp(argv[1], "remove"))
    {
        if (argv[2] == NULL)
        {
            printf("There is no software name!\n");
        }
        else
        {
            functions->Remove(argv[2]);
        }
    }

    else if (!strcmp(argv[1], "update"))
    {
        if (argv[2] == NULL)
        {
            printf("There is no software name!\n");
        }
        else
        {
            functions->Update(argv[3]);
        }
    }

    else if (!strcmp(argv[1], "install"))
    {
        if (argv[2] == NULL)
        {
            printf("There is no software name!\n");
        }
        else
        {
            if (argv[3] == NULL)
            {
                functions->Install(argv[2]);
            }
            else
            {
                functions->Install(argv[2], argv[3]);
            }
        }
    }

    else if (!strcmp(argv[1], "uninstall"))
    {
        if (argv[2] == NULL)
        {
            printf("There is no software name!\n");
        }
        else
        {
            /* uninstall(argv[2]); */
        }
    }

    else if (!strcmp(argv[1], "settings"))
    {
        functions->Settings();
    }

    else if (!strcmp(argv[1], "help"))
    {
        functions->Help(0, NULL);
    }

    else
    {
        functions->Help(2, argv[1]);
    }

    return 0;
}
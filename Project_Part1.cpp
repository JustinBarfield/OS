#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fstream>

using namespace std;

// Function to print the current directory
void printCurrentDirectory()
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        cout << "root>" << cwd << "?> ";
    }
    else
    {
        perror("getcwd() error");
    }
}

// Function to list files in the current directory
void listFiles(const string &flag)
{
    DIR *dir;
    struct dirent *entry;
    struct stat fileStat;

    dir = opendir(".");
    if (dir != NULL)
    {
        while ((entry = readdir(dir)) != NULL)
        {
            if (flag == "-curr")
            {
                // Only show non-deleted files/directories
                if (entry->d_name[0] != '.')
                {
                    cout << entry->d_name << endl;
                }
            }
            else if (flag == "-all")
            {
                // Show all files/directories (deleted and existing)
                cout << entry->d_name << endl;
            }
        }
        closedir(dir);
    }
    else
    {
        perror("opendir() error");
    }
}

// Function to change directory
void changeDirectory(const string &path)
{
    if (chdir(path.c_str()) != 0)
    {
        perror("chdir() error");
    }
}

// Function to create directory
void createDirectory(const string &name)
{
    if (mkdir(name.c_str(), 0777) != 0)
    {
        perror("mkdir() error");
    }
}

// Function to delete file or directory
void deleteFileOrDirectory(const string &name)
{
    if (remove(name.c_str()) != 0)
    {
        perror("remove() error");
    }
}

// Function to read file and display contents
void readFile(const string &filename)
{
    ifstream file(filename);
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            cout << line << endl;
        }
        file.close();
    }
    else
    {
        cerr << "Unable to open file: " << filename << endl;
    }
}

int main()
{
    string command;
    string option;

    while (true)
    {
        // Print current directory
        printCurrentDirectory();

        // Read user input
        getline(cin, command);

        // Parse command and option
        size_t spacePos = command.find(" ");
        if (spacePos != string::npos)
        {
            option = command.substr(spacePos + 1);
            command = command.substr(0, spacePos);
        }

        // Execute command
        if (command == "pwd")
        {
            // Print current directory path
            printCurrentDirectory();
            cout << endl;
        }
        else if (command == "cd")
        {
            // Change directory to the specified path
            changeDirectory(option);
        }
        else if (command == "ls")
        {
            // List files in the current directory based on the specified flag
            listFiles(option);
        }
        else if (command == "mkdir")
        {
            // Create a directory with the specified name in the current directory
            createDirectory(option);
        }
        else if (command == "delete")
        {
            // Delete the file or directory with the specified name in the current directory
            deleteFileOrDirectory(option);
        }
        else if (command == "read")
        {
            // Display the contents of the file in ASCII
            readFile(option);
        }
        else if (command == "quit")
        {
            // Exit the program
            break;
        }
        else
        {
            // Invalid command
            cout << "Invalid command. Please try again." << endl;
        }
    }

    return 0;
}

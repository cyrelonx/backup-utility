#include <filesystem>
#include <iostream>
#include <fstream>
#include <chrono>

using namespace std;
namespace fs = std::filesystem;

void backup(const fs::path& src, const fs::path& dest);
void copyFile(const fs::path& src, const fs::path& dest);

void copyFile(const fs::path& src, const fs::path& dest)
{
    if (!fs::exists(dest))
    {
        fs::create_directories(dest.parent_path());
        fs::copy_file(src, dest, fs::copy_options::overwrite_existing);
        cout << "New file: " << src << "\n";
    }
    
    else
    {
        auto srcTime = fs::last_write_time(src);
        auto destTime = fs::last_write_time(dest);

        if (srcTime > destTime)
        {
            fs::copy_file(src, dest, fs::copy_options::overwrite_existing);
            cout << "New file found: " << src << "\n";
        }
    }
}

void backup(const fs::path& src, const fs::path& dest)
{
    for (auto& entry: fs::recursive_directory_iterator(src))
    {
        auto relativePath = fs::relative(entry.path(), src);
        auto destPath = dest / relativePath;

        if (fs::is_directory(entry))
            fs::create_directories(destPath);
        else if (fs::is_regular_file(entry))
            copyFile(entry.path(), destPath);
    }
}

void printMenu()
{
    cout << "1. Create backup\n2. Exit\n";
    cout << "Enter your choice: ";
}

int main()
{
    printMenu();
    int choice;

    do
    {
        cin >> choice;
        switch (choice)
        {
            case 1:
            {
                system("cls");

                string src, dest;
                cout << "Enter files location from which you want to create backup: ";
                cin >> src;

                cout << "Enter the destination where you want to create backup files: ";
                cin >> dest;

                fs::path source(src);
                fs::path destination(dest);

                backup(source, destination);

                printMenu();
                break;
            }
        
            default:
            {
                cout << "Invalid option!\n";
                printMenu();
                break;
            }
        }
    } while (choice != 2);
    return 0;
}
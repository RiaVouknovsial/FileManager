//*Первое задание
//Реализовать простейший файловый менеджер с использованием ООП(классы,
//наследование и так далее).
//	Файловый менеджер должен иметь такие возможности :
//- показывать содержимое дисков;
//- создавать папки / файлы;
//- удалять папки / файлы;
//- переименовывать папки / файлы;
//- копировать / переносить папки / файлы;
//- вычислять размер папки / файла;
//- производить поиск по маске(с поиском по подпапкам) и так далее.*/
//
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <vector>
#include <direct.h>
#include <Windows.h>
#include <tchar.h>
#include <regex>
#include <filesystem>

using namespace std;
using namespace std::filesystem;

class FileManager
{
public:
    virtual void findDisks(const string& diskName) = 0;         //поиск диска
    virtual void displayAllDiskContents() = 0;                  //показ содаржимого диска
    virtual void CreateDirectory(const string& dir_name) = 0;    //создание папки
    virtual void CreateFile(const string& file_name) = 0;       //создание файла
    virtual void RemoveDirectory(const string& dir_name) = 0;    // удаление папки
    virtual void RemoveFile(const string& file_name) = 0;       // удаление файла
    virtual void RenameDirectory(const string& old_dir_name, const string& new_dir_name) = 0; // переименование папки
    virtual void RenameFile(const string& old_file_name, const string& new_file_name) = 0;      // переименование файла 
    virtual void CopyDirectory(const string& source_dir, const string& dest_dir) = 0;   // копирование папки
    virtual void CopyFile(const string& source_file, const string& dest_file) = 0;          // копирование файла
    virtual void MoveDirectory(const string& old_dir_path, const string& new_dir_path) = 0;  //перемещение папки
    virtual void MoveFile(const string& old_file_path, const string& new_file_path) = 0;   //перемещение файла
    virtual void ReadDirectory(const path& dir_path) = 0;                                         //получение списка папок и файлов
    virtual void ReadFile(const string& file_name) = 0;                                           // чтение файла
    virtual void MaskDirectory(const string& dir_path, const string& dir_mask) = 0;            //поиск по маске папки                                 //поиск по маске папки
    virtual void MaskFile(const string& file_path, const string& file_mask) = 0;                //поиск по маске файла                                            //поиск по маске файла

    virtual ~FileManager() {}
};

class Disk
{
private:
    const path m_path;
public:
    Disk(const path& path) : m_path(path) {}

    void displayContents() const
    {
        cout << "Disk " << m_path << ":" << endl;

        try
        {
            for (const auto& entry : directory_iterator(m_path))
            {
                if (is_regular_file(entry))
                {
                    cout << entry.path() << endl;
                }
                else if (is_directory(entry))
                {
                    cout << "[" << entry.path() << "]" << endl;
                }
            }
        }
        catch (const filesystem_error& ex)
        {
            cout << "Error: " << ex.what() << endl;
        }

        cout << endl;
    }

    const path& getPath() const { return m_path; }
};

class DiskManager : public FileManager
{
private:
    vector<Disk> m_disks;
public:
    void findDisks(const string& disk_name) override
    {
        try
        {
            path p(disk_name);

            if (is_directory(p) && exists(p))
            {
                m_disks.emplace_back(p);
            }
        }
        catch (const filesystem_error& ex)
        {
            cout << "Error: " << ex.what() << endl;
        }
    }

    void displayAllDiskContents() override
    {
        for (const auto& disk : m_disks)
        {
            disk.displayContents();
        }
    }

    void CreateDirectory(const string& dir_name) override {}
    void RemoveDirectory(const string& dir_name) override {}
    void RenameDirectory(const string& old_dir_name, const string& new_dir_name)override {}
    void CopyDirectory(const string& source_dir, const string& dest_dir) override {}
    void MoveDirectory(const string& old_dir_path, const string& new_dir_path) override {}
    void ReadDirectory(const path& dir_path) override {}
    void MaskDirectory(const string& dir_path, const string& dir_mask) override {}
    void CreateFile(const string& file_name) override {}
    void RemoveFile(const string& file_name) override {}
    void RenameFile(const string& old_file_name, const string& new_file_name)override {}
    void CopyFile(const string& source_file, const string& dest_file) override {}
    void MoveFile(const string& old_file_path, const string& new_file_path)override {}
    void ReadFile(const string& file_name) override {}
    void MaskFile(const string& file_path, const string& file_mask) override {}
};

class Directory :public FileManager
{
public:
    const string dir_name;
    const string old_dir_name;
    const string new_dir_name;
    const string source_dir;
    const string dest_dir;
    const string dir_path;
    const string old_dir_path;
    const string new_dir_path;
    const string dir_mask;
    size_t total_size;

    void CreateDirectory(const string& dir_name) override
    {
        if (_mkdir(dir_name.c_str()) == 0)
        {
            cout << "Directory created successfully!" << endl;
        }
        else
        {
            cout << "Error creating directory " << dir_name << endl;
        }
    }

    void RemoveDirectory(const string& dir_name) override
    {
        if (remove_all(dir_name) != 0)
        {
            cout << "Directory deleted successfully!" << endl;
        }
        else
        {
            cout << "Error deleting directory " << dir_name << endl;
        }
    }
    void RenameDirectory(const string& old_dir_name, const string& new_dir_name) override
    {
        if (rename(old_dir_name.c_str(), new_dir_name.c_str()) != 0)
        {
            cout << "Error renaming directory " << old_dir_name << " to " << new_dir_name << endl;
        }
        else
        {
            cout << "Directory renamed successfully!" << endl;
        }
    }

    void CopyDirectory(const string& source_dir, const string& dest_dir)
    {
        try
        {
            // проверка, существует ли исходный каталог
            if (!exists(source_dir))
            {
                cout << "Source directory does not exist." << endl;
                return;
            }

            // проверка, существует ли каталог назначения, при необходимости его создать
            if (!exists(dest_dir))
            {
                create_directory(dest_dir);
            }

            // перебор содержимого исходного каталога
            for (auto& entry : directory_iterator(source_dir))
            {
                // получить путь к текущей записи
                path entryPath = entry.path();

                // если запись - каталог, скопировать ее
                if (is_directory(entryPath))
                {
                    CopyDirectory(entryPath.string(), dest_dir + "\\" + entryPath.filename().string());
                }
                // если запись - файл, скопировать его в каталог
                else if (is_regular_file(entryPath))
                {
                    copy_file(entryPath, dest_dir + "\\" + entryPath.filename().string(), copy_options::overwrite_existing);
                }
            }

            cout << "Directory copied successfully!" << endl;
        }
        catch (const exception& e)
        {
            cout << "Error copying directory: " << e.what() << endl;
        }

    }

    void MoveDirectory(const string& old_dir_path, const string& new_dir_path)
    {
        try
        {
            // проверяем, существует ли исходный каталог
            if (!exists(old_dir_path))
            {
                cout << "The directory " << old_dir_path << " does not exist." << endl;
                return;
            }

            // проверяем, существует ли целевой каталог
            if (exists(new_dir_path))
            {
                cout << "The directory " << new_dir_path << " already exists." << endl;
                return;
            }

            // копируем каталог
            CopyDirectory(old_dir_path, new_dir_path);

            // удаляем исходный каталог
            remove_all(old_dir_path);

            cout << "Directory moved successfully!" << endl;
        }
        catch (const exception& e)
        {
            cout << "Error moving directory: " << e.what() << endl;
        }
    }
    void ReadDirectory(const path& dir_path) override
    {
        string dir_name = "C:\\Windows";
        // если папка существует
        if (exists(dir_name))
        {
            cout << "Subdirectories and Files:" << endl;
            for (const auto& entry : directory_iterator(dir_name))
            {
                if (entry.is_directory())
                {
                    cout << entry.path() << " [subdirectory]" << endl;
                }
                else if (entry.is_regular_file())
                {
                    cout << entry.path() << " [file]" << endl;
                }
            }
        }

    }
    size_t SizeDirectory(const string& dir_path)
    {
        total_size = 0;

        try
        {
            for (const auto& entry : recursive_directory_iterator(dir_path))
            {
                if (is_regular_file(entry))
                {
                    total_size += file_size(entry);
                }
            }
        }

        catch (const exception& e)
        {
            cout << "Error reading directory: " << e.what() << endl;
        }

        return total_size;
    }

    void MaskDirectory(const string& dir_path, const string& dir_mask)
    {
        try
        {
            // Проверка наличия директории
            if (!exists(dir_path))
            {
                throw runtime_error("Directory does not exist");
            }

            // Проверка, является ли путь директорией
            if (!is_directory(dir_path))
            {
                throw runtime_error("Path is not a directory");
            }

            // Создание регулярного выражения из маски
            string regex_str = "^" + dir_mask + "$";
            regex reg(regex_str);

            // Обход всех элементов в директории
            for (const auto& entry : recursive_directory_iterator(dir_path))
            {
                try
                {
                    // Проверка, является ли элемент директорией
                    if (!is_directory(entry))
                    {
                        continue;
                    }

                    // Получение имени файла и проверка наличия маски
                    string filename = entry.path().filename().string();
                    if (regex_match(filename, reg))
                    {
                        cout << entry.path() << endl;
                    }
                }
                catch (const filesystem_error& e)
                {
                    // Обработка исключения при проходе по директории
                    cout << "Error searching directory: " << e.what() << endl;
                }
            }
        }
        catch (const filesystem_error& e)
        {
            // Обработка исключения при проверке наличия директории
            cout << "Error accessing directory: " << e.what() << endl;
        }
        catch (const exception& e)
        {
            // Обработка других исключительных ситуаций
            cout << "Error: " << e.what() << endl;
        }
    }

    void findDisks(const string& disk_name) override {}
    void displayAllDiskContents() override {}
    void CreateFile(const string& file_name) override {}
    void RemoveFile(const string& file_name) override {}
    void RenameFile(const string& old_file_name, const string& new_file_name)override {}
    void CopyFile(const string& source_file, const string& dest_file) override {}
    void MoveFile(const string& old_file_path, const string& new_file_path)override {}
    void ReadFile(const string& file_name) override {}
    void MaskFile(const string& file_path, const string& file_mask) override {}
};


class File : public FileManager
{
public:
    const string file_name;
    const string old_file_name;
    const string new_file_name;
    const string source_file;
    const string dest_file;
    const string old_file_path;
    const string new_file_path;
    const string file_path;
    const string file_mask;

    void CreateFile(const string& file_name) override
    {
        ofstream file(file_name);
        if (file.is_open())
        {
            file << "This is a sample file." << endl;
            file.close();
            cout << "File created successfully!" << endl;
        }
        else
        {
            cout << "Error creating file " << file_name << endl;
        }
    }

    void RemoveFile(const string& file_name) override
    {
        if (remove(file_name.c_str()) != 0)
        {
            cout << "Error deleting file " << file_name << endl;
        }
        else
        {
            cout << "File deleted successfully!" << endl;
        }
    }

    void RenameFile(const string& old_file_name, const string& new_file_name) override
    {
        if (rename(old_file_name.c_str(), new_file_name.c_str()) != 0)
        {
            cout << "Error renaming file " << old_file_name << " to " << new_file_name << endl;
        }
        else
        {
            cout << "File renamed successfully!" << endl;
        }
    }

    void CopyFile(const string& source_file, const string& dest_file) override
    {
        try
        {
            // проверка существования исходного файла
            if (!exists(source_file))
            {
                cout << "The file " << source_file << " does not exist." << endl;
                return;
            }

            // копирование исходного файла в целевой файл
            copy_file(source_file, dest_file, copy_options::overwrite_existing);

            cout << "File copied successfully!" << endl;
        }
        catch (const exception& e)
        {
            cout << "Error copying file: " << e.what() << endl;
        }
    }

    void MoveFile(const string& old_file_path, const string& new_file_path) override
    {
        try
        {
            // проверка существования старого файла
            if (!exists(old_file_path))
            {
                cout << "The file " << old_file_path << " does not exist." << endl;
                return;
            }

            // проверка существования нового файла
            if (exists(new_file_path))
            {
                cout << "The file " << new_file_path << " already exists." << endl;
                return;
            }

            // переименование старого файла в новый файл
            rename(old_file_path, new_file_path);

            cout << "File moved successfully!" << endl;
        }
        catch (const exception& e)
        {
            cout << "Error moving file: " << e.what() << endl;
        }
    }

    void ReadFile(const string& file_name) override
    {

        ifstream file(file_name);
        if (file.is_open())
        {
            string line;
            while (getline(file, line))
            {
                cout << line << endl;
            }
            file.close();
            cout << "File read successfully!" << endl;
        }
        else
        {
            cout << "Error opening file " << file_name << endl;
        }
    }

    size_t SizeFile(const string& file_name)
    {
        ifstream file(file_name, ios::binary | ios::ate);
        if (!file.is_open())
        {
            // обработка ошибки открытия файла
        }
        size_t file_size = static_cast<size_t>(file.tellg());
        file.close();
        return file_size;
    }

    void MaskFile(const string& file_path, const string& file_mask)
    {
        try
        {
            // Проверка наличия директории
            if (!exists(file_path))
            {
                throw runtime_error("Directory does not exist");
            }

            // Проверка, является ли путь директорией
            if (!is_directory(file_path))
            {
                throw runtime_error("Path is not a directory");
            }

            // Создание регулярного выражения из маски
            string regex_str = file_mask;
            regex reg(regex_str);

            // Обход всех элементов в директории и поддиректориях
            for (const auto& entry : recursive_directory_iterator(file_path))
            {
                try
                {
                    // Проверка, является ли элемент файлом
                    if (!is_regular_file(entry))
                    {
                        continue;
                    }

                    // Получение полного пути к файлу и проверка наличия маски
                    string filepath = entry.path().string();
                    if (regex_search(filepath, reg))
                    {
                        cout << filepath << endl;
                    }
                }
                catch (const filesystem_error& e)
                {
                    // Обработка исключения при проходе по файлам
                    cout << "Error searching file: " << e.what() << endl;
                }
            }
        }
        catch (const filesystem_error& e)
        {
            // Обработка исключения при проверке наличия директории
            cout << "Error accessing directory: " << e.what() << endl;
        }
        catch (const std::exception& e)
        {
            // Обработка исключения при проверке прав доступа
            cout << "Error accessing directory: " << e.what() << endl;
        }
        catch (...)
        {
            // Обработка неизвестных исключений
            cout << "Unknown error occurred" << endl;
        }
    }
    void findDisks(const string& disk_name) override {}
    void displayAllDiskContents() override {}
    void CreateDirectory(const string& dir_name) override {}
    void RemoveDirectory(const string& dir_name) override {}
    void RenameDirectory(const string& old_dir_name, const string& new_dir_name)override {}
    void CopyDirectory(const string& source_dir, const string& dest_dir) override {}
    void MoveDirectory(const string& old_dir_path, const string& new_dir_path) override {}
    void ReadDirectory(const path& dir_path) override {}
    void MaskDirectory(const string& dir_path, const string& dir_mask) override {}
};

class Start
{
public:
    int choice;
    string dir_name;
    string dir_name1;
    string dir_name2;
    string old_dir_name;
    string new_dir_name;
    string source_dir;
    string dest_dir;
    string old_dir_path;
    string new_dir_path;
    string dir_path;
    string dir_mask;
    size_t dir_size;
    string file_name;
    string file_name1;
    string file_name2;
    string old_file_name;
    string new_file_name;
    string source_file;
    string dest_file;
    string old_file_path;
    string new_file_path;
    size_t file_size;
    string file_path;
    string file_mask;

    void StartPanel()
    {

        while (true)
        {
            cout << "\t\t\t===FILE  MANAGER===" << endl;
            cout << endl;
            cout << "\t\t Choose an option:" << endl;
            cout << endl;
            cout << "\t\t===DISK OPTION===" << endl;
            cout << "1. Display a disk contents" << endl;
            cout << endl;
            cout << "\t\t===DIRECTORY OPTIONS===" << endl;
            cout << "2. Create a directory" << endl;
            cout << "3. Remove a directory" << endl;
            cout << "4. Rename a directory" << endl;
            cout << "5. Copy a directory" << endl;
            cout << "6. Move a directory" << endl;
            cout << "7. Read a directory" << endl;
            cout << "8. Size of a directory" << endl;
            cout << "9. Search directory by mask" << endl;
            cout << endl;
            cout << "\t\t===FILE OPTIONS===" << endl;
            cout << "10. Create a file" << endl;
            cout << "11. Remove a file" << endl;
            cout << "12. Rename a file" << endl;
            cout << "13. Copy a file" << endl;
            cout << "14. Move a file" << endl;
            cout << "15. Read a file" << endl;
            cout << "16. Size of a file" << endl;
            cout << "17. Search file by mask" << endl;
            cout << "18. Quit" << endl;
            while (true)
            {
                cout << "Enter your choice (1-18): ";
                cin >> choice;
                cout << endl;
                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(cin.rdbuf()->in_avail());
                    cout << "Enter the number from 1 to 18!";
                    cout << endl;
                }
                else
                {
                    cin.ignore(32767, '\n');
                    break;
                }
            }

            switch (choice)
            {
            case 1:
            {
                DiskManager manager;

                // поиска диска с именем "C:"
                manager.findDisks("C:\\");

                // показ содержимого всех найденных дисков
                manager.displayAllDiskContents();

                system("pause"); // ожидание ввода перед очисткой экрана
                system("cls"); // очистка экрана
                break;

            }

            case 2:
            {
                Directory directory;
                dir_name1 = "TEST_dir";
                directory.CreateDirectory(dir_name1);
                dir_name2 = "DIR_dir";
                directory.CreateDirectory(dir_name2);

                system("pause"); // ожидание ввода перед очисткой экрана
                system("cls"); // очистка экрана
                break;
            }
            case 3:
            {
                Directory directory;
                dir_name = "DIR_dir";
                directory.RemoveDirectory(dir_name);

                system("pause"); // ожидание ввода перед очисткой экрана
                system("cls"); // очистка экрана
                break;
            }

            case 4:
            {
                Directory directory;
                old_dir_name = "TEST_dir";
                new_dir_name = "TEST1_dir";
                directory.RenameDirectory(old_dir_name, new_dir_name);

                system("pause"); // ожидание ввода перед очисткой экрана
                system("cls"); // очистка экрана
                break;
            }

            case 5:
            {
                Directory directory;
                source_dir = "TEST1_dir";
                dest_dir = "TEST2_dir";
                directory.CopyDirectory(source_dir, dest_dir);
                system("pause"); // ожидание ввода перед очисткой экрана
                system("cls"); // очистка экрана
                break;
            }

            case 6:
            {
                Directory directory;
                old_dir_path = "C:\\Users\\user\\source\\repos\\Project8-FileManager\\TEST_dir";
                new_dir_path = "E:\\TEST_dir";
                directory.MoveDirectory(old_dir_path, new_dir_path);
                system("pause"); // ожидание ввода перед очисткой экрана
                system("cls"); // очистка экрана
                break;
            }

            case 7:
            {
                Directory directory;
                dir_path = "C:\\Windows";
                directory.ReadDirectory(dir_path);
                system("pause"); // ожидание ввода перед очисткой экрана
                system("cls"); // очистка экрана
                break;
            }

            case 8:
            {
                Directory directory;
                dir_path = "C:\\Windows\\Microsoft.NET";
                dir_size = directory.SizeDirectory(dir_path);
                cout << "Size of directory " << dir_path << " is " << dir_size << " bytes" << endl;
                system("pause"); // ожидание ввода перед очисткой экрана
                system("cls"); // очистка экрана
                break;
            }

            case 9:
            {
                Directory directory;
                cout << "Directories matching the mask:" << endl;
                /* directory.MaskDirectory("C:\\Windows\\", "W*");*/
                string dir_path = "C:\\Windows\\Microsoft.NET";
                string dir_mask = "^a.*$";
                directory.MaskDirectory(dir_path, dir_mask);

                system("pause"); // ожидание ввода перед очисткой экрана
                system("cls"); // очистка экрана
                break;
            }

            case 10:
            {
                File file;
                file_name1 = "test_file.txt";
                file.CreateFile(file_name1);
                file_name2 = "test_file1.txt";
                file.CreateFile(file_name2);

                system("pause"); // ожидание ввода перед очисткой экрана
                system("cls"); // очистка экрана
                break;
            }

            case 11:
            {
                File file;
                string file_name = "test_file.txt";
                file.RemoveFile(file_name);

                system("pause"); // ожидание ввода перед очисткой экрана
                system("cls"); // очистка экрана
                break;
            }
            case 12:
            {
                File file;
                old_file_name = "test_file1.txt";
                new_file_name = "test_file2.txt";
                file.RenameFile(old_file_name, new_file_name);

                system("pause"); // ожидание ввода перед очисткой экрана
                system("cls"); // очистка экрана
                break;
            }

            case 13:
            {
                File file;
                source_file = "test_file2.txt";
                dest_file = "test_file1.txt";
                file.CopyFile(source_file, dest_file);

                system("pause"); // ожидание ввода перед очисткой экрана
                system("cls"); // очистка экрана
                break;
            }

            case 14:
            {
                File file;
                old_file_path = "C:\\Users\\user\\source\\repos\\Project8-FileManager\\test_file1.txt";
                new_file_path = "E:\\test_file1.txt";
                file.MoveFile(old_file_path, new_file_path);

                system("pause"); // ожидание ввода перед очисткой экрана
                system("cls"); // очистка экрана
                break;
            }

            case 15:
            {
                File file;
                file.ReadFile("test_file2.txt");

                system("pause"); // ожидание ввода перед очисткой экрана
                system("cls"); // очистка экрана
                break;
            }

            case 16:
            {
                File file;
                file_name = "test_file2.txt";
                file_size = file.SizeFile(file_name);
                cout << "File size of " << file_name << " is: " << file_size << " bytes." << endl;

                system("pause"); // ожидание ввода перед очисткой экрана
                system("cls"); // очистка экрана
                break;
            }

            case 17:
            {
                File file;
                file_path = "F:\\S.O.F.T";
                file_mask = "v.*";
                file.MaskFile(file_path, file_mask);


                system("pause"); // ожидание ввода перед очисткой экрана
                system("cls"); // очистка экрана
                break;
            }

            case 18:
            {
                cout << "Exiting program..." << endl;
                exit(0);
            }

            default:
            {
                cout << "Invalid choice, please try again." << endl;
                break;
            }
            }
        }
    }
};

int main()
{
    Start start;
    start.StartPanel();

    return 0;
}







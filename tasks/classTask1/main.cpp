/**
 * @file main.cpp
 * @brief задача написать программу, которая будет обслуживать текстовую key-value базу данных в
 * файле db.txt
 *
 * Должен компилироваться бинарник db, и откликаться на команды (команды подаются как аргументы при
 * запуске приложения *argv) `INSERT KEY=VALUE` - операция вставки пары в базу данных (если
 * добавляется ключ который уже есть в файле, то он обновляется) `DELETE KEY` - удалить пару из базы
 * данных `FIND KEY` - вывести значение по ключу в базе данных (возврашает сообщение с элементов в
 * консоль `FOUND: VALUE`, или выдаёт на вывод `NOT FOUND`, если элемента нет в базе данных) `PRINT
 * KEYS/VALUES` - вывести значение всех ключей/значений в базе данных `DROPALL` - удалить всю базу
 * данных (все значения из базы) Любая команда должна заканчиваться на '?' , можно обрабатывать
 * сразу несколько команд
 *
 * KEY и VALUE - гарантированно не пустые строки (используют символы букв и цифр, не используют
 * знаки) Как вы будете хранить в файле данные - на ваше усмотрение. Главное - чтобы
 * пользовательское API не нарушалось. Главное - база данных храниться в файле, но не в памяти и
 * программу запускаем по необходимости. Программа может быть неоптимальна с точки зрения
 * вычислений, главное чтобы правильно выполнялась основная логика команд.
 */

#include <algorithm>
#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

namespace
{
    constexpr char end_command_delimiter{'?'};
    constexpr char betwean_arguments_delimiter{' '};
    constexpr char betwean_records_delimiter{'\n'};
    constexpr char betwean_data_delimiter{' '};

}; // namespace

auto Split(std::string &str, char delimiter) -> std::vector<std::string>
{
    std::vector<std::string> result;
    std::string buff;
    for (auto symbool : str)
    {
        if (symbool != delimiter)
        {
            buff.push_back(symbool);
        }
        else
        {
            result.push_back(buff);
            buff.clear();
        }
    }
    if (!buff.empty())
        result.push_back(buff);
    return result;
}
auto Join(std::vector<std::string> &tokens, char delim) -> std::string
{
    std::string result;
    auto it = tokens.begin();
    for (; it < tokens.end() - 1; it++)
    {
        result.append(*it);
        result.push_back(delim);
    }
    result.append(*it);
    return result;
}

class FileReader
{
    std::ifstream mFile;

public:
    FileReader(std::string fileName) { mFile.open(fileName.c_str()); }
    bool readNext(std::string &content)
    {
        content.clear();
        std::getline(mFile, content);
        return (!content.empty()); // FIXME: Another
    }
    ~FileReader() { mFile.close(); }
};

class FileWriter
{

private:
    std::ofstream mFile;

public:
    FileWriter(std::string fileName) { mFile.open(fileName.c_str()); }

    template <typename T> void write(T data) { mFile << data; }

    ~FileWriter() { mFile.close(); }
};

class KeyValueDB
{
private:
    std::string _fileName;

protected:
    std::map<std::string, std::string> _data;

public:
    KeyValueDB(std::string fileName) : _fileName(fileName)
    {
        std::string record;
        FileReader fReader(fileName);
        while (fReader.readNext(record))
        {
            auto elements = Split(record, betwean_data_delimiter);
            auto id = elements.front();
            elements.erase(elements.begin());
            auto value = Join(elements, betwean_data_delimiter);
            _data[id] = value;
        }
    };

    ~KeyValueDB()
    {
        FileWriter fWriter(_fileName);
        for (const auto &pair : _data)
        {
            std::string line;
            line = pair.first + betwean_data_delimiter + pair.second + '\n';
            fWriter.write(line);
        }
    }
};

class KeyValueManager : public KeyValueDB
{
public:
    KeyValueManager(std::string fileName) : KeyValueDB(fileName){};

    void Insert(std::string id, std::string value) { _data[id] = value; }
    bool Delete(std::string id)
    {
        // Удаление элемента по ключу
        auto it = _data.find(id); // Находим итератор элемента
        if (it != _data.end())
        {
            _data.erase(it); // Удаляем элемент по найденному итератору
            return true;
        }
        else
            return false;
    }
    std::string Find(std::string id)
    {
        auto it = _data.find(id); // Ищем ключ в map
        if (it != _data.end())
        {
            return (it->second); // Возвращаем значение, если ключ найден
        }
        else
        {
            return ""; // Возвращаем сообщение, если ключ не найден
        }
    }
    // PRINT
    void GetAllKeys(std::vector<std::string> &result)
    {
        for (const auto &pair : _data)
        {
            result.push_back(pair.first);
        }
    }
    void GetAllValues(std::vector<std::string> &result)
    {
        for (const auto &pair : _data)
        {
            result.push_back(pair.second);
        }
    }
    void Dropall() { _data.clear(); }
};

class Command
{
private:
    std::string _name;
    std::vector<std::string> _arguments;

public:
    Command(std::string name, std::vector<std::string> args) : _name(name), _arguments(args) {}
    Command(std::string str)
    {
        auto elements = Split(str, betwean_arguments_delimiter);
        _name = elements.front();

        for (int i = 1; i < elements.size(); i++)
        {
            _arguments.push_back(elements[i]);
        }
    }

    std::string getName() { return _name; }
    std::vector<std::string> getArguments() { return _arguments; }
};

class ArgumentsParser
{
    std::queue<Command> _comands;

public:
    ArgumentsParser(int argc, char **argv)
    {
        std::string buff;
        for (int i = 1; i < argc; i++)
        {
            buff.append(argv[i]);
            if (buff.back() == end_command_delimiter)
            {
                buff.pop_back();
                _comands.push(Command(buff));
                buff.clear();
            }
            else
                buff.push_back(betwean_arguments_delimiter);
        }
    }

    bool ProcessNextCommand(KeyValueManager *db)
    {
        if (_comands.empty())
            return false;
        else
        {
            Command currentCommand = _comands.front();

            if (currentCommand.getName() == "INSERT")
            {
                auto args = currentCommand.getArguments();
                auto id = Split(args.front(), '=').front();
                std::string buff = Split(args.front(), '=').back();
                for (size_t i = 1; i < args.size(); i++)
                {
                    buff.push_back(betwean_arguments_delimiter);
                    buff.append(args[i]);
                }
                db->Insert(id, buff);
            }
            else if (currentCommand.getName() == "DELETE")
            {
                for (auto arg : currentCommand.getArguments())
                {
                    db->Delete(arg);
                }
            }
            else if (currentCommand.getName() == "FIND")
            {
                for (auto arg : currentCommand.getArguments())
                {
                    auto value = db->Find(arg);
                    if (!value.empty())
                        std::cout << "FOUND: " << value << std::endl;
                    else
                        std::cout << "NOT FOUND" << std::endl;
                }
            }
            else if (currentCommand.getName() == "PRINT")
            {
                auto args = currentCommand.getArguments();
                for (auto arg : args)
                {
                    std::vector<std::string> result;
                    if (arg == "KEYS")
                        db->GetAllKeys(result);
                    else if (arg == "VALUES")
                        db->GetAllValues(result);
                    else
                        std::cerr << "Unknown argument in command : " << currentCommand.getName()
                                  << " :" << arg;
                    for (auto line : result)
                    {
                        std::cout << line << std::endl;
                    }
                }
            }
            else if (currentCommand.getName() == "DROPALL")
            {
                db->Dropall();
            }
            else
                std::cerr << "Unknown command: " << currentCommand.getName() << std::endl;
            _comands.pop();
            return true;
        }
    }
};

int main(int argc, char **argv)
{
    KeyValueManager db("db.txt"); // = KeyValueManager("db.txt");
    ArgumentsParser ap = ArgumentsParser(argc, argv);
    // // return false if no more commands
    while (ap.ProcessNextCommand(&db))
    {
    }

    return 0;
}
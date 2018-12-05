#include "CmdlineParser.h"
#include <iostream>

CmdlineParser* CmdlineParser::m_instance = nullptr;

/**
 * @brief CmdlineParser::CmdlineParser
 * @param argc Количество параметров командной строки
 * @param argv Массив параметров командной строки
 */
CmdlineParser::CmdlineParser(int argc, char** argv)
{
    m_instance = this;

    m_argc = static_cast<size_t>(argc);
    m_argv.resize(m_argc);
    for (size_t i=0 ; i<m_argc ; i++)
        m_argv[i] = std::string(argv[i]);
}

CmdlineParser::~CmdlineParser()
{
    //dtor
}

/**
 * @brief Добавление строкового параметра
 * @param name Имя параметра (его идентификатор)
 * @param val Указатель на значение параметра
 * @param val_def Значение по умолчанию
 * @param desc Описание параметра
 */
void CmdlineParser::add_string_param(const std::string &name, std::string* val, const std::string &val_def, const std::string &desc)
{
    *val = val_def;
    param_rec_t rec;
    rec.type = EParamType::STRING;
    rec.name = name;
    rec.desc = desc;
    rec.val_str = val;
    rec.val_bool = nullptr;
    rec.val_int = nullptr;
    m_instance->m_params.push_back(rec);
}

/**
 * @brief Добавление булевого параметра
 * @param name Имя параметра (его идентификатор)
 * @param val Указатель на значение параметра
 * @param val_def Значение по умолчанию
 * @param desc Описание параметра
 */
void CmdlineParser::add_bool_param(const std::string &name, bool* val, const bool &val_def, const std::string &desc)
{
    *val = val_def;
    param_rec_t rec;
    rec.type = EParamType::BOOLEAN;
    rec.name = name;
    rec.desc = desc;
    rec.val_str = nullptr;
    rec.val_bool = val;
    rec.val_int = nullptr;
    m_instance->m_params.push_back(rec);
}

/**
 * @brief Добавление целочисленного параметра
 * @param name Имя параметра (его идентификатор)
 * @param val Указатель на значение параметра
 * @param val_def Значение по умолчанию
 * @param desc Описание параметра
 */
void CmdlineParser::add_int_param(const std::string &name, int* val, const int &val_def, const std::string &desc)
{
    *val = val_def;
    param_rec_t rec;
    rec.type = EParamType::INT;
    rec.name = name;
    rec.desc = desc;
    rec.val_str = nullptr;
    rec.val_bool = nullptr;
    rec.val_int = val;
    m_instance->m_params.push_back(rec);
}

/**
 * @brief Вывести описание параметров
 */
void CmdlineParser::show_desc()
{
    std::cout << "Usage:" << std::endl << "\t" <<m_instance->m_name << " ";
    for (param_rec_t &param : m_instance->m_params)
    {
        switch (param.type)
        {
        case EParamType::STRING:
            std::cout << "--" << param.name << " [val str](" << *param.val_str << ") ";
            break;
        case EParamType::BOOLEAN:
            std::cout << "--" << param.name << " ";
            break;
        case EParamType::INT:
            std::cout << "--" << param.name << " [val int](" << *param.val_int << ") ";
            break;
        }
    }

    std::cout << "\n\nParameters description:" << std::endl;
    for (param_rec_t &param : m_instance->m_params)
        std::cout << "\t--" << param.name << " - " << param.desc << std::endl;
}

/**
 * @brief Разбор аргументов командной строки
 */
void CmdlineParser::parse()
{
    for (param_rec_t &param : m_instance->m_params)
    {
        std::string s_name = "--" + param.name;
        for (size_t i=0 ; i<m_instance->m_argc ; i++)
            if (s_name.compare(m_instance->m_argv[i]) == 0)
                switch (param.type)
                {
                case EParamType::STRING:
                    *param.val_str = m_instance->m_argv[i + 1];
                    break;
                case EParamType::BOOLEAN:
                    *param.val_bool = true;
                    break;
                case EParamType::INT:
                    *param.val_int = std::stoi(m_instance->m_argv[i + 1]);
                    break;
                }
    }
}

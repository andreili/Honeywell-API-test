#include "hsc_include.h"
#include "log.h"
#include "hscpoints.h"
#include "hscpoint.h"
#include "hscpointparam.h"
#include <iostream>
#include <CmdlineParser.h>
#include <fstream>
#include "gbdata.h"
#include "hscserver.h"


/*
int __hsc_param_list_local(PNTNUM point, PNTTYP type, int lim, enumlist **list)
{
    *list = nullptr;
    enumlist *dict = nullptr;
    enumlist *dict_sub = nullptr;
    char src[255];
    int dict_no = 32;
    if (point == 0)
    {
        DICTitem *ditem = hsc_dict_find_by_number(nullptr, &dict_no);
        if (type)
        {
            if (ditem != nullptr)
            {
                int ret = hsc_dict_list(ditem, HSC_ENTITY, &dict);
                if (ret >= 0)
                {
                    *list = dict;
                    return ret;
                }
            }
        }
        else
        {
            if (ditem)
            {
                dict_sub = reinterpret_cast<enumlist*>(calloc(1001, 8));
                if (dict_sub != nullptr)
                {
                    int max_sz = 1000;
                    int items = 0;
                    hsc_dict_walk(ditem, src, hsc_dict_list_unique_last, (void*)dict_sub, (void*)&max_sz, (void*)&items);
                    *list = dict_sub;
                    return items;
                }
            }
        }

        free(dict_sub);
        hsc_enumlist_destroy(list);
        return -1;
    }

    STApoint *pnt_data = __hsc_point_access(point);
    if (pnt_data == nullptr)
    {
        hsc_enumlist_destroy(list);
        return -1;
    }

    DICTitem *ditem = hsc_dict_find_by_number(nullptr, &dict_no);
    if (ditem == nullptr)
    {
        hsc_enumlist_destroy(list);
        return -1;
    }

    int ret = hsc_dict_list(ditem, HSC_ENTITY, &dict);
    if (ret < 0)
    {
        free(dict_sub);
        hsc_enumlist_destroy(list);
        return -1;
    }

    if (pnt_data->subtype)
    {
        dict_no = pnt_data->subtype;

        DICTitem *dsitem = hsc_dict_find_by_number(ditem, &dict_no);
        if (dsitem)
        {
            int ret_sub = hsc_dict_list(dsitem, HSC_ENTITY, &dict_sub);
            if (ret_sub >= 0)
            {
                int res = hsc_param_list_merge(lim, 0, dict, ret, dict_sub, ret_sub, list);
                free(dict_sub);
                if (res >= 0)
                {
                    free(dict);
                    dict = *list;
                    ret = res;
                }
            }
        }
    }

    int params_count = (pnt_data->type != ANA) ? 0 : 4;
    PNTparam *param = hsc_param_find_first(pnt_data);
    while (param != nullptr)
    {
        param = hsc_param_find_next(pnt_data, &param, param, param->number);
        ++params_count;
    }

    dict_sub = reinterpret_cast<enumlist*>(calloc(params_count + 1, 8));
    if (dict_sub == nullptr)
    {
        free(dict);
        free(dict_sub);
        hsc_enumlist_destroy(list);
        return -1;
    }

    int dict_idx = 0;
    if (pnt_data->type == ANA)
    {
        ANApoint *ana_pnt = reinterpret_cast<ANApoint*>(pnt_data);
        for (int i=0 ; i<4 ; ++i)
        {
            __intchr(src, 9, (char*)ana_pnt->des.eudes[i], 8);  //TODO
            ftocstr(src, 8, src, 9);
            if (src[0] != ' ')
            {
                dict_sub[dict_idx].value = 5 + i;
                dict_sub[dict_idx++].text = strdup(src);
            }
        }
    }

    for (param = hsc_param_find_first(pnt_data) ; param != nullptr ; param = hsc_param_find_next(pnt_data, &param, param, param->number))
    {
        dict_sub[dict_idx].value = param->number;
        dict_sub[dict_idx++].text = strdup(param->name[0].string);
    }

    int res = hsc_param_list_merge(lim, 0, dict, ret, dict_sub, dict_idx, list);
    free(dict);
    free(dict_sub);
    if (res < 0)
    {
        hsc_enumlist_destroy(list);
        return -1;
    }
    return res;
}

int __hsc_param_list_create_limit(PNTNUM point, int limit, enumlist** list)
{
    *list = nullptr;
    enumlist *mem;
    int res = __hsc_param_list_local(point, 0, limit, &mem);
    if (point == 0)
    {
        *list = mem;
        return res;
    }

    STApoint *pnt_data = __hsc_point_access(point);
    if (pnt_data == nullptr)
    {
        *list = mem;
        return res;
    }

    if (pnt_data->type <= CDA)
    {
        *list = mem;
        return res;
    }

    enumlist *tmp;
    int gda_res = hsc_gda_param_list(point, &tmp);
    if (gda_res <= 0)
    {
        if (tmp)
            hsc_enumlist_destroy(&tmp);
        *list = mem;
        return res;
    }

    int ret = hsc_param_list_merge(limit, 1, mem, res, tmp, gda_res, list);
    free(mem);
    free(tmp);
    return ret;
}

int __hsc_param_list_filter(enumlist *list_src, int par, enumlist **list_dst)
{
    //
}

int __hsc_param_list_create(PNTNUM point, enumlist **list)
{
    enumlist *mem;
    int res = __hsc_param_list_create_limit(point, -1, &mem);
    if (res == -1)
        return -1;

    res = __hsc_param_list_filter(mem, res, list);
    if (res == -1)
    {
        hsc_enumlist_destroy(&mem);
        return -1;
    }
    else
    {
        free(mem);
        return res;
    }
}*/

bool dump_point;

void print_point_info(int level, HSCPoint *point)
{
    for (int i=0 ; i<level ; ++i)
        putchar('\t');
    std::cout << "Point #" << point->get_number() << " has name '" << point->get_name() << "' and type " << point->get_type() << std::endl;

    if (dump_point)
        point->dump_to_file(point->get_name() + ".dat");
}

#include "src/GBsysflg.h"

void print_point_param_info(HSCPointParam *param)
{
    if (param->get_type() == EPrmType::NONE)
        return;

    std::cout << "\t" << param->get_name() << "[" << param->get_index() << "]" << std::endl;

    if (param->is_historisable())
        std::cout << "HISTORY!!!" << std::endl;

    if (dump_point)
    {
        if (param->get_data())
        {
            param->dump_to_file(param->get_parent()->get_name() + "_" + param->get_name() + ".dat");
            param->dump_value_to_file(param->get_parent()->get_name() + "_" + param->get_name() + "_gValue.dat");
        }
        else
        {
            PNTNUM pnt = param->get_parent()->get_number();
            PRMNUM prm = param->get_index();
            int offset = 0;
            PARvalue val;
            uint2 type;
            GDAERR err;
            hsc_param_values_get(1, &pnt, &prm, &offset, &val, &type, &err);

            std::string fn = param->get_parent()->get_name() + "_" + param->get_name() + "_val_type_" + std::to_string(type) + ".dat";
            std::ofstream file(fn, std::ios::out | std::ios::binary | std::ios::app);
            file.write((char*)&val, sizeof(PARvalue));
            file.close();
        }
    }
}

void dump_data(char* data, int size, std::string fn)
{
    std::ofstream file(fn, std::ios::out | std::ios::binary | std::ios::app);
    file.write(data, size);
    file.close();
}

GBData *gb_data;

int main(int argc, char **argv)
{
    std::string point_name;
    int point_number;
    int log_level;
    bool show_tree, show_params, help, scan_all;

    CmdlineParser *cmd = new CmdlineParser(argc, argv);
    cmd->set_name("htest.exe");
    cmd->add_bool_param("help", &help, false, "Show this help");
    cmd->add_string_param("pname", &point_name, "2AC001M01", "Point name");
    cmd->add_int_param("pnum", &point_number, 0, "Point number");
    cmd->add_int_param("log", &log_level, 4, "Log level (4 - none, 3 - error, 2 - warning, 1 - info, 0 - verbose");
    cmd->add_bool_param("tree", &show_tree, false, "Show points tree, root - specified point");
    cmd->add_bool_param("params", &show_params, false, "Show parameters for a specified point");
    cmd->add_bool_param("dump", &dump_point, false, "Dump a specified point to file");
    cmd->add_bool_param("scan_all", &scan_all, false, "Scan all points and print acessible names");
    cmd->parse();

    if (help)
    {
        cmd->show_desc();
        return 0;
    }

    Log::set_level(static_cast<Log::ELogLevel>(log_level));

    HSCServer server;
    gb_data = new GBData();
    std::cout << "Initialization: ";
    if (!gb_data->load())
    {
        std::cout << "Failed!" << std::endl;
        return 0;
    }
    std::cout << "OK!" << std::endl;

    /*std::cout << "Initialization: ";
    if (c_gbload() != 0)
    {
        std::cout << "Failed!" << std::endl;
        return 1;
    }
    std::cout << "OK!" << std::endl;*/

    new HSCPoints();

    if (!HSCPoints::init())
        return 1;

    if (scan_all)
    {
        int count = HSCPoints::scan_all_points();
        std::cout << "Finded " << count << " points" << std::endl;
        for (int i=0 ; i<HSCPoints::get_points_count() ; ++i)
        {
            HSCPoint *point = HSCPoints::get_point_by_idx(i);
            std::cout << "pnt name: '" << point->get_name() << "';" << std::endl;
            //std::cout << "\t" << point->get_name() << " type #" << point->get_type() << " subtype " << point->get_subtype_name() << std::endl;
        }
    }

    HSCPoint *point = nullptr;

    if ((point_number == 0) && (point_name.size() == 0))
    {
        std::cout << "No specified points, exiting!" << std::endl;
        return 0;
    }

    if (point_number != 0)
        point = HSCPoints::get_point(point_number);
    else
        point = HSCPoints::get_point(point_name);

    if (point->get_number() == 0)
    {
        std::cout << "Invalid point, exiting!" << std::endl;
        return 0;
    }

    if (dump_point)
        point->dump_to_file(point->get_name() + ".dat");

    if (show_tree)
        point->recursive(0, print_point_info);

    if (show_params)
        point->params_proc(print_point_param_info);

    if (point != nullptr)
        delete point;

    return 0;
}

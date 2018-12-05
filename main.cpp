#include "hsc_include.h"
#include "log.h"
#include "hscpoints.h"
#include "hscpoint.h"
#include "hscpointparam.h"
#include <iostream>
#include <CmdlineParser.h>

#include <fstream>


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
    if (param->get_type() == 65535)
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

#include "src/GBcrttbl.h"
#include "src/GBpbttbl.h"
#include "src/GBgdatbl.h"
#include "src/GBtrbtbl.h"
#include "src/GBaastbl.h"
#include "src/GBsysflg.h"
#include "src/GBdiotbl.h"
#include "src/GBwdttbl.h"
//#include "src/GBtimvec.h"
#include "src/GBlogtbl.h"
#include "src/GBperlog.h"
#include "src/GBscnpri.h"
#include "src/GBscntbl.h"
#include "src/GBlfntbl.h"
#include "src/GBprtque.h"
#include "src/GBscnfmt.h"
#include "src/GBchntbl.h"
#include "src/GBrtutbl.h"
//#include "src/GBhatfst.h" -- in GBhatslw.h
#include "src/GBhatslw.h"
//#include "src/GBhatext.h" -- in GBhatslw.h
//#include "src/GBhatexc.h" -- in GBhatslw.h
#include "src/GBseclvl.h"
#include "src/GBsoekys.h"
#include "src/GBevtkys.h"
//#include "src/GBatttbl.h"
//#include "src/GBcattbl.h"
//#include "src/GBdsntbl.h"
//#include "src/GBdsrtbl.h"
#include "src/GBwndtbl.h"
#include "src/GBsyssts.h"
#include "src/GBschedl.h"
#include "src/GBdevtbl.h"
#include "src/GBscript.h"
#include "src/GBcontbl.h"
#include "src/GBalmshv.h"

#include "src/GBsys90.h"
#include "src/GBbase.h"

int main(int argc, char **argv)
{
    std::string point_name;
    int point_number;
    int log_level;
    bool show_tree, show_params, help;

    CmdlineParser *cmd = new CmdlineParser(argc, argv);
    cmd->set_name("htest.exe");
    cmd->add_bool_param("help", &help, false, "Show this help");
    cmd->add_string_param("pname", &point_name, "2AC001M01", "Point name");
    cmd->add_int_param("pnum", &point_number, 0, "Point number");
    cmd->add_int_param("log", &log_level, 4, "Log level (4 - none, 3 - error, 2 - warning, 1 - info, 0 - verbose");
    cmd->add_bool_param("tree", &show_tree, false, "Show points tree, root - specified point");
    cmd->add_bool_param("params", &show_params, false, "Show parameters for a specified point");
    cmd->add_bool_param("dump", &dump_point, false, "Dump a specified point to file");
    cmd->parse();

    if (help)
    {
        cmd->show_desc();
        return 0;
    }

    Log::set_level(static_cast<Log::ELogLevel>(log_level));

    printf("Initialization: ");
    if (c_gbload() != 0)
    {
        printf("Failed!\n");
        return 1;
    }
    printf("OK!\n");

    /*dump_data((char*)GBsys90, 1 * sizeof(sys90), "GBsys90.dat");

#include "gbattach_nt1.h"
    dump_data((char*)GBdirtry, NUM_FILES * sizeof(dirtry), "GBdirtry.dat");
    dump_data((char*)GBcrttbl, db_size[1], "GBcrttbl.dat");
    dump_data((char*)GBpbttbl, db_size[2], "GBpbttbl.dat");
    dump_data((char*)GBgdatbl, db_size[3], "GBgdatbl.dat");
    dump_data((char*)GBsysflg, db_size[4], "GBsysflg.dat");
    dump_data((char*)GBtrbtbl, db_size[5], "GBtrbtbl.dat");
    dump_data((char*)GBaastbl, db_size[6], "GBaastbl.dat");
    dump_data((char*)GBdiotbl, db_size[7], "GBdiotbl.dat");
    dump_data((char*)GBwdttbl, db_size[8], "GBwdttbl.dat");
    dump_data((char*)GBscnpri, db_size[9], "GBscnpri.dat");
    dump_data((char*)GBlogtbl, db_size[10], "GBlogtbl.dat");
    dump_data((char*)GBperlog, db_size[11], "GBperlog.dat");
    dump_data((char*)GBprtque, db_size[12], "GBprtque.dat");
    dump_data((char*)GBscntbl, db_size[13], "GBscntbl.dat");
    dump_data((char*)GBlfntbl, db_size[14], "GBlfntbl.dat");
    dump_data((char*)GBrtutbl, db_size[15], "GBrtutbl.dat");
    dump_data((char*)GBscnfmt, db_size[16], "GBscnfmt.dat");
    dump_data((char*)GBchntbl, db_size[17], "GBchntbl.dat");
    dump_data((char*)GBhatext, db_size[18], "GBhatext.dat");
    dump_data((char*)GBhatfst, db_size[19], "GBhatfst.dat");
    dump_data((char*)GBhatslw, db_size[20], "GBhatslw.dat");
    dump_data((char*)GBsoekys, db_size[21], "GBsoekys.dat");
    dump_data((char*)GBhatexc, db_size[22], "GBhatexc.dat");
    dump_data((char*)GBseclvl, db_size[23], "GBseclvl.dat");
    //dump_data((char*)GBcattbl, db_size[24], "GBcattbl.dat");
    dump_data((char*)GBevtkys, db_size[25], "GBevtkys.dat");
    //dump_data((char*)GBatttbl, db_size[26], "GBatttbl.dat");
    dump_data((char*)GBwndtbl, db_size[27], "GBwndtbl.dat");
    //dump_data((char*)GBdsntbl, db_size[28], "GBdsntbl.dat");
    //dump_data((char*)GBdsrtbl, db_size[29], "GBdsrtbl.dat");
    dump_data((char*)GBdevtbl, db_size[30], "GBdevtbl.dat");
    dump_data((char*)GBsyssts, db_size[31], "GBsyssts.dat");
    dump_data((char*)GBschedl, db_size[32], "GBschedl.dat");
    dump_data((char*)GBalmshv, db_size[33], "GBalmshv.dat");
    dump_data((char*)GBscript, db_size[34], "GBscript.dat");
    dump_data((char*)GBcontbl, db_size[35], "GBcontbl.dat");

    return 0;*/

    Log::set_level(Log::LOG_NONE);
    new HSCPoints();

    if (!HSCPoints::init())
        return 1;

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

    /*for (int i=1 ; i<argc ; ++i)
    {
        if (strcmp(argv[i], "--tree") == 0)
        {
            if (point == nullptr)
                for (int32_t i=1 ; i<1000*1000 ; ++i)
                    HSCPoints::get_point(i)->recursive(false, 0, print_point_info, print_point_param_info);
            else
                point->recursive(false, 0, print_point_info, print_point_param_info);
            return 0;
        }
        else if ((strcmp(argv[i], "--params") == 0) && (point != nullptr))
            point->recursive(true, 0, print_point_info, print_point_param_info);
        else if ((strcmp(argv[i], "--dump") == 0) && (point != nullptr))
        {
            printf("Dump...\n");
            point->dump_to_file(std::string(pnt_name) + ".dat");
        }
        else if (strcmp(argv[i], "--verbose") == 0)
            Log::set_level(Log::LOG_VERBOSE);
        else if (strcmp(argv[i], "--info") == 0)
            Log::set_level(Log::LOG_INFO);
        else if (strcmp(argv[i], "--warning") == 0)
            Log::set_level(Log::LOG_WARNING);
        else if (strcmp(argv[i], "--error") == 0)
            Log::set_level(Log::LOG_ERROR);
        else
        {
            pnt_name = argv[i];
            if (point != nullptr)
                delete point;
            point = HSCPoints::get_point(pnt_name);
            if (point->get_number() == 0)
            {
                printf("Unable to find point '%s'!\n", pnt_name);
                delete point;
                return 0;
            }
        }
    }*/

    return 0;
}

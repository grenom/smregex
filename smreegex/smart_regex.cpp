#include "smart.h"


using namespace std;
string version("2.09");
#ifdef WIN32
string arch("x86");
#elif WIN64
string arch("x64");
#endif // WIN32



int main(int argc, char* argv[])
{
	using namespace std;
	int opt, id_param; //= -1;
	string disk_path, smart_expath, name_param, name_option, smart_devtype, b_discovery, param_temp;
	//boost::filesystem::path full_path_smartctl(boost::filesystem::current_path());

	namespace po = boost::program_options;
	po::options_description desc((string)"This prog write VDV (vdv@oduv.so-ups.ru). Version: "+ version + "(" + arch + "). Date compile: " + (string)__DATE__);
	desc.add_options()
		("help,h", "this help")
		("id,i", po::value<int>(&id_param)->default_value(-1), "ID attrib")
		("name,n", po::value<string>(&name_param),"name attrib")
		("smartpath,s", po::value<string>(&smart_expath),"path to smartctl")
		("smartdevtype,t", po::value<string>(&smart_devtype), "smartctl option -d (can skip, use after ZABBIX autodiscovery)")
		("dev,d", po::value<string>(&disk_path), "path to dev")
		("option,o", po::value<string>(&name_option), "get option info: model, sn, rotrate, family, m2temp, m2availspare, m2used, m2r, m2w, m2powern, m2powerh")
		("discovery,c", po::value<string>(&b_discovery), "use for ZABBIX autodiscovery. type disks - all,nvme,ata,ata+hdd,ata+ssd,...")
		("temp", po::value<string>(&param_temp), "don't influence anything");
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);
#if _DEBUG
	cout << "DEBUG. Param:" << endl;
	cout << "id - " << id_param << endl;
	cout << "name - " << name_param << endl;
	cout << "smartpath - " << smart_expath << endl;
	cout << "dev - " << disk_path << endl;
	cout << "option - " << name_option << endl;
#endif

	if (vm.count("help"))
	{
		cout << desc << endl;
		return 0;
	}

	if (smart_expath.empty())
	{
		cout << "Path is empty to smartctl!" << endl;
		return 0;
	}


	int count_r = 0;
	if ((id_param != -1 || !name_param.empty()) && !disk_path.empty() && name_option.empty() && b_discovery.empty())//ищем в smart талице, параметры -i -n
	{

		string smartstr = get_smart_str(smart_expath, disk_path, smart_devtype);
		string out_val;
		if (search_smart_id_value(smartstr, id_param, name_param, &out_val))
		{
			cout << out_val;
		}
		else
		{
			cout << -1;
		}

		
	}
	else if ((id_param == -1 && name_param.empty()) && !disk_path.empty() && !name_option.empty() && b_discovery.empty() \
		&& (name_option == "family" || name_option == "model" || name_option == "sn" || name_option == "rotrate" || name_option == "m2temp" || name_option == "m2temp"\
			|| name_option == "m2availspare" || name_option == "m2used" || name_option == "m2r" || name_option == "m2w" || name_option == "m2powern" || name_option == "m2powerh"))
	{
		string smartstr=get_smart_str(smart_expath, disk_path, smart_devtype);

		string::const_iterator smartStart = smartstr.begin();
		string::const_iterator smartEnd = smartstr.end();
		boost::smatch xResults;
		boost::regex r_regex_optionalsearch;

		if (name_option == "family")
		{
			r_regex_optionalsearch = "Model Family:\\s+(.+?)\\n";
			if (boost::regex_search(smartStart, smartEnd, xResults, r_regex_optionalsearch))
			{
				cout << xResults[1];
				count_r++;
			}
		}
		else if (name_option == "model")
		{
			r_regex_optionalsearch="Device Model:\\s+(.+?)\\n";//first variant
			if (boost::regex_search(smartStart, smartEnd, xResults, r_regex_optionalsearch))
			{
				cout << xResults[1];
				count_r++;
			}
			else
			{
				xResults.empty();
				r_regex_optionalsearch = "Model Number:\\s+(.+?)\\n";//second variant
				if (boost::regex_search(smartStart, smartEnd, xResults, r_regex_optionalsearch))
				{
					cout << xResults[1];
					count_r++;
				}
			}
		}
		else if (name_option == "sn")
		{
			r_regex_optionalsearch = "Serial Number:\\s+(.+?)\\n";
			if (boost::regex_search(smartStart, smartEnd, xResults, r_regex_optionalsearch))
			{
				cout << xResults[1];
				count_r++;
			}
		}
		else if (name_option == "rotrate")
		{
			r_regex_optionalsearch = "Rotation Rate:\\s+(\\d+).+?\\n";
			if (boost::regex_search(smartStart, smartEnd, xResults, r_regex_optionalsearch))
			{
				cout << xResults[1];
				count_r++;
			}			
		}
		else if (name_option == "m2temp")
		{
			r_regex_optionalsearch = "Temperature:\\s+(\\d+).+?\\n";
			if (boost::regex_search(smartStart, smartEnd, xResults, r_regex_optionalsearch))
			{
				cout << xResults[1];
				count_r++;
			}
		}
		else if (name_option == "m2availspare")
		{
			r_regex_optionalsearch = "Available Spare:\\s+(\\d+).+?\\n";
			if (boost::regex_search(smartStart, smartEnd, xResults, r_regex_optionalsearch))
			{
				cout << xResults[1];
				count_r++;
			}
		}
		else if (name_option == "m2used")
		{
			r_regex_optionalsearch = "Percentage Used:\\s+(\\d+).+?\\n";
			if (boost::regex_search(smartStart, smartEnd, xResults, r_regex_optionalsearch))
			{
				cout << xResults[1];
				count_r++;
			}
		}
		else if (name_option == "m2r")
		{
			r_regex_optionalsearch = "Data Units Read:\\s+(.+?)\\s+\\[.+?\\].+?\\n";
			if (boost::regex_search(smartStart, smartEnd, xResults, r_regex_optionalsearch))
			{
				string replace_result = m2converter(xResults[1]);
				cout << replace_result;
				count_r++;
			}
		}
		else if (name_option == "m2w")
		{
			//r_regex_optionalsearch = "Data Units Written:\\s+.+?\\[(\\d+,\\d+).+?\\].+?\\n";
			r_regex_optionalsearch = "Data Units Written:\\s+(.+?)\\s+\\[.+?\\].+?\\n";
			if (boost::regex_search(smartStart, smartEnd, xResults, r_regex_optionalsearch))
			{
				//string replace_result = replace_dot_to_comma(xResults[1]);
				string replace_result = m2converter(xResults[1]);
				cout << replace_result;
				count_r++;
			}
		}
		else if (name_option == "m2powern")
		{
			r_regex_optionalsearch = "Power Cycles:\\s+(.+?)\\n";
			if (boost::regex_search(smartStart, smartEnd, xResults, r_regex_optionalsearch))
			{
				string replace_result = m2converter(xResults[1]);
				cout << replace_result;
				count_r++;
			}
		}
		else if (name_option == "m2powerh")
		{
			r_regex_optionalsearch = "Power On Hours:\\s+(.+?)\\n";
			if (boost::regex_search(smartStart, smartEnd, xResults, r_regex_optionalsearch))
			{
				string replace_result = m2converter(xResults[1]);
				cout << replace_result;
				count_r++;
			}
		}
		/*boost::regex r_regex_optionalsearch("Model Family:\\s+(.+?)\\n.*Device Model:\\s+(.+?)\\n.*Serial Number:\\s+(.+?)\\n.*Rotation Rate:\\s+(\\d+).+?\\n");*/
		if (count_r == 0)
		{
			//cout << "not found";
			cout << -1;

		}
	}
	else if ((id_param == -1 && name_param.empty()) && disk_path.empty() && name_option.empty() && !b_discovery.empty())
	{
		string smartstr = get_smart_str_autodiscovery(smart_expath);
		boost::regex r_regex("^([,/\\w]+)\\s+-d\\s+(\\w+).+?\\n");
		string::const_iterator smartStart = smartstr.begin();
		string::const_iterator smartEnd = smartstr.end();
		boost::smatch xResults;

		namespace pt = boost::property_tree;
		pt::ptree root;
		pt::ptree data_node;

		while (boost::regex_search(smartStart, smartEnd, xResults, r_regex))
		{
		pt::ptree diskdata;
			if (b_discovery == "all")
			{
				diskdata.put("{#DEV}", xResults[1]);
				diskdata.put("{#TYPE}", xResults[2]);
				data_node.push_back(make_pair("", diskdata));
				//cout << xResults[1] << ", "<< xResults[2] << endl;
				count_r++;
			}
			else if (b_discovery == "ata+hdd" && xResults[2] == "ata")
			{
				string smartstr_for_ad_hdd = get_smart_str(smart_expath, xResults[1], xResults[2]);
				string::const_iterator ad_hdd_start = smartstr_for_ad_hdd.begin();
				string::const_iterator ad_hdd_end = smartstr_for_ad_hdd.end();
				boost::regex ad_hdd_regex_rotrate(".+?Rotation Rate:\\s+\\d+ rpm\\n.+"); 
				//boost::regex ad_hdd_regex(".+?Rotation Rate:\\s+(\\d+).+?\\n.+");
				if (boost::regex_match(smartstr_for_ad_hdd, ad_hdd_regex_rotrate) || search_smart_id_value(smartstr_for_ad_hdd,3,"",NULL) || search_smart_id_value(smartstr_for_ad_hdd, 7, "", NULL))
				{
					diskdata.put("{#DEV}", xResults[1]);
					diskdata.put("{#TYPE}", xResults[2]);
					data_node.push_back(make_pair("", diskdata));
					count_r++;
				}
			}
			else if (b_discovery == "ata+ssd" && xResults[2] == "ata")
			{
				string smartstr_for_ad_ssd = get_smart_str(smart_expath, xResults[1], xResults[2]);
				string::const_iterator ad_ssd_start = smartstr_for_ad_ssd.begin();
				string::const_iterator ad_ssd_end = smartstr_for_ad_ssd.end();
				boost::regex ad_ssd_regex(".+?Rotation Rate:\\s+Solid State Device\\n.+");
				if (boost::regex_match(smartstr_for_ad_ssd, ad_ssd_regex))
				{
					diskdata.put("{#DEV}", xResults[1]);
					diskdata.put("{#TYPE}", xResults[2]);
					data_node.push_back(make_pair("", diskdata));
					count_r++;
				}
			}
			else 
			{
				if (b_discovery == xResults[2])
				{
					diskdata.put("{#DEV}", xResults[1]);
					diskdata.put("{#TYPE}", xResults[2]);
					data_node.push_back(make_pair("", diskdata));
					count_r++;
				}
			}
			smartStart = xResults[1].second;
		}

		if (count_r != 0)
		{
			root.add_child("data", data_node);
			pt::write_json(cout, root);
		}
		else
		{
			pt::ptree empty;
			std::string emptystr;
			empty.empty();
			empty.clear();
			//empty.put("1","2");
			data_node.push_back(make_pair(emptystr,empty));
			//data_node.push_back(make_pa);
			root.add_child("data", data_node);
			pt::write_json(cout, root);
			//cout << -1;
		}
	}
	else
	{
		cout << "Error parametrs. Using -h" << endl;
	}
#ifdef _DEBUG
	string temp1;
	cin >> temp1;
#endif
	return 0;
}


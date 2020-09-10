#include "smart.h"
#include "mydebug.h"
using namespace std;
string debugfilespath("C:\\Users\\vdv\\Documents\\Visual Studio 2017\\Projects\\smreegex\\smreegex\\smart_tests\\");


#ifdef GPPD
#include "pstream.h"
std::string get_command_str(std::string cmd) {
	using namespace std;
	string data;
	FILE * stream;
	const int max_buffer = 256;
	char buffer[max_buffer];
	cmd.append(" 2>&1");

	stream = popen(cmd.c_str(), "r");
	if (stream)
	{
		while (!feof(stream))
		{
			if (fgets(buffer, max_buffer, stream) != NULL)
			{
				data.append(buffer);
			}
		}
		pclose(stream);
	}
	return data;
}
#elif MINGW
std::string get_command_str(std::string cmd)
{
	using namespace std;
	FILE *in;
	string out = "";
	char buff[512];
	if (!(in = popen(cmd.c_str(), "r")))
	{
		return 0;
	}

	while (fgets(buff, sizeof(buff), in) != NULL)
	{
		out += buff;
	}
	pclose(in);

	return out;
}
#elif WIN64 || WIN32
std::string get_command_str(std::string cmd)
{
	using namespace std;
	FILE *in;
	string out = "";
	char buff[512];
	if (!(in = _popen(cmd.c_str(), "r")))
	{
		return 0;
	}

	while (fgets(buff, sizeof(buff), in) != NULL)
	{
		out += buff;
	}
	_pclose(in);

	return out;
}
#endif

int check_smartflag_str(std::string smart)
{
	boost::regex r_regex("Read Device Identity failed: Input/output error\\n");
	string::const_iterator smartStart = smart.begin();
	string::const_iterator smartEnd = smart.end();
	boost::smatch xResults;

	while (boost::regex_search(smartStart, smartEnd, xResults, r_regex))
	{
		return 0;
	}

	r_regex = "Unable to detect device type";
	smartStart = smart.begin();
	smartEnd = smart.end();
	xResults.empty();
	while (boost::regex_search(smartStart, smartEnd, xResults, r_regex))
	{
		return 0;
	}

	r_regex = "Read Device Identity failed: scsi error unsupported scsi opcode\\n";
	smartStart = smart.begin();
	smartEnd = smart.end();
	xResults.empty();
	while (boost::regex_search(smartStart, smartEnd, xResults, r_regex))
	{
		return 0;
	}


	return 1;
}

/*std::string get_smart_str(std::string smart_expath, std::string disk_path)
{
	string smartstr, strtemp;
	strtemp = smart_expath + " -a " + disk_path;
#ifdef _DEBUG
	smartstr = get_debugstr_fromfile("C:\\Users\\vdv\\Documents\\Visual Studio 2017\\Projects\\smart_regex\\smart_regex\\kiselev_ok_nv2.txt");
	//smartstr = get_debugstr_fromfile("C:\\Users\\vdv\\Documents\\Visual Studio 2017\\Projects\\smart_regex\\smart_regex\\kiselev_fail.txt");
	//smartstr = get_debugstr_fromfile("C:\\Users\\vdv\\Documents\\Visual Studio 2017\\Projects\\smart_regex\\smart_regex\\dsright_fail.txt");
	//smartstr = get_debugstr_fromfile("C:\\Users\\vdv\\Documents\\Visual Studio 2017\\Projects\\smart_regex\\smart_regex\\dsright_ok.txt");
	cout << "1smartstr" << endl << smartstr;
#elif NDEBUG
	smartstr = get_command_str(strtemp);
#endif
	if (check_smartflag_str(smartstr) == 0)
	{
		strtemp = smart_expath + " -d sat -a " + disk_path;
#ifdef _DEBUG
		//smartstr = get_debugstr_fromfile("C:\\Users\\vdv\\Documents\\Visual Studio 2017\\Projects\\smart_regex\\smart_regex\\dsright_fail.txt");
		smartstr = get_debugstr_fromfile("C:\\Users\\vdv\\Documents\\Visual Studio 2017\\Projects\\smart_regex\\smart_regex\\kiselev_fail_sat.txt");
		cout << "2smartstr" << endl << smartstr;
#elif NDEBUG
		smartstr = get_command_str(strtemp);
#endif	
		if (check_smartflag_str(smartstr) == 0)
		{
			strtemp = smart_expath + " -d ata -a " + disk_path;
#ifdef _DEBUG
			//smartstr = get_debugstr_fromfile("C:\\Users\\vdv\\Documents\\Visual Studio 2017\\Projects\\smart_regex\\smart_regex\\dsright_fail.txt");
			smartstr = get_debugstr_fromfile("C:\\Users\\vdv\\Documents\\Visual Studio 2017\\Projects\\smart_regex\\smart_regex\\kiselev_ok_ata.txt");
			cout << "3smartstr" << endl << smartstr;
#elif NDEBUG
			smartstr = get_command_str(strtemp);
#endif
		}
	}
	return smartstr;
}*/

std::string get_smart_str(std::string smart_expath, std::string disk_path, std::string devtype)
{
	string smartstr, strtemp;
	if (devtype.empty())
	{
		strtemp = smart_expath + " -a " + disk_path;
#ifdef _DEBUG
		//smartstr = get_debugstr_fromfile(debugfilespath+"morozov-av-nvme.txt");
		smartstr = get_debugstr_fromfile(debugfilespath + "kog_hdd_sdb.txt");
		cout << "Debug 1smartstr" << endl << smartstr;
#elif NDEBUG
		smartstr = get_command_str(strtemp);
#endif
	}
	else
	{
		strtemp = smart_expath + " -a -d " + devtype + " " + disk_path;
#ifdef _DEBUG
		if (disk_path == "/dev/csmi1,0")
		{
			smartstr = get_debugstr_fromfile(debugfilespath + "csmil1-0.txt");
		}
		else if (disk_path == "/dev/csmi1,1")
		{
			smartstr = get_debugstr_fromfile(debugfilespath + "csmil1-1.txt");
		}
		else
		{
			smartstr = get_debugstr_fromfile(debugfilespath + "kog_hdd_sdb.txt");
			//smartstr = get_debugstr_fromfile(debugfilespath + "vla_ok.txt");
			//smartstr = get_debugstr_fromfile(debugfilespath + "morozov-av-nvme.txt");
		}
		cout << "Debug 2smartstr" << endl << smartstr;
#elif NDEBUG
		smartstr = get_command_str(strtemp);
#endif
	}
	return smartstr;
}


std::string get_smart_str_autodiscovery(std::string smart_expath)
{
	string smartstr, strtemp;
	strtemp = smart_expath + "  --scan";
#ifdef _DEBUG
	//smartstr = get_debugstr_fromfile(debugfilespath+"autodiscover_kiselevda_with_nv2.txt");
	smartstr = get_debugstr_fromfile(debugfilespath + "kog_hdd_autodiscovery.txt");
	cout << "Debug 1autodiscovery" << endl << smartstr;
#elif NDEBUG
	smartstr = get_command_str(strtemp);
#endif
	return smartstr;
}


bool search_smart_id_value(std::string smartstr, int id_smart, std::string name_smart, std::string* out)
{
	bool count_r = false;
	//boost::regex r_regex("(\\d{1,3})\\s+(\\w+)\\s+(\\w+)\\s+(\\d{1,3})\\s+(\\d{1,3})\\s+(\\d{1,3})\\s+([\\w-]+)\\s+(\\w+)\\s+([\\w-]+)\\s+(\\d+).*\\n");
	boost::regex r_regex("^\\s*(\\d{1,3})\\s+([\\w-]+)\\s+[\\w-]+\\s+\\d+\\s+\\d+\\s+[\\d-]+\\s+.+?\\s+.+?\\s+.+?\\s+(\\d+).*?$");
	string::const_iterator smartStart = smartstr.begin();
	string::const_iterator smartEnd = smartstr.end();
	boost::smatch xResults;
	int debug_all_count = 0;
	while (boost::regex_search(smartStart, smartEnd, xResults, r_regex))
	{
		debug_all_count++;
		string tmp_smart_id = xResults[1];
		string tmp_smart_name = xResults[2];
		string tmp_smart_value = xResults[3];
		if ((atoi(tmp_smart_id.c_str()) == id_smart) || (name_smart == tmp_smart_name))
		{
			count_r=true;
			if (out != NULL)
			{
				out->assign(tmp_smart_value);
			}
			break;
			//cout << tmp_smart_value;
		}
		smartStart = xResults[1].second;
	}
	return count_r;
#if _DEBUG
	cout << endl << "DEBUG: All regex seach - " << debug_all_count << endl;
#endif
}



std::string replace_dot_to_comma(std::string str)
{
	using namespace std;
	boost::regex comma_replace(",");
	string replace_result = boost::regex_replace(str, comma_replace, ".");
	return replace_result;
}

std::string m2converter(std::string str)
{
	string out;
	boost::regex r("(\\d+)");
	boost::smatch result;
	string::const_iterator start = str.begin();
	string::const_iterator end = str.end();
	while (boost::regex_search(start,end,result,r))
	{
		out += result[1];
		start = result[1].second;
	}

	if (out.empty())
	{
		out = "-1";
	}
	return out;
}
#include "Input.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include <algorithm>
using namespace std;

static void pre_processing(Instance &in)
{
	for(int i = 0; i < in.n_tests; i++)
	{
		for(int j = 0; j < in.test[i].N; j++)
		{
			if(in.test[i].m[j].P - in.test[i].m[j].R >= (in.test[i].D - in.test[i].m[j].D - 1) * in.test[i].m[j].G)
			{
				in.test[i].m.erase(in.test[i].m.begin()+ j);
				in.test[i].N--;
			}
		}
	}

	for(int i = 0; i < in.n_tests; i++)
	{
		for(map<int, vector<int>>::iterator j = in.test[i].level.begin(); j != in.test[i].level.end(); j++)
		{
			if(j->second.size() > 1)
			{
				for(int k = 0; k < j->second.size() - 1; k++)
				{
					for(int l = k+1; l < j->second.size(); l++)
					{
						if(in.test[i].m[j->second[k]].G == in.test[i].m[j->second[l]].G)
						{
							if(in.test[i].m[j->second[k]].R <= in.test[i].m[j->second[l]].R)
							{
								in.test[i].m.erase(in.test[i].m.begin()+ j->second[k]);
				                in.test[i].N--;
								j->second.erase(j->second.begin()+ k);
								for(int r = k; r < j->second.size(); r++)
									j->second[r]--;
							}
							else
							{
								in.test[i].m.erase(in.test[i].m.begin()+ j->second[l]);
				                in.test[i].N--;
								j->second.erase(j->second.begin()+ l);
								for(int r = l; r < j->second.size(); r++)
									j->second[r]--;
							}
						}
						
					}
				}
			}
		}
	}
}

static string readFileIntoString(const string& path) {
	ostringstream ss;
	const char * c = path.c_str();
	ifstream input_file(c);
	if (!input_file.is_open()) {
		cerr << "Could not open the file - '"
			<< path << "'" << endl;
		exit(EXIT_FAILURE);
	}
	ss << input_file.rdbuf();
	return ss.str();
}

//Reads data for an instance
static Instance data(const string& path)
{
	Instance in;
	string file_contents;
	char delimiter = ' ';

	file_contents = readFileIntoString(path);

	istringstream sstream(file_contents);
	string record;

	int counter = 0;
	int number = 0;
	while (std::getline(sstream, record)) {
		string record1 = record;
		istringstream line(record);
		istringstream line1(record1);
		int num = 0;
		while (std::getline(line, record, delimiter)) {
			num++;
		}

		if (num == 3)
		{
			in.test.resize(in.test.size()+ 1);
			line1 >> in.test.back().N >> in.test.back().C >> in.test.back().D;
		}
		if(num == 4 && in.test.back().N != 0)
		{
			in.test.back().m.resize(in.test.back().m.size()+1);
			line1 >> in.test.back().m.back().D >> \
					in.test.back().m.back().P >> \
					in.test.back().m.back().R >> \
					in.test.back().m.back().G;
			in.test.back().level[in.test.back().m.back().D].push_back(in.test.back().m.size()-1);
		}
		
	}
	in.test.pop_back();
	in.n_tests = in.test.size();
	pre_processing(in);
	

	return in;
}


/*
	THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
				http://dev-c.com			
			(C) Alexander Blade 2015
*/

#include "scenario.h"
#include <string.h>
#include <direct.h>
#include <fstream>
#include "files.h"
#include <list>
#include <experimental/filesystem>
#include "keyboard.h"
#include <iomanip>
#include <ctime>
#include <sstream>

DWORD	vehUpdateTime;
DWORD	pedUpdateTime;
using namespace std;
namespace fs = std::experimental::filesystem;


void record() {
	char path[] = "AmodalMOTS\\";
	char scenarios_path[] = "AmodalMOTS-Scenarios\\";

	_mkdir(path);

	int max_samples = 30 * 60 * 1 + 1;
	DatasetAnnotator* S;

	int seq_number = 0;

	// Day sequences
	for (auto &p : fs::recursive_directory_iterator(scenarios_path)) {
		if (fs::is_regular_file(p)) {
			//生成时间戳
			time_t time_seconds = time(0);
			struct tm now_time;
			localtime_s(&now_time, &time_seconds);
			char time[80];
			sprintf_s(time, "%02d%02d%02d%02d", now_time.tm_mon + 1, now_time.tm_mday, now_time.tm_hour, now_time.tm_min);
			std::string time_str(time);
			//将数据Scenarios文件拷贝到output_path中
			std::string output_path = std::string(path) + time_str;
			_mkdir(output_path.c_str());
			fs::copy(p, output_path);
			int nsamples = 0;
			S = new DatasetAnnotator(output_path, p.path().string().c_str(), max_samples, 0);
			Sleep(10);
			int frame_id = 1;
			while (nsamples < max_samples) {
				nsamples = (*S).update(frame_id);
				if (nsamples > 0)  //因为update刚开始会空转一会。
				{
					frame_id = frame_id + 1;
				}
				WAIT(0);
			}
			delete static_cast <DatasetAnnotator *>(S);
			seq_number++;
		}
	}
}


void main()
{	
	while (true) {
		if (IsKeyJustUp(VK_F8)) {
			record();
		}
		WAIT(0);

	}
}

void ScriptMain()
{	
	srand(GetTickCount());
	main();
}

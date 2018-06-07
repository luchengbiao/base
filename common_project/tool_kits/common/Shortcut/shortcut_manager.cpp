#include "shortcut_manager.h"

//////////////////////////////////////////////////////////////////////////
//
//指令结构  #[指令]:[参数]  例：#playback:312323  (312323为course_id)
//
//1.MockHeart 模拟上课心跳  #mock_heart_start  || #mock_heart_stop
//2.MockAgoraLogout 模拟上课声网#mock_agora_out
//////////////////////////////////////////////////////////////////////////

ShortCutManager::ShortCutManager()
{

}

ShortCutManager::~ShortCutManager()
{

}

bool ShortCutManager::ParseCommand(std::string command_str)
{
	if (command_str.empty())
	{
		return false;
	}
	
	if (command_str.size() >1)
	{
		 std::string prefix = command_str.substr(0, 1);
		 if (prefix == "#")
		 {
			 std::string sub_command = command_str.substr(1);
			 int index = sub_command.find_first_of(':');
			 if (index >0)
			 {
				 std::string first_str = sub_command.substr(0, index);
				 std::string second_str = sub_command.substr(index+1);
				 if (first_str == "playback" && !second_str.empty())
				 {
					 /* OnlineCoursePlayBack(second_str,"online-course");
					  return false;*/
				 }
				 else if (first_str == "classback" && !second_str.empty())
				 {
					// OnlineCoursePlayBack(second_str, "class-course");
				 }
				 else if (first_str == "vedioes")
				 {
					 /* int sub_index = second_str.find_first_of(',');
					  if (sub_index >0)
					  {
					  std::string user_id = second_str.substr(0, sub_index);
					  std::string course_id = second_str.substr(sub_index + 1);
					  SeriesCoursePlayBack(user_id, course_id);
					  return false;
					  }*/
				 }
				 else if (first_str == "classroom")
				 {
					/* auto enter_cb = ([=](bool ret, std::string msg, int code)
					 {

					 });*/

					 //ClassroomForm::Enter(second_str, "", enter_cb);
				 }
				 //指令可以一直往后加
				 //else if(first_str == "TODO")
				 //{
				 //}
			 }
			 else
			 {
				 if (sub_command == "mock_heart_start")
				 {
					 emit SignalMockHeartStart();
				 }
				 else if (sub_command == "mock_heart_stop")
				 {
					 emit SignalMockHeartStop();
				 }
				 else if (sub_command == "mock_agora_out")
				 {
					 emit SignalMockAgoraLogout();
				 }
				 else if (sub_command == "mock_server_break")
				 {
					 emit SignalMockServerHeartStop();
				 }
				 else if (sub_command == "mock_server_start")
				 {
					 emit SignalMockServerHeartStart();
				 }
			 }
		 }
	}

	return false;
}

void ShortCutManager::OnlineCoursePlayBack(std::string course_id,std::string class_type)
{
//	auto cb = [this,course_id](int code, std::wstring msg, ResUrlMap res_map){
//		if (api_success(code))
//		{
//			if (res_map.size() > 0)
//			{
//				int i_course_id = 0;
//				nbase::StringToInt(course_id,&i_course_id);
//				CoursePlayBackForm::ShowO2OPlayWin(i_course_id,ONE_2_ONE,res_map);
//			}
//		}
//		else
//		{
//			ShowAutoCloseMsgBox(NULL, msg);
//		}
//	};
//
//	std::vector<std::string> vec;
//	vec.push_back("course-pdata-tea");
//	vec.push_back("course-pdata-stu");
//	vec.push_back("course-audio-mix");
//	vec.push_back("course-audio-tea");
//	vec.push_back("course-audio-stu");
////	CourseApi::GetMyO2OCourseRes(course_id, "online-course", vec, cb);
//	CourseApi::GetMyO2OCourseRes(course_id, class_type, vec, cb);
}

void ShortCutManager::SeriesCoursePlayBack(std::string user_id, std::string course_id)
{
	/*auto cb = [this](int code, std::wstring msg, VedioInfoVec vec){
		if (api_success(code))
		{
		if (vec.size() > 0)
		{
		VedioPlayerForm::ShowVedioPlay(vec, 0);
		}
		}
		else
		{
		ShowAutoCloseMsgBox(NULL, msg);
		}
		};

		CourseApi::GetSeriesCourse(user_id,course_id, cb);*/
}





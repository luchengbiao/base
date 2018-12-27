#include "shortcut_manager.h"
#include "base\util\string_number_conversions.h"

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
					 uint64_t number;

					 if (nbase::StringToUint64(second_str, &number))
					 {
						 emit SignalO2OPlayback(number);
					 }
				 }
				 else if (first_str == "classback" && !second_str.empty())
				 {
					 uint64_t number;

					 if (nbase::StringToUint64(second_str, &number))
					 {
						 emit SignalO2MPlayback(number);
					 }
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
				 else if (first_str == "reading_setting" &&!second_str.empty())
				 {
					 if (second_str == "0")
					 {
						emit SignalReadingSetting(false);
						return true;
					 }
					 else if (second_str == "1")
					 {
						emit SignalReadingSetting(true);
						return true;
					 }
				 }

				 else if (first_str == "classroom"&& !second_str.empty())
				 {
					 emit SignalMockOpenClassroom(second_str);
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
				 else
				 {
					 uint64_t number;

					 if (nbase::StringToUint64(sub_command, &number)) //#123
					 {
						 emit SignalO2OPlayback(number);
					 }
					 else if (sub_command.size() > 2 && 
							  sub_command.substr(0, 2) == "##" && 
							  nbase::StringToUint64(sub_command.substr(2), &number)) //###123
					 {
						 emit SignalO2MPlayback(number);
					 }
				 }
			 }
		 }
	}

	return false;
}
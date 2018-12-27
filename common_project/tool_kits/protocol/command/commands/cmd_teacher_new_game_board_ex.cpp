#include "cmd_teacher_new_game_board_ex.h"
#include "common/macros/json_def.h"
#include "json/json.h"
#include "protocol/command/entities/new_game_board_ex.h"

PROTO_NAMESPACE_BEGIN

IMPLEMENT_COMMAND_RUNTIME_INFO(CommandType::TeacherNewGameBoardEx, CmdTeacherNewGameBoardEx, CommandCell)

const NewGameBoardEx* CmdTeacherNewGameBoardEx::EntityOfCommand()
{
	if (!new_game_board_ex_)
	{
		new_game_board_ex_.reset(new NewGameBoardEx);

		new_game_board_ex_->board_type_ = this->IntAt(0);
		new_game_board_ex_->game_name_ = this->StringAt(1);
		new_game_board_ex_->game_mode_ = this->IntAt(2);

		auto ext_str = this->StringAt(3);
		Json::Value value;
		Json::Reader reader;
		if (reader.parse(ext_str, value))
		{
			JSON_FIELD_AS_INT(new_game_board_ex_->role_, role, value);
			JSON_FIELD_AS_INT(new_game_board_ex_->order_, order, value);
			JSON_FIELD_AS_INT(new_game_board_ex_->layout_, layout, value);
			JSON_FIELD_AS_INT(new_game_board_ex_->layout_index_, layoutIndex, value);
			JSON_FIELD_AS_STRING(new_game_board_ex_->white_name_, whiteName, value);
			JSON_FIELD_AS_STRING(new_game_board_ex_->white_url_, whiteUrl, value);
			JSON_FIELD_AS_STRING(new_game_board_ex_->black_name_, blackName, value);
			JSON_FIELD_AS_STRING(new_game_board_ex_->black_url_, blackUrl, value);
		}
	}

	return new_game_board_ex_.get();
}

PROTO_NAMESPACE_END
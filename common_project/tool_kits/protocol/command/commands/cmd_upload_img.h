#ifndef __PROTO_COMMANDS_CMD_UPLOAD_IMG_H__
#define __PROTO_COMMANDS_CMD_UPLOAD_IMG_H__
#include "../base/command_cell.h"

PROTO_NAMESPACE_BEGIN

class CmdUploadImg : public CommandCell
{
	DECLARE_COMMAND_RUNTIME_INFO(CmdUploadImg)

public:
	inline std::wstring	ImageFilePath() const { return image_file_path_; }
	inline void			SetImageFilePath(const std::wstring& image_file_path) { image_file_path_ = image_file_path; }

private:
	std::wstring		image_file_path_;
};

PROTO_NAMESPACE_END

#endif
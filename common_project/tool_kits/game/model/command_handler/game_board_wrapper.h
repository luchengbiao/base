#ifndef __GAME_MODEL_COMMAND_HANDLER_DRAW_BOARD_HODLER_H__
#define __GAME_MODEL_COMMAND_HANDLER_DRAW_BOARD_HODLER_H__
#include "game/public/enums/game_board_type.h"
#include "protocol/command/base/command_fwd.h"

class DrawBoard;
class BaseMarkForm;

GAME_NAMESPACE_BEGIN

class GameBoardWrapper
{
protected:
	typedef std::function<void(const std::string&, const std::wstring&)> FileDownloadedCallback;

public:
	virtual ~GameBoardWrapper() = default;

	virtual void			SwitchMasterToGameBoard(GameBoardType) = 0;

	virtual DrawBoard*		DrawBoardOnGameBoard(GameBoardType) const = 0;

	virtual BaseMarkForm*	MarkBoardOnGameBoard(GameBoardType) const = 0;

	virtual void			DownloadChessManual(const std::string& file_url, const FileDownloadedCallback& callback) = 0;

	virtual	void			HandleMasterCommandSpecially(const CommandSharedPtr&) = 0;

	virtual void			RefreshGameBoardWithMaster(GameBoardType) = 0;

	virtual void			OnLocalCommandConflictWithMaster() = 0;

	virtual void			OnWaitingRecovery() = 0;

	virtual void			OnStartingRecovery() = 0;

	virtual	void			OnRecoveryDone() = 0;
};

GAME_NAMESPACE_END

#endif
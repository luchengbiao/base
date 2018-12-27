#ifndef __GAME_VIEW_GO_CHESS_ID_GENERATOR_CHESS_ID_GENERATOR_H__
#define __GAME_VIEW_GO_CHESS_ID_GENERATOR_CHESS_ID_GENERATOR_H__
#include <memory>
#include <vector>
#include "game/model/go/go_trait.h"
#include "game/view/go/chess_id_generator/chess_id_generator_type.h"

GAME_NAMESPACE_BEGIN

class ChessIdGenerator
{
public:
	virtual ~ChessIdGenerator() = default;

	inline ChessIdGeneratorType Type() const { return type_; }

	virtual ChessId				NextChessId(const ChessId last_chess_id_moved_in) = 0;

protected:
	ChessIdGenerator(ChessIdGeneratorType type) : type_(type) {}

private:
	ChessIdGeneratorType type_{ ChessIdGeneratorType::None };
};

typedef std::shared_ptr<ChessIdGenerator> ChessIdGeneratorSharedPtr;

class ChessIdGeneratorHelper
{
	typedef std::vector<ChessIdGeneratorSharedPtr> ChessIdGenerators;

public:
	ChessIdGeneratorHelper(ChessId last_chess_id_to_move_in, ChessIdGeneratorType type = ChessIdGeneratorType::Default);

	ChessIdGeneratorType		CurrentGeneratorType() const;

	ChessId						NextChessId();

	void						Reset(ChessId last_chess_id_to_move_in, ChessIdGeneratorType type = ChessIdGeneratorType::Default);

private:
	void						LazyCreateGenerators();
	void						SwitchCurrentGenerator(ChessIdGeneratorType type);

private:
	ChessIdGenerators			generators_;
	ChessIdGeneratorSharedPtr	current_generator_;
	ChessId						last_chess_id_to_move_in_{ GoTrait::IdOfWhitePiece };
};

GAME_NAMESPACE_END

#endif
#include "paircommand.h"

PairCommand::PairCommand(QUndoCommand* first, QUndoCommand* second)
    : first(first), second(second) {
    setText("L");
}

PairCommand::~PairCommand(){
    delete second;
    delete first;
}

void PairCommand::redo(){
    first->redo();
    second->redo();
}

void PairCommand::undo(){
    second->undo();
    first->undo();
}

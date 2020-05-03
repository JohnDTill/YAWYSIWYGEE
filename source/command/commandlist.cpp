#include "commandlist.h"

CommandList::CommandList(std::vector<QUndoCommand*> commands)
    : commands(commands) {
    setText("L");
}

CommandList::~CommandList(){
    for(std::vector<QUndoCommand*>::size_type i = commands.size()-1; i < commands.size(); i--)
        delete commands[i];
}

void CommandList::redo(){
    for(QUndoCommand* c : commands) c->redo();
}

void CommandList::undo(){
    for(std::vector<QUndoCommand*>::size_type i = commands.size()-1; i < commands.size(); i--)
        commands[i]->undo();
}

#ifndef COMMANDLIST_H
#define COMMANDLIST_H

#include <QUndoCommand>

namespace Typeset{

class CommandList : public QUndoCommand{
public:
    std::vector<QUndoCommand*> commands;

public:
    CommandList(std::vector<QUndoCommand*> commands);
    ~CommandList();

protected:
    virtual void redo() final;
    virtual void undo() final;
};

}

#endif // COMMANDLIST_H

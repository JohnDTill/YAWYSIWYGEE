#ifndef COMMANDLIST_H
#define COMMANDLIST_H

#include <QUndoCommand>
#include <array>

class PairCommand : public QUndoCommand{
public:
    QUndoCommand* first;
    QUndoCommand* second;

public:
    PairCommand(QUndoCommand* first, QUndoCommand* second);
    ~PairCommand();

protected:
    virtual void redo() final;
    virtual void undo() final;
};

#endif // COMMANDLIST_H

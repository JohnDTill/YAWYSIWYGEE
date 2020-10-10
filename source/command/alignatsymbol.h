#ifndef ALIGNATSYMBOL_H
#define ALIGNATSYMBOL_H

#include <QUndoCommand>
class Cursor;
class Line;
class Text;

class AlignAtSymbol : public QUndoCommand{
private:
    Cursor& cursor;
    bool owns_data;
    std::vector<Text*> texts;

public:
    AlignAtSymbol(Cursor& cursor, Line* ll, Line* lr, const std::vector<qreal>& offsets);
    virtual ~AlignAtSymbol();

protected:
    virtual void redo() final;
    virtual void undo() final;
};

#endif // ALIGNATSYMBOL_H

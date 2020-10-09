#ifndef INDENT_H
#define INDENT_H

#include <QUndoCommand>
class Cursor;
class Line;
class QTextCursor;

class Indent : public QUndoCommand{
private:
    Cursor& cursor;
    Line* ll;
    Line* end;

public:
    Indent(Cursor& cursor, Line* ll, Line* end);

protected:
    virtual void redo() final;
    virtual void undo() final;
};

class Deindent : public QUndoCommand{
private:
    Cursor& cursor;
    Line* ll;
    Line* end;
    std::vector<uint8_t> spaces;

public:
    const bool found_spaces;

public:
    Deindent(Cursor& cursor, Line* ll, Line* end);

protected:
    virtual void redo() final;
    virtual void undo() final;

private:
    bool findSpaces();
    uint8_t countSpaces(const Line* l) const;
};

#endif // INDENT_H

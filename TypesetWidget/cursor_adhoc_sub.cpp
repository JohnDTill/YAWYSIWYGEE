//CODE-GEN FILE
//This file is generated from subfolder "meta".
//Changes to this file must be made in the meta project.

#include "cursor.h"

namespace Typeset{

template<ushort indicator, ushort sub>
static void checkTwoCharSub(Cursor& c, QTextCursor cursor){
    cursor.movePosition(QTextCursor::Left);
    cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);
    if(cursor.selectedText() == indicator){
        c.selectPreviousChar();
        c.selectPreviousChar();
        c.keystroke(QChar(sub));
    }
}

void Cursor::checkForSubstitution(const QChar& c){
    if(cursor.position() < 2) return;

    switch(c.unicode()){
        case ' ':
            checkSlashSub();
            break;
        case '_':
            checkTwoCharSub<60,8804>(*this, cursor);
            checkTwoCharSub<62,8805>(*this, cursor);
            checkTwoCharSub<61,8801>(*this, cursor);
            checkTwoCharSub<8801,8803>(*this, cursor);
            checkTwoCharSub<43,177>(*this, cursor);
            checkTwoCharSub<8847,8849>(*this, cursor);
            checkTwoCharSub<8848,8850>(*this, cursor);
            checkTwoCharSub<8744,8891>(*this, cursor);
            break;
        case '-':
            checkTwoCharSub<60,8592>(*this, cursor);
            checkTwoCharSub<124,8866>(*this, cursor);
            break;
        case ':':
            checkTwoCharSub<58,8759>(*this, cursor);
            checkTwoCharSub<45,8761>(*this, cursor);
            break;
        case '/':
            checkTwoCharSub<92,8744>(*this, cursor);
            checkTwoCharSub<61,8800>(*this, cursor);
            checkTwoCharSub<8712,8713>(*this, cursor);
            checkTwoCharSub<8707,8708>(*this, cursor);
            checkTwoCharSub<8834,8836>(*this, cursor);
            checkTwoCharSub<60,8814>(*this, cursor);
            checkTwoCharSub<62,8815>(*this, cursor);
            checkTwoCharSub<8801,8802>(*this, cursor);
            checkTwoCharSub<8776,8777>(*this, cursor);
            checkTwoCharSub<8773,8775>(*this, cursor);
            checkTwoCharSub<8801,8802>(*this, cursor);
            checkTwoCharSub<8715,8716>(*this, cursor);
            checkTwoCharSub<8834,8836>(*this, cursor);
            checkTwoCharSub<8838,8840>(*this, cursor);
            checkTwoCharSub<8835,8837>(*this, cursor);
            checkTwoCharSub<8839,8841>(*this, cursor);
            break;
        case '\\':
            checkTwoCharSub<47,8743>(*this, cursor);
            break;
        case '<':
            checkTwoCharSub<60,8810>(*this, cursor);
            checkTwoCharSub<8810,8920>(*this, cursor);
            break;
        case '=':
            checkTwoCharSub<58,8788>(*this, cursor);
            checkTwoCharSub<60,8656>(*this, cursor);
            break;
        case '>':
            checkTwoCharSub<45,8594>(*this, cursor);
            checkTwoCharSub<8592,8596>(*this, cursor);
            checkTwoCharSub<8656,8660>(*this, cursor);
            checkTwoCharSub<61,8658>(*this, cursor);
            checkTwoCharSub<8866,8614>(*this, cursor);
            checkTwoCharSub<62,8811>(*this, cursor);
            checkTwoCharSub<8811,8921>(*this, cursor);
            break;
        case '|':
            checkTwoCharSub<124,8214>(*this, cursor);
            break;
        case '~':
            checkTwoCharSub<126,8776>(*this, cursor);
            break;
        case '0':
            checkTwoCharSub<47,8709>(*this, cursor);
    }
}

}

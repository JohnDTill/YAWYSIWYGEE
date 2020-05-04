//CODE-GEN FILE
//This file is generated from subfolder "meta".
//Changes to this file must be made in the meta project.

#include "cursor.h"

//Could check for space ' ' in IF statement, then have single switch off both
//32-bit QChars as a 64-bit switch value.

void Cursor::checkForSubstitution(const QChar& c){
    if(cursor.position() < 2) return;

    switch(c.unicode()){
        case ' ': checkSlashSub(); break;
        case 95:
            cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, 2);
            switch(cursor.selectedText().front().unicode()){
                case 60: cursor.clearSelection(); keystroke(QChar(8804)); break;
                case 62: cursor.clearSelection(); keystroke(QChar(8805)); break;
                case 61: cursor.clearSelection(); keystroke(QChar(8801)); break;
                case 8801: cursor.clearSelection(); keystroke(QChar(8803)); break;
                case 43: cursor.clearSelection(); keystroke(QChar(177)); break;
                case 8847: cursor.clearSelection(); keystroke(QChar(8849)); break;
                case 8848: cursor.clearSelection(); keystroke(QChar(8850)); break;
                case 8744: cursor.clearSelection(); keystroke(QChar(8891)); break;
                default: cursor.movePosition(QTextCursor::Right);
            }
            break;
        case 45:
            cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, 2);
            switch(cursor.selectedText().front().unicode()){
                case 60: cursor.clearSelection(); keystroke(QChar(8592)); break;
                case 124: cursor.clearSelection(); keystroke(QChar(8866)); break;
                default: cursor.movePosition(QTextCursor::Right);
            }
            break;
        case 58:
            cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, 2);
            switch(cursor.selectedText().front().unicode()){
                case 58: cursor.clearSelection(); keystroke(QChar(8759)); break;
                case 45: cursor.clearSelection(); keystroke(QChar(8761)); break;
                default: cursor.movePosition(QTextCursor::Right);
            }
            break;
        case 47:
            cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, 2);
            switch(cursor.selectedText().front().unicode()){
                case 92: cursor.clearSelection(); keystroke(QChar(8744)); break;
                case 61: cursor.clearSelection(); keystroke(QChar(8800)); break;
                case 8712: cursor.clearSelection(); keystroke(QChar(8713)); break;
                case 8707: cursor.clearSelection(); keystroke(QChar(8708)); break;
                case 8834: cursor.clearSelection(); keystroke(QChar(8836)); break;
                case 60: cursor.clearSelection(); keystroke(QChar(8814)); break;
                case 8801: cursor.clearSelection(); keystroke(QChar(8802)); break;
                case 8776: cursor.clearSelection(); keystroke(QChar(8777)); break;
                case 8773: cursor.clearSelection(); keystroke(QChar(8775)); break;
                case 8715: cursor.clearSelection(); keystroke(QChar(8716)); break;
                case 8838: cursor.clearSelection(); keystroke(QChar(8840)); break;
                case 8835: cursor.clearSelection(); keystroke(QChar(8837)); break;
                case 8839: cursor.clearSelection(); keystroke(QChar(8841)); break;
                default: cursor.movePosition(QTextCursor::Right);
            }
            break;
        case 92:
            cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, 2);
            switch(cursor.selectedText().front().unicode()){
                case 47: cursor.clearSelection(); keystroke(QChar(8743)); break;
                default: cursor.movePosition(QTextCursor::Right);
            }
            break;
        case 60:
            cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, 2);
            switch(cursor.selectedText().front().unicode()){
                case 60: cursor.clearSelection(); keystroke(QChar(8810)); break;
                case 8810: cursor.clearSelection(); keystroke(QChar(8920)); break;
                default: cursor.movePosition(QTextCursor::Right);
            }
            break;
        case 61:
            cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, 2);
            switch(cursor.selectedText().front().unicode()){
                case 58: cursor.clearSelection(); keystroke(QChar(8788)); break;
                case 60: cursor.clearSelection(); keystroke(QChar(8656)); break;
                default: cursor.movePosition(QTextCursor::Right);
            }
            break;
        case 62:
            cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, 2);
            switch(cursor.selectedText().front().unicode()){
                case 45: cursor.clearSelection(); keystroke(QChar(8594)); break;
                case 8592: cursor.clearSelection(); keystroke(QChar(8596)); break;
                case 8656: cursor.clearSelection(); keystroke(QChar(8660)); break;
                case 61: cursor.clearSelection(); keystroke(QChar(8658)); break;
                case 8866: cursor.clearSelection(); keystroke(QChar(8614)); break;
                case 62: cursor.clearSelection(); keystroke(QChar(8811)); break;
                case 8811: cursor.clearSelection(); keystroke(QChar(8921)); break;
                default: cursor.movePosition(QTextCursor::Right);
            }
            break;
        case 124:
            cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, 2);
            switch(cursor.selectedText().front().unicode()){
                case 124: cursor.clearSelection(); keystroke(QChar(8214)); break;
                default: cursor.movePosition(QTextCursor::Right);
            }
            break;
        case 126:
            cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, 2);
            switch(cursor.selectedText().front().unicode()){
                case 126: cursor.clearSelection(); keystroke(QChar(8776)); break;
                default: cursor.movePosition(QTextCursor::Right);
            }
            break;
        case 48:
            cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, 2);
            switch(cursor.selectedText().front().unicode()){
                case 47: cursor.clearSelection(); keystroke(QChar(8709)); break;
                default: cursor.movePosition(QTextCursor::Right);
            }
            break;
    }
}

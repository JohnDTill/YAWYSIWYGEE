//CODE-GEN FILE
//This file is generated from subfolder "meta".
//Changes to this file must be made in the meta project.

#include "YAWYSIWYGEE_commands.h"
#include "MathBran/include/QMathBran_Macros.h"
#include <QString>

namespace Commands {

QString lookup(const QString& key){
    if(key.isEmpty() || key.size() > 8) return QString();

    uint8_t hash = static_cast<uint8_t>(key[0].unicode());
    for(uint8_t i = static_cast<uint8_t>(key.size())-1; i > 0; i--)
        hash += static_cast<uint8_t>(key[i].unicode())*(i+16);

    switch(hash){
        case 33:  return key=="vec"      ?  QString( MB_ACCENT_ARROW)+"⏴⏵" : QString();
        case 180: return key=="breve"    ?  QString( MB_ACCENT_BREVE)+"⏴⏵" : QString();
        case 93:  return key=="dddot"    ?  QString( MB_ACCENT_TRIPLE_DOTS)+"⏴⏵" : QString();
        case 114: return key=="ddot"     ?  QString( MB_ACCENT_DOUBLE_DOTS)+"⏴⏵" : QString();
        case 235: return key=="dot"      ?  QString( MB_ACCENT_DOT)+"⏴⏵" : QString();
        case 1:   return key=="hat"      ?  QString( MB_ACCENT_HAT)+"⏴⏵" : QString();
        case 215: return key=="bar"      ?  QString( MB_ACCENT_BAR)+"⏴⏵" : QString();
        case 85:  return key=="tilde"    ?  QString( MB_ACCENT_TILDE)+"⏴⏵" : QString();
        case 226: return key=="sum"      ?  QString( MB_SUMMATION) : QString();
        case 60:  return key=="prod"     ?  QString( MB_PRODUCT) : QString();
        case 248: return key=="coprod"   ?  QString( MB_COPRODUCT) : QString();
        case 182: return key=="bigcap"   ?  QString( MB_INTERSECTION) : QString();
        case 70:  return key=="bigcup"   ?  QString( MB_UNION) : QString();
        case 71:  return key=="biguplus" ?  QString(MB_UNION_PLUS) : QString();
        case 216: return key=="binom"    ?  QString( MB_BINOMIAL_COEFFICIENTS)+"⏴n⏵⏴k⏵" : QString();
        case 101: return key=="cases"    ?  QString( MB_CASES)+"⏴⏵⏴⏵⏴⏵⏴⏵" : QString();
        case 35:  return key=="frac"     ?  QString( MB_FRACTION)+"⏴⏵⏴⏵" : QString();
        case 126: return key=="ceil"     ?  QString( MB_GROUPING_CEIL)+"⏴⏵" : QString();
        case 133: return key=="floor"    ?  QString( MB_GROUPING_FLOOR)+"⏴⏵" : QString();
        case 225: return key=="()"       ?  QString( MB_GROUPING_PARENTHESIS)+"⏴⏵" : QString();
        case 136: return key=="[]"       ?  QString( MB_GROUPING_BRACKETS)+"⏴⏵" : QString();
        case 17:  return key=="eval"     ?  QString( MB_EVALSCRIPT)+"⏴a⏵⏴b⏵" : QString();
        case 249: return key=="abs"      ?  QString( MB_GROUPING_BARS)+"⏴⏵" : QString();
        case 232: return key=="norm"     ?  QString( MB_GROUPING_DOUBLE_BARS)+"⏴⏵" : QString();
        case 254: return key=="iiint"    ?  QString( MB_TRIPLE_INTEGRAL) : QString();
        case 186: return key=="iint"     ?  QString( MB_DOUBLE_INTEGRAL) : QString();
        case 223: return key=="int"      ?  QString( MB_INTEGRAL) : QString();
        case 192: return key=="oint"     ?  QString( MB_CONTOUR_INTEGRAL) : QString();
        case 4:   return key=="oiint"    ?  QString( MB_CLOSED_SURFACE_INTEGRAL) : QString();
        case 177: return key=="oiiint"   ?  QString( MB_CLOSED_VOLUME_INTEGRAL) : QString();
        case 6:   return key=="mat"      ?  QString( MB_MATRIX)+"⏴3⏵⏴3⏵" : QString();
        case 148: return key=="sqrt"     ?  QString( MB_ROOT)+"⏴⏵" : QString();
        case 157: return key=="_^"       ?  QString( MB_DUALSCRIPT)+"⏴⏵⏴⏵" : QString();
        case 94:  return key=="^"        ?  QString( MB_SUPERSCRIPT)+"⏴⏵" : QString();
        case 95:  return key=="_"        ?  QString( MB_SUBSCRIPT)+"⏴⏵" : QString();
        case 78:  return key=="max"      ?  QString( MB_UNDERSCRIPTED_MAX)+"⏴⏵" : QString();
        case 34:  return key=="min"      ?  QString( MB_UNDERSCRIPTED_MIN)+"⏴⏵" : QString();
        case 24:  return key=="sup"      ?  QString( MB_UNDERSCRIPTED_SUP)+"⏴⏵" : QString();
        case 227: return key=="inf"      ?  QString( MB_UNDERSCRIPTED_INF)+"⏴⏵" : QString();
        case 15:  return key=="lim"      ?  QString( MB_LIMIT)+"⏴⏵⏴⏵" : QString();
        default: return QString();
    }
}

};

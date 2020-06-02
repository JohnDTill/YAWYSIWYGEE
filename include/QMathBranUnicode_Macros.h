//CODE-GEN FILE
//This file defines macros to make use of the MathBran serialization format.
//The list of definitions is generated from a spreadsheet in the MathBran project.

#ifndef QMATHBRANUNICODE_MACROS
#define QMATHBRANUNICODE_MACROS

#define MATHBRAN_SUBSCRIPT_ALLOWED_CASES \
	case 48: \
	case 49: \
	case 50: \
	case 51: \
	case 52: \
	case 53: \
	case 54: \
	case 55: \
	case 56: \
	case 57: \
	case 43: \
	case 45: \
	case 61: \
	case 40: \
	case 41: \
	case 97: \
	case 101: \
	case 104: \
	case 105: \
	case 106: \
	case 107: \
	case 108: \
	case 109: \
	case 110: \
	case 111: \
	case 112: \
	case 114: \
	case 115: \
	case 116: \
	case 117: \
	case 118: \
	case 120: \
	case 946: \
	case 947: \
	case 961: \
	case 966: \
	case 967:

#define MATHBRAN_SYMBOL_TO_SUBSCRIPT_MAP \
	case 48: out << QChar(8320); break; \
	case 49: out << QChar(8321); break; \
	case 50: out << QChar(8322); break; \
	case 51: out << QChar(8323); break; \
	case 52: out << QChar(8324); break; \
	case 53: out << QChar(8325); break; \
	case 54: out << QChar(8326); break; \
	case 55: out << QChar(8327); break; \
	case 56: out << QChar(8328); break; \
	case 57: out << QChar(8329); break; \
	case 43: out << QChar(8330); break; \
	case 45: out << QChar(8331); break; \
	case 61: out << QChar(8332); break; \
	case 40: out << QChar(8333); break; \
	case 41: out << QChar(8334); break; \
	case 97: out << QChar(8336); break; \
	case 101: out << QChar(8337); break; \
	case 104: out << QChar(8341); break; \
	case 105: out << QChar(7522); break; \
	case 106: out << QChar(11388); break; \
	case 107: out << QChar(8342); break; \
	case 108: out << QChar(8343); break; \
	case 109: out << QChar(8344); break; \
	case 110: out << QChar(8345); break; \
	case 111: out << QChar(8338); break; \
	case 112: out << QChar(8346); break; \
	case 114: out << QChar(7523); break; \
	case 115: out << QChar(8347); break; \
	case 116: out << QChar(8348); break; \
	case 117: out << QChar(7524); break; \
	case 118: out << QChar(7525); break; \
	case 120: out << QChar(8339); break; \
	case 946: out << QChar(7526); break; \
	case 947: out << QChar(7527); break; \
	case 961: out << QChar(7528); break; \
	case 966: out << QChar(7529); break; \
	case 967: out << QChar(7530); break;

#define MATHBRAN_SUPERSCRIPT_ALLOWED_CASES \
	case 48: \
	case 49: \
	case 50: \
	case 51: \
	case 52: \
	case 53: \
	case 54: \
	case 55: \
	case 56: \
	case 57: \
	case 43: \
	case 45: \
	case 61: \
	case 40: \
	case 41: \
	case 65: \
	case 66: \
	case 68: \
	case 69: \
	case 71: \
	case 72: \
	case 73: \
	case 74: \
	case 75: \
	case 76: \
	case 77: \
	case 78: \
	case 79: \
	case 80: \
	case 82: \
	case 84: \
	case 85: \
	case 86: \
	case 87: \
	case 97: \
	case 98: \
	case 99: \
	case 100: \
	case 101: \
	case 102: \
	case 103: \
	case 104: \
	case 105: \
	case 106: \
	case 107: \
	case 108: \
	case 109: \
	case 110: \
	case 111: \
	case 112: \
	case 114: \
	case 115: \
	case 116: \
	case 117: \
	case 118: \
	case 119: \
	case 120: \
	case 121: \
	case 122: \
	case 946: \
	case 947: \
	case 948: \
	case 1013: \
	case 952: \
	case 953: \
	case 957: \
	case 966: \
	case 967: \
	case 8747: \
	case 632:

#define MATHBRAN_SYMBOL_TO_SUPERSCRIPT_MAP \
	case 48: out << QChar(8304); break; \
	case 49: out << QChar(185); break; \
	case 50: out << QChar(178); break; \
	case 51: out << QChar(179); break; \
	case 52: out << QChar(8308); break; \
	case 53: out << QChar(8309); break; \
	case 54: out << QChar(8310); break; \
	case 55: out << QChar(8311); break; \
	case 56: out << QChar(8312); break; \
	case 57: out << QChar(8313); break; \
	case 43: out << QChar(8314); break; \
	case 45: out << QChar(8315); break; \
	case 61: out << QChar(8316); break; \
	case 40: out << QChar(8317); break; \
	case 41: out << QChar(8318); break; \
	case 65: out << QChar(7468); break; \
	case 66: out << QChar(7470); break; \
	case 68: out << QChar(7472); break; \
	case 69: out << QChar(7473); break; \
	case 71: out << QChar(7475); break; \
	case 72: out << QChar(7476); break; \
	case 73: out << QChar(7477); break; \
	case 74: out << QChar(7478); break; \
	case 75: out << QChar(7479); break; \
	case 76: out << QChar(7480); break; \
	case 77: out << QChar(7481); break; \
	case 78: out << QChar(7482); break; \
	case 79: out << QChar(7484); break; \
	case 80: out << QChar(7486); break; \
	case 82: out << QChar(7487); break; \
	case 84: out << QChar(7488); break; \
	case 85: out << QChar(7489); break; \
	case 86: out << QChar(11389); break; \
	case 87: out << QChar(7490); break; \
	case 97: out << QChar(7491); break; \
	case 98: out << QChar(7495); break; \
	case 99: out << QChar(7580); break; \
	case 100: out << QChar(7496); break; \
	case 101: out << QChar(7497); break; \
	case 102: out << QChar(7584); break; \
	case 103: out << QChar(7501); break; \
	case 104: out << QChar(688); break; \
	case 105: out << QChar(8305); break; \
	case 106: out << QChar(690); break; \
	case 107: out << QChar(7503); break; \
	case 108: out << QChar(737); break; \
	case 109: out << QChar(7504); break; \
	case 110: out << QChar(8319); break; \
	case 111: out << QChar(7506); break; \
	case 112: out << QChar(7510); break; \
	case 114: out << QChar(691); break; \
	case 115: out << QChar(738); break; \
	case 116: out << QChar(7511); break; \
	case 117: out << QChar(7512); break; \
	case 118: out << QChar(7515); break; \
	case 119: out << QChar(695); break; \
	case 120: out << QChar(739); break; \
	case 121: out << QChar(696); break; \
	case 122: out << QChar(7611); break; \
	case 946: out << QChar(7517); break; \
	case 947: out << QChar(7518); break; \
	case 948: out << QChar(7519); break; \
	case 1013: out << QChar(7499); break; \
	case 952: out << QChar(7615); break; \
	case 953: out << QChar(7589); break; \
	case 957: out << QChar(7609); break; \
	case 966: out << QChar(7520); break; \
	case 967: out << QChar(7521); break; \
	case 8747: out << QChar(7604); break; \
	case 632: out << QChar(7602); break;

#endif

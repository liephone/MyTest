
#include "URLCodec.h"
#include <stdlib.h>

using namespace std;


string URLCodec::encode(const string& url)
{
	string out;
	out.resize(url.size() * 3 + 1);
	int len = InternalUrlEncode(url.c_str(), &out[0], true, false);
	out.resize(len);
	return out;
}

string URLCodec::decode(const string& url)
{
	string out;
	out.resize(url.size()+1);
	int len = InternalUrlDecode(url.c_str(), &out[0], true);
	out.resize(len);
	return out;
}

string URLCodec::encodeFromMap(const map<string, string>& data)
{
	string ret;
	map<string, string>::const_iterator it;
	for (it = data.begin(); it != data.end(); it++) {
		if (!ret.empty()) {
			ret.append("&");
		}

		ret.append(URLCodec::encode(it->first));
		ret.append("=");
		ret.append(URLCodec::encode(it->second));
	}

	return ret;
}

/*
map<string, string> URLCodec::decodeToMap(const string& data)
{
	map<string, string> ret;

	//TODO: 성능 문제
	vector<string> nameValues = StringUtil::splitAndTrim(data, '&');

	for (size_t i = 0; i < nameValues.size(); i++) {
		string nvString = nameValues[i];
		size_t pos = nvString.find('=');
		if (pos != nvString.npos) {
			ret[nvString.substr(0, pos)] = URLCodec::decode(nvString. substr(pos + 1));
		}
	}

	return ret;
}
*/

bool URLCodec::IsValidUrlChar(char ch, bool unsafe_only)
{
	if(ch < 0)
		return false;

	if (unsafe_only) {
		return !(ch <= ' ' || strchr("\\\"^&`<>[]{}", ch));
	} else {
		return isalnum(ch) || strchr("-_.!~*'()", ch);
	}
}

int URLCodec::HexPairValue(const char* code) 
{
	int value = 0;
	const char * pch = code;

	for (;;) {
		int digit = *pch++;

		if (digit >= '0' && digit <= '9') {
			value += digit - '0';
		}
		else if (digit >= 'A' && digit <= 'F') {
			value += digit - 'A' + 10;
		}
		else if (digit >= 'a' && digit <= 'f') {
			value += digit - 'a' + 10;
		}
		else {
			return -1;
		}

		if (pch == code + 2)
			return value;

		value <<= 4;
	}
}

int URLCodec::InternalUrlDecode(const char* source, char* dest, bool encode_space_as_plus)
{
	char * start = dest;

	while (*source) {
		switch (*source) {
		case '+':
			if (encode_space_as_plus) {
				*(dest++) = ' ';
			} else {
				*dest++ = *source;
			}
			break;

		case '%':
			if (source[1] && source[2]) {
				int value = HexPairValue(source + 1);
				if (value >= 0) {
					*(dest++) = value;
					source += 2;
				} else {
					*dest++ = '?';
				}
			} else {
				*dest++ = '?';
			}
			break;

		default:
			*dest++ = *source;
		}
		source++;
	}

	*dest = 0;
	return dest - start;
}

int URLCodec::InternalUrlEncode(const char* source, char* dest, bool encode_space_as_plus, bool unsafe_only)
{
	static const char *digits = "0123456789ABCDEF";
	char *start = dest;

	while (*source) {
		unsigned char ch = static_cast<unsigned char>(*source);

		if (*source == ' ' && encode_space_as_plus && !unsafe_only) {
			*dest++ = '+';
		} else if (IsValidUrlChar(ch, unsafe_only)) {
			*dest++ = *source;
		} else {
			*dest++ = '%';
			*dest++ = digits[(ch >> 4) & 0x0F];
			*dest++ = digits[       ch & 0x0F];
		}
		source++;
	}
	*dest = 0;

	return dest - start;
}


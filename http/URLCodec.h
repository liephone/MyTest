#ifndef _URLCODEC_H
#define _URLCODEC_H

#include <string>
#include <map>

class URLCodec
{
public:
	URLCodec() {};
	~URLCodec() {};

	static std::string encode(const std::string& url);
	static std::string decode(const std::string& url);
	static std::string encodeFromMap(std::map<std::string, std::string>& data);
	//static std::map<std::string, std::string> decodeToMap(const std::string& data);

private:
	static bool IsValidUrlChar(char ch, bool unsafe_only);
	static int HexPairValue(const char* code);
	static int InternalUrlDecode(const char* source, char* dest, bool encode_space_as_plus);
	static int InternalUrlEncode(const char *source, char *dest, bool encode_space_as_plus, bool unsafe_only);

};


#endif//_URLCODEC_H

#pragma once
#include <string>
#include <fstream>
#include <iostream>
class Reader
{
private:
	std::string m_str;  ///< @brief »ñµÃÐè½âÎöµÄ×Ö·ûÐòÁÐ
	int m_pos;          ///< @brief µ±Ç°Ö¸Ïò×Ö·ûÐòÁÐµÄÎ»ÖÃ
	int m_len;          ///< #brief ×Ö·ûÐòÁÐ³¤¶È
public:
	bool isEOF();     ///< @brief ÊÇ·ñ½áÊø
	int getLine();    ///< @brief ·µ»ØÐÐºÅ
	int getRow();     ///< @brief ·µ»ØÁÐºÅ

	char ReadChar();  ///< @brief ¶ÁÈ¡Ò»¸ö×Ö·û
	char PeekChar();  ///< @brief Ô¤¶ÁÈ¡Ò»¸ö×Ö·û

	void ignoreSpace();  ///< @brief Ìø¹ý¿Õ°××Ö·û

						 /// @brief Æ¥ÅäÒ»¸ö×Ö·û
						 /// @note  Æ¥ÅäÒ»¸ö×Ö·û£¬Ê§°ÜÊä³ö´íÎó
						 /// @param[in] ch ÒªÆ¥ÅäµÄ×Ö·û
						 /// @parampin] bIgnoreSpace ºöÂÔ¿Õ¸ñ
	void match(char ch, bool bIgnoreSpace);

	/// @brief Æ¥ÅäÒ»¸ö×Ö·û´®
	/// @note  Æ¥ÅäÒ»¸ö×Ö·û´®£¬Ê§°ÜÊä³ö´íÎó
	/// @param[in] str ÒªÆ¥ÅäµÄ×Ö·û´®
	/// @param[in] bIgnoreSpace ºöÂÔ¿Õ¸ñ
	void match(std::string str, bool bIgnoreSpace);

	/// @brief ³¢ÊÔÆ¥ÅäÒ»¸ö×Ö·û
	/// @note  ³¢ÊÔÆ¥ÅäÒ»¸ö×Ö·û£¬Ê§°ÜÊä³ö´íÎó
	/// @param[in] ch ÒªÆ¥ÅäµÄ×Ö·û
	/// @param[in] bIgnoreSpace ºöÂÔ¿Õ¸ñ
	/// @param[in] bMatch Èç¹ûÄÜÆ¥Åä£¬ÊÇ·ñÆ¥ÅäÕâ¸ö×Ö·û
	/// @return  true=¿ÉÒÔÆ¥Åä£¬false=Æ¥ÅäÊ§°Ü
	bool tryMatch(char ch, bool bIgnoreSpace, bool bMatch);

	/// @brief ³¢ÊÔÆ¥ÅäÒ»¸ö×Ö·û´®
	/// @note  ³¢ÊÔÆ¥ÅäÒ»¸ö×Ö·û´®£¬Ê§°ÜÊä³ö´íÎó
	/// @param[in] str ÒªÆ¥ÅäµÄ×Ö·û´®
	/// @param[in] bIgnoreSpace ºöÂÔ¿Õ¸ñ
	/// @param[in] bMatch Èç¹ûÄÜÆ¥Åä£¬ÊÇ·ñÆ¥ÅäÕâ¸ö×Ö·û
	/// @return  true=¿ÉÒÔÆ¥Åä£¬false=Æ¥ÅäÊ§°Ü
	bool tryMatch(std::string str, bool bIgnoreSpace, bool bMatch);
public:
	/// @brief ´ÓÎÄ±¾¹¹Ôì
	/// @param[in] srcText Òª½øÐÐÆ¥ÅäµÄÔ­Ê¼×Ö·û´®
	Reader(const std::string& srcText);

	/// @brief ´ÓÁ÷¹¹Ôì
	/// @param[in] pStream Òª½øÐÐÆ¥ÅäµÄÔ­Ê¼Êý¾ÝÁ÷
	Reader(std::ifstream* pStream);
	~Reader();
};
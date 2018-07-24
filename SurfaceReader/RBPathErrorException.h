//RBPathErrorException: provides an exception thrown when the path cannot be obtained for some reason.
#ifndef RBPATHERROREXCEPTION_H
#define RB_PATH_FUNCS_H
#pragma once
//Standard c++ includes.
#include <string>

namespace RaisedBar
{
	namespace PathFunctions
	{
		class RBPathErrorException
		{
		public:
			RBPathErrorException(const std::wstring& msg)
				: strExceptionMsg(msg)
			{
			}

			const wchar_t* what() const
			{
				return strExceptionMsg.c_str();
			}

		private:
			std::wstring strExceptionMsg;
		};
	}
}
#endif
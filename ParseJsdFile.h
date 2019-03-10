// ParseJSDFile.h

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>

//Enum defining the function/script type.
enum FunctionType { 
	ID_TYPE_FUNCTION, //function.
	ID_TYPE_SCRIPT //script.
};
struct JawsFunction {
	std::wstring Name; //function/script name.
	std::wstring Synopsis; //Synopsis of the script/function.
	std::wstring Description; //the function/scripts  description.
FunctionType Type;
};
//We now need to tell fusion that the JawsFunction structure is a first-class citizen.
BOOST_FUSION_ADAPT_STRUCT(JawsFunction,
    (std::wstring, Name)
    (std::wstring, Synopsis)
    (std::wstring, Description)
	(FunctionType, Type)
	)
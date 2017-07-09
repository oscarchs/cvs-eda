#include "StringSplitter.h"

namespace LL
{
    void StringSplitter::set_string(std::string new_string)
    {
        _V_data.clear();
        _V_string=new_string;
    }
    std::string StringSplitter::get_string()
    {
        return _V_string;
    }
    bool StringSplitter::split(char character)
    {
        _V_data.clear();
        std::string data=_V_string;
        if(!data.size())
            return false;
        for(unsigned int position=data.find(character);position<data.size();position=data.find(character))
        {
            _V_data.push_back(data.substr(0,position));
            data=data.substr(position+1,data.size()-(position+1));
        }
        if(data.size())
            _V_data.push_back(data);
        return true;
    }
    unsigned int StringSplitter::size()
    {
        return _V_data.size();
    }
    void StringSplitter::clear()
    {
        _V_data.clear();
    }
    const std::string StringSplitter::operator [] (unsigned int index)
    {
        return _V_data[index];
    }
    StringSplitter::~StringSplitter()
    {
        _V_data.clear();
    }
}
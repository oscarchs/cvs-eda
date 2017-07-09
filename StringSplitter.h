#ifndef INCLUDED_LL_STRINGSPLITTER_H
#define INCLUDED_LL_STRINGSPLITTER_H


#include <string>
#include <vector>

namespace LL
{
    class StringSplitter
    {
        private:
            std::string _V_string;
            std::vector<std::string> _V_data;
        public:
            void set_string(std::string new_string);
            std::string get_string();
            bool split(char character='\n');
            unsigned int size();
            void clear();
            const std::string operator [] (unsigned int index);
            ~StringSplitter();
    };
}

#endif // INCLUDED_LL_STRINGSPLITTER_H
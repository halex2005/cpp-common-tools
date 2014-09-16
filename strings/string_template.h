#ifndef __STRING_TEMPLATE_H__
#define __STRING_TEMPLATE_H__

#include <initializer_list>
#include <string>
#include <sstream>
#include <cstddef>

namespace strings
{
    class string_template
    {
    public:
        string_template(const char *t)
            : _source(t)
        {
        }

        std::string substitute(const std::initializer_list<std::pair<const char*, const char *>> &substitutions)
        {
            return "value";
        }
    private:
        std::string _source;
    };


    struct string_substitution_list {};

    inline std::ostream &operator<<(std::ostream &stream, string_substitution_list &&list)
    {
        return stream;
    }

    inline std::stringstream &operator<<(std::stringstream &stream, const string_substitution_list &list)
    {
        return stream;
    }

    inline string_substitution_list operator%(const string_template &t, int value)
    {
        // todo: add value to list
        return string_substitution_list();
    }

    inline string_substitution_list &operator%(string_substitution_list &&list, int value)
    {
        // todo: add value to list
        return list;
    }

    inline string_substitution_list &operator%(string_substitution_list &&list, const char *value)
    {
        // todo: add value to list
        return list;
    }

    inline string_substitution_list &operator%(string_substitution_list &&list, const std::string &value)
    {
        // todo: add value to list
        return list;
    }

}

#endif

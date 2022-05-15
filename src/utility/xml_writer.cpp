//
// MIT License
//
// Copyright (c) 2022 Jan Möller
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#include "bugspray/utility/xml_writer.hpp"

#include <stdexcept>

namespace bs
{
xml_writer::xml_writer(std::ostream& ostream, std::string_view version, std::string_view encoding)
    : m_ostream(ostream)
{
    m_ostream << R"(<?xml version=")" << version << R"(" encoding=")" << encoding << R"("?>)";
}

xml_writer::~xml_writer()
{
    if (!m_open_elements.empty())
        throw std::runtime_error{"unclosed tags"};
}

void xml_writer::open_element(std::string_view tag)
{
    write_newline();
    write_indentation();
    m_open_elements.push_back(tag);
    m_ostream << R"(<)" << tag;
}

void xml_writer::close_element()
{
    auto const tag = m_open_elements.back();
    m_open_elements.pop_back();

    write_newline();
    write_indentation();
    m_ostream << R"(</)" << tag << R"(>)";
}

void xml_writer::write_attribute(std::string_view key, std::string_view value)
{
    m_ostream << ' ' << key << '=' << '\"' << value << '\"';
}

void xml_writer::close_attribute_section()
{
    m_ostream << '>';
}

void xml_writer::close_attribute_and_element()
{
    m_open_elements.pop_back();
    m_ostream << "/>";
}

void xml_writer::write_content(std::string_view content)
{
    m_ostream << content;
}

void xml_writer::write_indentation()
{
    for (std::size_t i = 0; i < m_open_elements.size() * 2; ++i)
        m_ostream << ' ';
}

void xml_writer::write_newline()
{
    m_ostream << '\n';
}
} // namespace bs
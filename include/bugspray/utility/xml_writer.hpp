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

#ifndef BUGSPRAY_XML_WRITER_HPP
#define BUGSPRAY_XML_WRITER_HPP

#include <concepts>
#include <ostream>
#include <string_view>
#include <vector>

namespace bs
{
struct xml_writer
{
    explicit xml_writer(std::ostream& ostream, std::string_view version = "1.0", std::string_view encoding = "UTF-8");
    ~xml_writer();

    void open_element(std::string_view tag);
    void close_element();

    void write_attribute(std::string_view key, std::string_view value);
    void close_attribute_section();
    void close_attribute_and_element();

    void write_content(std::string_view content);

  private:
    std::ostream&                 m_ostream;
    std::vector<std::string_view> m_open_elements;

    void write_indentation();
    void write_newline();
};
} // namespace bs

#endif // BUGSPRAY_XML_WRITER_HPP

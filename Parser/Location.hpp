//
// Created by user on 17-August-2023.
//

#ifndef LATTICE_LOCATION_HPP
#define LATTICE_LOCATION_HPP

#include <string>
#include <sstream>
#include <mutex>
#include <vector>

namespace Parser {

    class LineInfo
    {
            int offset;
            std::string filename;
            int line;
            int column;

        public:

    };

    class SourceFile
    {
            std::string name;
            int base;
            int size;

            std::mutex mutex;
            std::vector<int> lines;
            std::vector<LineInfo> infos;

        public:
            [[nodiscard]] std::string getName() const
            {
                return name;
            }

            [[nodiscard]] int getBase() const
            {
                return base;
            }

            [[nodiscard]] int getSize() const
            {
                return size;
            }

            int getLineCount()
            {
                int n;

                {
                    std::lock_guard<std::mutex> lock(mutex);
                    n = (int) (lines.size());
                }

                return n;
            }

            void addLine(int offset)
            {
                std::lock_guard<std::mutex> lock(mutex);
                std::size_t i = lines.size();
                if ((i == 0 || lines[i - 1] < offset) && offset < size) {
                    lines.push_back(offset);
                }
            }

            void mergeLine(int line)
            {
                if (line < 1)
                    throw std::invalid_argument("line must be positive");

                {
                    std::lock_guard<std::mutex> lock(mutex);

                    if (line > lines.size())
                        throw std::invalid_argument("line is out of range");

                    // To merge the line numbered <line> with the line numbered <line+1>,
                    // we need to remove the entry in lines corresponding to the line
                    // numbered <line+1>. The entry in lines corresponding to the line
                    // numbered <line+1> is located at index <line>, since indices in lines
                    // are 0-based and line numbers are 1-based.
                    lines.erase(lines.begin() + line);
                }
            }

            std::vector<int> getLines()
            {
                std::vector<int> l;

                {
                    std::lock_guard<std::mutex> lock(mutex);
                    l = lines;
                }

                return l;
            }

            bool setLines(std::vector<int> l)
            {
                std::size_t s = size;
                int i = 0;
                for (auto offset: l) {
                    if (i > 0 && offset <= l[i - 1] || s <= offset) {
                        return false;
                    }
                    i++;
                }

                {
                    std::lock_guard<std::mutex> lock(mutex);
                    lines = l;
                }

                return true;
            }

            void setLinesForContent(const std::vector<std::byte> &content)
            {
//                void File::SetLinesForContent(const std::vector<std::byte>& content) {
//                    std::vector<int> lines;
//                    int line = 0;
//                    for (std::size_t offset = 0; offset < content.size(); ++offset) {
//                        std::byte b = content[offset];
//                        if (line >= 0) {
//                            lines.push_back(line);
//                        }
//                        line = -1;
//                        if (b == static_cast<std::byte>('\n')) {
//                            line = static_cast<int>(offset) + 1;
//                        }
//                    }
//
//                    // set lines table
//                    {
//                        std::lock_guard<std::mutex> lock(mutex);
//                        this->lines = lines;
//                    }
//                }

                // TODO: implement this
            }
    };
}

#endif //LATTICE_LOCATION_HPP

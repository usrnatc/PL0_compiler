//
// Created by user on 17-August-2023.
//

#ifndef LATTICE_LOCATION_HPP
#define LATTICE_LOCATION_HPP

#include <string>
#include <sstream>
#include <mutex>
#include <vector>
#include <stdexcept>
#include <cstdint>
#include <algorithm>

namespace Parser {

    const int NO_POSITION = 0;

    struct LineInfo
    {
        int offset;
        std::string filename;
        int line;
        int column;

        LineInfo(int offset, std::string filename, int line, int column)
                : offset(offset)
                  , filename(std::move(filename))
                  , line(line)
                  , column(column)
        {}
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
                std::vector<int> l;
                int line = 0;
                for (std::size_t offset = 0;
                     offset < content.size(); ++offset) {
                    std::byte b = content[offset];
                    if (line >= 0) {
                        l.push_back(line);
                    }
                    line = -1;
                    if (b == static_cast<std::byte>('\n')) {
                        line = static_cast<int>(offset) + 1;
                    }
                }

                {
                    std::lock_guard<std::mutex> lock(mutex);
                    this->lines = l;
                }
            }

            int lineStart(int line)
            {
                if (line < 1)
                    throw std::invalid_argument("expected line >= 1");

                int result;

                {
                    std::lock_guard<std::mutex> lock(mutex);

                    if (line > lines.size()) {
                        throw std::invalid_argument(
                                "expected line < file size");
                    }

                    result = base + lines[line - 1];
                }

                return result;
            }

            void addLineColumnInfo(int offset, std::string filename, int line,
                                   int column)
            {
                std::lock_guard<std::mutex> lock(mutex);

                int i = infos.size();

                if ((i == 0 || infos[i - 1].offset < offset) && offset < size) {
                    infos.emplace_back(offset, std::move(filename), line,
                                       column);
                }
            }

            static int searchInts(const std::vector<int> &a, int x)
            {
                int i = 0;
                int j = a.size();

                while (i < j) {
                    int h = ((int) (((std::uint64_t) i + j))) >> 1;

                    if (a[h] <= x) {
                        i = h + 1;
                    } else {
                        j = h;
                    }
                }

                return i - 1;
            }

            int searchLineInfos(const std::vector<LineInfo> &a, int x)
            {
                // FIXME: we shouldn't be sorting this every time we want to search
                std::sort(infos.begin(), infos.end(),
                          [](const LineInfo &lhs, const LineInfo &rhs) {
                              return lhs.offset < rhs.offset;
                          });

                auto it = std::lower_bound(infos.begin(), infos.end(), x,
                                           [](const LineInfo &info, int value) {
                                               return info.offset < value;
                                           });

                return static_cast<int>(std::distance(infos.begin(), it)) - 1;
            }

            LineInfo unpack(int offset, bool adjusted)
            {
                int line;
                int column;
                std::string filename;

                {
                    std::lock_guard<std::mutex> lock(mutex);

                    filename = std::move(name);
                    int i = searchInts(lines, offset);

                    if (i >= 0) {
                        line = i + 1;
                        column = offset - lines[i] + 1;
                    }

                    if (adjusted && !infos.empty()) {
                        LineInfo alt = infos[i];
                        filename = std::move(alt.filename);
                        i = searchLineInfos(infos, offset);

                        if (i >= 0) {
                            int d = line - (i + 1);
                            line = alt.line + d;

                            if (!alt.column) { // alt.column == 0
                                column = 0;
                            } else if (!d) { // d == 0
                                column = alt.column + (offset - alt.offset);
                            }
                        }
                    }
                }

                return {offset, filename, line, column};
            }
    };
}

#endif //LATTICE_LOCATION_HPP

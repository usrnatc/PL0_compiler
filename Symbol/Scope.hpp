//
// Created by user on 16-August-2023.
//

#ifndef LATTICE_SCOPE_HPP
#define LATTICE_SCOPE_HPP

#include <memory>
#include "SymbolEntry.hpp"
#include <map>
#include <vector>
#include <ranges>

namespace Symbol {

    class Scope
    {
            std::unique_ptr<Scope> parent;
            int level;
            std::unique_ptr<SymbolEntry> ownerEntry;
            std::map<std::string, std::shared_ptr<SymbolEntry>> entries;
            int variableSpace;

        public:
            Scope(std::unique_ptr<Scope> parent, int level,
                  std::unique_ptr<SymbolEntry> ownerEntry)
                    : parent(std::move(parent))
                      , level(level)
                      , ownerEntry(std::move(ownerEntry))
                      , variableSpace(0)
                      , entries(
                            std::map<std::string, std::shared_ptr<SymbolEntry>>())
            {}

            Scope *getParent()
            {
                return parent.get();
            }

            [[nodiscard]] int getLevel() const
            {
                return level;
            }

            std::vector<std::shared_ptr<SymbolEntry>> getEntries()
            {
                auto result = std::vector<std::shared_ptr<SymbolEntry>>();
                std::ranges::copy(std::views::values(entries),
                                  std::back_inserter(result));
                return result;
            }

            std::shared_ptr<SymbolEntry> lookup(const std::string &name)
            {
                auto result = entries.find(name);
                if (result != entries.end())
                    return result->second;

                if (parent != nullptr)
                    return parent->lookup(name);

                return nullptr;
            }

            std::shared_ptr<SymbolEntry>
            addEntry(std::shared_ptr<SymbolEntry> entry)
            {
                if (entries.contains(entry->getName()))
                    return nullptr;

                entry->getScope(this);
                entries[entry->getName()] = entry;
                return entry;
            }

            void resolveScope()
            {
                for (auto &entry: entries)
                    entry.second->resolve();
            }

            [[nodiscard]] int getVariableSpace() const
            {
                return variableSpace;
            }

            int allocVariableSpace(int size)
            {
                int offset = variableSpace;
                variableSpace += size;
                (void) offset; // FIXME: implement StackMachine
                return variableSpace;
            }
    };
}

#endif //LATTICE_SCOPE_HPP

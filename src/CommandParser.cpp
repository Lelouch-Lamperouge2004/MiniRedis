#include "CommandParser.h"

#include <iostream>
#include <sstream>
#include <vector>

CommandParser::CommandParser(
    LRUCache &cache,
    PersistenceManager &persistenceManager)
    : cache(cache),
      persistenceManager(persistenceManager)
{
}

void CommandParser::start()
{
    std::string input;

    while (true)
    {
        std::cout << "> ";

        std::getline(std::cin, input);

        if (input.empty())
        {
            continue;
        }

        std::stringstream ss(input);

        std::vector<std::string> tokens;

        std::string word;

        while (ss >> word)
        {
            tokens.push_back(word);
        }

        if (tokens.empty())
        {
            continue;
        }

        std::string command = tokens[0];

        if (command == "SET")
        {
            if (tokens.size() == 3)
            {
                cache.set(tokens[1], tokens[2]);

                std::cout << "OK" << std::endl;
            }
            else if (tokens.size() == 5 && tokens[3] == "EX")
            {
                int ttlSeconds = std::stoi(tokens[4]);

                cache.set(tokens[1],
                          tokens[2],
                          ttlSeconds);

                std::cout << "OK" << std::endl;
            }
            else
            {
                std::cout
                    << "Usage: SET <key> <value> [EX seconds]"
                    << std::endl;
            }
        }
        else if (command == "GET")
        {
            if (tokens.size() != 2)
            {
                std::cout << "Usage: GET <key>" << std::endl;
                continue;
            }

            std::cout << cache.get(tokens[1]) << std::endl;
        }
        else if (command == "DEL")
        {
            if (tokens.size() != 2)
            {
                std::cout << "Usage: DEL <key>" << std::endl;
                continue;
            }

            bool deleted = cache.erase(tokens[1]);

            if (deleted)
            {
                std::cout << "Deleted" << std::endl;
            }
            else
            {
                std::cout << "Key Not Found" << std::endl;
            }
        }
        else if (command == "DISPLAY")
        {
            cache.display();
        }
        else if (command == "SIZE")
        {
            std::cout << cache.size() << std::endl;
        }
        else if (command == "EXISTS")
        {
            if (tokens.size() != 2)
            {
                std::cout << "Usage: EXISTS <key>" << std::endl;
                continue;
            }

            if (cache.exists(tokens[1]))
            {
                std::cout << "true" << std::endl;
            }
            else
            {
                std::cout << "false" << std::endl;
            }
        }
        else if (command == "TTL")
        {
            if (tokens.size() != 2)
            {
                std::cout << "Usage: TTL <key>" << std::endl;
                continue;
            }

            std::cout << cache.ttl(tokens[1]) << std::endl;
        }
        else if (command == "CLEAR")
        {
            cache.clear();
            std::cout << "Cache Cleared" << std::endl;
        }
        else if (command == "EXIT")
        {
            persistenceManager.save(cache);

            std::cout << "Cache Saved" << std::endl;
            std::cout << "Goodbye" << std::endl;

            break;
        }
        else
        {
            std::cout << "Unknown Command" << std::endl;
        }
    }
}
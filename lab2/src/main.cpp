#include <iostream>
#include <ostream>

#include "PrisonerDilemma/Player/Player.h"
#include "PrisonerDilemma/GameClass/Game.h"
#include "PrisonerDilemma/factory/StrategyFactory.h"
#include "PrisonerDilemma/strategies/config/AlwaysCooperate/AlwaysCooperate.h"
#include "PrisonerDilemma/strategies/config/AlwaysDefect/AlwaysDefect.h"
#include "PrisonerDilemma/strategies/config/GoByMajority/GoByMajority.h"
#include "PrisonerDilemma/strategies/config/ToughTitForTat/ToughTitForTat.h"
#include "PrisonerDilemma/utils/CommandLineParser/CommandLineParser.h"



int main(int argc, char* argv[]) {
    CommandLineParser parser(argc, argv);
    std::map<std::string, std::string> parsed_args = parser.parse();

    // возможные входные параметры: mode, steps, config, matrix, strategies
    int steps = 10;
    std::string mode = "detailed";
    std::string config = "";
    std::string matrix = "";
    std::vector<std::string> strategies;

    try {
        // Парсинг параметров с валидацией
        if (parsed_args.find("steps") != parsed_args.end()) {
            try {
                steps = std::stoi(parsed_args["steps"]);
                // Случай 1-3: валидация steps
                if (steps <= 0) {
                    std::cerr << "Error: Number of steps must be positive (> 0)" << std::endl;
                    return 1;
                }
            } catch (const std::invalid_argument& e) {
                // Случай 27: steps не число
                std::cerr << "Error: steps parameter must be an integer" << std::endl;
                return 1;
            } catch (const std::out_of_range& e) {
                // Случай 2: steps очень большой
                std::cerr << "Error: steps value is out of range" << std::endl;
                return 1;
            }
        }

        if (parsed_args.find("mode") != parsed_args.end()) {
            mode = parsed_args["mode"];
            // Случай 4: валидация режима
            if (mode != "detailed" && mode != "tournament" && mode != "simple") {
                std::cerr << "Error: Unknown mode '" << mode << "'. "
                          << "Use: detailed, tournament, or simple" << std::endl;
                return 1;
            }
        }

        if (parsed_args.find("config") != parsed_args.end()) {
            config = parsed_args["config"];
        }

        if (parsed_args.find("matrix") != parsed_args.end()) {
            matrix = parsed_args["matrix"];
        }

        auto factory = StrategyFactory::getInstance();

        // Регистрация стратегий
        factory->register_strategy("AlwaysCooperate",
            [] { return std::make_unique<AlwaysCooperate>(); });
        factory->register_strategy("AlwaysDefect",
            [] { return std::make_unique<AlwaysDefect>(); });
        factory->register_strategy("GoByMajority",
            [] { return std::make_unique<GoByMajority>(); });
        factory->register_strategy("ToughTitForTat",
            [] { return std::make_unique<ToughTitForTat>(); });

        Player player1(factory->create_by_name("GoByMajority"));
        Player player2(factory->create_by_name("AlwaysDefect"));
        Player player3(factory->create_by_name("AlwaysCooperate"));

        Game game(player1, player2, player3, mode);
        game.playGame(steps);

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}


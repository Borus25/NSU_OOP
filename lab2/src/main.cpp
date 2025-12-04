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

    // возможные входные параметры: mode, steps, config, matrix
    int steps = parsed_args.find("steps") != parsed_args.end() ?
                            std::stoi(parsed_args["steps"]) : 10;
    std::string mode = parsed_args.find("mode") != parsed_args.end() ?
                            parsed_args["mode"] : "detailed";
    std::string config = parsed_args.find("config") != parsed_args.end() ?
                            parsed_args["config"] : "";
    std::string matrix = parsed_args.find("matrix") != parsed_args.end() ?
                            parsed_args["matrix"] : "";


    auto factory = StrategyFactory::getInstance();

    // Короткая запись с make_unique
    factory->register_strategy("AlwaysCooperate",
        [] { return std::make_unique<AlwaysCooperate>(); });
    factory->register_strategy("AlwaysDefect",
        [] { return std::make_unique<AlwaysDefect>(); });
    factory->register_strategy("GoByMajority",
        [] { return std::make_unique<GoByMajority>(); });
    factory->register_strategy("ToughTitForTat",
        [] { return std::make_unique<ToughTitForTat>(); });

    try {
        Player player1(factory->create_by_name("GoByMajority"));
        Player player2(factory->create_by_name("AlwaysDefect"));
        Player player3(factory->create_by_name("AlwaysCooperate"));

        Game game(player1, player2, player3);
        game.playGame(steps);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}


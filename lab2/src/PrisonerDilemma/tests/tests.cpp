#include "test_runner.h"
#include "../strategies/basic_strategy/History/History.h"
#include "../Player/Player.h"
#include "../GameClass/Game.h"
#include "../utils/CommandLineParser/CommandLineParser.h"
#include "../strategies/config/AlwaysCooperate/AlwaysCooperate.h"
#include "../strategies/config/AlwaysDefect/AlwaysDefect.h"

void TestHistoryEmpty() {
    History h;
    ASSERT(h.isEmpty());
}

void TestHistoryAddChoice() {
    History h;
    h.addChoiceToHistory(COOPERATE);
    ASSERT(!h.isEmpty());
}

void TestHistoryGetLastChoice() {
    History h;
    h.addChoiceToHistory(COOPERATE);
    h.addChoiceToHistory(DEFECT);
    ASSERT_EQUAL(h.getLastChoice(), DEFECT);
}

void TestHistoryGetChoices() {
    History h;
    h.addChoiceToHistory(COOPERATE);
    h.addChoiceToHistory(DEFECT);
    h.addChoiceToHistory(COOPERATE);

    vector<Choice> choices = h.getChoices();
    ASSERT_EQUAL(choices.size(), size_t(3));
    ASSERT_EQUAL(choices[0], COOPERATE);
    ASSERT_EQUAL(choices[1], DEFECT);
    ASSERT_EQUAL(choices[2], COOPERATE);
}

void TestHistoryGetIChoice() {
    History h;
    h.addChoiceToHistory(COOPERATE);
    h.addChoiceToHistory(DEFECT);
    h.addChoiceToHistory(COOPERATE);

    ASSERT_EQUAL(h.getIChoice(0), COOPERATE);
    ASSERT_EQUAL(h.getIChoice(1), DEFECT);
    ASSERT_EQUAL(h.getIChoice(2), COOPERATE);
}

void TestPlayerConstructor() {
    auto strategy = make_unique<AlwaysCooperate>();
    Player player(move(strategy));

    ASSERT_EQUAL(player.getTotalScore(), 0);
    ASSERT_EQUAL(player.getStrategyName(), "AlwaysCooperate");
}

void TestPlayerAddScore() {
    auto strategy = make_unique<AlwaysCooperate>();
    Player player(move(strategy));

    player.addScore(5);
    ASSERT_EQUAL(player.getTotalScore(), 5);

    player.addScore(3);
    ASSERT_EQUAL(player.getTotalScore(), 8);
}

void TestPlayerUpdateHistory() {
    auto strategy = make_unique<AlwaysCooperate>();
    Player player(move(strategy));

    player.updateHistory(COOPERATE);
    player.updateHistory(DEFECT);

    const History& history = player.getHistory();
    ASSERT_EQUAL(history.getChoices().size(), size_t(2));
    ASSERT_EQUAL(history.getChoices()[0], COOPERATE);
    ASSERT_EQUAL(history.getChoices()[1], DEFECT);
}

void TestPlayerGetHistory() {
    auto strategy = make_unique<AlwaysCooperate>();
    Player player(move(strategy));

    player.updateHistory(COOPERATE);
    player.updateHistory(COOPERATE);

    const History& history = player.getHistory();
    ASSERT(!history.isEmpty());
    ASSERT_EQUAL(history.getLastChoice(), COOPERATE);
}

void TestPlayerMakeChoice() {
    auto strategy = make_unique<AlwaysCooperate>();
    Player player(move(strategy));

    History opponentHistory1;
    History opponentHistory2;

    Choice choice = player.makeChoice(opponentHistory1, opponentHistory2);
    ASSERT_EQUAL(choice, COOPERATE);
}

void TestPlayerReset() {
    auto strategy = make_unique<AlwaysCooperate>();
    Player player(move(strategy));

    player.addScore(100);
    player.updateHistory(DEFECT);
    player.updateHistory(COOPERATE);

    ASSERT_EQUAL(player.getTotalScore(), 100);
    ASSERT_EQUAL(player.getHistory().getChoices().size(), size_t(2));

    player.reset();

    ASSERT_EQUAL(player.getTotalScore(), 0);
    ASSERT(player.getHistory().isEmpty());
}

void TestGameConstructor() {
    auto strategy1 = make_unique<AlwaysCooperate>();
    auto strategy2 = make_unique<AlwaysDefect>();
    auto strategy3 = make_unique<AlwaysCooperate>();

    Player p1(move(strategy1));
    Player p2(move(strategy2));
    Player p3(move(strategy3));

    Game game(p1, p2, p3, "detailed");

    vector<long long> scores = game.getScores();
    ASSERT_EQUAL(scores.size(), size_t(3));
    ASSERT_EQUAL(scores[0], 0LL);
    ASSERT_EQUAL(scores[1], 0LL);
    ASSERT_EQUAL(scores[2], 0LL);
}

void TestGamePlayRound() {
    auto strategy1 = make_unique<AlwaysCooperate>();
    auto strategy2 = make_unique<AlwaysCooperate>();
    auto strategy3 = make_unique<AlwaysCooperate>();

    Player p1(move(strategy1));
    Player p2(move(strategy2));
    Player p3(move(strategy3));

    Game game(p1, p2, p3);
    game.playRound();

    vector<long long> scores = game.getScores();
    ASSERT_EQUAL(scores[0], 7LL);
    ASSERT_EQUAL(scores[1], 7LL);
    ASSERT_EQUAL(scores[2], 7LL);
}

void TestGamePlayRound_DDD() {
    auto strategy1 = make_unique<AlwaysDefect>();
    auto strategy2 = make_unique<AlwaysDefect>();
    auto strategy3 = make_unique<AlwaysDefect>();

    Player p1(move(strategy1));
    Player p2(move(strategy2));
    Player p3(move(strategy3));

    Game game(p1, p2, p3);
    game.playRound();

    vector<long long> scores = game.getScores();
    ASSERT_EQUAL(scores[0], 1LL);
    ASSERT_EQUAL(scores[1], 1LL);
    ASSERT_EQUAL(scores[2], 1LL);
}

void TestGamePlayRound_Mixed() {
    auto strategy1 = make_unique<AlwaysCooperate>();
    auto strategy2 = make_unique<AlwaysCooperate>();
    auto strategy3 = make_unique<AlwaysDefect>();

    Player p1(move(strategy1));
    Player p2(move(strategy2));
    Player p3(move(strategy3));

    Game game(p1, p2, p3);
    game.playRound();

    vector<long long> scores = game.getScores();
    ASSERT_EQUAL(scores[0], 3LL);
    ASSERT_EQUAL(scores[1], 3LL);
    ASSERT_EQUAL(scores[2], 9LL);
}

void TestGamePlayMultipleRounds() {
    auto strategy1 = make_unique<AlwaysCooperate>();
    auto strategy2 = make_unique<AlwaysCooperate>();
    auto strategy3 = make_unique<AlwaysCooperate>();

    Player p1(move(strategy1));
    Player p2(move(strategy2));
    Player p3(move(strategy3));

    Game game(p1, p2, p3);
    game.playGame(5);

    vector<long long> scores = game.getScores();
    // 5 раундов по 7 баллов каждому
    ASSERT_EQUAL(scores[0], 35LL);
    ASSERT_EQUAL(scores[1], 35LL);
    ASSERT_EQUAL(scores[2], 35LL);
}

void TestGameGetScores() {
    auto strategy1 = make_unique<AlwaysCooperate>();
    auto strategy2 = make_unique<AlwaysDefect>();
    auto strategy3 = make_unique<AlwaysCooperate>();

    Player p1(move(strategy1));
    Player p2(move(strategy2));
    Player p3(move(strategy3));

    Game game(p1, p2, p3);

    vector<long long> scores = game.getScores();
    ASSERT_EQUAL(scores.size(), size_t(3));
    ASSERT_EQUAL(scores[0], 0LL);
    ASSERT_EQUAL(scores[1], 0LL);
    ASSERT_EQUAL(scores[2], 0LL);
}

void TestGameGetWinner_AllEqual() {
    auto strategy1 = make_unique<AlwaysCooperate>();
    auto strategy2 = make_unique<AlwaysCooperate>();
    auto strategy3 = make_unique<AlwaysCooperate>();

    Player p1(move(strategy1));
    Player p2(move(strategy2));
    Player p3(move(strategy3));

    Game game(p1, p2, p3);
    game.playRound();

    // Все получают по 7, победитель первый с индексом 0
    int winner = game.getWinner();
    ASSERT_EQUAL(winner, 0);
}

void TestGameGetWinner_SecondPlayer() {
    auto strategy1 = make_unique<AlwaysDefect>();
    auto strategy2 = make_unique<AlwaysDefect>();
    auto strategy3 = make_unique<AlwaysDefect>();

    Player p1(move(strategy1));
    Player p2(move(strategy2));
    Player p3(move(strategy3));

    Game game(p1, p2, p3);
    game.playRound();

    p2.addScore(10);  // Добавляем баллы второму игроку

    int winner = game.getWinner();
    ASSERT_EQUAL(winner, 1);
}

void TestGameReset() {
    auto strategy1 = make_unique<AlwaysCooperate>();
    auto strategy2 = make_unique<AlwaysCooperate>();
    auto strategy3 = make_unique<AlwaysCooperate>();

    Player p1(move(strategy1));
    Player p2(move(strategy2));
    Player p3(move(strategy3));

    Game game(p1, p2, p3);
    game.playGame(3);

    vector<long long> scoresAfterGame = game.getScores();
    ASSERT_EQUAL(scoresAfterGame[0], 21LL);  // 3 * 7

    game.reset();

    vector<long long> scoresAfterReset = game.getScores();
    ASSERT_EQUAL(scoresAfterReset[0], 0LL);
    ASSERT_EQUAL(scoresAfterReset[1], 0LL);
    ASSERT_EQUAL(scoresAfterReset[2], 0LL);
}

// ==================== COMMANDLINEPARSER TESTS ====================

void TestCommandLineParserEmpty() {
    const char* argv[] = {"program"};
    int argc = 1;

    CommandLineParser parser(argc, (char**)argv);
    auto result = parser.parse();

    ASSERT_EQUAL(result.empty(), true);
}

void TestCommandLineParserSingleKey() {
    const char* argv[] = {"program", "--mode", "detailed"};
    int argc = 3;

    CommandLineParser parser(argc, (char**)argv);
    auto result = parser.parse();

    ASSERT(result.find("mode") != result.end());
    ASSERT_EQUAL(result["mode"], "detailed");
}

void TestCommandLineParserMultipleKeys() {
    const char* argv[] = {"program", "--mode", "tournament", "--steps", "100"};
    int argc = 5;

    CommandLineParser parser(argc, (char**)argv);
    auto result = parser.parse();

    ASSERT_EQUAL(result["mode"], "tournament");
    ASSERT_EQUAL(result["steps"], "100");
}

void TestCommandLineParserKeyWithEquals() {
    const char* argv[] = {"program", "--mode=detailed", "--steps=50"};
    int argc = 3;

    CommandLineParser parser(argc, (char**)argv);
    auto result = parser.parse();

    ASSERT_EQUAL(result["mode"], "detailed");
    ASSERT_EQUAL(result["steps"], "50");
}

void TestCommandLineParserMixedFormats() {
    const char* argv[] = {"program", "--mode", "detailed", "--steps=100"};
    int argc = 4;

    CommandLineParser parser(argc, (char**)argv);
    auto result = parser.parse();

    ASSERT_EQUAL(result["mode"], "detailed");
    ASSERT_EQUAL(result["steps"], "100");
}

void TestCommandLineParserShortKey() {
    const char* argv[] = {"program", "-m", "detailed"};
    int argc = 3;

    CommandLineParser parser(argc, (char**)argv);
    auto result = parser.parse();

    ASSERT_EQUAL(result["m"], "detailed");
}

void TestCommandLineParserKeyWithoutValue() {
    const char* argv[] = {"program", "--flag"};
    int argc = 2;

    CommandLineParser parser(argc, (char**)argv);
    auto result = parser.parse();

    ASSERT_EQUAL(result["flag"], "true");
}

void TestCommandLineParserPositionalArgs() {
    const char* argv[] = {"program", "value1", "value2"};
    int argc = 3;

    CommandLineParser parser(argc, (char**)argv);
    auto result = parser.parse();

    ASSERT_EQUAL(result["arg1"], "value1");
    ASSERT_EQUAL(result["arg2"], "value2");
}

// ==================== MAIN ====================

int main() {
    TestRunner tr;

    // History tests
    RUN_TEST(tr, TestHistoryEmpty);
    RUN_TEST(tr, TestHistoryAddChoice);
    RUN_TEST(tr, TestHistoryGetLastChoice);
    RUN_TEST(tr, TestHistoryGetChoices);
    RUN_TEST(tr, TestHistoryGetIChoice);

    // Player tests
    RUN_TEST(tr, TestPlayerConstructor);
    RUN_TEST(tr, TestPlayerAddScore);
    RUN_TEST(tr, TestPlayerUpdateHistory);
    RUN_TEST(tr, TestPlayerGetHistory);
    RUN_TEST(tr, TestPlayerMakeChoice);
    RUN_TEST(tr, TestPlayerReset);


    // Game tests
    RUN_TEST(tr, TestGameConstructor);
    RUN_TEST(tr, TestGamePlayRound);
    RUN_TEST(tr, TestGamePlayRound_DDD);
    RUN_TEST(tr, TestGamePlayRound_Mixed);
    RUN_TEST(tr, TestGamePlayMultipleRounds);
    RUN_TEST(tr, TestGameGetScores);
    RUN_TEST(tr, TestGameGetWinner_AllEqual);
    RUN_TEST(tr, TestGameGetWinner_SecondPlayer);
    RUN_TEST(tr, TestGameReset);

    // CommandLineParser tests
    RUN_TEST(tr, TestCommandLineParserEmpty);
    RUN_TEST(tr, TestCommandLineParserSingleKey);
    RUN_TEST(tr, TestCommandLineParserMultipleKeys);
    RUN_TEST(tr, TestCommandLineParserKeyWithEquals);
    RUN_TEST(tr, TestCommandLineParserMixedFormats);
    RUN_TEST(tr, TestCommandLineParserShortKey);
    RUN_TEST(tr, TestCommandLineParserKeyWithoutValue);
    RUN_TEST(tr, TestCommandLineParserPositionalArgs);

    return 0;
}


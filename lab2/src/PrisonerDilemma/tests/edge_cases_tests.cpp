#include "test_runner.h"
#include "../strategies/basic_strategy/History/History.h"
#include "../Player/Player.h"
#include "../GameClass/Game.h"
#include "../utils/CommandLineParser/CommandLineParser.h"
#include "../strategies/config/AlwaysCooperate/AlwaysCooperate.h"
#include "../strategies/config/AlwaysDefect/AlwaysDefect.h"

// ==================== EDGE CASES TESTS ====================

// ==================== ИСТОРИЯ ====================

void TestHistoryGetLastChoiceEmpty() {
    // Случай 11: getLastChoice() на пустой истории
    History h;

    try {
        Choice c = h.getLastChoice();
        ASSERT(false);  // Не должно достигнуть здесь
    } catch (const std::out_of_range& e) {
        ASSERT(true);  // Ожидаемо выброшено исключение
    }
}

void TestHistoryGetIChoiceOutOfRange() {
    // Случай 12: getIChoice() с неверным индексом
    History h;
    h.addChoiceToHistory(COOPERATE);
    h.addChoiceToHistory(DEFECT);

    try {
        [[maybe_unused]] Choice c = h.getIChoice(100);  // Индекс вне диапазона
        ASSERT(false);  // Не должно достигнуть здесь
    } catch (const std::out_of_range& e) {
        ASSERT(true);  // Ожидаемо выброшено исключение
    }
}

// ==================== PLAYER ====================

void TestPlayerAddScoreNegative() {
    // Случай 14: отрицательные очки
    auto strategy = make_unique<AlwaysCooperate>();
    Player player(move(strategy));

    try {
        player.addScore(-10);
        ASSERT(false);  // Не должно достигнуть здесь
    } catch (const std::invalid_argument& e) {
        ASSERT(true);  // Ожидаемо выброшено исключение
    }
}

void TestPlayerConstructorWithNullptr() {
    // Случай 16: Player с nullptr стратегией
    std::unique_ptr<Strategy> nullStrategy = nullptr;

    try {
        Player player(move(nullStrategy));
        ASSERT(false);  // Не должно достигнуть здесь
    } catch (const std::invalid_argument& e) {
        ASSERT(true);  // Ожидаемо выброшено исключение
    }
}

void TestPlayerLargeScore() {
    // Случай 15: проверка что long long работает с большими числами
    auto strategy = make_unique<AlwaysCooperate>();
    Player player(move(strategy));

    // Добавляем очень большое значение
    player.addScore(1000000000);  // 1 миллиард
    player.addScore(1000000000);  // Еще 1 миллиард

    long long expectedScore = 2000000000LL;
    ASSERT_EQUAL(player.getTotalScore(), expectedScore);
    // Если бы использовался int, переполнилось бы
}

// ==================== GAME ====================

void TestGameSamePlayerObjects() {
    // Случай 26: Game с одинаковыми Player объектами
    auto strategy1 = make_unique<AlwaysCooperate>();
    auto strategy2 = make_unique<AlwaysDefect>();

    Player p1(move(strategy1));
    Player p2(move(strategy2));

    try {
        Game game(p1, p1, p1);  // Все одинаковые
        ASSERT(false);  // Не должно достигнуть здесь
    } catch (const std::invalid_argument& e) {
        ASSERT(true);  // Ожидаемо выброшено исключение
    }
}

void TestGameDifferentPlayers() {
    // Противоположный тест: Game с разными Player объектами должен работать
    auto strategy1 = make_unique<AlwaysCooperate>();
    auto strategy2 = make_unique<AlwaysDefect>();
    auto strategy3 = make_unique<AlwaysCooperate>();

    Player p1(move(strategy1));
    Player p2(move(strategy2));
    Player p3(move(strategy3));

    try {
        Game game(p1, p2, p3);
        ASSERT(true);  // Должно успешно создаться
    } catch (const std::exception& e) {
        ASSERT(false);  // Не должно быть исключения
    }
}

void TestGameWinnerTiebreaker() {
    // Случай 25: поведение при ничьей (одинаковые очки)
    auto strategy1 = make_unique<AlwaysCooperate>();
    auto strategy2 = make_unique<AlwaysCooperate>();
    auto strategy3 = make_unique<AlwaysCooperate>();

    Player p1(move(strategy1));
    Player p2(move(strategy2));
    Player p3(move(strategy3));

    Game game(p1, p2, p3);
    game.playRound();  // Все получают 7

    // При ничьей должен вернуться первый игрок (индекс 0)
    int winner = game.getWinner();
    ASSERT_EQUAL(winner, 0);
}

// ==================== COMMANDLINEPARSER ====================

void TestCommandLineParserEmptyValue() {
    // Случай 8: ключ с пустым значением (--mode=)
    const char* argv[] = {"program", "--mode="};
    int argc = 2;

    CommandLineParser parser(argc, (char**)argv);
    auto result = parser.parse();

    // Пустое значение должно быть заменено на "true"
    ASSERT_EQUAL(result["mode"], "true");
}

// ==================== MAIN ====================

int main() {
    TestRunner tr;

    // Edge Cases - History
    RUN_TEST(tr, TestHistoryGetLastChoiceEmpty);
    RUN_TEST(tr, TestHistoryGetIChoiceOutOfRange);

    // Edge Cases - Player
    RUN_TEST(tr, TestPlayerAddScoreNegative);
    RUN_TEST(tr, TestPlayerConstructorWithNullptr);
    RUN_TEST(tr, TestPlayerLargeScore);

    // Edge Cases - Game
    RUN_TEST(tr, TestGameSamePlayerObjects);
    RUN_TEST(tr, TestGameDifferentPlayers);
    RUN_TEST(tr, TestGameWinnerTiebreaker);

    // Edge Cases - CommandLineParser
    RUN_TEST(tr, TestCommandLineParserEmptyValue);

    return 0;
}


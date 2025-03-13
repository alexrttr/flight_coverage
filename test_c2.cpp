#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <fstream>
#include <sstream>
#include <cstdlib>

void run_program() {
    std::system("./forest_fire");
}

void create_input(const std::string& content) {
    std::ofstream input("INPUT");
    input << content;
}

std::string read_output() {
    std::ifstream output("OUTPUT");
    std::stringstream ss;
    ss << output.rdbuf();
    return ss.str();
}

TEST_CASE("Input validation") {
    SECTION("Invalid L value (non-integer)") {
        create_input("5.5\n1\n0 0 1 1");
        run_program();
        REQUIRE(read_output() == "ERROR\n");
    }

    SECTION("Invalid N value (too large)") {
        create_input("100\n101\n0 0 1 1");
        run_program();
        REQUIRE(read_output() == "ERROR\n");
    }

    SECTION("Identical airplane points") {
        create_input("100\n1\n5 5 5 5");
        run_program();
        REQUIRE(read_output() == "ERROR\n");
    }
}

TEST_CASE("Full coverage cases") {
    SECTION("Single central horizontal line") {
        // A horizontal flight at y = 50 in a 100 km square covers the entire area.
        create_input("100\n1\n0 50 100 50");
        run_program();
        REQUIRE(read_output() == "OK\n");
    }

    SECTION("Two diagonal flights covering square") {
        // Two diagonal flights from (0,0)->(100,100) and (0,100)->(100,0)
        // result in full coverage of the square.
        create_input("100\n2\n0 0 100 100\n0 100 100 0");
        run_program();
        REQUIRE(read_output() == "OK\n");
    }
}

TEST_CASE("Partial coverage cases") {
    SECTION("Single bottom edge line") {
        // A flight along the bottom edge (from (0,0) to (100,0))
        // covers only points with y <= 50, so uncovered points should have y > 50.
        create_input("100\n1\n0 0 100 0");
        run_program();
        auto output = read_output();

        // Since part is uncovered, output should not be "OK"
        REQUIRE(output.find("OK") == std::string::npos);
        
        double x, y;
        int count = std::sscanf(output.c_str(), "%lf %lf", &x, &y);
        REQUIRE(count == 2);

        // The uncovered point should lie within the forest boundaries.
        REQUIRE(x >= 0.0);
        REQUIRE(x <= 100.0);
        REQUIRE(y >= 0.0);
        REQUIRE(y <= 100.0);

        // In this case, uncovered region should be above y=50.
        REQUIRE(y > 50.0);
    }
}

TEST_CASE("Precision requirements") {
    SECTION("Coordinate formatting with three decimals") {
        create_input("100\n1\n0 0 100 0");
        run_program();
        auto output = read_output();
        REQUIRE(output.find('.') != std::string::npos);
        REQUIRE(output.size() >= 8);
    }
}

TEST_CASE("Edge cases") {
    SECTION("Minimum square size") {
        // 1 km square with a horizontal flight through the center covers the area.
        create_input("1\n1\n0 0.5 1 0.5");
        run_program();
        REQUIRE(read_output() == "OK\n");
    }

    SECTION("Maximum airplanes with identical diagonal flights") {
        // With 100 identical diagonal flights, the coverage is the same as one diagonal.
        // For a diagonal flight from (0,0) to (100,100), some regions (e.g. near (100,0))
        // remain uncovered.
        std::stringstream ss;
        ss << "100\n100\n";
        for (int i = 0; i < 100; i++) {
            ss << "0 0 100 100\n";
        }
        create_input(ss.str());
        run_program();
        auto output = read_output();
        REQUIRE(output.find("OK") == std::string::npos);
        
        double x, y;
        int count = std::sscanf(output.c_str(), "%lf %lf", &x, &y);
        REQUIRE(count == 2);

        // Check that the uncovered point lies within the forest boundaries.
        REQUIRE(x >= 0.0);
        REQUIRE(x <= 100.0);
        REQUIRE(y >= 0.0);
        REQUIRE(y <= 100.0);
    }
}


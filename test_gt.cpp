#include <gtest/gtest.h>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <string>

// Helper function: writes input to "INPUT", runs the executable "forest_fire",
// reads the result from "OUTPUT", and returns it.
std::string run_forest_fire_task(const std::string& input_data) {
    std::ofstream input_file("INPUT");
    input_file << input_data;
    input_file.close();
    system("./forest_fire");
    std::ifstream output_file("OUTPUT");
    std::string result;
    std::getline(output_file, result);
    output_file.close();
    return result;
}

TEST(ForestFireTest, FullCoverage) {
    // A flight that crosses horizontally at y=50 in a 100 km square covers every point.
    std::string input = "100\n1\n0 50 100 50\n";
    std::string result = run_forest_fire_task(input);
    EXPECT_EQ(result, "OK");
}

TEST(ForestFireTest, PartialCoverage) {
    // A flight along the bottom edge (from (0,0) to (100,0)) covers only points with y<=50.
    // The upper region remains uncovered.
    std::string input = "100\n1\n0 0 100 0\n";
    std::string result = run_forest_fire_task(input);

    // Since part of the square is not covered, we expect a pair of coordinates.
    EXPECT_NE(result, "OK");
    double x, y;
    int count = sscanf(result.c_str(), "%lf %lf", &x, &y);
    EXPECT_EQ(count, 2);
}

TEST(ForestFireTest, EdgeBoundaryCoverage) {
    // Two flights with endpoints outside the square but crossing it:
    // One horizontal flight through y=50 and one vertical flight through x=50.
    // Their combined coverage should cover the entire square.
    std::string input = "100\n2\n-50 50 150 50\n50 -50 50 150\n";
    std::string result = run_forest_fire_task(input);
    EXPECT_EQ(result, "OK");
}

TEST(ForestFireTest, LargeForest) {
    // For a 1000 km square, these five flights do not cover the entire area.
    // Expected: an uncovered point is reported (output is not "OK").
    std::string input = 
        "1000\n5\n"
        "0 500 1000 500\n"   // horizontal at y=500 (covers y in [450,550])
        "500 0 500 1000\n"   // vertical at x=500 (covers x in [450,550])
        "100 100 900 100\n"  // horizontal at y=100 (covers y in [50,150])
        "100 900 900 900\n"  // horizontal at y=900 (covers y in [850,950])
        "100 100 100 900\n"; // vertical at x=100 (covers x in [50,150])
    std::string result = run_forest_fire_task(input);
    EXPECT_NE(result, "OK");
    double x, y;
    int count = sscanf(result.c_str(), "%lf %lf", &x, &y);
    EXPECT_EQ(count, 2);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


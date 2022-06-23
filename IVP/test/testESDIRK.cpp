#include<fstream>
#include<string>
#include"../include/json.hpp"
#include"../include/IncAllMethod.h"
#include"catch.hpp"

using json = nlohmann::json;
using vec6Func = std::function<Vec<real,6>(Vec<real,6>,real)>;

TEST_CASE("ESDIRK", "[ESDIRK]")
{
    json j;
    std::ifstream jfile("./test/inputfile/condition.json");
    jfile >> j;
    jfile.close();

    std::ofstream outfile("./output/testESDIRK.txt", std::ios::app);
    outfile << "!!! ESDIRK !!!" << std::endl;

    Vec<real,6> U0_1{0.994, 0, 0, 0, -2.0015851063790825224, 0};
    real T1 = 17.06521656015796;
    Vec<real,6> U0_2{0.87978, 0, 0, 0, -0.3797, 0};
    real T2 = 19.14045706162071;
    vec6Func f = [](Vec<real,6> u, real t){
        real mu = 1.0/81.45;
        real f1 = u[3];
        real f2 = u[4];
        real f3 = u[5];
        real f4 = 2*u[4]+u[0] - (mu*(u[0]+mu-1))/pow((pow(u[1],2)+pow(u[2],2)+pow(u[0]+mu-1,2)),1.5)
                  - ((1-mu)*(u[0]+mu))/pow((pow(u[1],2)+pow(u[2],2)+pow(u[0]+mu,2)),1.5);
        real f5 = -2*u[3]+u[1] - (mu*u[1])/pow((pow(u[1],2)+pow(u[2],2)+pow(u[0]+mu-1,2)),1.5)
                  - ((1-mu)*u[1])/pow((pow(u[1],2)+pow(u[2],2)+pow(u[0]+mu,2)),1.5);
        real f6 = -(mu*u[2])/pow((pow(u[1],2)+pow(u[2],2)+pow(u[0]+mu-1,2)),1.5)
                  -((1-mu)*u[2])/pow((pow(u[1],2)+pow(u[2],2)+pow(u[0]+mu,2)),1.5);
        return Vec<real,6>{f1,f2,f3,f4,f5,f6};
    };

    SECTION("test for (10.198)")
    {
        outfile << "Test for (10.198): " << std::endl;
        SECTION("For T1"){
            clock_t begin, end;
            int steps = static_cast<int>(j["ESDIRK"]["steps"]);
            outfile << "For T1, " << std::endl;
            for(int i=1; i<4; i++){
                begin = clock();
                ESDIRKsolver<6> esdirkMethod(pow(2,i-1)*steps);
                auto res = esdirkMethod.solve(U0_1, f, T1);
                end = clock();
                outfile << "When steps = " << pow(2,i-1)*steps << ", result = " << res;
                auto solution_err = norm(res-U0_1, 0);
                outfile << "; solution error is: " << solution_err;
                outfile << "; CPU time: " << (end-begin)*1.0/CLOCKS_PER_SEC << std::endl;
            }
        }
    }

    SECTION("test for (10.199)")
    {
        outfile << "Test for (10.199): " << std::endl;
        SECTION("For T2"){
            clock_t begin, end;
            int steps = static_cast<int>(j["ESDIRK"]["steps"]);
            outfile << "For T2, " << std::endl;
            for(int i=1; i<4; i++){
                begin = clock();
                ESDIRKsolver<6> esdirkMethod(pow(2,i-1)*steps);
                auto res = esdirkMethod.solve(U0_2, f, T2);
                end = clock();
                outfile << "When steps = " << pow(2,i-1)*steps << ", result = " << res;
                auto solution_err = esdirkMethod.solution_error(U0_2, f, T2);
                outfile << "; solution error is: " << solution_err;
                outfile << "; CPU time: " << (end-begin)*1.0/CLOCKS_PER_SEC << std::endl;
            }
        }
    }

    outfile.close();
}
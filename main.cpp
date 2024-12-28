#include "BlackScholesModel.h"
#include "CallOption.h"
#include "PutOption.h"
#include "BarrierOption.h"
#include "AsianOption.h"
#include <iostream>
#include <memory>
#include <vector>

void displayMenu() {
    std::cout << "\n--- Menu ---\n\n";
    std::cout << "0 - End Programm\n";
    std::cout << '\n';
    std::cout << "European Option\n";
    std::cout << "---------------\n";
    std::cout << "1 - Call\n";
    std::cout << "2 - Put\n";
    std::cout << '\n';
    std::cout << "Barrrier Option\n";
    std::cout << "---------------\n";
    std::cout << "3 - Up-and-Out\n";
    std::cout << "4 - Down-and-Out\n";
    std::cout << "5 - Up-and-In\n";
    std::cout << "6 - Down-and-In\n";
    std::cout << '\n';
    std::cout << "Asian Option\n";
    std::cout << "------------\n";
    std::cout << "7 - Call\n";
    std::cout << "8 - Put\n";
    std::cout << '\n';
}

int main() {

    double spot, rate, volatility, dividend;
    std::cout << '\n';
    std::cout << "OPTION PRICER\n";
    std::cout << "-------------\n\n";
    std ::cout<< "MARI M.\nDONATI ARRANZ M.\nCHAIGNE T.\n";


    while (true) {

        // Parametres generaux

        std::cout << "\n*************************";
        std::cout << "\n\n";
        std::cout << "--- Option Parameters ---\n";
        std::cout << "\nSpot price\n    S0 = ";
        std::cin >> spot;
        std::cout << "\nRisk-free rate\n    r = ";
        std::cin >> rate;
        std::cout << "\nVolatility\n    sigma = ";
        std::cin >> volatility;
        std::cout << "\nDividend yield\n    q = ";
        std::cin >> dividend;

        BlackScholesModel model(spot, rate, volatility, dividend);

        // Choix du type d'option 

        displayMenu();
        
        int choice{-1};
        std::cout << "Choose option type: ";
        std::cin >> choice; 

        if (choice == 0) {
            std::cout << "\nEND\n";
            break;
        }

        // Parametres generaux (suite)

        double strike, maturity;
        std::cout << '\n';
        std::cout << "Strike price\n    K = ";
        std::cin >> strike;
        std::cout << '\n';
        std::cout << "Maturity in years\n    T = ";
        std::cin >> maturity;


        // Resultats

        int numPaths = 10000;  // Nombre de simulations Monte-Carlo
        int steps = 100;       // Nombre de pas temporels

        if (choice == 1) {
            // Call Option
            std::cout << "\nComputing..." << std::flush;
            CallOption callOption(strike, maturity);
            double price = model.priceAnalytic(&callOption, true);
            double hedgeCost = callOption.hedgeCost(model, steps);
            std::cout << "\n\n--- Results ---\n\n";
            std::cout << "Option price..........." << price << "\n";
            std::cout << "Hedge cost............." << hedgeCost << "\n";

        } else if (choice == 2) {
            // Put Option
            std::cout << "\nComputing..." << std::flush;
            PutOption putOption(strike, maturity);
            double price = model.priceAnalytic(&putOption, false);
            double hedgeCost = putOption.hedgeCost(model, steps);
            std::cout << "\n\n--- Results ---\n\n";
            std::cout << "Option price..........." << price << "\n";
            std::cout << "Hedge cost............." << hedgeCost << "\n";

        } else if (choice >= 3 && choice <= 6) {
            // Barrier Options
            double barrier;
            std::cout << '\n';
            std::cout << "Barrier level:\n    B = ";
            std::cin >> barrier;

            // Demander le type de l'option (Call ou Put)
            OptionType optionType;
            int optionTypeChoice;
            std::cout << "\nOption type ? Call = 1 ; Put = 2\n";
            std::cin >> optionTypeChoice;
            std::cout << "\nComputing..." << std::flush;
            if (optionTypeChoice == 1) {
                optionType = OptionType::Call;
            } else if (optionTypeChoice == 2) {
                optionType = OptionType::Put;
            } else {
                std::cout << "Invalid input\n";
                continue;
            }

            BarrierType barrierType;
            switch (choice) {
                case 3:
                    barrierType = BarrierType::UpAndOut;
                    break;
                case 4:
                    barrierType = BarrierType::DownAndOut;
                    break;
                case 5:
                    barrierType = BarrierType::UpAndIn;
                    break;
                case 6:
                    barrierType = BarrierType::DownAndIn;
                    break;
            }

            BarrierOption barrierOption(strike, maturity, barrier, barrierType, optionType);
            double price = barrierOption.price(model, numPaths, steps);
            double hedgeCost = barrierOption.hedgeCost(model,steps);
            std::cout << "\n\n--- Results ---\n\n";
            std::cout << "Option price..........." << price << "\n";
            std::cout << "Hedge cost............." << hedgeCost << "\n";

        } else if (choice == 7 || choice == 8) {
            // Asian Options
            std::cout << "\nComputing..." << std::flush;
            OptionType optionType = (choice == 7) ? OptionType::Call : OptionType::Put;

            AsianOption asianOption(strike, maturity, optionType);
            double price = asianOption.price(model, numPaths, steps);
            double hedgeCost = asianOption.hedgeCost(model, steps);
            std::cout << "\n\n--- Results ---\n\n";
            std::cout << "Option price..........." << price << "\n";
            std::cout << "Hedge cost............." << hedgeCost << "\n";

        } else {
            std::cout << "Invalid input\n";
        }
    }

    return 0;
}

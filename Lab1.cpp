#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>

class Game {
public:
    virtual void play() = 0; 
    virtual ~Game() = default;
};

class Hamurabi : public Game {
private:
    int year;
    int population;
    int acres;
    int bushels;
    int landPrice;

    const std::string saveFileName = "savegame.txt";

    // Вспомогательные функции
    int getUserInput(const std::string& prompt) {
        int value;
        while (true) {
            std::cout << prompt;
            if (std::cin >> value && value >= 0) {
                return value;
            } else {
                std::cout << "Пожалуйста, введите корректное неотрицательное число.\n";
                std::cin.clear();
                std::cin.ignore(10000, '\n');
            }
        }
    }

    int randomInRange(int min, int max) {
        return min + rand() % (max - min + 1);
    }

    void printStatus() {
        std::cout << "\nГод " << year << "\n";
        std::cout << "Население: " << population << "\n";
        std::cout << "Пашни: " << acres << " акров\n";
        std::cout << "Запасы зерна: " << bushels << " бушелей\n";
        std::cout << "Стоимость земли: " << landPrice << " бушелей за акр\n\n";
    }

    void save() {
        std::ofstream ofs(saveFileName);
        if (ofs) {
            ofs << year << "\n"
                << population << "\n"
                << acres << "\n"
                << bushels << "\n"
                << landPrice << "\n";
            std::cout << "Игра сохранена.\n";
        } else {
            std::cout << "Ошибка при сохранении игры.\n";
        }
    }

    bool load() {
        std::ifstream ifs(saveFileName);
        if (ifs) {
            ifs >> year >> population >> acres >> bushels >> landPrice;
            std::cout << "Загружена сохранённая игра.\n";
            return true;
        }
        return false;
    }

    bool saveExists() {
        std::ifstream ifs(saveFileName);
        return static_cast<bool>(ifs);
    }

public:
    Hamurabi() {
        year = 1;
        population = 100;
        acres = 1000;
        bushels = 2800;
        landPrice = 20;
        srand(static_cast<unsigned int>(time(nullptr)));
    }

    void play() override {
        std::cout << "Добро пожаловать в игру Хаммураби!\n";

        
        if (saveExists()) {
            std::cout << "Обнаружена сохранённая игра.\n";
            std::cout << "Хотите продолжить или начать новую игру? (y/n): ";
            char choice;
            std::cin >> choice;
            if ( choice == 'y' || choice == 'Y') {
                load();
            } else {
                std::remove(saveFileName.c_str()); 
                std::cout << "Начинаем новую игру.\n";
            }
        }

        const int max_years = 10;

        for (; year <= max_years; ++year) {
            printStatus();

           
            std::cout << "Хотите сохранить игру и выйти или продолжить(y/n): ";
            char saveChoice;
            std::cin >> saveChoice;
            if (saveChoice == 'y' || saveChoice == 'Y') {
                save();
                break;
            }
            else
            {
                
            }

            
            landPrice = randomInRange(10, 30);


          
            int landToBuy = getUserInput("Сколько акров земли хотите купить? ");

            while (landToBuy < 0) {
                std::cout << "Неверное значение\n";
                landToBuy = getUserInput("Сколько акров земли хотите купить? ");
            }

            while (landToBuy*landPrice > bushels)  {
                std::cout << "Не хватает пшеницы\n";
                landToBuy = getUserInput("Сколько акров земли хотите купить? ");
            }

            
            int landToSell = getUserInput("Сколько акров земли хотите продать? ");
            
            while (landToSell < 0) {
                std::cout << "Неверное значение\n";
                landToBuy = getUserInput("Сколько акров земли хотите продать? ");
            }


            while (landToSell > acres) {
                std::cout << "У вас нет столько земли для продажи.\n";
                landToSell = getUserInput("Сколько акров земли хотите продать? ");
            }

            acres += landToBuy - landToSell;
            bushels -= landToBuy * landPrice;
            bushels += landToSell * landPrice;

            
            int food = getUserInput("Сколько бушелей зерна хотите потратить на еду? ");
            while (food > bushels) {
                std::cout << "Недостаточно зерна!\n";
                food = getUserInput("Сколько бушелей зерна хотите потратить на еду? ");
            }
            bushels -= food;

            
            int seed = getUserInput("Сколько бушелей зерна хотите посеять? ");
            while (seed > bushels) {
                std::cout << "Недостаточно зерна для посева!\n";
                seed = getUserInput("Сколько бушелей зерна хотите посеять? ");
            }
            if (seed / 2 > population * acres) {
                std::cout << "Недостаточно населения или земли для такого посева! Посев сокращён.\n";
                seed = population * acres * 2;
            }
            bushels -= seed;

            std::cout << std::endl;
            int peopleFed = food / 20;
            int starved = (peopleFed >= population) ? 0 : (population - peopleFed);
            if (starved > 0) {
                std::cout << starved << " человек умерли от голода.\n";
            } else {
                std::cout << "Голодом никто не умер.\n";
            }
            population -= starved;

            if (population <= 0) {
                std::cout << "Все население умерло. Игра окончена.\n";
                std::remove(saveFileName.c_str()); 
                break;
            }

            int harvestPerAcre = randomInRange(1, 6);
            int harvest = (seed / 2) * harvestPerAcre;
            bushels += harvest;
            std::cout << "С урожая собрано " << harvest << " бушелей зерна (" << harvestPerAcre << " бушелей с акра).\n";

            int ratsEat = (bushels > 0) ? randomInRange(0, bushels / 10) : 0;
            bushels -= ratsEat;
            std::cout << "Крысы съели " << ratsEat << " бушелей зерна.\n";

            int immigrants = (bushels > 0) ? randomInRange(0, 10) : 0;
            population += immigrants;
            std::cout << "Пришло " << immigrants << " новых жителей.\n";

            std::cout << "Конец года " << year << ".\n\n";
        }


        if (year > max_years) {
            std::cout << "Игра завершена по окончании 10 лет.\n";
            std::remove(saveFileName.c_str()); 
        }
    }
};

int main() {
    Hamurabi game;
    game.play();
    return 0;
}
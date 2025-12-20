
class GameState final
{

public:

    GameState();

    struct ResourcesToUse
    {
        int landToBuy;
        int landToSell;
        int grainToPlant;
        int grainToEat;
    };

    void getReport() const;

    ::GameState::ResourcesToUse getPlayerInput() const;
    bool processRound(int landToBuy, int landToSell, int grainToPlant, int grainToEat);
    void getFinalEvaluation() const;

    void saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);

    int getRound();

private:

    struct Resources
    {
        int people;
        int land;
        int grain;
    };

    int round;
    Resources wasteResources;
    Resources incomeResources;
    Resources currentResources;
    int landPrice;
    int grainPerAcre;
    bool facedPlague;

    int totalStarved;
    std::mt19937 gen;

    int getRandom(int min, int max);

};

GameState::GameState():
    round(1),
    facedPlague(false),
    grainPerAcre(0),
    landPrice(0),
    totalStarved(0)
{
    currentResources.people = 100;
    currentResources.land = 1000;
    currentResources.grain = 2800;

    wasteResources.people = 0;
    wasteResources.grain = 0;
    wasteResources.land = 0;

    incomeResources.people = 0;
    incomeResources.grain = 0;
    incomeResources.land = 0;

    std::uniform_int_distribution<> dis(17, 26);
    landPrice = dis(gen);
}

void GameState::getReport() const
{
    std::cout << "Мой повелитель, соизволь поведать тебе \n";
    std::cout << "В " << round << " году твоего высочайшего правления\n";

    if ( wasteResources.people > 0 )
    {
        std::cout << wasteResources.people << " человек умерли с голоду";
        if ( incomeResources.people > 0 )
            std::cout << ", и " << incomeResources.people << " человек прибыли в наш великий город.\n";
        else
            std::cout << ".\n";
    }

    if ( facedPlague )
    {
        std::cout << "Чума уничтожила половину населения.\n";
    }

    std::cout << "Население города составляет " << currentResources.people << " человек.\n";

    if ( incomeResources.grain > 0 )
    {
        std::cout << "Мы собрали " << incomeResources.grain << " бушелей пшеницы, по ";
        std::cout << grainPerAcre << " бушелей с акра.\n";
    }
    if ( wasteResources.grain > 0 )
    {
        std::cout << "Крысы истребили " << wasteResources.grain << " бушелей пшеницы.\n";
    }

    std::cout << "В амбарах осталось " << currentResources.grain << " бушелей пшеницы.\n";
    std::cout << "Город сейчас занимает " << currentResources.land << " акров.\n";
    std::cout << "1 акр земли стоит сейчас " << landPrice << " бушелей пшеницы.\n\n";
}

GameState::ResourcesToUse GameState::getPlayerInput() const
{
    GameState::ResourcesToUse resourcesToUse = {0, 0, 0, 0};
    std::cout << "Что пожелаешь, повелитель?\n";

    std::cout << "Сколько акров земли повелеваешь купить? ";
    std::cin >> resourcesToUse.landToBuy;

    while ( landPrice * resourcesToUse.landToBuy > currentResources.grain )
    {
        std::cout << "О, повелитель, пощади нас! У нас нет столько пшеницы, чтобы купить " << resourcesToUse.landToBuy << " акров!\n";
        std::cin >> resourcesToUse.landToBuy;
    }

    std::cout << "Сколько акров земли повелеваешь продать? ";
    std::cin >> resourcesToUse.landToSell;

    while (currentResources.land - resourcesToUse.landToSell < 0)
    {
        std::cout << "О, повелитель, пощади нас! У нас нет столько земли, чтобы продать " << resourcesToUse.landToSell << " акров!\n";
        std::cin >> resourcesToUse.landToSell;
    }
    int landAfterSell = currentResources.land - resourcesToUse.landToSell;

    std::cout << "Сколько бушелей пшеницы повелеваешь съесть? ";
    std::cin >> resourcesToUse.grainToEat;

    while (currentResources.grain - resourcesToUse.grainToEat < 0)
    {
        std::cout << "О, повелитель, пощади нас! У нас нет столько пшеницы, чтобы съесть " << resourcesToUse.grainToEat << " бушелей!\n";
        std::cin >> resourcesToUse.grainToEat;
    }
    int grainAfterFood = currentResources.grain - resourcesToUse.grainToEat;

    std::cout << "Сколько акров земли повелеваешь засеять? ";
    int landToPlant;
    std::cin >> landToPlant;
    resourcesToUse.grainToPlant = landToPlant * 5;

    while ( landToPlant - landAfterSell > 0)
    {
        std::cout << "О, повелитель, пощади нас! У нас нет столько земли, чтобы засеять " << landToPlant << " акров!\n";
        std::cin >> landToPlant;
    }

    while ( resourcesToUse.grainToPlant > grainAfterFood)
    {
        std::cout << "О, повелитель, пощади нас! У нас нет столько пшеницы, чтобы засеять " << landToPlant << " акров!\n";
        std::cin >> landToPlant;
        resourcesToUse.grainToPlant = landToPlant * 5;
    }

    while ( landToPlant * 10 > currentResources.people)
    {
        std::cout << "О, повелитель, пощади нас! У нас нет столько людей, чтобы засеять " << landToPlant << " акров!\n";
        std::cin >> landToPlant;
    }

    return resourcesToUse;

}

bool GameState::processRound(int landToBuy, int landToSell, int grainToPlant, int grainToEat)
{

    currentResources.grain += landToSell * landPrice;
    currentResources.grain -= landToBuy * landPrice;

    currentResources.land -= landToSell;
    currentResources.land += landToBuy;

    currentResources.grain -= grainToEat;
    currentResources.grain -= grainToPlant;

    int foodNeeded = currentResources.people * 20;
    int starvedThisRound = (grainToEat >= foodNeeded) ? 0 : currentResources.people - (grainToEat / 20);
    wasteResources.people = starvedThisRound;
    if (currentResources.people > 0 && starvedThisRound > 0) {
        double starvationPercent = (static_cast<double>(starvedThisRound) / (currentResources.people + starvedThisRound)) * 100.0;
        if (starvationPercent > 45.0) {
            std::cout << "Более 45% населения умерло от голода! Правитель Египта, ваше правление окончено!\n";
            return false;
        }
    }
    totalStarved += starvedThisRound;

    currentResources.people -= starvedThisRound;
    if (currentResources.people <= 0) {
        std::cout << "Всё население города вымерло! Ваше правление окончено!\n";
        return false; // Игра окончена
    }

    int currentGrainPerAcre = getRandom(1, 6);

    int landPlantedThisRound = grainToPlant / 5;
    int grainHarvestedThisRound = landPlantedThisRound * currentGrainPerAcre;
    currentResources.grain += grainHarvestedThisRound;

    int ratsEatenThisRound = getRandom(0, static_cast<int>(currentResources.grain * 0.07));
    currentResources.grain -= ratsEatenThisRound;
    wasteResources.grain = ratsEatenThisRound;

    bool plagueThisRound = (getRandom(1, 100) <= 15);
    if (plagueThisRound) {
        currentResources.people /= 2;
    }
    facedPlague = plagueThisRound;

    int immigrationCalc = (starvedThisRound / 2) + (5 - currentGrainPerAcre) * (currentResources.grain + ratsEatenThisRound) / 600 + 1;
    int immigrantsThisRound = std::max(0, std::min(50, immigrationCalc));
    currentResources.people += immigrantsThisRound;
    incomeResources.people = immigrantsThisRound;

    grainPerAcre = currentGrainPerAcre;
    incomeResources.grain = grainHarvestedThisRound;

    landPrice = getRandom(17, 26);
    round++;

    if (round > 10) {
        return false;
    }
    return true;
}

void GameState::getFinalEvaluation() const
{
    if (round <= 10) {
        return;
    }

    double avgStarvedPercentage = (static_cast<double>(totalStarved) / 10.0);
    int landPerPerson = (currentResources.people > 0) ? currentResources.land / currentResources.people : 0;

    std::cout << "\n--- Финальная оценка ---\n";
    std::cout << "Среднегодовой процент умерших от голода P: " << avgStarvedPercentage << "%\n";
    std::cout << "Количество акров земли на одного жителя L: " << landPerPerson << "\n";

    if (avgStarvedPercentage > 33.0 && landPerPerson < 7) {
        std::cout << "Из-за вашей некомпетентности в управлении, народ устроил бунт, и изгнал вас их города. Теперь вы вынуждены влачить жалкое существование в изгнании.\n";
    } else if (avgStarvedPercentage > 10.0 && landPerPerson < 9) {
        std::cout << "Вы правили железной рукой, подобно Нерону и Ивану Грозному. Народ вздохнул с облегчением, и никто больше не желает видеть вас правителем.\n";
    } else if (avgStarvedPercentage > 3.0 && landPerPerson < 10) {
        std::cout << "Вы справились вполне неплохо, у вас, конечно, есть недоброжелатели, но многие хотели бы увидеть вас во главе города снова.\n";
    } else {
        std::cout << "Фантастика! Карл Великий, Дизраэли и Джефферсон вместе не справились бы лучше.\n";
    }
}

void GameState::saveToFile(const std::string& filename) const
{
    std::ofstream outFile(filename, std::ios::binary);
    if (outFile.is_open())
    {

        outFile.write(reinterpret_cast<const char*>(&round), sizeof(round));
        outFile.write(reinterpret_cast<const char*>(&wasteResources), sizeof(wasteResources));
        outFile.write(reinterpret_cast<const char*>(&incomeResources), sizeof(incomeResources));
        outFile.write(reinterpret_cast<const char*>(&currentResources), sizeof(currentResources));
        outFile.write(reinterpret_cast<const char*>(&landPrice), sizeof(landPrice));
        outFile.write(reinterpret_cast<const char*>(&grainPerAcre), sizeof(grainPerAcre));
        outFile.write(reinterpret_cast<const char*>(&facedPlague), sizeof(facedPlague));
        outFile.write(reinterpret_cast<const char*>(&totalStarved), sizeof(totalStarved));

        outFile.close();
        std::cout << "Игра сохранена в " << filename << std::endl;
    }
    else
    {
        std::cerr << "Ошибка: не удалось открыть файл " << filename << " для записи.\n";
    }
}

bool GameState::loadFromFile(const std::string& filename)
{
    std::ifstream inFile(filename, std::ios::binary);
    if (inFile.is_open())
    {
        inFile.read(reinterpret_cast<char*>(&round), sizeof(round));
        inFile.read(reinterpret_cast<char*>(&wasteResources), sizeof(wasteResources));
        inFile.read(reinterpret_cast<char*>(&incomeResources), sizeof(incomeResources));
        inFile.read(reinterpret_cast<char*>(&currentResources), sizeof(currentResources));
        inFile.read(reinterpret_cast<char*>(&landPrice), sizeof(landPrice));
        inFile.read(reinterpret_cast<char*>(&grainPerAcre), sizeof(grainPerAcre));
        inFile.read(reinterpret_cast<char*>(&facedPlague), sizeof(facedPlague));
        inFile.read(reinterpret_cast<char*>(&totalStarved), sizeof(totalStarved));

        inFile.close();
        return true;
    }
    else
    {
        return false;
    }
}

inline int GameState::getRound()
{
    return round;
}


int GameState::getRandom(int min, int max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}